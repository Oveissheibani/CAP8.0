"""
Preset registry — each .py file in this directory exports a `PRESET` object
of type Preset; importing this package collects them all into PRESETS.

To add a new analysis preset:
    1. Drop a new file into this directory (e.g. my_preset.py)
    2. Define a top-level `PRESET = Preset(...)` in it
    3. The GUI picks it up automatically next launch.

No changes to the GUI source are required.
"""
from __future__ import annotations

import importlib
import pkgutil
from dataclasses import dataclass, field
from typing import Callable, Any

from cap_ini_builder import Job


@dataclass
class Preset:
    """A named, opinionated default Job configuration."""
    name: str                                 # display name in the GUI dropdown
    description: str = ""                     # one-line tooltip
    build: Callable[[], Job] = field(default=None)


def _discover_presets() -> dict[str, Preset]:
    out: dict[str, Preset] = {}
    pkg = __name__
    for info in pkgutil.iter_modules(__path__):  # type: ignore[name-defined]
        if info.name.startswith("_"):
            continue
        mod = importlib.import_module(f"{pkg}.{info.name}")
        preset = getattr(mod, "PRESET", None)
        if isinstance(preset, Preset):
            out[preset.name] = preset
    return out


PRESETS: dict[str, Preset] = _discover_presets()
