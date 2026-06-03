"""Kinematic acceptance window + multiplicity-bin thresholds.

Both are physics knobs (audit fixes #3 and #9): pT/eta cuts define the
phase-space window the analysis is restricted to, and the multiplicity
cuts define the Low / Mid / High event-multiplicity bins used to split
every pair histogram.  All values are forwarded to provenance-study and
through cap-mechanism-ladder to every rung.

Defaults are 'no cut' for kinematics and 20/80 for multiplicity (the old
hardcoded values).  Leaving a kinematic field blank disables that cut.
"""
from __future__ import annotations

import tkinter as tk
from tkinter import ttk

from .base import Panel, bind_to_state


class AcceptancePanel(Panel):
    title = "Acceptance / multiplicity bins"

    def build(self) -> None:
        s = self.state.setdefault("acceptance", {
            "pt_min":  "",     "pt_max":  "",
            "eta_min": "",     "eta_max": "",
            "mult_low":  20,   "mult_high": 80,
            "sphero_low":  0.3, "sphero_high": 0.7,
            "validate_graph": False,
            "dump_events":   0,    # 0 = don't dump; N>0 = first N events
        })

        self.pt_min      = tk.StringVar(value=str(s["pt_min"]))
        self.pt_max      = tk.StringVar(value=str(s["pt_max"]))
        self.eta_min     = tk.StringVar(value=str(s["eta_min"]))
        self.eta_max     = tk.StringVar(value=str(s["eta_max"]))
        self.mlow        = tk.IntVar   (value=int(s["mult_low"]))
        self.mhigh       = tk.IntVar   (value=int(s["mult_high"]))
        self.slow        = tk.DoubleVar(value=float(s.get("sphero_low", 0.3)))
        self.shigh       = tk.DoubleVar(value=float(s.get("sphero_high", 0.7)))
        self.validate    = tk.BooleanVar(value=bool(s["validate_graph"]))
        self.dump_events = tk.IntVar   (value=int(s.get("dump_events", 0)))
        for var, key in [(self.pt_min,      "pt_min"),
                         (self.pt_max,      "pt_max"),
                         (self.eta_min,     "eta_min"),
                         (self.eta_max,     "eta_max"),
                         (self.mlow,        "mult_low"),
                         (self.mhigh,       "mult_high"),
                         (self.slow,        "sphero_low"),
                         (self.shigh,       "sphero_high"),
                         (self.validate,    "validate_graph"),
                         (self.dump_events, "dump_events")]:
            bind_to_state(var, s, key)

        f = self.frame
        # Kinematic window — left half.
        ttk.Label(f, text="pT [GeV]").grid(row=0, column=0, sticky="w")
        ttk.Entry(f, textvariable=self.pt_min, width=8
                  ).grid(row=0, column=1, sticky="w", padx=(8, 0))
        ttk.Label(f, text="–").grid(row=0, column=2, padx=(2, 2))
        ttk.Entry(f, textvariable=self.pt_max, width=8
                  ).grid(row=0, column=3, sticky="w")

        ttk.Label(f, text="η").grid(row=1, column=0, sticky="w",
                                    pady=(4, 0))
        ttk.Entry(f, textvariable=self.eta_min, width=8
                  ).grid(row=1, column=1, sticky="w", padx=(8, 0),
                         pady=(4, 0))
        ttk.Label(f, text="–").grid(row=1, column=2, padx=(2, 2),
                                    pady=(4, 0))
        ttk.Entry(f, textvariable=self.eta_max, width=8
                  ).grid(row=1, column=3, sticky="w", pady=(4, 0))

        ttk.Label(f, text="leave blank for no cut",
                  foreground="#888").grid(row=2, column=0, columnspan=4,
                                          sticky="w", pady=(2, 6))

        # Multiplicity-bin thresholds — right half.
        ttk.Separator(f, orient="vertical"
                      ).grid(row=0, column=4, rowspan=6, sticky="ns",
                             padx=(16, 16))
        ttk.Label(f, text="Low / Mid boundary").grid(row=0, column=5,
                                                     sticky="w")
        ttk.Spinbox(f, from_=1, to=10000, increment=1, width=6,
                    textvariable=self.mlow
                    ).grid(row=0, column=6, sticky="w", padx=(8, 0))
        ttk.Label(f, text="Mid / High boundary").grid(row=1, column=5,
                                                      sticky="w",
                                                      pady=(4, 0))
        ttk.Spinbox(f, from_=1, to=10000, increment=1, width=6,
                    textvariable=self.mhigh
                    ).grid(row=1, column=6, sticky="w", padx=(8, 0),
                           pady=(4, 0))
        ttk.Label(f, text="hadrons / event",
                  foreground="#888").grid(row=2, column=5, columnspan=2,
                                          sticky="w", pady=(2, 6))

        # Spherocity (event-shape) bin thresholds — sits below the
        # multiplicity boundaries on the right side of the grid.
        ttk.Label(f, text="JetLike / MidShape S0").grid(
            row=3, column=5, sticky="w", pady=(4, 0))
        ttk.Spinbox(f, from_=0.0, to=1.0, increment=0.05, width=6,
                    format="%.2f", textvariable=self.slow
                    ).grid(row=3, column=6, sticky="w", padx=(8, 0),
                           pady=(4, 0))
        ttk.Label(f, text="MidShape / Isotropic S0").grid(
            row=4, column=5, sticky="w", pady=(2, 0))
        ttk.Spinbox(f, from_=0.0, to=1.0, increment=0.05, width=6,
                    format="%.2f", textvariable=self.shigh
                    ).grid(row=4, column=6, sticky="w", padx=(8, 0),
                           pady=(2, 0))
        ttk.Label(f, text="transverse spherocity, [0,1]",
                  foreground="#888").grid(row=5, column=5, columnspan=2,
                                          sticky="w", pady=(2, 6))

        # Full-width footer rows BELOW both columns (right side uses rows 0-5).
        ttk.Separator(f, orient="horizontal"
                      ).grid(row=6, column=0, columnspan=7,
                             sticky="ew", pady=(8, 4))
        ttk.Label(f, text="Dump first N events (event-history JSON)"
                  ).grid(row=7, column=0, columnspan=3, sticky="w")
        ttk.Spinbox(f, from_=0, to=10000, increment=10, width=6,
                    textvariable=self.dump_events
                    ).grid(row=7, column=3, sticky="w")
        ttk.Label(f, text="(0 = off; recommended ≤ 200 for interactive use)",
                  foreground="#888"
                  ).grid(row=7, column=4, columnspan=3, sticky="w", padx=(8, 0))

        ttk.Checkbutton(f, text="--validate-graph (abort on first DAG "
                        "consistency failure)",
                        variable=self.validate
                        ).grid(row=8, column=0, columnspan=7,
                               sticky="w", pady=(4, 0))
