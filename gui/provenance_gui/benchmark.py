"""Local run benchmarking + parallelism recommendation.

Measures how fast ``provenance-study`` runs on THIS machine, then predicts how
long the configured study/ladder will take and recommends single vs parallel
plus a job count.  Reality check baked in: one ``provenance-study`` process is
single-threaded, so extra cores only help when there are multiple rungs to run
concurrently (the ladder).  For a one-shot standalone run, parallel does
nothing — the recommendation says so.

No tkinter import here, so the pure prediction helpers are unit-testable
without a display or the binary.
"""
from __future__ import annotations

import math
import os
import subprocess
import tempfile
import time
from dataclasses import dataclass


# --------------------------------------------------------------------- pure
def predict_ladder_seconds(events: int, rungs: int, rate1: float,
                           jobs: int, efficiency: float = 1.0) -> float:
    """Predicted wall-clock seconds.

    events     — events per rung (per provenance-study process)
    rungs      — number of rungs (1 == a single standalone run)
    rate1      — measured events/sec for ONE process
    jobs       — how many processes run concurrently
    efficiency — measured parallel efficiency (0..1); concurrent processes
                 slow each other when memory/IO-bound.
    """
    if rate1 <= 0:
        return float("inf")
    per = events / rate1                       # one rung, one process
    rungs = max(1, rungs)
    if rungs == 1:
        return per                             # parallel can't split one run
    eff_jobs = max(1, min(jobs, rungs))
    if eff_jobs == 1:
        return rungs * per
    batches = math.ceil(rungs / eff_jobs)
    return batches * per / max(efficiency, 1e-6)


@dataclass
class Recommendation:
    jobs: int
    mode: str            # "single" | "parallel"
    pred_single_s: float
    pred_rec_s: float
    note: str


def recommend(events: int, rungs: int, rate1: float, efficiency: float,
              cores: int, ram_jobs: int) -> Recommendation:
    """Pick single vs parallel + a job count, and predict both wall times."""
    pred_single = predict_ladder_seconds(events, rungs, rate1, 1, efficiency)

    if rungs <= 1:
        return Recommendation(
            1, "single", pred_single, pred_single,
            "Single run: one provenance-study process is single-threaded, so "
            "extra cores would not speed this up. Use the saved cores for "
            "other work.")

    # Cap parallelism by the smallest of: rungs, cores the user allows, and
    # the RAM-safe job count from the advisor.
    cap = max(1, min(rungs, cores, ram_jobs))
    pred_cap = predict_ladder_seconds(events, rungs, rate1, cap, efficiency)

    # Parallel only worth it if it actually beats single by a clear margin and
    # the measured efficiency isn't terrible.
    if cap > 1 and efficiency >= 0.55 and pred_cap < 0.9 * pred_single:
        speedup = pred_single / pred_cap if pred_cap > 0 else 1.0
        return Recommendation(
            cap, "parallel", pred_single, pred_cap,
            "Parallel with %d job(s): ~%.1fx faster than single at measured "
            "efficiency %.0f%% (capped by rungs/cores/RAM)."
            % (cap, speedup, 100 * efficiency))
    return Recommendation(
        1, "single", pred_single, pred_single,
        "Single recommended: measured parallel efficiency (%.0f%%) is too low "
        "to beat a sequential run here — likely memory- or IO-bound."
        % (100 * efficiency))


def fmt_seconds(s: float) -> str:
    if s == float("inf"):
        return "?"
    s = int(round(s))
    if s < 90:
        return "%ds" % s
    if s < 5400:
        return "%dm %02ds" % (s // 60, s % 60)
    return "%dh %02dm" % (s // 3600, (s % 3600) // 60)


# --------------------------------------------------------------- subprocess
def _time_cmd(cmd: list[str], cwd: str | None = None) -> float:
    t0 = time.time()
    subprocess.run(cmd, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL,
                   cwd=cwd)
    return time.time() - t0


def _study_argv(study_bin: str, events: int, species: str, ecm: str,
                seed: str, out: str) -> list[str]:
    return [study_bin, "--events", str(events), "--species", species,
            "--ecm", ecm, "--seed", seed, "--out", out]


@dataclass
class BenchResult:
    ok: bool
    rate1: float
    efficiency: float
    probe_jobs: int
    cores: int
    rec: Recommendation | None
    note: str


def run_benchmark(study_bin: str, *, events: int, rungs: int, species: str,
                  ecm: str, seed: str, bench_events: int, cores: int,
                  ram_jobs: int, probe_jobs: int | None = None) -> BenchResult:
    """Run a quick calibration and return measured rate, parallel efficiency,
    and a recommendation for the full (events x rungs) run.

    bench_events — small event count for the timing sample (e.g. 150).
    probe_jobs   — concurrency level for the efficiency probe (default
                   min(cores, 4)); set 1 to skip the probe.
    """
    if not os.path.exists(study_bin):
        return BenchResult(False, 0.0, 1.0, 1, cores, None,
                           "study binary not found: %s" % study_bin)
    probe_jobs = probe_jobs or max(1, min(cores, 4))
    workdir = tempfile.mkdtemp(prefix="cap-bench-")

    # --- single-process rate ---
    out1 = os.path.join(workdir, "b1.root")
    t1 = _time_cmd(_study_argv(study_bin, bench_events, species, ecm, seed, out1))
    rate1 = bench_events / t1 if t1 > 0 else 0.0
    if rate1 <= 0:
        return BenchResult(False, 0.0, 1.0, probe_jobs, cores, None,
                           "calibration run produced no timing")

    # --- parallel efficiency probe: launch `probe_jobs` concurrently ---
    efficiency = 1.0
    if probe_jobs > 1:
        procs = []
        t0 = time.time()
        for k in range(probe_jobs):
            outk = os.path.join(workdir, "p%d.root" % k)
            procs.append(subprocess.Popen(
                _study_argv(study_bin, bench_events, species, ecm, seed, outk),
                stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL))
        for p in procs:
            p.wait()
        wall = time.time() - t0
        throughput = (probe_jobs * bench_events) / wall if wall > 0 else 0.0
        efficiency = max(0.0, min(1.0, throughput / (probe_jobs * rate1)))

    rec = recommend(events, rungs, rate1, efficiency, cores, ram_jobs)
    return BenchResult(True, rate1, efficiency, probe_jobs, cores, rec,
                       "ok")
