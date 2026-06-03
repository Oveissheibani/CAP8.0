"""Top-level App — wires panels into a single window."""
from __future__ import annotations

import os
import queue
import sys
import tkinter as tk
from pathlib import Path
from tkinter import messagebox, ttk

from .panels import (AcceptancePanel, GenealogyRequestPanel,
                     GeneratorSelectPanel, HerwigPanel, LadderDesignerPanel,
                     LogPanel, ParallelismPanel, ParticlesPairsPanel,
                     PythiaPanel, ReportPanel, RunParamsPanel, StagesPanel)
from .pipeline import (Runner, build_commands, run_warnings, shell_quote,
                       write_run_manifest, commands_to_script,
                       commands_to_oneliner)
from .widgets import scrollable
from . import theme

REPO = Path(__file__).resolve().parents[2]


class App:
    def __init__(self, root: tk.Misc | None = None,
                 container: tk.Misc | None = None) -> None:
        """Standalone (root=None): create our own themed Tk window.
        Embedded (root given): build into `container` and share the host's
        Tk root for the event loop — used when run-cap hosts us as a tab."""
        self.state: dict = {"repo": REPO}
        self.embedded = root is not None

        if root is None:
            self.root = tk.Tk()
            self.root.title("CAP — Provenance")
            self.root.geometry("1180x780")
            self.root.minsize(960, 640)
            theme.install(self.root)        # match the main GUI's dark style
            container = self.root
        else:
            self.root = root
            if container is None:
                container = root
        self.container = container

        self._build_chrome()
        self._build_tabs()
        self._build_footer()

        self.q: "queue.Queue[str]" = queue.Queue()
        self.runner = Runner(self.q)
        if not self.embedded:
            self.root.protocol("WM_DELETE_WINDOW", self._on_close)
        self.root.after(120, self._poll)

    # -------------------------------------------------- chrome (header/sep)
    def _build_chrome(self) -> None:
        hdr = ttk.Frame(self.container, padding=(12, 8)); hdr.pack(fill=tk.X)
        ttk.Label(hdr, text="🧬 CAP Provenance",
                  font=("Helvetica", 16, "bold")).pack(side=tk.LEFT)
        ttk.Separator(self.container, orient="horizontal").pack(fill=tk.X)

    # -------------------------------------------------- tabs
    def _build_tabs(self) -> None:
        nb = ttk.Notebook(self.container)
        nb.pack(fill=tk.BOTH, expand=True, padx=10, pady=8)

        tab_run       = ttk.Frame(nb); nb.add(tab_run,       text="Run")
        tab_particles = ttk.Frame(nb); nb.add(tab_particles, text="Particles & pairs")
        tab_pythia    = ttk.Frame(nb); nb.add(tab_pythia,    text="Pythia")
        tab_herwig    = ttk.Frame(nb); nb.add(tab_herwig,    text="Herwig")
        tab_geneal    = ttk.Frame(nb); nb.add(tab_geneal,    text="Genealogy")
        tab_log       = ttk.Frame(nb); nb.add(tab_log,       text="Output")

        # ------------- Run tab (scrollable so nothing clips) -------------
        run_inner = scrollable(tab_run)
        RunParamsPanel       (run_inner, self.state).pack()
        GeneratorSelectPanel (run_inner, self.state).pack()
        AcceptancePanel      (run_inner, self.state).pack()
        StagesPanel     (run_inner, self.state).pack()
        self.parallel = ParallelismPanel(run_inner, self.state); self.parallel.pack()
        ReportPanel     (run_inner, self.state).pack()

        # ------------- Particles & pairs tab -------------
        pp_inner = scrollable(tab_particles)
        pp_panel = ParticlesPairsPanel(pp_inner, self.state)
        pp_panel.frame.pack(fill=tk.BOTH, expand=True)

        # ------------- Pythia tab (config base + ladder designer) -------------
        ladder_panel = LadderDesignerPanel(tab_pythia, self.state)
        ladder_panel.frame.pack(fill=tk.X, padx=8, pady=(8, 0))
        py_panel = PythiaPanel(tab_pythia, self.state)
        py_panel.frame.pack(fill=tk.BOTH, expand=True)

        # ------------- Herwig tab (generator-comparison config) -------------
        hw_panel = HerwigPanel(tab_herwig, self.state)
        hw_panel.frame.pack(fill=tk.BOTH, expand=True)

        # ------------- Genealogy study-request tab -------------
        gen_inner = scrollable(tab_geneal)
        gen_panel = GenealogyRequestPanel(gen_inner, self.state)
        gen_panel.frame.pack(fill=tk.BOTH, expand=True)

        # ------------- Output tab -------------
        self.log = LogPanel(tab_log, self.state)
        self.log.frame.pack(fill=tk.BOTH, expand=True)

    # -------------------------------------------------- footer
    def _build_footer(self) -> None:
        ft = ttk.Frame(self.container, padding=(10, 6)); ft.pack(fill=tk.X)
        self.run_btn  = ttk.Button(ft, text="▶ Run",  command=self._on_run)
        self.stop_btn = ttk.Button(ft, text="■ Stop", command=self._on_stop,
                                   state=tk.DISABLED)
        self.run_btn .pack(side=tk.LEFT)
        self.stop_btn.pack(side=tk.LEFT, padx=(6, 0))
        ttk.Button(ft, text="Dry-run",
                   command=self._on_dryrun).pack(side=tk.LEFT, padx=(12, 0))
        ttk.Button(ft, text="⧉ Terminal cmds",
                   command=self._show_cli).pack(side=tk.LEFT, padx=(6, 0))

        ttk.Separator(ft, orient="vertical"
                      ).pack(side=tk.LEFT, fill=tk.Y, padx=12)
        ttk.Button(ft, text="Plots",   command=lambda:
                   self._open(self._outdir() / "plots")).pack(side=tk.LEFT)
        ttk.Button(ft, text="PDF",     command=self._open_pdf
                   ).pack(side=tk.LEFT, padx=(6, 0))
        ttk.Button(ft, text="Outputs", command=lambda:
                   self._open(self._outdir())).pack(side=tk.LEFT, padx=(6, 0))

        self.status = tk.StringVar(value="Ready")
        ttk.Label(ft, textvariable=self.status,
                  foreground="#445").pack(side=tk.RIGHT)

    # -------------------------------------------------- actions
    def _on_run(self) -> None:
        # Make sure the advisor has populated _resolved_jobs.
        self.parallel.refresh()
        # build_commands() reads the PRIOR manifest to decide which finished
        # units to skip; write_run_manifest() then records THIS run's plan.
        cmds, _ = build_commands(self.state)
        warnings = run_warnings(self.state)
        try:
            done, total = write_run_manifest(self.state)
        except Exception:                              # never block a run
            done, total = 0, 0
        if not cmds:
            messagebox.showinfo(
                "Nothing to run",
                ("\n\n".join(warnings) if warnings else
                 "Everything is already done, or all stages are unchecked."))
            return
        self.log.clear()
        for w in warnings:
            self.log.write("⚠ " + w + "\n\n", "error")
        self.log.write(f"output  : {self._outdir()}\n", "cmd")
        self.log.write(f"jobs    : {self.state.get('_resolved_jobs', 1)}\n",
                       "cmd")
        if done:
            self.log.write(f"resume  : skipping {done}/{total} finished "
                           f"unit(s) — reusing their output\n", "stage")
        self.log.write(f"stages  : {len(cmds)}\n\n", "cmd")
        self._set_running(True)
        self.log.start_clock()
        self.runner.start(cmds, self.state, on_done=self._on_done)

    def _on_dryrun(self) -> None:
        self.parallel.refresh()
        cmds, _ = build_commands(self.state)
        self.log.clear()
        self.log.write(f"# dry-run — would execute {len(cmds)} stage(s)\n",
                       "stage")
        for i, c in enumerate(cmds, 1):
            self.log.write(f"\n# [{i}/{len(cmds)}]\n", "cmd")
            self.log.write("  " + " ".join(shell_quote(x) for x in c)
                           + "\n", "cmd")

    def _show_cli(self) -> None:
        """Pop up the exact terminal commands that reproduce the current GUI
        configuration — copy-pasteable, or save as a .sh to run / hand off."""
        self.parallel.refresh()
        cmds, _ = build_commands(self.state)
        if not cmds:
            messagebox.showinfo(
                "Nothing to run",
                "No stages are enabled, or everything is already done "
                "(resume).  Toggle a stage on, then try again.")
            return
        script   = commands_to_script(self.state, cmds)
        oneliner = commands_to_oneliner(self.state, cmds)

        win = tk.Toplevel(self.root)
        win.title("Terminal commands — current configuration")
        win.geometry("940x600")
        ttk.Label(win, padding=(10, 8),
                  text=("The EXACT commands the ▶ Run button would execute for "
                        "your current settings.\n"
                        "• One command  — paste into a terminal in a single "
                        "shot, runs everything (chained with &&).\n"
                        "• Script  — the same steps with per-stage comments, "
                        "or Save as a .sh.")
                  ).pack(fill=tk.X)

        nb = ttk.Notebook(win); nb.pack(fill=tk.BOTH, expand=True,
                                        padx=10, pady=(0, 4))

        def _make_tab(title: str, content: str, wrap: str):
            frame = ttk.Frame(nb); nb.add(frame, text=title)
            t = tk.Text(frame, wrap=wrap, font=("Menlo", 11), height=20)
            ysb = ttk.Scrollbar(frame, orient="vertical", command=t.yview)
            t.configure(yscrollcommand=ysb.set)
            if wrap == "none":
                xsb = ttk.Scrollbar(frame, orient="horizontal", command=t.xview)
                t.configure(xscrollcommand=xsb.set)
                xsb.grid(row=1, column=0, sticky="ew")
            t.grid(row=0, column=0, sticky="nsew")
            ysb.grid(row=0, column=1, sticky="ns")
            frame.rowconfigure(0, weight=1); frame.columnconfigure(0, weight=1)
            t.insert("1.0", content)
            return t

        # One-command tab first — it's the headline feature.  Word-wrap so the
        # very long single line is fully visible without horizontal scrolling.
        one_txt    = _make_tab("One command", oneliner, "word")
        script_txt = _make_tab("Script (commented)", script, "none")

        def _active_text() -> tk.Text:
            return one_txt if nb.index(nb.select()) == 0 else script_txt

        def _copy() -> None:
            self.root.clipboard_clear()
            self.root.clipboard_append(_active_text().get("1.0", "end-1c"))
            which = "one-command" if nb.index(nb.select()) == 0 else "script"
            self.status.set(f"Copied {which} to clipboard")

        def _save() -> None:
            from tkinter import filedialog
            p = filedialog.asksaveasfilename(
                title="Save run script", defaultextension=".sh",
                initialfile="run-provenance.sh",
                initialdir=str(self._outdir()))
            if p:
                with open(p, "w") as fh:
                    fh.write(script + "\n")
                os.chmod(p, 0o755)
                self.status.set(f"Saved {p}")

        bar = ttk.Frame(win, padding=(10, 8)); bar.pack(fill=tk.X)
        ttk.Button(bar, text="⧉ Copy current tab",
                   command=_copy).pack(side=tk.LEFT)
        ttk.Button(bar, text="Save .sh…",
                   command=_save).pack(side=tk.LEFT, padx=(6, 0))
        ttk.Button(bar, text="Close",
                   command=win.destroy).pack(side=tk.RIGHT)

    def _on_stop(self) -> None:
        self.runner.stop()

    def _on_done(self) -> None:
        # Called from the runner thread; bounce to the Tk thread.
        self.root.after(0, lambda: self._set_running(False))

    def _set_running(self, running: bool) -> None:
        self.run_btn .configure(state=tk.DISABLED if running else tk.NORMAL)
        self.stop_btn.configure(state=tk.NORMAL if running else tk.DISABLED)
        self.status.set("Running…" if running else "Ready")
        if not running:
            self.log.stop_clock()
            if (self._outdir() / "reports" / "provenance-report.pdf").exists():
                if self.state.get("stages", {}).get("open_pdf"):
                    self._open_pdf()

    # -------------------------------------------------- helpers
    def _outdir(self) -> Path:
        return Path(self.state.get("outdir", str(REPO / "provenance"))
                    ).expanduser().resolve()

    def _open_pdf(self) -> None:
        p = self._outdir() / "reports" / "provenance-report.pdf"
        if p.exists():
            self._open(p)
        else:
            messagebox.showinfo("No PDF yet", f"Not found:\n{p}")

    @staticmethod
    def _open(path: Path) -> None:
        import platform, subprocess
        try:
            if platform.system() == "Darwin":
                subprocess.run(["open", str(path)])
            elif platform.system() == "Linux":
                subprocess.run(["xdg-open", str(path)])
            elif platform.system() == "Windows":
                import os
                os.startfile(str(path))                    # type: ignore[attr-defined]
        except Exception:
            pass

    # -------------------------------------------------- main loop plumbing
    def _poll(self) -> None:
        try:
            while True:
                line = self.q.get_nowait()
                tag = None
                if line.startswith("\0"):
                    _, tag, line = line.split("\0", 2)
                self.log.write(line, tag)
        except queue.Empty:
            pass
        self.log.tick()
        self.root.after(120, self._poll)

    def _on_close(self) -> None:
        if self.runner.is_running():
            if not messagebox.askyesno("Quit?",
                                       "A pipeline is running. Kill it?"):
                return
            self.runner.stop()
        self.root.destroy()

    def run(self) -> None:
        self.root.mainloop()


def embed(parent: tk.Misc, root: tk.Misc) -> "App":
    """Build the full provenance UI inside `parent` (e.g. a run-cap notebook
    tab), sharing the host's Tk `root` for the event loop.  Returns the App so
    the caller can keep a reference (the poll loop is registered on `root`)."""
    return App(root=root, container=parent)


def main() -> int:
    try:
        App().run()
    except tk.TclError as exc:
        print(f"Tk error: {exc}", file=sys.stderr)
        return 1
    return 0
