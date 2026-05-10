"""
Shared preset format for run-cap and build-ini-gui.

A "preset" is a single JSON file capturing the entire visible state of
either GUI.  Save it anywhere, share it between users / machines, load
it back into a fresh GUI session and pick up exactly where you left off.

Schema (version 1):
{
  "version":   1,
  "saved_at":  "2026-05-04T12:34:56Z",
  "saved_by":  "run-cap" | "build-ini-gui",
  "common": {
      "pythia_cfg": { preset, panels, numeric, stable, ctau_*, custom },
      "herwig_cfg": { preset, panels, numeric, stable, ctau_*, custom }
  },
  "build_ini_gui": {           # only present if saved by build-ini-gui
      "name", "outdir", "nevents", "nreport",
      "preset_name", "gen_kind",
      "analyses":   {AnalysisChoice → bool},
      "gen_vars":   {key → str/bool}      # the per-kind dropdown fields
  },
  "run_cap": {                 # only present if saved by run-cap
      "compose": {
          "gen_kind", "n_evt", "seed", "output_subdir",
          "analyses": {key → bool},
          "keep_final_only", "keep_quarks", "keep_neutrinos",
          "keep_photons", "keep_gauge_bosons",
          "stage",
          "run_derived", "run_bf",
          "hepmc3_input_file", "herwig_run_file", ...
      },
      "wsu": {
          "name", "user_home", "partition", "mail",
          "enable_pythia", "enable_herwig", "enable_hepmc3",
          "install_mode_*",
          "paths": { ... },
          "n_tasks", "events_per_task", "seed_offset",
          "time_*", "mem_*",
          "remote_outdir", "local_outdir"
      }
  }
}

Loading is permissive: missing keys are silently skipped, type
mismatches default to "leave the live var alone".  This keeps presets
forward-compatible with future GUI extensions.
"""

from __future__ import annotations

import json
from datetime import datetime, timezone
from pathlib import Path
from typing import Any


SCHEMA_VERSION = 1


def save_preset(state: dict, path: Path | str) -> Path:
    """Write *state* to *path* as pretty JSON.  Adds version + timestamp
    automatically."""
    p = Path(path)
    state = dict(state)
    state.setdefault("version", SCHEMA_VERSION)
    state["saved_at"] = datetime.now(timezone.utc).strftime("%Y-%m-%dT%H:%M:%SZ")
    p.write_text(json.dumps(state, indent=2, sort_keys=True),
                 encoding="utf-8")
    return p


def load_preset(path: Path | str) -> dict:
    """Read JSON preset at *path*.  Raises ValueError if not a dict or
    not a v1+ preset."""
    p = Path(path)
    data = json.loads(p.read_text(encoding="utf-8"))
    if not isinstance(data, dict):
        raise ValueError(f"{p} is not a JSON object")
    if int(data.get("version", 0)) < 1:
        raise ValueError(f"{p} is missing version (not a CAP preset?)")
    return data


# ---------------------------------------------------------------------------
#  Generator-config dict serialisation (Pythia + Herwig common path)
# ---------------------------------------------------------------------------
# The cfg state dicts in both GUIs hold tk Vars under "_*" keys.  We
# strip those when serializing and ignore them when restoring.
def cfg_to_json(cfg: dict) -> dict:
    return {k: v for k, v in cfg.items() if not k.startswith("_")}


def cfg_from_json(payload: dict, target: dict) -> None:
    """Update *target* in place with non-private keys from *payload*."""
    if not isinstance(payload, dict):
        return
    for k, v in payload.items():
        if k.startswith("_"):
            continue
        target[k] = v


# ---------------------------------------------------------------------------
#  Helpers for tk.Var-keyed dicts
# ---------------------------------------------------------------------------
def vars_to_dict(d) -> dict:
    """{key: tk.Var} → {key: var.get()}.  Tolerant of None vars."""
    out = {}
    for k, v in (d or {}).items():
        try:
            out[k] = v.get()
        except Exception:
            pass
    return out


def dict_to_vars(payload, d) -> None:
    """Inverse of vars_to_dict — write payload values into existing tk.Vars."""
    if not isinstance(payload, dict):
        return
    for k, val in payload.items():
        if k in d:
            try: d[k].set(val)
            except Exception: pass


def set_var(var, payload: dict, key: str) -> None:
    """If payload[key] exists, set var to it.  No-op otherwise."""
    if isinstance(payload, dict) and key in payload and var is not None:
        try: var.set(payload[key])
        except Exception: pass
