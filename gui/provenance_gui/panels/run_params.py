"""Run parameters — events, energy, seed, output root.

Particle/species selection lives in its own 'Particles & pairs' tab now;
this panel keeps just the simple top-level run knobs.
"""
from __future__ import annotations

import tkinter as tk
from pathlib import Path
from tkinter import filedialog, ttk

from .base import Panel, bind_to_state


class RunParamsPanel(Panel):
    title = "Run"

    def build(self) -> None:
        s = self.state
        s.setdefault("events",  20000)
        s.setdefault("ecm",     13000.0)
        s.setdefault("seed",    12345)
        s.setdefault("outdir",  str(s["repo"] / "provenance"))
        # Chunked-resume size: runs over this many events auto-split into
        # resumable chunks (0 = never chunk).  Default protects long runs.
        s.setdefault("chunk_events", 50000)

        self.events_var  = tk.IntVar   (value=s["events"])
        self.ecm_var     = tk.DoubleVar(value=s["ecm"])
        self.seed_var    = tk.IntVar   (value=s["seed"])
        self.chunk_var   = tk.IntVar   (value=int(s["chunk_events"]))
        self.outdir_var  = tk.StringVar(value=s["outdir"])
        for var, key in [(self.events_var,  "events"),
                         (self.ecm_var,     "ecm"),
                         (self.seed_var,    "seed"),
                         (self.chunk_var,   "chunk_events"),
                         (self.outdir_var,  "outdir")]:
            bind_to_state(var, s, key)

        f = self.frame
        ttk.Label(f, text="Events").grid(row=0, column=0, sticky="w")
        ttk.Spinbox(f, from_=1000, to=10_000_000, increment=10000, width=10,
                    textvariable=self.events_var
                    ).grid(row=0, column=1, sticky="w", padx=(8, 24))

        ttk.Label(f, text="√s [GeV]").grid(row=0, column=2, sticky="w")
        ttk.Spinbox(f, from_=200.0, to=14000.0, increment=100.0, width=10,
                    textvariable=self.ecm_var
                    ).grid(row=0, column=3, sticky="w", padx=(8, 0))

        ttk.Label(f, text="Seed").grid(row=1, column=0, sticky="w",
                                       pady=(6, 0))
        ttk.Spinbox(f, from_=1, to=999_999_999, increment=1, width=10,
                    textvariable=self.seed_var
                    ).grid(row=1, column=1, sticky="w", padx=(8, 24),
                           pady=(6, 0))

        ttk.Label(f, text="Resume chunk").grid(row=1, column=2, sticky="w",
                                               pady=(6, 0))
        ttk.Spinbox(f, from_=0, to=10_000_000, increment=25000, width=10,
                    textvariable=self.chunk_var
                    ).grid(row=1, column=3, sticky="w", padx=(8, 0),
                           pady=(6, 0))

        ttk.Label(f, text="Output").grid(row=2, column=0, sticky="w",
                                         pady=(6, 0))
        ttk.Entry(f, textvariable=self.outdir_var, width=48
                  ).grid(row=2, column=1, sticky="we", columnspan=2,
                         padx=(8, 4), pady=(6, 0))
        ttk.Button(f, text="…", width=3,
                   command=self._pick).grid(row=2, column=3, sticky="w",
                                            pady=(6, 0))
        f.columnconfigure(1, weight=1)
        f.columnconfigure(2, weight=1)

    def _pick(self) -> None:
        d = filedialog.askdirectory(initialdir=self.outdir_var.get(),
                                    title="Output root")
        if d:
            self.outdir_var.set(d)
