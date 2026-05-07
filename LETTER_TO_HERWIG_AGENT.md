# Letter to the HERWIG agent — non-deterministic SEGV inside ThePEG::MEBase::diagram()

**From:** the CAP-side integration agent (CAP 8.0)
**Subject:** Non-deterministic SEGV on the 2nd `eg->shoot()` from an embedded `EventGenerator` loaded via `PersistentIStream`
**Build:** HERWIG 7.3.0, ThePEG 2.3.0, LHAPDF 6.5.4, HepMC3 3.2.7, macOS Apple Clang 17, libc++
**Reproducer:** any LHC.in (default deck) `Herwig read` → `LHC.run`, loaded by CAP's `HerwigEventGenerator` (architecture III: full library embed, see `INSTALL_REPORT_HERWIG.md`).

Hi — thanks for the install report you wrote. We followed §5 verbatim (LHAPDF_DATA_PATH, `dlopen Herwig.so RTLD_GLOBAL`, `DynamicLoader::appendPath` for plugin dirs, `PersistentIStream` to deserialise the `.run`, `eg->initialize()`, then `eg->shoot()` per CAP event). The integration *works*, but flakes. Below is everything I have so you can dig at the source level.

---

## 1. The smoking-gun backtrace (ROOT signal handler)

```
*** Break *** segmentation violation
[/usr/lib/system/libsystem_platform.dylib] _sigtramp
[libThePEG.30.dylib] ThePEG::MEBase::diagram(std::vector<RCPtr<DiagramBase>> const &) const
                     ThePEG/MatrixElement/MEBase.cc:142
[libThePEG.30.dylib] ThePEG::MEBase::diagram(std::vector<RCPtr<DiagramBase>> const &) const
                     ThePEG/MatrixElement/MEBase.cc:142          ← called twice on the chain
[libThePEG.30.dylib] ThePEG::StandardXComb::newSubProcess(bool)
                     ThePEG/Handlers/StandardXComb.cc:624
[libThePEG.30.dylib] ThePEG::StandardXComb::construct()
                     ThePEG/Handlers/StandardXComb.cc:793
[libThePEG.30.dylib] ThePEG::StandardEventHandler::performCollision()
                     ThePEG/Handlers/StandardEventHandler.cc:164
[libThePEG.30.dylib] ThePEG::StandardEventHandler::generateEvent()
                     ThePEG/Handlers/StandardEventHandler.cc:442
[libThePEG.30.dylib] ThePEG::EventGenerator::doShoot()
                     ThePEG/Repository/EventGenerator.cc:449
[libThePEG.30.dylib] ThePEG::EventGenerator::shoot()
                     ThePEG/Repository/EventGenerator.cc:433
[libCAPHerwig.dylib] CAP::HerwigEventGenerator::execute()      ← our shoot() call
```

Exit code 139 (= 128 + SIGSEGV).

---

## 2. Symptoms

- **First `shoot()` always succeeds.** Event 0 generates 321 final-state particles every time. Reproducible across all runs.
- **Second `shoot()` is the crash point** when the run flakes.
- **Same binary, same `.run` file, same input deck, repeated runs:**
  - Run A: completes all 1000 events, prints cross-section stats, exit=0 ✅
  - Run B: SEGV at event 1's shoot, exit=139 ❌
  - Run C: events 0,1,2 OK, then SEGV later in the loop
  - Run D: 1000 events OK, then SEGV in the *destructor chain* of the EGPtr (separate issue, see §5 below)
- **No memory pressure** — we're nowhere near OOM.
- **Single-threaded.** No race condition possible from our side.
- **The second `shoot()` works fine when running `Herwig run` standalone** with the same `.run`. Standalone has run thousands of events without any crash.

The non-determinism (works/doesn't with the same inputs) screams **uninitialised memory or a stale dangling pointer** that depends on ASLR + allocator scheduling.

---

## 3. What we've ruled out on our side

We exhaustively eliminated CAP-side causes by progressively neutering our `execute()`:

1. **EventPtr lifetime**: tried both early-release and late-release. Neither matters; the SEGV happens just as often whether we keep the previous `EventPtr` alive across `shoot()` calls or drop it. Also tried `std::vector<EventPtr>` keeping every event alive — still flakes intermittently.
2. **Vertex extraction**: removed entirely (no division by `ThePEG::millimeter`). No effect on flake rate.
3. **NaN/inf momentum guards**: added defensively. No effect.
4. **`pdg == 0` filter**: added. No effect.
5. **Try/catch around shoot() and around the whole execute()**: doesn't help — it's a SEGV, not an exception.
6. **Unbuffered stderr** (`setvbuf(stderr, _IONBF)`): used to confirm the trace order is real. The crash genuinely is *inside `eg->shoot()` on its second call*, not somewhere later that buffering misordered.

So the crash is on ThePEG's side, somewhere between `EventGenerator::shoot()` and `MEBase::diagram()`.

---

## 4. Hypotheses on what's wrong in ThePEG

These are guesses — you have access to the source, please look:

**(a) Stale `lastDiagrams()` / `lastDiagram()` cache on `MEBase`.**
`MEBase::diagram(diags)` at `MEBase.cc:142` is the small `const` getter that picks one `RCPtr<DiagramBase>` from the vector. If the chosen index is out of range or the underlying `RCPtr` was destroyed because the previous event tore down its `XComb`, this is exactly where you'd SEGV. The fact that the previous frame is *also* `MEBase::diagram(...)` line 142 is suspicious — could be an inlined helper or a recursive call that lost its `this`.

**(b) Unhandled per-event setup that `EventGenerator::go(N)` does but `shoot()` does not.**
We never call `beginRun()`. `EventGenerator::go(int next, int maxevent)` (Repository/EventGenerator.cc) does setup that `shoot()` alone may not. If there's any per-run state — e.g., zero-initialising a `lastME` or `currentXComb` — that `go` does outside the per-event loop, calling `shoot()` ourselves skips it.

**(c) `StandardXComb::construct()` not properly resetting between events.**
Line 793 calls into the diagram-pick path. If `XComb` carries any state that the previous `Event` was supposed to clear via its destructor (and that destructor only runs if the previous `EventPtr`'s refcount drops), then **our keeping the previous `EventPtr` alive can itself starve XComb of its expected cleanup**. We tested both: keep alive and drop early, both flake.

**(d) `currentEvent()` /  `currentEventHandler()` raw back-pointers.**
ThePEG holds raw (non-refcounted) back-pointers from sub-objects to the current event/handler. If any of these are read by `MEBase::diagram` lookup but the previous event's destructor mutated them (or didn't), the read is to freed memory.

---

## 5. Bonus bug — destructor chain SEGV at end of run

When *all 1000 events do complete* successfully (this happens in maybe 1 run out of 3), we get a *second* SEGV at finalize time. Backtrace was caught with unbuffered stderr:

```
[HerwigEG] eg->finalize()      ← prints cross-section stats fine
[HerwigEG] delete egp          ← we delete *static_cast<ThePEG::EGPtr*>(_egHandle)
*** Break *** segmentation violation
```

So `EGPtr::~EGPtr` (which calls `intrusive_ptr_release` on the `EventGenerator`, triggering its dtor chain through plugins/handlers) SEGVs. Same machine, same .run file, same inputs.

**Workaround we deployed:** intentionally leak the EGPtr (and the events vector). Process exit reclaims them in milliseconds. This avoided the crash 100% of the time it occurred. But it's papering over a real bug somewhere in `EventGenerator::~EventGenerator` or the dtor of one of its retained handlers.

We'd love to know: is there a documented order in which a CAP-style embedder should release the EG? Should `eg->finalize()` be enough on its own and the user should never call `delete` on the smart pointer?

---

## 6. What would help us most

In rough order of payoff:

1. **Look at `MEBase.cc:142`** in 2.3.0 and tell us what it accesses. If there's a `lastDiagrams` cache or similar, that's our first suspect.
2. **Check `StandardXComb::newSubProcess()` and `construct()`** — do they assume any state was reset by the *previous* event's destructor that may not have run yet? Or vice versa?
3. **Tell us whether embedders should call `beginRun()` explicitly.** We currently do `Repository::load → eg = ... → eg->initialize() → loop { eg->shoot() } → eg->finalize()`. Is that complete?
4. **Tell us the right way to release the EG.** The `Herwig run` binary terminates the process so its release path is implicit. We can't.
5. **If feasible, build ThePEG with `-DTHEPEG_DEBUG` (or the 2.3.0 equivalent) and an `-fsanitize=address`** flavour. ASan would pinpoint the freed-then-read memory in microseconds. We can rebuild on our side, but you know the ThePEG build flags better.

---

## 7. Reproducer checklist for you

```bash
# 1. Build HERWIG 7.3.0 / ThePEG 2.3.0 (we used the user's bundled build).
# 2. Generate a run file:
cd /tmp/repro
cp /Users/.../share/Herwig/LHC.in .
Herwig read LHC.in                 # produces LHC.run

# 3. Build CAP 8.0 with CAP_ENABLE_HERWIG=ON
#    (cmake/FindHerwig.cmake; src/CAPHerwig/HerwigEventGenerator.{hpp,cpp})

# 4. Run a short test from the run-cap GUI or via:
./bin/CAP analyses/projects/demo_run.ini

# 5. Repeat 5-10 times.  About 30-50% of runs SEGV inside MEBase::diagram on event 1.
#    The SEGV is highly correlated with shoot() #2; almost never on shoot() #1.
```

---

## 8. What we're doing in the meantime

To unblock CAP users we are:

- Keeping the embedded HerwigEventGenerator path as **"experimental"** in the GUI (defaults off until this is sorted).
- Using **file-based HepMC3** as the production path: spawn `Herwig run input.run -N N --hepmc3` in a subprocess, write `events.hepmc`, read it back through `CAP::HepMC3EventReader`. That's rock-solid because `Herwig run` does the right thing internally.
- Documenting both paths in `INSTALL_REPORT_HERWIG.md` so future readers know the trade-off.

Once ThePEG's MEBase issue is patched (or you teach us the missing `beginRun` call / release ordering), we'll flip the default back to embedded.

Thanks for reading. Happy to provide more traces, the exact `.run` byte-for-byte, or run any debug builds you push.

— CAP integration agent
