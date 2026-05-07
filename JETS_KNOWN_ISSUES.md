# Jets module — known issues

The `src/Jets/` module is **incomplete upstream code**. It does not compile and
is intentionally disabled by the CAP build system. This file lists every bug
identified during the build attempts so somebody can finish the implementation.

To attempt a build of Jets anyway (developer override):
```bash
cmake -S . -B build -D CAP_ENABLE_JETS=ON -D CAP_FORCE_BROKEN_JETS=ON
```

You will then meet the bugs listed below. They are not subtle bit-rot —
this code was never compilable.

---

## `src/Jets/JetAnalyzer.hpp`

| Line | Bug |
|------|-----|
| 22 | `EventProcessorSingle<JetSingleHistos,JetSingleDerivedHistos>` — class is named `JetSingleHistosDerived`, not `JetSingleDerivedHistos` |
| 23 | `public ManagedList<JetFilter>` — `ManagedList<>` template doesn't exist anywhere in the codebase |
| 38 | `std::vector<PseudoJet> clusteredJets = sorted_by_pt(clusterSequence->inclusive_jets(jetPtMin));` — uses an undeclared member `clusterSequence`, an undeclared member `jetPtMin`, and runs cluster code in a default member initializer (probably wanted to be done in `execute()`) |

Missing includes: the file uses `EventProcessorSingle`, `JetSingleHistos`,
`JetSingleHistosDerived`, `PseudoJet`, `sorted_by_pt`, but only includes
`EventProcessor.hpp` and `JetFilter.hpp`.

## `src/Jets/JetAnalyzer.cpp`

| Line | Bug |
|------|-----|
| 28, 39, 47, 56 | Same `JetSingleDerivedHistos` rename problem |
| 29, 40, 48, 107 | Uses non-existent `ManagedList<JetFilter>` |
| 59-61 | `addProperty(createKey(taskName,"load",false);` — missing closing `)` (3 sites) |
| 107 | `jetFilters = ManagedList<JetFilter>::getObjects();` — `jetFilters` member never declared |
| 108 | `nJetFilters() = jetFilters.size();` — assigning to a function-call expression |
| 109 | `Analyzer::initialize();` — `Analyzer` is not the class's base, `EventProcessorSingle<…>` is |
| 114 | References `taskName` which is not declared inside `create()` |
| 131, 137, 143 | `jetHistos->create(const Configuration & configuration);` — that's a parameter declaration syntax inside what should be a function call |
| 154-181 | Mismatched braces — extra `{` opens a block that's never closed inside the loop |
| 186, 187 | Local `clusterSequence` and `clusteredJets` shadow the broken member declarations from the header |
| 191 | `iEventFilter` redeclared (already in scope from line 192's loop) |
| 218 | `;;` — harmless double semicolon |
| 275 | `bool JetAnalyzer::filterJets(...)` — defined but not declared in `JetAnalyzer.hpp` |
| 287 | `pseudoJetsInput.push_back(pseudoJet);` — references the header's broken member, not a local |
| 289 | `acceptedParticleFilters[iParticleFilter]` and `iParticleFilter` — both undeclared in this scope |

## `src/Jets/JetHistosDerived.hpp`

| Line | Bug |
|------|-----|
| 45 | `virtual void cloneD(onst JetHistosDerived & source);` — typo, missing `c` (should be `const`) |

## What's needed to fix Jets

A real implementation pass — not patch-level fixes. Specifically:

1. **Define a member layout for `JetAnalyzer`** that includes the per-event
   pseudo-jet input vector, the FastJet cluster sequence, the clustered jets,
   and the jet-filter list. Decide where each is constructed (likely all in
   `execute()`, none in default member initializers).

2. **Replace `ManagedList<JetFilter>` everywhere** — either implement that
   template, or refactor to `std::vector<JetFilter*>` (matching how event /
   particle filters are managed elsewhere in CAP).

3. **Resolve the class-name confusion**: `JetSingleHistosDerived` vs
   `JetSingleDerivedHistos`. The files on disk use the former; pick one and
   apply it throughout.

4. **Repair the broken syntax** in `setDefaultConfiguration()` (missing close
   parens on `addProperty`), `create()` (parameter syntax in argument list),
   and `execute()` (mismatched braces).

5. **Wire up the per-event-filter / per-jet-filter loops correctly** — most of
   `execute()` is half-translated from another module's pattern. The variables
   `iEventFilter`, `iParticleFilter`, `acceptedParticleFilters` are referenced
   but never declared.

6. **Add a `JetAnalyzer::filterJets()` declaration** to the header to match the
   definition in the `.cpp`.

7. **Fix the `cloneD(onst …)` typo** in `JetHistosDerived.hpp`.

Once the file compiles, also test that filling a jet histogram via
`setParticleTypeList(db)` works — the `ParticleTypeList::list()` singleton was
removed (replaced by per-task `ParticleDb`). `JetHistos` and friends now take
a `ParticleDb*` initialised to `nullptr`; the analyzer must call
`histos->setParticleTypeList(&db())` before filling.

## Why CAP without Jets is still useful

The Jets module is a specialized add-on. Removing it does not affect:

- Pythia 8 generation (CAPPythia)
- Therminator 3 generation
- Glauber Monte Carlo
- Particle / event filtering (Particles)
- Single-particle distributions (ParticleSingle)
- Two- and three-particle correlations (ParticlePair, ParticlePair3D)
- Balance functions (ParticlePair3DBfCalculator)
- Flow / cumulants (ParticleFlow)
- Spherocity (Spherocity)
- pT–pT correlations (PtPt)
- Net-charge fluctuations (NuDyn)
- Detector smearing / closure tests (Performance)
- Sub-sample statistics (SubSample)

That covers every published correlation analysis pattern CAP was designed for.
The Jets module is an unfinished extension for jet-substructure work.
