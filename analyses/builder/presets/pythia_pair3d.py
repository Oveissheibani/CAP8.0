"""Pythia 8 pp → 3D pair correlations (HBT-style: Qinv, side / out / long)."""
from cap_ini_builder import (
    Job, Generator, GeneratorKind, AnalysisChoice,
    default_particle_filters, default_event_filters, Binning, Binning1D,
)
from . import Preset


def _build() -> Job:
    return Job(
        name             = "pythia_pp_13TeV_pair3d",
        output_dir       = "pythia_pp_13TeV_pair3d",
        n_events         = 10000,
        n_events_report  = 1000,
        generator        = Generator(
            kind=GeneratorKind.PYTHIA,
            energy=13000.0, idA=2212, idB=2212, seed=98765,
            soft_qcd=True, use_qcd_cr=True,
        ),
        particle_filters = default_particle_filters(),
        event_filters    = default_event_filters(),
        analyses         = [
            AnalysisChoice.GLOBAL,
            AnalysisChoice.SINGLE,
            AnalysisChoice.PAIR,
            AnalysisChoice.PAIR3D,
        ],
        binning          = Binning(
            n=Binning1D(200, 0.0, 200.0),
            pt=Binning1D(80, 0.0, 8.0),
            eta=Binning1D(120, -6.0, 6.0),
            y=Binning1D(120, -6.0, 6.0),
            phi=Binning1D(72, 0.0, 6.28318531),
            qinv=Binning1D(1000, 0.0, 50.0),
            delta_ps=Binning1D(80, -4.0, 4.0),
            delta_po=Binning1D(80, -4.0, 4.0),
            delta_pl=Binning1D(80, -4.0, 4.0),
        ),
    )


PRESET = Preset(
    name="Pythia pp 13 TeV → 3D pair correlations (HBT)",
    description="Adds Qinv / side / out / long pair-momentum binning on top "
                "of the standard single-particle analysis.",
    build=_build,
)
