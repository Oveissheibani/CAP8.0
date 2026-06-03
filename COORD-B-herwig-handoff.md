# Herwig support & Pythia-vs-Herwig comparison — handoff (from Session B)

Goal (from the user): run the provenance study with **both Pythia and Herwig**,
and make **generator-vs-generator comparison** a first-class output.

## Key architectural fact

The provenance machinery is already generator-agnostic:

    <generator> → EventHistory DAG → ProvenanceTagger → observables

There are two DAG builders feeding the tagger:
- `PythiaHistoryBuilder`  (used by provenance-study today)
- `HepMC3HistoryBuilder`  (exists; built when `CAP_ENABLE_HEPMC3=ON`)

Herwig 7 emits HepMC3, so the clean route is **Herwig → HepMC3 →
`HepMC3HistoryBuilder` → same tagger/observables**.  Nothing in the tagger or
the observables needs to change.

## What Session B already did (GUI side, done)

- `panels/genpanel.py` — generic generator config panel (data-driven clone of
  the Pythia panel).
- `panels/herwig.py` — `HerwigPanel` using the existing `HERWIG_PRESETS /
  HERWIG_BOOL_TOGGLES / HERWIG_NUMERIC_KNOBS / collect_herwig_lines` from
  `analyses/builder/generator_presets.py`.  New "Herwig" tab.
- `panels/generators.py` — generator selector (Pythia / Herwig / both =
  comparison), writes `state['generators']`.
- `pipeline.py` — `materialise_herwig_in()` writes a `herwig.in` deck when
  Herwig is selected (ready for the engine; does not change today's Pythia run).
- The plotter's `overlay_ladder` is already generator-agnostic, so a comparison
  is just two `.root` files (one per generator) overlaid — no plotter change
  needed beyond per-curve labelling (B will add generator labels).

## What Session A needs to add (engine)

1. **`provenance-study --hepmc3 FILE`** — read a HepMC3 event file and build the
   EventHistory via the existing `HepMC3HistoryBuilder` instead of generating
   with Pythia.  Build with `CAP_ENABLE_HEPMC3=ON` (HepMC3HistoryBuilder.cpp is
   already there).  Output the same `.root` + `.root.txt` so the plotter/report
   consume it unchanged.
   - Note: HepMC status codes are coarser than Pythia's; the tagger header
     already mentions HepMC paths, so confirm stage inference is sane for
     Herwig (cluster hadronization vs string).
   - A Herwig resonance resolver (analogue of `pythiaResonanceResolver`) or a
     PDG-list fallback for the `FromResonance` tag.

2. **Herwig run integration** — either:
   (a) provenance-study spawns `Herwig read`/`Herwig run` from a `.in` deck
       (the GUI already produces `configs/herwig.in`), then reads the `.hepmc`;
       run-cap already has the HepMC-output-block injection helper to reuse; or
   (b) leave generation to the user/run-cap and just consume the `.hepmc` via
       `--hepmc3`.  (b) is the smaller change.

3. **Generator comparison naming** — for comparison runs, the GUI will write
   one `.root` per generator into a shared dir (e.g. `compare/pythia.root`,
   `compare/herwig.root`); `cap-provenance-plot --ladder compare/` overlays
   them.  No engine change needed for this part — just confirm `.root.txt`
   carries a `generator=` token so the report can label curves.

4. **report**: `provenance-report.cpp` hard-codes "Pythia 8" in the abstract /
   method (buildPaper structural code).  Parameterise the generator name from
   the summary.  (B owns captions/section bodies; the abstract is structural —
   coordinate.)

## Suggested pipeline flow once (1)+(2) land

    for gen in selected_generators:
        if gen == pythia: provenance-study --config pythia.cmnd --out compare/pythia.root
        if gen == herwig: Herwig read/run herwig.in -> X.hepmc
                          provenance-study --hepmc3 X.hepmc --out compare/herwig.root
    cap-provenance-plot --ladder compare/   # overlays Pythia vs Herwig
