# CAP — Correlation Analysis Package

A ROOT-based C++ framework for correlation, balance-function, jet, flow,
spherocity and HBT analyses on Monte-Carlo and experimental particle-physics
data.

## Quick install

One launcher, several modes:

```bash
./install              # auto: GUI if available, else CLI
./install --gui        # graphical installer (Tkinter)
./install --cli        # text-prompt installer
./install --headless   # non-interactive end-to-end (CI / batch jobs)
./install --preflight  # just print the cross-platform readiness report
./install --run        # launch run-cap (job runner) once CAP is built
./install --build-ini  # launch the .ini composer (build-ini-gui)
./install --reset      # wipe .cap-config and start over
```

Before any setup, `install` runs a **cross-platform pre-flight check**
(`scripts/preflight.sh`) that verifies bash, Python 3 + tkinter, CMake,
a C++ compiler, and ROOT, and probes for Pythia 8, FastJet, HepMC3, LHAPDF,
YODA, and Rivet. If anything is missing, it prints exact install commands for
your platform — Homebrew (Apple Silicon and Intel), MacPorts, apt (Debian /
Ubuntu), dnf (Fedora / RHEL), conda, CVMFS, and environment modules.

The installer then scans for ROOT, Pythia 8, and FastJet — including the same
package managers plus the Wayne State Grid software trees — and either picks
the best candidate or asks you. It can also download and build a fresh in-tree
Pythia and the vendored FastJet for you.

Every action is logged to `logs/`. On a build failure, look at
`logs/last-setup.log` or `logs/last-build.log`.

## Running

After the installer finishes, the easiest way to run jobs is the runner GUI:

```bash
./install --run        # opens run-cap (Tk) — pick project, task, output
```

That gives you drop-downs for every project under `projects/`, lists the tasks
inside the chosen `.ini`, and supports event-count / random-seed overrides
without editing files by hand.

Or run directly from the command line:

```bash
source SetupCAP.sh
./bin/CAP RunAnalysis Pythia/pp_13.7TeV RunAna.ini test
```

This launches the example pp 13 TeV Pythia workflow defined in
`projects/Pythia/pp_13.7TeV/RunAna.ini`. Output histograms land in
`./histos/test/`.

## Prerequisites recap

| Component | Required? | Notes |
|-----------|-----------|-------|
| CMake ≥ 3.16 | yes | |
| C++14 compiler | yes | GCC 9+, Clang 10+, Apple Clang |
| ROOT 6 | yes | components: EG MathCore MathMore RIO Hist Tree Net |
| Pythia 8 | optional | only for `CAP_ENABLE_PYTHIA` / `CAP_ENABLE_JETS` |
| FastJet 3 | optional | only for `CAP_ENABLE_JETS` (the source is vendored) |

## Layout

```
.
├── install                  ← top-level launcher (CLI | GUI | headless | run | build-ini)
├── setup-cap                ← CLI installer (with bash 3.2 readarray shim for old macOS)
├── setup-cap-gui            ← thin wrapper → gui/setup-cap-gui
├── run-cap                  ← thin wrapper → gui/run-cap
├── gui/                     ← Tk GUIs (job runner, graphical installer, shared theming)
│   ├── run-cap                  ← graphical job runner
│   └── setup-cap-gui            ← graphical installer
├── analyses/builder/        ← .ini composer GUI + generator presets bank
│   ├── build-ini-gui            ← entry point (also reachable via ./install --build-ini)
│   ├── cap_ini_builder.py
│   ├── cap_theme.py             ← shared 18-slot palette + persistence at ~/.cap_theme.json
│   ├── cap_preset.py            ← shared JSON preset format
│   ├── generator_presets.py     ← 26 Pythia + 15 Herwig tunes
│   └── wsu_script_generator.py  ← Wayne State warrior 3-script SLURM bundle
├── scripts/                 ← shared shell helpers
│   ├── preflight.sh             ← cross-platform readiness check + install hints
│   └── cap-logging.sh           ← logging library shared by CLI / GUI
├── SetupCAP.sh              ← shell environment setup, sourced from .cap-config
├── CMakeLists.txt           ← top-level CMake
├── cmake/                   ← FindPythia8.cmake, FindFastJet.cmake
├── src/                     ← CAP source (one library per subdir)
├── projects/                ← .ini configurations for example analyses
├── fastjet-3.4.3/           ← FastJet source — gitignored, fetched on demand by setup-cap
└── DB/                      ← particle / decay databases
```

## License

See [LICENSE](LICENSE).
