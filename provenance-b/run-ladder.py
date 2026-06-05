#!/usr/bin/env python3
"""MPI + CR mechanism-ladder run, BOTH generators, with resume.

For each generator (Pythia, Herwig) and each cumulative rung
  baseline   : MPI off, CR off
  +MPI       : MPI on,  CR off
  +MPI+CR    : MPI on,  CR on
it generates that configuration, runs provenance-study, and writes
  <outdir>/ladder/<gen>_<rung>.root
Then it overlays all six with cap-provenance-plot --compare, and builds ONE
mechanism-ladder report (provenance-report --summaries) with a column per
(generator, rung) so you can read MPI's and CR's effect across the row.

Reuses the same pipeline pieces as the GUI (mechanisms.mech_lines, the proven
Herwig deck recipe).  Resume: a rung whose <gen>_<rung>.root.txt exists is
skipped.  Herwig .hepmc are deleted right after each rung's study (disk).

Example (50k events per rung -> 6 configs):
  python3 provenance-b/run-ladder.py --events 50000 \
      --outdir "/Volumes/T7 Shield/ladder50k"
"""
from __future__ import annotations

import argparse
import subprocess
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(REPO / "gui"))

from provenance_gui import pipeline, mechanisms, herwig_runner   # noqa: E402

# Cumulative rungs: (dir-safe name, column label, mechanisms ON).
RUNGS = [
    ("baseline", "base",     []),
    ("MPI",      "+MPI",     ["MPI"]),
    ("MPICR",    "+MPI+CR",  ["MPI", "CR"]),
]
GEN_LABEL = {"pythia": "Pythia 8", "herwig": "Herwig 7"}
COMMON = mechanisms.common_mechanisms()          # ["MPI", "CR"]


def run(argv, cwd=None) -> int:
    print("\n=== " + " ".join(pipeline.shell_quote(x) for x in argv) + " ===",
          flush=True)
    return subprocess.run(argv, cwd=cwd).returncode


def main() -> int:
    ap = argparse.ArgumentParser(
        description=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--events", type=int, default=50000)
    ap.add_argument("--species", default="211")
    ap.add_argument("--ecm", type=float, default=13000.0)
    ap.add_argument("--seed", type=int, default=12345)
    ap.add_argument("--outdir", default=str(REPO / "provenance-b" / "ladder"))
    ap.add_argument("--keep-hepmc", action="store_true")
    ap.add_argument("--no-pdf", action="store_true")
    ap.add_argument("--entropy", action="store_true",
                    help="ALSO accumulate the entropy / information block in "
                         "every rung (the ladder report then gains the "
                         "entropy-across-the-ladder table).  Note: rungs "
                         "already finished WITHOUT --entropy are skipped by "
                         "resume; delete their .root.txt to re-run them with "
                         "entropy on.")
    ap.add_argument("--systems", action="store_true",
                    help="ALSO accumulate the fragmentation-system "
                         "observables in every rung (lambda-vs-CR is the "
                         "headline: CR exists to minimize the total string "
                         "length, and this makes that action visible).")
    ap.add_argument("--print-only", action="store_true")
    a = ap.parse_args()

    outdir   = Path(a.outdir).expanduser().resolve()
    laddir   = outdir / "ladder"
    herwigd  = outdir / "herwig"
    plotsd   = outdir / "plots"
    reportsd = outdir / "reports"
    for d in (laddir, herwigd, plotsd, reportsd):
        d.mkdir(parents=True, exist_ok=True)

    study  = pipeline.study_binary(REPO)
    report = pipeline.report_binary(REPO)
    ev     = str(int(a.events))
    common_args = ["--events", ev, "--species", a.species,
                   "--ecm", str(float(a.ecm)), "--seed", str(int(a.seed))]
    if a.entropy:
        common_args.append("--entropy")
    if a.systems:
        common_args.append("--systems")

    # Build the full plan: (description, list-of-commands, hepmc-to-delete|None)
    cmds: list[tuple[list[str], str | None]] = []
    summaries: list[str] = []          # "Label=path.txt" for the report

    for gen in ("pythia", "herwig"):
        if gen == "herwig" and not (herwig_runner.herwig_available()
                                    and pipeline.herwig_supported(study)):
            print("Herwig unavailable or engine lacks --hepmc3; skipping Herwig.")
            continue
        for rname, rlabel, mechs in RUNGS:
            root = laddir / ("%s_%s.root" % (gen, rname))
            summaries.append("%s %s=%s" % (GEN_LABEL[gen], rlabel,
                                           str(root) + ".txt"))
            done = (root.with_suffix(".root.txt")).exists()
            if done:
                print("resume: %s_%s already done — skip" % (gen, rname))
                continue
            lines = mechanisms.mech_lines(gen, mechs, only=COMMON)
            if gen == "pythia":
                cmnd = laddir / ("pythia_%s.cmnd" % rname)
                cmnd.write_text("! ladder rung %s\n" % rname
                                + "\n".join(lines) + "\n")
                cmds.append(([study] + common_args
                             + ["--config", str(cmnd), "--out", str(root)],
                             None))
            else:                       # herwig
                stem = "herwig_%s" % rname
                deck = herwig_runner.build_deck(herwigd, stem, lines,
                                                seed=a.seed)
                hp = herwig_runner.hepmc_path(herwigd, stem)
                rung_cmds: list[list[str]] = []
                if not hp.exists():
                    rung_cmds += herwig_runner.gen_commands(herwigd, stem,
                                                            int(a.events))
                rung_cmds.append([study] + common_args
                                 + ["--out", str(root), "--hepmc3", str(hp)])
                for k, c in enumerate(rung_cmds):
                    last = (k == len(rung_cmds) - 1)
                    cmds.append((c, str(hp) if (last and not a.keep_hepmc)
                                 else None))

    # Tail: overlay + ladder report.
    tail = [
        ([str(REPO / "cap-provenance-plot"), "--compare", str(laddir),
          "--outdir", str(plotsd)], None),
        ([report, "--summaries", ";".join(summaries),
          "--figures", str(plotsd),
          "--title", "Provenance across the MPI+CR mechanism ladder",
          "--outdir", str(reportsd), "--out", "provenance-ladder"]
         + ([] if a.no_pdf else ["--pdf"]), None),
    ]

    print("=" * 70)
    print("LADDER PLAN: 2 generators x 3 rungs (baseline / +MPI / +MPI+CR)")
    print("  generation/study stages to run now: %d (rest already done)"
          % len(cmds))
    print("  columns in report: %d" % len(summaries))
    print("  outdir: %s" % outdir)
    print("=" * 70)
    if a.print_only:
        for c, _ in cmds + tail:
            print("  " + " ".join(pipeline.shell_quote(x) for x in c))
        return 0

    for c, hepmc in (cmds + tail):
        cwd = str(reportsd) if (c and c[0] == "pdflatex") else None
        rc = run(c, cwd=cwd)
        if rc != 0:
            print("\nSTAGE FAILED (exit %d). Re-run the SAME command — finished "
                  "rungs are skipped." % rc, file=sys.stderr)
            return rc
        if hepmc and Path(hepmc).exists():
            try:
                sz = Path(hepmc).stat().st_size / 1e9
                Path(hepmc).unlink()
                print("    [disk] deleted %s (%.1f GB freed)"
                      % (Path(hepmc).name, sz))
            except OSError as exc:
                print("    [disk] could not delete %s: %s" % (hepmc, exc))

    print("\n=== ladder finished ===")
    print("report: %s/provenance-ladder.pdf" % reportsd)
    return 0


if __name__ == "__main__":
    sys.exit(main())
