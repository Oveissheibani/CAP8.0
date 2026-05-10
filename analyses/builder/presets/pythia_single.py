"""Pythia 8 pp inelastic → single-particle distributions."""
from cap_ini_builder import (
    Job, Generator, GeneratorKind, AnalysisChoice,
    default_particle_filters, default_event_filters, Binning,
)
from . import Preset


def _build() -> Job:
    return Job(
        name             = "pythia_pp_13TeV_single",
        output_dir       = "pythia_pp_13TeV_single",
        n_events         = 1000,
        n_events_report  = 100,
        generator        = Generator(
            kind=GeneratorKind.PYTHIA,
            energy=13000.0, idA=2212, idB=2212, seed=12345,
            soft_qcd=True, use_qcd_cr=True,
        ),
        particle_filters = default_particle_filters(),
        event_filters    = default_event_filters(),
        analyses         = [AnalysisChoice.GLOBAL, AnalysisChoice.SINGLE],
        binning          = Binning.default(),
    )


PRESET = Preset(
    name="Pythia pp 13 TeV → Single-particle",
    description="pp inelastic at √s=13 TeV via Pythia 8; fills "
                "multiplicity/pT/η/y/φ histograms for π, K, p (±) and ALL.",
    build=_build,
)
