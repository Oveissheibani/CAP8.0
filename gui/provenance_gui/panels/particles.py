"""Particles & pairs — the study's particle pool and explicit pair requests.

This is the single place where the user declares WHICH particles the study
tracks and WHICH two-particle combinations to correlate.  It is deliberately
list-driven rather than a fixed grid:

  * Particle pool — add any number of PDGs (presets or a typed |pdg|).  The
    pool drives ``--species`` for every stage, exactly like the single-
    particle list.
  * Pair requests — either "all pairs" (the N(N+1)/2 combinations of the
    pool) or an explicitly composed list: pick A and B from the pool and add
    that pair.  The chosen pairs drive ``cap-provenance-plot --pairs``.

State written:
  state['species'] : comma-joined pool, e.g. "211,321,2212" (or "0" = all).
  state['pairs']   : list of canonical "a x b" strings; empty == all pairs.
"""
from __future__ import annotations

import tkinter as tk
from tkinter import ttk

from .base import Panel

_NAMES = {211: "pi", 321: "K", 2212: "p", 2112: "n", 310: "K0S",
          3122: "Lambda", 3312: "Xi", 3334: "Omega", 421: "D0", 411: "D"}

PRESETS = [("pi", 211), ("K", 321), ("p", 2212), ("K0S", 310), ("Lambda", 3122)]


def name_of(pdg: int) -> str:
    return _NAMES.get(int(pdg), str(pdg))


def parse_pool(species_str) -> list[int]:
    out: list[int] = []
    for tok in str(species_str).replace(" ", "").split(","):
        if not tok:
            continue
        try:
            v = int(tok)
        except ValueError:
            continue
        if v != 0 and v not in out:
            out.append(v)
    return out


def all_pairs(pool: list[int]) -> list[str]:
    """Canonical (a<=b) "a x b" strings for every unordered combination."""
    s = sorted(set(pool))
    return ["%dx%d" % (s[i], s[j]) for i in range(len(s)) for j in range(i, len(s))]


class ParticlesPairsPanel(Panel):
    title = ""        # rendered straight into a notebook tab

    def build(self) -> None:
        self.frame = ttk.Frame(self.frame.master, padding=10)

        s = self.state
        # Seed the pool from any existing species string (back-compat).
        s.setdefault("species", "211")
        s["_particles"] = parse_pool(s.get("species", "211")) or [211]
        s.setdefault("pairs_mode", "all")
        s.setdefault("pairs", [])

        # Two stacked sections.
        self._pool_box = ttk.LabelFrame(self.frame, text=" Particle pool ",
                                        padding=10)
        self._pool_box.pack(fill=tk.X)
        self._pair_box = ttk.LabelFrame(self.frame, text=" Pair requests ",
                                        padding=10)
        self._pair_box.pack(fill=tk.X, pady=(10, 0))

        self._pool_inner: ttk.Frame | None = None
        self._pair_inner: ttk.Frame | None = None
        self._build_pool_controls()
        self._render_pool()
        self._render_pairs()

    # ============================================================ pool
    def _build_pool_controls(self) -> None:
        bar = ttk.Frame(self._pool_box); bar.pack(fill=tk.X)
        ttk.Label(bar, text="add |pdg|:").pack(side=tk.LEFT)
        self._pdg_var = tk.StringVar()
        e = ttk.Entry(bar, textvariable=self._pdg_var, width=10)
        e.pack(side=tk.LEFT, padx=(4, 4))
        e.bind("<Return>", lambda _e: self._add_typed())
        ttk.Button(bar, text="Add", command=self._add_typed).pack(side=tk.LEFT)
        ttk.Separator(bar, orient="vertical").pack(side=tk.LEFT, fill=tk.Y,
                                                   padx=10)
        ttk.Label(bar, text="quick:").pack(side=tk.LEFT)
        for label, pdg in PRESETS:
            ttk.Button(bar, text=label, width=6,
                       command=lambda p=pdg: self._add(p)
                       ).pack(side=tk.LEFT, padx=(0, 2))

    def _render_pool(self) -> None:
        if self._pool_inner is not None:
            self._pool_inner.destroy()
        self._pool_inner = ttk.Frame(self._pool_box)
        self._pool_inner.pack(fill=tk.X, pady=(8, 0))
        pool = self.state["_particles"]
        if not pool:
            ttk.Label(self._pool_inner, text="(pool empty — add a particle)",
                      foreground="#888").pack(side=tk.LEFT)
            return
        ttk.Label(self._pool_inner, text="tracking:").pack(side=tk.LEFT,
                                                           padx=(0, 6))
        for pdg in pool:
            chip = ttk.Frame(self._pool_inner)
            chip.pack(side=tk.LEFT, padx=(0, 6))
            ttk.Label(chip, text="%s (%d)" % (name_of(pdg), pdg)
                      ).pack(side=tk.LEFT)
            ttk.Button(chip, text="×", width=2,
                       command=lambda p=pdg: self._remove(p)
                       ).pack(side=tk.LEFT, padx=(2, 0))

    def _add_typed(self) -> None:
        raw = self._pdg_var.get().strip()
        self._pdg_var.set("")
        try:
            self._add(int(raw))
        except ValueError:
            pass

    def _add(self, pdg: int) -> None:
        pool = self.state["_particles"]
        if pdg != 0 and pdg not in pool:
            pool.append(pdg)
            self._commit()

    def _remove(self, pdg: int) -> None:
        pool = self.state["_particles"]
        if pdg in pool:
            pool.remove(pdg)
            self._commit()

    def _commit(self) -> None:
        """Pool changed → refresh species string, prune pairs, re-render."""
        pool = self.state["_particles"]
        self.state["species"] = ",".join(str(p) for p in pool) or "0"
        valid = set(all_pairs(pool))
        self.state["pairs"] = [p for p in self.state.get("pairs", [])
                               if p in valid]
        self._render_pool()
        self._render_pairs()

    # ============================================================ pairs
    def _render_pairs(self) -> None:
        if self._pair_inner is not None:
            self._pair_inner.destroy()
        self._pair_inner = ttk.Frame(self._pair_box)
        self._pair_inner.pack(fill=tk.X)

        pool = self.state["_particles"]
        if len(pool) < 2:
            self.state["pairs_mode"] = "all"
            self.state["pairs"] = []
            ttk.Label(self._pair_inner,
                      text="Add 2+ particles to request pairs.",
                      foreground="#888").pack(anchor="w")
            return

        self._mode_var = tk.StringVar(value=self.state.get("pairs_mode", "all"))
        row = ttk.Frame(self._pair_inner); row.pack(fill=tk.X)
        ttk.Radiobutton(row, text="All pairs (%d)" % len(all_pairs(pool)),
                        value="all", variable=self._mode_var,
                        command=self._on_mode).pack(side=tk.LEFT)
        ttk.Radiobutton(row, text="Choose pairs", value="custom",
                        variable=self._mode_var,
                        command=self._on_mode).pack(side=tk.LEFT, padx=(16, 0))

        self._custom_fr = ttk.Frame(self._pair_inner)
        self._custom_fr.pack(fill=tk.X, pady=(8, 0))
        if self._mode_var.get() == "custom":
            self._build_custom()

    def _on_mode(self) -> None:
        mode = self._mode_var.get()
        self.state["pairs_mode"] = mode
        if mode == "all":
            self.state["pairs"] = []          # empty == all (plotter default)
        self._render_pairs()

    def _build_custom(self) -> None:
        pool = self.state["_particles"]
        opts = ["%s (%d)" % (name_of(p), p) for p in pool]
        self._opt_to_pdg = {o: p for o, p in zip(opts, pool)}

        comp = ttk.Frame(self._custom_fr); comp.pack(fill=tk.X)
        ttk.Label(comp, text="pair:").pack(side=tk.LEFT)
        self._a_var = tk.StringVar(value=opts[0])
        self._b_var = tk.StringVar(value=opts[0])
        ttk.Combobox(comp, textvariable=self._a_var, values=opts, width=12,
                     state="readonly").pack(side=tk.LEFT, padx=(4, 2))
        ttk.Label(comp, text="×").pack(side=tk.LEFT)
        ttk.Combobox(comp, textvariable=self._b_var, values=opts, width=12,
                     state="readonly").pack(side=tk.LEFT, padx=(2, 6))
        ttk.Button(comp, text="Add pair",
                   command=self._add_pair).pack(side=tk.LEFT)

        chosen = ttk.Frame(self._custom_fr); chosen.pack(fill=tk.X, pady=(8, 0))
        sel = self.state.get("pairs", [])
        if not sel:
            ttk.Label(chosen, text="(no pairs chosen yet)",
                      foreground="#888").pack(anchor="w")
            return
        for p in sel:
            a, b = p.split("x")
            chip = ttk.Frame(chosen); chip.pack(side=tk.LEFT, padx=(0, 6),
                                                pady=2)
            ttk.Label(chip, text="%s × %s" % (name_of(int(a)),
                                                   name_of(int(b)))
                      ).pack(side=tk.LEFT)
            ttk.Button(chip, text="×", width=2,
                       command=lambda pp=p: self._remove_pair(pp)
                       ).pack(side=tk.LEFT, padx=(2, 0))

    def _add_pair(self) -> None:
        a = self._opt_to_pdg.get(self._a_var.get())
        b = self._opt_to_pdg.get(self._b_var.get())
        if a is None or b is None:
            return
        key = "%dx%d" % (min(a, b), max(a, b))
        sel = self.state.setdefault("pairs", [])
        if key not in sel:
            sel.append(key)
        self._render_pairs()

    def _remove_pair(self, key: str) -> None:
        sel = self.state.setdefault("pairs", [])
        if key in sel:
            sel.remove(key)
        self._render_pairs()
