#!/usr/bin/env python3
"""Headless launcher for the provenance pipeline — the SAME code the GUI's Run
button uses (provenance_gui.pipeline).  It builds the commands for the chosen
configuration (which also writes the Pythia .cmnd and the per-chunk Herwig .in
decks), prints the exact one-liner, then runs the stages sequentially with
resume.

Re-run the identical command after a crash and it SKIPS the chunks that already
finished (their .root.txt exists under a matching config fingerprint) — that is
the "resume power".

Examples
  # 200k events, Pythia + Herwig, single thread, resumable (50k chunks):
  python3 provenance-b/run-cli.py --events 200000 --generators pythia,herwig \
      --chunk 50000 --jobs 1 --outdir provenance-b/run200k

  # just show the command, do not run:
  python3 provenance-b/run-cli.py ... --print-only
"""
from __future__ import annotations

import argparse
import subprocess
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(REPO / "gui"))

from provenance_gui import pipeline   # noqa: E402  (after sys.path tweak)


def build_state(a: argparse.Namespace) -> dict:
    return {
        "repo":          REPO,
        "outdir":        str(Path(a.outdir).expanduser().resolve()),
        "generators":    [g.strip() for g in a.generators.split(",") if g.strip()],
        "events":        int(a.events),
        "ecm":           float(a.ecm),
        "seed":          int(a.seed),
        "species":       a.species,
        "chunk_events":  int(a.chunk),
        "stages":        {"standalone": True, "plot": True,
                          "report": True, "pdflatex": not a.no_pdf},
        "parallel":      {"caffeinate": not a.no_caffeinate, "nice": True},
        "_resolved_jobs": int(a.jobs),
        "acceptance":    {k: True for k, on in
                          (("entropy", a.entropy), ("systems", a.systems))
                          if on},
        "ladder_rungs":  [],
        "report":        {"mode": "paper"},
        "compare_mechanisms": False,
        "pairs":         [],
    }


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--events", type=int, default=200000)
    ap.add_argument("--generators", default="pythia,herwig")
    ap.add_argument("--species", default="211")
    ap.add_argument("--ecm", type=float, default=13000.0)
    ap.add_argument("--seed", type=int, default=12345)
    ap.add_argument("--chunk", type=int, default=50000,
                    help="resume-chunk size; 0 disables chunking")
    ap.add_argument("--jobs", type=int, default=1,
                    help="parallel jobs (1 = single thread)")
    ap.add_argument("--outdir", default=str(REPO / "provenance-b" / "run"))
    ap.add_argument("--no-pdf", action="store_true",
                    help="skip the pdflatex stage")
    ap.add_argument("--no-caffeinate", action="store_true")
    ap.add_argument("--keep-hepmc", action="store_true",
                    help="keep Herwig .hepmc files (default: delete each one "
                         "right after its .root is made, to save disk — the "
                         ".hepmc are huge, ~150 KB/event).  Resume is keyed on "
                         "the .root.txt so deleting the .hepmc is safe.")
    ap.add_argument("--entropy", action="store_true",
                    help="ALSO accumulate the entropy / information "
                         "observables (multiplicity entropy, stage entropy "
                         "profile, mutual-information recovery) and include "
                         "the entropy section + figures in the report.  "
                         "Changes the resume fingerprint, so cached "
                         "non-entropy chunks are correctly re-run.")
    ap.add_argument("--systems", action="store_true",
                    help="ALSO accumulate the fragmentation-system "
                         "observables (string/cluster masses, hadrons per "
                         "system, rapidity span, lambda measure, charge "
                         "ordering, B-Bbar and strangeness pairing, cluster "
                         "fission) and include the section + figures in the "
                         "report.  Changes the resume fingerprint.")
    ap.add_argument("--print-only", action="store_true",
                    help="print the one-liner + per-stage list and exit")
    a = ap.parse_args()

    state = build_state(a)
    # build_commands() also writes the .cmnd / Herwig decks and honours resume
    # by reading the PRIOR manifest; write_run_manifest() records THIS plan.
    cmds, generated = pipeline.build_commands(state)
    warnings = pipeline.run_warnings(state)
    try:
        done, total = pipeline.write_run_manifest(state)
    except Exception:                                    # never block a run
        done, total = 0, 0

    for w in warnings:
        print("WARNING:", w)
    if not cmds:
        print("Nothing to run (all stages disabled, or everything already "
              "done via resume).")
        return 0

    print("=" * 70)
    print("ONE COMMAND (copy-paste equivalent):")
    print(pipeline.commands_to_oneliner(state, cmds))
    print("=" * 70)
    if done:
        print(f"resume: skipping {done}/{total} finished unit(s)\n")
    print(f"stages: {len(cmds)}   outdir: {state['outdir']}\n")

    if a.print_only:
        return 0

    reports_dir = Path(state["outdir"]).expanduser().resolve() / "reports"
    for i, argv in enumerate(cmds, 1):
        print(f"\n=== [{i}/{len(cmds)}] "
              + " ".join(pipeline.shell_quote(x) for x in argv) + " ===",
              flush=True)
        # pdflatex is invoked with a bare filename, so it must run FROM the
        # reports/ directory (mirrors what Runner._run does in the GUI).
        cwd = str(reports_dir) if (argv and argv[0] == "pdflatex") else None
        rc = subprocess.run(argv, cwd=cwd).returncode
        if rc != 0:
            print(f"\nstage {i} FAILED (exit {rc}).  Fix it, then re-run the "
                  f"SAME command — finished chunks are skipped.", file=sys.stderr)
            return rc
        # Disk hygiene: once a Herwig provenance-study (--hepmc3) has produced
        # its .root, the giant .hepmc it read is no longer needed (resume is
        # keyed on the .root.txt).  Delete it to keep the run from filling the
        # disk — unless --keep-hepmc was given.
        if (not a.keep_hepmc) and ("--hepmc3" in argv) and ("--out" in argv):
            hepmc = Path(argv[argv.index("--hepmc3") + 1])
            root_txt = Path(argv[argv.index("--out") + 1] + ".txt")
            if root_txt.exists() and hepmc.exists():
                try:
                    sz = hepmc.stat().st_size / 1e9
                    hepmc.unlink()
                    print(f"    [disk] deleted {hepmc.name} "
                          f"({sz:.1f} GB freed — chunk .root is kept)")
                except OSError as exc:
                    print(f"    [disk] could not delete {hepmc}: {exc}")
    print("\n=== pipeline finished ===")
    print(f"report: {state['outdir']}/reports/provenance-report.pdf")
    return 0


if __name__ == "__main__":
    sys.exit(main())
