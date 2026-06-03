"""Herwig 7 configuration panel.

A thin subclass of GeneratorPanel wired to the Herwig tables in the shared
generator_presets.py (the same data run-cap / build-ini-gui use), so a Herwig
deck produced here is consistent with the rest of CAP.  Used for the
Pythia-vs-Herwig generator comparison.
"""
from __future__ import annotations

import sys
from pathlib import Path

from .genpanel import GeneratorPanel

# generator_presets lives in analyses/builder; the launcher usually adds it to
# sys.path, but make sure (and degrade gracefully if it's missing).
_REPO = Path(__file__).resolve().parents[3]
_BUILDER = _REPO / "analyses" / "builder"
if _BUILDER.is_dir() and str(_BUILDER) not in sys.path:
    sys.path.insert(0, str(_BUILDER))

try:
    from generator_presets import (                       # type: ignore
        HERWIG_PRESETS, HERWIG_BOOL_TOGGLES, HERWIG_NUMERIC_KNOBS,
        collect_herwig_lines,
    )
except Exception as exc:                                  # noqa: BLE001
    HERWIG_PRESETS = {"(presets unavailable)": []}
    HERWIG_BOOL_TOGGLES = {}
    HERWIG_NUMERIC_KNOBS = {}
    def collect_herwig_lines(*_a, **_kw):                 # type: ignore
        return [f"# generator_presets import failed: {exc}"]


class HerwigPanel(GeneratorPanel):
    PRESETS = HERWIG_PRESETS
    TOGGLES = HERWIG_BOOL_TOGGLES
    KNOBS = HERWIG_NUMERIC_KNOBS
    COLLECTOR = staticmethod(collect_herwig_lines)
    STATE_KEY = "herwig"
    PREVIEW_TITLE = "Resolved Herwig .in lines"
    # Herwig accepts PDG ints here (collect_herwig_lines maps them).
    DECAY_PRESETS = [
        ("K0S", 310), ("Lambda", 3122), ("Sigma+", 3222), ("Sigma-", 3112),
        ("Xi-", 3312), ("Xi0", 3322), ("Omega-", 3334), ("pi0", 111),
        ("D0", 421), ("D+", 411), ("Ds", 431), ("J/psi", 443),
        ("B0", 511), ("B+", 521),
    ]
