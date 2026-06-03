"""Panel — abstract base for every UI section.

Each panel takes its parent widget and the shared AppState dict, builds
its widgets, and binds variable updates back into state.  Panels never
read from sibling panels directly; only through state.
"""
from __future__ import annotations

import tkinter as tk
from tkinter import ttk
from typing import Any, Callable


def bind_to_state(var: tk.Variable, state: dict, key: str,
                  on_change: Callable[[Any], None] | None = None) -> None:
    """Mirror a Tk variable into state[key] on every write, tolerating
    transient invalid values (empty string while the user is mid-typing,
    a partial '1.' for a DoubleVar, etc).

    IntVar/DoubleVar.get() raises TclError when the entry is empty; the
    raw widget value is fine, just not parseable yet.  This adapter:
    1. Catches the error so typing doesn't pop a traceback.
    2. Falls back to the underlying Tcl string value so the state still
       reflects what's on screen (useful for downstream string fields
       like pt_min — empty string means 'no cut').

    Pass `on_change` to also fire side-effects after a successful update."""
    def _update(*_a):
        try:
            v = var.get()
        except tk.TclError:
            # Reach behind the typed-Variable wrapper for the raw string.
            try:
                v = var._tk.globalgetvar(str(var))         # type: ignore[attr-defined]
            except Exception:
                return
        state[key] = v
        if on_change is not None:
            try:
                on_change(v)
            except Exception:
                pass
    var.trace_add("write", _update)


class Panel:
    title: str = ""

    def __init__(self, parent: tk.Widget, state: dict[str, Any]):
        self.state = state
        self.frame = ttk.LabelFrame(parent, text=f" {self.title} ",
                                    padding=10) if self.title else \
                     ttk.Frame(parent)
        self.build()

    def pack(self, **kw) -> None:
        defaults = dict(fill=tk.X, pady=(0, 6))
        defaults.update(kw)
        self.frame.pack(**defaults)

    # Hook — subclasses override.
    def build(self) -> None:
        raise NotImplementedError

    # Hook — invoked when other panels' state changes.  Override if needed.
    def refresh(self) -> None:
        pass
