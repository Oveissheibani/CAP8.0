"""Herwig event generation for the provenance pipeline.

Reuses the EXACT recipe proven in the main GUI (run-cap._herwig_to_hepmc):
build a Herwig .in deck from the shipped LHC.in template, inject the HepMC
output snippet + the user's config/mechanism lines BEFORE `saverun` (so they
are baked into the .run), then `Herwig read` → `.run` and `Herwig run
--numevents N` → `.hepmc`.  The `.hepmc` is then consumed by
`provenance-study --hepmc3`.

This is what makes a Herwig LADDER work like Pythia: each rung gets its own
deck with that rung's mechanism on/off lines, so each rung is an independent
`.run`/`.hepmc`.

No tkinter import — pure logic + path building, unit-testable.  The actual
Herwig binary runs on the user's machine (HW_PREFIX); nothing here needs
Herwig present except `build_deck` which reads the shipped LHC.in.
"""
from __future__ import annotations

import os
from pathlib import Path

# Same default as run-cap; HW_PREFIX env overrides.
_DEFAULT_PREFIX = "/Users/oveissheibani/LocalHerwig/LocalHerwig/opt"


def hw_prefix() -> str:
    return os.environ.get("HW_PREFIX", _DEFAULT_PREFIX)


def herwig_bin() -> Path:
    return Path(hw_prefix()) / "bin" / "Herwig"


def herwig_available() -> bool:
    """True if the Herwig binary AND the shipped LHC.in template are present."""
    return herwig_bin().is_file() and \
        (Path(hw_prefix()) / "share" / "Herwig" / "LHC.in").is_file()


def _env_script() -> Path:
    return Path(hw_prefix()) / "herwig-env.sh"


def build_deck(work_dir: Path, run_stem: str, config_lines: list[str],
               seed: int | None = None) -> Path:
    """Write <work_dir>/<run_stem>.in = shipped LHC.in with the HepMC output
    snippet + `config_lines` injected BEFORE `saverun`, and the saverun target
    renamed to `run_stem` (so `Herwig read` produces <run_stem>.run).

    `seed` (when given) sets the Herwig random-number seed via
    `set /Herwig/Random:Seed N` — ESSENTIAL for chunked/resumable runs, where
    each chunk MUST use a distinct seed or it just regenerates identical events.

    Mirrors run-cap's _inject_hepmc_into_in + _inject_extra_lines_into_in
    (both insert before saverun, which is the correctness-critical point —
    anything after saverun is NOT stored in the .run)."""
    base = Path(hw_prefix()) / "share" / "Herwig" / "LHC.in"
    text = base.read_text(errors="ignore")

    block_lines = [
        "",
        "# ---- CAP-injected (provenance): HepMC output + config ----",
        "read snippets/HepMC.in",
        f"set /Herwig/Analysis/HepMC:Filename {run_stem}.hepmc",
        "set /Herwig/Analysis/HepMC:PrintEvent 100000000",
    ]
    if seed is not None:
        block_lines.append(f"set /Herwig/Random:Seed {int(seed)}")
    block_lines += [ln for ln in config_lines if str(ln).strip()]
    block_lines += ["# ----------------------------------------------------", ""]
    block = "\n".join(block_lines) + "\n"

    out, inserted = [], False
    for line in text.splitlines(keepends=True):
        s = line.lstrip()
        if (not inserted) and s.startswith("saverun "):
            out.append(block)
            sp = s.split()
            gen = sp[2] if len(sp) > 2 else "/Herwig/Generators/EventGenerator"
            out.append(f"saverun {run_stem} {gen}\n")
            inserted = True
            continue
        out.append(line)
    if not inserted:                      # non-standard deck: append our own
        out.append(block)
        out.append(f"saverun {run_stem} /Herwig/Generators/EventGenerator\n")

    work_dir.mkdir(parents=True, exist_ok=True)
    deck = work_dir / f"{run_stem}.in"
    deck.write_text("".join(out))
    return deck


def hepmc_path(work_dir: Path, run_stem: str) -> Path:
    return work_dir / f"{run_stem}.hepmc"


def gen_commands(work_dir: Path, run_stem: str, events: int) -> list[list[str]]:
    """The two shell commands (wrapped in `bash -lc` so the Runner's argv
    Popen can execute them) that build the .run and generate the .hepmc:

        source herwig-env.sh && cd <work> && Herwig read  <stem>.in
        source herwig-env.sh && cd <work> && Herwig run   <stem>.run --numevents N
    """
    env = _env_script()
    wd = str(work_dir)
    src = f'source "{env}" && cd "{wd}" && '
    return [
        ["bash", "-lc", src + f'Herwig read "{run_stem}.in"'],
        ["bash", "-lc", src + f'Herwig run "{run_stem}.run" --numevents {int(events)}'],
    ]
