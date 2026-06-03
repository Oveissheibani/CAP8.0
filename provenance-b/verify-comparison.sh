#!/usr/bin/env bash
# Cross-generator comparison smoke test (Session B).
# Builds provenance-report, generates a Pythia baseline matched to the existing
# Herwig baseline, runs the plotter in --compare mode, then builds the ONE
# comparison report (genealogy + pair tables for both generators, overlay
# figures).  Run from the repo root:  bash provenance-b/verify-comparison.sh
set -e
cd "$(dirname "$0")/.."          # -> repo root (CAP8.0-main)
ROOT_DIR="$(pwd)"
CMP="provenance-b/compare"
PLOTS="provenance-b/cmp-plots"
REPORTS="provenance-b/cmp-report"
LOGS="provenance-b/logs"
mkdir -p "$PLOTS" "$REPORTS" "$LOGS"

echo "=== [1/5] build provenance-study + provenance-report (build-B) ==="
# ProvenanceObservables changed (initiating-parton histos + summary), so BOTH
# binaries must rebuild.
cmake --build build-B --target provenance-study provenance-report \
  2>&1 | tee "$LOGS/cmp-build.log" | tail -10
STUDY=build-B/src/EventHistory/provenance-study
REPORT=build-B/src/EventHistory/provenance-report
[ -x "$STUDY" ]  || STUDY=bin/provenance-study
[ -x "$REPORT" ] || REPORT=bin/provenance-report

echo "=== [2/5] Pythia baseline (MPI off, K0S stable, 211) -> $CMP/pythia.root ==="
"$STUDY" --events 2000 --species 211 --ecm 13000 --seed 12345 \
  --config "$CMP/pythia_baseline.cmnd" --out "$CMP/pythia.root" \
  2>&1 | tee "$LOGS/cmp-pythia.log" | tail -6

echo "=== [3/5] regenerate Herwig root from cached .hepmc (new histos) -> $CMP/herwig.root ==="
# Reuse the existing herwig_baseline.hepmc; just re-run the study so the new
# pt_initparton_* histograms + summary section are present.
HEPMC="provenance-b/herwig/herwig_baseline.hepmc"
if [ -f "$HEPMC" ]; then
  "$STUDY" --events 2000 --species 211 --ecm 13000 --seed 12345 \
    --hepmc3 "$HEPMC" --out "$CMP/herwig.root" \
    2>&1 | tee "$LOGS/cmp-herwig.log" | tail -6
else
  echo "  ! $HEPMC missing — falling back to stale herwig_baseline.root (no initparton)"
  cp -f "$CMP/herwig_baseline.root"     "$CMP/herwig.root"
  cp -f "$CMP/herwig_baseline.root.txt" "$CMP/herwig.root.txt"
fi
ls -la "$CMP"/pythia.root "$CMP"/herwig.root

echo "=== [4/5] plotter --compare (every figure overlays both) ==="
# Stage EXACTLY the two canonical roots into a clean dir so the *.root glob in
# discover_generators can't pick up stale leftovers (e.g. an old
# herwig_baseline.root from an earlier smoke test that lacks the newest
# histograms — that would make require_all skip the new figures).
CMP2="$CMP/_twogen"
rm -rf "$CMP2"; mkdir -p "$CMP2"
for g in pythia herwig; do
  ln -sf "$(pwd)/$CMP/$g.root"     "$CMP2/$g.root"
  ln -sf "$(pwd)/$CMP/$g.root.txt" "$CMP2/$g.root.txt"
done
./cap-provenance-plot --compare "$CMP2" --outdir "$PLOTS" \
  2>&1 | tee "$LOGS/cmp-plot.log" | tail -18
echo "compare figures produced:"; ls "$PLOTS"/compare_*.pdf 2>/dev/null | wc -l
echo "initparton figures:"; ls "$PLOTS"/compare_pt_initparton_*.pdf 2>/dev/null | sed 's#.*/##'

echo "=== [5/6] INDIVIDUAL per-generator reports (single --summary each) ==="
# Each generator gets its OWN report with its OWN single-run decomposition
# figures (FIGURES set, incl. initparton_pt) and both parton tables
# (string-endpoint + initiating).  These are separate from the comparison.
for g in pythia herwig; do
  gp="provenance-b/${g}-plots"
  glabel=$([ "$g" = pythia ] && echo "Pythia 8" || echo "Herwig 7")
  echo "--- $g individual report ---"
  ./cap-provenance-plot --root "$CMP/$g.root" --outdir "$gp" \
    2>&1 | tee "$LOGS/ind-$g-plot.log" | tail -3
  "$REPORT" --summary "$CMP/$g.root.txt" --figures "$gp" \
    --title "Provenance Decomposition of Pion Production -- $glabel" \
    --outdir "$REPORTS" --out "provenance-$g" --pdf \
    2>&1 | tee "$LOGS/ind-$g-report.log" | tail -3
done

echo "=== [6/6] ONE comparison report (both summaries) ==="
"$REPORT" --summary "$CMP/pythia.root.txt" --summary2 "$CMP/herwig.root.txt" \
  --label1 "Pythia 8" --label2 "Herwig 7" \
  --figures "$PLOTS" --outdir "$REPORTS" --out provenance-comparison --pdf \
  2>&1 | tee "$LOGS/cmp-report.log" | tail -12

echo
echo "=== INITIATING-PARTON (gluon-capable) fractions — the fix ==="
for g in pythia herwig; do
  echo "--- $g ---"
  awk '/by initiating parton flavour:/{f=1;next} f&&/%/{print "  "$0} f&&/^[A-Za-z]/&&!/%/{f=0}' \
    "$CMP/$g.root.txt" 2>/dev/null | head -6
done

echo
echo "=== RESULT — three reports ==="
ls -la "$REPORTS"/provenance-pythia.pdf \
       "$REPORTS"/provenance-herwig.pdf \
       "$REPORTS"/provenance-comparison.pdf 2>/dev/null
echo "individual : $REPORTS/provenance-pythia.pdf , $REPORTS/provenance-herwig.pdf"
echo "comparison : $REPORTS/provenance-comparison.pdf"
