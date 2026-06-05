# COORD — multi-agent coordination for the parton-tracking feature

Two Claude sessions are sharing this folder.  Each owns a slice of the
codebase; touch only your own slice unless you coordinate via this file.

Last updated: 2026-05-27

## Session A — C++ engine + GUI + runner

**Owns:**
- `src/EventHistory/*.cpp / *.hpp / *.inc`
- `src/EventHistory/provenance-study.cpp`
- `src/EventHistory/provenance-report.cpp` (structural code only — Session B
  may edit the `pairedGroups()` table and figure interpretations)
- `cap-mechanism-ladder`
- `build-A/` build directory (if used)
- (GUI reassigned to Session B on 2026-05-29 by the user — see B's owns.)
- (provenance-study.cpp `--hepmc3` Herwig input path reassigned to B on
  2026-05-29 by the user — B added the HepMC3 event source; rest of the file
  remains A's.  A: heads up, untested by B — needs a HepMC3-enabled build.)

**Working on right now:**
- Verifying the resonance-resolver fix (task #59 in this session's tracker).
- Multi-species `--species 211,321,2212` engine just landed (task #57);
  per-species + per-species-pair histograms are emitted with `_S<pdg>` /
  `_S<a>x<b>` suffixes.

**Not touching:**
- `cap-provenance-plot` (Session B's domain).
- `src/EventHistory/provenance-report-captions.inc` figure-caption strings
  (Session B may edit).
- `src/EventHistory/provenance-report.cpp` `pairedGroups()` and report
  section bodies (Session B may rearrange to surface multi-species).

## Session B — plotter + report figure layout

**Owns:**
- `cap-provenance-plot` (Python)
- `src/EventHistory/provenance-report-captions.inc`
- `src/EventHistory/provenance-report.cpp` figure layout / pairedGroups
  table / per-species report sections
- `cap-provenance-gui` (reassigned from A, 2026-05-29, user request)
- `gui/provenance_gui/**/*.py` (reassigned from A, 2026-05-29)
- `build-B/` build directory (if used)

**Primary task (handoff brief lives below in this file):**
Multi-species auto-discovery — when `provenance-study --species` is a
comma-list, the plotter and the report should automatically generate
per-species and per-species-pair figures.  Detect by scanning the .root
file or reading `species_list=` from the .root.txt summary.

**Not touching:**
- Any file in `src/EventHistory/` except the two named above.
- `provenance-study.cpp`, `cap-mechanism-ladder`.

## Build state isolation (important)

- Session A:  `cd build-A && cmake .. && cmake --build . -j4`
- Session B:  `cd build-B && cmake .. && cmake --build . -j4`

If you only have `build/`, take turns building.  Concurrent `cmake --build`
on the same directory will produce linker errors.

## Output directory isolation

- Session A runs land in `provenance/runs/` and `provenance/ladder/`.
- Session B test runs go in `provenance-b/runs/` (use `--out` and
  `--outdir` to redirect).

## Status pings

Each session leaves one-line notes here when it starts / finishes a unit
of work.  Don't be precious about formatting.

- [A] 2026-05-27  resolver fix landed; ready for the user to verify
  `FromResonance ~58 %` on a fresh smoke run.
- [B] 2026-05-29  starting multi-species auto-discovery: plotter
  (`cap-provenance-plot`) + report figure layout
  (`provenance-report.cpp` section bodies / `pairedGroups()` /
  captions.inc).  Using build-B/ and provenance-b/ for tests.
- [B] 2026-05-29  done editing.  Plotter: `discover_species()` (reads
  `species_list=`, falls back to ROOT-key scan) + `emit_single_run()`
  emits per-species (`_S<pdg>`) single-particle and per-species-pair
  (`_S<a>x<b>`, N(N+1)/2) figures; single-species path byte-identical.
  Report: parses `species_list=`; `renderFiguresPaired()` now takes a
  `std::function` predicate + optional suffix; per-species /
  per-species-pair subsections under the existing single/two-particle
  sections; config table lists the full species set.  `pairedGroups()`
  and captions.inc UNCHANGED (suffix-aware lookup made edits there
  unnecessary).  buildPaper() structure untouched apart from those two
  section bodies.  Logic verified via standalone harnesses; the cmake
  build-B + provenance-study/-plot/-report run still needs to happen on
  a host with ROOT+Pythia (not available in my sandbox).
- [B] 2026-05-29  user test was INVALID (stale binaries, not a code bug):
  build-B configured CAP_ENABLE_PYTHIA=OFF so provenance-study was never
  rebuilt there; run fell back to stale bin/provenance-study (05-27,
  pre-multi-species) -> single-species 211 output, no species_list=.
  bin/provenance-report also stale; the fresh multi-species report binary
  built fine in build-B but wasn't installed.  Fix: reconfigure build-B
  with Pythia ON, rebuild, and run the binaries straight out of build-B
  (or cmake --install).  My plotter/report code unchanged — both handled
  the single-species input correctly.
- [B] 2026-05-29  GUI work requested by user, but GUI is A's slice — B made
  NO edits to gui/.  Wrote `COORD-B-gui-handoff.md` (ladder↔Pythia coupling,
  species-pair picker, genealogy boundary config, genealogy-tree viz) + a
  standalone genealogy-tree prototype for the user.
- [B] needs: provenance-study.cpp — `--pairs <a>x<b>,...` flag to compute
  only selected species-pairs (GUI pair picker; all-pairs is too costly).
- [B] needs: provenance-study.cpp / ProvenanceTagger — `--genealogy-stop
  <Stage>` (or `--genealogy-depth N`) to bound how far ancestry is walked
  (genealogy-boundary config).  Details in COORD-B-gui-handoff.md.
- [B] 2026-05-29  user reassigned the GUI to B.  B now owns
  `cap-provenance-gui` + `gui/provenance_gui/**`.  Building: ladder↔Pythia
  coupling (pipeline.py), species+pair picker, plotter --species/--pairs
  output filter, Explorer genealogy boundary config + clean tree.  A: the
  two `[B] needs` engine flags (--pairs, --genealogy-stop) still stand;
  GUI degrades gracefully without them.
- [B] 2026-05-29  GUI redesign (round 2): new `panels/particles.py`
  (ParticlesPairsPanel — particle pool + explicit pair composer, own tab)
  and `panels/genealogy.py` (GenealogyRequestPanel — a study-request form,
  own tab) REPLACE the old auto-grid pairs picker and the DAG Explorer.
  `panels/pairs.py` tombstoned; `panels/explorer.py` left on disk but
  unwired.  run_params.py no longer owns species (the pool does).  Tabs:
  Run | Particles & pairs | Pythia | Genealogy | Output.
- [B] 2026-05-29  ladder redesign: the fixed 4-preset ladder is gone.  New
  `LadderDesignerPanel` (in ladder.py) lives in the Pythia tab — arbitrary
  named rungs, each toggling ISR/FSR/MPI/CR/rope on the shared Pythia base.
  pipeline.py writes ladder.txt from state['ladder_rungs'] in
  cap-mechanism-ladder's "name: MECH..." format (validated).  Old auto-couple
  via rungs.cumulative_rungs now only seeds the default sweep.
- [B] needs: cap-mechanism-ladder (A's tool) — support PER-RUNG override
  lines (arbitrary readString), not just the 5 mechanism tokens, so users
  can vary parameters (tunes/knobs) per rung, not only mechanisms.  Suggested
  ladder.txt extension: "name: MECH... | readString;readString".  GUI already
  captures per-rung 'overrides'; emission waits on this.
- [B] 2026-05-29  GUI: tabs (Run/Particles/Genealogy) now scrollable via
  widgets.scrollable() so inputs never clip.  New benchmark.py (no tkinter)
  measures local evt/s + parallel efficiency and recommends single vs
  parallel + job count; Parallelism panel gained Local/Grid choice (grid =
  Wayne State, deferred), a CPU-power slider (cap cores), and a Run-benchmark
  button.  Benchmark shells out to bin/provenance-study (needs current bin).
- [B] 2026-05-29  ladder builder reworked to a DERIVATION model: per
  mechanism choose off/baseline/vary + sweep mode (cumulative | all
  combinations); the app derives state['ladder_rungs'] and previews them
  live.  Pipeline unchanged (still writes name: mechs).  Ladder preview shows
  the parallelism implication (N runs -> auto J jobs); 'auto' mode relabelled
  'Parallel — auto (match ladder)' and already uses advise(rungs), so the
  ladder size drives the job count automatically.
- [B] 2026-05-29  GUI polish + integration: fixed acceptance.py grid
  collision (separator was slashing through the spherocity row).  Added DNA
  emoji to the header.  Adopted the shared analyses/builder/cap_theme (via a
  defensive provenance_gui/theme.py shim) so the provenance GUI matches
  run-cap's dark style.  Made provenance_gui.app embeddable
  (App(root, container) + embed(parent, root)).  EDITED gui/run-cap (NOT in
  any COORD owns list — flagging here): added a "Provenance 🧬" notebook tab
  that embeds our GUI, wrapped in try/except so it can never block run-cap
  launching.  A/anyone: shout if gui/run-cap is actually owned elsewhere.
- [B] 2026-05-29  Herwig + Pythia-vs-Herwig comparison started.  GUI side
  done: generic GeneratorPanel (genpanel.py), HerwigPanel (Herwig tab, uses
  existing generator_presets Herwig tables), generator selector
  (generators.py, state['generators']), pipeline materialise_herwig_in().
  Plotter overlay is already generator-agnostic.  Full design +
  engine contract in COORD-B-herwig-handoff.md.
- [B] needs: provenance-study.cpp — `--hepmc3 FILE` input mode reusing the
  existing HepMC3HistoryBuilder (build CAP_ENABLE_HEPMC3=ON) so Herwig events
  (Herwig → HepMC3) flow through the same tagger.  Plus a Herwig resonance
  resolver / PDG fallback, and a `generator=` token in .root.txt for curve
  labelling.  This is the one real blocker for Herwig + generator comparison.
- [B] 2026-05-29  generator-comparison flow wired in pipeline.py: anything
  other than plain single-Pythia runs each generator into compare/<gen>.root;
  >1 generator overlays them via cap-provenance-plot --ladder compare/.  The
  Herwig run is gated by herwig_supported() (probes provenance-study --help
  for --hepmc3/--herwig flags) so it stays dormant on today's Pythia-only
  binary and auto-activates when A lands the flag — no GUI change needed.
  Verified command-building incl. single-Herwig edge case (no stray Pythia run).
- [B] 2026-05-29  plotter generator-comparison overlays: cap-provenance-plot
  gained --compare DIR (overlays <generator>.root with FIXED per-generator
  colours: Pythia=blue, Herwig=red, ... + pretty labels "Pythia 8"/"Herwig 7",
  output compare_<hist>.*, title "across generators").  overlay_ladder now
  takes colours= + context_label=.  Pipeline compare mode calls --compare.
  discover_generators() ordering/labels/colours logic-tested; no regression.
- [B] 2026-05-29  RESUME mechanism — Layer 1 done (Session B, no engine
  change): gui/provenance_gui/resume.py + pipeline/app wiring.  A run writes
  an atomic manifest (.cap-run-state.json) with a config fingerprint + unit
  list; on re-run after a crash/shutdown, units whose <out>.root.txt already
  exists (same fingerprint) are skipped — so a multi-unit job (comparison /
  ladder) reuses finished units, and a single run that finished but lost its
  plot/report stage re-runs only the cheap stages.  Full design in
  COORD-B-resume-design.md.  Unit + end-to-end tested.
- [B] needs: provenance-study.cpp — `--checkpoint FILE [--checkpoint-every N]`
  + `--resume FILE`: atomic mid-run snapshot of partial histograms +
  events-done + Pythia RNG state, resume to target (bit-identical).  This is
  Layer 2 — the only way to save a SINGLE long run killed mid-generation.
- [B] needs: cap-mechanism-ladder — `--resume` (skip rungs whose .root exists).
- [B] 2026-05-29  cross-generator MECHANISM comparison + Herwig ladder:
  new mechanisms.py (generator-agnostic catalogue: MPI/CR map to BOTH gens,
  ISR/FSR/rope Pythia-only).  GUI 'compare mechanisms across generators'
  toggle (generators.py).  Pipeline: when on + >1 gen + ladder rungs, runs
  one unit per (generator, rung) into compare/<gen>_<rung>.root varying only
  the shared mechanisms; Pythia runs today, Herwig gated by herwig_supported()
  + writes per-rung Herwig decks.  Plotter --compare now parses <gen>_<rung>
  names -> labels "Pythia 8 · +MPI" coloured by generator.  Ladder builder
  shows per-mechanism generator availability.  Resume/plan_units product-aware.
  All pure logic tested; no regression.  Herwig execution still waits on the
  --hepmc3 engine flag (COORD-B-herwig-handoff.md).
- [B] 2026-05-29  FIX: GUI run crashed with 'unknown option: --sphero-low'
  because it ran the STALE bin/provenance-study (no sphero flags).  pipeline.py
  now probes the binary's --help (supported_flags(), cached) and drops any
  acceptance flag the binary doesn't advertise — so a stale bin/ degrades
  gracefully instead of erroring.  (Root cause is the recurring stale bin/:
  current source DOES support --sphero-low; user should refresh bin/ from
  build-B for sphero/Herwig/etc.)
- [B] 2026-05-29  HERWIG NOW RUNS (engine wired): provenance-study.cpp gained
  `--hepmc3 FILE` — reads HepMC3 events via the EXISTING HepMC3HistoryBuilder
  instead of Pythia (Pythia still inits for the resonance resolver).  Guarded
  by CAP_ENABLE_HEPMC3; CMakeLists links HepMC3::HepMC3 + sets the define for
  the provenance-study target.  GUI: Generators tab has a 'Herwig .hepmc'
  field; the Herwig unit runs `provenance-study --hepmc3 <file>` →
  compare/herwig.root, overlaid with Pythia.  herwig_supported() auto-detects
  --hepmc3 in --help.  UNTESTED BY B (no ROOT/Pythia/HepMC3/Herwig in sandbox)
  — needs a HepMC3-enabled build:  cmake .. -DCAP_ENABLE_PYTHIA=ON
  -DCAP_ENABLE_HEPMC3=ON <hepmc3 paths>.  Brace/paren balanced; Python side
  compiles + gating tested.  (Auto Herwig deck+run orchestration deferred —
  user supplies .hepmc from the main GUI's proven Herwig flow for now.)
- [B] 2026-05-29  ✅ VERIFIED ON HARDWARE: Pythia + Herwig provenance +
  comparison report all run from the CLI.  build-B reconfigured with
  CAP_ENABLE_HEPMC3=ON (HepMC3 at /Users/oveissheibani/LocalHerwig/.../opt);
  provenance-study --hepmc3 read a real Herwig LHC_1000.hepmc → herwig.root
  (engine C++ compiled+ran clean first try).  cap-provenance-plot --compare →
  10 compare_* overlays (Pythia 8 vs Herwig 7).  provenance-report → 11-page
  PDF, 0 LaTeX errors.  Run binary needs DYLD_FALLBACK_LIBRARY_PATH=<hepmc3
  lib> on macOS (or rpath).  FOLLOW-UPS: (a) Herwig shows Primary 0% / Gluon
  0% — HepMC3 status→Stage mapping in HepMC3HistoryBuilder/StageTaxonomy is
  coarse for cluster hadronization (A's area or known limit); (b) compare_*
  figures currently land in the report's single-particle section — B to add a
  dedicated 'Generator comparison' section.
- [B] 2026-05-29  CHUNKED RESUME for long runs (no engine checkpoint needed):
  a Pythia run over chunk_events auto-splits into resumable chunks (Layer-1
  per-chunk skip via .root.txt), then `hadd` merges histograms + new
  cap-merge-summaries merges the .txt summaries.  Interrupted at chunk 7/10 →
  re-run does 7-10 + remerge.  GUI: 'Resume chunk' field on Run tab (default
  50000; 0=off).  Ladder builder: one-click presets (Classic sweep / MPI /
  CR / Pythia↔Herwig).  pipeline now auto-picks the FRESHEST provenance-study
  / -report (bin/ vs build-B) so the GUI stops running stale binaries.  All
  chunk logic + merger logic-tested.  Herwig ladder DESIGN works (mechanisms
  show generator availability) but per-rung Herwig EXECUTION still needs auto
  Herwig→.hepmc generation (Herwig not even on PATH; one .hepmc at a time).
- [B] 2026-05-29  HERWIG WIRING COMPLETE (ladders like Pythia): new
  herwig_runner.py reuses run-cap's exact recipe — copy shipped LHC.in, inject
  HepMC snippet + config/mechanism lines BEFORE saverun, rename saverun to the
  run-stem, then `source herwig-env.sh && Herwig read X.in` → X.run →
  `Herwig run X.run --numevents N` → X.hepmc → provenance-study --hepmc3.
  Pipeline: single Herwig → compare/herwig.root; per-rung Herwig (compare_mech)
  → herwig_<rung>.root with that rung's MPI/CR lines.  Caches .hepmc (skip
  regen on resume).  HW_PREFIX env (default LocalHerwig).  DECAY-DISABLE
  AUDIT: `set /Herwig/Particles/<name>:Stable Stable` is correct ThePEG syntax;
  name table verified vs Particles.in; and herwig_runner injects it BEFORE
  saverun (the correctness-critical point) — verified by deck-injection test.
  (cτ cut `DecayHandler:MaxLifeTime <v>*mm` plausible but verify interface name
  on the install.)  Deck-injection + command logic unit-tested; Herwig run
  itself untested by B (no Herwig in sandbox) — user verifies on hardware.
- [B] needs: provenance-report.cpp — parameterise the hard-coded "Pythia 8"
  in the abstract/method (buildPaper structural) from the summary's generator.
- [B] FOUND BUG (A to fix): ProvenanceObservables::report() (lines ~301-320)
  reads BARE hist names ("pt_origin_<class>", "pt_parton_<class>").  In
  multi-species mode those only exist suffixed (_S<pdg>), so the by-origin /
  by-parton summary tables print all 0.00 % (seen in multi.root.txt).  Pair
  table is fine (uses counters).  Fix: when _species.size()>1 sum integral()
  over all "pt_origin_<class>_S*" keys.  Affects B's report tab:origin /
  tab:parton + headline bullets; does NOT affect figures.

---
## [B] 2026-06-02 — Herwig ladder wiring VERIFIED end-to-end
Smoke test (2000 evt, K0S stabilized + MPI off) passed on hardware:
deck -> `Herwig read` (accepted `set /Herwig/Particles/K_S0:Stable Stable`
and `set /Herwig/Shower/ShowerHandler:MPIHandler NULL`) -> `Herwig run`
-> 112 MB .hepmc -> `provenance-study --hepmc3` -> .root + .root.txt.
Numbers sane (FromResonance 71%, FromWeakDecay 28.5%).

FIX (mine, gui/provenance_gui/mechanisms.py): Herwig MPI-off was
`MPIHandler:pTmin0 1000.0` — REJECTED, value outside parameter limits.
Now `set /Herwig/Shower/ShowerHandler:MPIHandler NULL`.

[A] NEEDS (not mine, shared): analyses/builder/generator_presets.py
no_mpi toggle lines 458 & 495 still use the broken `pTmin0 1000.0`.
Same fix applies. Main-GUI Herwig "MPI OFF" toggle will fail `Herwig read`.

[A] KNOWN: HepMC3HistoryBuilder Primary=0% (cluster PDG 81/91 not treated
as parton->hadron boundary). Confirmed again here. Gluon=0% is real physics.
Clear to proceed to 500k run.

## [B] 2026-06-02 — Herwig chunked-resume (full run matrix)
herwig_runner.build_deck now takes seed= -> injects `set /Herwig/Random:Seed N`
(chunks MUST differ or they regenerate identical events).
pipeline._herwig_cmds now mirrors _pythia_cmds: chunk_events>0 splits a long
Herwig run into N decks (seed=base+idx) -> .run -> .hepmc -> .root, then
hadd + cap-merge-summaries. Resume skips finished chunks; cached .hepmc reused.
plan_units() chunk-aware for the generator-standalone path too.
Matrix verified (logic): single/chunked x resume on/off for Pythia AND Herwig,
override .hepmc = single unit. NOTE: chunks execute SEQUENTIALLY in Runner
(true concurrent execution only via cap-mechanism-ladder --jobs); chunking
gives crash-recovery, not multi-core speedup, for a single rung.

## [B] 2026-06-02 — Cross-generator COMPARISON report
Goal (user): ONE comparison report (individual per-gen reports stay), with
genealogy fraction tables for BOTH generators side by side, every comparison
histogram overlaying BOTH, and a pair-origin comparison.

cap-provenance-plot (B): added COMPARE_HISTS — a CURATED set of observables
BOTH generators populate (origin/parton spectra, pair ancestry dphi/deta/mass,
decay_chain_depth, n_parton_ancestors, common_ancestor_depth, multiplicity,
spherocity). Compare mode now iterates COMPARE_HISTS (was the thin LADDER_HISTS).
DELIBERATELY EXCLUDES pair_MPI_* and pair_Shower_* and SharedParton-depth: those
need per-particle MPI-index/shower-origin tags that PythiaHistoryBuilder sets from
status codes but HepMC3HistoryBuilder cannot infer -> they'd be Pythia-only. They
stay in Pythia's individual report. overlay_ladder now labels an empty series
"(no entries)" so a zero-Herwig curve never looks like a missing plot.

provenance-report.cpp (B figure-layout): added --summary2/--label1/--label2,
isCompareFigure, fillCompareTable (union of class names, per-gen % columns),
buildComparison() = abstract + how-to-read + "what is comparable" table (MPI/CR
comparable; shower partially; hadronization string-vs-cluster NOT, drives the
divergences) + genealogy compare tables (origin,parton) + pair-origin compare
table + compare_* figures + config. Single-summary path UNCHANGED (individual
reports unaffected).

pipeline.py (B): report stage adds --summary2 + labels when compare_overlay and
NOT compare_mech (plain pythia.root + herwig.root).

NOTE for [A]: MPI-index / ISR-FSR shower-lineage tagging via HepMC3HistoryBuilder
would let those decompositions enter the comparison too (currently Pythia-only).

Verify on Mac (cmake/ROOT not in B's sandbox): bash provenance-b/verify-comparison.sh

## [B] 2026-06-02 — Gluon-origin fix: INITIATING-parton classification
DIAGNOSIS (verified via --dump-events, 12304 pions): the "parton flavour"
table = leadPartonPdg = Lund STRING ENDPOINT, which is a quark by construction
-> Gluon 0% is CORRECT, not a bug. The gluon-origin info lives in hardPartonPdg
(already computed, never histogrammed): hardParton Gluon = 63.6%, NonPartonic
26%, LightQuark 7.8%.

CHANGE (B edited Session A files — ADDITIVE, existing histos untouched):
- ProvenanceObservables.cpp/.hpp: refactored switch -> partonClassOfPdg();
  added classifyInitiatingParton(); accumulate() now fills
  pt_initparton_<Flavour>/eta_initparton_<Flavour> using hardPartonPdg with an
  MPI-parton fallback (history.node(mpiIndex).pdg) so MPI gluons count too;
  report() now prints "by parton flavour (string endpoint):" AND a new
  "by initiating parton flavour:" section.
- provenance-report.cpp (B-owned layout): parse the renamed/added sections
  (d.initparton); comparison report + single report show BOTH a string-endpoint
  table and an INITIATING-parton table (the gluon-vs-quark origin).
- cap-provenance-plot (B): INITPARTON_SERIES + initparton_pt single figure +
  pt_initparton_Gluon/LightQuark/Strange compare overlays.

[A] FYI: I touched ProvenanceObservables.{cpp,hpp}. Pure addition (new hist
names + new summary lines + a refactored-but-equivalent classifyParton).
Existing leadParton behaviour and names unchanged. Rebuild provenance-study.

NOTE: existing .root files predate this -> must regenerate (Herwig from the
cached .hepmc). verify-comparison.sh updated to rebuild both binaries + re-run.

## [B] 2026-06-02 — initiating-parton made GENERATOR-AGNOSTIC (Herwig fix)
First cut classified by hardPartonPdg (HardProcess stage) — but the HepMC3
builder NEVER tags HardProcess, so Herwig was 99.55% NonPartonic. Fixed:
ProvenanceTagger now computes `initiatingPartonPdg` = the TOPMOST parton in
the lineage (walk up; first parton whose parents are NOT partons = beam/hard
vertex). Works for BOTH paths: Pythia (incoming hard parton) and Herwig
(top-of-shower parton in the HepMC graph). ProvenanceObservables +
classifyInitiatingParton now use t.initiatingPartonPdg. provenance-study dump
gains "initiatingPartonPdg". Pythia verified gluon 63.66% via hardParton; the
agnostic walk should keep Pythia gluon-dominated AND give Herwig a real number.
[A] FYI: added one field to ProvenanceTag + a walk in ProvenanceTagger::tag().

## [B] 2026-06-02 — Herwig Primary 0% FIXED (cluster boundary)
DIAGNOSIS (parsed real herwig_baseline.hepmc): dominant DIRECT parent of
primary pi+/- is PDG 81 = Herwig CLUSTER. The builder treated the cluster as a
real hadron -> every cluster-produced primary mislabelled DecayProducts ->
FromWeakDecay. Primary=0%, FromWeakDecay inflated to 28.49%.

FIX (B edited Session A file HepMC3HistoryBuilder.cpp): added
pdgIsHadronizationBoundary(pdg) = {81,91,92} (Herwig cluster / generic cluster
/ Lund string). A hadron whose parent is a boundary now counts as fromPartons
-> PrimaryHadrons. PROVEN on real .hepmc (Python sim of corrected logic):
Primary 0.00% -> 24.09%. Expect FromWeakDecay 28.5% -> ~4-5%, FromResonance
~71% unchanged. Pythia path UNAFFECTED (uses PythiaHistoryBuilder).
[A] FYI: 4-line addition to HepMC3HistoryBuilder.cpp, behind a named helper.
Rebuild provenance-study; regenerate Herwig roots.

## [B] 2026-06-03 — MPI+CR mechanism ladder (both generators)
provenance-report.cpp (B): added --summaries "L1=p1;L2=p2;..." mechanism-LADDER
mode -> fillMultiTable (N columns) + buildLadderComparison: one column per
(generator,rung), tables for origin/initparton/endpoint-parton/pair across all
columns + compare_* overlay figures. Single/two-summary paths unchanged.
run-ladder.py (NEW, B): drives 2 gens x 3 cumulative rungs (baseline/+MPI/
+MPI+CR) via mechanisms.mech_lines + herwig deck recipe; resume per-rung;
auto-delete Herwig .hepmc after each study; then compare-plot + ladder report.
Rung mech lines verified. Needs provenance-report rebuilt for --summaries.

## [B] 2026-06-04 — Entropy / information module (Phase 5, opt-in)
NEW EntropyObservables.hpp/.cpp (src/EventHistory): self-contained accumulator
— multiplicity Shannon entropy per |eta| window (Kharzeev-Levin observable),
per-stage entropy-production profile over the EventHistory DAG, and mutual-
information "information recovery" rows (I(initParton;species/pT), primary vs
decay subsets, I(NF;NB)).  Miller-Madow corrected; pure C++14, no ROOT;
unit-tested standalone (17 checks).
WIRING (all guarded, default off => byte-identical behaviour):
 - provenance-study: --entropy flag; appends "entropy:"-marked block to
   .root.txt + writes ent_* histograms.  Flag off = unchanged output.
 - provenance-report: parses entropy markers only when present; new gated
   sections in paper / comparison / ladder modes (tables + figures).
 - cap-provenance-plot: entropy_* single-run figures + compare_ent_* figures
   (require_all so never silently single-generator); stage profiles are
   never normalized.
 - pipeline.py: acceptance["entropy"] -> --entropy on every study cmd
   (single/compare/chunked/ladder); acceptance is already a fingerprint key
   so resume correctly invalidates.  GUI checkbox in AcceptancePanel.
 - run-cli.py / run-ladder.py: --entropy pass-through.
[A] FYI: one new source file in src/EventHistory/CMakeLists.txt lib list;
no existing class touched except additive edits listed above.  Rebuild
EventHistory + provenance-study + provenance-report.

## [B] 2026-06-04 — Entropy content upgrades (same guarded pattern)
EntropyObservables: + Renyi-2 ("collision") entropy via UNBIASED power-sum
estimator; + <N> and KL maximal-entanglement ratio S/ln<N> per window;
+ FB mutual information scanned over eta gaps 0..2 (ent_fb_mi_gap hist);
+ event-level I(N_MPI;N_ch) from mpiIndex tags (Pythia-only physics, 0 by
construction on HepMC input); + per-event <S_event> per stage next to the
ensemble S_occ (their difference = event-diversity information).
Report: entmult rows now carry S2/meanN/S-lnN keyword pairs (parser reads
old AND new format); KL-test 6-col table + 5-col KL compare table; stage
tables gained the <S_event> column (4-col rows still render).
Plotter: entropy_kno (KNO collapse, TMultiGraph), entropy_fb_mi_gap +
compare_ent_fb_mi_gap (value profiles, never normalized), entropy_nmpi.
NEW provenance-b/run-entropy-scan.py: multi-sqrt(s) driver (resumable,
per-point skip on existing .root.txt) -> entropy-scan.csv + S vs ln sqrt(s)
PDF.  All layers re-verified (C++ unit tests x2, parser round-trip, py
syntax, scan-parser on real output).

## [B] 2026-06-05 — Report coherence + hadronization-interface observables
ProvenanceObservables: NEW event-level, generator-agnostic hadronization
histograms — had_npartons (partons entering hadronization), had_nprimary
(primary hadrons before decays), had_ratio (hadrons per parton: THE
string-vs-cluster fingerprint).  Additive; legacy names untouched.
cap-provenance-plot: REPORT_ORDER canonical stem list + sort_manifest()
(applied before writing figures.manifest) — figures now read topically,
each raw plot immediately followed by its normalized/fraction variant;
applies to single, compare and ladder output (compare_/ladder_ prefixes
and _S species suffixes stripped for ordering).  had_* added to
SINGLE_FIGURES(+GLOBAL) and to COMPARE_HISTS (hadronization group first).
provenance-report: renderFiguresGrouped() — figures render under
\subsection* topic headers (origin -> hadronization interface -> parton
ancestry -> event context -> pair core(raw/normalized/fraction) -> SS/OS
-> resonance -> sharing/flavour -> MPI -> shower -> HF -> activity bins;
leftovers under "Other decompositions" so nothing drops).  Used in paper,
comparison and ladder modes.  normalizedFigStem() unit-tested (8 checks);
had_* fills unit-tested; manifest sort verified.
[A] FYI: all additive; rebuild EventHistory + provenance-study +
provenance-report.

## [B] 2026-06-05 — Multiplicity concepts disentangled (hadron / charged / centrality)
ProvenanceObservables (always-on, additive): nmult_hadrons (ALL final
hadrons, charged+neutral), nmult_charged (+ _eta10 tracker window, + _fwd
ALICE-V0M-like windows = pp centrality ESTIMATOR), nmult_nmpi (model-truth
centrality: distinct MPI scatters; Pythia tags only), and the
nmult_nch_vs_nmpi_{sum,n} pair -> mean-Nch-vs-NMPI estimator-quality
profile (divided at plot time, hadd-safe).  Legacy event_multiplicity
UNCHANGED but every caption/glossary now states it is the STUDIED-SPECIES
count (pair-bin axis), not N_ch.  Plotter: figures + compare entries
(nmult_nmpi excluded from compare per no-silently-single-generator
policy), emit_nch_vs_nmpi skips when only one N_MPI bin (HepMC input).
Report: "Event multiplicity and centrality" topic + 4 glossary entries.
Unit-tested: crafted event verifies hadron/charged/eta10/fwd/NMPI/profile
counts (6 checks); figure-coverage audit zero strays; py syntax OK.
[A] FYI: additive only; rebuild EventHistory + provenance-study +
provenance-report.

## [B] 2026-06-05 — Audit pass: 3 bugs fixed
1. cap-provenance-plot emit_nch_vs_nmpi: histogram used AFTER TFile::Close
   (dangling ROOT object) -> clone + SetDirectory(0) before close.
2. provenance-report fillGlossaryTable: term cell was NOT texEscape'd ->
   new "Charged multiplicity (N_ch)" term would break pdflatex; both cells
   now escaped.
3. cap-mechanism-ladder: no --entropy argument -> GUI ladder stage with the
   entropy checkbox would crash on unknown flag; added parse + per-rung
   forward (mirrors --validate-graph).
Known accepted behaviour: 2 of 20 paired-figure groups span two topics
(sharedparton depth|mpi, origin|parton fraction) and now render as
correctly-captioned singletons in their own topics — deliberate, graceful.

## [B] 2026-06-05 — FragmentationSystems module (Phase 6, opt-in --systems)
NEW FragmentationSystems.hpp/.cpp: generator-agnostic "fragmentation
system" = union-find component of primary hadrons sharing a hadronization
source (string partons / Herwig cluster PDG 81); kinematics from the
primary-hadron sum (conserved in both models -> comparable masses BY
CONSTRUCTION).  Observables: nsystems/event, nhad/system, system mass
(+zoom), <n> vs mass (Lund ln m^2 scaling vs cluster fission), rapidity
span, lambda string-length measure (CR's minimization target -> ladder
headline), charge ordering (OS/SS x same/cross-system + rapidity
neighbours), local pT compensation, B-Bbar + strangeness pairing,
Herwig-only explicit cluster fission chain (top vs decaying masses).
Wiring (guarded, default off): provenance-study --systems; pipeline
acceptance["systems"] (resume-fingerprint-safe); GUI checkbox;
cap-mechanism-ladder --systems parse+forward; run-cli/run-ladder
--systems.  Plotter: FRAG_FIGURES + FRAG_OVERLAYS + COMPARE_FRAG_HISTS
(require_all) + <n>-vs-mass profile (clone-before-close).  Report:
"fragmentation:" parser sections, paper/comparison/ladder tables (lambda
across rungs), own figure section, comparison topic, 3 glossary entries.
Verified: 22-check unit test (Pythia-like 2-string + Herwig-like cluster
chain incl. fission counts), parser round-trip on real output, pipeline
flag routing + fingerprint, figure-coverage zero strays, all py syntax.
[A] FYI: one new source in CMakeLists lib list; everything else additive.

## [B] 2026-06-05 — Smoke-test audit: table overflow fixed, reports consolidated
Root cause of overflowing tables: plain `l` tabular columns never line-
break, so prose cells ran past the page frame.  Fixed by p{..\textwidth}
specs on every prose-bearing table in provenance-report.cpp: comparable-
components (worst offender), repro env, frag + frag-compare, entropy +
entropy-compare.  Glossary already used p{}; all numeric tables audited
and left alone (short cells).  Requires provenance-report rebuild.
Ladder lives in its own work dir BY DESIGN (its --compare plot stage
writes compare_* figures + figures.manifest that would clobber the main
run's plots if shared); consolidation = cp its provenance-ladder.pdf into
the main reports/ — baked into the smoke/run recipes.

## [B] 2026-06-05 — WSU "warrior" grid inventory filed (future deployment)
Grid/WSU/warrior_grid_inventory.md + grid_audit_report_20260605.txt.
Key facts for launching provenance on the cluster: SLURM (sbatch,
partition mdtp); toolchain gnu7/7.3.0 (system cc is 4.8.5 — must export
CC/CXX/FC); ROOT 6.28.10 module is C++14 — matches our codebase exactly.
Pythia 8.317 built WITH HepMC3+LHAPDF (good for provenance-study);
Herwig 7.1.0 is HepMC2-only via ThePEG 2.1.0 — BUT our --hepmc3 path uses
HepMC3::deduce_reader, which auto-reads HepMC2 ascii, so warrior's
existing Herwig output is already consumable without rebuilding Herwig.
Chunked-resume design maps naturally onto SLURM array jobs (one chunk =
one task).  Not started; filed for when we deploy.

## [B] 2026-06-05 — WSU deployment plan written
provenance-b/WSU_DEPLOYMENT_PLAN.md: 6 phases (build on warrior with
CAP_PYTHIA8_PATH/CAP_HEPMC3_PATH -> sbatch smokes -> SLURM-array submitter
(one task = gen x rung x subsample, resume via .root.txt) -> combine =
hadd central values + CAP SubSampleStatCalculator for subsample errors ->
reports -> production 120 tasks / 4.5M events).  HONEST FLAG for [A]:
SubSampleStatCalculator::execute has the squareDifferenceLists combine +
export COMMENTED OUT in 8.0 ("needs to be fixed"); underlying math exists
in Helpers/HistogramGroup.  We need it re-enabled (or will ship a 100-line
standalone fallback) before grid Phase D.  Risk register in the doc.
