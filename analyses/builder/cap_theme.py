"""
Centralized color theme for the CAP run-cap / build-ini-gui Tk GUIs.

Why this module exists
======================
Tk's ttk.Style is global, but only ttk widgets pick it up.  Plain Tk
widgets (Text, Canvas, Listbox, Toplevel) need explicit configure()
calls.  And `apply once at startup` doesn't work either: any new
Toplevel opened later (Pythia/Herwig config window, Performance
monitor, Settings dialog itself) is born system-themed.

So this module owns three things:

    1. The 14-slot palette + three presets (DARK default, LIGHT,
       HIGH_CONTRAST).
    2. apply_theme(root, palette)  — reconfigures ttk.Style and every
       tk.* widget that has been register_widget()'d.
    3. JSON persistence at ~/.cap_theme.json so the user's choice
       survives across run-cap restarts.

Usage from the GUI:

    import cap_theme as theme
    theme.install(root)                  # call once after Tk() — applies
                                          # the persisted theme (or DARK)
    theme.register(my_text_widget, 'terminal')   # for non-ttk widgets
    theme.open_settings_dialog(root)     # opens the picker

Theme is applied to all currently-known widgets immediately, and to any
widget you `register()` later.
"""

from __future__ import annotations

import json
import tkinter as tk
import tkinter.ttk as ttk
from pathlib import Path
from typing import Callable

# ---------------------------------------------------------------------------
#  The 14 color slots.  Every UI element maps to exactly one slot.
# ---------------------------------------------------------------------------
PALETTE_SLOTS: list[tuple[str, str]] = [
    # (key, human-readable label for the settings dialog)
    ("bg",            "Window background"),
    ("fg",            "Primary text"),
    ("bg_alt",        "Panel / LabelFrame background"),
    ("border",        "Frame edge / separator"),
    ("accent",        "Accent (selected tab, focus ring)"),
    ("accent_fg",     "Text on accent"),
    ("button_bg",     "Button background"),
    ("button_fg",     "Button text"),
    ("entry_bg",      "Input field background"),
    ("entry_fg",      "Input field text"),
    ("terminal_bg",   "Terminal background"),
    ("terminal_fg",   "Terminal default text"),
    ("log_info",      "Log: info / hint (cyan-ish)"),
    ("log_ok",        "Log: success (green)"),
    ("log_warn",      "Log: warning (amber)"),
    ("log_error",     "Log: error (red)"),
    ("muted",         "Muted hint text"),
    ("title_fg",      "Header / title text"),
]


# ---------------------------------------------------------------------------
#  Presets
# ---------------------------------------------------------------------------
THEME_DARK: dict[str, str] = {
    "bg":          "#000000",   # pure black per user request
    "fg":          "#ffffff",
    "bg_alt":      "#0c0c10",
    "border":      "#2a2a2e",
    "accent":      "#1f6feb",   # GitHub-blue
    "accent_fg":   "#ffffff",
    "button_bg":   "#161b22",
    "button_fg":   "#f0f6fc",
    "entry_bg":    "#0d1117",
    "entry_fg":    "#f0f6fc",
    "terminal_bg": "#000000",
    "terminal_fg": "#e6edf3",
    "log_info":    "#79c0ff",
    "log_ok":      "#7ee787",
    "log_warn":    "#d29922",
    "log_error":   "#ff7b72",
    "muted":       "#6e7681",
    "title_fg":    "#ffffff",
}

THEME_LIGHT: dict[str, str] = {
    "bg":          "#f6f8fa",
    "fg":          "#1f2328",
    "bg_alt":      "#ffffff",
    "border":      "#d0d7de",
    "accent":      "#0969da",
    "accent_fg":   "#ffffff",
    "button_bg":   "#f6f8fa",
    "button_fg":   "#1f2328",
    "entry_bg":    "#ffffff",
    "entry_fg":    "#1f2328",
    "terminal_bg": "#f6f8fa",
    "terminal_fg": "#1f2328",
    "log_info":    "#0969da",
    "log_ok":      "#1a7f37",
    "log_warn":    "#9a6700",
    "log_error":   "#cf222e",
    "muted":       "#656d76",
    "title_fg":    "#0f1115",
}

THEME_HIGH_CONTRAST: dict[str, str] = {
    "bg":          "#000000",
    "fg":          "#ffffff",
    "bg_alt":      "#000000",
    "border":      "#ffffff",
    "accent":      "#ffd000",
    "accent_fg":   "#000000",
    "button_bg":   "#000000",
    "button_fg":   "#ffffff",
    "entry_bg":    "#000000",
    "entry_fg":    "#ffffff",
    "terminal_bg": "#000000",
    "terminal_fg": "#ffffff",
    "log_info":    "#80e0ff",
    "log_ok":      "#80ff80",
    "log_warn":    "#ffd000",
    "log_error":   "#ff8080",
    "muted":       "#bfbfbf",
    "title_fg":    "#ffd000",
}

PRESETS: dict[str, dict[str, str]] = {
    "Dark (default)":   THEME_DARK,
    "Light":            THEME_LIGHT,
    "High contrast":    THEME_HIGH_CONTRAST,
}

# Default for first launch
DEFAULT_PRESET_NAME = "Dark (default)"

# Persistence
CONFIG_PATH = Path.home() / ".cap_theme.json"


# ---------------------------------------------------------------------------
#  Module-private state — current palette + registered non-ttk widgets
# ---------------------------------------------------------------------------
_current_palette: dict[str, str] = dict(THEME_DARK)
_registered_widgets: list[tuple[tk.Widget, str]] = []
_listeners: list[Callable[[dict[str, str]], None]] = []
_root: tk.Tk | None = None


# ---------------------------------------------------------------------------
#  Public API
# ---------------------------------------------------------------------------
def install(root: tk.Tk) -> dict[str, str]:
    """Apply the persisted theme (or DARK by default) to *root*.

    Call once, after creating the Tk root and before building the UI.
    Returns the active palette dict so the caller can sample colors
    for ad-hoc widgets it doesn't want to register.
    """
    global _root
    _root = root
    palette = load_palette()
    apply_theme(root, palette)
    return palette


def load_palette() -> dict[str, str]:
    """Load palette from CONFIG_PATH, fall back to DEFAULT_PRESET_NAME."""
    if CONFIG_PATH.is_file():
        try:
            data = json.loads(CONFIG_PATH.read_text(encoding="utf-8"))
            if isinstance(data, dict):
                # Fill in missing keys from DARK so partial files still work.
                merged = dict(THEME_DARK)
                for k in data:
                    if k in merged:
                        merged[k] = str(data[k])
                return merged
        except (json.JSONDecodeError, OSError):
            pass
    return dict(PRESETS[DEFAULT_PRESET_NAME])


def save_palette(palette: dict[str, str]) -> None:
    """Persist *palette* to CONFIG_PATH (writes JSON)."""
    try:
        CONFIG_PATH.write_text(
            json.dumps(palette, indent=2, sort_keys=True), encoding="utf-8")
    except OSError:
        pass


def current() -> dict[str, str]:
    """Return a copy of the live palette."""
    return dict(_current_palette)


def register(widget: tk.Widget, role: str = "default") -> None:
    """Track *widget* so theme changes reach non-ttk widgets.

    role values understood:
        'default'      — generic Tk widget; bg=bg, fg=fg
        'terminal'     — uses terminal_bg / terminal_fg + log_* tag colors
        'preview'      — small monospace preview box (terminal_bg/fg)
        'entry_text'   — multiline text entry (entry_bg/fg)
        'canvas'       — tk.Canvas: only bg
    """
    _registered_widgets.append((widget, role))
    # Apply current palette immediately so the new widget doesn't flash.
    _apply_to_widget(widget, role, _current_palette)


def unregister(widget: tk.Widget) -> None:
    global _registered_widgets
    _registered_widgets = [(w, r) for (w, r) in _registered_widgets
                           if w is not widget]


def add_listener(fn: Callable[[dict[str, str]], None]) -> None:
    """Subscribe to theme changes; *fn* is called with the new palette
    every time apply_theme() runs.  Useful for matplotlib plots or other
    code that owns its own colors."""
    _listeners.append(fn)


def apply_theme(root: tk.Tk, palette: dict[str, str]) -> None:
    """Reconfigure ttk.Style + every registered tk widget."""
    global _current_palette
    _current_palette = dict(palette)

    style = ttk.Style(root)
    # 'clam' is the only built-in ttk theme that fully respects color
    # configuration on every platform, including macOS.  We force-switch.
    try:
        style.theme_use("clam")
    except tk.TclError:
        pass

    p = palette
    # --- Root window itself ---
    try:
        root.configure(background=p["bg"])
    except tk.TclError:
        pass

    # --- ttk widget styles ---
    style.configure(".",
                    background=p["bg"],
                    foreground=p["fg"],
                    fieldbackground=p["entry_bg"],
                    bordercolor=p["border"],
                    troughcolor=p["bg_alt"],
                    lightcolor=p["bg_alt"],
                    darkcolor=p["bg_alt"],
                    selectbackground=p["accent"],
                    selectforeground=p["accent_fg"],
                    focuscolor=p["accent"])

    style.configure("TFrame",       background=p["bg"])
    style.configure("TLabel",       background=p["bg"], foreground=p["fg"])
    style.configure("TLabelframe",  background=p["bg"], foreground=p["fg"],
                                    bordercolor=p["border"])
    style.configure("TLabelframe.Label", background=p["bg"],
                                    foreground=p["title_fg"])
    style.configure("TButton",      background=p["button_bg"],
                                    foreground=p["button_fg"],
                                    bordercolor=p["border"],
                                    focusthickness=1,
                                    padding=4)
    style.map("TButton",
              background=[("active", p["accent"]), ("pressed", p["accent"])],
              foreground=[("active", p["accent_fg"]),
                          ("pressed", p["accent_fg"])])
    style.configure("TCheckbutton", background=p["bg"], foreground=p["fg"])
    style.map("TCheckbutton",
              background=[("active", p["bg_alt"])])
    style.configure("TRadiobutton", background=p["bg"], foreground=p["fg"])
    style.map("TRadiobutton",
              background=[("active", p["bg_alt"])])
    style.configure("TEntry",       fieldbackground=p["entry_bg"],
                                    foreground=p["entry_fg"],
                                    insertcolor=p["entry_fg"],
                                    bordercolor=p["border"])
    style.configure("TCombobox",    fieldbackground=p["entry_bg"],
                                    foreground=p["entry_fg"],
                                    background=p["button_bg"],
                                    bordercolor=p["border"])
    style.map("TCombobox",
              fieldbackground=[("readonly", p["entry_bg"])],
              foreground=[("readonly", p["entry_fg"])])
    style.configure("TNotebook",      background=p["bg"], borderwidth=0)
    style.configure("TNotebook.Tab",  background=p["bg_alt"],
                                      foreground=p["fg"],
                                      padding=(12, 4))
    style.map("TNotebook.Tab",
              background=[("selected", p["accent"])],
              foreground=[("selected", p["accent_fg"])])
    style.configure("TPanedwindow",   background=p["bg"])
    style.configure("Vertical.TScrollbar",
                    background=p["bg_alt"], troughcolor=p["bg"],
                    bordercolor=p["border"], arrowcolor=p["fg"])
    style.configure("Horizontal.TScrollbar",
                    background=p["bg_alt"], troughcolor=p["bg"],
                    bordercolor=p["border"], arrowcolor=p["fg"])
    style.configure("TSeparator",     background=p["border"])
    style.configure("TProgressbar",
                    background=p["accent"], troughcolor=p["bg_alt"])

    # Header / title styles — used by the new title bar
    style.configure("Title.TLabel",
                    background=p["bg"], foreground=p["title_fg"],
                    font=("TkDefaultFont", 22, "bold"))
    style.configure("Subtitle.TLabel",
                    background=p["bg"], foreground=p["muted"],
                    font=("TkDefaultFont", 11))
    style.configure("Header.TFrame", background=p["bg"])
    # An accent button for the Settings ⚙ etc.
    style.configure("Accent.TButton",
                    background=p["accent"], foreground=p["accent_fg"])
    style.map("Accent.TButton",
              background=[("active", p["accent"]), ("pressed", p["accent"])])

    # --- Sweep registered tk.* widgets ---
    for w, role in list(_registered_widgets):
        try:
            _apply_to_widget(w, role, palette)
        except tk.TclError:
            # Widget destroyed; drop it
            unregister(w)

    # --- Notify listeners (matplotlib, etc.) ---
    for fn in list(_listeners):
        try: fn(palette)
        except Exception: pass


def open_settings_dialog(root: tk.Tk) -> None:
    """Open a Toplevel that lets the user customise every slot in the
    palette and pick from the three presets.  Live-previews changes
    by calling apply_theme() on every pick."""
    import tkinter.colorchooser as cc
    from tkinter import messagebox

    win = tk.Toplevel(root)
    win.title("CAP — Theme settings")
    win.geometry("640x780")

    # Make the dialog itself follow the active theme
    win.configure(background=_current_palette["bg"])
    register(win, "default")

    # ---- Top: preset row + 'Apply preset' button ----
    top = ttk.Frame(win, padding=8); top.pack(fill=tk.X)
    ttk.Label(top, text="Preset:").pack(side=tk.LEFT)
    preset_var = tk.StringVar(value=DEFAULT_PRESET_NAME)
    cb = ttk.Combobox(top, textvariable=preset_var,
                      values=list(PRESETS.keys()),
                      state="readonly", width=22)
    cb.pack(side=tk.LEFT, padx=8)

    # The dict the dialog edits — starts as a copy of the live palette
    work = dict(_current_palette)

    swatches: dict[str, ttk.Label] = {}

    def _refresh_swatches():
        for k, sw in swatches.items():
            sw.configure(background=work[k])

    def _apply_preset():
        nonlocal work
        new = PRESETS.get(preset_var.get())
        if not new: return
        work = dict(new)
        _refresh_swatches()
        apply_theme(root, work)

    ttk.Button(top, text="Apply preset",
               command=_apply_preset, style="Accent.TButton"
               ).pack(side=tk.LEFT)

    # ---- Scrollable middle area: one row per slot ----
    mid_outer = ttk.Frame(win); mid_outer.pack(fill=tk.BOTH, expand=True,
                                                padx=8, pady=4)
    canvas = tk.Canvas(mid_outer, highlightthickness=0,
                       background=_current_palette["bg"])
    register(canvas, "canvas")
    vbar = ttk.Scrollbar(mid_outer, orient="vertical", command=canvas.yview)
    canvas.configure(yscrollcommand=vbar.set)
    vbar.pack(side=tk.RIGHT, fill=tk.Y)
    canvas.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
    rows = ttk.Frame(canvas)
    canvas.create_window((0, 0), window=rows, anchor="nw")
    rows.bind("<Configure>",
              lambda _e: canvas.configure(scrollregion=canvas.bbox("all")))

    def _make_row(parent, slot_key, slot_label, r):
        ttk.Label(parent, text=slot_label, width=34, anchor="w"
                  ).grid(row=r, column=0, sticky="w", padx=6, pady=2)
        sw = ttk.Label(parent, text=" "*8,
                       background=work[slot_key], width=10, relief="solid",
                       borderwidth=1)
        sw.grid(row=r, column=1, sticky="w", padx=4, pady=2)
        swatches[slot_key] = sw
        hex_var = tk.StringVar(value=work[slot_key])
        ent = ttk.Entry(parent, textvariable=hex_var, width=10)
        ent.grid(row=r, column=2, sticky="w", padx=4, pady=2)

        def _pick():
            initial = work[slot_key]
            (rgb, hexv) = cc.askcolor(initial, parent=win,
                                      title=f"Pick {slot_label}")
            if hexv:
                work[slot_key] = hexv
                hex_var.set(hexv)
                sw.configure(background=hexv)
                apply_theme(root, work)
        def _on_enter(_e=None):
            v = hex_var.get().strip()
            if not v.startswith("#") or len(v) not in (4, 7):
                return
            work[slot_key] = v
            sw.configure(background=v)
            apply_theme(root, work)

        ent.bind("<Return>",   _on_enter)
        ent.bind("<FocusOut>", _on_enter)
        ttk.Button(parent, text="Pick…", command=_pick
                   ).grid(row=r, column=3, sticky="w", padx=4, pady=2)

    for r, (k, lbl) in enumerate(PALETTE_SLOTS):
        _make_row(rows, k, lbl, r)

    # ---- Bottom button row ----
    btns = ttk.Frame(win, padding=8); btns.pack(fill=tk.X)
    def _save_default():
        save_palette(work)
        messagebox.showinfo("Theme",
            f"Saved to:\n{CONFIG_PATH}\n\nThis will be loaded next time "
            f"run-cap starts.", parent=win)
    def _reset_dark():
        nonlocal work
        work = dict(THEME_DARK)
        _refresh_swatches()
        apply_theme(root, work)
        preset_var.set("Dark (default)")
    def _close():
        win.destroy()
    ttk.Button(btns, text="✕ Cancel",     command=_close
               ).pack(side=tk.RIGHT)
    ttk.Button(btns, text="Save as default",
               command=_save_default, style="Accent.TButton"
               ).pack(side=tk.RIGHT, padx=(0, 8))
    ttk.Button(btns, text="↺ Reset to Dark", command=_reset_dark
               ).pack(side=tk.LEFT)


# ---------------------------------------------------------------------------
#  Internal: apply palette to a single tk.* widget given its role
# ---------------------------------------------------------------------------
def _apply_to_widget(w: tk.Widget, role: str, p: dict[str, str]) -> None:
    cls = w.winfo_class()
    try:
        if role == "terminal":
            w.configure(background=p["terminal_bg"],
                        foreground=p["terminal_fg"],
                        insertbackground=p["terminal_fg"])
            # Re-apply tag colors on the terminal text widget
            if isinstance(w, tk.Text):
                w.tag_configure("info",  foreground=p["log_info"])
                w.tag_configure("ok",    foreground=p["log_ok"])
                w.tag_configure("warn",  foreground=p["log_warn"])
                w.tag_configure("error", foreground=p["log_error"])
        elif role == "preview":
            w.configure(background=p["terminal_bg"],
                        foreground=p["terminal_fg"],
                        insertbackground=p["terminal_fg"])
        elif role == "entry_text":
            w.configure(background=p["entry_bg"],
                        foreground=p["entry_fg"],
                        insertbackground=p["entry_fg"])
        elif role == "canvas":
            w.configure(background=p["bg"], highlightthickness=0)
        else:  # default
            if cls in ("Toplevel", "Tk", "Frame", "Labelframe"):
                w.configure(background=p["bg"])
            elif cls == "Label":
                w.configure(background=p["bg"], foreground=p["fg"])
            elif cls == "Button":
                w.configure(background=p["button_bg"],
                            foreground=p["button_fg"],
                            activebackground=p["accent"],
                            activeforeground=p["accent_fg"])
            elif cls == "Text":
                w.configure(background=p["entry_bg"],
                            foreground=p["entry_fg"],
                            insertbackground=p["entry_fg"])
            elif cls == "Listbox":
                w.configure(background=p["entry_bg"],
                            foreground=p["entry_fg"],
                            selectbackground=p["accent"],
                            selectforeground=p["accent_fg"])
            elif cls == "Canvas":
                w.configure(background=p["bg"], highlightthickness=0)
    except tk.TclError:
        pass
