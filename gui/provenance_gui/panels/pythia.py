"""Pythia configuration — preset + toggles + numeric knobs + decay
disablers + cτ cut + custom lines.

Data comes from the shared `analyses/builder/generator_presets.py` so this
panel and the existing run-cap Pythia dialog stay in lock-step.  The
panel's output is collected through the same `collect_pythia_strings`
function run-cap and build-ini-gui use, so a generated .cmnd file is
indistinguishable from one produced by the other tools.

The widgets are organised into expandable category sections so the user
can collapse what they don't care about.
"""
from __future__ import annotations

import sys
import tkinter as tk
from pathlib import Path
from tkinter import ttk

from .base import Panel

# Import the shared preset data.  analyses/builder is added to sys.path
# by the app launcher; if that hasn't happened yet (e.g. running this
# panel standalone in a test), fall back gracefully.
REPO = Path(__file__).resolve().parents[3]
if str(REPO / "analyses" / "builder") not in sys.path:
    sys.path.insert(0, str(REPO / "analyses" / "builder"))
try:
    from generator_presets import (                       # type: ignore
        PYTHIA_PRESETS, PYTHIA_BOOL_TOGGLES, PYTHIA_NUMERIC_KNOBS,
        collect_pythia_strings,
    )
except Exception as exc:                                  # noqa: BLE001
    PYTHIA_PRESETS         = {"(presets unavailable)": []}
    PYTHIA_BOOL_TOGGLES    = {}
    PYTHIA_NUMERIC_KNOBS   = {}
    def collect_pythia_strings(*_a, **_kw):               # type: ignore
        return [f"# generator_presets import failed: {exc}"]


# ---------------------------------------------------------------------------
# A small reusable "collapsible category" widget.  Two-state arrow + frame.
# ---------------------------------------------------------------------------
class _CategoryBox(ttk.Frame):
    def __init__(self, parent, title, expanded=False):
        super().__init__(parent)
        self._open = tk.BooleanVar(value=expanded)
        self._hdr  = ttk.Checkbutton(
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


# ---------------------------------------------------------------------------
class PythiaPanel(Panel):
    title = ""        # rendered directly into a notebook tab — no LabelFrame

    DECAY_PRESETS = [
        ("K0S",      310),
        ("Lambda",   3122),
        ("Sigma+",   3222),
        ("Sigma-",   3112),
        ("Xi-",      3312),
        ("Xi0",      3322),
        ("Omega-",   3334),
        ("pi0",      111),
        ("D0",       421),
        ("D+",       411),
        ("Ds",       431),
        ("J/psi",    443),
        ("B0",       511),
        ("B+",       521),
    ]

    def build(self) -> None:
        s = self.state.setdefault("pythia", {
            "preset":  next(iter(PYTHIA_PRESETS)),
            "panels":  {},
            "numeric": {},
            "custom":  "",
            "stable":  [],
            "ctau_enable": False,
            "ctau_value":  10.0,         # mm
        })

        # Override the Panel ctor's LabelFrame with a plain Frame so the
        # notebook tab provides the container.
        self.frame = ttk.Frame(self.frame.master, padding=8)
        # The base class already packed nothing — caller will pack us.

        # Two-pane split: configuration on the left, live-preview on the right.
        body = ttk.PanedWindow(self.frame, orient=tk.HORIZONTAL)
        body.pack(fill=tk.BOTH, expand=True)
        cfg_outer  = ttk.Frame(body)
        preview_fr = ttk.Frame(body)
        body.add(cfg_outer,  weight=3)
        body.add(preview_fr, weight=2)

        # ===== left: scrollable cfg pane =====
        canvas = tk.Canvas(cfg_outer, highlightthickness=0,
                           background=cfg_outer.tk.call("ttk::style",
                                                        "lookup",
                                                        "TFrame",
                                                        "-background"))
        vbar = ttk.Scrollbar(cfg_outer, orient="vertical",
                             command=canvas.yview)
        canvas.configure(yscrollcommand=vbar.set)
        vbar.pack(side=tk.RIGHT, fill=tk.Y)
        canvas.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        cfg = ttk.Frame(canvas)
        win = canvas.create_window((0, 0), window=cfg, anchor="nw")
        cfg.bind("<Configure>",
                 lambda _e: canvas.configure(
                     scrollregion=canvas.bbox("all")))
        canvas.bind("<Configure>",
                    lambda e: canvas.itemconfig(win, width=e.width))

        self._build_preset_section(cfg)
        self._build_toggles_section(cfg)
        self._build_numerics_section(cfg)
        self._build_decay_section(cfg)
        self._build_custom_section(cfg)

        # ===== right: preview =====
        ttk.Label(preview_fr, text="Resolved readString lines",
                  foreground="#666").pack(anchor="w")
        self.preview = tk.Text(preview_fr, wrap="none",
                               font=("Menlo", 10),
                               background="#0e1116",
                               foreground="#cbd0d6")
        self.preview.pack(fill=tk.BOTH, expand=True)

        self._refresh_preview()

    # --------------------------------------------------- preset
    def _build_preset_section(self, parent) -> None:
        s = self.state["pythia"]
        ttk.Label(parent, text="Preset (tuned set)").pack(anchor="w")
        names = list(PYTHIA_PRESETS.keys())
        self.preset_var = tk.StringVar(value=s["preset"]
                                       if s["preset"] in names
                                       else names[0])
        combo = ttk.Combobox(parent, textvariable=self.preset_var,
                             values=names, state="readonly")
        combo.pack(fill=tk.X, pady=(2, 8))

        def _on(*_a):
            s["preset"] = self.preset_var.get()
            self._refresh_preview()
        self.preset_var.trace_add("write", _on)

    # --------------------------------------------------- bool toggles
    def _build_toggles_section(self, parent) -> None:
        s = self.state["pythia"]
        panels = s.setdefault("panels", {})
        ttk.Label(parent, text="Toggles",
                  font=("Helvetica", 11, "bold")
                  ).pack(anchor="w", pady=(4, 2))
        by_cat: dict[str, list[tuple[str, dict]]] = {}
        for key, spec in PYTHIA_BOOL_TOGGLES.items():
            by_cat.setdefault(spec.get("category", "Other"),
                              []).append((key, spec))
        for cat, entries in by_cat.items():
            box = _CategoryBox(parent, cat)
            box.pack(fill=tk.X, pady=2)
            for key, spec in entries:
                v = tk.BooleanVar(value=bool(panels.get(key, False)))
                v.trace_add("write",
                            lambda *_a, k=key, v=v: (
                                panels.__setitem__(k, v.get()),
                                self._refresh_preview()))
                ttk.Checkbutton(box.body, text=spec["label"],
                                variable=v).pack(anchor="w")

    # --------------------------------------------------- numeric knobs
    def _build_numerics_section(self, parent) -> None:
        s = self.state["pythia"]
        numeric = s.setdefault("numeric", {})
        ttk.Label(parent, text="Numeric knobs",
                  font=("Helvetica", 11, "bold")
                  ).pack(anchor="w", pady=(8, 2))
        by_cat: dict[str, list[tuple[str, dict]]] = {}
        for key, spec in PYTHIA_NUMERIC_KNOBS.items():
            by_cat.setdefault(spec.get("category", "Other"),
                              []).append((key, spec))
        for cat, entries in by_cat.items():
            box = _CategoryBox(parent, cat)
            box.pack(fill=tk.X, pady=2)
            for key, spec in entries:
                row = ttk.Frame(box.body); row.pack(fill=tk.X, pady=1)
                ttk.Label(row, text=spec["label"], width=42,
                          anchor="w").pack(side=tk.LEFT)
                var = tk.StringVar(value=str(numeric.get(key, "")))
                ttk.Entry(row, textvariable=var, width=12
                          ).pack(side=tk.LEFT, padx=(4, 4))
                ttk.Label(row, text=f"default {spec.get('default','—')}",
                          foreground="#888"
                          ).pack(side=tk.LEFT)
                var.trace_add("write",
                              lambda *_a, k=key, v=var: (
                                  numeric.__setitem__(k, v.get()),
                                  self._refresh_preview()))

    # --------------------------------------------------- decay disablers
    def _build_decay_section(self, parent) -> None:
        s = self.state["pythia"]
        stable = s.setdefault("stable", [])

        ttk.Label(parent, text="Decay disablers",
                  font=("Helvetica", 11, "bold")
                  ).pack(anchor="w", pady=(8, 2))

        box = _CategoryBox(parent, "Make these particles stable", expanded=True)
        box.pack(fill=tk.X, pady=2)

        # Quick-pick checkboxes.
        grid = ttk.Frame(box.body); grid.pack(fill=tk.X)
        self._stable_vars: dict[int, tk.BooleanVar] = {}
        for i, (name, pdg) in enumerate(self.DECAY_PRESETS):
            v = tk.BooleanVar(value=pdg in stable)
            self._stable_vars[pdg] = v
            v.trace_add("write",
                        lambda *_a, p=pdg, v=v: self._on_stable_toggle(p, v))
            ttk.Checkbutton(grid, text=f"{name}  ({pdg})", variable=v
                            ).grid(row=i // 3, column=i % 3,
                                   sticky="w", padx=(0, 18), pady=1)

        # Free-form PDG entry — adds anything not in the quick list.
        custom_row = ttk.Frame(box.body); custom_row.pack(fill=tk.X,
                                                          pady=(6, 0))
        ttk.Label(custom_row,
                  text="Other PDGs (comma-separated)").pack(side=tk.LEFT)
        self._extra_pdg_var = tk.StringVar()
        ttk.Entry(custom_row, textvariable=self._extra_pdg_var, width=24
                  ).pack(side=tk.LEFT, padx=(6, 6))
        ttk.Button(custom_row, text="Add",
                   command=self._add_extra_pdgs).pack(side=tk.LEFT)

        # Global cτ cut.
        ct = _CategoryBox(parent, "Global cτ cut (mm)", expanded=True)
        ct.pack(fill=tk.X, pady=(6, 0))
        row = ttk.Frame(ct.body); row.pack(fill=tk.X)
        self._ctau_on  = tk.BooleanVar(value=bool(s["ctau_enable"]))
        self._ctau_val = tk.DoubleVar (value=float(s["ctau_value"]))
        ttk.Checkbutton(row, text="treat everything with cτ > ",
                        variable=self._ctau_on
                        ).pack(side=tk.LEFT)
        ttk.Spinbox(row, from_=0.01, to=10000.0, increment=1.0, width=8,
                    textvariable=self._ctau_val
                    ).pack(side=tk.LEFT)
        ttk.Label(row, text=" mm as stable").pack(side=tk.LEFT)
        for v, k in [(self._ctau_on, "ctau_enable"),
                     (self._ctau_val, "ctau_value")]:
            v.trace_add("write",
                        lambda *_a, vv=v, kk=k: (
                            s.__setitem__(kk, vv.get()),
                            self._refresh_preview()))

    def _on_stable_toggle(self, pdg: int, var: tk.BooleanVar) -> None:
        stable = self.state["pythia"].setdefault("stable", [])
        if var.get():
            if pdg not in stable:
                stable.append(pdg)
        else:
            try: stable.remove(pdg)
            except ValueError: pass
        self._refresh_preview()

    def _add_extra_pdgs(self) -> None:
        raw = self._extra_pdg_var.get()
        stable = self.state["pythia"].setdefault("stable", [])
        for tok in raw.replace(";", ",").split(","):
            tok = tok.strip()
            if not tok: continue
            try:
                pdg = int(tok)
            except ValueError:
                continue
            if pdg not in stable:
                stable.append(pdg)
            if pdg in self._stable_vars:
                self._stable_vars[pdg].set(True)
        self._extra_pdg_var.set("")
        self._refresh_preview()

    # --------------------------------------------------- custom lines
    def _build_custom_section(self, parent) -> None:
        s = self.state["pythia"]
        ttk.Label(parent, text="Custom readString lines",
                  font=("Helvetica", 11, "bold")
                  ).pack(anchor="w", pady=(8, 2))
        ttk.Label(parent,
                  text="one line per Pythia setting — e.g. Tune:pp = 14",
                  foreground="#888").pack(anchor="w")
        self.custom = tk.Text(parent, height=5, wrap="none",
                              font=("Menlo", 10))
        self.custom.insert("1.0", s.get("custom", ""))
        self.custom.pack(fill=tk.X, pady=(2, 0))
        self.custom.bind("<KeyRelease>", lambda _e: self._on_custom_change())

    def _on_custom_change(self) -> None:
        self.state["pythia"]["custom"] = self.custom.get("1.0", tk.END).rstrip()
        self._refresh_preview()

    # --------------------------------------------------- live preview
    def _refresh_preview(self) -> None:
        s = self.state["pythia"]
        lines = collect_pythia_strings(
            preset_name    = s.get("preset", ""),
            bool_flags     = s.get("panels", {}),
            numeric_values = s.get("numeric", {}),
            custom_lines   = s.get("custom", ""),
            stable_pdgs    = s.get("stable", []),
            ctau_max_mm    = (s.get("ctau_value")
                              if s.get("ctau_enable") else None),
        )
        self.preview.configure(state=tk.NORMAL)
        self.preview.delete("1.0", tk.END)
        self.preview.insert(tk.END, "\n".join(lines) + "\n")
        self.preview.configure(state=tk.NORMAL)

        # Notify dependents (e.g. the ladder panel re-derives its rungs from
        # the current mechanism toggles).  Best-effort: never let a subscriber
        # error break the preview.
        for cb in self.state.get("_pythia_change_cbs", []):
            try:
                cb()
            except Exception:
                pass

    @staticmethod
    def resolved_lines(state: dict) -> list[str]:
        """Static helper used by pipeline.py to materialise the current
        Pythia configuration as readString lines."""
        s = state.get("pythia", {})
        return collect_pythia_strings(
            preset_name    = s.get("preset", ""),
            bool_flags     = s.get("panels", {}),
            numeric_values = s.get("numeric", {}),
            custom_lines   = s.get("custom", ""),
            stable_pdgs    = s.get("stable", []),
            ctau_max_mm    = (s.get("ctau_value")
                              if s.get("ctau_enable") else None),
        )
