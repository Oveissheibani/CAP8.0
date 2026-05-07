# Missing implementations in CAP

The shipped `projects/*/...ini` files reference task classes via
`TaskClassName = CAP::SomeClass`. **Many of these classes are not implemented
in `src/`.** This file is the punch-list of what's missing so somebody
familiar with the CAP design can finish them.

This is one of three "incomplete-refactor" issues we've found in the codebase:

1. `JETS_KNOWN_ISSUES.md` — the Jets module's source files don't compile
2. **This file** — the orchestrator and post-processing classes were never
   written
3. The .ini key-format refactor — done in source but the .ini files were
   never updated. We added backward-compat in `Task.cpp` (see commit message
   for `Task::createTask` / `configureSubtasks`).

---

## Status

After the patches in this branch, `bin/CAP` runs and:

- Reads any `.ini` file
- Successfully resolves the **top-level** `TaskName` and `TaskClassName`
- Tries to instantiate the named class via ROOT's `TClass::GetClass`
- **Fails because the class doesn't exist**

For the canonical workflow file `projects/Pythia/pp_13.7TeV/RunAna.ini`,
**10 of 16 referenced classes are missing**. The error is
`UnknownClassException name=CAP::<missing-class>`.

---

## Classes that exist (good — don't re-implement)

These are present in `src/`, compile, and are linked into the corresponding
shared library:

| Class | File | Library |
|---|---|---|
| `CAP::Task` | `Base/Task.{cpp,hpp}` | `libBase` |
| `CAP::EventProcessor` | `Particles/EventProcessor.{cpp,hpp}` | `libParticles` |
| `CAP::EventProcessorSingle<H,DH>` | `Particles/EventProcessorSingle.hpp` | `libParticles` |
| `CAP::EventProcessorPair<H,DH,Bf,SH>` | `Particles/EventProcessorPair.hpp` | `libParticles` |
| `CAP::EventIterator` | `Particles/EventIterator.{cpp,hpp}` | `libParticles` |
| `CAP::PythiaEventGenerator` | `CAPPythia/PythiaEventGenerator.{cpp,hpp}` | `libCAPPythia` |
| `CAP::ParticleSingleAnalyzer` | `ParticleSingle/ParticleSingleAnalyzer.{cpp,hpp}` | `libParticleSingle` |
| `CAP::ParticlePairAnalyzer` | `ParticlePair/ParticlePairAnalyzer.{cpp,hpp}` | `libParticlePair` |
| `CAP::ParticlePair3DAnalyzer` | `ParticlePair3D/ParticlePair3DAnalyzer.{cpp,hpp}` | `libParticlePair3D` |
| `CAP::GlobalAnalyzer` | `Global/GlobalAnalyzer.{cpp,hpp}` | `libGlobal` |
| `CAP::SpherocityAnalyzer` | `Spherocity/SpherocityAnalyzer.{cpp,hpp}` | `libSpherocity` |
| `CAP::PtPtAnalyzer` | `PtPt/PtPtAnalyzer.{cpp,hpp}` | `libPtPt` |
| `CAP::NuDynAnalyzer` | `NuDyn/NuDynAnalyzer.{cpp,hpp}` | `libNuDyn` |
| `CAP::PerformanceAnalyzer` | `Performance/PerformanceAnalyzer.{cpp,hpp}` | `libPerformance` |
| `CAP::SubSampleStatCalculator` | `SubSample/SubSampleStatCalculator.{cpp,hpp}` | `libSubSample` |
| `CAP::Therminator3` (and friends) | `Therminator3/...` | `libTherminator3` |
| `CAP::GlauberGenerator` (and friends) | `Glauber/...` | `libGlauber` |
| `CAP::BasicEventGen` | `BasicEventGen/BasicEventGen.{cpp,hpp}` | `libBasicEventGen` |

---

## Missing — classes referenced by .ini files but not in `src/`

### Top-level orchestrator

#### `CAP::RunAnalysis`

- **Role**: Top-level container task. Holds a list of subtasks; on `execute()`
  walks each subtask once. Has no analysis logic of its own.
- **Likely body**: Trivial — basically `class RunAnalysis : public Task {}`
  with a `ClassDef` macro and a `ClassImp` line.
- **Estimated size**: 30–50 lines C++.
- **Risk**: Low. Could even be done by aliasing to `Task` in the source-side
  fallback (in the spirit of the existing back-compat) — if `TClass::GetClass`
  returns null for `CAP::RunAnalysis`, fall through to using `CAP::Task`.

### Particle-database loaders

#### `CAP::ParticleTypeTask` and `CAP::ParticleDbTask`

- **Role**: Build a `ParticleDb` from on-disk data (`DB/` directory) before
  the event loop. Owns one or more `ParticleDb` instances, registers them in
  `EventProcessor::_managedParticleDbs` for downstream tasks to pick up.
- **Inputs from .ini**: `nParticleDbs`, `ParticleDbName<k>`, `ParticleDbOwner<k>`,
  plus filter/exclude flags like `DbDisableNeutralParticles`, `DbDisableWeakDecays`.
- **Code already exists for the heavy lifting**: `ParticleDb`, `ParticleDbParser`,
  `ParticleDbXmlWriter` are all in `src/Particles/`. The Task is just a thin
  wrapper that calls those.
- **Estimated size**: 100–200 lines.

### Filter creators (parse .ini blocks → Filter objects)

#### `CAP::EventFilterCreator`

- **Role**: Read `EventFilterCreator:EventFilter:N` and `EventFilterCreator:EventFilter:Filter<k>:*`
  blocks from the .ini, build `EventFilter` objects, register them.
- **Inputs from .ini**: per-filter Name, Title, nConditions, and per-condition
  Type/Subtype/Minimum/Maximum.
- **Code already exists for**: `EventFilter`, `Condition`, `ConditionDoubleRange`, etc.
  This task just walks the .ini configuration and constructs them.
- **Estimated size**: 150–250 lines.

#### `CAP::ParticleFilterCreator`

- Identical pattern to `EventFilterCreator`, but for `ParticleFilter`.
- **Estimated size**: 150–250 lines.

#### `CAP::FilterCreator`

- Combined version that creates Event, Particle, *and* Jet filters in one
  pass. Used by some `RunBf`-style configs.
- **Estimated size**: 200–300 lines.

### Loop drivers

#### `CAP::FileIterator`

- **Role**: Post-processing loop driver. Iterates over a list of input
  histogram `.root` files (instead of events) and runs `*Calculator` subtasks
  on each.
- **Inputs from .ini**: `nLevels`, plus per-level path patterns.
- **Estimated size**: 100–200 lines.

### Post-processing calculators

These are the **hardest to stub** because they encode actual physics.

#### `CAP::ParticleSingleCalculator`

- **Role**: Read a `*Histos.root` file produced by `ParticleSingleAnalyzer`,
  compute derived histograms (yields, normalised pT, …), write them out.
- **Estimated size**: 300+ lines, requires understanding of which derived
  observables are wanted.

#### `CAP::ParticlePairCalculator`

- **Role**: Same pattern, for pair correlation observables.
- **Estimated size**: 500+ lines, includes balance-function combinations
  if not split into a separate `ParticlePairBfCalculator`.

#### `CAP::ParticlePair3DCalculator`

- **Role**: Same, for 3-D HBT correlations (Qinv / side / out / long).
- **Estimated size**: 500+ lines.

#### `CAP::ParticlePair3DBfCalculator`

- **Role**: Reads the derived 3-D histograms and computes balance functions
  in 3-D space.
- **Estimated size**: 200–400 lines.

#### `CAP::GlobalCalculator`

- **Role**: Post-processes `GlobalAnalyzer` output (event-level cumulants).
- **Estimated size**: 200 lines.

---

## Other missing classes (referenced by some .ini files but unclear)

| Class | Referenced in | Notes |
|---|---|---|
| `CAP::ParticleDecayerTask` | `RA.ini`, `RAP.ini` | Decays unstable particles using `ParticleDb`. Worker class `ParticleDecayer` exists; this is just a Task wrapper. |
| `CAP::EventEfficiencyFilterCreator` | `RAP.ini` | Builds `EventEfficiency` filters |
| `CAP::ParticleEfficiencyFilterCreator` | `RAP.ini` | Builds `ParticleEfficiency` filters |
| `CAP::ParticleSingleAnalyzerGEN` / `…REC` | `RAP.ini` | Analyzers with explicit GEN/REC suffixes — possibly just renamed copies of `ParticleSingleAnalyzer`. |

---

## Recommended order if implementing

1. **`RunAnalysis`** — trivial, unblocks the entire .ini load path.
2. **`ParticleTypeTask`** — needed for any analysis that reads the particle DB.
3. **`EventFilterCreator` + `ParticleFilterCreator`** — needed for any
   non-trivial event/particle selection.
4. **`FileIterator`** — needed for the post-processing pass (`RunDerived`).
5. The **Calculator** classes — biggest payoff but hardest, requires physics
   knowledge.

After step 3, the basic Pythia generator → analyzer pipeline should be
runnable end-to-end (no derived histograms, but `SingleGen.root`,
`PairGen.root`, `Pair3DGen.root` would be produced).

---

## Balance-Function antiparticle convention (read before wiring real BF)

`BalanceFunctionCalculator.cpp` (still un-compiled — see above) determines
the antiparticle of a given particle filter **purely by index offset**, not
by PDG ID, name pattern, or `ParticleDb` lookup.  The relevant lines (511,
596 in the commented-out original `execute()` body):

```cpp
unsigned int nSpecies = particleFilters.size() / 2;
String pn1Bar = (particleFilter1 + nSpecies)->name();
String pn2Bar = (particleFilter2 + nSpecies)->name();
```

**Convention:** if the `.ini` lists `N` particle filters, the framework
assumes the first `N/2` are particles and the next `N/2` are their
antiparticles in matching species order.  Anti-of-`filter[i]` is
`filter[i + N/2]`.

**Required ordering** (matches shipped `RunAna3D.ini`):

```
ParticleFilterName0  = KP        ← particle  (idx 0)
ParticleFilterName1  = K0P
...
ParticleFilterName5  = OP
ParticleFilterName6  = KM        ← anti of KP  (idx 0 + 6)
ParticleFilterName7  = K0M       ← anti of K0P (idx 1 + 6)
...
ParticleFilterName11 = OM
```

**Forbidden ordering** (interleaved):

```
ParticleFilterName0  = KP
ParticleFilterName1  = KM        ← BREAKS BF: anti math says KP↔LaP
ParticleFilterName2  = K0P
...
```

`analyses/builder/cap_ini_builder.py::default_particle_filters()` was
re-ordered from `[PiP, PiM, KP, KM, PP, PM, ALL]` to
`[PiP, KP, PP, PiM, KM, PM, ALL]` to honor this convention.  The
catch-all `ALL` filter sits at the end; pair-style analyzers drop it
before computing pairs, so it never enters the BF arithmetic.

**Caveat for whoever picks up real BF:**  the active `execute()` body in
`BalanceFunctionCalculator.cpp` is empty — the orchestrator loop that
walks `(eventFilter × particleFilter1 × particleFilter2)` and feeds the
four pair histograms into `calculate_BalFct(...)` was never finished by
Pruneau (only the utility methods `calculate_CI`, `calculate_CD`,
`calculate_BalFct`, `calculate_BalFctSum`, `calculate_Diff` are
implemented).  When wiring a real `ParticlePairBfCalculator`, the
orchestrator needs to be reconstructed from the commented-out block in
`execute()` (lines 422-640).

---

## Quick alternative: bypass the missing pieces

For testing purposes, somebody could write a minimal `.ini` that uses only
classes that exist:

- `CAP::EventIterator` — driver
- `CAP::PythiaEventGenerator` — input
- `CAP::ParticleSingleAnalyzer` — output

…with `EventFilter` and `ParticleFilter` instances constructed directly in
code (or via a tiny inline `FilterCreator` stub) rather than read from the
.ini. That would exercise the analysis path without needing to write all the
missing orchestrator classes.

---

## Build / install infrastructure (works, no action needed)

The work in this branch produced the following durable improvements,
independent of the missing-class issue:

- Portable `SetupCAP.sh` (autodetects deps; supports brew, conda, CVMFS, modules, taps)
- Modern `src/CMakeLists.txt` with `option(CAP_ENABLE_*)` flags
- `cmake/FindPythia8.cmake`, `cmake/FindFastJet.cmake` — proper CMake find modules
- Top-level `CMakeLists.txt` so `cmake -S . -B build` works from repo root
- `setup-cap` (CLI installer) and `setup-cap-gui` (Tk installer) with shared logging
- `run-cap` (Tk runner GUI) with project / .ini / task drop-downs
- `./install` launcher with `--gui` / `--cli` / `--headless` / `--run` modes
- Fixed numerous bit-rot bugs in `PythiaEventGenerator.cpp`, `GlauberHistos.cpp`,
  `Jets/*` (re-disabled — see `JETS_KNOWN_ISSUES.md`)
- Backward-compat in `Task.cpp` for the legacy `:TaskClassName` / `:nSubtasks` /
  `:Subtask<k>:TaskName` / `:Severity` keys (so the task tree loads from
  current shipped .ini files)
- Auto-relocation of misplaced install artifacts
- Comprehensive logs in `logs/` for every action

These are independent of the missing-class issue and benefit any future work.
