"""Glauber Monte Carlo for Pb+Pb at √s=5.02 TeV."""
from cap_ini_builder import (
    Job, Generator, GeneratorKind, AnalysisChoice,
    EventFilter, ParticleFilter, Binning, Binning1D,
)
from . import Preset


def _build() -> Job:
    return Job(
        name             = "glauber_pbpb_5TeV",
        output_dir       = "glauber_pbpb_5TeV",
        n_events         = 10000,
        n_events_report  = 1000,
        generator        = Generator(
            kind=GeneratorKind.GLAUBER,
            nucleus_A="Pb", nucleus_B="Pb",
            sigma_NN=72.0,           # mb at √s=5.02 TeV
            impact_min=0.0, impact_max=20.0,
        ),
        particle_filters = [
            ParticleFilter("ALL", "ALL"),
        ],
        event_filters    = [
            EventFilter("ALL",        "ALL"),
            EventFilter("Cent00to05", "0-5%",   mult_min=0.0,    mult_max=1e9),  # placeholder; refined post-loop
            EventFilter("Cent05to10", "5-10%",  mult_min=0.0,    mult_max=1e9),
            EventFilter("Cent10to20", "10-20%", mult_min=0.0,    mult_max=1e9),
        ],
        analyses         = [AnalysisChoice.GLOBAL],   # Glauber produces only event-level info
        binning          = Binning(
            n=Binning1D(500, 0.0, 5000.0),  # Npart can run high in Pb+Pb
            pt=Binning1D(100, 0.0, 5.0),
            eta=Binning1D(100, -5.0, 5.0),
            y=Binning1D(100, -5.0, 5.0),
            phi=Binning1D(72, 0.0, 6.28318531),
        ),
    )


PRESET = Preset(
    name="Glauber MC Pb+Pb 5.02 TeV",
    description="Geometric Monte Carlo; produces Npart, Ncoll, eccentricity, "
                "reaction-plane angles. No particles emitted.",
    build=_build,
)
