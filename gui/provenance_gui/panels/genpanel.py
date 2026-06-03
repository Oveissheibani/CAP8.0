"""Generic event-generator configuration panel.

A data-driven clone of the Pythia panel that works for any generator whose
presets/toggles/knobs follow the generator_presets.py schema.  A subclass just
sets the class attributes below; the widget logic (preset combo, categorised
toggles, numeric knobs, decay disablers, cτ cut, custom lines, live preview)
is identical.

The proven PythiaPanel is left untouched; HerwigPanel (and any future
generator) builds on this.

Subclass contract:
    PRESETS       : dict[str, list[str]]        — tuned-set library
    TOGGLES       : dict[str, dict]             — bool toggles (category/label/lines)
    KNOBS         : dict[str, dict]             — numeric knobs (category/label/default)
    COLLECTOR     : callable(preset_name, bool_flags, numeric_values,
                             custom_lines, stable_pdgs, *, ctau_max_mm) -> list[str]
    STATE_KEY     : str                         — state[...] slot for this generator
    PREVIEW_TITLE : str                         — header over the live preview
    DECAY_PRESETS : list[tuple[str, int]]       — quick-pick stable particles
"""
from __future__ import annotations

import tkinter as tk
from tkinter import ttk

from .base import Panel


class _CategoryBox(ttk.Frame):
    """Collapsible category section (two-state arrow + body)."""
    def __init__(self, parent, title, expanded=False):
        super().__init__(parent)
        self._open = tk.BooleanVar(value=expanded)
        self._hdr = ttk.Checkbutton(
            self, style="Toolbutton",
            text=("▼ " if expanded else "▶ ") + title,
            variable=self._open, command=self._toggle)
        self._hdr.pack(fill=tk.X, anchor="w")
        self.body = ttk.Frame(self, padding=(18, 4, 4, 4))
        if expanded:
            self.body.pack(fill=tk.X)

    def _toggle(self) -> None:
        if self._open.get():
            self.body.pack(fill=tk.X)
            self._hdr.configure(text="▼ " + self._hdr.cget("text")[2:])
        else:
            self.body.forget()
            self._hdr.configure(text="▶ " + self._hdr.cget("text")[2:])


class GeneratorPanel(Panel):
    title = ""

    # --- subclass overrides ---
    PRESETS: dict = {}
    TOGGLES: dict = {}
    KNOBS: dict = {}
    COLLECTOR = staticmethod(lambda *a, **k: [])
    STATE_KEY = "generator"
    PREVIEW_TITLE = "Resolved configuration lines"
    DECAY_PRESETS: list = [
        ("K0S", 310), ("Lambda", 3122), ("pi0", 111),
        ("D0", 421), ("D+", 411), ("B0", 511), ("B+", 521),
    ]

    # ------------------------------------------------------------------ build
    def build(self) -> None:
        s = self.state.setdefault(self.STATE_KEY, {
            "preset":  next(iter(self.PRESETS)) if self.PRESETS else "",
            "panels":  {}, "numeric": {}, "custom": "", "stable": [],
            "ctau_enable": False, "ctau_value": 10.0,
        })

        self.frame = ttk.Frame(self.frame.master, padding=8)

        body = ttk.PanedWindow(self.frame, orient=tk.HORIZONTAL)
        body.pack(fill=tk.BOTH, expand=True)
        cfg_outer = ttk.Frame(body)
        preview_fr = ttk.Frame(body)
        body.add(cfg_outer, weight=3)
        body.add(preview_fr, weight=2)

        canvas = tk.Canvas(cfg_outer, highlightthickness=0)
        try:
            from .. import theme as _th
            _th.register(canvas, "canvas")
        except Exception:
            pass
        vbar = ttk.Scrollbar(cfg_outer, orient="vertical", command=canvas.yview)
        canvas.configure(yscrollcommand=vbar.set)
        vbar.pack(side=tk.RIGHT, fill=tk.Y)
        canvas.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        cfg = ttk.Frame(canvas)
        win = canvas.create_window((0, 0), window=cfg, anchor="nw")
        cfg.bind("<Configure>",
                 lambda _e: canvas.configure(scrollregion=canvas.bbox("all")))
        canvas.bind("<Configure>",
                    lambda e: canvas.itemconfig(win, width=e.width))

        self._build_preset(cfg)
        self._build_toggles(cfg)
        self._build_numerics(cfg)
        self._build_decay(cfg)
        self._build_custom(cfg)

        ttk.Label(preview_fr, text=self.PREVIEW_TITLE,
                  foreground="#888").pack(anchor="w")
        self.preview = tk.Text(preview_fr, wrap="none", font=("Menlo", 10),
                               background="#0e1116", foreground="#cbd0d6")
        try:
            from .. import theme as _th
            _th.register(self.preview, "preview")
        except Exception:
            pass
        self.preview.pack(fill=tk.BOTH, expand=True)
        self._refresh_preview()

    # ------------------------------------------------------------- sections
    def _build_preset(self, parent) -> None:
        s = self.state[self.STATE_KEY]
        ttk.Label(parent, text="Preset (tuned set)").pack(anchor="w")
        names = list(self.PRESETS.keys()) or ["(no presets)"]
        self.preset_var = tk.StringVar(
            value=s["preset"] if s["preset"] in names else names[0])
        ttk.Combobox(parent, textvariable=self.preset_var, values=names,
                     state="readonly").pack(fill=tk.X, pady=(2, 8))

        def _on(*_a):
            s["preset"] = self.preset_var.get()
            self._refresh_preview()
        self.preset_var.trace_add("write", _on)

    def _build_toggles(self, parent) -> None:
        s = self.state[self.STATE_KEY]
        panels = s.setdefault("panels", {})
        ttk.Label(parent, text="Toggles", font=("Helvetica", 11, "bold")
                  ).pack(anchor="w", pady=(4, 2))
        by_cat: dict[str, list] = {}
        for key, spec in self.TOGGLES.items():
            by_cat.setdefault(spec.get("category", "Other"), []).append(
                (key, spec))
        for cat, entries in by_cat.items():
            box = _CategoryBox(parent, cat); box.pack(fill=tk.X, pady=2)
            for key, spec in entries:
                v = tk.BooleanVar(value=bool(panels.get(key, False)))
                v.trace_add("write", lambda *_a, k=key, v=v: (
                    panels.__setitem__(k, v.get()), self._refresh_preview()))
                ttk.Checkbutton(box.body, text=spec["label"], variable=v
                                ).pack(anchor="w")

    def _build_numerics(self, parent) -> None:
        s = self.state[self.STATE_KEY]
        numeric = s.setdefault("numeric", {})
        ttk.Label(parent, text="Numeric knobs", font=("Helvetica", 11, "bold")
                  ).pack(anchor="w", pady=(8, 2))
        by_cat: dict[str, list] = {}
        for key, spec in self.KNOBS.items():
            by_cat.setdefault(spec.get("category", "Other"), []).append(
                (key, spec))
        for cat, entries in by_cat.items():
            box = _CategoryBox(parent, cat); box.pack(fill=tk.X, pady=2)
            for key, spec in entries:
                row = ttk.Frame(box.body); row.pack(fill=tk.X, pady=1)
                ttk.Label(row, text=spec["label"], width=42, anchor="w"
                          ).pack(side=tk.LEFT)
                var = tk.StringVar(value=str(numeric.get(key, "")))
                ttk.Entry(row, textvariable=var, width=12
                          ).pack(side=tk.LEFT, padx=(4, 4))
                ttk.Label(row, text=f"default {spec.get('default','—')}",
                          foreground="#888").pack(side=tk.LEFT)
                var.trace_add("write", lambda *_a, k=key, v=var: (
                    numeric.__setitem__(k, v.get()), self._refresh_preview()))

    def _build_decay(self, parent) -> None:
        s = self.state[self.STATE_KEY]
        stable = s.setdefault("stable", [])
        ttk.Label(parent, text="Decay disablers",
                  font=("Helvetica", 11, "bold")).pack(anchor="w", pady=(8, 2))
        box = _CategoryBox(parent, "Make these particles stable", expanded=True)
        box.pack(fill=tk.X, pady=2)
        grid = ttk.Frame(box.body); grid.pack(fill=tk.X)
        self._stable_vars: dict[int, tk.BooleanVar] = {}
        for i, (name, pdg) in enumerate(self.DECAY_PRESETS):
            v = tk.BooleanVar(value=pdg in stable)
            self._stable_vars[pdg] = v
            v.trace_add("write",
                        lambda *_a, p=pdg, v=v: self._on_stable(p, v))
            ttk.Checkbutton(grid, text="%s  (%d)" % (name, pdg), variable=v
                            ).grid(row=i // 3, column=i % 3, sticky="w",
                                   padx=(0, 18), pady=1)

        ct = _CategoryBox(parent, "Global cτ cut (mm)", expanded=True)
        ct.pack(fill=tk.X, pady=(6, 0))
        row = ttk.Frame(ct.body); row.pack(fill=tk.X)
        self._ctau_on = tk.BooleanVar(value=bool(s["ctau_enable"]))
        self._ctau_val = tk.DoubleVar(value=float(s["ctau_value"]))
        ttk.Checkbutton(row, text="treat everything with cτ > ",
                        variable=self._ctau_on).pack(side=tk.LEFT)
        ttk.Spinbox(row, from_=0.01, to=10000.0, increment=1.0, width=8,
                    textvariable=self._ctau_val).pack(side=tk.LEFT)
        ttk.Label(row, text=" mm as stable").pack(side=tk.LEFT)
        for v, k in [(self._ctau_on, "ctau_enable"),
                     (self._ctau_val, "ctau_value")]:
            v.trace_add("write", lambda *_a, vv=v, kk=k: (
                s.__setitem__(kk, vv.get()), self._refresh_preview()))

    def _on_stable(self, pdg: int, var: tk.BooleanVar) -> None:
        stable = self.state[self.STATE_KEY].setdefault("stable", [])
        if var.get():
            if pdg not in stable:
                stable.append(pdg)
        else:
            try:
                stable.remove(pdg)
            except ValueError:
                pass
        self._refresh_preview()

    def _build_custom(self, parent) -> None:
        s = self.state[self.STATE_KEY]
        ttk.Label(parent, text="Custom configuration lines",
                  font=("Helvetica", 11, "bold")).pack(anchor="w", pady=(8, 2))
        self.custom = tk.Text(parent, height=5, wrap="none",
                              font=("Menlo", 10))
        try:
            from .. import theme as _th
            _th.register(self.custom, "entry_text")
        except Exception:
            pass
        self.custom.insert("1.0", s.get("custom", ""))
        self.custom.pack(fill=tk.X, pady=(2, 0))
        self.custom.bind("<KeyRelease>", lambda _e: self._on_custom())

    def _on_custom(self) -> None:
        self.state[self.STATE_KEY]["custom"] = self.custom.get(
            "1.0", tk.END).rstrip()
        self._refresh_preview()

    # ------------------------------------------------------------- preview
    def _collect(self):
        s = self.state[self.STATE_KEY]
        # Pass positionally: the Pythia/Herwig collectors name their 5th arg
        # differently (stable_pdgs vs stable_pdgs_or_names).
        return type(self).COLLECTOR(
            s.get("preset", ""), s.get("panels", {}), s.get("numeric", {}),
            s.get("custom", ""), s.get("stable", []),
            ctau_max_mm=(s.get("ctau_value") if s.get("ctau_enable") else None),
        )

    def _refresh_preview(self) -> None:
        try:
            lines = self._collect()
        except Exception as exc:                       # noqa: BLE001
            lines = ["# config error: %r" % exc]
        self.preview.configure(state=tk.NORMAL)
        self.preview.delete("1.0", tk.END)
        self.preview.insert(tk.END, "\n".join(lines) + "\n")

    @classmethod
    def resolved_lines(cls, state: dict) -> list[str]:
        """Materialise the current config for the pipeline (static helper)."""
        s = state.get(cls.STATE_KEY, {})
        return cls.COLLECTOR(
            s.get("preset", ""), s.get("panels", {}), s.get("numeric", {}),
            s.get("custom", ""), s.get("stable", []),
            ctau_max_mm=(s.get("ctau_value") if s.get("ctau_enable") else None),
        )
