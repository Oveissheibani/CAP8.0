"""Ladder builder — derive the rungs from how you toggle mechanisms.

You don't hand-build each rung.  Instead you classify each mechanism as:

    off       — never on in any rung
    baseline  — on in EVERY rung (your fixed configuration)
    vary      — swept across rungs (this is what the ladder studies)

…and pick a derivation mode:

    cumulative          — start from the baseline and switch the varied
                          mechanisms on one at a time (the classic
                          shower -> +MPI -> +MPI+CR -> ... sweep)
    combinations        — every on/off combination of the varied mechanisms
                          (2^k rungs)

The panel derives the rung list and shows it live, so you can see exactly which
Pythia runs the ladder will launch.  The Pythia configuration shown below this
panel is the shared BASE spliced into every rung (via --extra-cmnd); the five
mechanisms here are what cap-mechanism-ladder flips per rung.

State:
  state['ladder_axes']  = {'choice': {mech: 'off'|'baseline'|'vary'},
                           'mode': 'cumulative'|'combinations'}
  state['ladder_rungs'] = derived [{'name','mechs','overrides'}, ...]
"""
from __future__ import annotations

import itertools
import tkinter as tk
from tkinter import ttk

from .base import Panel
from ..system_info import advise
from ..mechanisms import availability, common_mechanisms

MECHS = ["ISR", "FSR", "MPI", "CR", "rope"]
CHOICES = ["off", "baseline", "vary"]


class LadderDesignerPanel(Panel):
    title = ""        # packed into the Pythia tab by the App

    def build(self) -> None:
        self.frame = ttk.LabelFrame(
            self.frame.master,
            text=" Ladder builder — derive rungs from mechanism toggles ",
            padding=10)

        self._ax = self.state.setdefault("ladder_axes", {
            "choice": {"ISR": "baseline", "FSR": "baseline",
                       "MPI": "vary", "CR": "vary", "rope": "vary"},
            "mode": "cumulative",
        })
        self.state.setdefault("ladder_rungs", [])

        ttk.Label(self.frame, foreground="#666", justify="left", wraplength=860,
                  text="Classify each mechanism, choose how to sweep, and the "
                       "ladder is derived below. The Pythia config under this "
                       "panel is the shared base for every rung.").pack(
            anchor="w", pady=(0, 8))
        ttk.Label(self.frame, foreground="#888", justify="left", wraplength=860,
                  text="Cross-generator: " + " · ".join(
                      "%s = %s" % (m, availability(m)) for m in MECHS)
                  + ".  Only %s carry over to Herwig, so a Pythia/Herwig "
                    "mechanism comparison varies those." % (
                      ", ".join(common_mechanisms()))).pack(
            anchor="w", pady=(0, 8))

        # ---- derivation mode ----
        modefr = ttk.Frame(self.frame); modefr.pack(fill=tk.X)
        ttk.Label(modefr, text="Sweep:").pack(side=tk.LEFT)
        self._mode_var = tk.StringVar(value=self._ax.get("mode", "cumulative"))
        ttk.Radiobutton(modefr, text="Cumulative (add one at a time)",
                        value="cumulative", variable=self._mode_var,
                        command=self._on_mode).pack(side=tk.LEFT, padx=(6, 0))
        ttk.Radiobutton(modefr, text="All on/off combinations",
                        value="combinations", variable=self._mode_var,
                        command=self._on_mode).pack(side=tk.LEFT, padx=(14, 0))

        # ---- one-click presets (easier design) ----
        pf = ttk.Frame(self.frame); pf.pack(fill=tk.X, pady=(6, 0))
        ttk.Label(pf, text="Quick:").pack(side=tk.LEFT)
        for label, choice, mode in (
            ("Classic sweep",
             {"ISR": "baseline", "FSR": "baseline", "MPI": "vary",
              "CR": "vary", "rope": "vary"}, "cumulative"),
            ("MPI on/off",
             {"ISR": "baseline", "FSR": "baseline", "MPI": "vary",
              "CR": "baseline", "rope": "off"}, "combinations"),
            ("CR on/off",
             {"ISR": "baseline", "FSR": "baseline", "MPI": "baseline",
              "CR": "vary", "rope": "off"}, "combinations"),
            ("Pythia↔Herwig (MPI,CR)",
             {"ISR": "baseline", "FSR": "baseline", "MPI": "vary",
              "CR": "vary", "rope": "off"}, "combinations"),
        ):
            ttk.Button(pf, text=label, width=20,
                       command=lambda c=choice, m=mode: self._apply_preset(c, m)
                       ).pack(side=tk.LEFT, padx=(6, 0))

        # ---- per-mechanism classification + live preview, side by side ----
        body = ttk.Frame(self.frame); body.pack(fill=tk.X, pady=(8, 0))
        grid = ttk.Frame(body); grid.pack(side=tk.LEFT, anchor="n")
        ttk.Label(grid, text="mechanism", width=10).grid(row=0, column=0,
                                                         sticky="w")
        ttk.Label(grid, text="role").grid(row=0, column=1, sticky="w",
                                          padx=(8, 0))
        self._choice_vars: dict[str, tk.StringVar] = {}
        for i, m in enumerate(MECHS):
            ttk.Label(grid, text=m, width=10).grid(row=i + 1, column=0,
                                                   sticky="w", pady=1)
            v = tk.StringVar(value=self._ax["choice"].get(m, "off"))
            self._choice_vars[m] = v
            ttk.Combobox(grid, textvariable=v, values=CHOICES,
                         state="readonly", width=10
                         ).grid(row=i + 1, column=1, sticky="w", padx=(8, 0))
            v.trace_add("write",
                        lambda *_a, mm=m, vv=v: self._set_choice(mm, vv.get()))

        prev = ttk.Frame(body); prev.pack(side=tk.LEFT, anchor="n", padx=(24, 0))
        ttk.Label(prev, text="derived rungs", foreground="#445").pack(anchor="w")
        self._preview = tk.Text(prev, width=40, height=8, wrap="none",
                                font=("Menlo", 10), background="#0e1116",
                                foreground="#cbd0d6")
        self._preview.pack(fill=tk.BOTH, expand=True)

        self._conn = ttk.Label(self.frame, foreground="#367", justify="left")
        self._conn.pack(anchor="w", pady=(8, 0))

        self._derive()

    # --------------------------------------------------------------- edits
    def _on_mode(self) -> None:
        self._ax["mode"] = self._mode_var.get()
        self._derive()

    def _apply_preset(self, choice: dict, mode: str) -> None:
        self._ax["choice"] = dict(choice)
        self._ax["mode"] = mode
        self._mode_var.set(mode)
        for m, v in self._choice_vars.items():
            v.set(choice.get(m, "off"))      # trace updates _ax + re-derives
        self._derive()

    def _set_choice(self, mech: str, role: str) -> None:
        self._ax["choice"][mech] = role
        self._derive()

    # --------------------------------------------------------------- derive
    def _derive(self) -> None:
        choice = self._ax["choice"]
        mode = self._ax["mode"]
        baseline = [m for m in MECHS if choice.get(m) == "baseline"]
        vary = [m for m in MECHS if choice.get(m) == "vary"]

        rung_mechs: list[list[str]] = []
        if mode == "combinations":
            for r in range(len(vary) + 1):
                for combo in itertools.combinations(vary, r):
                    rung_mechs.append(
                        sorted(baseline + list(combo), key=MECHS.index))
        else:  # cumulative
            rung_mechs.append(sorted(baseline, key=MECHS.index))
            cum = list(baseline)
            for v in vary:
                cum = cum + [v]
                rung_mechs.append(sorted(cum, key=MECHS.index))

        # Build named, de-duplicated rung records.
        out: list[dict] = []
        seen: set[tuple] = set()
        for mechs in rung_mechs:
            key = tuple(mechs)
            if key in seen:
                continue
            seen.add(key)
            out.append({"name": "+".join(mechs) if mechs else "none",
                        "mechs": mechs, "overrides": ""})
        self.state["ladder_rungs"] = out

        # Preview.
        self._preview.configure(state=tk.NORMAL)
        self._preview.delete("1.0", tk.END)
        for i, r in enumerate(out, 1):
            self._preview.insert(tk.END, "%2d. %s\n" % (i, r["name"]))
        self._preview.configure(state=tk.DISABLED)

        # Connection to parallelism.
        n = len(out)
        a = advise(n)
        if n <= 1:
            txt = "→ 1 run. A single run is single-threaded — parallel won't help."
        else:
            txt = ("→ %d independent runs. Parallel-auto would launch %d at a "
                   "time (cores=%d); set on the Run tab → Parallelism."
                   % (n, min(a.jobs, n), a.p_cores))
        self._conn.configure(text=txt)

        # Tell the parallelism advisor the rung count changed.
        cb = self.state.get("_on_ladder_change")
        if callable(cb):
            cb()
