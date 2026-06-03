# CAP — Knowledge Booklet (Session B)

A living map of the whole program, built by reading the code.  Kept updated as
I learn.  Status: ✅ read & understood, 🟡 partial, ⬜ not yet read.

Last updated: 2026-05-29 (after reading Base framework, generators/readers,
analyzers, and EventHistory internals).

---

## 0. What CAP is  ✅

**CAP = Correlation Analysis Package** — a ROOT-based C++ framework for
correlation, balance-function, jet, flow, spherocity and HBT analyses on
Monte-Carlo and experimental particle data.  ~87k lines C++ (`src/`, one
library per subdir), ~8k lines Tk GUI Python, plus installer/launcher scripts.
Authors: Claude Pruneau, Victor Gonzalez (Wayne State).

Two worlds share the repo:
1. **Main CAP framework** — `bin/CAP <Task> <project> <ini> <out>` runs a
   *task tree* from a `.ini`: generator/reader → filters → analyzers →
   histograms in `histos/<out>/`.  GUI: `run-cap`, `build-ini-gui`.
2. **Provenance / parton-tracking feature** (Session B) — `src/EventHistory`
   + standalone tools (`provenance-study`, `cap-mechanism-ladder`,
   `cap-provenance-plot`, `provenance-report`) + `gui/provenance_gui`.  Does
   NOT go through `bin/CAP`.

---

## 1. Framework core (src/Base, src/Particles)  ✅

**Task model** (`src/Base/Task.hpp`): composite tree.  `Task` ⟵ `Object` +
`ConfigurationManager`.  Holds `vector<Task*> _subTasks` + `_parentTask`.
Lifecycle (parent-first init/execute, parent-last finalize):
`setDefaultConfiguration() → configure(Configuration&) → initialize() →
execute() (looped) → finalize()` (+ postProcess/partial/reset/clear).

**CAP.cxx** (`bin/CAP`): args `(taskName, configPath, configFile, histPath)` →
load Configuration from `.ini` → `createTask()` factory builds the named task →
`initialize()→execute()→finalize()`.

**Configuration** (`src/Base/Configuration.hpp` ⟵ `Properties`): `.ini` parsed
into `(key,value)` Property list; colon-namespaced keys
(`RunAnalysis:PARTICLE_DB:IMPORT:FILE_NAME`).  Typed accessors
`valueInt/Double/Bool/String`.

**Event/Particle model** (`src/Particles/`):
- `Particle`: `_pid`, `ParticleType* _type`, `VectorLorentz _momentum/_position`,
  `_live`, `Particle* _parent`, `vector<Particle*> _children`, `_truth`.
  `isPrimary()/isSecondary()`.
- `ParticleType`: PDG props (mass/charge/spin/width), quark content, decay modes,
  `_decayRndmSelector`.
- `Event`: `vector<Particle*> _particles`, `_projectileA/B`, indexed event-level
  arrays `_multiplicity/_energy/_charge/_strangeness/_baryon/_ptSum/_spherocity`
  (index 0–4 per particle-selection set).
- `ParticleDb` (`DB/` data): `loadFromAscii2(particles.data, decays.data)`,
  `findPdgCode()`, enable/disable species + weak decays.

**Event loop**: `EventProcessor` ⟵ `Task` (the generator/reader/analyzer base) —
manages events, particle/event filters, particle DBs; `filterEvent()` /
`filterParticles()`.  `EventIterator` ⟵ EventProcessor drives the loop:
reads `EVENT:REQUESTED:N`, per event runs subtasks (generator fills the event,
analyzers consume), stops on `EndOfDataException`.  Single-particle analyzers
use `EventProcessorSingle<H1,H2>`; pair analyzers `EventProcessorPair<H1,H2,H3,H4>`.

**Filters** (`src/Base/Filter.hpp` template; `ParticleFilter`, `EventFilter`):
condition lists; particle filter types pT/eta/y/phi/pdg/charge/strange/...;
event filter types mult/energy/charge/sphero bins.  Accepted particles collected
per filter index 0–7.

**Task selection at runtime**: no enum — `.ini` gives `TASK:CLASS`/`TaskClassName`
(e.g. `PythiaEventGenerator`, `HepMC3EventReader`); `Task::createTask()` factory
instantiates by class-name string via ROOT reflection.

---

## 2. Generators & readers  ✅

All ⟵ `EventProcessor` (same initialize/execute/finalize contract).  Generator
chosen via `.ini` `TASK:CLASS`.

- **PythiaEventGenerator** (`src/CAPPythia`): wraps `Pythia8::Pythia`;
  `pythia->next()` per event, walks the record, status/PDG filters, fills CAP
  Particles.  Keys: `Beams:*`, `SeedValue`, `UseQCDCR/UseRopes/UseShoving`,
  `SaveFinalOnly` (default keep status==1), `KeepStatuses` (status whitelist),
  `Option0..29` → `readString`.
- **HerwigEventGenerator** (`src/CAPHerwig`): EMBEDDED in-process Herwig via
  ThePEG.  Loads a `.run` (from `Herwig read input.in`), `eg->shoot()` per event.
  **Known SEGV hazard**: ThePEG cross-event back-pointers — it NEVER frees events
  mid-run (holds all in `_allEventsPtr`, leaks at end).  *File-based HepMC3 is the
  PRODUCTION-PREFERRED path* (run-cap routes Generator=Herwig through
  `Herwig run` → `.hepmc` → HepMC3EventReader to avoid this).  Key:
  `HerwigRunFile`, `LHAPDFDataPath`, `HerwigPluginPath`.
- **HepMC3EventReader** (`src/CAPHepMC3`): `HepMC3::deduce_reader(file)` (ASCII
  .hepmc / .hepmc3 / .root), `read_event()` per event, status/PDG filters → CAP
  Particles.  Decoupled from ThePEG.  Keys: `HepMC3InputFile`, `SaveFinalOnly`
  (default status==1), `KeepStatuses`.  Consumes ANY HepMC3 producer
  (Herwig/Sherpa/EPOS/MadGraph/Pythia-with-HepMC).
- Others: BasicEventGen (toy), Epos/PHSD (file readers), Glauber (geometry only),
  Therminator3 (thermal hadron gas).

---

## 3. Analyzers + the multi-pass model  ✅

Each module: `<X>Analyzer` ⟵ `EventProcessorSingle/Pair<...>`, with
`<X>Histos` (fill pass) + `<X>DerivedHistos` (derived) [+ `<X>BfHistos`].
Histogram lifecycle: createHistograms/fillHistograms (per event)/saveHistograms;
derived classes `calculateDerivedHistograms(base)`.

- ParticleSingle: yields/spectra (pT/phi/eta/y, 2D, sum-pT).
- ParticlePair / ParticlePair3D: 2-particle correlations (Δφ,Δη[,mass]) →
  C2/R2/G2/P2; BF pass → B12/B21 (balance functions).
- Global: event-level n/e/q/s/b.  NuDyn: factorial moments/cumulants (ν_dyn).
  PtPt: pT fluctuations.  Spherocity: event-shape S0.  ParticleFlow: vn.
- SubSample: not an analyzer — sub-sample variance for stat errors.

**Three-pass chain (encoded in .ini)**: Gen (EventIterator fills histos →
`SingleGen.root`...) → RunDerived (imports Gen, computes correlation
functions/cumulants → `*Derived.root`) → RunBF (imports two charge-separated
derived sets → balance functions).  `.ini` task tree: RunAnalysis with subtasks
ParticleTypeTask / EventFilterCreator / ParticleFilterCreator / EventIterator
(→ generator + analyzers).

---

## 4. Provenance feature — src/EventHistory  ✅ (Session B's area)

**Pipeline**:
```
provenance-study  (Pythia gen  OR  --hepmc3 read)
   → EventHistory DAG  (PythiaHistoryBuilder | HepMC3HistoryBuilder)
   → ProvenanceTagger.tagFinalState(history) -> per-hadron ProvenanceTag
   → ProvenanceObservables (single) + PairProvenanceObservables (pair)
   → <out>.root (257 hists single-species) + <out>.root.txt summary
cap-mechanism-ladder → provenance-study across Pythia mechanism rungs (ladder.txt)
cap-provenance-plot  → PyROOT overlays:  --root | --ladder DIR | --compare DIR
provenance-report    → CAP::LatexDocument paper/beamer from .root.txt + figures.manifest
```

**Stage taxonomy** (`StageTaxonomy.hpp`): generator-agnostic `enum Stage`
(numeric ↑ with time): Unknown=0, Beam=10, HardProcess=20, MPI=30, ISR=40,
FSR=50, BeamRemnants=60, PartonsPreHadronization=70, PrimaryHadrons=80,
DecayProducts=90, FinalState=100.  `isPartonic()/isHadronic()/stageOrder()`.
Each builder maps the generator's status zoo onto this.

**EventHistory** = DAG of `ParticleNode`s (pdg, status, isFinal, 4-mom,
production vertex, Stage, parent/child links).

**ProvenanceTagger**: walks parent links from each final hadron → records
production stage, first non-parton parent (resonance? via curated PDG list OR a
Pythia particle-DB cτ resolver), pre-hadronization parton ancestors + deepest
hard-process parton, MPI/ISR/FSR ancestry, charm/bottom chain, decay-chain
depth.  Origin classes Primary/FromResonance/FromWeakDecay/Unknown; parton
classes LightQuark/Strange/Charm/Bottom/Gluon/NonPartonic.

**Observables**: ProvenanceObservables (single-particle pt_/eta_ by
origin/parton/HF/shower/MPI; n_parton_ancestors; decay_chain_depth) and
PairProvenanceObservables (dphi_/deta_/mass_ pair classes SameResonance/
SameWeakParent/SharedParton/Unrelated + SS/OS, resonance subtypes, MPI, shower,
HF, mult/spherocity bins; common_ancestor_depth; event_multiplicity/spherocity
GLOBAL no suffix).

### ⭐ Why Herwig shows Primary 0% / Gluon 0%  — VERIFIED end-to-end in code
Traced through ALL three files (HepMC3HistoryBuilder.cpp, ProvenanceTagger.cpp,
ProvenanceObservables.cpp), not inferred.

CONTRAST OF THE TWO BUILDERS:
- PythiaHistoryBuilder.stageFromPythiaStatus(): stage assigned DIRECTLY from the
  particle's own Pythia status code — 11-19 Beam, 21-29 HardProcess, 31-39 MPI,
  41-49 ISR, 51-59 FSR, 61-69 BeamRemnants, 71-79 PartonsPreHadronization,
  **81-89 PrimaryHadrons**, 91-99 DecayProducts.  Authoritative, clean.
- HepMC3HistoryBuilder: HepMC status is only final(1)/decayed(2)/doc(3)/beam(4),
  so stage is INFERRED from the graph.  Hadron rule: `fromPartons && !fromHadrons`
  → PrimaryHadrons; any hadron parent → DecayProducts.  pdgIsParton = |pdg|<10 or 21.

ORIGIN classification (ProvenanceTagger.tag + classifyOrigin):
  isPrimaryHadron = (stage==PrimaryHadrons); isFromDecay = (stage==DecayProducts).
  classifyOrigin: isPrimaryHadron→Primary; elif isFromDecay→(isFromResonance?
  FromResonance : FromWeakDecay); else Unknown.

- **Primary 0% = BUG/artifact (CONFIRMED).**  Herwig hadronizes via CLUSTERS
  (PDG 81/91).  A primary hadron's parent is a cluster → cluster is NOT a parton
  → hadron hits `fromHadrons` → stage=DecayProducts → isPrimaryHadron=false,
  isFromDecay=true, isResonance(81)=false → classifyOrigin = **FromWeakDecay**.
  So Primary=0% and the ~40% "FromWeakDecay" is really mislabeled primaries
  (matches observed: Primary 0 / FromResonance 60 / FromWeakDecay 40).
  PRECISE FIX (HepMC3HistoryBuilder.cpp, ~5 lines; Session A's file, coordinate):
    add pdgIsCluster(pdg){ a==81||a==91||a==92; }  and in the hadron branch set
    fromPartons if a parent isParton OR isCluster, fromHadrons only for REAL
    hadron parents (not parton, not cluster).  → hadron-from-cluster becomes
    PrimaryHadrons; parton ancestry already walks through the cluster (the
    partons feeding it already get PartonsPreHadronization because their end
    vertex emits a non-parton cluster).
- **Gluon 0% = REAL PHYSICS (CONFIRMED).**  classifyParton switches on
  |leadPartonPdg| at PartonsPreHadronization (1/2 light, 3 s, 4 c, 5 b, 21 g).
  Herwig's cluster model forces g→qq̄ before clustering, so pre-hadronization
  partons are quarks → leadPartonPdg never 21 → Gluon 0%.  Pythia strings keep
  gluon kinks → Gluon ~5%.  Correct model difference, NOT a bug.  (This is why
  Herwig LightQuark/Strange WERE populated — parton ancestry works fine.)

### B's engine change (provenance-study.cpp `--hepmc3`)  ✅ VERIFIED
`--hepmc3 FILE` (guarded `CAP_ENABLE_HEPMC3`): `HepMC3::deduce_reader` loop →
`HepMC3HistoryBuilder.build(genEvent, history)` instead of `pythia->next()`;
Pythia still inits for the resonance resolver.  Tested on real Herwig
LHC_1000.hepmc → herwig.root.  CMakeLists links HepMC3 + define for the target.

---

## 5. GUI layer  🟡

- **run-cap** (`gui/run-cap`, ~4450 lines): main job runner.  Tabs Compose&Run /
  Subsample / Wayne State / (B-added) Provenance (embeds provenance_gui).  Runs
  `bin/CAP <task> <project> <ini> <out>`.  **Herwig file-based flow** (✅ read):
  `HW_PREFIX` env (default `/Users/oveissheibani/LocalHerwig/LocalHerwig/opt`),
  binary `<prefix>/bin/Herwig`, env `<prefix>/herwig-env.sh`, base deck
  `<prefix>/share/Herwig/LHC.in`.  `_herwig_to_hepmc(N)`: copy LHC.in → inject
  HepMC snippet (`read snippets/HepMC.in` + `HepMC:Filename`) BEFORE `saverun`
  (`_inject_hepmc_into_in`) → inject config lines (`collect_herwig_lines`,
  `_inject_extra_lines_into_in`) → `source herwig-env.sh && cd <dir> && Herwig
  read X.in` → `X.run` → `Herwig run X.run --numevents N` → `X.hepmc`.  Caches
  `<stem>_<N>.hepmc`.  ← THIS is the reusable Herwig-ladder recipe.
- **analyses/builder** (🟡): `cap_ini_builder.py` (Job schema → .ini),
  `build-ini-gui`, `generator_presets.py` (PYTHIA_/HERWIG_ PRESETS/BOOL_TOGGLES/
  NUMERIC_KNOBS + collect_pythia_strings/collect_herwig_lines), `wsu_script_
  generator.py` (SLURM), `cap_theme.py` (dark theme install/register),
  `cap_preset.py`.
- **gui/provenance_gui** (✅, B wrote it): app.py (embeddable App + tabs),
  panels/ (run_params, generators, particles, pythia, herwig, genpanel, ladder,
  acceptance, stages, parallelism, report, genealogy, log_pane), pipeline.py
  (build_commands + Runner + chunked resume + freshest-binary), resume.py,
  mechanisms.py (Pythia+Herwig mechanism catalogue; MPI/CR common), benchmark.py,
  rungs.py, widgets.py (scrollable), theme.py (cap_theme shim).

---

## 6. Cross-cutting facts  ✅
- Herwig NOT on PATH; run-cap runs it via HW_PREFIX + herwig-env.sh.  Recipe
  fully solved + reusable for a per-rung Herwig ladder.
- provenance-study generates Pythia internally but only READS HepMC for others
  → Herwig ladder = per-rung `Herwig read/run` (reuse run-cap recipe), not a
  fundamental blocker.
- GUI repeatedly ran stale `bin/`; B added freshest-binary resolution (bin/ vs
  build-B) in pipeline.py.
- macOS runtime: HepMC3-linked binary needs
  `DYLD_FALLBACK_LIBRARY_PATH=<HW_PREFIX>/lib` (or rpath).
- build-B must be configured `-DCAP_ENABLE_HEPMC3=ON` with HepMC3 paths from
  `/Users/oveissheibani/LocalHerwig/LocalHerwig/opt`.

## 7. Open / to verify next
- [ ] ProvenanceTagger.cpp internals (exact origin/parton classification rules)
      — re-read to be 100% sure before touching Herwig cluster handling.
- [ ] PythiaHistoryBuilder stage mapping (to mirror for Herwig clusters).
- [ ] analyses/builder cap_ini_builder Job schema details.
- [ ] run-cap Compose/WSU tabs + bin/CAP .ini generation path (non-provenance).
- [ ] Latex module (CAP::LatexDocument) API used by provenance-report.

## Herwig gluon-origin: EVIDENCE from the real HepMC (2026-06-02)
Inspected /Users/oveissheibani/LocalHerwig + herwig_baseline.hepmc.
- Format: HepMC2 IO_GenEvent (HepMC::Version 3.02.07 reader).
- Hard partons tagged status 11 (e.g. P 10003 pid 21 gluon), production vertex
  has beam proton (pid 2212, status 4) as input.
- Final hadrons ARE connected up through shower/cluster to these hard partons
  (not a disconnected doc line) — PROVEN by parsing the file in Python and
  running the topmost-parton walk: pi+/- initiatingParton = 20%/0%/88% gluon
  across 3 events.
=> The generator-agnostic ProvenanceTagger::initiatingPartonPdg walk (topmost
   parton whose parent is non-parton) resolves Herwig gluons correctly. The
   old 99.55% NonPartonic was the pre-fix binary keying on Stage::HardProcess
   (never assigned by HepMC3HistoryBuilder). Rebuild -> Herwig gluon ~50-60%.

## Gluon-origin: SOLVED on hardware (2026-06-02)
initiatingPartonPdg (topmost-parton walk) verified on real runs:
  Pythia : LightQuark 80.4%, Gluon 18.7%, NonPartonic 0%
  Herwig : LightQuark 60.4%, Gluon 36.0%, NonPartonic 0%
Both report real gluon fractions (was 0% / 99.55% NonPartonic before).
Comparison report tab:cmp-initparton shows both columns.
Interpretation: this is the INITIAL-STATE / topmost parton extracted from the
proton (matches "initial ones"). Differs from outgoing gluon-JET origin
(hardPartonPdg ~64% Pythia, Pythia-only). The topmost walk is generator-
agnostic so it is the correct basis for the cross-generator comparison.
Figure-skip gotcha: stale herwig_baseline.root in compare/ made require_all
skip initparton figures; verify-comparison.sh now stages exactly 2 roots.

## Herwig Primary 0% — SOLVED on hardware (2026-06-02)
Cluster-boundary fix (HepMC3HistoryBuilder pdgIsHadronizationBoundary {81,91,92})
verified end-to-end:
  Herwig origin: Primary 0% -> 22.93%, FromWeakDecay 28.49% -> 5.56%,
                 FromResonance 71.06% (unchanged).
  Pythia origin UNCHANGED (34.0/62.6/3.5) — PythiaHistoryBuilder path untouched.
Both generators now have comparable genealogy. Gluon-origin + Primary both fixed.
=> Provenance comparison is publication-sane. Clear for the 500k run.
