"""
cap_ini_builder — composes CAP .ini files from a schema-driven configuration.

Acts as a "middle-man" between user intent and the CAP loader: knows the
package structure (which task classes exist, which keys each one consumes,
what reasonable defaults are), and emits a complete, internally-consistent
.ini file.

Usage from Python:

    from cap_ini_builder import (
        Job, Generator, ParticleFilter, EventFilter, Binning,
        AnalysisChoice, write_ini,
    )

    job = Job(
        name        = "my_pp13TeV_test",
        output_dir  = "test",
        n_events    = 1000,
        generator   = Generator(kind="Pythia", energy=13000, idA=2212, idB=2212, seed=12345),
        particle_filters = [
            ParticleFilter(name="PiP", title="#pi^{+}", pdg=211),
            ParticleFilter(name="PiM", title="#pi^{-}", pdg=-211),
            ParticleFilter(name="ALL", title="ALL"),
        ],
        event_filters    = [
            EventFilter(name="ALL", title="ALL"),
            EventFilter(name="MB",  title="MB", mult_min=0, mult_max=1e6),
        ],
        analyses    = [AnalysisChoice.SINGLE, AnalysisChoice.PAIR],
        binning     = Binning.default(),
    )

    write_ini(job, "analyses/projects/my_pp13TeV_test.ini")

The GUI build-ini-gui builds a Job interactively and calls write_ini.
"""
from __future__ import annotations

from dataclasses import dataclass, field
from enum import Enum
from pathlib import Path
from typing import Optional


# ---------------------------------------------------------------------------
#  Schema — plain dataclasses so anybody can construct one in pure Python
# ---------------------------------------------------------------------------

class GeneratorKind(str, Enum):
    PYTHIA = "Pythia"
    THERMINATOR = "Therminator"
    GLAUBER = "Glauber"
    BASIC = "Basic"      # minimal toy
    EPOS_READER = "EposReader"
    PHSD_READER = "PhsdReader"
    HEPMC3 = "HepMC3"    # HepMC3 file reader (any HepMC3 producer)
    HERWIG = "Herwig"    # embedded HERWIG 7 + ThePEG (in-process generation)


class AnalysisChoice(str, Enum):
    GLOBAL = "Global"            # event-level observables (multiplicity, ET, …)
    SINGLE = "Single"            # ParticleSingleAnalyzer
    PAIR   = "Pair"              # ParticlePairAnalyzer
    PAIR3D = "Pair3D"            # ParticlePair3DAnalyzer (HBT-style)
    SPHEROCITY = "Spherocity"
    NUDYN  = "NuDyn"
    PTPT   = "PtPt"
    JETS   = "Jets"              # currently broken upstream — see JETS_KNOWN_ISSUES.md


@dataclass
class Generator:
    """Generator configuration. Only the fields relevant to the chosen kind
    are read; the rest are ignored."""
    kind: GeneratorKind = GeneratorKind.PYTHIA
    # Pythia
    energy: float = 13000.0       # GeV centre-of-mass
    idA: int = 2212               # PDG of beam A (default proton)
    idB: int = 2212               # PDG of beam B
    seed: int = 12345
    soft_qcd: bool = True
    use_qcd_cr: bool = True
    use_ropes: bool = False
    # Therminator
    temperature: float = 165.6    # MeV
    mu_B: float = 28.5
    mu_I: float = -0.9
    mu_S: float = 6.9
    mu_C: float = 0.0
    model: str = "BlastWave"      # BlastWave, HadronGas, Hydro2D, Hydro3D, BWA, KrakowSFO
    # Glauber
    nucleus_A: str = "Pb"         # element symbol or PDG-like name
    nucleus_B: str = "Pb"
    sigma_NN: float = 60.0        # mb
    impact_min: float = 0.0
    impact_max: float = 20.0
    # File readers
    input_file: str = ""
    # ---- Universal "keep" flags ---------------------------------------
    # These fields apply to ANY event source (Pythia / Herwig / HepMC3 /
    # EPOS / PHSD).  The C++ classes all support the same set of
    # SaveFinalOnly / SaveQuarks / SaveNeutrinos / SavePhotons /
    # SaveGaugeBosons properties.  Default = analyzers see only stable
    # hadrons + leptons (the standard MB workflow).
    keep_final_only:   bool = True
    keep_quarks:       bool = False
    keep_neutrinos:    bool = False
    keep_photons:      bool = False
    keep_gauge_bosons: bool = False
    # KeepStatuses spec — passed straight to the C++ generators'
    # KeepStatuses property.  Empty falls back to keep_final_only
    # behaviour (keep_final_only=True → "1" / =False → "all").  For
    # BF-per-stage workflows the GUI sets this from STAGE_FILTER_SETS.
    # Examples: "1", "1,2", "1,2,11,21,23,51,52", "all".
    keep_statuses:     str  = ""
    # Generator-config: extra raw lines emitted at the bottom of the
    # generator's .ini block.  Pythia: each becomes Option<n>.
    pythia_extra_options: list = field(default_factory=list)
    # Herwig: extra .in-deck lines spliced BEFORE the saverun line of
    # the scratch deck (only used when Herwig kind / file-based path).
    herwig_extra_in_lines: list = field(default_factory=list)
    # Tag the chosen preset so the .ini reflects what the user picked
    # (handy for replicating runs from saved .ini files).
    pythia_preset_name:   str  = ""
    herwig_preset_name:   str  = ""
    # HepMC3 reader (HERWIG / EPOS / MadGraph / Sherpa bridge)
    hepmc3_input_file: str = ""
    hepmc3_save_final_only: bool = True
    hepmc3_save_quarks: bool = False
    hepmc3_save_neutrinos: bool = False
    hepmc3_save_photons: bool = False
    hepmc3_save_gauge_bosons: bool = False
    # Embedded HERWIG 7 + ThePEG (in-process generator).
    # The .run file is produced by `Herwig read input.in` — see
    # INSTALL_REPORT_HERWIG.md §6.
    herwig_run_file: str = ""
    herwig_lhapdf_data_path: str = ""
    herwig_plugin_path: str = ""           # ":"-separated dirs
    herwig_save_final_only: bool = True
    herwig_save_quarks: bool = False
    herwig_save_neutrinos: bool = False
    herwig_save_photons: bool = False
    herwig_save_gauge_bosons: bool = False

    def to_classname(self) -> str:
        return {
            GeneratorKind.PYTHIA:      "CAP::PythiaEventGenerator",
            GeneratorKind.THERMINATOR: "CAP::Therminator3",
            GeneratorKind.GLAUBER:     "CAP::GlauberGenerator",
            GeneratorKind.BASIC:       "CAP::BasicEventGen",
            GeneratorKind.EPOS_READER: "CAP::EposEventReader",
            GeneratorKind.PHSD_READER: "CAP::PHSDEventReader",
            GeneratorKind.HEPMC3:      "CAP::HepMC3EventReader",
            GeneratorKind.HERWIG:      "CAP::HerwigEventGenerator",
        }[self.kind]

    def task_name(self) -> str:
        return {
            GeneratorKind.PYTHIA:      "PythiaEventGenerator",
            GeneratorKind.THERMINATOR: "TherminatorGenerator",
            GeneratorKind.GLAUBER:     "GlauberGenerator",
            GeneratorKind.BASIC:       "BasicEventGen",
            GeneratorKind.EPOS_READER: "EposEventReader",
            GeneratorKind.PHSD_READER: "PHSDEventReader",
            GeneratorKind.HEPMC3:      "HepMC3EventReader",
            GeneratorKind.HERWIG:      "HerwigEventGenerator",
        }[self.kind]


@dataclass
class ParticleFilter:
    """A particle selection. PDG None = ALL. pt/eta/y min/max are optional."""
    name: str                       # short id used as suffix in histo names
    title: str = ""                 # ROOT-LaTeX title; defaults to name
    pdg: Optional[int] = None       # exact PDG match; None = no PDG cut
    pt_min: Optional[float] = None
    pt_max: Optional[float] = None
    eta_min: Optional[float] = None
    eta_max: Optional[float] = None
    y_min: Optional[float] = None
    y_max: Optional[float] = None
    charge: Optional[int] = None    # +1 / -1 / 0 / None

    def __post_init__(self):
        if not self.title:
            self.title = self.name


@dataclass
class EventFilter:
    """Event-level selection. Most commonly a multiplicity range."""
    name: str
    title: str = ""
    mult_min: Optional[float] = None
    mult_max: Optional[float] = None
    energy_min: Optional[float] = None
    energy_max: Optional[float] = None

    def __post_init__(self):
        if not self.title:
            self.title = self.name


@dataclass
class Binning1D:
    nbins: int
    min: float
    max: float


@dataclass
class Binning:
    """Histogram binning across analyzers. Edit anything you don't want to
    use the defaults."""
    n:    Binning1D = field(default_factory=lambda: Binning1D(500, 0.0, 500.0))     # multiplicity
    pt:   Binning1D = field(default_factory=lambda: Binning1D(200, 0.0, 10.0))      # pT (GeV)
    eta:  Binning1D = field(default_factory=lambda: Binning1D(200, -5.0, 5.0))
    y:    Binning1D = field(default_factory=lambda: Binning1D(200, -5.0, 5.0))
    phi:  Binning1D = field(default_factory=lambda: Binning1D(72, 0.0, 6.28318531))
    # Pair 3D
    qinv:    Binning1D = field(default_factory=lambda: Binning1D(200, 0.0, 2.0))
    delta_ps: Binning1D = field(default_factory=lambda: Binning1D(80, -2.0, 2.0))
    delta_po: Binning1D = field(default_factory=lambda: Binning1D(80, -2.0, 2.0))
    delta_pl: Binning1D = field(default_factory=lambda: Binning1D(80, -2.0, 2.0))

    @classmethod
    def default(cls) -> "Binning":
        return cls()


@dataclass
class Job:
    name: str = "my_analysis"
    output_dir: str = "test"
    n_events: int = 1000
    n_events_report: int = 1000
    generator: Generator = field(default_factory=Generator)
    particle_filters: list[ParticleFilter] = field(default_factory=list)
    event_filters: list[EventFilter] = field(default_factory=list)
    analyses: list[AnalysisChoice] = field(default_factory=list)
    binning: Binning = field(default_factory=Binning)
    # Optional post-analysis stages.  Each emits a separate top-level
    # task block in the same .ini file.  RunDerived consumes the
    # *Gen.root files; RunBf consumes Pair / Pair3D outputs.
    run_derived: bool = False
    run_bf: bool = False


# ---------------------------------------------------------------------------
#  Sensible defaults
# ---------------------------------------------------------------------------

def default_particle_filters() -> list[ParticleFilter]:
    """Default (= final-state hadron) particle-filter list.

    ORDER MATTERS for the Balance-Function pipeline.  CAP determines the
    antiparticle of a given filter purely by INDEX OFFSET:

        antiparticle(filter_i)  =  filter[i + nSpecies]
                                                       (nSpecies = N/2)

    so the first half of the list must be particles and the second half
    their antiparticles, in matching species order.  See
    src/ParticlePair/BalanceFunctionCalculator.cpp (lines 511, 596 of
    the commented-out execute() block).

    The catch-all ``ALL`` filter goes at the end — pair-style analyzers
    drop it before computing pairs, so it never enters the BF
    arithmetic and doesn't break the symmetric layout.

    For BF-per-stage analyses see also:
      - parton_particle_filters()        — partons (u/d/s/c, gluon)
      - intermediate_hadron_filters()    — pre-decay charged hadrons
      - lepton_particle_filters()        — e/μ/τ for EW BF studies

    All four filter sets share the same particle/antiparticle index
    layout so the BF symmetry rule above keeps holding.
    """
    return final_hadron_filters()


def final_hadron_filters() -> list[ParticleFilter]:
    """Final-state stable hadrons — π±, K±, p,p̄ + ALL.  Same as
    historical default_particle_filters().  Use with KeepStatuses=1
    (or default SaveFinalOnly=1)."""
    return [
        # particles (first half) — π+, K+, p
        ParticleFilter("PiP", "#pi^{+}",   pdg=211),
        ParticleFilter("KP",  "K^{+}",     pdg=321),
        ParticleFilter("PP",  "p",         pdg=2212),
        # antiparticles (second half, MATCHING species order)
        ParticleFilter("PiM", "#pi^{-}",   pdg=-211),
        ParticleFilter("KM",  "K^{-}",     pdg=-321),
        ParticleFilter("PM",  "#bar{p}",   pdg=-2212),
        ParticleFilter("ALL", "ALL"),
    ]


def parton_particle_filters() -> list[ParticleFilter]:
    """Partons (quarks + gluon) for hard-scatter / shower-stage BF.

    Use with KeepStatuses including 23 (hard scatter) and / or 51,52
    (post-shower) — see Pythia / HepMC status conventions.

    BF symmetry: 4 quark species (u/d/s/c — t and b are heavy enough
    to often not appear), then 4 antiquarks in matching order, then
    a charge-blind ALL filter.  Gluon is excluded from BF pairs (no
    antiparticle) — it's available via the ALL filter instead.
    """
    return [
        # particles (first half) — quarks
        ParticleFilter("Up",      "u",       pdg=2),
        ParticleFilter("Down",    "d",       pdg=1),
        ParticleFilter("Strange", "s",       pdg=3),
        ParticleFilter("Charm",   "c",       pdg=4),
        # antiparticles (second half, MATCHING order)
        ParticleFilter("UpBar",      "#bar{u}", pdg=-2),
        ParticleFilter("DownBar",    "#bar{d}", pdg=-1),
        ParticleFilter("StrangeBar", "#bar{s}", pdg=-3),
        ParticleFilter("CharmBar",   "#bar{c}", pdg=-4),
        # catch-all (includes gluons)
        ParticleFilter("ALL", "ALL"),
    ]


def intermediate_hadron_filters() -> list[ParticleFilter]:
    """Pre-decay hadrons — same species as final_hadron_filters() but
    intended to be paired with KeepStatuses=2 (HepMC: particle that
    later decayed).  Useful for studying how decay smears the BF.
    """
    return [
        ParticleFilter("PiP_int", "#pi^{+}_{int}",   pdg=211),
        ParticleFilter("KP_int",  "K^{+}_{int}",     pdg=321),
        ParticleFilter("PP_int",  "p_{int}",         pdg=2212),
        ParticleFilter("PiM_int", "#pi^{-}_{int}",   pdg=-211),
        ParticleFilter("KM_int",  "K^{-}_{int}",     pdg=-321),
        ParticleFilter("PM_int",  "#bar{p}_{int}",   pdg=-2212),
        ParticleFilter("ALL",     "ALL"),
    ]


def lepton_particle_filters() -> list[ParticleFilter]:
    """Charged leptons — e±, μ±, τ± + ALL.  For W/Z BF studies."""
    return [
        ParticleFilter("eM",   "e^{-}",   pdg=11),
        ParticleFilter("muM",  "#mu^{-}", pdg=13),
        ParticleFilter("tauM", "#tau^{-}",pdg=15),
        ParticleFilter("eP",   "e^{+}",   pdg=-11),
        ParticleFilter("muP",  "#mu^{+}", pdg=-13),
        ParticleFilter("tauP", "#tau^{+}",pdg=-15),
        ParticleFilter("ALL",  "ALL"),
    ]


# Stage → (filter set, suggested KeepStatuses spec) registry.  GUI uses
# this to populate a Stage dropdown.
STAGE_FILTER_SETS = {
    "final_hadrons":   (final_hadron_filters,        "1"),
    "intermediate_hadrons": (intermediate_hadron_filters, "2"),
    "partons":         (parton_particle_filters,     "23,51,52"),
    "leptons":         (lepton_particle_filters,     "1"),
}


def default_event_filters() -> list[EventFilter]:
    return [
        EventFilter("ALL", "ALL"),
        EventFilter("MB",  "MB", mult_min=0.0, mult_max=1e6),
    ]


# ---------------------------------------------------------------------------
#  .ini composition — this is where the CAP knowledge lives.
# ---------------------------------------------------------------------------
#
#  We emit the LEGACY key names (TaskName, TaskClassName, nSubtasks,
#  Subtask<k>:TaskName, Subtask<k>:TaskClassName) because (a) they're what
#  the shipped projects/ files use, (b) Task.cpp now has a back-compat shim
#  accepting these (see TECHNICAL_REPORT.tex section "Configuration system:
#  backward-compatibility shim"). Per-task keys (HistogramsExportPath,
#  nParticleDbs, nEventsRequested, etc.) also stay in the legacy form here.
#
#  When the source-side configuration system is finalised on the new
#  <typeName>:* layout, this composer is the single place to update.
# ---------------------------------------------------------------------------


_HEADER = """\
# =============================================================================
#  Generated by analyses/builder/cap_ini_builder.py
#  Job   : {name}
#  Events: {n_events}
#  Output: {output_dir}
# =============================================================================
"""


def _emit_top_task(job: Job, lines: list[str]) -> None:
    """RunAnalysis top-level container."""
    n_subtasks = 4   # ParticleTypeTask + EventFilterCreator + ParticleFilterCreator + EventIterator
    lines.append("# ----- Top-level container --------------------------------------------------")
    lines.append(f"RunAnalysis:TaskName               = RunAnalysis")
    lines.append(f"RunAnalysis:TaskClassName          = CAP::RunAnalysis")
    lines.append(f"RunAnalysis:Severity               = Info")
    lines.append(f"RunAnalysis:nSubtasks              = {n_subtasks}")
    lines.append("")


def _emit_particle_db_task(lines: list[str]) -> None:
    lines.append("# ----- Subtask 0: ParticleTypeTask (loads default particle DB) --------------")
    lines.append("RunAnalysis:Subtask0:TaskName              = ParticleTypeTask")
    lines.append("RunAnalysis:Subtask0:TaskClassName         = CAP::ParticleTypeTask")
    lines.append("RunAnalysis:Subtask0:nSubtasks             = 0")
    lines.append("RunAnalysis:Subtask0:nParticleDbs          = 1")
    lines.append("RunAnalysis:Subtask0:ParticleDbName0       = DefaultDb")
    lines.append("RunAnalysis:Subtask0:ParticleDbOwner0      = 1")
    lines.append("")


def _emit_event_filter_creator(job: Job, lines: list[str]) -> None:
    lines.append("# ----- Subtask 1: EventFilterCreator ----------------------------------------")
    lines.append("RunAnalysis:Subtask1:TaskName              = EventFilterCreator")
    lines.append("RunAnalysis:Subtask1:TaskClassName         = CAP::EventFilterCreator")
    lines.append("RunAnalysis:Subtask1:nSubtasks             = 0")
    n = len(job.event_filters)
    lines.append(f"RunAnalysis:Subtask1:nEventFilters         = {n}")
    for k, f in enumerate(job.event_filters):
        lines.append(f"RunAnalysis:Subtask1:EventFilterName{k}     = {f.name}")
        lines.append(f"RunAnalysis:Subtask1:EventFilterOwner{k}    = 1")
    lines.append("")


def _emit_particle_filter_creator(job: Job, lines: list[str]) -> None:
    lines.append("# ----- Subtask 2: ParticleFilterCreator -------------------------------------")
    lines.append("RunAnalysis:Subtask2:TaskName              = ParticleFilterCreator")
    lines.append("RunAnalysis:Subtask2:TaskClassName         = CAP::ParticleFilterCreator")
    lines.append("RunAnalysis:Subtask2:nSubtasks             = 0")
    n = len(job.particle_filters)
    lines.append(f"RunAnalysis:Subtask2:nParticleFilters      = {n}")
    for k, f in enumerate(job.particle_filters):
        lines.append(f"RunAnalysis:Subtask2:ParticleFilterName{k}  = {f.name}")
        lines.append(f"RunAnalysis:Subtask2:ParticleFilterOwner{k} = 1")
    lines.append("")


_ANALYZER_PREFIX = {
    "GlobalAnalyzer":         "GA",
    "ParticleSingleAnalyzer": "PS",
    "ParticlePairAnalyzer":   "PP",
    "ParticlePair3DAnalyzer": "P3",
    "SpherocityAnalyzer":     "SP",
    "NuDynAnalyzer":          "ND",
    "PtPtAnalyzer":           "PT",
    "JetAnalyzer":            "JT",
}


def _short_prefix(task_name: str) -> str:
    """Stable, unique 2-letter prefix per analyzer — used to namespace the
    histogram base names in the global ManagedObjects store so that
    e.g. PS_ALL_PiP doesn't collide with PP_ALL_PiP."""
    return _ANALYZER_PREFIX.get(task_name, task_name[:2])


_ANALYZER_CLASS = {
    AnalysisChoice.GLOBAL:     ("GlobalAnalyzer",          "CAP::GlobalAnalyzer",         "GlobalGen.root"),
    AnalysisChoice.SINGLE:     ("ParticleSingleAnalyzer",  "CAP::ParticleSingleAnalyzer", "SingleGen.root"),
    AnalysisChoice.PAIR:       ("ParticlePairAnalyzer",    "CAP::ParticlePairAnalyzer",   "PairGen.root"),
    AnalysisChoice.PAIR3D:     ("ParticlePair3DAnalyzer",  "CAP::ParticlePair3DAnalyzer", "Pair3DGen.root"),
    AnalysisChoice.SPHEROCITY: ("SpherocityAnalyzer",      "CAP::SpherocityAnalyzer",     "Spherocity.root"),
    AnalysisChoice.NUDYN:      ("NuDynAnalyzer",           "CAP::NuDynAnalyzer",          "NuDyn.root"),
    AnalysisChoice.PTPT:       ("PtPtAnalyzer",            "CAP::PtPtAnalyzer",           "PtPt.root"),
    AnalysisChoice.JETS:       ("JetAnalyzer",             "CAP::JetAnalyzer",            "Jets.root"),
}


def _emit_event_iterator(job: Job, lines: list[str]) -> None:
    n_subtasks = 1 + len(job.analyses)         # generator + analyzers
    lines.append("# ----- Subtask 3: EventIterator (per-event loop) ----------------------------")
    lines.append("RunAnalysis:Subtask3:TaskName              = EventIterator")
    lines.append("RunAnalysis:Subtask3:TaskClassName         = CAP::EventIterator")
    lines.append(f"RunAnalysis:Subtask3:nSubtasks             = {n_subtasks}")
    lines.append("")

    # Subtask 0 of EventIterator: the generator itself
    gen = job.generator
    lines.append("# Subtask 3.0: generator")
    lines.append(f"RunAnalysis:Subtask3:Subtask0:TaskName             = {gen.task_name()}")
    lines.append(f"RunAnalysis:Subtask3:Subtask0:TaskClassName        = {gen.to_classname()}")
    lines.append("RunAnalysis:Subtask3:Subtask0:nSubtasks            = 0")
    # The generator OWNS its own Event stream (the EventProcessor needs at
    # least one entry in _managedEvents — it backs the event() accessor).
    lines.append("RunAnalysis:Subtask3:Subtask0:nEventsStreams       = 1")
    lines.append(f"RunAnalysis:Subtask3:Subtask0:StreamName0          = {gen.task_name()}Stream")
    lines.append("RunAnalysis:Subtask3:Subtask0:StreamOwner0         = 1")
    lines.append("RunAnalysis:Subtask3:Subtask0:nParticleDbs         = 1")
    lines.append("RunAnalysis:Subtask3:Subtask0:ParticleDbName0      = DefaultDb")
    lines.append("RunAnalysis:Subtask3:Subtask0:ParticleDbOwner0     = 0")
    lines.append("RunAnalysis:Subtask3:Subtask0:nEventFilters        = 1")
    lines.append("RunAnalysis:Subtask3:Subtask0:EventFilterName0     = ALL")
    lines.append("RunAnalysis:Subtask3:Subtask0:EventFilterOwner0    = 0")
    lines.append("RunAnalysis:Subtask3:Subtask0:nParticleFilters     = 1")
    lines.append("RunAnalysis:Subtask3:Subtask0:ParticleFilterName0  = ALL")
    lines.append("RunAnalysis:Subtask3:Subtask0:ParticleFilterOwner0 = 0")
    lines.append("")

    # Subtasks 1..N: each analyzer the user picked
    gen_stream_name = f"{gen.task_name()}Stream"
    for i, choice in enumerate(job.analyses, start=1):
        task_name, class_name, out_file = _ANALYZER_CLASS[choice]
        lines.append(f"# Subtask 3.{i}: {choice.value} analysis")
        lines.append(f"RunAnalysis:Subtask3:Subtask{i}:TaskName             = {task_name}")
        lines.append(f"RunAnalysis:Subtask3:Subtask{i}:TaskClassName        = {class_name}")
        lines.append(f"RunAnalysis:Subtask3:Subtask{i}:nSubtasks            = 0")
        # Analyzer borrows the generator's Event stream
        lines.append(f"RunAnalysis:Subtask3:Subtask{i}:nEventsStreams       = 1")
        lines.append(f"RunAnalysis:Subtask3:Subtask{i}:StreamName0          = {gen_stream_name}")
        lines.append(f"RunAnalysis:Subtask3:Subtask{i}:StreamOwner0         = 0")
        lines.append(f"RunAnalysis:Subtask3:Subtask{i}:nParticleDbs         = 1")
        lines.append(f"RunAnalysis:Subtask3:Subtask{i}:ParticleDbName0      = DefaultDb")
        lines.append(f"RunAnalysis:Subtask3:Subtask{i}:ParticleDbOwner0     = 0")
        # Event filters
        lines.append(f"RunAnalysis:Subtask3:Subtask{i}:nEventFilters        = {len(job.event_filters)}")
        for k, ef in enumerate(job.event_filters):
            lines.append(f"RunAnalysis:Subtask3:Subtask{i}:EventFilterName{k}     = {ef.name}")
            lines.append(f"RunAnalysis:Subtask3:Subtask{i}:EventFilterOwner{k}    = 0")
        # Particle filters (for pair analyzers, exclude "ALL" since pairs need named species)
        plist = [f for f in job.particle_filters if f.name != "ALL"] \
                if choice in (AnalysisChoice.PAIR, AnalysisChoice.PAIR3D) \
                else job.particle_filters
        lines.append(f"RunAnalysis:Subtask3:Subtask{i}:nParticleFilters     = {len(plist)}")
        for k, pf in enumerate(plist):
            lines.append(f"RunAnalysis:Subtask3:Subtask{i}:ParticleFilterName{k}  = {pf.name}")
            lines.append(f"RunAnalysis:Subtask3:Subtask{i}:ParticleFilterOwner{k} = 0")
        lines.append(f"RunAnalysis:Subtask3:Subtask{i}:ImportHistograms     = 0")
        lines.append(f"RunAnalysis:Subtask3:Subtask{i}:ExportHistograms     = 1")
        lines.append(f"RunAnalysis:Subtask3:Subtask{i}:HistogramsExportPath = Default")
        lines.append(f"RunAnalysis:Subtask3:Subtask{i}:HistogramsExportFile = {out_file}")
        lines.append("")


def _emit_eventiterator_params(job: Job, lines: list[str]) -> None:
    lines.append("# ----- EventIterator parameters --------------------------------------------")
    lines.append(f"EventIterator:nEventsRequested = {job.n_events}")
    lines.append(f"EventIterator:nEventsReport    = {job.n_events_report}")
    lines.append("")


def _emit_event_filter_creator_params(job: Job, lines: list[str]) -> None:
    lines.append("# ----- Event filter definitions --------------------------------------------")
    lines.append(f"EventFilterCreator:EventFilter:N = {len(job.event_filters)}")
    for k, f in enumerate(job.event_filters):
        lines.append(f"EventFilterCreator:EventFilter:Filter{k}:Name        = {f.name}")
        lines.append(f"EventFilterCreator:EventFilter:Filter{k}:Title       = {f.title}")
        conditions = []
        if f.mult_min is not None or f.mult_max is not None:
            conditions.append(("DoubleRange", "MULT_0",
                               f.mult_min or 0.0, f.mult_max or 1e9))
        if f.energy_min is not None or f.energy_max is not None:
            conditions.append(("DoubleRange", "ENERGY",
                               f.energy_min or 0.0, f.energy_max or 1e9))
        lines.append(f"EventFilterCreator:EventFilter:Filter{k}:nConditions = {len(conditions)}")
        for ci, (typ, sub, lo, hi) in enumerate(conditions):
            lines.append(f"EventFilterCreator:EventFilter:Filter{k}:Condition{ci}:Type    = {typ}")
            lines.append(f"EventFilterCreator:EventFilter:Filter{k}:Condition{ci}:Subtype = {sub}")
            lines.append(f"EventFilterCreator:EventFilter:Filter{k}:Condition{ci}:Minimum = {lo}")
            lines.append(f"EventFilterCreator:EventFilter:Filter{k}:Condition{ci}:Maximum = {hi}")
    lines.append("")


def _emit_particle_filter_creator_params(job: Job, lines: list[str]) -> None:
    lines.append("# ----- Particle filter definitions -----------------------------------------")
    lines.append(f"ParticleFilterCreator:ParticleFilter:N = {len(job.particle_filters)}")
    for k, f in enumerate(job.particle_filters):
        lines.append(f"ParticleFilterCreator:ParticleFilter:Filter{k}:Name        = {f.name}")
        lines.append(f"ParticleFilterCreator:ParticleFilter:Filter{k}:Title       = {f.title}")
        conditions = []
        if f.pt_min is not None or f.pt_max is not None:
            conditions.append(("DoubleRange", "PT",
                               f.pt_min or 0.0, f.pt_max or 1e6))
        if f.eta_min is not None or f.eta_max is not None:
            conditions.append(("DoubleRange", "ETA",
                               f.eta_min or -1e6, f.eta_max or 1e6))
        if f.y_min is not None or f.y_max is not None:
            conditions.append(("DoubleRange", "Y",
                               f.y_min or -1e6, f.y_max or 1e6))
        if f.pdg is not None:
            conditions.append(("Integer", "PDG", f.pdg, f.pdg))
        if f.charge is not None:
            conditions.append(("Integer", "CHARGE", f.charge, f.charge))
        lines.append(f"ParticleFilterCreator:ParticleFilter:Filter{k}:nConditions = {len(conditions)}")
        for ci, (typ, sub, lo, hi) in enumerate(conditions):
            lines.append(f"ParticleFilterCreator:ParticleFilter:Filter{k}:Condition{ci}:Type    = {typ}")
            lines.append(f"ParticleFilterCreator:ParticleFilter:Filter{k}:Condition{ci}:Subtype = {sub}")
            lines.append(f"ParticleFilterCreator:ParticleFilter:Filter{k}:Condition{ci}:Minimum = {lo}")
            lines.append(f"ParticleFilterCreator:ParticleFilter:Filter{k}:Condition{ci}:Maximum = {hi}")
    lines.append("")


def _emit_generator_params(job: Job, lines: list[str]) -> None:
    g = job.generator
    name = g.task_name()
    lines.append(f"# ----- {name} parameters --------------------------------------")
    if g.kind == GeneratorKind.PYTHIA:
        lines.append(f"{name}:SetSeed         = 1")
        lines.append(f"{name}:SeedValue       = {g.seed}")
        lines.append(f"{name}:Energy          = {g.energy}")
        lines.append(f"{name}:Beam            = {g.idA}")
        lines.append(f"{name}:Target          = {g.idB}")
        lines.append(f"{name}:UseQCDCR        = {1 if g.use_qcd_cr else 0}")
        lines.append(f"{name}:UseRopes        = {1 if g.use_ropes else 0}")
        # Particle-keep flags — universal across all generators.
        lines.append(f"{name}:SaveFinalOnly   = {1 if g.keep_final_only   else 0}")
        lines.append(f"{name}:SaveQuarks      = {1 if g.keep_quarks       else 0}")
        lines.append(f"{name}:SaveNeutrinos   = {1 if g.keep_neutrinos    else 0}")
        lines.append(f"{name}:SavePhotons     = {1 if g.keep_photons      else 0}")
        lines.append(f"{name}:SaveGaugeBosons = {1 if g.keep_gauge_bosons else 0}")
        # Legacy alias — RemovePhotons==1 forces SavePhotons=0 inside
        # the C++ generator regardless of what we wrote above.
        lines.append(f"{name}:RemovePhotons   = {0 if g.keep_photons else 1}")
        lines.append(f"{name}:Option0         = Init:showProcesses=off")
        lines.append(f"{name}:Option1         = Init:showMultipartonInteractions=off")
        lines.append(f"{name}:Option2         = Init:showChangedSettings=off")
        lines.append(f"{name}:Option3         = Init:showChangedParticleData=off")
        lines.append(f"{name}:Option4         = Next:numberCount={job.n_events_report}")
        lines.append(f"{name}:Option5         = Next:numberShowInfo=0")
        lines.append(f"{name}:Option6         = Next:numberShowProcess=0")
        lines.append(f"{name}:Option7         = Next:numberShowEvent=0")
        if g.soft_qcd:
            lines.append(f"{name}:Option8         = SoftQCD:inelastic=on")
        # Generator-config extras: each user-supplied line lands in a
        # fresh Option<n>.  PythiaEventGenerator iterates Option0..29.
        # We start at 9 so we don't clobber the Init/Next prints above.
        if g.pythia_preset_name:
            lines.append(f"# Pythia preset: {g.pythia_preset_name}")
        for k, opt in enumerate(g.pythia_extra_options or []):
            n = 9 + k
            if n > 29:           # PythiaEventGenerator's hard cap
                lines.append(f"# (skipped extra Pythia option — slot {n} > 29): {opt}")
                continue
            lines.append(f"{name}:Option{n}         = {opt}")
    elif g.kind == GeneratorKind.THERMINATOR:
        lines.append(f"{name}:Temperature       = {g.temperature}")
        lines.append(f"{name}:MuB               = {g.mu_B}")
        lines.append(f"{name}:MuI               = {g.mu_I}")
        lines.append(f"{name}:MuS               = {g.mu_S}")
        lines.append(f"{name}:MuC               = {g.mu_C}")
        lines.append(f"{name}:Model             = {g.model}")
    elif g.kind == GeneratorKind.GLAUBER:
        lines.append(f"{name}:NucleusA          = {g.nucleus_A}")
        lines.append(f"{name}:NucleusB          = {g.nucleus_B}")
        lines.append(f"{name}:CrossSectionNN    = {g.sigma_NN}")
        lines.append(f"{name}:ImpactParameterMin = {g.impact_min}")
        lines.append(f"{name}:ImpactParameterMax = {g.impact_max}")
    elif g.kind in (GeneratorKind.EPOS_READER, GeneratorKind.PHSD_READER):
        lines.append(f"{name}:InputFile         = {g.input_file}")
    elif g.kind == GeneratorKind.HEPMC3:
        # HepMC3EventReader keys — see src/CAPHepMC3/HepMC3EventReader.hpp.
        # HepMC3InputFile is REQUIRED; the rest follow the same conventions
        # as PythiaEventGenerator.  Particle-keep flags emitted below.
        lines.append(f"{name}:HepMC3InputFile   = {g.hepmc3_input_file}")
        _emit_keep_flags(lines, name, g)
    elif g.kind == GeneratorKind.HERWIG:
        # Embedded HERWIG 7 — see src/CAPHerwig/HerwigEventGenerator.hpp.
        # HerwigRunFile must point at a .run file produced beforehand by
        # `Herwig read input.in` (see INSTALL_REPORT_HERWIG.md §6).
        lines.append(f"{name}:HerwigRunFile     = {g.herwig_run_file}")
        lines.append(f"{name}:LHAPDFDataPath    = {g.herwig_lhapdf_data_path}")
        lines.append(f"{name}:HerwigPluginPath  = {g.herwig_plugin_path}")
        _emit_keep_flags(lines, name, g)
    lines.append("")


def _emit_keep_flags(lines: list, name: str, g: 'Generator') -> None:
    """Emit the universal SaveFinalOnly / SaveQuarks / SaveNeutrinos /
    SavePhotons / SaveGaugeBosons / RemovePhotons / KeepStatuses keys.
    Used by every generator that supports them (Pythia, Herwig,
    HepMC3).  The GUI's Particle keep panel sets keep_* and the
    Stage panel sets keep_statuses; both flow in via Generator.*."""
    lines.append(f"{name}:SaveFinalOnly     = {1 if g.keep_final_only   else 0}")
    lines.append(f"{name}:SaveQuarks        = {1 if g.keep_quarks       else 0}")
    lines.append(f"{name}:SaveNeutrinos     = {1 if g.keep_neutrinos    else 0}")
    lines.append(f"{name}:SavePhotons       = {1 if g.keep_photons      else 0}")
    lines.append(f"{name}:SaveGaugeBosons   = {1 if g.keep_gauge_bosons else 0}")
    lines.append(f"{name}:RemovePhotons     = {0 if g.keep_photons      else 1}")
    if g.keep_statuses:
        lines.append(f"{name}:KeepStatuses      = {g.keep_statuses}")


def _emit_analyzer_binning(job: Job, lines: list[str]) -> None:
    """For each analyzer in the job, emit:

      1. The shared ``HistogramsScale`` / ``HistogramsForceRewrite`` flags
         used by older src/ paths.

      2. A complete ``<task>:HISTOGRAM_1:*`` block — USE / CREATE / EXPORT /
         N — and per-instance ``HISTOGRAM_1_<k>:NAME / BASE_NAME / OWNER``
         entries (one histogram per (eventFilter × particleFilter) pair),
         so that ``EventProcessorSingle::initialize()`` actually
         instantiates and calls ``create()`` on the histograms.  Without
         this block the analyzer's ``execute()`` reads
         ``histograms_1()[index]`` from an empty vector and segfaults.

      3. The per-analyzer binning keys that match the keys each analyzer's
         ``Histos`` class actually reads (the names differ across
         analyzers — Global uses per-instance ``HISTOGRAM_1_<k>:n_nbins``,
         Single/Pair use analyzer-level ``HISTOGRAM:n1_nbins`` etc.).
    """
    b = job.binning
    for choice in job.analyses:
        task_name, _, out_file = _ANALYZER_CLASS[choice]

        # Compute how many histograms each analyzer expects.
        # Single-particle analyzers (Global, Single, NuDyn, ...) loop
        #   index = iEventFilter * nParticleFilters() + iParticleFilter
        # so they need nEF * nPF histograms.
        # Pair / Pair3D analyzers loop
        #   index = iEventFilter * nPF^2 + iPF1 * nPF + iPF2
        # so they need nEF * nPF^2 histograms (one per ordered species pair).
        # PAIR/PAIR3D analyzers exclude the catch-all "ALL" filter.
        plist_pf = [f for f in job.particle_filters if f.name != "ALL"] \
                   if choice in (AnalysisChoice.PAIR, AnalysisChoice.PAIR3D) \
                   else job.particle_filters
        plist_ef = job.event_filters

        # Pair-style analyzers (Pair, Pair3D, NuDyn) all index their
        # histogram vector as iEF * nPF^2 + iPF1 * nPF + iPF2, so they need
        # nEF * nPF^2 entries.  Everything else is a single-particle
        # analyzer with iEF * nPF + iPF and only needs nEF * nPF.
        is_pair = choice in (AnalysisChoice.PAIR, AnalysisChoice.PAIR3D, AnalysisChoice.NUDYN)
        n_histos = (len(plist_ef) * len(plist_pf) * len(plist_pf)) if is_pair \
                   else (len(plist_ef) * len(plist_pf))

        lines.append(f"# ----- {task_name} binning --------------------------------")
        lines.append(f"{task_name}:HistogramsScale         = 1")
        lines.append(f"{task_name}:HistogramsForceRewrite  = 1")

        # ManagedObjects<HISTOGRAM_1> block
        lines.append(f"{task_name}:HISTOGRAM_1:USE                  = 1")
        lines.append(f"{task_name}:HISTOGRAM_1:CREATE               = 1")
        lines.append(f"{task_name}:HISTOGRAM_1:IMPORT               = 0")
        lines.append(f"{task_name}:HISTOGRAM_1:EXPORT               = 1")
        lines.append(f"{task_name}:HISTOGRAM_1:EXPORT:PATH          = DEFAULT")
        lines.append(f"{task_name}:HISTOGRAM_1:EXPORT:FILE_NAME     = {out_file}")
        lines.append(f"{task_name}:HISTOGRAM_1:EXPORT:BASE_NAME     = {_short_prefix(task_name)}")
        lines.append(f"{task_name}:HISTOGRAM_1:EXPORT:FORCE_REWRITE = 1")
        lines.append(f"{task_name}:HISTOGRAM_1:N                    = {n_histos}")
        # HISTOGRAM_2 (derived) — leave count at 0; postProcess/calculateDerived
        # is only invoked at end-of-run and we want a clean per-event run first.
        lines.append(f"{task_name}:HISTOGRAM_2:USE                  = 0")
        lines.append(f"{task_name}:HISTOGRAM_2:CREATE               = 0")
        lines.append(f"{task_name}:HISTOGRAM_2:N                    = 0")

        # Per-instance entries.  For Pair/Pair3D/NuDyn we emit one
        # histogram per (event filter, particle filter 1, particle
        # filter 2) triplet because those analyzers index into the
        # vector as iEF*nPF^2 + iPF1*nPF + iPF2.  For everything else,
        # one per (ef, pf).
        def _ordered_keys():
            if is_pair:
                for ef in plist_ef:
                    for pf1 in plist_pf:
                        for pf2 in plist_pf:
                            yield ef, pf1, pf2
            else:
                for ef in plist_ef:
                    for pf in plist_pf:
                        yield ef, pf, None

        k = 0
        for ef, pf1, pf2 in _ordered_keys():
            if pf2 is not None:
                base = f"{_short_prefix(task_name)}_{ef.name}_{pf1.name}_{pf2.name}"
            else:
                base = f"{_short_prefix(task_name)}_{ef.name}_{pf1.name}"
            lines.append(f"{task_name}:HISTOGRAM_1_{k}:NAME              = {base}")
            lines.append(f"{task_name}:HISTOGRAM_1_{k}:BASE_NAME         = {base}")
            lines.append(f"{task_name}:HISTOGRAM_1_{k}:TITLE             = {base}")
            lines.append(f"{task_name}:HISTOGRAM_1_{k}:OWNER             = 1")

            # Global analyzer is the odd one out: GlobalHistos::configure()
            # reads its binning from PER-INSTANCE keys
            # (<task>:HISTOGRAM_1_<k>:n_nbins, etc.). Every other
            # ``Histos`` class we ship reads analyzer-level
            # ``<task>:HISTOGRAM:*`` keys instead — those are emitted
            # below the loop.
            if choice == AnalysisChoice.GLOBAL:
                lines.append(f"{task_name}:HISTOGRAM_1_{k}:n_nbins           = 500")
                lines.append(f"{task_name}:HISTOGRAM_1_{k}:n_min             = 0.0")
                lines.append(f"{task_name}:HISTOGRAM_1_{k}:n_max             = 5000.0")
                lines.append(f"{task_name}:HISTOGRAM_1_{k}:e_nbins           = 500")
                lines.append(f"{task_name}:HISTOGRAM_1_{k}:e_min             = 0.0")
                lines.append(f"{task_name}:HISTOGRAM_1_{k}:e_max             = 10000.0")
                lines.append(f"{task_name}:HISTOGRAM_1_{k}:q_nbins           = 200")
                lines.append(f"{task_name}:HISTOGRAM_1_{k}:q_min             = -100.0")
                lines.append(f"{task_name}:HISTOGRAM_1_{k}:q_max             = 100.0")
                lines.append(f"{task_name}:HISTOGRAM_1_{k}:s_nbins           = 200")
                lines.append(f"{task_name}:HISTOGRAM_1_{k}:s_min             = -100.0")
                lines.append(f"{task_name}:HISTOGRAM_1_{k}:s_max             = 100.0")
                # NOTE: GlobalHistos.cpp:168 reads "b_Bins" (capital B) — a
                # typo in the source we work around by emitting both keys.
                lines.append(f"{task_name}:HISTOGRAM_1_{k}:b_nbins           = 200")
                lines.append(f"{task_name}:HISTOGRAM_1_{k}:b_Bins            = 200")
                lines.append(f"{task_name}:HISTOGRAM_1_{k}:b_min             = -100.0")
                lines.append(f"{task_name}:HISTOGRAM_1_{k}:b_max             = 100.0")
                lines.append(f"{task_name}:HISTOGRAM_1_{k}:ptSum_nbins       = 100")
                lines.append(f"{task_name}:HISTOGRAM_1_{k}:ptSum_min         = 0.0")
                lines.append(f"{task_name}:HISTOGRAM_1_{k}:ptSum_max         = 10000.0")
                lines.append(f"{task_name}:HISTOGRAM_1_{k}:ptAvg_nbins       = 100")
                lines.append(f"{task_name}:HISTOGRAM_1_{k}:ptAvg_min         = 0.0")
                lines.append(f"{task_name}:HISTOGRAM_1_{k}:ptAvg_max         = 10.0")
            k += 1

        # Analyzer-level binning keys, in the names each analyzer's
        # ``Histos`` class actually reads.  All non-Global analyzers in
        # CAP take their binning from <task>:HISTOGRAM:<key>.
        if choice == AnalysisChoice.SINGLE:
            lines.append(f"{task_name}:HISTOGRAM:eta_fill             = 1")
            lines.append(f"{task_name}:HISTOGRAM:rapidity_fill        = 0")
            lines.append(f"{task_name}:HISTOGRAM:p2_fill              = 0")
            lines.append(f"{task_name}:HISTOGRAM:FillPid              = 0")
            lines.append(f"{task_name}:HISTOGRAM:FillPtvsY            = 0")
            lines.append(f"{task_name}:HISTOGRAM:n1_nbins             = {b.n.nbins}")
            lines.append(f"{task_name}:HISTOGRAM:n1_min               = {b.n.min}")
            lines.append(f"{task_name}:HISTOGRAM:n1_max               = {b.n.max}")
            lines.append(f"{task_name}:HISTOGRAM:pt_nbins             = {b.pt.nbins}")
            lines.append(f"{task_name}:HISTOGRAM:pt_min               = {b.pt.min}")
            lines.append(f"{task_name}:HISTOGRAM:pt_max               = {b.pt.max}")
            lines.append(f"{task_name}:HISTOGRAM:phi_nbins            = {b.phi.nbins}")
            lines.append(f"{task_name}:HISTOGRAM:phi_min              = {b.phi.min}")
            lines.append(f"{task_name}:HISTOGRAM:phi_max              = {b.phi.max}")
            lines.append(f"{task_name}:HISTOGRAM:eta_nbins            = {b.eta.nbins}")
            lines.append(f"{task_name}:HISTOGRAM:eta_min              = {b.eta.min}")
            lines.append(f"{task_name}:HISTOGRAM:eta_max              = {b.eta.max}")
            lines.append(f"{task_name}:HISTOGRAM:rapidity_nbins       = {b.y.nbins}")
            lines.append(f"{task_name}:HISTOGRAM:rapidity_min         = {b.y.min}")
            lines.append(f"{task_name}:HISTOGRAM:rapidity_max         = {b.y.max}")
        elif choice in (AnalysisChoice.PAIR, AnalysisChoice.PAIR3D):
            # ParticlePairHistos::configure reads n2_* (pair multiplicity
            # binning) and three boolean fill toggles in addition to the
            # standard pt/eta/y/phi binning.  All of them have to be
            # emitted or createNewHistogram throws n_x<1 on the n2 histo.
            lines.append(f"{task_name}:HISTOGRAM:eta_fill             = 1")
            lines.append(f"{task_name}:HISTOGRAM:rapidity_fill        = 0")
            lines.append(f"{task_name}:HISTOGRAM:p2_fill              = 0")
            lines.append(f"{task_name}:HISTOGRAM:n2_nbins             = {b.n.nbins}")
            lines.append(f"{task_name}:HISTOGRAM:n2_min               = {b.n.min}")
            lines.append(f"{task_name}:HISTOGRAM:n2_max               = {b.n.max}")
            lines.append(f"{task_name}:HISTOGRAM:pt_nbins             = {b.pt.nbins}")
            lines.append(f"{task_name}:HISTOGRAM:pt_min               = {b.pt.min}")
            lines.append(f"{task_name}:HISTOGRAM:pt_max               = {b.pt.max}")
            lines.append(f"{task_name}:HISTOGRAM:phi_nbins            = {b.phi.nbins}")
            lines.append(f"{task_name}:HISTOGRAM:phi_min              = {b.phi.min}")
            lines.append(f"{task_name}:HISTOGRAM:phi_max              = {b.phi.max}")
            lines.append(f"{task_name}:HISTOGRAM:eta_nbins            = {b.eta.nbins}")
            lines.append(f"{task_name}:HISTOGRAM:eta_min              = {b.eta.min}")
            lines.append(f"{task_name}:HISTOGRAM:eta_max              = {b.eta.max}")
            lines.append(f"{task_name}:HISTOGRAM:rapidity_nbins       = {b.y.nbins}")
            lines.append(f"{task_name}:HISTOGRAM:rapidity_min         = {b.y.min}")
            lines.append(f"{task_name}:HISTOGRAM:rapidity_max         = {b.y.max}")
            if choice == AnalysisChoice.PAIR3D:
                lines.append(f"{task_name}:HISTOGRAM:Qinv_nbins           = {b.qinv.nbins}")
                lines.append(f"{task_name}:HISTOGRAM:Qinv_min             = {b.qinv.min}")
                lines.append(f"{task_name}:HISTOGRAM:Qinv_max             = {b.qinv.max}")
                lines.append(f"{task_name}:HISTOGRAM:DeltaPs_nbins        = {b.delta_ps.nbins}")
                lines.append(f"{task_name}:HISTOGRAM:DeltaPs_min          = {b.delta_ps.min}")
                lines.append(f"{task_name}:HISTOGRAM:DeltaPs_max          = {b.delta_ps.max}")
                lines.append(f"{task_name}:HISTOGRAM:DeltaPo_nbins        = {b.delta_po.nbins}")
                lines.append(f"{task_name}:HISTOGRAM:DeltaPo_min          = {b.delta_po.min}")
                lines.append(f"{task_name}:HISTOGRAM:DeltaPo_max          = {b.delta_po.max}")
                lines.append(f"{task_name}:HISTOGRAM:DeltaPl_nbins        = {b.delta_pl.nbins}")
                lines.append(f"{task_name}:HISTOGRAM:DeltaPl_min          = {b.delta_pl.min}")
                lines.append(f"{task_name}:HISTOGRAM:DeltaPl_max          = {b.delta_pl.max}")
        elif choice == AnalysisChoice.SPHEROCITY:
            # SpherocityHistos::configure reads three Fill flags (capital F)
            # gating which spherocity histograms get created.  Match the
            # eta/y/pt binning that other analyzers use for consistency.
            lines.append(f"{task_name}:HISTOGRAM:FillS0                = 1")
            lines.append(f"{task_name}:HISTOGRAM:FillS1                = 0")
            lines.append(f"{task_name}:HISTOGRAM:FillS1VsS0            = 0")
            lines.append(f"{task_name}:HISTOGRAM:spherocity_nbins      = 100")
            lines.append(f"{task_name}:HISTOGRAM:spherocity_min        = 0.0")
            lines.append(f"{task_name}:HISTOGRAM:spherocity_max        = 1.0")
        elif choice == AnalysisChoice.NUDYN:
            # NuDynHistos::configure reads only event-class binning;
            # everything else is per-pair multiplicity moments.
            lines.append(f"{task_name}:HISTOGRAM:evt_nbins             = {b.n.nbins}")
            lines.append(f"{task_name}:HISTOGRAM:evt_min               = {b.n.min}")
            lines.append(f"{task_name}:HISTOGRAM:evt_max               = {b.n.max}")
        elif choice == AnalysisChoice.PTPT:
            # PtPtHistos::configure reads the same evt_* event-class
            # binning as NuDyn; create() builds h_evt/h_f1..h_f4 and a
            # series of Q1..Q4 profiles, all using evt_nbins/min/max.
            lines.append(f"{task_name}:HISTOGRAM:evt_nbins             = {b.n.nbins}")
            lines.append(f"{task_name}:HISTOGRAM:evt_min               = {b.n.min}")
            lines.append(f"{task_name}:HISTOGRAM:evt_max               = {b.n.max}")
        # Legacy aliases — older Analyzer code paths still read these.
        lines.append(f"{task_name}:nBins_n1                = {b.n.nbins}")
        lines.append(f"{task_name}:Min_n1                  = {b.n.min}")
        lines.append(f"{task_name}:Max_n1                  = {b.n.max}")
        lines.append(f"{task_name}:nBins_pt                = {b.pt.nbins}")
        lines.append(f"{task_name}:Min_pt                  = {b.pt.min}")
        lines.append(f"{task_name}:Max_pt                  = {b.pt.max}")
        lines.append(f"{task_name}:nBins_eta               = {b.eta.nbins}")
        lines.append(f"{task_name}:Min_eta                 = {b.eta.min}")
        lines.append(f"{task_name}:Max_eta                 = {b.eta.max}")
        lines.append(f"{task_name}:nBins_y                 = {b.y.nbins}")
        lines.append(f"{task_name}:Min_y                   = {b.y.min}")
        lines.append(f"{task_name}:Max_y                   = {b.y.max}")
        lines.append(f"{task_name}:nBins_phi               = {b.phi.nbins}")
        if choice == AnalysisChoice.PAIR3D:
            lines.append(f"{task_name}:nBins_Qinv              = {b.qinv.nbins}")
            lines.append(f"{task_name}:Min_Qinv                = {b.qinv.min}")
            lines.append(f"{task_name}:Max_Qinv                = {b.qinv.max}")
            lines.append(f"{task_name}:nBins_DeltaPs           = {b.delta_ps.nbins}")
            lines.append(f"{task_name}:Min_DeltaPs             = {b.delta_ps.min}")
            lines.append(f"{task_name}:Max_DeltaPs             = {b.delta_ps.max}")
            lines.append(f"{task_name}:nBins_DeltaPo           = {b.delta_po.nbins}")
            lines.append(f"{task_name}:Min_DeltaPo             = {b.delta_po.min}")
            lines.append(f"{task_name}:Max_DeltaPo             = {b.delta_po.max}")
            lines.append(f"{task_name}:nBins_DeltaPl           = {b.delta_pl.nbins}")
            lines.append(f"{task_name}:Min_DeltaPl             = {b.delta_pl.min}")
            lines.append(f"{task_name}:Max_DeltaPl             = {b.delta_pl.max}")
        lines.append("")


# ---------------------------------------------------------------------------
#  Public API
# ---------------------------------------------------------------------------

_DERIVED_OF = {
    AnalysisChoice.GLOBAL:     ("GlobalCalculator",         "CAP::GlobalCalculator",
                                "GlobalGen.root",            "GlobalDerivedGen.root"),
    AnalysisChoice.SINGLE:     ("ParticleSingleCalculator", "CAP::ParticleSingleCalculator",
                                "SingleGen.root",            "SingleDerivedGen.root"),
    AnalysisChoice.PAIR:       ("ParticlePairCalculator",   "CAP::ParticlePairCalculator",
                                "PairGen.root",              "PairDerivedGen.root"),
    AnalysisChoice.PAIR3D:     ("ParticlePair3DCalculator", "CAP::ParticlePair3DCalculator",
                                "Pair3DGen.root",            "Pair3DDerivedGen.root"),
}

# Order RunDerived's Calculator subtasks: Single must come BEFORE Pair / Pair3D
# because the Pair Calculator borrows ParticleSingleDerivedHistos from the
# static ManagedObjects pool that the Single Calculator owns.
_CALCULATOR_ORDER = [
    AnalysisChoice.GLOBAL,
    AnalysisChoice.SINGLE,
    AnalysisChoice.PAIR,
    AnalysisChoice.PAIR3D,
]


def _emit_calculator_binning(job: Job, lines: list[str]) -> None:
    """For each Calculator (Single / Pair / Pair3D / Global) emit the
    HISTOGRAM_* config blocks needed to:
      - HISTOGRAM_1: import the existing base histos (n1, n2, …) from
                     the stage-1 *Gen.root file
      - HISTOGRAM_2: create + export the derived histos (R2, C2, …) to
                     the stage-2 *DerivedGen.root file
      - HISTOGRAM_3 (Pair only): skipped (BF is stage 3)
      - HISTOGRAM_4 (Pair only): borrow the ParticleSingleDerivedHistos
                                 owned by the Single Calculator running
                                 earlier in the same RunDerived block
                                 (static-pool sharing keyed by name).

    Plus the analyzer-level HISTOGRAM:* binning keys mirrored under the
    Calculator's task name (Histos classes' configure() reads from
    <task>:HISTOGRAM:* using this->name())."""

    derived_choices = [c for c in _CALCULATOR_ORDER if c in job.analyses
                       and c in _DERIVED_OF]
    if not derived_choices:
        return

    b = job.binning

    for choice in derived_choices:
        calc_task, _, in_file, out_file = _DERIVED_OF[choice]
        ana_task, _, _ana_out          = _ANALYZER_CLASS[choice]
        ana_short                       = _short_prefix(ana_task)
        derived_short                   = f"{ana_short}Derived"

        # Filter lists — pair-style analyzers exclude the catch-all "ALL"
        # particle filter so the index space matches stage 1 exactly.
        is_pair = choice in (AnalysisChoice.PAIR, AnalysisChoice.PAIR3D)
        plist_pf = [f for f in job.particle_filters if f.name != "ALL"] \
                   if is_pair \
                   else job.particle_filters
        plist_ef = job.event_filters

        # Histogram counts.  Pair-style: nEF * nPF^2.  Single-style: nEF * nPF.
        if is_pair:
            n_hist1 = len(plist_ef) * len(plist_pf) * len(plist_pf)
        else:
            n_hist1 = len(plist_ef) * len(plist_pf)
        n_hist2 = n_hist1
        n_hist4 = len(plist_ef) * len(plist_pf)   # Pair only — single-style index

        lines.append(f"# ----- {calc_task} binning ----------------------------------")
        lines.append(f"{calc_task}:HistogramsScale         = 1")
        lines.append(f"{calc_task}:HistogramsForceRewrite  = 1")

        # HISTOGRAM_1: imports the n1/n2 written by stage-1 Analyzer.
        lines.append(f"{calc_task}:HISTOGRAM_1:USE                  = 1")
        lines.append(f"{calc_task}:HISTOGRAM_1:CREATE               = 0")
        lines.append(f"{calc_task}:HISTOGRAM_1:IMPORT               = 1")
        lines.append(f"{calc_task}:HISTOGRAM_1:IMPORT:PATH          = DEFAULT")
        lines.append(f"{calc_task}:HISTOGRAM_1:IMPORT:FILE_NAME     = {in_file}")
        lines.append(f"{calc_task}:HISTOGRAM_1:IMPORT:BASE_NAME     = {ana_short}")
        lines.append(f"{calc_task}:HISTOGRAM_1:EXPORT               = 0")
        lines.append(f"{calc_task}:HISTOGRAM_1:N                    = {n_hist1}")

        # HISTOGRAM_2: creates the R2/C2 derived histos and writes them out.
        lines.append(f"{calc_task}:HISTOGRAM_2:USE                  = 1")
        lines.append(f"{calc_task}:HISTOGRAM_2:CREATE               = 1")
        lines.append(f"{calc_task}:HISTOGRAM_2:IMPORT               = 0")
        lines.append(f"{calc_task}:HISTOGRAM_2:EXPORT               = 1")
        lines.append(f"{calc_task}:HISTOGRAM_2:EXPORT:PATH          = DEFAULT")
        lines.append(f"{calc_task}:HISTOGRAM_2:EXPORT:FILE_NAME     = {out_file}")
        lines.append(f"{calc_task}:HISTOGRAM_2:EXPORT:BASE_NAME     = {derived_short}")
        lines.append(f"{calc_task}:HISTOGRAM_2:EXPORT:FORCE_REWRITE = 1")
        lines.append(f"{calc_task}:HISTOGRAM_2:N                    = {n_hist2}")

        if is_pair:
            # HISTOGRAM_3 (Pair BF holders) — skipped here; BF is stage 3.
            lines.append(f"{calc_task}:HISTOGRAM_3:USE                  = 0")
            lines.append(f"{calc_task}:HISTOGRAM_3:CREATE               = 0")
            lines.append(f"{calc_task}:HISTOGRAM_3:N                    = 0")
            # HISTOGRAM_4 — borrow ParticleSingleDerivedHistos from the
            # Single Calculator's HISTOGRAM_2 via the static
            # ManagedObjects pool.  IMPORT=0 because IMPORT happens at
            # initialize-time, BEFORE Single Calc has had a chance to
            # write SingleDerivedGen.root → file-not-found.
            #
            # The static-pool sharing relies on Single Calculator running
            # initialize() FIRST (it does — Subtask4 vs Pair = Subtask5
            # in the RunDerived task tree), at which point Single Calc
            # creates the named histos with OWNER=1 and registers them
            # in the pool.  Pair Calc then borrows by name (OWNER=0).
            lines.append(f"{calc_task}:HISTOGRAM_4:USE                  = 1")
            lines.append(f"{calc_task}:HISTOGRAM_4:CREATE               = 0")
            lines.append(f"{calc_task}:HISTOGRAM_4:IMPORT               = 0")
            lines.append(f"{calc_task}:HISTOGRAM_4:EXPORT               = 0")
            lines.append(f"{calc_task}:HISTOGRAM_4:N                    = {n_hist4}")

        # ---- Per-instance HISTOGRAM_1 entries ---------------------------
        # NAMEs match exactly what the stage-1 Analyzer's binning block
        # wrote (so load() finds the right TKeys in the .root file).
        def _ordered_pair_keys():
            for ef in plist_ef:
                for pf1 in plist_pf:
                    for pf2 in plist_pf:
                        yield ef, pf1, pf2

        def _ordered_single_keys(filters_pf):
            for ef in plist_ef:
                for pf in filters_pf:
                    yield ef, pf

        k = 0
        if is_pair:
            for ef, pf1, pf2 in _ordered_pair_keys():
                base = f"{ana_short}_{ef.name}_{pf1.name}_{pf2.name}"
                lines.append(f"{calc_task}:HISTOGRAM_1_{k}:NAME      = {base}")
                lines.append(f"{calc_task}:HISTOGRAM_1_{k}:BASE_NAME = {base}")
                lines.append(f"{calc_task}:HISTOGRAM_1_{k}:TITLE     = {base}")
                lines.append(f"{calc_task}:HISTOGRAM_1_{k}:OWNER     = 1")
                k += 1
        else:
            for ef, pf in _ordered_single_keys(plist_pf):
                base = f"{ana_short}_{ef.name}_{pf.name}"
                lines.append(f"{calc_task}:HISTOGRAM_1_{k}:NAME      = {base}")
                lines.append(f"{calc_task}:HISTOGRAM_1_{k}:BASE_NAME = {base}")
                lines.append(f"{calc_task}:HISTOGRAM_1_{k}:TITLE     = {base}")
                lines.append(f"{calc_task}:HISTOGRAM_1_{k}:OWNER     = 1")
                # Global is the odd one out — its Histos' configure() reads
                # binning from PER-INSTANCE keys.  Mirror what the analyzer
                # binning emitter does for stage 1.
                if choice == AnalysisChoice.GLOBAL:
                    for key, val in [
                        ("n_nbins", 500),  ("n_min", 0.0),     ("n_max", 5000.0),
                        ("e_nbins", 500),  ("e_min", 0.0),     ("e_max", 10000.0),
                        ("q_nbins", 200),  ("q_min", -100.0),  ("q_max", 100.0),
                        ("s_nbins", 200),  ("s_min", -100.0),  ("s_max", 100.0),
                        ("b_nbins", 200),  ("b_Bins", 200),
                        ("b_min", -100.0), ("b_max", 100.0),
                        ("ptSum_nbins", 100), ("ptSum_min", 0.0), ("ptSum_max", 10000.0),
                        ("ptAvg_nbins", 100), ("ptAvg_min", 0.0), ("ptAvg_max", 10.0),
                    ]:
                        lines.append(f"{calc_task}:HISTOGRAM_1_{k}:{key} = {val}")
                k += 1

        # ---- Per-instance HISTOGRAM_2 entries ---------------------------
        # NAMEs use the "Derived" prefix so they're distinct from
        # HISTOGRAM_1 (the same TFile may receive both).  OWNER=1 puts
        # them in the static pool for downstream tasks (Pair Calculator)
        # to borrow by name.
        k = 0
        if is_pair:
            for ef, pf1, pf2 in _ordered_pair_keys():
                base = f"{derived_short}_{ef.name}_{pf1.name}_{pf2.name}"
                lines.append(f"{calc_task}:HISTOGRAM_2_{k}:NAME      = {base}")
                lines.append(f"{calc_task}:HISTOGRAM_2_{k}:BASE_NAME = {base}")
                lines.append(f"{calc_task}:HISTOGRAM_2_{k}:TITLE     = {base}")
                lines.append(f"{calc_task}:HISTOGRAM_2_{k}:OWNER     = 1")
                k += 1
        else:
            for ef, pf in _ordered_single_keys(plist_pf):
                base = f"{derived_short}_{ef.name}_{pf.name}"
                lines.append(f"{calc_task}:HISTOGRAM_2_{k}:NAME      = {base}")
                lines.append(f"{calc_task}:HISTOGRAM_2_{k}:BASE_NAME = {base}")
                lines.append(f"{calc_task}:HISTOGRAM_2_{k}:TITLE     = {base}")
                lines.append(f"{calc_task}:HISTOGRAM_2_{k}:OWNER     = 1")
                # Global per-instance binning (mirrors HISTOGRAM_1 above).
                if choice == AnalysisChoice.GLOBAL:
                    for key, val in [
                        ("n_nbins", 500),  ("n_min", 0.0),     ("n_max", 5000.0),
                        ("e_nbins", 500),  ("e_min", 0.0),     ("e_max", 10000.0),
                        ("q_nbins", 200),  ("q_min", -100.0),  ("q_max", 100.0),
                        ("s_nbins", 200),  ("s_min", -100.0),  ("s_max", 100.0),
                        ("b_nbins", 200),  ("b_Bins", 200),
                        ("b_min", -100.0), ("b_max", 100.0),
                        ("ptSum_nbins", 100), ("ptSum_min", 0.0), ("ptSum_max", 10000.0),
                        ("ptAvg_nbins", 100), ("ptAvg_min", 0.0), ("ptAvg_max", 10.0),
                    ]:
                        lines.append(f"{calc_task}:HISTOGRAM_2_{k}:{key} = {val}")
                k += 1

        # ---- Per-instance HISTOGRAM_4 entries (Pair Calculators only) ---
        # NAMEs match the Single Calculator's HISTOGRAM_2 NAMEs at the
        # corresponding (event filter × particle filter) cell — but Pair
        # Calculator excludes the "ALL" particle filter, so we filter
        # against the same plist_pf used for HISTOGRAM_1/2 above.
        if is_pair:
            single_derived_short = f"{_short_prefix('ParticleSingleAnalyzer')}Derived"  # = "PSDerived"
            k = 0
            for ef, pf in _ordered_single_keys(plist_pf):
                base = f"{single_derived_short}_{ef.name}_{pf.name}"
                lines.append(f"{calc_task}:HISTOGRAM_4_{k}:NAME      = {base}")
                lines.append(f"{calc_task}:HISTOGRAM_4_{k}:BASE_NAME = {base}")
                lines.append(f"{calc_task}:HISTOGRAM_4_{k}:TITLE     = {base}")
                # OWNER=0: don't try to register this name in the
                # static ManagedObjects pool — the SingleCalculator
                # already registered the same name with OWNER=1.  We
                # only need to LOAD it (via IMPORT=1) for our own use
                # in calculateDerivedHistograms.
                lines.append(f"{calc_task}:HISTOGRAM_4_{k}:OWNER     = 0")
                k += 1

        # ---- Analyzer-level HISTOGRAM:* binning keys --------------------
        # Mirrored under the Calculator's task name so the Histos classes'
        # configure() (which reads via this->name()) sees the same binning
        # the Analyzer used in stage 1.  Anything we don't mirror would
        # default to zero and the create() in HISTOGRAM_2 would throw.
        if choice == AnalysisChoice.SINGLE:
            for key, val in [
                ("eta_fill", 1), ("rapidity_fill", 0), ("p2_fill", 0),
                ("FillPid", 0), ("FillPtvsY", 0),
                ("n1_nbins", b.n.nbins), ("n1_min", b.n.min), ("n1_max", b.n.max),
                ("pt_nbins", b.pt.nbins), ("pt_min", b.pt.min), ("pt_max", b.pt.max),
                ("phi_nbins", b.phi.nbins), ("phi_min", b.phi.min), ("phi_max", b.phi.max),
                ("eta_nbins", b.eta.nbins), ("eta_min", b.eta.min), ("eta_max", b.eta.max),
                ("rapidity_nbins", b.y.nbins), ("rapidity_min", b.y.min), ("rapidity_max", b.y.max),
            ]:
                lines.append(f"{calc_task}:HISTOGRAM:{key:<22} = {val}")
        elif choice in (AnalysisChoice.PAIR, AnalysisChoice.PAIR3D):
            for key, val in [
                ("eta_fill", 1), ("rapidity_fill", 0), ("p2_fill", 0),
                ("n2_nbins", b.n.nbins), ("n2_min", b.n.min), ("n2_max", b.n.max),
                ("pt_nbins", b.pt.nbins), ("pt_min", b.pt.min), ("pt_max", b.pt.max),
                ("phi_nbins", b.phi.nbins), ("phi_min", b.phi.min), ("phi_max", b.phi.max),
                ("eta_nbins", b.eta.nbins), ("eta_min", b.eta.min), ("eta_max", b.eta.max),
                ("rapidity_nbins", b.y.nbins), ("rapidity_min", b.y.min), ("rapidity_max", b.y.max),
            ]:
                lines.append(f"{calc_task}:HISTOGRAM:{key:<22} = {val}")
            if choice == AnalysisChoice.PAIR3D:
                for key, val in [
                    ("Qinv_nbins", b.qinv.nbins), ("Qinv_min", b.qinv.min), ("Qinv_max", b.qinv.max),
                    ("DeltaPs_nbins", b.delta_ps.nbins),
                    ("DeltaPs_min", b.delta_ps.min), ("DeltaPs_max", b.delta_ps.max),
                    ("DeltaPo_nbins", b.delta_po.nbins),
                    ("DeltaPo_min", b.delta_po.min), ("DeltaPo_max", b.delta_po.max),
                    ("DeltaPl_nbins", b.delta_pl.nbins),
                    ("DeltaPl_min", b.delta_pl.min), ("DeltaPl_max", b.delta_pl.max),
                ]:
                    lines.append(f"{calc_task}:HISTOGRAM:{key:<22} = {val}")

        # Legacy aliases (older code paths still read these).
        lines.append(f"{calc_task}:nBins_n1                = {b.n.nbins}")
        lines.append(f"{calc_task}:Min_n1                  = {b.n.min}")
        lines.append(f"{calc_task}:Max_n1                  = {b.n.max}")
        lines.append(f"{calc_task}:nBins_pt                = {b.pt.nbins}")
        lines.append(f"{calc_task}:Min_pt                  = {b.pt.min}")
        lines.append(f"{calc_task}:Max_pt                  = {b.pt.max}")
        lines.append(f"{calc_task}:nBins_eta               = {b.eta.nbins}")
        lines.append(f"{calc_task}:Min_eta                 = {b.eta.min}")
        lines.append(f"{calc_task}:Max_eta                 = {b.eta.max}")
        lines.append(f"{calc_task}:nBins_y                 = {b.y.nbins}")
        lines.append(f"{calc_task}:Min_y                   = {b.y.min}")
        lines.append(f"{calc_task}:Max_y                   = {b.y.max}")
        lines.append(f"{calc_task}:nBins_phi               = {b.phi.nbins}")
        lines.append("")


def _emit_run_derived(job: Job, lines: list[str]) -> None:
    """Emit the RunDerived top-level task + a Calculator subtask for
    every selected analysis that has a derived counterpart.  Mirrors
    the layout used in projects/Pythia/pp_13.7TeV/RA.ini.

    Subtask ordering matters: Single Calculator runs BEFORE Pair /
    Pair3D Calculator so that ParticleSingleDerivedHistos owned by
    Single can be borrowed by Pair via the static ManagedObjects pool."""
    # Order so dependencies (Single first) come earlier.
    derived_choices = [c for c in _CALCULATOR_ORDER if c in job.analyses
                       and c in _DERIVED_OF]
    if not derived_choices:
        return                  # nothing to derive

    lines.append("# ============================================================")
    lines.append("# RunDerived — second-pass post-processing of the *Gen.root")
    lines.append("# files produced by RunAnalysis.  Invoked by:")
    lines.append("#   bin/CAP RunDerived <project> <ini> <outdir>")
    lines.append("# ============================================================")
    lines.append("RunDerived:TaskName               = RunDerived")
    lines.append("RunDerived:TaskClassName          = CAP::RunAnalysis")
    lines.append("RunDerived:Severity               = Info")
    lines.append(f"RunDerived:nSubtasks              = {3 + len(derived_choices)}")
    lines.append("RunDerived:Subtask0:TaskName       = ParticleTypeTask")
    lines.append("RunDerived:Subtask0:TaskClassName  = CAP::ParticleTypeTask")
    lines.append("RunDerived:Subtask0:nSubtasks      = 0")
    lines.append("RunDerived:Subtask0:nParticleDbs   = 1")
    lines.append("RunDerived:Subtask0:ParticleDbName0= DefaultDb")
    lines.append("RunDerived:Subtask0:ParticleDbOwner0=1")
    lines.append("RunDerived:Subtask1:TaskName       = EventFilterCreator")
    lines.append("RunDerived:Subtask1:TaskClassName  = CAP::EventFilterCreator")
    lines.append("RunDerived:Subtask1:nSubtasks      = 0")
    lines.append(f"RunDerived:Subtask1:nEventFilters  = {len(job.event_filters)}")
    for k, ef in enumerate(job.event_filters):
        lines.append(f"RunDerived:Subtask1:EventFilterName{k}  = {ef.name}")
        lines.append(f"RunDerived:Subtask1:EventFilterOwner{k} = 1")
    lines.append("RunDerived:Subtask2:TaskName       = ParticleFilterCreator")
    lines.append("RunDerived:Subtask2:TaskClassName  = CAP::ParticleFilterCreator")
    lines.append("RunDerived:Subtask2:nSubtasks      = 0")
    lines.append(f"RunDerived:Subtask2:nParticleFilters= {len(job.particle_filters)}")
    for k, pf in enumerate(job.particle_filters):
        lines.append(f"RunDerived:Subtask2:ParticleFilterName{k}  = {pf.name}")
        lines.append(f"RunDerived:Subtask2:ParticleFilterOwner{k} = 1")

    for i, choice in enumerate(derived_choices, start=3):
        task, cls, in_file, out_file = _DERIVED_OF[choice]
        lines.append(f"RunDerived:Subtask{i}:TaskName       = {task}")
        lines.append(f"RunDerived:Subtask{i}:TaskClassName  = {cls}")
        lines.append(f"RunDerived:Subtask{i}:nSubtasks      = 0")
        lines.append(f"RunDerived:Subtask{i}:HistogramsImportPath = Default")
        lines.append(f"RunDerived:Subtask{i}:HistogramsImportFile = {in_file}")
        lines.append(f"RunDerived:Subtask{i}:HistogramsExportPath = Default")
        lines.append(f"RunDerived:Subtask{i}:HistogramsExportFile = {out_file}")
        lines.append(f"RunDerived:Subtask{i}:HistogramsForceRewrite = 1")
        # CRITICAL: each Calculator must borrow the same filters as the
        # corresponding Analyzer used in stage 1.  Without these,
        # nEventFilters defaults to 0 and EventProcessor::calculateDerived
        # has an empty loop — every derived histogram stays at zero
        # entries.  Pair-style Calculators exclude the "ALL" particle
        # filter to match stage-1 indexing.
        lines.append(f"RunDerived:Subtask{i}:nEventFilters    = {len(job.event_filters)}")
        for k, ef in enumerate(job.event_filters):
            lines.append(f"RunDerived:Subtask{i}:EventFilterName{k}     = {ef.name}")
            lines.append(f"RunDerived:Subtask{i}:EventFilterOwner{k}    = 0")
        plist_pf = [f for f in job.particle_filters if f.name != "ALL"] \
                   if choice in (AnalysisChoice.PAIR, AnalysisChoice.PAIR3D) \
                   else job.particle_filters
        lines.append(f"RunDerived:Subtask{i}:nParticleFilters = {len(plist_pf)}")
        for k, pf in enumerate(plist_pf):
            lines.append(f"RunDerived:Subtask{i}:ParticleFilterName{k}  = {pf.name}")
            lines.append(f"RunDerived:Subtask{i}:ParticleFilterOwner{k} = 0")
    lines.append("")


def _emit_run_bf(job: Job, lines: list[str]) -> None:
    """RunBf — Balance Function calculator that reads the pair-derived
    histograms and writes PairBFGen.root.  Only meaningful when Pair
    or Pair3D was part of the analysis."""
    has_pair   = AnalysisChoice.PAIR   in job.analyses
    has_pair3d = AnalysisChoice.PAIR3D in job.analyses
    if not (has_pair or has_pair3d):
        return

    # 1D-Pair BF AND 3D-Pair BF — both wired now.  The 1D stub lives in
    # analyses/stubs/StubParticlePairBfCalculator.hpp under a renamed
    # filename so it doesn't collide with the broken shipped header.
    sub_calculators = []
    if has_pair:
        sub_calculators.append(("ParticlePairBfCalculator",
                                "CAP::ParticlePairBfCalculator",
                                "PairDerivedGen.root", "PairBFGen.root"))
    if has_pair3d:
        sub_calculators.append(("ParticlePair3DBfCalculator",
                                "CAP::ParticlePair3DBfCalculator",
                                "Pair3DDerivedGen.root", "Pair3DBFGen.root"))

    lines.append("# ============================================================")
    lines.append("# RunBf — Balance Function calculator.  Invoked by:")
    lines.append("#   bin/CAP RunBf <project> <ini> <outdir>")
    lines.append("# Reads the DerivedGen.root produced by RunDerived.")
    lines.append("# ============================================================")
    lines.append("RunBf:TaskName                    = RunBf")
    lines.append("RunBf:TaskClassName               = CAP::RunAnalysis")
    lines.append("RunBf:Severity                    = Info")
    lines.append(f"RunBf:nSubtasks                   = {3 + len(sub_calculators)}")
    lines.append("RunBf:Subtask0:TaskName            = ParticleTypeTask")
    lines.append("RunBf:Subtask0:TaskClassName       = CAP::ParticleTypeTask")
    lines.append("RunBf:Subtask0:nSubtasks           = 0")
    lines.append("RunBf:Subtask0:nParticleDbs        = 1")
    lines.append("RunBf:Subtask0:ParticleDbName0     = DefaultDb")
    lines.append("RunBf:Subtask0:ParticleDbOwner0    = 1")
    lines.append("RunBf:Subtask1:TaskName            = EventFilterCreator")
    lines.append("RunBf:Subtask1:TaskClassName       = CAP::EventFilterCreator")
    lines.append("RunBf:Subtask1:nSubtasks           = 0")
    lines.append(f"RunBf:Subtask1:nEventFilters       = {len(job.event_filters)}")
    for k, ef in enumerate(job.event_filters):
        lines.append(f"RunBf:Subtask1:EventFilterName{k}     = {ef.name}")
        lines.append(f"RunBf:Subtask1:EventFilterOwner{k}    = 1")
    lines.append("RunBf:Subtask2:TaskName            = ParticleFilterCreator")
    lines.append("RunBf:Subtask2:TaskClassName       = CAP::ParticleFilterCreator")
    lines.append("RunBf:Subtask2:nSubtasks           = 0")
    lines.append(f"RunBf:Subtask2:nParticleFilters    = {len(job.particle_filters)}")
    for k, pf in enumerate(job.particle_filters):
        lines.append(f"RunBf:Subtask2:ParticleFilterName{k}  = {pf.name}")
        lines.append(f"RunBf:Subtask2:ParticleFilterOwner{k} = 1")

    for i, (task, cls, in_file, out_file) in enumerate(sub_calculators, start=3):
        lines.append(f"RunBf:Subtask{i}:TaskName            = {task}")
        lines.append(f"RunBf:Subtask{i}:TaskClassName       = {cls}")
        lines.append(f"RunBf:Subtask{i}:nSubtasks           = 0")
        lines.append(f"RunBf:Subtask{i}:HistogramsImportPath = Default")
        lines.append(f"RunBf:Subtask{i}:HistogramsImportFile = {in_file}")
        lines.append(f"RunBf:Subtask{i}:HistogramsExportPath = Default")
        lines.append(f"RunBf:Subtask{i}:HistogramsExportFile = {out_file}")
        lines.append(f"RunBf:Subtask{i}:HistogramsForceRewrite = 1")
        # The 1D BF Calculator reads the filter list from its own task
        # config (subtask-key propagation maps RunBf:Subtask<i>:foo →
        # ParticlePairBfCalculator:foo).  Particle filters MUST be in
        # the BF split convention (first half particles, second half
        # antiparticles in matching species order) — see
        # default_particle_filters() in this file and the
        # "Balance-Function antiparticle convention" section of
        # MISSING_CLASSES.md.  Pair-style: drop the catch-all "ALL".
        plist_pf = [f for f in job.particle_filters if f.name != "ALL"]
        lines.append(f"RunBf:Subtask{i}:nEventFilters       = {len(job.event_filters)}")
        for k, ef in enumerate(job.event_filters):
            lines.append(f"RunBf:Subtask{i}:EventFilterName{k}     = {ef.name}")
            lines.append(f"RunBf:Subtask{i}:EventFilterOwner{k}    = 0")
        lines.append(f"RunBf:Subtask{i}:nParticleFilters    = {len(plist_pf)}")
        for k, pf in enumerate(plist_pf):
            lines.append(f"RunBf:Subtask{i}:ParticleFilterName{k}  = {pf.name}")
            lines.append(f"RunBf:Subtask{i}:ParticleFilterOwner{k} = 0")
    lines.append("")


def render_ini(job: Job) -> str:
    """Return the complete .ini text for `job`."""
    if not job.analyses:
        raise ValueError("Job has no analyses selected — pick at least one.")
    if not job.particle_filters:
        raise ValueError("Job has no particle filters — add at least one.")
    if not job.event_filters:
        raise ValueError("Job has no event filters — add at least one.")

    lines: list[str] = []
    lines.append(_HEADER.format(name=job.name,
                                n_events=job.n_events,
                                output_dir=job.output_dir).rstrip())
    lines.append("")

    _emit_top_task(job, lines)
    _emit_particle_db_task(lines)
    _emit_event_filter_creator(job, lines)
    _emit_particle_filter_creator(job, lines)
    _emit_event_iterator(job, lines)

    _emit_eventiterator_params(job, lines)
    _emit_event_filter_creator_params(job, lines)
    _emit_particle_filter_creator_params(job, lines)
    _emit_generator_params(job, lines)
    _emit_analyzer_binning(job, lines)

    # Optional second/third top-level tasks.  RunBf implies RunDerived
    # because the BF calculator consumes derived histograms.
    if job.run_derived or job.run_bf:
        _emit_run_derived(job, lines)
        # The Calculator subclasses driven by RunDerived need their own
        # full HISTOGRAM_* config blocks (mirroring the Analyzer's, but
        # with IMPORT/EXPORT flipped for stage 2 semantics).
        _emit_calculator_binning(job, lines)
    if job.run_bf:
        _emit_run_bf(job, lines)

    return "\n".join(lines) + "\n"


def write_ini(job: Job, path: str | Path) -> Path:
    """Write the .ini for `job` to `path`. Creates parent directory if needed."""
    p = Path(path)
    p.parent.mkdir(parents=True, exist_ok=True)
    p.write_text(render_ini(job), encoding="utf-8")
    return p
