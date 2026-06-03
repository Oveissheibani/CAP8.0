"""Generator-agnostic mechanism catalogue.

Pythia and Herwig are different generators ("apples and oranges"), but they
share conceptual *mechanisms* — multiparton interactions, colour
reconnection, parton shower.  To compare the SAME mechanism's effect across
both, we map each generic mechanism to the on/off command lines for each
generator that supports toggling it.

Command strings are taken from cap-mechanism-ladder (Pythia) and
analyses/builder/generator_presets.py (Herwig), so they stay consistent with
the rest of CAP.

Support reality:
  - MPI, CR  → both generators have a clean on/off  → usable for apples-to-
               apples cross-generator comparison.
  - ISR, FSR → Pythia only (Herwig's QTilde shower has no clean ISR/FSR off).
  - rope     → Pythia only (Herwig uses cluster hadronization, no string ropes).

No tkinter import — pure data + helpers, unit-testable.
"""
from __future__ import annotations

MECH_ORDER = ["ISR", "FSR", "MPI", "CR", "rope"]

MECHANISMS: dict[str, dict] = {
    "ISR": {
        "label": "Initial-state radiation",
        "pythia": {"on": ["PartonLevel:ISR = on"],
                   "off": ["PartonLevel:ISR = off"]},
    },
    "FSR": {
        "label": "Final-state radiation",
        "pythia": {"on": ["PartonLevel:FSR = on"],
                   "off": ["PartonLevel:FSR = off"]},
    },
    "MPI": {
        "label": "Multiparton interactions",
        "pythia": {"on": ["PartonLevel:MPI = on"],
                   "off": ["PartonLevel:MPI = off"]},
        # Herwig: MPI is on by default; disable it by detaching the MPI handler
        # from the shower (a huge pTmin0 is REJECTED — it's outside the
        # parameter's allowed limits, verified on the local install).
        "herwig": {"on": [],
                   "off": ["set /Herwig/Shower/ShowerHandler:MPIHandler NULL"]},
    },
    "CR": {
        "label": "Colour reconnection",
        "pythia": {"on": ["ColourReconnection:reconnect = on"],
                   "off": ["ColourReconnection:reconnect = off"]},
        "herwig": {"on": ["set /Herwig/Hadronization/ColourReconnector:"
                          "ColourReconnection Yes"],
                   "off": ["set /Herwig/Hadronization/ColourReconnector:"
                           "ColourReconnection No"]},
    },
    "rope": {
        "label": "String ropes",
        "pythia": {"on": ["Ropewalk:RopeHadronization = on",
                          "Ropewalk:doFlavour = on",
                          "PartonVertex:setVertex = on"],
                   "off": ["Ropewalk:RopeHadronization = off"]},
    },
}

GENERATORS = ("pythia", "herwig")


def supports(mech: str, generator: str) -> bool:
    """True if `generator` can toggle `mech` on/off."""
    return generator in MECHANISMS.get(mech, {})


def mechanisms_for(generator: str) -> list[str]:
    """Mechanisms this generator can toggle, in canonical order."""
    return [m for m in MECH_ORDER if supports(m, generator)]


def common_mechanisms() -> list[str]:
    """Mechanisms every generator can toggle — the apples-to-apples set for a
    cross-generator comparison (currently MPI, CR)."""
    return [m for m in MECH_ORDER
            if all(supports(m, g) for g in GENERATORS)]


def availability(mech: str) -> str:
    """Human label of which generators support a mechanism, e.g.
    'Pythia, Herwig' or 'Pythia only'."""
    gens = [g for g in GENERATORS if supports(mech, g)]
    names = {"pythia": "Pythia", "herwig": "Herwig"}
    if set(gens) == set(GENERATORS):
        return ", ".join(names[g] for g in GENERATORS)
    if gens:
        return names[gens[0]] + " only"
    return "(none)"


def mech_lines(generator: str, enabled_mechs, *, only=None) -> list[str]:
    """Config lines that set EVERY supported mechanism explicitly on/off for
    `generator`: a mechanism in `enabled_mechs` is turned on, otherwise off.

    `only` (optional iterable) restricts to that subset of mechanisms — used
    so a comparison varies only the mechanisms under study and leaves the rest
    at the generator's tuned defaults.
    """
    enabled = set(enabled_mechs or [])
    pool = list(only) if only is not None else mechanisms_for(generator)
    out: list[str] = []
    for m in MECH_ORDER:
        if m not in pool or not supports(m, generator):
            continue
        spec = MECHANISMS[m][generator]
        out += spec["on"] if m in enabled else spec["off"]
    return out
