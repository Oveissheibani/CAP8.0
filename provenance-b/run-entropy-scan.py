#!/usr/bin/env python3
"""Energy scan of the collision entropy — S(multiplicity) vs sqrt(s).

The Kharzeev-Levin entanglement entropy grows like ln(1/x) ~ ln(s), so a
single energy cannot test the DYNAMICS.  This driver runs provenance-study
(--entropy) at several centre-of-mass energies, parses the entropy block of
each summary, and writes:

  <outdir>/entropy-scan.csv          one row per (generator, ecm, window)
  <outdir>/entropy-scan.pdf          S vs ln(sqrt(s)) per window (PyROOT;
                                     skipped gracefully if ROOT is absent)

Resume: a run is skipped when its .root.txt already exists, so the scan can
be interrupted and re-launched with the same command.

Pythia only by default (Herwig needs one .hepmc per energy; point
--herwig-hepmc at a 'pattern with {ecm}' if you have them).

Example
  python3 provenance-b/run-entropy-scan.py --events 20000 \
      --ecms 900,2760,7000,13000 --outdir "/Volumes/T7 Shield/ent_scan"
"""
from __future__ import annotations

import argparse
import csv
import re
import subprocess
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(REPO / "gui"))

from provenance_gui import pipeline   # noqa: E402

# Window labels in the order EntropyObservables reports them.
WINDOWS = ["|eta|<0.5", "|eta|<1.0", "|eta|<2.0", "full"]


def parse_entropy_block(txt_path: Path) -> list[dict]:
    """Rows {window, S, err, S2, meanN, ratio} from one .root.txt summary."""
    rows: list[dict] = []
    in_mult = False
    try:
        text = txt_path.read_text(errors="replace")
    except OSError:
        return rows
    for line in text.splitlines():
        if "entropy: multiplicity entropy" in line:
            in_mult = True
            continue
        if in_mult:
            if line.strip().startswith("entropy:"):
                break
            m = re.match(
                r"\s*(\S+)\s+([\d.eE+-]+)\s+err\s+([\d.eE+-]+)"
                r"(?:\s+S2\s+([\d.eE+-]+))?(?:\s+meanN\s+([\d.eE+-]+))?"
                r"(?:\s+S/lnN\s+([\d.eE+-]+))?", line)
            if m:
                rows.append({"window": m.group(1), "S": m.group(2),
                             "err": m.group(3), "S2": m.group(4) or "",
                             "meanN": m.group(5) or "",
                             "ratio": m.group(6) or ""})
    return rows


def main() -> int:
    ap = argparse.ArgumentParser(
        description=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--events", type=int, default=20000,
                    help="events PER energy point (default 20000)")
    ap.add_argument("--ecms", default="900,2760,7000,13000",
                    help="comma-separated sqrt(s) values in GeV")
    ap.add_argument("--species", default="211")
    ap.add_argument("--seed", type=int, default=12345)
    ap.add_argument("--outdir", default=str(REPO / "provenance-b" / "ent_scan"))
    ap.add_argument("--print-only", action="store_true")
    a = ap.parse_args()

    outdir = Path(a.outdir).expanduser().resolve()
    outdir.mkdir(parents=True, exist_ok=True)
    study = pipeline.study_binary(REPO)
    ecms = [float(e) for e in a.ecms.replace(" ", "").split(",") if e]

    # ---- run (or resume) every energy point ------------------------------
    results: list[tuple[float, Path]] = []
    for ecm in ecms:
        root = outdir / ("pythia_ecm%d.root" % int(ecm))
        txt = Path(str(root) + ".txt")
        results.append((ecm, txt))
        if txt.exists():
            print("resume: ecm=%g already done — skip" % ecm)
            continue
        argv = [study, "--events", str(int(a.events)),
                "--species", a.species, "--ecm", str(ecm),
                "--seed", str(int(a.seed)), "--entropy",
                "--out", str(root)]
        print("\n=== " + " ".join(pipeline.shell_quote(x) for x in argv)
              + " ===", flush=True)
        if a.print_only:
            continue
        rc = subprocess.run(argv).returncode
        if rc != 0:
            print("energy point %g FAILED (exit %d); re-run the same command "
                  "to resume." % (ecm, rc), file=sys.stderr)
            return rc
    if a.print_only:
        return 0

    # ---- collect into a CSV ----------------------------------------------
    csv_path = outdir / "entropy-scan.csv"
    table: list[dict] = []
    for ecm, txt in results:
        for r in parse_entropy_block(txt):
            r["ecm"] = ecm
            table.append(r)
    with open(csv_path, "w", newline="") as fh:
        w = csv.DictWriter(fh, fieldnames=["ecm", "window", "S", "err",
                                           "S2", "meanN", "ratio"])
        w.writeheader()
        for r in table:
            w.writerow(r)
    print("\nwrote %s (%d rows)" % (csv_path, len(table)))

    # Console summary: S vs ecm per window + the KL ratio.
    print("\n%-12s" % "window" + "".join("%14s" % ("%g GeV" % e)
                                          for e, _ in results))
    for win in WINDOWS:
        vals = {r["ecm"]: r for r in table if r["window"] == win}
        line = "%-12s" % win
        ratio = "%-12s" % "  S/lnN"
        for ecm, _ in results:
            r = vals.get(ecm)
            line += "%14s" % (r["S"] if r else "--")
            ratio += "%14s" % ((r["ratio"] or "--") if r else "--")
        print(line)
        print(ratio)

    # ---- figure: S vs ln sqrt(s), one curve per window (PyROOT) ----------
    try:
        import math
        import ROOT
        ROOT.gROOT.SetBatch(True)
        ROOT.gStyle.SetOptStat(0)
        colours = [ROOT.kAzure + 1, ROOT.kGreen + 2,
                   ROOT.kOrange + 7, ROOT.kRed + 1]
        mg = ROOT.TMultiGraph()
        legend = ROOT.TLegend(0.14, 0.66, 0.46, 0.88)
        legend.SetBorderSize(1)
        graphs = []
        for i, win in enumerate(WINDOWS):
            g = ROOT.TGraphErrors()
            for ecm, _ in results:
                rs = [r for r in table
                      if r["window"] == win and r["ecm"] == ecm]
                if not rs:
                    continue
                k = g.GetN()
                g.SetPoint(k, math.log(ecm), float(rs[0]["S"]))
                g.SetPointError(k, 0.0, float(rs[0]["err"]))
            if g.GetN() == 0:
                continue
            g.SetMarkerStyle(20); g.SetMarkerSize(0.9)
            g.SetMarkerColor(colours[i % 4]); g.SetLineColor(colours[i % 4])
            mg.Add(g, "PL")
            legend.AddEntry(g, win, "pl")
            graphs.append(g)
        if graphs:
            c = ROOT.TCanvas("c_scan", "", 900, 680)
            mg.SetTitle(";ln(#sqrt{s} / GeV);S(multiplicity) [nats]")
            mg.Draw("A")
            legend.Draw()
            pdf = str(outdir / "entropy-scan.pdf")
            c.SaveAs(pdf)
            print("wrote %s" % pdf)
    except Exception as exc:                       # ROOT missing etc.
        print("figure skipped (%s); the CSV has everything." % exc)
    return 0


if __name__ == "__main__":
    sys.exit(main())
