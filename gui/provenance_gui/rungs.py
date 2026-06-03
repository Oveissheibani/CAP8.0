"""Single source of truth for mechanism-ladder rungs.

The ladder used to be a fixed list (shower / +MPI / +CR / +rope) duplicated
in both the Ladder panel and the pipeline builder.  It is now *derived* from
the mechanisms the user actually enabled in the Pythia configuration panel,
so the ladder and the Pythia tab stay in lock-step — that is the modular
coupling the report's "mechanism ladder" section is meant to show.

No imports from the panels or the pipeline here, so both can import this
module without a cycle.
"""
from __future__ import annotations

from collections import OrderedDict

# Pythia bool-toggle keys (see analyses/builder/generator_presets.py) that map
# onto ladder mechanisms.  Membership is cumulative: each rung adds one more.
_CR_TOGGLES   = ("cr_qcd_mode1", "cr_qcd_mode2")
_ROPE_TOGGLES = ("ropes_on", "shoving_on")


def cumulative_rungs(pythia_panels: dict | None) -> "OrderedDict[str, list[str]]":
    """Build the cumulative ladder rungs from the Pythia panel toggles.

    Returns an ordered {rung_name: [mechanisms]} mapping.  Reading it
    left-to-right isolates each mechanism's effect:

      * base shower  = ISR / FSR, minus any the user switched OFF
        (`no_isr` / `no_fsr`);
      * `+MPI` rung unless `no_mpi` is set;
      * `+CR` rung iff a colour-reconnection mode is enabled;
      * `+rope` rung iff ropes or shoving is enabled.

    With an empty/None config this reproduces the historical default ladder
    (shower / +MPI / +CR / +rope) so existing behaviour is unchanged when the
    user hasn't touched the Pythia tab.
    """
    p = pythia_panels if pythia_panels else {}

    # Empty config → legacy default ladder (all four rungs).
    legacy = not p
    shower = [m for m, off in (("ISR", "no_isr"), ("FSR", "no_fsr"))
              if not p.get(off)]

    rungs: "OrderedDict[str, list[str]]" = OrderedDict()
    rungs["shower"] = shower[:]
    cum = shower[:]

    if legacy or not p.get("no_mpi"):
        cum = cum + ["MPI"]
        rungs["shower+MPI"] = cum[:]

    if legacy or p.get(_CR_TOGGLES[0]) or p.get(_CR_TOGGLES[1]):
        cum = cum + ["CR"]
        rungs["shower+MPI+CR" if "MPI" in cum else "shower+CR"] = cum[:]

    if legacy or p.get(_ROPE_TOGGLES[0]) or p.get(_ROPE_TOGGLES[1]):
        cum = cum + ["rope"]
        rungs["shower+MPI+CR+rope"] = cum[:]

    return rungs
