# Plan: Balance Function at every collision stage

## Goal

Compute the balance function (BF) at each evolution stage of a pp/AA collision so we can study **how charge correlations propagate** through:

1. **Hard scatter** (status≈23, partonic)
2. **Parton shower** (status≈51,52)
3. **Hadronization, pre-decay** (status=2)
4. **Final state, post-decay** (status=1)

User must be in full control of which set the analyzers consume — same .ini, different keep filter, separate output folder, then plot the four BFs side by side and watch them evolve.

---

## What works today

- `histos/<Generator>/<KeepProfile>/<run>/` layout — different keep selections already land in different folders, so **stage isolation at the file-system level is solved**.
- `SaveFinalOnly / SaveQuarks / SaveNeutrinos / SavePhotons / SaveGaugeBosons` keys propagate from the GUI into the .ini and into all three generators (Pythia, Herwig, HepMC3).
- **Default keep set (final-state hadrons + leptons) runs cleanly through all stages 1/2/3** for both Pythia and Herwig→HepMC3.

## What blocks "BF at every stage" today

Three issues, in increasing difficulty:

### A. Particle DB doesn't have non-hadron entries

`DB/ParticleData/particles.data` only contains stable hadrons + a handful of leptons/photons. When `findPdgCode(pdg)` is called for:

- partons (PDG 1..9, gluon=21)
- gauge bosons (PDG 23, 24, 25, 32, 33, 34, 37)
- exotic / SUSY (everything > 1e6)

it throws. Today's generators catch the throw and skip the particle — but that means even with `SaveQuarks=1` no quark survives because they're all silently dropped at DB lookup.

**Fix:** add a `partons.data` (or extend `particles.data`) with synthetic entries for u/d/s/c/b/t (charge ±1/3 ±2/3, masses), gluon (charge 0, mass 0), W±/Z/H/γ if not already there. ~50 lines of plain-text data.

### B. Some analyzers assume hadron-like properties

Audit needed; concretely:

- **SpherocityAnalyzer** — sums pT vectors. Fine for any particle.
- **NuDynAnalyzer** — counts net charge per filter. Quarks have ±1/3, ±2/3 — works if filters accept them.
- **PtPtAnalyzer** — sums of pT pairs. Fine.
- **GlobalAnalyzer** — n / E / q / s / b totals. Fine numerically; q for partons is fractional so the `q_nbins` histogram needs wider bins.
- **ParticleSingleAnalyzer** — fills h_n1_pt etc. Fine.
- **ParticlePairAnalyzer / ParticlePair3DAnalyzer** — pair correlations. Fine.

The crash you saw with **all 5 keep flags ON** is most likely:

- Either Pythia generator-side: when `SaveFinalOnly=1` AND `SaveQuarks=1`, the post-final-only filter (`continue` if `!isFinal()`) still drops all partons — so `SaveQuarks=1` is a no-op in that combination. The crash is probably from a different keep flag interacting with a downstream histo (like Spherocity dividing by 0 if zero accepted particles in a filter).
- Or the factory pool growing past 5000 particles during a parton-shower event with all kept and analyzers running concurrently.

Concrete defensive measures:

```cpp
// Top of every Analyzer::execute() particle loop:
for (auto * p : particles) {
    if (!p) continue;
    if (!p->getType()) continue;          // ← survive missing-DB particles
    const double pt = p->getMomentum().Pt();
    if (!std::isfinite(pt) || pt <= 0) continue;
    ...
}
```

### C. ParticleFilter logic might pre-reject everything

The shipped filters in run-cap (PiP / KP / PP / PiM / KM / PM / ALL) match by PDG code. They REJECT partons / gauge bosons. So even if Pythia keeps them, the analyzer sees zero accepted in the per-species filter, the `h_n1` histo for that filter is empty for that event, and a downstream `Pair / N1*N1` calculation may divide by zero.

**Fix:** add a parallel set of "stage" filters:
- `PARTON_Q+` — PDG ∈ {1,2,3,4,5,6} (positively-charged partons)
- `PARTON_Q-` — PDG ∈ {-1,-2,-3,-4,-5,-6}
- `PARTON_GLUON` — PDG = 21
- `INTERMEDIATE_HADRON_*` — status==2 + species
- `FINAL_HADRON_*` — what we have now (default)

The user picks stage filters for stage-N analyses, hadron filters for stage-1 analyses.

---

## Sequenced work plan

I'd land these in order:

| Phase | Deliverable | Cost | Risk |
|---|---|---|---|
| 1 | **Defensive guards** (`if (!type) continue;` / NaN-pT skip) in every Analyzer::execute() | ~80 lines C++ | low — backward compatible |
| 2 | **Extend particles.data** with partons + gauge bosons + gluon | ~60 lines data + tests | low — additive |
| 3 | **Stage filters** in run-cap default filter set (PARTON_*, INTERMEDIATE_*, FINAL_*) | ~40 lines Python | low |
| 4 | **Status-code filter** in `HepMC3EventReader` (lift `status != 1` into a list) | ~20 lines C++ | low |
| 5 | **GUI: per-stage analysis panel** — checkbox per stage, generates one .ini per stage with the right filters + KeepProfile | ~100 lines Python | medium — UX design |
| 6 | **Plot helper** that loads the N stage .root files and overlays the BF curves | ~80 lines Python | low |
| 7 | (Optional, depends on HERWIG agent) **Push `WriteStatus` down into ThePEG** so on-disk file is smaller per stage | their work | their effort |

After phase 1+2+3, you can already run:
```
keep flags = final + quarks + photons + neutrinos + gauge_bosons → ALL particles
filter set = PARTON_Q+, PARTON_Q-, GLUON
→ output: histos/Pythia/all_status+q+n+g+b/myrun_partons/
```
and CAP shouldn't crash. Then the same generator output can be re-analyzed with the FINAL_HADRON filter set:
```
filter set = PiP, KP, PP, PiM, KM, PM
→ output: histos/Pythia/all_status+q+n+g+b/myrun_finals/
```

Pair-by-stage plot is then four CAP runs over the same .hepmc / Pythia stream.

---

## Recommendation for next concrete step

Start with **Phase 1 (defensive guards)** — it's cheap, low-risk, and unblocks the
"all flags on" workflow you just attempted without the SIGABRT. I can do this now if you want, then move on to Phase 2 (DB extension) and Phase 3 (stage filters).

Phase 7 (HERWIG-side WriteStatus) is documented in `LETTER_TO_HERWIG_AGENT_v2.md` — purely an optimization for grid-scale runs, doesn't block the core BF-per-stage capability.
