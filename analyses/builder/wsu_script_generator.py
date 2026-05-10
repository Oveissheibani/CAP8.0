"""
Wayne State `warrior` cluster — automatic SLURM-bundle generator.

Given a `WSUJob` description (paths to existing personal installs, generator
toggles, array shape, output dir), this module writes a self-contained
folder under  Grid/WSU/auto/<run-name>/  that you can `scp` to warrior and
`sbatch` directly.  No login-node steps; everything runs on compute nodes.

Bundle layout (exactly three SLURM scripts + steering + README):

    Grid/WSU/auto/<name>/
    ├── install_and_build.sh   # 1. install/build — verify or rebuild deps,
    │                          #    then cmake + make CAP itself, linked
    │                          #    against $HOME/Herwig, $HOME/PYTHIA8,
    │                          #    $HOME/LHAPDF, $HOME/EPOS4/.../hepmc3
    ├── 20_pythia.cmnd         # Pythia readString block (only if enabled)
    ├── 20_herwig.in           # Herwig 7 deck       (only if enabled)
    ├── workflow_array.sh      # 2. one task = one seed:
    │                          #    Pythia/Herwig → RunAnalysis (Single+Pair)
    │                          #    → RunDerived → RunBf
    ├── subsample.sh           # 3. combine the per-task ROOT files
    └── README.md              # upload + sbatch chain

SLURM header conventions:
- Every script sets --job-name (prefixed with the bundle name),
  --partition, --output, --error, --time, --mem, --ntasks, --cpus-per-task,
  --mail-type, --mail-user.
- workflow_array.sh adds --array=0-(n_tasks-1).
- All paths inside the scripts are absolute — they don't depend on cwd.

Defaults pulled verbatim from herwig7_install_prompt 2.txt:
- partition `mdtp`
- modules  gnu7/7.3.0, cmake/3.21.1, root/6.28.10, fastjet/3.4.0, gsl/2.5
- existing personal installs at $HOME/Herwig, $HOME/PEG, $HOME/PYTHIA8,
  $HOME/LHAPDF, plus the EPOS4-shipped HepMC3 at $HOME/EPOS4/install/hepmc3
- seed offset 1000 (avoid the seed=0 RNG-init trap from §9)
"""

from __future__ import annotations

from dataclasses import dataclass, field
from pathlib import Path
from typing import Dict, List, Optional


# ---------------------------------------------------------------------------
#  Cluster defaults
# ---------------------------------------------------------------------------
DEFAULT_USER_HOME = "/wsu/home/hx/hx45/hx4574"
DEFAULT_PARTITION = "mdtp"
DEFAULT_MAIL      = "your_email@domain.com"

DEFAULT_MODULES = [
    "gnu7/7.3.0",
    "cmake/3.21.1",
    "root/6.28.10",
    "fastjet/3.4.0",
    "gsl/2.5",
]

DEFAULT_PATHS = {
    "herwig_prefix":   "$HOME/Herwig/install",
    "thepeg_prefix":   "$HOME/PEG/install",
    "pythia_prefix":   "$HOME/PYTHIA8",
    "lhapdf_prefix":   "$HOME/LHAPDF/install",
    "lhapdf_data":     "$HOME/LHAPDF/install/share/LHAPDF",
    "hepmc3_prefix":   "$HOME/EPOS4/install/hepmc3",
    "cap_prefix":      "$HOME/CAP",
    "cap_source":      "$HOME/CAP/CAP8.0-main",  # cmake source root
}


# ---------------------------------------------------------------------------
#  Job description
# ---------------------------------------------------------------------------
@dataclass
class WSUJob:
    name: str = "run01"
    user_home: str = DEFAULT_USER_HOME
    partition: str = DEFAULT_PARTITION
    mail: str = DEFAULT_MAIL

    # Generator selection
    enable_pythia: bool = True
    enable_herwig: bool = False
    enable_epos:   bool = False              # placeholder
    enable_hepmc3_reader_only: bool = False  # if True, no generator runs;
                                             # the workflow expects an
                                             # external .hepmc input

    # Install mode per dependency: "link" reuses existing prefix,
    # "rebuild" generates an inline rebuild step in install_and_build.sh
    install_mode_pythia: str = "link"
    install_mode_herwig: str = "link"
    install_mode_lhapdf: str = "link"

    paths: Dict[str, str] = field(default_factory=lambda: dict(DEFAULT_PATHS))
    modules: List[str] = field(default_factory=lambda: list(DEFAULT_MODULES))

    # Array geometry
    n_tasks: int = 50
    events_per_task: int = 10000
    seed_offset: int = 1000

    # Resources
    time_install:    str = "04:00:00"
    mem_install:     str = "16G"
    cpus_install:    int = 4

    time_workflow:   str = "08:00:00"
    mem_workflow:    str = "8G"
    cpus_workflow:   int = 2

    time_subsample:  str = "01:00:00"
    mem_subsample:   str = "16G"
    cpus_subsample:  int = 4

    # Steering blobs (already composed by collect_pythia_strings /
    # collect_herwig_lines from generator_presets — preserves your local
    # preset / numeric / stable-particles / cτ-cut / custom choices).
    pythia_cmnd_lines: List[str] = field(default_factory=list)
    herwig_in_lines:   List[str] = field(default_factory=list)

    # CAP analysis bits — drives which CAP commands the workflow invokes
    # AND what `subsample.sh` combines.  These mirror the Compose-tab
    # checkboxes 1:1 so the grid run matches the local run.
    project_dir: str = "$CAP_PREFIX/analyses/projects"
    ini_filename: str = "auto.ini"
    cap_bin: str = "$CAP_PREFIX/bin/CAP"

    # Per-stage toggles
    run_analysis: bool = True            # always — emits RunAnalysis*.root
    run_derived:  bool = False           # only if user enabled it locally
    run_bf:       bool = False           # only if user enabled it locally

    # Mirror of self.compose_choice_vars — purely informational, written
    # into the README so the user can see at a glance which analyzers
    # the .ini configures.
    analyses_enabled: Dict[str, bool] = field(default_factory=dict)

    # Subsample patterns — which ROOT files to combine.  Auto-derived
    # from run_analysis/run_derived/run_bf at write time; the user can
    # override by editing this list.
    subsample_patterns: List[str] = field(default_factory=list)

    # Where work directories land on warrior
    output_dir: str = "$HOME/cap_runs"


# ---------------------------------------------------------------------------
#  SLURM header helper
# ---------------------------------------------------------------------------
def _slurm_header(job: WSUJob, *, jobname_suffix: str, log_basename: str,
                  cpus: int, mem: str, time: str,
                  array: Optional[str] = None) -> str:
    """Returns the #SBATCH preamble.  log_basename is the filename root
    that goes under {output_dir}/logs/.  Array variant uses %A_%a; non-array
    uses %j."""
    out_pat = (f"{job.output_dir}/logs/{log_basename}_%A_%a.out"
               if array else f"{job.output_dir}/logs/{log_basename}_%j.out")
    err_pat = (f"{job.output_dir}/logs/{log_basename}_%A_%a.err"
               if array else f"{job.output_dir}/logs/{log_basename}_%j.err")
    lines = [
        "#!/bin/bash",
        f"#SBATCH --job-name={job.name}_{jobname_suffix}",
        f"#SBATCH --partition={job.partition}",
        f"#SBATCH --output={out_pat}",
        f"#SBATCH --error={err_pat}",
        "#SBATCH --ntasks=1",
        f"#SBATCH --cpus-per-task={cpus}",
        f"#SBATCH --time={time}",
        f"#SBATCH --mem={mem}",
        "#SBATCH --mail-type=END,FAIL",
        f"#SBATCH --mail-user={job.mail}",
    ]
    if array:
        lines.append(f"#SBATCH --array={array}")
    lines.append("")
    return "\n".join(lines)


def _env_block(job: WSUJob) -> str:
    """Module setup + path exports.  Inlined into every script — it's
    short enough that having it self-contained beats sourcing a separate
    file (no ordering surprises, no chmod confusion).

    Path lookup uses .get(...) with the module-level DEFAULT_PATHS as
    a fallback so older saved presets that pre-date a new path key
    (e.g. cap_source) still work after a reload — the user just sees
    the default instead of a KeyError."""
    p = lambda k: job.paths.get(k) or DEFAULT_PATHS.get(k, "")
    return f"""set -euo pipefail
module purge
module load {' '.join(job.modules)}
export CC=$(which gcc)
export CXX=$(which g++)
export FC=$(which gfortran)
ulimit -s unlimited

# Personal install paths
export HERWIG_PREFIX={p('herwig_prefix')}
export THEPEG_PREFIX={p('thepeg_prefix')}
export PYTHIA_PREFIX={p('pythia_prefix')}
export LHAPDF_PREFIX={p('lhapdf_prefix')}
export LHAPDF_DATA_PATH={p('lhapdf_data')}
export HEPMC3_PREFIX={p('hepmc3_prefix')}
export CAP_PREFIX={p('cap_prefix')}
export CAP_SOURCE={p('cap_source')}

# Composite link/run paths
export PATH=$HERWIG_PREFIX/bin:$THEPEG_PREFIX/bin:$LHAPDF_PREFIX/bin:$CAP_PREFIX/bin:$PATH
export LD_LIBRARY_PATH=$HERWIG_PREFIX/lib:$HERWIG_PREFIX/lib/Herwig:$THEPEG_PREFIX/lib:$THEPEG_PREFIX/lib/ThePEG:$LHAPDF_PREFIX/lib:$HEPMC3_PREFIX/lib64:$CAP_PREFIX/lib:${{LD_LIBRARY_PATH:-}}

mkdir -p {job.output_dir}/logs
"""


# ---------------------------------------------------------------------------
#  Script 1: install_and_build.sh
# ---------------------------------------------------------------------------
def _script_install_and_build(job: WSUJob) -> str:
    """One script that:
      1. Verifies the modules + the user's existing personal installs,
         OR rebuilds whichever dep is in 'rebuild' mode.
      2. Configures CAP via cmake, linking to those installs.
      3. Builds CAP.
      4. Confirms bin/CAP exists.
    """
    h = _slurm_header(job, jobname_suffix="install",
                      log_basename="install_and_build",
                      cpus=job.cpus_install,
                      mem=job.mem_install, time=job.time_install)

    # ---- Optional rebuild blocks ----
    rebuild_blocks: list[str] = []

    if job.install_mode_lhapdf == "rebuild":
        rebuild_blocks.append(r"""
# ── LHAPDF 6.5.4 (rebuild from source) ───────────────────────────────────
echo "[install] LHAPDF rebuild requested"
SRC=$HOME/HERWIG2/sources; mkdir -p $SRC; cd $SRC
VER=6.5.4
[[ -f LHAPDF-${VER}.tar.gz ]] || \
  wget --no-check-certificate -O LHAPDF-${VER}.tar.gz \
       https://lhapdf.hepforge.org/downloads/?f=LHAPDF-${VER}.tar.gz
rm -rf LHAPDF-${VER} && tar xzf LHAPDF-${VER}.tar.gz && cd LHAPDF-${VER}
./configure --prefix=$LHAPDF_PREFIX --disable-python \
    CC=$CC CXX=$CXX FC=$FC \
    CXXFLAGS='-O2 -g -fPIC -std=c++14'
make -j4 && make install
$LHAPDF_PREFIX/bin/lhapdf-config --version
""")

    if job.enable_pythia and job.install_mode_pythia == "rebuild":
        rebuild_blocks.append(r"""
# ── Pythia 8 (rebuild from source) ───────────────────────────────────────
echo "[install] Pythia rebuild requested"
SRC=$HOME/PYTHIA8_NEW/sources; mkdir -p $SRC $PYTHIA_PREFIX; cd $SRC
VER=8.310
[[ -f pythia${VER//./}.tgz ]] || \
  wget --no-check-certificate -O pythia${VER//./}.tgz \
       https://pythia.org/download/pythia83/pythia${VER//./}.tgz
rm -rf pythia${VER//./} && tar xzf pythia${VER//./}.tgz && cd pythia${VER//./}
./configure --prefix=$PYTHIA_PREFIX --with-lhapdf6=$LHAPDF_PREFIX \
            --with-hepmc3=$HEPMC3_PREFIX \
            --with-fastjet3=$(dirname $(dirname $(which fastjet-config))) \
            --cxx=$CXX --cxx-common='-O2 -fPIC -std=c++14'
make -j4 && make install
$PYTHIA_PREFIX/bin/pythia8-config --version
""")

    if job.enable_herwig and job.install_mode_herwig == "rebuild":
        rebuild_blocks.append(r"""
# ── ThePEG 2.3.0 + Herwig 7.3.0 (rebuild from source) ────────────────────
echo "[install] Herwig rebuild requested"
SRC=$HOME/HERWIG2/sources; mkdir -p $SRC; cd $SRC

VER=2.3.0
[[ -f ThePEG-${VER}.tar.bz2 ]] || \
  wget --no-check-certificate -O ThePEG-${VER}.tar.bz2 \
       https://thepeg.hepforge.org/downloads/?f=ThePEG-${VER}.tar.bz2
rm -rf ThePEG-${VER} && tar xjf ThePEG-${VER}.tar.bz2 && cd ThePEG-${VER}
./configure --prefix=$THEPEG_PREFIX \
    --with-gsl=$(dirname $(dirname $(which gsl-config))) \
    --with-boost=/usr \
    --with-hepmc=$HEPMC3_PREFIX --with-hepmcversion=3 \
    --with-lhapdf=$LHAPDF_PREFIX \
    --with-fastjet=$(dirname $(dirname $(which fastjet-config))) \
    CC=$CC CXX=$CXX FC=$FC F77=$FC \
    CXXFLAGS='-O2 -g -fPIC -std=c++14' \
    CPPFLAGS="-I$HEPMC3_PREFIX/include -I$LHAPDF_PREFIX/include" \
    LDFLAGS="-L$HEPMC3_PREFIX/lib64 -L$LHAPDF_PREFIX/lib -Wl,-rpath,$HEPMC3_PREFIX/lib64 -Wl,-rpath,$LHAPDF_PREFIX/lib"
make -j4 && make install

cd $SRC
VER=7.3.0
[[ -f Herwig-${VER}.tar.bz2 ]] || \
  wget --no-check-certificate -O Herwig-${VER}.tar.bz2 \
       https://herwig.hepforge.org/downloads/?f=Herwig-${VER}.tar.bz2
rm -rf Herwig-${VER} && tar xjf Herwig-${VER}.tar.bz2 && cd Herwig-${VER}
./configure --prefix=$HERWIG_PREFIX \
    --with-thepeg=$THEPEG_PREFIX \
    --with-fastjet=$(dirname $(dirname $(which fastjet-config))) \
    --with-gsl=$(dirname $(dirname $(which gsl-config))) \
    --with-boost=/usr \
    CC=$CC CXX=$CXX FC=$FC F77=$FC \
    CXXFLAGS='-O2 -g -fPIC -std=c++14'
make -j4 && make install
$HERWIG_PREFIX/bin/herwig-config --version
""")

    rebuild_section = ("\n".join(rebuild_blocks) if rebuild_blocks
                       else "echo '[install] all deps in LINK mode — using "
                            "existing personal installs.'")

    cmake_flags = []
    if job.enable_pythia:
        cmake_flags.append("-DCAP_ENABLE_PYTHIA=ON  -DPYTHIA8_DIR=$PYTHIA_PREFIX")
    if job.enable_herwig:
        cmake_flags.append("-DCAP_ENABLE_HERWIG=ON  -DCAP_HERWIG_PATH=$HERWIG_PREFIX")
    cmake_flags.append("-DCAP_ENABLE_HEPMC3=ON  -DHEPMC3_PREFIX=$HEPMC3_PREFIX")
    cmake_flags.append("-DCAP_ENABLE_LHAPDF=ON  -DLHAPDF_DIR=$LHAPDF_PREFIX")
    cmake_str = " \\\n      ".join(cmake_flags)

    return h + _env_block(job) + f"""
echo "──────────────────────────────────────────────────────────────"
echo "  CAP install / build for bundle: {job.name}"
echo "──────────────────────────────────────────────────────────────"

# ── Step 1: Verify modules + existing installs ───────────────────────────
echo "[verify] toolchain"
gcc --version | head -1
cmake --version | head -1
root-config --version

echo "[verify] existing personal installs"
test -x $HEPMC3_PREFIX/bin/HepMC3-config  || {{ echo "MISSING $HEPMC3_PREFIX"; exit 2; }}
{('test -x $LHAPDF_PREFIX/bin/lhapdf-config || { echo "MISSING $LHAPDF_PREFIX"; exit 2; }'
  if job.install_mode_lhapdf == "link" else "")}
{('test -x $PYTHIA_PREFIX/bin/pythia8-config || echo "WARN: no pythia8-config (may be header-only Pythia)"'
  if job.enable_pythia and job.install_mode_pythia == "link" else "")}
{('test -x $HERWIG_PREFIX/bin/herwig-config  || { echo "MISSING $HERWIG_PREFIX"; exit 2; }'
  if job.enable_herwig and job.install_mode_herwig == "link" else "")}

# ── Step 2: Optional dep rebuild blocks ──────────────────────────────────
{rebuild_section}

# ── Step 3: Configure + build CAP itself ─────────────────────────────────
echo "[build] CAP cmake configure"
test -d $CAP_SOURCE || {{ echo "MISSING $CAP_SOURCE"; exit 3; }}
mkdir -p $CAP_PREFIX/build
cd $CAP_PREFIX/build
cmake -DCMAKE_BUILD_TYPE=Release \\
      -DCMAKE_INSTALL_PREFIX=$CAP_PREFIX \\
      {cmake_str} \\
      $CAP_SOURCE
make -j{job.cpus_install}
make install

# ── Step 4: Smoke ────────────────────────────────────────────────────────
test -x $CAP_PREFIX/bin/CAP || {{ echo "build OK but bin/CAP missing"; exit 4; }}
echo "[OK] $CAP_PREFIX/bin/CAP ready."
$CAP_PREFIX/bin/CAP --help 2>&1 | head -5 || true
echo "[done] install_and_build complete for {job.name}"
"""


# ---------------------------------------------------------------------------
#  Steering files
# ---------------------------------------------------------------------------
def _file_pythia_cmnd(job: WSUJob) -> str:
    header = [
        f"! Pythia 8 steering — auto-generated by run-cap WSU panel.",
        f"! Bundle: {job.name}    Events/task: {job.events_per_task}",
        "!",
        "Beams:idA = 2212",
        "Beams:idB = 2212",
        "Beams:eCM = 13000.",
        f"Main:numberOfEvents = {job.events_per_task}",
        "Main:timesAllowErrors = 10",
        "",
    ]
    return "\n".join(header + (job.pythia_cmnd_lines or []) + [""])


def _file_herwig_in(job: WSUJob) -> str:
    header = [
        f"# Herwig 7 deck — auto-generated by run-cap WSU panel.",
        f"# Bundle: {job.name}    Events/task: {job.events_per_task}",
        "",
        "read LHC.in",
        "set /Herwig/Generators/EventGenerator:EventHandler:LuminosityFunction:Energy 13000.0",
        f"set /Herwig/Generators/EventGenerator:NumberOfEvents {job.events_per_task}",
        "",
    ]
    body = list(job.herwig_in_lines or [])
    tail = [
        "",
        "# HepMC writer — must come BEFORE saverun.",
        "cd /Herwig/Analysis",
        "set /Herwig/Analysis/HepMCFile:Filename events.hepmc",
        "set /Herwig/Analysis/HepMCFile:Format GenEventHepMC3",
        "set /Herwig/Analysis/HepMCFile:Units GeV_mm",
        f"set /Herwig/Analysis/HepMCFile:PrintEvent {job.events_per_task}",
        "insert /Herwig/Generators/EventGenerator:AnalysisHandlers 0 /Herwig/Analysis/HepMCFile",
        "",
        "saverun pp_run /Herwig/Generators/EventGenerator",
        "",
    ]
    return "\n".join(header + body + tail)


# ---------------------------------------------------------------------------
#  Helper: assemble the CAP invocation block based on the user's
#          Compose-tab analysis selections (run_analysis / run_derived /
#          run_bf).
# ---------------------------------------------------------------------------
def _emit_cap_chain(job: WSUJob) -> str:
    """Bash snippet inlined into workflow_array.sh.  RunAnalysis is
    always present if requested (it executes whatever analyzer subtasks
    the .ini lists — Single, Pair, Pair3D, NuDyn, ... per the local
    Compose-tab choices).  RunDerived and RunBf are conditional."""
    lines: list[str] = []
    if job.run_analysis:
        lines.append('echo "[CAP] RunAnalysis"')
        lines.append(
            f"$CAP_PREFIX/bin/CAP RunAnalysis "
            f"{job.project_dir} {job.ini_filename} $WORK")
    if job.run_derived:
        lines.append('echo "[CAP] RunDerived"')
        lines.append(
            f"$CAP_PREFIX/bin/CAP RunDerived  "
            f"{job.project_dir} {job.ini_filename} $WORK")
    if job.run_bf:
        lines.append('echo "[CAP] RunBf"')
        lines.append(
            f"$CAP_PREFIX/bin/CAP RunBf       "
            f"{job.project_dir} {job.ini_filename} $WORK")
    if not lines:
        lines.append('echo "[warn] no CAP stages selected — nothing to do"')
    return "\n".join(lines)


def _default_subsample_patterns(job: WSUJob) -> list[str]:
    """If the user didn't override `subsample_patterns`, derive sensible
    defaults from which CAP stages are enabled."""
    if job.subsample_patterns:
        return list(job.subsample_patterns)
    pats = []
    if job.run_bf:        pats.append("*/RunBf*.root")
    if job.run_derived:   pats.append("*/RunDerived*.root")
    if job.run_analysis:  pats.append("*/RunAnalysis*.root")
    return pats or ["*/*.root"]


# ---------------------------------------------------------------------------
#  Script 2: workflow_array.sh — generator + RunAnalysis + RunDerived + RunBf
# ---------------------------------------------------------------------------
def _script_workflow_array(job: WSUJob) -> str:
    arr = f"0-{job.n_tasks-1}"
    h = _slurm_header(job, jobname_suffix="workflow",
                      log_basename="workflow",
                      cpus=job.cpus_workflow, mem=job.mem_workflow,
                      time=job.time_workflow, array=arr)

    # Determine which generator block to embed.  The bundle is built for
    # one primary generator; if multiple are enabled we pick Pythia first
    # because it's more forgiving on the cluster.  Same logic as in run-cap.
    if job.enable_pythia:
        gen_label = "pythia"
    elif job.enable_herwig:
        gen_label = "herwig"
    elif job.enable_hepmc3_reader_only:
        gen_label = "hepmc3"
    else:
        gen_label = "pythia"        # fallback default

    if gen_label == "pythia":
        gen_block = f"""# ── Generator: Pythia 8 ──────────────────────────────────────────
# CAP drives Pythia directly via the .ini (Generator kind = Pythia).
# We only need the steering file + per-task seed override.
cp $BUNDLE/20_pythia.cmnd $WORK/run.cmnd
cat >> $WORK/run.cmnd <<EOF
Random:setSeed = on
Random:seed = $SEED
Main:numberOfEvents = {job.events_per_task}
EOF
"""
    elif gen_label == "herwig":
        gen_block = f"""# ── Generator: Herwig 7 (file-based HepMC3 bridge) ───────────────
cp $BUNDLE/20_herwig.in $WORK/run.in

# Patch the per-task seed.  Replace if present, append if not.
if grep -q "RandomNumberGenerator:Seed" $WORK/run.in; then
    sed -i -E "s|^.*RandomNumberGenerator:Seed.*$|set /Herwig/Generators/EventGenerator:RandomNumberGenerator:Seed $SEED|" $WORK/run.in
else
    sed -i "/^saverun /i set /Herwig/Generators/EventGenerator:RandomNumberGenerator:Seed $SEED" $WORK/run.in
fi

# Phase 1: deck → .run
$HERWIG_PREFIX/bin/Herwig read $WORK/run.in
# Phase 2: .run → events.hepmc
$HERWIG_PREFIX/bin/Herwig run $WORK/pp_run.run -N {job.events_per_task} -s $SEED
ls -la $WORK/events.hepmc
"""
    else:  # hepmc3 reader-only
        gen_block = """# ── Generator: HepMC3 reader-only (no on-cluster generator) ──────
echo "[info] HepMC3 reader-only mode — using events.hepmc supplied externally."
test -s $WORK/events.hepmc || {
    echo "ERROR: $WORK/events.hepmc missing — copy your input file in first."
    exit 5
}
"""

    return h + _env_block(job) + f"""
# Per-array-task variables
TASK=$SLURM_ARRAY_TASK_ID
SEED=$(( TASK + {job.seed_offset} ))
BUNDLE={job.output_dir}/{job.name}_bundle    # uploaded location of this bundle
WORK={job.output_dir}/{job.name}/{gen_label}/$(printf "%04d" $TASK)
mkdir -p $WORK
cd $WORK

echo "──────────────────────────────────────────────────────────────"
echo "  workflow task=$TASK seed=$SEED  generator={gen_label}"
echo "  WORK=$WORK"
echo "──────────────────────────────────────────────────────────────"

{gen_block}

# ── CAP analysis chain ───────────────────────────────────────────
# Stage selection mirrors the Compose-tab checkboxes you ticked
# locally.  The .ini already encodes which analyzer subtasks
# (Single / Pair / Pair3D / NuDyn / ...) RunAnalysis launches.
{_emit_cap_chain(job)}
echo "[done] task=$TASK"
ls -la $WORK
"""


# ---------------------------------------------------------------------------
#  Script 3: subsample.sh — combine
# ---------------------------------------------------------------------------
def _script_subsample(job: WSUJob) -> str:
    h = _slurm_header(job, jobname_suffix="subsample",
                      log_basename="subsample",
                      cpus=job.cpus_subsample,
                      mem=job.mem_subsample, time=job.time_subsample)
    gen_label = ("pythia" if job.enable_pythia else
                 "herwig" if job.enable_herwig else "hepmc3")
    patterns = _default_subsample_patterns(job)

    # Build a bash array so the script loops over every requested pattern
    # — one combined ROOT per stage that ran.  Cleaner than a hard-coded
    # 'RunBf' line.
    pat_array = " \\\n               ".join(f"'{p}'" for p in patterns)

    return h + _env_block(job) + f"""
# Combine per-task ROOT files into  $OUTROOT/_combined/ .
# The patterns reflect the CAP stages your workflow selected:
#   run_analysis = {job.run_analysis}
#   run_derived  = {job.run_derived}
#   run_bf       = {job.run_bf}
OUTROOT={job.output_dir}/{job.name}/{gen_label}
COMBINE=$OUTROOT/_combined
mkdir -p $COMBINE
cd $OUTROOT

PATTERNS=( {pat_array} )

echo "──────────────────────────────────────────────────────────────"
echo "  subsample combine — bundle '{job.name}'"
echo "  OUTROOT=$OUTROOT"
echo "  patterns: ${{PATTERNS[@]}}"
echo "──────────────────────────────────────────────────────────────"

for PAT in "${{PATTERNS[@]}}"; do
    # Stage label = the leading capitalised word in the pattern,
    # e.g. 'RunBf' → output 'RunBf_combined.root'.
    STAGE=$(echo "$PAT" | sed -E 's|^\\*/([A-Za-z]+).*|\\1|')
    OUT=$COMBINE/${{STAGE}}_combined.root
    echo "[combine] stage=$STAGE  pattern=$PAT  out=$OUT"

    if [ -f $CAP_SOURCE/analyses/builder/subsample_aggregator.py ]; then
        python3 $CAP_SOURCE/analyses/builder/subsample_aggregator.py \\
            --in  $OUTROOT --out $COMBINE --pattern "$PAT" || true
    else
        # Fallback: ROOT's hadd works for histogram-only outputs.
        FILES=$(ls $OUTROOT/$PAT 2>/dev/null || true)
        if [ -n "$FILES" ]; then
            hadd -f "$OUT" $FILES || true
        else
            echo "  [skip] no files matched $PAT"
        fi
    fi
done

ls -la $COMBINE
echo "[done] subsample combine for {job.name}"
"""


# ---------------------------------------------------------------------------
#  README
# ---------------------------------------------------------------------------
def _readme(job: WSUJob, written: list[str]) -> str:
    gen_label = ("Pythia" if job.enable_pythia else
                 "Herwig" if job.enable_herwig else "HepMC3-reader")
    return f"""# CAP run bundle — `{job.name}`

Auto-generated by the **Wayne State** tab of run-cap.  Three SLURM
scripts plus the generator steering file(s).  Upload the whole folder
to warrior, then chain the sbatches.

## Cluster

- Account home : `{job.user_home}`
- Partition    : `{job.partition}`
- Modules      : {' '.join(job.modules)}

## Generator

- Primary           : **{gen_label}**
- Pythia enabled    : {"yes" if job.enable_pythia else "no"} (mode: {job.install_mode_pythia})
- Herwig enabled    : {"yes" if job.enable_herwig else "no"} (mode: {job.install_mode_herwig})
- HepMC3 reader-only: {"yes" if job.enable_hepmc3_reader_only else "no"}

## Array geometry

- Tasks            : {job.n_tasks}
- Events per task  : {job.events_per_task}
- Seed offset      : {job.seed_offset}  (avoids the seed=0 trap)

## CAP workflow stages (mirrors local Compose-tab choices)

- RunAnalysis : {"yes" if job.run_analysis else "no"}
- RunDerived  : {"yes" if job.run_derived else "no"}
- RunBf       : {"yes" if job.run_bf else "no"}

Analyzers enabled in the .ini:

{chr(10).join(f"  - {k}: {'on' if v else 'off'}" for k, v in (job.analyses_enabled or {}).items()) or "  (auto from project .ini)"}

Subsample combine patterns: `{', '.join(_default_subsample_patterns(job))}`

## How to run

```bash
# 1.  Upload this entire folder.
rsync -av Grid/WSU/auto/{job.name}/ warrior:{job.output_dir}/{job.name}_bundle/

# 2.  ssh in.
ssh warrior
cd {job.output_dir}/{job.name}_bundle
chmod +x *.sh

# 3.  Three sbatches, chained with --dependency=afterok.
INSTALL=$(sbatch --parsable install_and_build.sh)
WORKFLOW=$(sbatch --parsable --dependency=afterok:$INSTALL workflow_array.sh)
sbatch --dependency=afterok:$WORKFLOW subsample.sh
```

## What each script does

| Script                  | Purpose                                                        |
|-------------------------|----------------------------------------------------------------|
| `install_and_build.sh`  | Verifies modules + linked installs, optionally rebuilds deps,  |
|                         | configures CAP via cmake, runs `make install`.                 |
| `workflow_array.sh`     | One SLURM array — each task: run generator (one seed),         |
|                         | then CAP `RunAnalysis` → `RunDerived` → `RunBf`.               |
| `subsample.sh`          | Combine per-task ROOT outputs into `<gen>/_combined/`.         |

## Files

{chr(10).join(f'  - `{n}`' for n in written)}

## Re-generating

Open run-cap, go to the **Wayne State** tab, edit fields, click ▶
Generate scripts.  This folder is overwritten in place.
"""


# ---------------------------------------------------------------------------
#  Top-level entry point
# ---------------------------------------------------------------------------
def write_bundle(job: WSUJob, dest_root: Path) -> List[Path]:
    """Render the bundle for *job* into  dest_root / job.name /  and
    return the list of file paths written."""
    dest = Path(dest_root) / job.name
    dest.mkdir(parents=True, exist_ok=True)
    written: List[Path] = []

    def _emit(name: str, content: str, *, executable: bool = True) -> None:
        path = dest / name
        path.write_text(content, encoding="utf-8")
        if executable and name.endswith(".sh"):
            path.chmod(path.stat().st_mode | 0o755)
        written.append(path)

    # 1. Install / build (always present)
    _emit("install_and_build.sh", _script_install_and_build(job))

    # Steering files (only the relevant ones)
    if job.enable_pythia:
        _emit("20_pythia.cmnd", _file_pythia_cmnd(job), executable=False)
    if job.enable_herwig:
        _emit("20_herwig.in",   _file_herwig_in(job),   executable=False)

    # 2. Workflow array (always present)
    _emit("workflow_array.sh", _script_workflow_array(job))

    # 3. Subsample combine (always present)
    _emit("subsample.sh", _script_subsample(job))

    # README
    _emit("README.md",
          _readme(job, [p.name for p in written]),
          executable=False)
    return written


# ---------------------------------------------------------------------------
#  CLI smoke
# ---------------------------------------------------------------------------
if __name__ == "__main__":
    import sys
    job = WSUJob(name="smoke",
                 enable_pythia=True, enable_herwig=True,
                 pythia_cmnd_lines=["Tune:pp = 14",
                                    "111:mayDecay = false"],
                 herwig_in_lines=["set /Herwig/Particles/pi0:Stable Stable"])
    out = Path(sys.argv[1]) if len(sys.argv) > 1 else Path("/tmp/wsu_smoke")
    written = write_bundle(job, out)
    print(f"Wrote {len(written)} files into {out / job.name}/")
    for p in written:
        print(f"  {p.name}")
