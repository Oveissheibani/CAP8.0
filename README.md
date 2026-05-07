# CAP — Correlation Analysis Package

A ROOT-based C++ framework for correlation, balance-function, jet, flow,
spherocity and HBT analyses on Monte-Carlo and experimental particle-physics
data.

## Quick install

Three ways to set up the build, all driven by the same detector:

```bash
./install              # auto: GUI if available, else CLI
./install --gui        # graphical installer (Tkinter)
./install --cli        # text-prompt installer
./install --headless   # non-interactive end-to-end (CI / batch jobs)
```

The installer scans your machine for ROOT, Pythia 8, and FastJet — including
Homebrew, MacPorts, conda, CVMFS, environment modules, and the Wayne State Grid
software trees — then either picks the best candidate or asks you. It can also
download and build a fresh in-tree Pythia and the vendored FastJet for you.

Every action is logged to `logs/`. On a build failure, look at
`logs/last-setup.log` or `logs/last-build.log`.

See **[BUILDING.md](BUILDING.md)** for the full guide and **[CODEBASE_MAP.md](CODEBASE_MAP.md)**
for a tour of the source tree.

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
├── install             ← top-level launcher (CLI | GUI | headless | run)
├── setup-cap           ← CLI installer
├── setup-cap-gui       ← graphical (Tkinter) installer
├── run-cap             ← graphical (Tkinter) job runner
├── SetupCAP.sh         ← shell environment setup, sourced from .cap-config
├── BUILDING.md         ← full build guide
├── CODEBASE_MAP.md     ← architecture & module reference
├── CMakeLists.txt      ← top-level CMake
├── cmake/              ← FindPythia8.cmake, FindFastJet.cmake
├── scripts/            ← logging library shared by CLI / GUI
├── src/                ← CAP source (one library per subdir)
├── projects/           ← .ini configurations for example analyses
├── fastjet-3.4.3/      ← vendored FastJet source (built on demand)
└── DB/                 ← particle / decay databases
```

## License

See [LICENSE](LICENSE).
