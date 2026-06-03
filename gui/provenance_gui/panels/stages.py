"""Pipeline-stage toggles."""
from __future__ import annotations

import tkinter as tk
from tkinter import ttk

from .base import Panel


class StagesPanel(Panel):
    title = "Stages"

    STAGES = [
        ("standalone", "provenance-study"),
        ("ladder",     "mechanism ladder"),
        ("plot",       "overlay plots"),
        ("report",     "LaTeX report"),
        ("pdflatex",   "compile PDF"),
        ("open_pdf",   "open PDF when done"),
    ]

    def build(self) -> None:
        s = self.state.setdefault("stages",
                                  {k: True for k, _ in self.STAGES})
        self.vars: dict[str, tk.BooleanVar] = {}
        for i, (key, label) in enumerate(self.STAGES):
            v = tk.BooleanVar(value=s.get(key, True))
            v.trace_add("write",
                        lambda *_a, k=key, v=v: s.__setitem__(k, v.get()))
            ttk.Checkbutton(self.frame, text=label, variable=v
                            ).grid(row=i // 3, column=i % 3,
                                   sticky="w", padx=(0, 24), pady=2)
            self.vars[key] = v
