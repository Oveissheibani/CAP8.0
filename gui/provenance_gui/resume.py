"""Resume / checkpoint core for long provenance runs (Layer 1).

A "job" is a set of units, each unit = one provenance-study process that writes
one <out>.root (+ <out>.root.txt).  This module decides which units are already
finished so a re-run after a crash / shutdown / accidental close skips them
instead of regenerating millions of events.

Crash-safety rests on one fact: provenance-study writes its summary .root.txt
*after* closing the ROOT file, so the .txt only exists if the unit finished.
A killed unit has no .txt → it re-runs; a finished one is skipped.

A small manifest (`<outdir>/.cap-run-state.json`) stores a fingerprint of the
job config + the planned unit list, so we only ever reuse outputs that belong
to the *identical* job.  All writes are atomic (temp + os.replace), so a power
cut can never leave a half-written manifest.

No tkinter import here — pure logic, unit-testable without a display.
"""
from __future__ import annotations

import hashlib
import json
import os
import time
from pathlib import Path

MANIFEST_NAME = ".cap-run-state.json"
SCHEMA = 1

# State keys whose values determine whether a produced .root is reusable.
# (Pairs only affect plotting, not the study output, so they're excluded.)
_FINGERPRINT_KEYS = (
    "events", "ecm", "seed", "species", "process",
    "acceptance", "generators", "pythia", "herwig", "herwig_hepmc",
    "compare_mechanisms", "ladder_rungs", "chunk_events",
)


def fingerprint(state: dict) -> str:
    """Stable short hash of the job config.  Two runs with the same
    fingerprint produce identical study outputs, so finished units of one are
    reusable by the other."""
    subset = {k: state.get(k) for k in _FINGERPRINT_KEYS}
    canonical = json.dumps(subset, sort_keys=True, default=str)
    return hashlib.sha1(canonical.encode("utf-8")).hexdigest()[:16]


def manifest_path(outdir) -> Path:
    return Path(outdir) / MANIFEST_NAME


def read_manifest(outdir) -> dict | None:
    p = manifest_path(outdir)
    if not p.is_file():
        return None
    try:
        return json.loads(p.read_text(encoding="utf-8"))
    except (json.JSONDecodeError, OSError):
        return None


def write_manifest(outdir, fp: str, units: list[str]) -> None:
    """Atomically write the run manifest.  `units` is the list of output
    .root paths the job plans to produce."""
    p = manifest_path(outdir)
    p.parent.mkdir(parents=True, exist_ok=True)
    data = {"version": SCHEMA, "fingerprint": fp,
            "updated": time.strftime("%Y-%m-%dT%H:%M:%S"),
            "units": [str(u) for u in units]}
    tmp = p.with_suffix(p.suffix + ".tmp")
    tmp.write_text(json.dumps(data, indent=2), encoding="utf-8")
    os.replace(tmp, p)            # atomic on POSIX/Windows


def unit_complete(root_path) -> bool:
    """A unit is done iff its summary .root.txt exists (written last)."""
    return os.path.exists(str(root_path) + ".txt")


def resumable(outdir, fp: str, units: list[str]) -> bool:
    """True if a prior manifest matches this fingerprint AND at least one (but
    not all) of the planned units is already complete — i.e. there is genuine
    finished work to reuse and remaining work to do."""
    man = read_manifest(outdir)
    if not man or man.get("fingerprint") != fp:
        return False
    done = [u for u in units if unit_complete(u)]
    return 0 < len(done) < len(units)


def pending_units(outdir, fp: str, units: list[str]) -> list[str]:
    """Units that still need to run.  If the manifest doesn't match the
    fingerprint we can't trust existing outputs, so everything is pending."""
    man = read_manifest(outdir)
    if not man or man.get("fingerprint") != fp:
        return list(units)
    return [u for u in units if not unit_complete(u)]


def completed_units(outdir, fp: str, units: list[str]) -> list[str]:
    man = read_manifest(outdir)
    if not man or man.get("fingerprint") != fp:
        return []
    return [u for u in units if unit_complete(u)]
