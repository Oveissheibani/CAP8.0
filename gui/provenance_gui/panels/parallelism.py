"""Parallelism mode, CPU-power control, and a local run benchmark.

The benchmark times provenance-study on a small sample to measure this
machine's events/sec and how well concurrent processes scale, then predicts
the full run's wall time and recommends single vs parallel + a job count.
Grid execution (Wayne State) is stubbed for now — local only.
"""
from __future__ import annotations

import threading
import tkinter as tk
from pathlib import Path
from tkinter import ttk

from .base import Panel, bind_to_state
from ..system_info import advise, p_cores
from ..benchmark import run_benchmark, fmt_seconds


class ParallelismPanel(Panel):
    title = "Parallelism"

    MODES = [
        ("seq",    "Sequential"),
        ("auto",   "Parallel — auto (match ladder)"),
        ("manual", "Parallel — manual"),
    ]

    def build(self) -> None:
        cores = p_cores()
        s = self.state.setdefault("parallel", {
            "mode": "auto", "jobs_manual": 2,
            "caffeinate": True, "nice": True,
        })
        s.setdefault("target", "local")
        s.setdefault("max_cores", cores)
        s.setdefault("bench_events", 150)

        self._cores = cores
        self._bench_running = False

        self.mode_var       = tk.StringVar (value=s["mode"])
        self.jobs_var       = tk.IntVar    (value=s["jobs_manual"])
        self.caffeinate_var = tk.BooleanVar(value=s["caffeinate"])
        self.nice_var       = tk.BooleanVar(value=s["nice"])
        self.target_var     = tk.StringVar (value=s["target"])
        self.max_cores_var  = tk.IntVar    (value=int(s["max_cores"]))
        for var, key in [(self.mode_var,       "mode"),
                         (self.jobs_var,       "jobs_manual"),
                         (self.caffeinate_var, "caffeinate"),
                         (self.nice_var,       "nice"),
                         (self.target_var,     "target")]:
            bind_to_state(var, s, key)

        # Ladder designer notifies us when the rung count changes.
        self.state["_on_ladder_change"] = self.refresh

        f = self.frame

        # ---- run target (local / grid) ----
        tgt = ttk.Frame(f); tgt.grid(row=0, column=0, columnspan=4, sticky="w")
        ttk.Label(tgt, text="Run on:").pack(side=tk.LEFT)
        ttk.Radiobutton(tgt, text="This Mac (local)", value="local",
                        variable=self.target_var,
                        command=self.refresh).pack(side=tk.LEFT, padx=(6, 0))
        self._grid_rb = ttk.Radiobutton(
            tgt, text="Wayne State grid (coming soon)", value="grid",
            variable=self.target_var, state="disabled")
        self._grid_rb.pack(side=tk.LEFT, padx=(12, 0))

        # ---- mode radios + manual jobs ----
        modes = ttk.Frame(f); modes.grid(row=1, column=0, columnspan=4,
                                         sticky="w", pady=(6, 0))
        for val, lbl in self.MODES:
            ttk.Radiobutton(modes, text=lbl, value=val,
                            variable=self.mode_var,
                            command=self.refresh).pack(side=tk.LEFT,
                                                       padx=(0, 12))
        ttk.Label(modes, text="manual jobs:").pack(side=tk.LEFT)
        ttk.Spinbox(modes, from_=1, to=64, increment=1, width=4,
                    textvariable=self.jobs_var, command=self.refresh
                    ).pack(side=tk.LEFT, padx=(4, 0))

        # ---- CPU power (max cores the app may use) ----
        cpu = ttk.Frame(f); cpu.grid(row=2, column=0, columnspan=4, sticky="we",
                                     pady=(6, 0))
        ttk.Label(cpu, text="CPU power — use up to").pack(side=tk.LEFT)
        self._cores_lbl = ttk.Label(cpu, width=3,
                                    text=str(self.max_cores_var.get()))
        ttk.Scale(cpu, from_=1, to=max(1, cores), orient="horizontal",
                  length=160, variable=self.max_cores_var,
                  command=lambda _v: self._on_cores()
                  ).pack(side=tk.LEFT, padx=(6, 4))
        self._cores_lbl.pack(side=tk.LEFT)
        ttk.Label(cpu, text="of %d cores" % cores,
                  foreground="#888").pack(side=tk.LEFT, padx=(4, 0))

        ttk.Checkbutton(f, text="caffeinate", variable=self.caffeinate_var
                        ).grid(row=3, column=0, sticky="w", pady=(6, 0))
        ttk.Checkbutton(f, text="nice -n 10", variable=self.nice_var
                        ).grid(row=3, column=1, sticky="w", pady=(6, 0))

        # ---- benchmark ----
        bench = ttk.Frame(f); bench.grid(row=4, column=0, columnspan=4,
                                         sticky="we", pady=(8, 0))
        self._bench_btn = ttk.Button(bench, text="Run benchmark",
                                     command=self._start_benchmark)
        self._bench_btn.pack(side=tk.LEFT)
        ttk.Label(bench, foreground="#888",
                  text="  times a %d-event sample to predict & recommend"
                       % int(s["bench_events"])).pack(side=tk.LEFT)

        self.bench_lbl = ttk.Label(f, text="", foreground="#367", justify="left")
        self.bench_lbl.grid(row=5, column=0, columnspan=4, sticky="we",
                            pady=(4, 0))

        self.advice_lbl = ttk.Label(f, text="", foreground="#445",
                                    justify="left")
        self.advice_lbl.grid(row=6, column=0, columnspan=3, sticky="we",
                             pady=(8, 0))
        ttk.Button(f, text="↻", width=3, command=self.refresh
                   ).grid(row=6, column=3, sticky="e", pady=(8, 0))

        f.columnconfigure(0, weight=1)
        self.refresh()

    # ------------------------------------------------------------------ jobs
    def _on_cores(self) -> None:
        n = max(1, int(self.max_cores_var.get()))
        self.state["parallel"]["max_cores"] = n
        self._cores_lbl.configure(text=str(n))
        self.refresh()

    def refresh(self) -> None:
        rungs = self._rung_count()
        a = advise(rungs)
        mode = self.mode_var.get()
        cap = max(1, int(self.max_cores_var.get()))

        if mode == "seq":
            resolved = 1
            msg = f"{a.note}  →  sequential, 1 process at a time"
        elif mode == "auto":
            resolved = min(a.jobs, cap)
            msg = f"{a.note}  →  auto picks {resolved} job(s) (≤ {cap} cores)"
        else:                                            # manual
            resolved = min(max(1, self.jobs_var.get()), cap)
            msg = f"{a.note}  →  manual: {resolved} job(s) (≤ {cap} cores)"
            if self.jobs_var.get() > a.jobs:
                msg += f"   ⚠ above advisor's safe limit ({a.jobs}) — OOM risk"
        self.advice_lbl.configure(text=msg)
        self.state["_resolved_jobs"] = resolved

    def _rung_count(self) -> int:
        rungs = self.state.get("ladder_rungs")
        if isinstance(rungs, list) and rungs:
            return max(1, len(rungs))
        ladder = self.state.get("ladder", {})
        return max(1, sum(1 for v in ladder.values() if v)) if ladder else 1

    # ------------------------------------------------------------- benchmark
    def _set_bench_busy(self, busy: bool) -> None:
        self._bench_running = busy
        self._bench_btn.configure(state=tk.DISABLED if busy else tk.NORMAL,
                                  text="Benchmarking…" if busy
                                  else "Run benchmark")

    def _start_benchmark(self) -> None:
        if self._bench_running:
            return
        repo = Path(self.state["repo"])
        study_bin = str(repo / "bin" / "provenance-study")
        params = dict(
            study_bin=study_bin,
            events=int(self.state.get("events", 20000) or 20000),
            rungs=self._rung_count(),
            species=str(self.state.get("species", "211") or "211"),
            ecm=str(float(self.state.get("ecm", 13000.0) or 13000.0)),
            seed=str(int(self.state.get("seed", 12345) or 12345)),
            bench_events=int(self.state["parallel"].get("bench_events", 150)),
            cores=max(1, int(self.max_cores_var.get())),
            ram_jobs=advise(self._rung_count()).jobs,
        )
        self._set_bench_busy(True)
        self.bench_lbl.configure(text="running calibration…")
        threading.Thread(target=self._bench_worker, args=(params,),
                         daemon=True).start()

    def _bench_worker(self, params: dict) -> None:
        try:
            res = run_benchmark(**params)
        except Exception as exc:                          # noqa: BLE001
            res = exc
        # Bounce back to the Tk thread.
        self.frame.after(0, lambda: self._bench_done(res, params))

    def _bench_done(self, res, params: dict) -> None:
        self._set_bench_busy(False)
        if isinstance(res, Exception):
            self.bench_lbl.configure(
                text="benchmark failed: %r" % res, foreground="#a33")
            return
        if not res.ok or res.rec is None:
            self.bench_lbl.configure(
                text="benchmark could not run: %s\n(needs a current "
                     "bin/provenance-study — build/install it first)" % res.note,
                foreground="#a33")
            return
        rec = res.rec
        text = (
            "measured %.0f evt/s (1 proc)" % res.rate1
            + ("  ·  parallel %.0f%% efficient at %d proc"
               % (100 * res.efficiency, res.probe_jobs)
               if res.probe_jobs > 1 else "")
            + "\nfull run (%d evt × %d rung): single %s"
              % (params["events"], params["rungs"],
                 fmt_seconds(rec.pred_single_s))
            + ("  ·  recommended %s" % fmt_seconds(rec.pred_rec_s)
               if rec.mode == "parallel" else "")
            + "\n→ %s" % rec.note)
        self.bench_lbl.configure(text=text, foreground="#367")

        # Apply the recommendation to the controls.
        if rec.mode == "parallel":
            self.mode_var.set("manual")
            self.jobs_var.set(rec.jobs)
        else:
            self.mode_var.set("seq")
        self.refresh()
