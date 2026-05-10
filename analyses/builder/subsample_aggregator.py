#!/usr/bin/env python3
"""
subsample_aggregator — combine ROOT histogram files across subsample
directories.

Used by the N×M subsample-grid runner in run-cap.  Given a list of
input directories, each containing the same set of *.root files, this
module produces a single output directory with one .root file per
filename, where each output histogram is the bin-by-bin SUM of the
same-named histogram across the inputs.

Two modes:
  - aggregate(input_dirs, output_dir)   single-level sum
  - aggregate_grid(base_dir, N, M)      hierarchical:
      Phase A: for each i in [0,N): aggregate raw_i_*/ → sub_i/
      Phase B:                       aggregate sub_*/  → final/

CLI usage:
    ./subsample_aggregator.py --grid histos/demo_run/ --rows 5 --cols 10
or:
    ./subsample_aggregator.py --inputs A/ B/ C/ --output combined/
"""
from __future__ import annotations

import argparse
import os
import sys
from pathlib import Path
from typing import Iterable


def _import_root():
    try:
        import ROOT  # noqa: F401
        ROOT.gROOT.SetBatch(True)
        ROOT.gErrorIgnoreLevel = ROOT.kWarning
        return ROOT
    except ImportError:
        sys.stderr.write(
            "subsample_aggregator: PyROOT not available.\n"
            "  source SetupCAP.sh   # to put ROOT on PATH\n"
        )
        sys.exit(2)


# ----------------------------------------------------------------------
#  Single-level aggregation: sum every histogram across input_dirs.
# ----------------------------------------------------------------------
def aggregate(input_dirs: list[Path],
              output_dir: Path,
              progress=lambda msg: None) -> dict:
    """Sum every histogram in every *.root file across input_dirs.

    For each filename present in the FIRST input dir, opens that file
    in every input dir, walks its TKeys, and writes a summed copy of
    each histogram to output_dir/<filename>.

    Returns a summary dict {file_name: {n_inputs, n_histos, n_skipped}}.
    """
    ROOT = _import_root()
    input_dirs = [Path(p) for p in input_dirs]
    output_dir = Path(output_dir)
    output_dir.mkdir(parents=True, exist_ok=True)

    if not input_dirs:
        return {}

    # Use the first dir as the reference for which files exist.
    ref = input_dirs[0]
    if not ref.is_dir():
        progress(f"  skipping: reference dir {ref} does not exist")
        return {}

    file_names = sorted(p.name for p in ref.glob("*.root"))
    summary: dict[str, dict] = {}

    for fname in file_names:
        inputs = [d / fname for d in input_dirs if (d / fname).is_file()]
        if len(inputs) < 1:
            continue

        progress(f"  aggregating {fname}  ({len(inputs)} files)")

        # Open all inputs.
        in_files = []
        for p in inputs:
            f = ROOT.TFile.Open(str(p), "READ")
            if not f or f.IsZombie():
                progress(f"    skip (cannot open): {p}")
                continue
            in_files.append(f)

        out_path = output_dir / fname
        out_file = ROOT.TFile.Open(str(out_path), "RECREATE")
        if not out_file or out_file.IsZombie():
            progress(f"    cannot create {out_path}")
            for f in in_files:
                f.Close()
            continue

        n_histos = 0
        n_skipped = 0

        # Use the first file's TKeys as the reference set.
        first = in_files[0]
        for key in first.GetListOfKeys():
            cls = key.GetClassName()
            name = key.GetName()
            # Only sum histograms (TH1, TH2, TH3 — TProfile too).
            if not (cls.startswith("TH") or cls.startswith("TProfile")):
                continue
            try:
                obj = first.Get(name)
                if obj is None:
                    n_skipped += 1
                    continue
                summed = obj.Clone()
                summed.SetDirectory(out_file)

                for f in in_files[1:]:
                    other = f.Get(name)
                    if other is None:
                        continue
                    try:
                        summed.Add(other)
                    except Exception:
                        # Histos with mismatched binning will throw —
                        # skip rather than abort.
                        pass

                summed.Write()
                n_histos += 1
            except Exception as exc:
                n_skipped += 1
                progress(f"    skip {name}: {exc}")

        out_file.Close()
        for f in in_files:
            f.Close()

        summary[fname] = {
            "n_inputs":  len(in_files),
            "n_histos":  n_histos,
            "n_skipped": n_skipped,
        }

    return summary


# ----------------------------------------------------------------------
#  Hierarchical N×M aggregation.
# ----------------------------------------------------------------------
def aggregate_grid(base_dir: Path,
                   n_rows: int,
                   n_cols: int,
                   progress=lambda msg: None) -> dict:
    """Run the two-phase N×M aggregation.

    Phase A: for each i in [0, n_rows): combine
        base_dir/raw_{i}_{0}/ … raw_{i}_{n_cols-1}/   →   base_dir/sub_{i}/

    Phase B: combine
        base_dir/sub_{0}/ … sub_{n_rows-1}/             →   base_dir/final/

    Returns {phase: {…summary per phase…}}.
    """
    base_dir = Path(base_dir)
    overall: dict = {"phase_a": {}, "phase_b": None}

    # Phase A — row-wise.
    sub_dirs = []
    for i in range(n_rows):
        row_inputs = [base_dir / f"raw_{i}_{j}" for j in range(n_cols)]
        present = [d for d in row_inputs if d.is_dir()]
        if not present:
            progress(f"Phase A row {i}: no raw dirs found, skipping")
            continue
        sub_dir = base_dir / f"sub_{i}"
        progress(f"\n=== Phase A — row {i} ({len(present)}/{n_cols} dirs) → {sub_dir.name}")
        overall["phase_a"][i] = aggregate(present, sub_dir, progress=progress)
        sub_dirs.append(sub_dir)

    # Phase B — combine the rows.
    if not sub_dirs:
        progress("Phase B: no sub_*/ dirs, skipping final aggregation")
        return overall

    final_dir = base_dir / "final"
    progress(f"\n=== Phase B — combining {len(sub_dirs)} rows → {final_dir.name}")
    overall["phase_b"] = aggregate(sub_dirs, final_dir, progress=progress)

    return overall


# ----------------------------------------------------------------------
#  CLI
# ----------------------------------------------------------------------
def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="mode", required=True)

    g_simple = sub.add_parser("simple", help="single-level aggregate")
    g_simple.add_argument("--inputs", nargs="+", required=True, help="input dirs")
    g_simple.add_argument("--output", required=True, help="output dir")

    g_grid = sub.add_parser("grid", help="N×M hierarchical aggregate")
    g_grid.add_argument("base", help="histos/<outdir>/ root containing raw_i_j/")
    g_grid.add_argument("--rows", type=int, required=True, help="N")
    g_grid.add_argument("--cols", type=int, required=True, help="M")

    args = ap.parse_args()

    def echo(msg):
        print(msg, flush=True)

    if args.mode == "simple":
        s = aggregate([Path(p) for p in args.inputs], Path(args.output), echo)
        for fname, info in s.items():
            print(f"  {fname:30s}  {info['n_inputs']:3d} inputs, "
                  f"{info['n_histos']:5d} histos, {info['n_skipped']} skipped")
    elif args.mode == "grid":
        s = aggregate_grid(Path(args.base), args.rows, args.cols, echo)
        for i, files in s["phase_a"].items():
            total = sum(v["n_histos"] for v in files.values())
            print(f"  Phase A row {i}: {total} histos written")
        if s["phase_b"]:
            total = sum(v["n_histos"] for v in s["phase_b"].values())
            print(f"  Phase B (final): {total} histos written")


if __name__ == "__main__":
    main()
