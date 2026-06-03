"""Live output pane — terminal-like, with elapsed-time and save-log."""
from __future__ import annotations

import time
import tkinter as tk
from pathlib import Path
from tkinter import filedialog, ttk

from .base import Panel


class LogPanel(Panel):
    title = ""

    def build(self) -> None:
        # Replace default LabelFrame with a plain Frame for a notebook tab.
        self.frame = ttk.Frame(self.frame.master, padding=4)

        bar = ttk.Frame(self.frame); bar.pack(fill=tk.X)
        ttk.Button(bar, text="Clear", command=self.clear).pack(side=tk.LEFT)
        ttk.Button(bar, text="Save…", command=self.save
                   ).pack(side=tk.LEFT, padx=(6, 0))
        self.elapsed_var = tk.StringVar(value="—")
        ttk.Label(bar, textvariable=self.elapsed_var,
                  foreground="#666").pack(side=tk.RIGHT)

        self.text = tk.Text(self.frame, wrap="word", font=("Menlo", 10),
                            background="#0e1116", foreground="#cbd0d6",
                            insertbackground="#cbd0d6")
        sb = ttk.Scrollbar(self.frame, command=self.text.yview)
        self.text.configure(yscrollcommand=sb.set)
        sb.pack(side=tk.RIGHT, fill=tk.Y)
        self.text.pack(fill=tk.BOTH, expand=True)

        self.text.tag_configure("stage", foreground="#9be07a")
        self.text.tag_configure("warn",  foreground="#f4c277")
        self.text.tag_configure("error", foreground="#f47171")
        self.text.tag_configure("cmd",   foreground="#7ab6ff")

        self._start_time: float | None = None

    # --- API used by App / pipeline -------------------------------------
    def write(self, text: str, tag: str | None = None) -> None:
        self.text.configure(state=tk.NORMAL)
        if tag:
            self.text.insert(tk.END, text, tag)
        else:
            self.text.insert(tk.END, text)
        self.text.see(tk.END)

    def clear(self) -> None:
        self.text.configure(state=tk.NORMAL)
        self.text.delete("1.0", tk.END)

    def save(self) -> None:
        path = filedialog.asksaveasfilename(
            defaultextension=".log",
            initialfile=f"provenance-{time.strftime('%Y%m%d-%H%M%S')}.log")
        if path:
            Path(path).write_text(self.text.get("1.0", tk.END))

    def start_clock(self) -> None:
        self._start_time = time.time()

    def stop_clock(self) -> None:
        self._start_time = None
        self.elapsed_var.set("—")

    def tick(self) -> None:
        if self._start_time is not None:
            dt = time.time() - self._start_time
            self.elapsed_var.set(f"{int(dt // 60)}m {int(dt % 60):02d}s")
