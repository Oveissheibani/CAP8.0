# WSU "warrior" deployment plan — provenance at scale

Goal: run the full provenance program (genealogy + comparison + the MPI/CR
mechanism ladder + fragmentation systems + entropy) on the Wayne State grid,
with (a) every ladder configuration as INDEPENDENT SLURM jobs, (b) events
generated in SUBSAMPLES (small chunks, distinct seeds), and (c) subsample
combination through CAP's OWN SubSample machinery so the merged results carry
proper subsample error bars.

Companion facts: `Grid/WSU/warrior_grid_inventory.md` (toolchain, prefixes,
SLURM), audited 2026-06-05.

---

## 0. Why this maps cleanly onto what already exists

| Need | Already exists |
|---|---|
| C++ standard on warrior (ROOT 6.28.10 = C++14) | whole provenance module is C++14 by design |
| Pythia on warrior | 8.317 built WITH HepMC3 + LHAPDF at `$H/pythia/PYTHIA8/install/pythia` |
| Herwig on warrior | 7.1.0 (HepMC2 output) — our `--hepmc3` input uses `HepMC3::deduce_reader`, which auto-reads HepMC2 ascii: NO Herwig rebuild needed |
| Independent-job resume | chunk = `.root.txt` completion marker (same mechanism as local runs) |
| Subsample layout | CAP's grid convention: NMAINJOBS x NSUBJOBS bunches writing one subdirectory per subjob (`Grid/WSU/BatchRun.sh`) |
| Subsample statistics | `src/SubSample/SubSampleStatCalculator` scans the subdirectories; variance math in `HistogramGroup::squareDifferenceLists` |
| Mechanism ladder decks | `mechanisms.py` rung configs are plain text; each (gen, rung, subsample) is one self-contained study invocation |
| Bonus | EPOS 4 is installed and writes HepMC3 — a THIRD generator column later, for free |

## 1. The job matrix

One SLURM array task = one (generator, rung, subsample):

```
generators : pythia, herwig            (later: epos)
rungs      : baseline, +MPI, +MPI+CR
subsamples : k = 0 .. K-1   (seed = BASE + k, ~25k-50k events each)
```

Directory layout (Panasas, one production folder):

```
$PROD/<gen>_<rung>/SUB00/provenance.root(.txt)
$PROD/<gen>_<rung>/SUB01/...
...
```

This is EXACTLY the layout SubSampleStatCalculator expects (it iterates the
subdirectories of an import path).  Resume/requeue safety: a task whose
SUB##/provenance.root.txt exists exits immediately.

Herwig tasks generate their own .hepmc INSIDE the job and delete it after the
study (the local auto-delete pattern), so only .root files persist —
important because the Panasas quota situation is ambiguous (47 GB vs du).

## 2. Phase plan

### Phase A — install + build (half a day; EVERYTHING via sbatch)
POLICY: nothing computes on the login node — clone is the only interactive
step; the build itself is an sbatch job.
```bash
ssh hx4574@<warrior>
H=/wsu/home/hx/hx45/hx4574
git clone <fork-url> $H/CAP8.0      # from the user's fork (pushed from the Mac)

cat > $H/CAP8.0/build-job.sh <<'EOS'
#!/bin/bash
#SBATCH --partition=mdtp --nodes=1 --ntasks=8 --time=01:00:00
#SBATCH --job-name=cap-build --output=%x-%j.log
module load gnu7/7.3.0 cmake/3.21.1 root/6.28.10 fastjet/3.4.0 gsl/2.5
export CC=$(which gcc) CXX=$(which g++) FC=$(which gfortran)   # system cc is 4.8.5!
H=/wsu/home/hx/hx45/hx4574
cd $H/CAP8.0
cmake -S . -B build-grid \
  -D CAP_ENABLE_PYTHIA=ON \
  -D CAP_PYTHIA8_PATH=$H/pythia/PYTHIA8/install/pythia \
  -D CAP_ENABLE_HEPMC3=ON \
  -D CAP_HEPMC3_PATH=$H/EPOS4/install/hepmc3
cmake --build build-grid -j8 --target EventHistory provenance-study provenance-report
EOS
sbatch $H/CAP8.0/build-job.sh
```
Checkpoints (in the job log): cmake reports "Pythia 8 8.317 located", HepMC3
found via its own config; binaries appear in build-grid/src/EventHistory/.

### Phase B — single-job smoke (one sbatch each)
```bash
# B1: Pythia path (5-min job)
sbatch --partition=mdtp --nodes=1 --ntasks=1 --time=00:20:00 --wrap "\
  module load gnu7/7.3.0 root/6.28.10 gsl/2.5 fastjet/3.4.0; \
  $H/CAP8.0/build-grid/src/EventHistory/provenance-study -n 2000 \
    --systems --entropy -o $H/smoke/pythia.root"
# B2: Herwig path — generate 2000 HepMC2 events with the EXISTING Herwig
#     (runtime env block from the inventory), then:
#     provenance-study --hepmc3 herwig.hepmc -o $H/smoke/herwig.root
```
Checkpoints: .root.txt summaries contain the genealogy + fragmentation +
entropy blocks; Herwig summary shows nonzero Primary fraction (validates the
HepMC2-through-deduce_reader path on 7.1.0 output).
KNOWN RISK to verify here: Herwig 7.1.0 deck syntax for the MPI-off rung
(`set /Herwig/Shower/ShowerHandler:MPIHandler NULL`) and the CR switch —
confirm both exist in 7.1.0's repository before trusting the ladder rungs.

### Phase C — the submitter (new, thin)
`provenance-b/wsu/submit-ladder.sh` + `job.sh` (to be written when we start):
- generates the (gen, rung, k) list, materialises rung decks via
  `mechanisms.py` output (checked into the production dir for provenance),
- submits ONE sbatch array per generator
  (`--array=0-<R*K-1>`, `--partition=mdtp`, 1 task each),
- job.sh: module loads + seed = BASE + k + the single provenance-study
  invocation + (Herwig only) generate-then-delete .hepmc,
- idempotent: re-submitting skips tasks whose .root.txt exists.

### Phase D — combine with CAP subsampling (the requirement)
Two-level combine per (gen, rung):
1. SUM for central values: `hadd merged.root SUB*/provenance.root` +
   `cap-merge-summaries` for the .txt (counts add; same as local chunks).
2. SUBSAMPLE ERRORS: SubSampleStatCalculator over the SUB## directories.
   HONEST STATUS: the Task wrapper in CAP 8.0 currently has its combine call
   commented out ("needs to be fixed" — squareDifferenceLists + export are
   disabled), while the underlying math `HistogramGroup::squareDifferenceLists`
   exists in src/Helpers.  ACTION ITEM before Phase D: re-enable + validate on
   toy subsamples (known-variance Poisson test), coordinating with [A] since
   src/SubSample is framework code.  Fallback if blocked: a small standalone
   subsample combiner over TH1s (mean + s/sqrt(K) per bin) — 100 lines,
   same directory convention, no framework edit.
Deliverables per (gen, rung): merged.root (+txt) AND subsample-errors.root.

### Phase E — reports
Plotting/report need ROOT (module, fine) + pdflatex (verify availability;
CentOS often has texlive).  Two options:
- on warrior: cap-provenance-plot + provenance-report directly on the merged
  outputs (one more sbatch);
- or rsync `$PROD/**/merged.root(.txt)` to the Mac (small files) and build
  reports locally with the exact commands we already use.
Either way the report pipeline is unchanged: per-generator individual
reports, the comparison report, and the N-column ladder report via
`--summaries`.

ALL-SBATCH POLICY (applies to every phase): builds, smokes, the array
production, the combine step, and on-grid plotting/reporting each run as
sbatch jobs; combine/report jobs declare dependencies with
`--dependency=afterok:<array-jobid>` so the whole chain can be submitted in
one sitting and walks itself.

### Phase F — production scale
With ~free CPUs the binding constraint is Herwig generation time per task:
- pythia: 3 rungs x 20 subsamples x 50k = 3M events  (~25 min/task)
- herwig: 3 rungs x 20 subsamples x 25k = 1.5M events (~1-2 h/task)
120 tasks total → an overnight, not a week.  10x our local statistics, with
real subsample error bars on every table — exactly what the paper needs for
the small classes (SameResonance at the 0.2% level) and the MI bias columns.

## 3. Risk register
1. Herwig 7.1.0 vs local 7.3 deck syntax (MPI-off / CR switches)   — Phase B2 check.
2. SubSampleStatCalculator combine disabled in 8.0                  — Phase D action item (+fallback).
3. Panasas quota ambiguity (47 GB?)                                 — hepmc delete-in-job; ask HPC admins.
4. pdflatex availability on warrior                                 — Phase E option 2 fallback.
5. Login-node etiquette                                             — all compute via sbatch, incl. smokes.
6. Pythia 8.317 vs local version differences in defaults            — record `Pythia::settings` in repro appendix (already done by the report).
