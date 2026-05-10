# `analyses/` — user-defined analysis tooling

Self-contained workspace for building CAP analyses **without touching `src/`**.
Adding a new analysis type is a one-file change inside this folder.

```
analyses/
├── builder/                    Modular .ini composer
│   ├── cap_ini_builder.py      Schema + .ini renderer (Python)
│   ├── build-ini-gui           Tk wizard
│   └── presets/                One Python file per analysis type
│       ├── pythia_single.py
│       ├── pythia_pair3d.py
│       └── glauber_pbpb.py
├── projects/                   Generated .ini files land here
└── stubs/                      (future) C++ stubs of missing CAP classes
```

## What it does

The CAP runtime needs a `.ini` file describing the task tree, the filters,
and per-analyzer parameters. Hand-writing one is error-prone — you have to
keep the task-tree subtask indices, the filter creator counts, the analyzer
binning, and the per-task parameter blocks consistent.

The `build-ini-gui` is a Tk wizard that:

- knows what each task class expects,
- validates the inputs you give it,
- emits a complete, internally consistent `.ini`.

Output lands in `analyses/projects/<job-name>.ini`. The runner GUI
(`./install --run`) automatically picks up files from this directory in
addition to the canonical `projects/`.

## How to use it

```bash
./install --build-ini
```

Wizard tabs:

1. **Basics** — job name, output sub-folder, event count, report frequency.
2. **Generator** — Pythia 8, Therminator 3, Glauber MC, Basic toy, or a file
   reader (EPOS / PHSD). Only the fields relevant to the chosen kind are read.
3. **Particle filters** — table with rows for each filter (PDG, charge,
   pT/η/y ranges). + button to add, ✕ on each row to remove. Defaults give
   you π⁺/π⁻/K⁺/K⁻/p/p̄/ALL.
4. **Event filters / multiplicity** — table for event-level cuts
   (multiplicity range, energy range). Defaults: ALL + MB.
5. **Analyses** — checkboxes for which analyzers to attach (Single,
   Pair, Pair3D, Global, Spherocity, NuDyn, PtPt, Jets — Jets is currently
   disabled at build time).
6. **Histogram binning** — n / pT / η / y / φ / Q_inv / ΔP_side/out/long.
7. **Preview** — read-only view of the rendered `.ini`.

Bottom buttons: `Refresh preview`, `Save to analyses/projects/…`,
`Save and open in run-cap`.

The **Preset** dropdown at the top loads opinionated defaults for the most
common analysis types (one Python file per preset).

## Adding a new preset

1. Create a new file in `analyses/builder/presets/` — say `my_analysis.py`.
2. Inside it, define a `_build()` function returning a `Job` and a
   top-level `PRESET = Preset(name=..., description=..., build=_build)`.

Example minimum:

```python
from cap_ini_builder import (
    Job, Generator, GeneratorKind, AnalysisChoice,
    default_particle_filters, default_event_filters, Binning,
)
from . import Preset

def _build():
    return Job(
        name="alice_pp_5TeV_single",
        output_dir="alice_pp_5TeV_single",
        n_events=10000,
        generator=Generator(
            kind=GeneratorKind.PYTHIA, energy=5020.0, idA=2212, idB=2212,
        ),
        particle_filters=default_particle_filters(),
        event_filters=default_event_filters(),
        analyses=[AnalysisChoice.SINGLE],
        binning=Binning.default(),
    )

PRESET = Preset(
    name="ALICE pp 5.02 TeV → Single particle",
    description="...",
    build=_build,
)
```

Restart `build-ini-gui` and your preset appears in the **Preset** dropdown.

## Using the composer from Python (no GUI)

```python
import sys
sys.path.insert(0, "analyses/builder")
from cap_ini_builder import (
    Job, Generator, GeneratorKind, AnalysisChoice,
    default_particle_filters, default_event_filters, Binning, write_ini,
)

job = Job(
    name="quick_test",
    output_dir="quick_test",
    n_events=500,
    generator=Generator(kind=GeneratorKind.PYTHIA, energy=13000),
    particle_filters=default_particle_filters(),
    event_filters=default_event_filters(),
    analyses=[AnalysisChoice.SINGLE],
)
write_ini(job, "analyses/projects/quick_test.ini")
```

## Note on the running-it side

The generated `.ini` files use the legacy key naming
(`TaskClassName`, `nSubtasks`, `Subtask<k>:TaskName`) because that's what
the back-compat shim in `src/Base/Task.cpp` accepts and what the rest of
the shipped `projects/` files use. They will load correctly into the task
tree.

The runtime side still depends on the orchestrator and calculator classes
listed in `MISSING_CLASSES.md` (`CAP::RunAnalysis`, `CAP::ParticleTypeTask`,
`CAP::EventFilterCreator`, `CAP::ParticleFilterCreator`, etc.). The
`.ini` builder produces files ready for the day those classes exist; in
the meantime, the smoke-test workflow (`run-cap` → "Smoke test") exercises
the analyzer code path directly.

`stubs/` is reserved for future C++ stubs of those missing classes — when
implemented, the loop closes and the GUI-built `.ini` files run end-to-end.
