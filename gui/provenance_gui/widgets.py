"""Small reusable Tk widgets / helpers shared across panels."""
from __future__ import annotations

import tkinter as tk
from tkinter import ttk

from . import theme


def scrollable(parent: tk.Widget) -> ttk.Frame:
    """Fill `parent` with a vertically scrolling area and return the inner
    frame to pack content into.

    Guarantees every widget stays reachable no matter how small the window
    gets — the canvas grows a scrollbar instead of clipping content.  The
    inner frame is kept the full canvas width so child panels still expand
    horizontally as before.
    """
    outer = ttk.Frame(parent)
    outer.pack(fill=tk.BOTH, expand=True)

    canvas = tk.Canvas(outer, highlightthickness=0)
    theme.register(canvas, "canvas")        # follow window bg (no light flash)
    vbar = ttk.Scrollbar(outer, orient="vertical", command=canvas.yview)
    canvas.configure(yscrollcommand=vbar.set)
    vbar.pack(side=tk.RIGHT, fill=tk.Y)
    canvas.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)

    inner = ttk.Frame(canvas)
    win = canvas.create_window((0, 0), window=inner, anchor="nw")

    inner.bind("<Configure>",
               lambda _e: canvas.configure(scrollregion=canvas.bbox("all")))
    canvas.bind("<Configure>",
                lambda e: canvas.itemconfig(win, width=e.width))

    # Mouse-wheel scrolling, bound only while the pointer is over this canvas
    # so multiple scrollable tabs don't fight over the global wheel event.
    def _on_wheel(e):
        if getattr(e, "num", None) == 4:        # X11 scroll up
            delta = -1
        elif getattr(e, "num", None) == 5:      # X11 scroll down
            delta = 1
        else:                                   # macOS / Windows
            delta = -1 if e.delta > 0 else 1
        canvas.yview_scroll(delta, "units")

    def _bind(_e=None):
        canvas.bind_all("<MouseWheel>", _on_wheel)
        canvas.bind_all("<Button-4>", _on_wheel)
        canvas.bind_all("<Button-5>", _on_wheel)

    def _unbind(_e=None):
        canvas.unbind_all("<MouseWheel>")
        canvas.unbind_all("<Button-4>")
        canvas.unbind_all("<Button-5>")

    canvas.bind("<Enter>", _bind)
    canvas.bind("<Leave>", _unbind)
    return inner
