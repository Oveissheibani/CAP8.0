#!/usr/bin/env python3
"""Aggregate leadPartonPdg (string ENDPOINT) vs hardPartonPdg (INITIATING /
hard-scatter parton) over a provenance-study --dump-events JSON, to show WHERE
the gluon ancestry lives.  Confirms the diagnosis: string endpoints are never
gluons (correct physics), but the initiating hard parton frequently is.

Usage: python3 diagnose-gluon.py <run>.events.json
"""
import json, sys, collections

def flavour(pdg):
    a = abs(int(pdg))
    return {1: "LightQuark", 2: "LightQuark", 3: "Strange",
            4: "Charm", 5: "Bottom", 21: "Gluon"}.get(a, "NonPartonic(0/other)")

path = sys.argv[1] if len(sys.argv) > 1 else "dump.events.json"
data = json.load(open(path))

lead = collections.Counter()
hard = collections.Counter()
init = collections.Counter()
n = 0
for ev in data.get("events", []):
    for h in ev.get("hadrons", []):
        n += 1
        lead[flavour(h.get("leadPartonPdg", 0))] += 1
        hard[flavour(h.get("hardPartonPdg", 0))] += 1
        init[flavour(h.get("initiatingPartonPdg", 0))] += 1

order = ["LightQuark", "Strange", "Charm", "Bottom", "Gluon",
         "NonPartonic(0/other)"]
print("studied hadrons: %d\n" % n)
hdr = "%-22s %12s %12s %12s" % ("flavour", "leadParton", "hardParton",
                                "initParton")
print(hdr); print("-" * len(hdr))
for f in order:
    lp = 100.0 * lead[f] / n if n else 0
    hp = 100.0 * hard[f] / n if n else 0
    ip = 100.0 * init[f] / n if n else 0
    print("%-22s %11.2f%% %11.2f%% %11.2f%%" % (f, lp, hp, ip))
print("\nleadParton = string ENDPOINT (Lund) -> gluons impossible by construction")
print("hardParton = nearest HardProcess parton (Pythia status-code path only)")
print("initParton = generator-AGNOSTIC topmost parton -> works for Herwig too")
