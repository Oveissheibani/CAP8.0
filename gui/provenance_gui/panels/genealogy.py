"""Genealogy study request.

This panel replaces the old DAG explorer.  Instead of browsing one event's
ancestry graph, the user *declares the study they want*: how far back to trace
each studied hadron, how deep, and which ancestry dimensions to decompose the
observables by.  The result is a structured request in ``state['genealogy']``
plus a plain-language summary the user can read back.

Conceptually this is the genealogy analogue of the particle/pair request: the
user composes WHAT to ask of the event history, not a fixed canned view.

State written:
  state['genealogy'] = {
    'boundary'     : earliest stage to trace back to,
    'max_depth'    : decay-chain depth cap,
    'dims'         : {dimension_key: bool},   # decompose-by selections
    'apply_single' : bool,                    # single-particle observables
    'apply_pair'   : bool,                     # two-particle observables
  }
"""
from __future__ import annotations

import tkinter as tk
from tkinter import ttk

from .base import Panel

# Earliest stage the trace may stop at (mirrors StageTaxonomy.hpp ordering).
BOUNDARIES = [
    ("Beam (full event history)",          "Beam"),
    ("Hard process",                       "HardProcess"),
    ("Pre-hadronization partons",          "PartonsPreHadronization"),
    ("Primary hadrons only",               "PrimaryHadrons"),
]

# Ancestry dimensions the study can decompose observables by.  Keys match the
# provenance tags the engine records.
DIMENSIONS = [
    ("origin",      "Production origin  (primary / resonance / weak decay)", True),
    ("parton",      "Ancestor parton flavour",                              True),
    ("shower",      "Shower lineage  (ISR / FSR)",                          False),
    ("mpi",         "MPI relationship",                                     False),
    ("hf",          "Heavy-flavour decay chain  (charm / bottom)",          False),
    ("decay_depth", "Decay-chain depth",                                    False),
]


class GenealogyRequestPanel(Panel):
    title = ""        # rendered straight into a notebook tab

    def build(self) -> None:
        self.frame = ttk.Frame(self.frame.master, padding=12)

        g = self.state.setdefault("genealogy", {})
        g.setdefault("boundary", "Beam")
        g.setdefault("max_depth", 20)
        g.setdefault("dims", {k: d for k, _l, d in DIMENSIONS})
        g.setdefault("apply_single", True)
        g.setdefault("apply_pair", True)

        ttk.Label(self.frame,
                  text="Request a genealogy study",
                  font=("Helvetica", 13, "bold")).pack(anchor="w")
        ttk.Label(self.frame, foreground="#666",
                  text="Describe how far back to trace each studied hadron "
                       "and which ancestry dimensions to decompose by.").pack(
            anchor="w", pady=(0, 10))

        body = ttk.PanedWindow(self.frame, orient=tk.HORIZONTAL)
        body.pack(fill=tk.BOTH, expand=True)
        left = ttk.Frame(body)
        right = ttk.Frame(body)
        body.add(left, weight=3)
        body.add(right, weight=2)

        # ---- left: the request controls ----
        scope = ttk.LabelFrame(left, text=" Trace scope ", padding=10)
        scope.pack(fill=tk.X)
        ttk.Label(scope, text="Track ancestry back to").grid(row=0, column=0,
                                                             sticky="w")
        self._bound_var = tk.StringVar(
            value=self._label_for(g["boundary"]))
        ttk.Combobox(scope, textvariable=self._bound_var, state="readonly",
                     width=30, values=[lbl for lbl, _v in BOUNDARIES]
                     ).grid(row=0, column=1, sticky="w", padx=(8, 0))
        ttk.Label(scope, text="Max decay-chain depth").grid(row=1, column=0,
                                                            sticky="w",
                                                            pady=(8, 0))
        self._depth_var = tk.IntVar(value=int(g["max_depth"]))
        ttk.Spinbox(scope, from_=1, to=50, width=6,
                    textvariable=self._depth_var
                    ).grid(row=1, column=1, sticky="w", padx=(8, 0),
                           pady=(8, 0))

        dims = ttk.LabelFrame(left, text=" Decompose observables by ",
                              padding=10)
        dims.pack(fill=tk.X, pady=(10, 0))
        self._dim_vars: dict[str, tk.BooleanVar] = {}
        for key, label, _d in DIMENSIONS:
            v = tk.BooleanVar(value=bool(g["dims"].get(key, _d)))
            self._dim_vars[key] = v
            ttk.Checkbutton(dims, text=label, variable=v,
                            command=self._commit).pack(anchor="w")

        applyf = ttk.LabelFrame(left, text=" Apply to ", padding=10)
        applyf.pack(fill=tk.X, pady=(10, 0))
        self._single_var = tk.BooleanVar(value=bool(g["apply_single"]))
        self._pair_var = tk.BooleanVar(value=bool(g["apply_pair"]))
        ttk.Checkbutton(applyf, text="Single-particle observables "
                        "(pT, eta)", variable=self._single_var,
                        command=self._commit).pack(anchor="w")
        ttk.Checkbutton(applyf, text="Two-particle observables "
                        "(dphi, deta, mass)", variable=self._pair_var,
                        command=self._commit).pack(anchor="w")

        # ---- right: live request summary ----
        ttk.Label(right, text="Request summary", foreground="#666").pack(
            anchor="w")
        self._summary = tk.Text(right, wrap="word", height=14,
                                font=("Menlo", 10), background="#0e1116",
                                foreground="#cbd0d6")
        self._summary.pack(fill=tk.BOTH, expand=True, pady=(2, 0))
        ttk.Label(right, foreground="#888",
                  text="Note: boundary / depth limits need the engine "
                       "--genealogy-stop flag (pending); the decompose-by "
                       "selection already shapes the figures and report.").pack(
            anchor="w", pady=(6, 0))

        # Wire change handlers.
        self._bound_var.trace_add("write", lambda *_a: self._commit())
        self._depth_var.trace_add("write", lambda *_a: self._commit())
        self._commit()

    # ------------------------------------------------------------------ utils
    @staticmethod
    def _label_for(value: str) -> str:
        for lbl, v in BOUNDARIES:
            if v == value:
                return lbl
        return BOUNDARIES[0][0]

    @staticmethod
    def _value_for(label: str) -> str:
        for lbl, v in BOUNDARIES:
            if lbl == label:
                return v
        return "Beam"

    def _commit(self) -> None:
        g = self.state["genealogy"]
        g["boundary"] = self._value_for(self._bound_var.get())
        try:
            g["max_depth"] = int(self._depth_var.get())
        except (tk.TclError, ValueError):
            pass
        g["dims"] = {k: v.get() for k, v in self._dim_vars.items()}
        g["apply_single"] = self._single_var.get()
        g["apply_pair"] = self._pair_var.get()
        self._render_summary(g)

    def _render_summary(self, g: dict) -> None:
        chosen = [label for key, label, _d in DIMENSIONS if g["dims"].get(key)]
        scopes = []
        if g["apply_single"]:
            scopes.append("single-particle")
        if g["apply_pair"]:
            scopes.append("two-particle")
        lines = [
            "Trace each studied hadron back to:",
            "    %s" % self._label_for(g["boundary"]),
            "",
            "Max decay-chain depth: %d" % g["max_depth"],
            "",
            "Decompose %s observables by:" % (" & ".join(scopes) or "(none)"),
        ]
        if chosen:
            lines += ["    - " + c.split("  ")[0] for c in chosen]
        else:
            lines.append("    (no dimensions selected)")
        self._summary.configure(state=tk.NORMAL)
        self._summary.delete("1.0", tk.END)
        self._summary.insert(tk.END, "\n".join(lines) + "\n")
