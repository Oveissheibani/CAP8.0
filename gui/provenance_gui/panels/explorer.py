"""Genealogy DAG widget — interactive ancestry-tree plot.

For each event in the JSON dump produced by `provenance-study
--dump-events N`, this panel:

  * lists every studied final hadron on the left,
  * draws the chosen hadron's ancestry tree on a Tk Canvas, with
    nodes coloured by Stage (HardProcess / MPI / ISR / FSR / partons /
    primary hadrons / decays / final state),
  * highlights the path from the selected hadron up to its hard-process
    parton in a contrast colour,
  * lets the user limit how far back the walk goes via a depth slider,
  * shows the clicked node's details (PDG, stage, kinematics, parent
    list) in a side panel.

The widget is intentionally read-only: it visualises what the tagger
already computed, it doesn't change the analysis.  Use it to verify
ancestry decisions case-by-case and to convince yourself that a
particular plot is showing the physics it claims to.
"""
from __future__ import annotations

import json
import math
import tkinter as tk
from pathlib import Path
from tkinter import filedialog, messagebox, ttk

from .base import Panel


# ----- mini PDG table ------------------------------------------------------
_PDG_NAME = {
    1: "d", -1: "dbar", 2: "u", -2: "ubar", 3: "s", -3: "sbar",
    4: "c", -4: "cbar", 5: "b", -5: "bbar", 21: "g",
    111: "pi0", 211: "pi+", -211: "pi-",
    321: "K+", -321: "K-", 310: "K0S", 130: "K0L",
    411: "D+", -411: "D-", 421: "D0", -421: "D0bar", 431: "Ds+", -431: "Ds-",
    443: "J/psi", 511: "B0", -511: "B0bar", 521: "B+", -521: "B-",
    2112: "n", 2212: "p", -2212: "pbar", 3122: "Lambda",
    113: "rho0", 213: "rho+", -213: "rho-",
    223: "omega", 333: "phi",
    313: "K*0", 323: "K*+", -323: "K*-",
    221: "eta", 331: "etaPrime",
    1114: "Delta-", 2114: "Delta0", 2214: "Delta+", 2224: "Delta++",
}


def pdg_label(pdg: int) -> str:
    n = _PDG_NAME.get(int(pdg))
    return n if n else str(pdg)


# Stage taxonomy mirror — kept lock-step with StageTaxonomy.hpp.  The order
# defines the visual columns of the DAG (earlier stages on the left).
STAGE_ORDER = [
    "Beam", "HardProcess", "MPI", "ISR", "FSR", "BeamRemnants",
    "PartonsPreHadronization", "PrimaryHadrons", "DecayProducts",
    "FinalState", "Unknown",
]
STAGE_COLOUR = {
    "Beam":                    "#4a5568",
    "HardProcess":             "#f56565",
    "MPI":                     "#ed8936",
    "ISR":                     "#ecc94b",
    "FSR":                     "#48bb78",
    "BeamRemnants":            "#38b2ac",
    "PartonsPreHadronization": "#4299e1",
    "PrimaryHadrons":          "#9f7aea",
    "DecayProducts":           "#ed64a6",
    "FinalState":              "#a0aec0",
    "Unknown":                 "#718096",
}


# ===========================================================================
class ExplorerPanel(Panel):                # kept name for the existing wire-up
    title = ""

    def build(self) -> None:
        self.frame = ttk.Frame(self.frame.master, padding=6)
        self.doc: dict | None = None
        self.event_idx     = 0
        self.selected_node = None
        self.depth_limit   = tk.IntVar(value=20)
        # Genealogy definition (modular): how far back to track + which
        # optional shower/MPI layers to count.  Combine freely.
        self.boundary_var  = tk.StringVar(value="Beam")
        self.show_mpi = tk.BooleanVar(value=True)
        self.show_isr = tk.BooleanVar(value=True)
        self.show_fsr = tk.BooleanVar(value=True)
        self._build_toolbar()
        self._build_body()
        self._refresh_empty()

    # ----- toolbar (file picker + event navigation) ---------------------
    def _build_toolbar(self) -> None:
        bar = ttk.Frame(self.frame); bar.pack(fill=tk.X, pady=(0, 4))
        ttk.Button(bar, text="Load events.json…",
                   command=self._open_file).pack(side=tk.LEFT)
        ttk.Button(bar, text="Auto-find",
                   command=self._auto_find).pack(side=tk.LEFT, padx=(6, 0))
        self.path_var = tk.StringVar(value="")
        ttk.Label(bar, textvariable=self.path_var,
                  foreground="#666").pack(side=tk.LEFT, padx=(12, 0))

        nav = ttk.Frame(self.frame); nav.pack(fill=tk.X, pady=(0, 4))
        ttk.Button(nav, text="◀ prev", width=8,
                   command=lambda: self._step(-1)).pack(side=tk.LEFT)
        ttk.Button(nav, text="next ▶", width=8,
                   command=lambda: self._step(+1)
                   ).pack(side=tk.LEFT, padx=(4, 0))
        self.event_label = ttk.Label(nav, text="event —",
                                     font=("Helvetica", 11, "bold"))
        self.event_label.pack(side=tk.LEFT, padx=(14, 0))
        self.summary_label = ttk.Label(nav, text="", foreground="#445")
        self.summary_label.pack(side=tk.LEFT, padx=(14, 0))

        # Depth-limit slider — how far back the genealogy is drawn.
        ttk.Label(nav, text="depth:").pack(side=tk.RIGHT, padx=(0, 4))
        ttk.Scale(nav, from_=1, to=50, orient="horizontal",
                  variable=self.depth_limit, length=140,
                  command=lambda _v: self._redraw()
                  ).pack(side=tk.RIGHT)

        # Genealogy-definition row: track-back boundary + optional layers.
        gen = ttk.Frame(self.frame); gen.pack(fill=tk.X, pady=(0, 4))
        ttk.Label(gen, text="track ancestry back to:").pack(side=tk.LEFT)
        cb = ttk.Combobox(gen, textvariable=self.boundary_var, width=24,
                          state="readonly",
                          values=["Beam", "HardProcess",
                                  "PartonsPreHadronization", "PrimaryHadrons"])
        cb.pack(side=tk.LEFT, padx=(4, 14))
        cb.bind("<<ComboboxSelected>>", lambda _e: self._redraw())
        ttk.Label(gen, text="include layers:").pack(side=tk.LEFT)
        for lab, var in (("MPI", self.show_mpi), ("ISR", self.show_isr),
                         ("FSR", self.show_fsr)):
            ttk.Checkbutton(gen, text=lab, variable=var,
                            command=self._redraw).pack(side=tk.LEFT, padx=(0, 6))

    # ----- body (hadron list | canvas | node details) -------------------
    def _build_body(self) -> None:
        body = ttk.PanedWindow(self.frame, orient=tk.HORIZONTAL)
        body.pack(fill=tk.BOTH, expand=True)

        # Left: hadron list.
        left = ttk.LabelFrame(body, text=" Final hadrons ", padding=4)
        self.hadron_tree = ttk.Treeview(
            left, columns=("pdg", "pt", "origin"), show="headings",
            height=22)
        for col, w, lbl in [("pdg", 80, "PDG"),
                            ("pt",  60, "pT"),
                            ("origin", 110, "origin")]:
            self.hadron_tree.heading(col, text=lbl)
            self.hadron_tree.column(col, width=w, anchor="center")
        self.hadron_tree.pack(fill=tk.BOTH, expand=True)
        self.hadron_tree.bind("<<TreeviewSelect>>",
                              lambda _e: self._on_hadron_select())

        # Center: canvas with the genealogy.
        mid = ttk.LabelFrame(body, text=" Genealogy ", padding=4)
        self.canvas = tk.Canvas(mid, background="#0e1116",
                                highlightthickness=0)
        hbar = ttk.Scrollbar(mid, orient="horizontal",
                             command=self.canvas.xview)
        vbar = ttk.Scrollbar(mid, orient="vertical",
                             command=self.canvas.yview)
        self.canvas.configure(xscrollcommand=hbar.set,
                              yscrollcommand=vbar.set)
        vbar.pack(side=tk.RIGHT,  fill=tk.Y)
        hbar.pack(side=tk.BOTTOM, fill=tk.X)
        self.canvas.pack(fill=tk.BOTH, expand=True)
        self.canvas.bind("<Button-1>", self._on_canvas_click)

        # Right: node details.
        right = ttk.LabelFrame(body, text=" Node detail ", padding=4)
        self.detail = tk.Text(right, wrap="word", font=("Menlo", 10),
                              background="#0e1116", foreground="#cbd0d6",
                              width=44)
        self.detail.pack(fill=tk.BOTH, expand=True)
        for tag, fg in [("hdr", "#9be07a"), ("warn", "#f4c277"),
                        ("good", "#7ab6ff")]:
            self.detail.tag_configure(tag, foreground=fg)

        # Legend at the bottom of the right pane.
        legend = ttk.Frame(right); legend.pack(fill=tk.X, pady=(4, 0))
        ttk.Label(legend, text="legend:", foreground="#666"
                  ).pack(side=tk.LEFT)
        for stage in ("HardProcess", "MPI", "ISR", "FSR",
                      "PartonsPreHadronization", "PrimaryHadrons",
                      "DecayProducts", "FinalState"):
            sw = tk.Canvas(legend, width=12, height=12,
                           background=STAGE_COLOUR.get(stage, "#888"),
                           highlightthickness=0)
            sw.pack(side=tk.LEFT, padx=(6, 2))
            ttk.Label(legend, text=stage[:3]).pack(side=tk.LEFT)

        body.add(left,  weight=2)
        body.add(mid,   weight=6)
        body.add(right, weight=3)

    # ----- file loading --------------------------------------------------
    def _open_file(self) -> None:
        out  = self.state.get("outdir", "")
        init = str(Path(out) / "runs") if out else "."
        path = filedialog.askopenfilename(
            initialdir=init, title="events.json",
            filetypes=[("JSON", "*.json"), ("All", "*.*")])
        if path:
            self._load(Path(path))

    def _auto_find(self) -> None:
        outdir = self.state.get("outdir")
        if not outdir:
            messagebox.showinfo("No output folder", "Set output root first.")
            return
        guess = Path(outdir) / "runs" / "pions.root.events.json"
        if guess.exists(): self._load(guess)
        else: messagebox.showinfo("Not found",
                                  f"No file at:\n{guess}\n\n"
                                  "Run with --dump-events N first.")

    def _load(self, path: Path) -> None:
        try:
            with open(path) as fh: self.doc = json.load(fh)
            self.path_var.set(str(path))
            self.event_idx = 0
            self.selected_node = None
            self._refresh()
        except Exception as exc:                          # noqa: BLE001
            messagebox.showerror("Load failed", str(exc))

    # ----- navigation ----------------------------------------------------
    def _step(self, delta: int) -> None:
        if not self.doc: return
        events = self.doc.get("events", [])
        if not events: return
        self.event_idx = max(0, min(len(events) - 1,
                                    self.event_idx + delta))
        self.selected_node = None
        self._refresh()

    def _refresh_empty(self) -> None:
        self.event_label.configure(text="event —")
        self.summary_label.configure(text="(load an events.json file)")
        for iid in self.hadron_tree.get_children():
            self.hadron_tree.delete(iid)
        self.canvas.delete("all")
        self.detail.configure(state=tk.NORMAL)
        self.detail.delete("1.0", tk.END)

    def _refresh(self) -> None:
        if not self.doc: return self._refresh_empty()
        events = self.doc.get("events", [])
        if not events: return self._refresh_empty()
        ev = events[self.event_idx]

        # Build index: node_id -> node dict.
        self._nodes = {n["i"]: n for n in ev.get("nodes", [])}

        # Build children index from the parents list.
        children: dict[int, list[int]] = {}
        for n in self._nodes.values():
            for p in n.get("parents", []):
                children.setdefault(p, []).append(n["i"])
        self._children = children

        # Hadron list (left).
        for iid in self.hadron_tree.get_children():
            self.hadron_tree.delete(iid)
        for h in ev.get("hadrons", []):
            origin = ("primary" if h["isPrimary"] else
                      "resonance" if h["isFromResonance"] else
                      "decay" if h["isFromDecay"] else "?")
            self.hadron_tree.insert(
                "", "end", iid=str(h["finalIndex"]),
                values=(pdg_label(h["finalPdg"]),
                        f"{h['pt']:.2f}", origin))

        self.event_label.configure(
            text=f"event {self.event_idx + 1} / {len(events)}")
        self.summary_label.configure(
            text=(f"id={ev.get('event_id','?')}  "
                  f"mult={ev.get('multiplicity','?')}  "
                  f"nodes in DAG = {len(self._nodes)}  "
                  f"hadrons={len(ev.get('hadrons',[]))}"))

        self.canvas.delete("all")
        self.detail.configure(state=tk.NORMAL)
        self.detail.delete("1.0", tk.END)
        self.detail.insert(tk.END,
                           "Select a final hadron on the left to draw "
                           "its ancestry tree.\n")

    def _on_hadron_select(self) -> None:
        sel = self.hadron_tree.selection()
        if not sel: return
        try: start = int(sel[0])
        except ValueError: return
        self.selected_start = start
        self._redraw()

    # ----- ancestry collection + layout ---------------------------------
    def _collect_ancestry(self, start: int) -> tuple[
            set[int], list[tuple[int,int]], set[int]]:
        """BFS up the parent chain from `start`, capped at depth_limit.
        Returns:
            nodes  — set of node indices reached
            edges  — list of (parent, child) tuples to draw
            path   — set of indices on the highlighted lead-branch path
                     from `start` up to the deepest reachable ancestor.
        """
        depth = max(1, int(self.depth_limit.get()))
        nodes: set[int] = {start}
        edges: list[tuple[int,int]] = []
        path: set[int] = {start}
        frontier = [start]
        # BFS.
        for _ in range(depth):
            nxt: list[int] = []
            for ni in frontier:
                node = self._nodes.get(ni)
                if not node: continue
                # Track the lead-branch path (first parent each step).
                if ni in path and node["parents"]:
                    path.add(node["parents"][0])
                for p in node.get("parents", []):
                    if p not in nodes:
                        nodes.add(p); nxt.append(p)
                    edges.append((p, ni))
            if not nxt: break
            frontier = nxt
        return nodes, edges, path

    def _stage_visible(self, stage: str) -> bool:
        """A stage is shown iff it's at or beyond the chosen track-back
        boundary AND (for the optional shower/MPI layers) its toggle is on."""
        try:    order = STAGE_ORDER.index(stage)
        except ValueError: order = len(STAGE_ORDER) - 1
        try:    b = STAGE_ORDER.index(self.boundary_var.get())
        except ValueError: b = 0
        if order < b:
            return False
        if stage == "MPI" and not self.show_mpi.get(): return False
        if stage == "ISR" and not self.show_isr.get(): return False
        if stage == "FSR" and not self.show_fsr.get(): return False
        return True

    def _redraw(self) -> None:
        self.canvas.delete("all")
        if not getattr(self, "_nodes", None): return
        if not hasattr(self, "selected_start"): return

        start = self.selected_start
        nodes, edges, path = self._collect_ancestry(start)

        # Apply the genealogy boundary + layer toggles.  The studied hadron is
        # always kept.
        visible = {ni for ni in nodes
                   if self._stage_visible(
                       self._nodes[ni].get("stage", "Unknown"))}
        visible.add(start)

        # Bridge each visible node to its nearest visible ancestor(s) so hidden
        # layers don't break the tree (mirrors the prototype's visAncestor).
        parents_of: dict[int, list[int]] = {}
        for p, c in edges:
            parents_of.setdefault(c, []).append(p)
        draw_edges: set[tuple[int, int]] = set()
        for c in visible:
            seen: set[int] = set()
            stack = list(parents_of.get(c, []))
            while stack:
                p = stack.pop()
                if p in seen: continue
                seen.add(p)
                if p in visible:
                    draw_edges.add((p, c))
                else:
                    stack.extend(parents_of.get(p, []))

        # Compact columns: only the stages that actually appear, left-to-right
        # in StageTaxonomy order (no empty gaps for hidden stages).
        def sidx(ni: int) -> int:
            st = self._nodes[ni].get("stage", "Unknown")
            try: return STAGE_ORDER.index(st)
            except ValueError: return len(STAGE_ORDER) - 1
        present = sorted({sidx(ni) for ni in visible})
        col_of = {s: i for i, s in enumerate(present)}

        by_col: dict[int, list[int]] = {}
        for ni in visible:
            by_col.setdefault(col_of[sidx(ni)], []).append(ni)

        x_gap, y_gap, margin_x, margin_y = 160, 36, 40, 30
        positions: dict[int, tuple[float, float]] = {}
        for col, lst in by_col.items():
            lst.sort()
            for row, ni in enumerate(lst):
                positions[ni] = (margin_x + col * x_gap,
                                 margin_y + row * y_gap)

        # Stage column headers across the top.
        for s, col in col_of.items():
            self.canvas.create_text(
                margin_x + col * x_gap, 8, anchor="n",
                fill="#7b8696", font=("Menlo", 8), text=STAGE_ORDER[s][:14])

        if positions:
            xs = [p[0] for p in positions.values()]
            ys = [p[1] for p in positions.values()]
            self.canvas.configure(
                scrollregion=(0, 0, max(xs) + x_gap, max(ys) + y_gap))

        # Edges first (so nodes paint on top).
        for parent, child in draw_edges:
            if parent not in positions or child not in positions: continue
            x1, y1 = positions[parent]; x2, y2 = positions[child]
            is_path = parent in path and child in path
            self.canvas.create_line(
                x1 + 12, y1, x2 - 12, y2,
                fill=("#f4c277" if is_path else "#3a4150"),
                width=(2 if is_path else 1),
                arrow="last", arrowshape=(8, 9, 3))

        # Nodes (studied hadron ringed white + slightly larger).
        self._hit = {}                              # canvas_item -> node_idx
        for ni, (x, y) in positions.items():
            n  = self._nodes[ni]
            st = n.get("stage", "Unknown")
            fill = STAGE_COLOUR.get(st, "#888")
            is_start = (ni == start)
            r = 12 if is_start else 11
            item = self.canvas.create_oval(
                x - r, y - r, x + r, y + r, fill=fill,
                outline=("#fff" if is_start else "#222"),
                width=(3 if is_start else 1))
            self._hit[item] = ni
            self.canvas.create_text(
                x + r + 4, y, anchor="w",
                fill="#cbd0d6", font=("Menlo", 9), text=pdg_label(n["pdg"]))

        self._show_node_detail(start)

    # ----- canvas click handler -----------------------------------------
    def _on_canvas_click(self, evt) -> None:
        # Convert event coords to canvas coords (account for scroll).
        cx = self.canvas.canvasx(evt.x)
        cy = self.canvas.canvasy(evt.y)
        item = self.canvas.find_closest(cx, cy)
        if not item: return
        ni = self._hit.get(item[0])
        if ni is None: return
        self._show_node_detail(ni)

    def _show_node_detail(self, ni: int) -> None:
        node = self._nodes.get(ni)
        if not node: return
        self.detail.configure(state=tk.NORMAL)
        self.detail.delete("1.0", tk.END)
        self.detail.insert(tk.END,
                           f"node {ni}  ({pdg_label(node['pdg'])})\n", "hdr")
        self.detail.insert(tk.END, "─" * 40 + "\n")
        def w(k, v): self.detail.insert(tk.END, f"  {k:<18}{v}\n")
        w("PDG",         f"{node['pdg']}")
        w("stage",       node.get("stage", "?"))
        w("isFinal",     "yes" if node.get("isFinal") else "no")
        w("pT",          f"{node.get('pt', 0):.4f}")
        w("eta",         f"{node.get('eta', 0):+.4f}")
        w("phi",         f"{node.get('phi', 0):+.4f}")
        parents = node.get("parents", [])
        kids    = self._children.get(ni, [])
        w("# parents",   str(len(parents)))
        w("# children",  str(len(kids)))
        if parents:
            self.detail.insert(tk.END, "\n  parents:\n", "hdr")
            for p in parents:
                pn = self._nodes.get(p)
                self.detail.insert(tk.END,
                    f"    {p}: {pdg_label(pn['pdg']) if pn else '?'} "
                    f"({pn.get('stage','?') if pn else '?'})\n")
        if kids:
            self.detail.insert(tk.END, "\n  children:\n", "hdr")
            for c in kids:
                cn = self._nodes.get(c)
                self.detail.insert(tk.END,
                    f"    {c}: {pdg_label(cn['pdg']) if cn else '?'} "
                    f"({cn.get('stage','?') if cn else '?'})\n")
