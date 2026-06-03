# Resume / checkpoint design (from Session B)

Goal: an 8-hour run that dies at hour 5 (power loss, OS shutdown, app closed)
must not waste the completed compute.  Only `provenance-study` (event
generation + histogramming) is expensive; plotting/report are seconds.

A job is a set of **units**, each = one `provenance-study` process writing one
`<out>.root` (+ `.root.txt`).  Resume splits along that boundary.

## Layer 1 — unit-level resume  (DONE, Session B, no engine change)

Files: `gui/provenance_gui/resume.py` (+ pipeline/app wiring).

- **Done-signal:** `provenance-study` writes its summary `.root.txt` *after*
  closing the ROOT file, so a unit is finished iff its `.root.txt` exists.  A
  killed unit has no `.txt` → it re-runs; a finished one is skipped.  No
  fragile mid-write detection.
- **Manifest:** `<outdir>/.cap-run-state.json` stores a config **fingerprint**
  (hash of events/ecm/seed/species/process/acceptance/generators/pythia/herwig/
  ladder_rungs) + the planned unit list.  Written atomically (temp + os.replace)
  so a power cut never corrupts it.
- **Resume rule:** on Run, if a manifest with the SAME fingerprint exists,
  units whose `.root.txt` is present are skipped; the rest run.  Different
  fingerprint ⇒ different job ⇒ nothing reused (outputs would be wrong).
- **Covers:** comparison jobs (compare/pythia.root, compare/herwig.root) and
  the stage-level case (study finished but a later stage failed/closed → rerun
  skips the 8-hour study, just re-plots/reports).  Deterministic: each unit is
  independent and seeded.
- Verified with unit + end-to-end logic tests.

What Layer 1 does NOT cover: a single monolithic `provenance-study` killed
*mid-generation* (one unit, half its events done).  That needs Layer 2.

## Layer 2 — within-run resume  (Session A, engine)

For a single long run, `provenance-study` must checkpoint mid-stream:

1. `--checkpoint FILE [--checkpoint-every N]` — every N events, **atomically**
   (temp + rename) snapshot: partial histograms + events-done + the **Pythia
   RNG state** (`pythia.rndm.dumpState`).  Always a complete previous snapshot.
2. `--resume FILE` — load the snapshot, restore the RNG, continue filling the
   same histograms up to the target event count, then write the final `.root`
   as usual.

Histograms are additive and Pythia's RNG state is serialisable, so a resumed
run is bit-identical to an uninterrupted one.  Cadence N is the safety/I-O
trade-off knob.

The GUI is ready to drive this: it can pass `--checkpoint compare/<gen>.ckpt`
and, when a unit's `.ckpt` exists but its `.root.txt` doesn't (killed
mid-run), pass `--resume`.  Wiring that is a one-line pipeline change once the
flags exist.

## Layer 2b — ladder tool

`cap-mechanism-ladder` runs all rungs in one process.  Add `--resume` so it
skips rungs whose `<rung>.root` already exists.  (Or let the GUI drive rungs
itself as units, reusing Layer 1.)

## `[B] needs` (filed in COORD.md)

- provenance-study: `--checkpoint FILE [--checkpoint-every N]` + `--resume FILE`
  (atomic snapshot of histograms + events-done + Pythia RNG; resume to target).
- cap-mechanism-ladder: `--resume` (skip rungs whose .root exists).
