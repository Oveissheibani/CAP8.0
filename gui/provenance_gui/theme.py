"""Thin shim over the shared CAP theme (analyses/builder/cap_theme.py).

Lets the provenance GUI adopt the same dark 'clam' look as the main run-cap
launcher, and — because both import the SAME cap_theme module off sys.path —
register()ing our non-ttk widgets here means run-cap's theme reaches them too
when we're embedded as a tab.

Everything is best-effort: if cap_theme can't be found (e.g. running the
provenance GUI from a checkout without analyses/builder), these become no-ops
and the GUI falls back to the default Tk look.
"""
from __future__ import annotations

import sys
from pathlib import Path

_theme = None
try:
    _builder = Path(__file__).resolve().parents[2] / "analyses" / "builder"
    if _builder.is_dir() and str(_builder) not in sys.path:
        sys.path.insert(0, str(_builder))
    import cap_theme as _theme            # type: ignore  # noqa: E402
except Exception:                          # noqa: BLE001
    _theme = None


def available() -> bool:
    return _theme is not None


def install(root) -> None:
    """Apply the persisted CAP theme to a freshly created Tk root."""
    if _theme is not None:
        try:
            _theme.install(root)
        except Exception:                  # noqa: BLE001
            pass


def register(widget, role: str = "default") -> None:
    """Register a non-ttk widget (Text/Canvas) so it follows the theme."""
    if _theme is not None:
        try:
            _theme.register(widget, role)
        except Exception:                  # noqa: BLE001
            pass


def color(slot: str, fallback: str = "#000000") -> str:
    """Look up a palette colour, falling back if the theme is unavailable."""
    if _theme is not None:
        try:
            return _theme.current().get(slot, fallback)
        except Exception:                  # noqa: BLE001
            return fallback
    return fallback
