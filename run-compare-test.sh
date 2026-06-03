#!/usr/bin/env bash
# Pythia-vs-Herwig provenance smoke test (no GUI).
#
#   ./run-compare-test.sh                 # Pythia only (Herwig skipped)
#   HEPMC=/path/events.hepmc ./run-compare-test.sh   # Pythia + Herwig compare
#
# Knobs:  EVENTS=5000  SPECIES=211  OUT=provenance-b
#
# Produce the Herwig .hepmc with the main GUI (run-cap -> Compose ->
# Generator=Herwig) or via:  Herwig read your.in && Herwig run your.run -N N
set -uo pipefail
cd "$(dirname "$0")"
REPO="$PWD"
OUT="${OUT:-provenance-b}"
EVENTS="${EVENTS:-5000}"
SPECIES="${SPECIES:-211}"

# --- pick the freshest binaries (bin/ may be stale vs build-B/) -------------
pick() { local a="$1" b="$2"; if [ -f "$b" ] && { [ ! -f "$a" ] || [ "$b" -nt "$a" ]; }; then echo "$b"; else echo "$a"; fi; }
STUDY=$(pick  "$REPO/bin/provenance-study"  "$REPO/build-B/src/EventHistory/provenance-study")
REPORT=$(pick "$REPO/bin/provenance-report" "$REPO/build-B/src/EventHistory/provenance-report")
PLOT="$REPO/cap-provenance-plot"

mkdir -p "$OUT/compare" "$OUT/plots" "$OUT/reports" "$OUT/logs"
LOG="$OUT/logs/compare-test.log"
exec > >(tee "$LOG") 2>&1

echo "=== binaries ==="
echo "  study : $STUDY"
echo "  report: $REPORT"
if "$STUDY" --help 2>&1 | grep -q -- '--hepmc3'; then
  echo "  [ok] --hepmc3 supported (Herwig input enabled)"; HEPMC_OK=1
else
  echo "  [WARN] --hepmc3 NOT in this binary — rebuild with CAP_ENABLE_HEPMC3=ON; Herwig will be skipped"; HEPMC_OK=0
fi

echo; echo "=== [1/4] Pythia -> compare/pythia.root ==="
"$STUDY" --events "$EVENTS" --species "$SPECIES" \
         --out "$OUT/compare/pythia.root" || { echo "Pythia stage FAILED"; exit 1; }

echo; echo "=== [2/4] Herwig -> compare/herwig.root ==="
if [ "${HEPMC:-}" ] && [ -s "${HEPMC:-/nonexistent}" ] && [ "$HEPMC_OK" = 1 ]; then
  echo "  HepMC input: $HEPMC"
  "$STUDY" --events "$EVENTS" --species "$SPECIES" \
           --hepmc3 "$HEPMC" --out "$OUT/compare/herwig.root" \
    || echo "  Herwig stage FAILED (see above)"
else
  echo "  skipped — set HEPMC=/path/to/events.hepmc (and build with --hepmc3) to include Herwig."
fi

echo; echo "=== [3/4] overlay (Pythia vs Herwig) -> plots/ ==="
"$PLOT" --compare "$OUT/compare" --outdir "$OUT/plots"

echo; echo "=== [4/4] report (+PDF) -> reports/ ==="
"$REPORT" --summary "$OUT/compare/pythia.root.txt" \
          --figures "$OUT/plots" --mode paper \
          --outdir "$OUT/reports" --pdf || echo "  (report/pdflatex returned nonzero)"

echo; echo "=== outputs ==="
ls -la "$OUT/compare"/*.root "$OUT/plots"/compare_*."pdf" 2>/dev/null | head -20
echo "report: $OUT/reports/provenance-report.pdf"
echo "log   : $LOG"
