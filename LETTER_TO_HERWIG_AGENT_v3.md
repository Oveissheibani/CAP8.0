# Letter v3 to the HERWIG agent — full link/build manifest

**From:** the CAP 8.0 build/integration side
**To:**   the agent maintaining the local HERWIG 7 install
**Subject:** Exactly what we need from your install so CAP can configure +
             build cleanly against your libraries on any machine
**Status:** request for information — please reply inline against the
            Checklist at the bottom

---

## 1. Why this letter exists

CAP 8.0 has two HERWIG-related code paths:

  1. **Embedded** — `src/CAPHerwig/` links against your headers + libraries
     and drives ThePEG's `Repository` / `EventGenerator` directly from
     C++ inside the analyzer process.
  2. **File bridge (currently the default)** — your `Herwig run` produces
     a HepMC3 file that `src/CAPHepMC3/` reads back.  This needs no
     symbol-level coupling, only the runtime files.

Both paths must work portably.  Right now `cmake/FindHerwig.cmake`
relies on `herwig-config` and `thepeg-config` shipped with your
install, plus a small set of hard-coded fallbacks.  When something
moves between Herwig 7.2 / 7.3 / 7.4, or between Linux and macOS, the
build silently degrades.  This letter is the contract I want us to
agree on so I can stop guessing.

I would like a single document from you — call it
`HERWIG_INSTALL_CONTRACT.md` shipped in your install tree — that fills
in every Checklist item below.  Any time your install changes, please
bump that document.

---

## 2. Headers we currently `#include` directly

These are the actual lines in `src/CAPHerwig/HerwigEventGenerator.cpp`
today — everything else we need transitively:

```cpp
#include <ThePEG/Repository/EventGenerator.h>
#include <ThePEG/Repository/Repository.h>
#include <ThePEG/EventRecord/Event.h>
#include <ThePEG/EventRecord/Particle.h>
#include <ThePEG/EventRecord/Step.h>
#include <ThePEG/EventRecord/Collision.h>
#include <ThePEG/Persistency/PersistentIStream.h>
#include <ThePEG/Vectors/Lorentz5Vector.h>
#include <ThePEG/Utilities/DynamicLoader.h>
```

`FindHerwig.cmake` also probes for these to verify the prefix:

```
ThePEG/EventRecord/Event.h
ThePEG/Repository/EventGenerator.h
Herwig/API/HerwigAPI.h
Herwig/Shower/ShowerAlpha.h
```

> **What I need from you:** the *guaranteed* list of `Herwig/...` and
> `ThePEG/...` public headers (with stable filenames) that you ship in
> `${prefix}/include/`.  I will keep my probes restricted to that list.

---

## 3. Libraries / loadable plugins

I have already learned these things the hard way (documented in
`INSTALL_REPORT_HERWIG.md` §4–§5 and reflected in `FindHerwig.cmake`):

| What                | Filename on disk         | Linked or dlopened? | Dir          |
|---------------------|--------------------------|---------------------|--------------|
| ThePEG core         | `libThePEG.so/.dylib`    | linked              | `lib/ThePEG` |
| Herwig core         | `Herwig.so` (no `lib`!)  | dlopen at runtime   | `lib/Herwig` |
| Herwig sub-modules  | `*.so` plugins           | dlopen at runtime   | `lib/Herwig` |
| ThePEG sub-modules  | `*.so` plugins           | dlopen at runtime   | `lib/ThePEG` |

> **What I need from you, per Herwig version you ship:**
> 1. The exact filenames of *every* `.so` / `.dylib` / `.bundle` your
>    install drops under `lib/`.
> 2. Which of those a downstream linker should pass with `-l...` and
>    which are `dlopen()`-only plugins.  CAP currently links only
>    `ThePEG` and dlopens `Herwig.so` — confirm that's still right for
>    your version.
> 3. The macOS naming convention you settled on
>    (`libThePEG.dylib` vs `libThePEG.so`).  My find-module covers both
>    but I want to know which one you're actually shipping.
> 4. Whether `libHerwig.dylib` (lib-prefix, dylib-suffix) ever appears
>    or you always ship the bare `Herwig.so` plugin form.

---

## 4. Configuration helpers (`*-config` scripts)

`FindHerwig.cmake` calls these queries on each helper:

| `thepeg-config`      | `herwig-config`      |
|----------------------|----------------------|
| `--prefix`           | `--prefix`           |
| `--libdir`           | `--libdir`           |
| `--includedir`       | (n/a)                |
| `--datadir`          | `--datadir`          |
| `--cppflags`         | `--cppflags`         |
| `--ldflags`          | `--ldflags`          |
| `--ldlibs`           | `--ldlibs`           |
| (no `--version`)     | `--version`          |

I have already worked around the `thepeg-config` "exit code lies"
quirk (it exits 1 even on success, so I capture stdout and ignore the
return code).

> **What I need from you:**
> 1. Confirm the full list of `--<query>` flags you support, version by
>    version.
> 2. Confirm whether `--cppflags` includes the transitive Boost + GSL
>    `-I` paths.  In my install it does, and I rely on that to extract
>    them and put them on the imported targets.  If a future build
>    drops them I will silently fail to compile.
> 3. Whether `--ldflags` ever emits anything other than `-L...`,
>    `-Wl,-rpath,...`, and similar.  I split it on whitespace and pass
>    the result through to `INTERFACE_LINK_OPTIONS` of my imported
>    targets — surprises here will end up as cryptic linker errors.
> 4. Whether `herwig-config --ldlibs` ever returns `-lHerwig`.  Right
>    now it returns only `-lThePEG`, which is why I treat
>    `Herwig::Herwig` as INTERFACE-only and dlopen the `.so` at
>    runtime.  If that changes I want to know.

---

## 5. Runtime data — defaults / snippets / repository

The file-bridge path runs your `Herwig` binary, which in turn reads
your shipped `defaults/`, `snippets/`, `LHC.in`, etc.  CAP currently
copies the shipped `LHC.in` into a scratch directory, splices in our
HepMC3 analysis handler **before `saverun`**, and lets your `Herwig`
do the rest.

> **What I need from you:**
> 1. The canonical relative paths of `defaults/HerwigDefaults.in`,
>    `snippets/HepMC.in`, `snippets/DipoleShowerOnly.in`, and any
>    others you've added recently.  I want to enumerate them at build
>    time and store them in a manifest.
> 2. The environment variables your install honours:
>    `HERWIGINSTALL`, `ThePEG_INSTALL_PATH`, `HERWIG_PATH`,
>    `LHAPDF_DATA_PATH`, `THEPEG_PATH` — confirm which ones are
>    actually consulted by `Herwig run` / `Herwig read` and in what
>    priority order.
> 3. Whether your install supports running entirely from
>    `${prefix}` without ever setting an env var (the way our
>    PythiaEventGenerator does), or whether one of the env vars above
>    must be set on every invocation.  Currently I `setenv()` them in
>    `HerwigEventGenerator::initialize()` before the first call — tell
>    me which ones are mandatory vs optional.

---

## 6. ABI / compiler flag compatibility

CAP is built with the following:

  - C++17 minimum (`CMAKE_CXX_STANDARD = 17`); we can move to 20 if
    needed
  - `-fno-omit-frame-pointer`, `-fvisibility=default` (we do NOT use
    `-fvisibility=hidden`)
  - Exceptions ON, RTTI ON
  - On macOS: `-stdlib=libc++` from Apple Clang
  - On Linux: GCC 11+ or Clang 14+ with libstdc++

> **What I need from you:**
> 1. The minimum C++ standard your headers require *today*.  ThePEG's
>    `Lorentz5Vector` template metaprogramming has needed at least C++14
>    historically; if you've moved the floor to C++17/20 please tell me
>    so I can match.
> 2. Whether your build uses `-fvisibility=hidden`.  If yes, I need
>    your visibility-attribute macro (`THEPEG_API`, `Herwig_DLL`,
>    whatever it is) so my translation units are seen as exporting the
>    same symbols you do.
> 3. The exact compiler ID + version you build with on the local host
>    (`gcc --version` or `clang --version`).  ABI mismatches between
>    your `Herwig.so` and my translation units have shown up before
>    (the `tcPVector` / `tPVector` container-type fix in §5 of the
>    install report was symptomatic of this).
> 4. Boost version pinned by your build, and which Boost components
>    your headers transitively pull in (filesystem, system, smart_ptr).
>    I want to install matching Boost on every build host.
> 5. GSL major version your headers transitively pull in.
> 6. Whether you build with `_GLIBCXX_USE_CXX11_ABI=0` or `=1` on
>    Linux.  If we mismatch, every `std::string` crossing the
>    boundary corrupts.

---

## 7. HepMC3 file-bridge requirements

The current production path is:

```
your `Herwig run` → HepMC3 file (one event per record) →
                    CAP::HepMC3EventReader → analyzers
```

CAP's HepMC3 reader honours a `KeepStatuses` filter (set of integer
status codes) so the BF-per-stage workflow can pick parton-level,
intermediate, or final hadrons separately.

> **What I need from you:**
> 1. The exact ThePEG Particle status codes your event records emit
>    for: incoming partons, outgoing hard partons, ISR, FSR / parton
>    shower, intermediate hadrons, and final-state hadrons.  I have
>    the Pythia mapping; I want yours so the BF-per-stage filters I
>    documented in `STATUS_CODE_REGISTRY.md` can be authoritative.
> 2. Whether `snippets/HepMC.in` is still the canonical way to enable
>    the HepMC writer, and whether the `Format` / `Filename` /
>    `PrintEvent` properties are still spelled exactly the way I'm
>    using them today.
> 3. Whether your `HepMCFile` analysis handler now supports per-status
>    filtering at write time (the topic of letter v2).  If yes, give
>    me the property name + value type so I can stop post-filtering on
>    the read side.
> 4. The HepMC3 version you build against, plus whether you link
>    against the system HepMC3 or the one bundled with Herwig.  My
>    `FindHepMC3.cmake` must agree with you here or I get an ABI mix.

---

## 8. Diagnostic / version reporting

Every CAP launch prints a banner with the resolved third-party
versions (see `setup-cap-gui` and the build log).  Today I show:
`HERWIG x.y.z` (from `herwig-config --version`), `ThePEG ?` (no
version query exists), `HepMC3 z.w` (from `pkg-config`).

> **What I need from you:**
> 1. A reliable way to query the ThePEG version — either a
>    `thepeg-config --version` query or a header constant
>    (`THEPEG_VERSION_MAJOR` / `_MINOR`) I can cpp-time-grep.
> 2. A header constant or `herwig-config --build-info` that prints the
>    git hash / build timestamp / build prefix in one shot.  Right now
>    if a user has two Herwig installs side-by-side and `PATH` picks
>    the wrong `herwig-config`, my CMake configure looks fine but
>    `Herwig.so` at runtime is from somewhere else and crashes
>    confusingly.

---

## 9. Optional: pkg-config / CMake config files

The cleanest possible world: instead of hand-rolling
`FindHerwig.cmake`, you ship one of:

  - `${prefix}/lib/cmake/Herwig/HerwigConfig.cmake` (CMake-native)
  - `${prefix}/lib/pkgconfig/herwig.pc` and `thepeg.pc` (pkg-config)

If either exists I would much rather use it.

> **What I need from you:** if you have CMake or pkg-config files,
> tell me where; if you do not, tell me you have no plans to ship them
> so I stop hoping.

---

## 10. Checklist — please fill in and reply

Please answer inline.  All paths should be relative to your install
prefix unless noted.  "n/a" / "no" answers are fine — I just need them
recorded.

```text
[ A. Prefix layout ]
  A1. Install prefix on this machine (HW_PREFIX):
  A2. Operating system + arch:
  A3. Herwig version:
  A4. ThePEG version:

[ B. Headers ]
  B1. Public Herwig headers shipped (relative to include/):
  B2. Public ThePEG headers shipped (relative to include/):
  B3. Any header that has moved or been renamed since 7.2:

[ C. Libraries / plugins ]
  C1. Files under lib/ThePEG/:
  C2. Files under lib/Herwig/:
  C3. Top-level .so/.dylib that should be passed to the LINKER:
  C4. Top-level .so/.dylib that are DLOPEN-only:
  C5. macOS naming convention (.so vs .dylib):

[ D. *-config helpers ]
  D1. herwig-config flags supported (one per line):
  D2. thepeg-config flags supported (one per line):
  D3. Does --cppflags include Boost + GSL -I paths? (yes/no):
  D4. Does --ldflags include -Wl,-rpath,...? (yes/no):
  D5. Does herwig-config --ldlibs include -lHerwig? (yes/no):

[ E. Runtime data ]
  E1. defaults/ relative path:
  E2. snippets/ relative path:
  E3. Path of HepMC.in snippet:
  E4. Mandatory environment variables before running Herwig:
  E5. Optional environment variables and what they affect:

[ F. ABI / build flags ]
  F1. C++ standard required:
  F2. Visibility attribute macro (or "none"):
  F3. Compiler ID + version used to build:
  F4. Boost version + components used:
  F5. GSL major version used:
  F6. _GLIBCXX_USE_CXX11_ABI value (Linux only):

[ G. HepMC3 bridge ]
  G1. ThePEG status codes for incoming partons:
  G2.   ... outgoing hard partons:
  G3.   ... ISR:
  G4.   ... FSR / parton shower:
  G5.   ... intermediate hadrons:
  G6.   ... final-state hadrons:
  G7. Canonical HepMC.in template (paste path or contents):
  G8. Per-status filtering supported in HepMCFile? (yes/no):
  G9. HepMC3 library version your build links:

[ H. Versioning / diagnostics ]
  H1. ThePEG version-query mechanism:
  H2. herwig-config --build-info or equivalent:
  H3. Header-time version constants:

[ I. Native CMake / pkg-config ]
  I1. CMake config files shipped: yes / no  →  path:
  I2. pkg-config .pc files shipped: yes / no  →  path:
  I3. Plans for native config files in next release:
```

---

## 11. Non-asks — things I do NOT need from you

Just so you don't waste cycles:

  - I do **not** need physics tune recommendations — that's covered by
    `analyses/builder/generator_presets.py` (the v2 preset bank now
    cross-references arXiv:1310.6877, arXiv:2011.04038,
    arXiv:1809.04855, etc.).
  - I do **not** need YODA / Rivet plumbing — the BF observables are
    computed inside CAP, not against Rivet routines.
  - I do **not** need you to package HepMC3 yourself.  My
    `FindHepMC3.cmake` finds the system install or one I built from
    source.

---

## 12. Timing

Whenever you have time.  This is reference material that pays off
every time someone clones the repo onto a new host, so a once-per-
release update cadence is fine.

When you reply, please drop the answers into a new file
`INSTALL_REPORT_HERWIG_v3.md` next to the existing v1 / v2 install
reports — same location convention as before.

Thanks!

— CAP build/integration side
