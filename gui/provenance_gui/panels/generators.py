"""Generator selector — which event generator(s) the study uses.

Selecting both Pythia and Herwig turns the study into a generator comparison:
the pipeline runs the provenance analysis once per generator into a shared
comparison directory, and the plotter overlays them (the same overlay it uses
for mechanism-ladder rungs).  Configure each generator on its own tab.

State: state['generators'] = ['pythia'] | ['herwig'] | ['pythia','herwig'].
"""
from __future__ import annotations

import tkinter as tk
from tkinter import filedialog, ttk

from .base import Panel, bind_to_state
from ..mechanisms import common_mechanisms


class GeneratorSelectPanel(Panel):
    title = "Generators"

    def build(self) -> None:
        gens = self.state.setdefault("generators", ["pythia"])
        self.state.setdefault("compare_mechanisms", False)

        f = self.frame
        self._py = tk.BooleanVar(value="pythia" in gens)
        self._hw = tk.BooleanVar(value="herwig" in gens)
        ttk.Checkbutton(f, text="Pythia 8", variable=self._py,
                        command=self._sync).grid(row=0, column=0, sticky="w")
        ttk.Checkbutton(f, text="Herwig 7", variable=self._hw,
                        command=self._sync).grid(row=0, column=1, sticky="w",
                                                 padx=(16, 0))

        self._cmp = tk.BooleanVar(value=bool(self.state["compare_mechanisms"]))
        self._cmp_cb = ttk.Checkbutton(
            f, text="Compare the same mechanisms across generators (%s)"
                    % ", ".join(common_mechanisms()),
            variable=self._cmp, command=self._sync)
        self._cmp_cb.grid(row=1, column=0, columnspan=3, sticky="w",
                          pady=(4, 0))

        # Herwig .hepmc input (Herwig → .hepmc via the main GUI; provenance-
        # study reads it with --hepmc3).  Shown only when Herwig is selected.
        self.state.setdefault("herwig_hepmc", "")
        self._hepmc_row = ttk.Frame(f)
        self._hepmc_row.grid(row=2, column=0, columnspan=3, sticky="we",
                             pady=(4, 0))
        ttk.Label(self._hepmc_row, text="Herwig .hepmc:").pack(side=tk.LEFT)
        self._hepmc_var = tk.StringVar(value=self.state["herwig_hepmc"])
        bind_to_state(self._hepmc_var, self.state, "herwig_hepmc")
        ttk.Entry(self._hepmc_row, textvariable=self._hepmc_var, width=44
                  ).pack(side=tk.LEFT, padx=(6, 4))
        ttk.Button(self._hepmc_row, text="…", width=3,
                   command=self._pick_hepmc).pack(side=tk.LEFT)

        self._note = ttk.Label(f, foreground="#888", justify="left")
        self._note.grid(row=3, column=0, columnspan=3, sticky="w", pady=(4, 0))
        self._sync()

    def _pick_hepmc(self) -> None:
        p = filedialog.askopenfilename(
            title="Herwig HepMC3 file",
            filetypes=[("HepMC3", "*.hepmc *.hepmc3"), ("All", "*.*")])
        if p:
            self._hepmc_var.set(p)

    def _sync(self) -> None:
        gens = []
        if self._py.get():
            gens.append("pythia")
        if self._hw.get():
            gens.append("herwig")
        if not gens:                       # never leave it empty
            gens = ["pythia"]
            self._py.set(True)
        self.state["generators"] = gens

        multi = len(gens) > 1
        # The cross-mechanism toggle only applies to a multi-generator compare.
        self.state["compare_mechanisms"] = bool(self._cmp.get() and multi)
        self._cmp_cb.configure(state=tk.NORMAL if multi else tk.DISABLED)

        # The .hepmc field is only relevant when Herwig is in play.
        if "herwig" in gens:
            self._hepmc_row.grid()
        else:
            self._hepmc_row.grid_remove()

        if gens == ["pythia"]:
            msg = "Single generator: Pythia 8 (configure on the Pythia tab)."
        elif gens == ["herwig"]:
            msg = ("Single generator: Herwig 7 (configure on the Herwig tab). "
                   "Needs the engine HepMC3 input mode — see COORD [B] needs.")
        elif self.state["compare_mechanisms"]:
            msg = ("Mechanism comparison: the ladder rungs from the Pythia tab "
                   "are run in BOTH generators (varying only %s, the mechanisms "
                   "both share) and overlaid as 'Pythia · rung' vs "
                   "'Herwig · rung'. Herwig runs pending the engine flag."
                   % ", ".join(common_mechanisms()))
        else:
            msg = ("Comparison mode: each generator is run into the same "
                   "directory and overlaid. Herwig execution needs the engine "
                   "HepMC3 input mode (pending) — Pythia runs today.")
        self._note.configure(text=msg)
