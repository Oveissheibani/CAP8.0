"""Report mode selection."""
from __future__ import annotations

import tkinter as tk
from tkinter import ttk

from .base import Panel


class ReportPanel(Panel):
    title = "Report"

    def build(self) -> None:
        s = self.state.setdefault("report", {"mode": "paper"})
        self.mode_var = tk.StringVar(value=s["mode"])
        self.mode_var.trace_add(
            "write",
            lambda *_a: s.__setitem__("mode", self.mode_var.get()))
        ttk.Label(self.frame, text="Document").grid(row=0, column=0,
                                                    sticky="w")
        ttk.Radiobutton(self.frame, text="paper",        value="paper",
                        variable=self.mode_var
                        ).grid(row=0, column=1, sticky="w", padx=(8, 0))
        ttk.Radiobutton(self.frame, text="presentation", value="presentation",
                        variable=self.mode_var
                        ).grid(row=0, column=2, sticky="w", padx=(12, 0))
