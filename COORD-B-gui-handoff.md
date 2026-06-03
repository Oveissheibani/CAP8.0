# GUI modularity — handoff spec (from Session B)

Session B owns the plotter + report; the GUI is **Session A's** slice. At the
user's request, B investigated the GUI and prototyped the genealogy view but
made **no edits** to any `gui/` file. This doc is the handoff: concrete
touch-points, real config keys, and the two engine flags B needs from A.

The user's "modular" goal, restated: let the run be *composed* — ladder driven
by the Pythia choices, user-chosen species pairs (compute is too big to do all),
a configurable definition of "genealogy" (how far back / which ancestors), and a
single clear tree picture of the whole event-generator stack.

---

## 1. Ladder driven by the Pythia panel  (GUI-only — `pipeline.py`)

**Now:** `build_commands()` hardcodes the rungs and the Ladder panel has four
fixed checkboxes:

```python
rung_specs = {
    "shower":             ["ISR", "FSR"],
    "shower+MPI":         ["ISR", "FSR", "MPI"],
    "shower+MPI+CR":      ["ISR", "FSR", "MPI", "CR"],
    "shower+MPI+CR+rope": ["ISR", "FSR", "MPI", "CR", "rope"],
}
```

**Change:** derive the cumulative rungs from the mechanisms the user actually
selected in the Pythia tab (`state["pythia"]["panels"]`). Real toggle keys
(from `analyses/builder/generator_presets.py`, `PYTHIA_BOOL_TOGGLES`):

- Colour reconnection: `cr_qcd_mode1`, `cr_qcd_mode2` (and `cr_off`)
- Ropes: `ropes_on`, `shoving_on`
- Shower/MPI debug-off switches: `no_mpi`, `no_isr`, `no_fsr`

Proposed logic (mechanism is "in play" unless its debug-off switch is set;
CR/rope rungs appended only if the user enabled them):

```python
def ladder_rungs_from_pythia(pythia_panels: dict) -> dict[str, list[str]]:
    p = pythia_panels
    base = [m for m, off in (("ISR","no_isr"), ("FSR","no_fsr"), ("MPI","no_mpi"))
            if not p.get(off)]
    rungs = {"shower": [m for m in base if m in ("ISR","FSR")]}
    if "MPI" in base:
        rungs["shower+MPI"] = base[:]
    if p.get("cr_qcd_mode1") or p.get("cr_qcd_mode2"):
        rungs["shower+MPI+CR"] = base + ["CR"]
    if p.get("ropes_on") or p.get("shoving_on"):
        rungs["...+rope"] = base + ["CR", "rope"]
    return rungs
```

`build_commands()` already materialises `ladder.txt` from a rung dict, so this
is a drop-in replacement for the literal `rung_specs`. No engine change.

---

## 2. Species + pair picker  (Run tab — needs an engine flag)

- Keep the existing top inputs (energy / events / seed / acceptance) — they're
  already clean. Add beside them: the species list (already flows verbatim as
  `--species 211,321,2212`) plus a pair grid. N species → N(N+1)/2 unordered
  pairs; the user ticks which to include.
- New state key: `state["pairs"] = ["211x321", "211x2212", ...]`.
- `pipeline.py` passes them to `provenance-study` via a **new** `--pairs` flag.

**Engine dependency (Session A):** `provenance-study` currently computes *every*
pair implied by `--species`. A GUI/plotter filter would still pay the full
O(pairs) cost. To actually save compute the engine must compute only the
requested pairs — see flag (A) below. The plotter and report already adapt to
whatever pair histograms exist in the ROOT file, so nothing downstream changes.

---

## 3. Genealogy boundary config  (display now; compute later)

Two independent layers — the prototype B showed implements the display layer:

- **Display (front-end, doable today, `explorer.py`):** add a "track ancestry
  back to `<stage>`" selector + per-layer include toggles (MPI / ISR / FSR) next
  to the existing depth slider. Filter the drawn DAG client-side: a stage is
  visible iff `stageIndex >= boundary` AND its layer toggle is on; connectors
  bridge to the nearest *visible* ancestor. (Exact algorithm = the prototype's
  `stageVisible()` / `visAncestor()`.)
- **Compute (engine, Session A):** to make the boundary actually limit how far
  the `ProvenanceTagger` walks (and therefore which tags it records), add flag
  (B) below. Until then the boundary is a *view* filter only.

---

## 4. Genealogy tree visualization  (GUI — `explorer.py`)

Replace the current canvas draw with the data-driven layered layout from the
prototype. Most pieces already exist in `explorer.py`:

- Columns = `STAGE_ORDER` (already defined, mirrors `StageTaxonomy.hpp`).
- Node fill = `STAGE_COLOUR` (already defined).
- Connectors routed to the nearest *visible* ancestor (so hidden layers bridge).
- Studied hadron ringed; event nodes that aren't ancestors drawn faded.
- Driven by the #3 boundary + layer toggles; data from the `--dump-events` JSON
  the Explorer already loads.

Goal is legibility, not new data — "one picture of the whole generator stack."

---

## Engine flags Session B needs from Session A

(A) `provenance-study --pairs <a>x<b>[,<c>x<d>...]` — compute only the listed
    species-pairs instead of all N(N+1)/2. Enables the pair picker (#2).

(B) `provenance-study --genealogy-stop <Stage>` (or `--genealogy-depth N`) —
    bound how far back the `ProvenanceTagger` walks ancestry. Enables the
    compute side of the genealogy boundary (#3).

Both live in `provenance-study.cpp` / `ProvenanceTagger` / `PairProvenanceObservables`,
all Session A. Filed as `[B] needs:` lines in `COORD.md`.
