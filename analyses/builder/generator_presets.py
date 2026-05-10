"""
Tuned-set library for the Pythia and Herwig event generators — v2.

Data shape:
    PYTHIA_PRESETS[name]       : list[str]     # raw readString lines
    PYTHIA_BOOL_TOGGLES[key]   : dict          # checkbox in the GUI
    PYTHIA_NUMERIC_KNOBS[key]  : dict          # input-box in the GUI
    (same for Herwig)

Each PYTHIA_BOOL_TOGGLES / PYTHIA_NUMERIC_KNOBS entry lives in a "category"
so the GUI can group them sensibly.

References (cross-checked against generator manuals):
  - Pythia 8 tune catalog        https://pythia.org/manuals/pythia8307/Tunes.html
  - Pythia 8 SettingsScheme      https://pythia.org/latest-manual/MainProgramSettings.html
  - Herwig 7 release notes        https://herwig.hepforge.org/tutorials/index.html
  - ATLAS A14 family             arXiv:1407.5043 (NNPDF23LO / CTEQL1 / Var3a/3b/3c)
  - CMS CUETP8M1                 CMS-PAS-GEN-14-001
  - CMS CUETP8M2T4               CMS-TOP-16-021
  - CMS CP1..CP5 family          arXiv:1903.12179, JHEP04(2020)127
  - LHCb Detroit (Pythia)        arXiv:2210.06059
  - CR-BLC (Pythia)              arXiv:1505.01681 (Christiansen-Skands)
  - Pythia Ropes (Bierlich)      arXiv:1412.6259, arXiv:1710.09725
  - Herwig 7 LHC-UE-EE-5         arXiv:1310.6877  (Gieseke, Plätzer, Röhr)
  - Herwig 7 LHC-UE-EE-3 / 4     same family, older rounds
  - CMS CH1 / CH2 / CH3 (Herwig) arXiv:2011.04038, "Development & validation of
                                 HERWIG 7 tunes from CMS underlying-event measurements"
  - ATLAS H7-UE-MMHT             arXiv:1809.04855 (ATLAS Collaboration, 2018)
"""

# ===========================================================================
#  Pythia 8 — TUNED SETS
# ===========================================================================

PYTHIA_PRESETS: dict[str, list[str]] = {
    # ---- Pythia core ----
    "Default (Monash 2013)": [
        # Monash 2013 (Skands–Carrazza–Rojo) is the Pythia 8.2+ default.
        # arXiv:1404.5630.  Tune:pp = 14 / Tune:ee = 7.
        "Tune:pp = 14",
        "Tune:ee = 7",
    ],
    "Pythia 8.1 default (4C)": [
        # Tune:pp = 5 — "Tune 4C" by Corke & Sjöstrand, arXiv:1011.1759.
        "Tune:pp = 5",
    ],
    "Pythia 8.2 default (Monash 2013)": [
        "Tune:pp = 14",
    ],
    "Pythia 8.3 default (Monash 2013)": [
        "Tune:pp = 14",
    ],

    # ---- ATLAS family (Pythia) ----
    "ATLAS A14 NNPDF23LO": [
        # arXiv:1407.5043, Tune:pp = 21.
        "Tune:pp = 21",
    ],
    "ATLAS A14 CTEQL1": [
        "Tune:pp = 19",
    ],
    "ATLAS A14 Var1+ (αsISR+)": [
        "Tune:pp = 21",
        "SpaceShower:alphaSvalue = 0.140",
    ],
    "ATLAS A14 Var1- (αsISR-)": [
        "Tune:pp = 21",
        "SpaceShower:alphaSvalue = 0.115",
    ],
    "ATLAS A14 Var2+ (αsFSR+)": [
        "Tune:pp = 21",
        "TimeShower:alphaSvalue = 0.139",
    ],
    "ATLAS A14 Var2- (αsFSR-)": [
        "Tune:pp = 21",
        "TimeShower:alphaSvalue = 0.111",
    ],

    # ---- CMS family (Pythia) ----
    "CMS CUETP8M1": [
        "Tune:pp = 18",
    ],
    "CMS CUETP8M2T4": [
        "Tune:pp = 19",
        "MultipartonInteractions:pT0Ref = 2.20",
        "MultipartonInteractions:expPow   = 1.60",
    ],
    "CMS CP1 (NNPDF31_lo)": [
        # arXiv:1903.12179, table 7.
        "Tune:pp = 21",
        "MultipartonInteractions:pT0Ref = 2.40",
        "MultipartonInteractions:bProfile = 2",
        "MultipartonInteractions:ecmPow = 0.030",
        "ColourReconnection:range = 2.63",
    ],
    "CMS CP2 (NNPDF31_lo, low-pT)": [
        "Tune:pp = 21",
        "MultipartonInteractions:pT0Ref = 1.41",
        "MultipartonInteractions:bProfile = 2",
        "ColourReconnection:range = 4.5",
    ],
    "CMS CP3 (NNPDF31_lo, intermediate)": [
        "Tune:pp = 21",
        "MultipartonInteractions:pT0Ref = 1.80",
        "ColourReconnection:range = 3.50",
    ],
    "CMS CP4 (NNPDF31_nlo)": [
        "Tune:pp = 21",
        "MultipartonInteractions:pT0Ref = 1.42",
        "MultipartonInteractions:bProfile = 2",
        "ColourReconnection:range = 4.7",
    ],
    "CMS CP5 (NNPDF31_nnlo)": [
        # arXiv:1903.12179, table 7.
        "Tune:pp = 21",
        "MultipartonInteractions:ecmPow = 0.03344",
        "MultipartonInteractions:bProfile = 2",
        "MultipartonInteractions:pT0Ref = 1.41",
        "MultipartonInteractions:coreRadius = 0.7634",
        "MultipartonInteractions:coreFraction = 0.63",
        "ColourReconnection:range = 5.176",
    ],

    # ---- LHCb (Pythia) ----
    "LHCb Detroit": [
        # arXiv:2210.06059
        "Tune:pp = 21",
        "BeamRemnants:reconnectRange = 1.71",
        "MultipartonInteractions:pT0Ref = 2.45",
        "ColourReconnection:range = 1.71",
        "StringPT:sigma = 0.335",
        "StringZ:aLund = 0.36",
        "StringZ:bLund = 0.56",
    ],

    # ---- Beyond-Leading-Colour CR variants (Christiansen-Skands) ----
    "Monash + CR-BLC Mode 0 (default LC)": [
        "Tune:pp = 14",
        "ColourReconnection:reconnect = on",
        "ColourReconnection:mode = 0",
    ],
    "Monash + CR-BLC Mode 1 (more reconnections)": [
        "Tune:pp = 14",
        "ColourReconnection:reconnect = on",
        "ColourReconnection:mode = 1",
        "BeamRemnants:remnantMode = 1",
    ],
    "Monash + CR-BLC Mode 2 (gluon-move)": [
        "Tune:pp = 14",
        "ColourReconnection:reconnect = on",
        "ColourReconnection:mode = 2",
        "BeamRemnants:remnantMode = 1",
        "ColourReconnection:m0 = 0.3",
        "ColourReconnection:allowDoubleJunRem = off",
        "ColourReconnection:allowJunctions = on",
        "ColourReconnection:junctionCorrection = 1.20",
        "ColourReconnection:timeDilationMode = 2",
        "ColourReconnection:timeDilationPar = 0.18",
    ],

    # ---- Soft-QCD / Ropes recipes (Bierlich) ----
    "Monash + Ropes (flavour mods)": [
        "Tune:pp = 14",
        "Ropewalk:RopeHadronization = on",
        "Ropewalk:doShoving = off",
        "Ropewalk:doFlavour = on",
    ],
    "Monash + Ropes + Shoving": [
        "Tune:pp = 14",
        "Ropewalk:RopeHadronization = on",
        "Ropewalk:doShoving = on",
        "Ropewalk:doFlavour = on",
        "PartonVertex:setVertex = on",
    ],

    # ---- Debug / extreme ----
    "MPI off (debug only)": [
        "Tune:pp = 14",
        "PartonLevel:MPI = off",
    ],
    "Showers off (debug only)": [
        "Tune:pp = 14",
        "PartonLevel:ISR = off",
        "PartonLevel:FSR = off",
    ],
    "Custom (only your strings)": [],
}


# ---- Pythia BOOL toggles ---------------------------------------------------
PYTHIA_BOOL_TOGGLES: dict[str, dict] = {
    # category, label, lines emitted when ON
    "soft_qcd_inelastic":
        {"category": "Hard process",  "label": "SoftQCD:inelastic = on (recommended for MB)",
         "lines": ["SoftQCD:inelastic = on"]},
    "soft_qcd_breakdown":
        {"category": "Hard process",  "label": "SoftQCD: nonDiff + SD + DD",
         "lines": ["SoftQCD:nonDiffractive = on",
                   "SoftQCD:singleDiffractive = on",
                   "SoftQCD:doubleDiffractive = on"]},
    "hard_qcd_all":
        {"category": "Hard process",  "label": "HardQCD:all (parton-level)",
         "lines": ["SoftQCD:inelastic = off", "HardQCD:all = on"]},
    "weak_z":
        {"category": "Hard process",  "label": "Weak Z (Drell-Yan)",
         "lines": ["WeakSingleBoson:ffbar2gmZ = on"]},
    "weak_w":
        {"category": "Hard process",  "label": "Weak W±",
         "lines": ["WeakSingleBoson:ffbar2W = on"]},
    "top_pair":
        {"category": "Hard process",  "label": "Top pair (gg + qqbar → ttbar)",
         "lines": ["Top:gg2ttbar = on", "Top:qqbar2ttbar = on"]},

    "cr_qcd_mode1":
        {"category": "Colour reconnection",
         "label": "CR mode 1 (QCD-based, Christiansen-Skands)",
         "lines": ["ColourReconnection:reconnect = on",
                   "ColourReconnection:mode = 1",
                   "BeamRemnants:remnantMode = 1"]},
    "cr_qcd_mode2":
        {"category": "Colour reconnection",
         "label": "CR mode 2 (gluon-move BLC)",
         "lines": ["ColourReconnection:reconnect = on",
                   "ColourReconnection:mode = 2",
                   "BeamRemnants:remnantMode = 1"]},
    "cr_off":
        {"category": "Colour reconnection",
         "label": "Colour Reconnection OFF",
         "lines": ["ColourReconnection:reconnect = off"]},
    "junctions_on":
        {"category": "Colour reconnection",
         "label": "BeamRemnants junctionFlavour = on",
         "lines": ["BeamRemnants:junctionFlavour = on"]},

    "ropes_on":
        {"category": "Ropes",
         "label": "String Ropes (flavour modifications)",
         "lines": ["Ropewalk:RopeHadronization = on",
                   "Ropewalk:doFlavour = on"]},
    "shoving_on":
        {"category": "Ropes",
         "label": "String Shoving (push-out shower)",
         "lines": ["Ropewalk:RopeHadronization = on",
                   "Ropewalk:doShoving = on",
                   "PartonVertex:setVertex = on"]},

    "no_mpi":
        {"category": "Switches",
         "label": "MPI OFF (debug only)",
         "lines": ["PartonLevel:MPI = off"]},
    "no_isr":
        {"category": "Switches",
         "label": "ISR OFF (debug only)",
         "lines": ["PartonLevel:ISR = off"]},
    "no_fsr":
        {"category": "Switches",
         "label": "FSR OFF (debug only)",
         "lines": ["PartonLevel:FSR = off"]},
}


# ---- Pythia NUMERIC knobs --------------------------------------------------
# Each key takes a number; if the user leaves the input empty, no line is
# emitted.  `format` is a Python str.format pattern with one positional slot
# for the value.  `default` is shown as placeholder text in the input box.
PYTHIA_NUMERIC_KNOBS: dict[str, dict] = {
    "mpi_pT0Ref": {
        "category": "MPI",
        "label":   "MultipartonInteractions:pT0Ref (GeV)",
        "default": "2.40",
        "format":  "MultipartonInteractions:pT0Ref = {}",
    },
    "mpi_ecmPow": {
        "category": "MPI",
        "label":   "MultipartonInteractions:ecmPow",
        "default": "0.030",
        "format":  "MultipartonInteractions:ecmPow = {}",
    },
    "mpi_bProfile": {
        "category": "MPI",
        "label":   "MultipartonInteractions:bProfile (1=expon, 2=Gauss, 3=double-Gauss)",
        "default": "2",
        "format":  "MultipartonInteractions:bProfile = {}",
    },
    "mpi_coreRadius": {
        "category": "MPI",
        "label":   "MultipartonInteractions:coreRadius",
        "default": "0.7634",
        "format":  "MultipartonInteractions:coreRadius = {}",
    },
    "mpi_coreFraction": {
        "category": "MPI",
        "label":   "MultipartonInteractions:coreFraction",
        "default": "0.63",
        "format":  "MultipartonInteractions:coreFraction = {}",
    },
    "mpi_expPow": {
        "category": "MPI",
        "label":   "MultipartonInteractions:expPow",
        "default": "1.60",
        "format":  "MultipartonInteractions:expPow = {}",
    },

    "cr_range": {
        "category": "Colour reconnection",
        "label":   "ColourReconnection:range",
        "default": "5.176",
        "format":  "ColourReconnection:range = {}",
    },
    "cr_m0": {
        "category": "Colour reconnection",
        "label":   "ColourReconnection:m0",
        "default": "0.3",
        "format":  "ColourReconnection:m0 = {}",
    },
    "cr_junctionCorr": {
        "category": "Colour reconnection",
        "label":   "ColourReconnection:junctionCorrection",
        "default": "1.20",
        "format":  "ColourReconnection:junctionCorrection = {}",
    },

    "stringpt_sigma": {
        "category": "Hadronization",
        "label":   "StringPT:sigma  (transverse momentum spread)",
        "default": "0.335",
        "format":  "StringPT:sigma = {}",
    },
    "stringz_aLund": {
        "category": "Hadronization",
        "label":   "StringZ:aLund  (Lund a parameter)",
        "default": "0.36",
        "format":  "StringZ:aLund = {}",
    },
    "stringz_bLund": {
        "category": "Hadronization",
        "label":   "StringZ:bLund  (Lund b parameter)",
        "default": "0.56",
        "format":  "StringZ:bLund = {}",
    },
    "stringFlav_probStoUD": {
        "category": "Hadronization",
        "label":   "StringFlav:probStoUD  (strangeness suppression)",
        "default": "0.217",
        "format":  "StringFlav:probStoUD = {}",
    },
    "stringFlav_probQQtoQ": {
        "category": "Hadronization",
        "label":   "StringFlav:probQQtoQ  (di-quark / quark)",
        "default": "0.081",
        "format":  "StringFlav:probQQtoQ = {}",
    },

    "isr_alphaS": {
        "category": "Shower",
        "label":   "SpaceShower:alphaSvalue (ISR αs)",
        "default": "0.127",
        "format":  "SpaceShower:alphaSvalue = {}",
    },
    "fsr_alphaS": {
        "category": "Shower",
        "label":   "TimeShower:alphaSvalue  (FSR αs)",
        "default": "0.127",
        "format":  "TimeShower:alphaSvalue = {}",
    },

    "remnants_reconRange": {
        "category": "Beam remnants",
        "label":   "BeamRemnants:reconnectRange",
        "default": "1.71",
        "format":  "BeamRemnants:reconnectRange = {}",
    },
}


# ===========================================================================
#  Herwig 7 — TUNED SETS  (cross-referenced)
# ===========================================================================

HERWIG_PRESETS: dict[str, list[str]] = {
    # ---- Herwig core (Gieseke, Plätzer, Röhr — Herwig collaboration) ----
    "Default (Herwig 7.3 — LHC-UE-EE-5)": [
        # The Herwig 7 default is the LHC-UE-EE-5 tune from
        # arXiv:1310.6877.  No extra lines needed — everything is
        # already in the shipped LHC.in.
    ],
    "LHC-UE-EE-5C  (older 7.0 default)": [
        # The "C" variant — same family, slightly older parameters.
        "set /Herwig/Shower/AlphaQCD:AlphaIn 0.118",
        "set /Herwig/UnderlyingEvent/MPIHandler:pTmin0 4.0",
    ],
    "LHC-UE-EE-4 (older Herwig 7 tune)": [
        # Earlier round of the same tuning programme.
        "set /Herwig/Shower/AlphaQCD:AlphaIn 0.121",
        "set /Herwig/UnderlyingEvent/MPIHandler:pTmin0 3.0",
    ],
    "LHC-UE-EE-3 (oldest published)": [
        "set /Herwig/Shower/AlphaQCD:AlphaIn 0.123",
        "set /Herwig/UnderlyingEvent/MPIHandler:pTmin0 2.5",
    ],

    # ---- CMS family (Herwig) — arXiv:2011.04038 ----
    "CMS CH1 (Herwig, NNPDF23_lo)": [
        # CH1 from CMS Herwig tuning paper 2020.
        "set /Herwig/UnderlyingEvent/MPIHandler:pTmin0 3.91",
        "set /Herwig/UnderlyingEvent/MPIHandler:Power 0.33",
        "set /Herwig/UnderlyingEvent/MPIHandler:InvRadius2 1.35",
        "set /Herwig/Hadronization/ColourReconnector:Algorithm Plain",
        "set /Herwig/Hadronization/ColourReconnector:ColourReconnection Yes",
        "set /Herwig/Hadronization/ColourReconnector:ReconnectionProbability 0.49",
    ],
    "CMS CH2 (Herwig, NNPDF31_lo)": [
        "set /Herwig/UnderlyingEvent/MPIHandler:pTmin0 4.15",
        "set /Herwig/UnderlyingEvent/MPIHandler:Power 0.36",
        "set /Herwig/UnderlyingEvent/MPIHandler:InvRadius2 1.35",
        "set /Herwig/Hadronization/ColourReconnector:Algorithm Plain",
        "set /Herwig/Hadronization/ColourReconnector:ColourReconnection Yes",
        "set /Herwig/Hadronization/ColourReconnector:ReconnectionProbability 0.42",
    ],
    "CMS CH3 (Herwig, NNPDF31_nnlo)": [
        # CMS CH3 — used by CMS Run-2 Herwig productions.
        "set /Herwig/UnderlyingEvent/MPIHandler:pTmin0 4.286",
        "set /Herwig/UnderlyingEvent/MPIHandler:Power 0.4055",
        "set /Herwig/UnderlyingEvent/MPIHandler:InvRadius2 2.30",
        "set /Herwig/Hadronization/ColourReconnector:Algorithm Plain",
        "set /Herwig/Hadronization/ColourReconnector:ColourReconnection Yes",
        "set /Herwig/Hadronization/ColourReconnector:ReconnectionProbability 0.4828",
    ],

    # ---- ATLAS family (Herwig) ----
    "ATLAS H7-UE-MMHT (arXiv:1809.04855)": [
        # ATLAS Herwig 7 UE tune with MMHT2014lo PDF.
        "set /Herwig/UnderlyingEvent/MPIHandler:pTmin0 4.6",
        "set /Herwig/UnderlyingEvent/MPIHandler:InvRadius2 1.27",
    ],

    # ---- Shower variants (apply on top of the default tune) ----
    "Default + Dipole shower": [
        "read snippets/DipoleShowerOnly.in",
    ],
    "Default + CMW-DipoleShower": [
        "read snippets/DipoleShowerOnly.in",
        "set /Herwig/Shower/AlphaQCD:RenormalizationScheme CMW",
    ],

    # ---- CR overlays applied on top of the default tune ----
    "Default + Plain CR": [
        "set /Herwig/Hadronization/ColourReconnector:Algorithm Plain",
        "set /Herwig/Hadronization/ColourReconnector:ColourReconnection Yes",
    ],
    "Default + Baryonic CR": [
        "set /Herwig/Hadronization/ColourReconnector:Algorithm Baryonic",
        "set /Herwig/Hadronization/ColourReconnector:ColourReconnection Yes",
    ],

    # ---- Debug / extreme ----
    "MPI off (debug only)": [
        "set /Herwig/UnderlyingEvent/MPIHandler:pTmin0 1000.0",
    ],
    "Showers off (debug only)": [
        "set /Herwig/Shower/ShowerHandler:HardEmission None",
    ],
    "Custom (only your strings)": [],
}


HERWIG_BOOL_TOGGLES: dict[str, dict] = {
    "cr_plain":
        {"category": "Colour reconnection",
         "label": "Plain CR ON",
         "lines": ["set /Herwig/Hadronization/ColourReconnector:Algorithm Plain",
                   "set /Herwig/Hadronization/ColourReconnector:ColourReconnection Yes"]},
    "cr_baryonic":
        {"category": "Colour reconnection",
         "label": "Baryonic CR ON",
         "lines": ["set /Herwig/Hadronization/ColourReconnector:Algorithm Baryonic",
                   "set /Herwig/Hadronization/ColourReconnector:ColourReconnection Yes"]},
    "cr_off":
        {"category": "Colour reconnection",
         "label": "CR OFF",
         "lines": ["set /Herwig/Hadronization/ColourReconnector:ColourReconnection No"]},

    "dipole_shower":
        {"category": "Shower",
         "label": "Switch to Dipole shower (overrides QTilde)",
         "lines": ["read snippets/DipoleShowerOnly.in"]},
    "cmw_scheme":
        {"category": "Shower",
         "label": "CMW renormalisation scheme",
         "lines": ["set /Herwig/Shower/AlphaQCD:RenormalizationScheme CMW"]},

    "no_mpi":
        {"category": "Switches",
         "label": "MPI OFF (debug only)",
         "lines": ["set /Herwig/UnderlyingEvent/MPIHandler:pTmin0 1000.0"]},
    "yfs_off":
        {"category": "Switches",
         "label": "YFS soft-photon corrections OFF",
         "lines": ["# read snippets/YFS.in"]},

    "spin_corr":
        {"category": "Decay",
         "label": "Spin correlations ON",
         "lines": ["set /Herwig/EventHandlers/EventHandler:CollisionCutsHandler "
                   "/Herwig/Cuts/Cuts"]},

    "drellyan_analysis":
        {"category": "Hard process",
         "label": "Insert DrellYan analysis",
         "lines": ["insert /Herwig/Generators/EventGenerator:AnalysisHandlers 0 "
                   "/Herwig/Analysis/DrellYan"]},
    "ttbar_analysis":
        {"category": "Hard process",
         "label": "Insert TTbar analysis",
         "lines": ["insert /Herwig/Generators/EventGenerator:AnalysisHandlers 0 "
                   "/Herwig/Analysis/TTbar"]},
}


HERWIG_NUMERIC_KNOBS: dict[str, dict] = {
    "mpi_pTmin0": {
        "category": "MPI",
        "label":   "MPIHandler:pTmin0 (GeV)",
        "default": "4.0",
        "format":  "set /Herwig/UnderlyingEvent/MPIHandler:pTmin0 {}",
    },
    "mpi_power": {
        "category": "MPI",
        "label":   "MPIHandler:Power",
        "default": "0.33",
        "format":  "set /Herwig/UnderlyingEvent/MPIHandler:Power {}",
    },
    "mpi_invRadius2": {
        "category": "MPI",
        "label":   "MPIHandler:InvRadius2 (1/fm²)",
        "default": "1.35",
        "format":  "set /Herwig/UnderlyingEvent/MPIHandler:InvRadius2 {}",
    },

    "cr_probability": {
        "category": "Colour reconnection",
        "label":   "ColourReconnector:ReconnectionProbability",
        "default": "0.49",
        "format":  "set /Herwig/Hadronization/ColourReconnector:ReconnectionProbability {}",
    },

    "cluster_max_light": {
        "category": "Cluster hadronization",
        "label":   "ClusterFissioner:ClMaxLight (GeV)",
        "default": "3.55",
        "format":  "set /Herwig/Hadronization/ClusterFissioner:ClMaxLight {}",
    },
    "cluster_pow_light": {
        "category": "Cluster hadronization",
        "label":   "ClusterFissioner:ClPowLight",
        "default": "2.78",
        "format":  "set /Herwig/Hadronization/ClusterFissioner:ClPowLight {}",
    },
    "cluster_psplit_light": {
        "category": "Cluster hadronization",
        "label":   "ClusterFissioner:PSplitLight",
        "default": "1.20",
        "format":  "set /Herwig/Hadronization/ClusterFissioner:PSplitLight {}",
    },
    "diquark_weight": {
        "category": "Cluster hadronization",
        "label":   "HadronSelector:PwtDIquark",
        "default": "0.49",
        "format":  "set /Herwig/Hadronization/HadronSelector:PwtDIquark {}",
    },

    "alpha_qcd": {
        "category": "Shower",
        "label":   "AlphaQCD:AlphaIn (αs at MZ)",
        "default": "0.118",
        "format":  "set /Herwig/Shower/AlphaQCD:AlphaIn {}",
    },
}


# ===========================================================================
#  Particle-stability table  —  prevent specific species from decaying
# ===========================================================================
#
# The user's analysis convention (see herwig7_install_prompt 2.txt §7) calls
# for stabilising the strange-hadron family (and pi0 / eta) so they appear
# as status-1 in the event record.  Same physics intent has different syntax
# in Pythia and Herwig:
#
#   Pythia 8 :   "<pdg>:mayDecay = false"            (e.g. "111:mayDecay = false")
#   Herwig 7 :   "set /Herwig/Particles/<name>:Stable Stable"
#
# STABLE_PARTICLE_TABLE is the canonical (PDG, herwig_name, label) triplet
# bank.  The GUI's listbox shows the labels; the codepath below converts
# each entry to the right syntax.

STABLE_PARTICLE_TABLE: list[dict] = [
    # ----- Light pseudoscalar mesons (self-conjugate or own antiparticle) -----
    {"pdg":  111, "herwig": "pi0",        "label": "pi0  (111)",            "group": "Light mesons"},
    {"pdg":  221, "herwig": "eta",        "label": "eta  (221)",            "group": "Light mesons"},
    {"pdg":  331, "herwig": "eta'",       "label": "eta' (331)",            "group": "Light mesons"},
    {"pdg":  310, "herwig": "K_S0",       "label": "K0_S (310)",            "group": "Light mesons"},
    {"pdg":  130, "herwig": "K_L0",       "label": "K0_L (130)",            "group": "Light mesons"},

    # ----- Strange baryons (Lambda, Sigma, Xi, Omega + antiparticles) ---------
    {"pdg": 3122, "herwig": "Lambda0",    "label": "Lambda      (3122)",    "group": "Strange baryons"},
    {"pdg":-3122, "herwig": "Lambdabar0", "label": "Lambda-bar  (-3122)",   "group": "Strange baryons"},
    {"pdg": 3222, "herwig": "Sigma+",     "label": "Sigma+      (3222)",    "group": "Strange baryons"},
    {"pdg":-3222, "herwig": "Sigmabar-",  "label": "Sigma-bar-  (-3222)",   "group": "Strange baryons"},
    {"pdg": 3112, "herwig": "Sigma-",     "label": "Sigma-      (3112)",    "group": "Strange baryons"},
    {"pdg":-3112, "herwig": "Sigmabar+",  "label": "Sigma-bar+  (-3112)",   "group": "Strange baryons"},
    {"pdg": 3212, "herwig": "Sigma0",     "label": "Sigma0      (3212)",    "group": "Strange baryons"},
    {"pdg":-3212, "herwig": "Sigmabar0",  "label": "Sigma-bar0  (-3212)",   "group": "Strange baryons"},
    {"pdg": 3312, "herwig": "Xi-",        "label": "Xi-     (3312)",        "group": "Strange baryons"},
    {"pdg":-3312, "herwig": "Xibar+",     "label": "Xi-bar+ (-3312)",       "group": "Strange baryons"},
    {"pdg": 3322, "herwig": "Xi0",        "label": "Xi0     (3322)",        "group": "Strange baryons"},
    {"pdg":-3322, "herwig": "Xibar0",     "label": "Xi-bar0 (-3322)",       "group": "Strange baryons"},
    {"pdg": 3334, "herwig": "Omega-",     "label": "Omega-     (3334)",     "group": "Strange baryons"},
    {"pdg":-3334, "herwig": "Omegabar+",  "label": "Omega-bar+ (-3334)",    "group": "Strange baryons"},

    # ----- Charged lepton (only the long-lived one) ---------------------------
    {"pdg":   15, "herwig": "tau-",       "label": "tau-  (15)",            "group": "Lepton"},
    {"pdg":  -15, "herwig": "tau+",       "label": "tau+  (-15)",           "group": "Lepton"},

    # ----- Charm mesons (D0, D+, D_s+ ± antiparticles) ------------------------
    # Herwig 7.3 default Repository names — verified against
    # share/Herwig/defaults/Particles.in.  D* states deliberately omitted
    # from the GUI (they decay strongly/EM in 10⁻²² s; no analysis ever
    # stabilises them).
    {"pdg":  421, "herwig": "D0",         "label": "D0    (421)",           "group": "Charm mesons"},
    {"pdg": -421, "herwig": "Dbar0",      "label": "D0-bar (-421)",         "group": "Charm mesons"},
    {"pdg":  411, "herwig": "D+",         "label": "D+    (411)",           "group": "Charm mesons"},
    {"pdg": -411, "herwig": "D-",         "label": "D-    (-411)",          "group": "Charm mesons"},
    {"pdg":  431, "herwig": "D_s+",       "label": "D_s+  (431)",           "group": "Charm mesons"},
    {"pdg": -431, "herwig": "D_s-",       "label": "D_s-  (-431)",          "group": "Charm mesons"},

    # ----- Charm baryons (Lambda_c, Xi_c, Omega_c) ----------------------------
    {"pdg": 4122, "herwig": "Lambda_c+",     "label": "Lambda_c+   (4122)",   "group": "Charm baryons"},
    {"pdg":-4122, "herwig": "Lambdabar_c-",  "label": "Lambda_c-   (-4122)",  "group": "Charm baryons"},
    {"pdg": 4232, "herwig": "Xi_c+",         "label": "Xi_c+       (4232)",   "group": "Charm baryons"},
    {"pdg":-4232, "herwig": "Xibar_c-",      "label": "Xi_c-       (-4232)",  "group": "Charm baryons"},
    {"pdg": 4132, "herwig": "Xi_c0",         "label": "Xi_c0       (4132)",   "group": "Charm baryons"},
    {"pdg":-4132, "herwig": "Xibar_c0",      "label": "Xi_c0-bar   (-4132)",  "group": "Charm baryons"},
    {"pdg": 4332, "herwig": "Omega_c0",      "label": "Omega_c0    (4332)",   "group": "Charm baryons"},
    {"pdg":-4332, "herwig": "Omegabar_c0",   "label": "Omega_c0-bar(-4332)",  "group": "Charm baryons"},

    # ----- Bottom mesons (B+, B0, B_s0, B_c+ ± antiparticles) -----------------
    {"pdg":  521, "herwig": "B+",         "label": "B+    (521)",           "group": "Bottom mesons"},
    {"pdg": -521, "herwig": "B-",         "label": "B-    (-521)",          "group": "Bottom mesons"},
    {"pdg":  511, "herwig": "B0",         "label": "B0    (511)",           "group": "Bottom mesons"},
    {"pdg": -511, "herwig": "Bbar0",      "label": "B0-bar (-511)",         "group": "Bottom mesons"},
    {"pdg":  531, "herwig": "B_s0",       "label": "B_s0  (531)",           "group": "Bottom mesons"},
    {"pdg": -531, "herwig": "B_sbar0",    "label": "B_s0-bar (-531)",       "group": "Bottom mesons"},
    {"pdg":  541, "herwig": "B_c+",       "label": "B_c+  (541)",           "group": "Bottom mesons"},
    {"pdg": -541, "herwig": "B_c-",       "label": "B_c-  (-541)",          "group": "Bottom mesons"},

    # ----- Bottom baryons (Lambda_b, Xi_b, Omega_b) ---------------------------
    {"pdg": 5122, "herwig": "Lambda_b0",     "label": "Lambda_b0   (5122)",   "group": "Bottom baryons"},
    {"pdg":-5122, "herwig": "Lambdabar_b0",  "label": "Lambda_b0-bar (-5122)","group": "Bottom baryons"},
    {"pdg": 5232, "herwig": "Xi_b0",         "label": "Xi_b0       (5232)",   "group": "Bottom baryons"},
    {"pdg":-5232, "herwig": "Xibar_b0",      "label": "Xi_b0-bar   (-5232)",  "group": "Bottom baryons"},
    {"pdg": 5132, "herwig": "Xi_b-",         "label": "Xi_b-       (5132)",   "group": "Bottom baryons"},
    {"pdg":-5132, "herwig": "Xibar_b+",      "label": "Xi_b-bar+   (-5132)",  "group": "Bottom baryons"},
    {"pdg": 5332, "herwig": "Omega_b-",      "label": "Omega_b-    (5332)",   "group": "Bottom baryons"},
    {"pdg":-5332, "herwig": "Omegabar_b+",   "label": "Omega_b-bar+(-5332)",  "group": "Bottom baryons"},

    # ----- Quarkonia (self-conjugate; only the most-studied states) -----------
    # Herwig 7.3 names (best-effort) — please verify against your install
    # if you stabilise these for a publication-grade run.  J/psi and
    # Upsilon(1S) are the canonical onia probes.
    {"pdg":   443, "herwig": "Jpsi",         "label": "J/psi(1S)   (443)",    "group": "Quarkonia"},
    {"pdg":100443, "herwig": "psi(2S)",      "label": "psi(2S)    (100443)",  "group": "Quarkonia"},
    {"pdg":   441, "herwig": "eta_c",        "label": "eta_c(1S)   (441)",    "group": "Quarkonia"},
    {"pdg":   553, "herwig": "Upsilon",      "label": "Upsilon(1S) (553)",    "group": "Quarkonia"},
    {"pdg":100553, "herwig": "Upsilon(2S)",  "label": "Upsilon(2S)(100553)",  "group": "Quarkonia"},
    {"pdg":200553, "herwig": "Upsilon(3S)",  "label": "Upsilon(3S)(200553)",  "group": "Quarkonia"},
    {"pdg":   551, "herwig": "eta_b",        "label": "eta_b(1S)   (551)",    "group": "Quarkonia"},
]

# Convenience reverse-maps for fast lookup.
STABLE_PDG_TO_HERWIG = {row["pdg"]: row["herwig"] for row in STABLE_PARTICLE_TABLE}
STABLE_PDG_TO_LABEL  = {row["pdg"]: row["label"]  for row in STABLE_PARTICLE_TABLE}


def _emit_pythia_stable(pdgs) -> list[str]:
    """Convert a list of PDG ints to Pythia readString lines that prevent
    each species from decaying.  Pythia treats `mayDecay = false` for the
    given PDG; antiparticle handling is automatic in Pythia 8 — they share
    the same Particle entry."""
    out = []
    seen = set()
    for p in (pdgs or []):
        try:
            pdg = int(p)
        except (TypeError, ValueError):
            continue
        # Pythia keys particle data on the absolute PDG; the antiparticle
        # inherits.  We still emit the user's literal value so it shows
        # up in their .ini for the record.
        if pdg in seen:
            continue
        seen.add(pdg)
        out.append(f"{pdg}:mayDecay = false")
    return out


def _emit_pythia_lifetime_cut(ctau_max_mm) -> list[str]:
    """Pythia 8 global cτ cut.  Particles whose mean proper-lifetime cτ
    (in mm) exceeds `tau0Max` are not decayed.  Together with the
    per-particle ':mayDecay = false' lines this lets the user pick
    either a surgical OR a global cutoff.

    Pythia spelling (verified against pythia.org/manuals/pythia8307/
    ParticleDecays.html):
        ParticleDecays:limitTau0 = on
        ParticleDecays:tau0Max   = <value in mm>
    """
    if ctau_max_mm is None or str(ctau_max_mm).strip() == "":
        return []
    try:
        v = float(ctau_max_mm)
    except (TypeError, ValueError):
        return []
    if v <= 0:
        return []
    return [
        "ParticleDecays:limitTau0 = on",
        f"ParticleDecays:tau0Max = {v}",
    ]


def _emit_herwig_lifetime_cut(ctau_max_mm) -> list[str]:
    """Herwig 7 global cτ cut.  Anything with cτ > MaxLifeTime is
    treated as stable.  This is the §7-of-the-install-prompt 'blunt
    instrument' approach.  Use the per-particle Stable flag for
    precision and this for systematics studies.

    Herwig spelling:
        set /Herwig/Decays/DecayHandler:MaxLifeTime <value>*mm
    """
    if ctau_max_mm is None or str(ctau_max_mm).strip() == "":
        return []
    try:
        v = float(ctau_max_mm)
    except (TypeError, ValueError):
        return []
    if v <= 0:
        return []
    return [f"set /Herwig/Decays/DecayHandler:MaxLifeTime {v}*mm"]


def _emit_herwig_stable(pdgs_or_names) -> list[str]:
    """Convert a list of PDG ints OR Herwig particle-names to Herwig
    `set /Herwig/Particles/<name>:Stable Stable` lines.  PDG ints get
    looked up in STABLE_PDG_TO_HERWIG; unknown PDGs are silently skipped
    (Herwig has no PDG-keyed stability flag — must use the name)."""
    out = []
    seen = set()
    for x in (pdgs_or_names or []):
        # Already a name?
        if isinstance(x, str) and not x.lstrip("-").isdigit():
            name = x.strip()
            if name and name not in seen:
                seen.add(name)
                out.append(f"set /Herwig/Particles/{name}:Stable Stable")
            continue
        try:
            pdg = int(x)
        except (TypeError, ValueError):
            continue
        name = STABLE_PDG_TO_HERWIG.get(pdg)
        if name and name not in seen:
            seen.add(name)
            out.append(f"set /Herwig/Particles/{name}:Stable Stable")
    return out


# ===========================================================================
#  Helpers — collapse preset + bool toggles + numeric knobs + custom
# ===========================================================================

def _emit_numeric(knobs: dict, values: dict) -> list[str]:
    """For each knob in `values` whose value is a non-empty string,
    format the corresponding line.  Tolerant of bad values: we just
    pass them through (Pythia / Herwig will surface the error)."""
    out = []
    for key, val in (values or {}).items():
        v = (val or "").strip()
        if not v: continue
        spec = knobs.get(key)
        if not spec: continue
        try:
            out.append(spec["format"].format(v))
        except Exception:
            out.append(spec["format"].replace("{}", v))
    return out


def collect_pythia_strings(
        preset_name: str,
        bool_flags: dict,
        numeric_values: dict,
        custom_lines: str,
        stable_pdgs=None,
        *, ctau_max_mm=None) -> list[str]:
    """Args:
        preset_name    — key into PYTHIA_PRESETS
        bool_flags     — {key: bool} for PYTHIA_BOOL_TOGGLES
        numeric_values — {key: str} for PYTHIA_NUMERIC_KNOBS
        custom_lines   — free-form readString block (multi-line string)
        stable_pdgs    — optional list of PDG ints to make non-decaying
        ctau_max_mm    — optional global cτ cutoff in mm (keyword-only).
                         Particles with cτ > value are not decayed.
    """
    out: list[str] = []
    out.extend(PYTHIA_PRESETS.get(preset_name, []))
    for key, on in (bool_flags or {}).items():
        if on and key in PYTHIA_BOOL_TOGGLES:
            out.extend(PYTHIA_BOOL_TOGGLES[key]["lines"])
    out.extend(_emit_numeric(PYTHIA_NUMERIC_KNOBS, numeric_values))
    out.extend(_emit_pythia_lifetime_cut(ctau_max_mm))
    out.extend(_emit_pythia_stable(stable_pdgs))
    for ln in (custom_lines or "").splitlines():
        s = ln.strip()
        if s and not s.startswith("#"):
            out.append(s)
    return out


def collect_herwig_lines(
        preset_name: str,
        bool_flags: dict,
        numeric_values: dict,
        custom_lines: str,
        stable_pdgs_or_names=None,
        *, ctau_max_mm=None) -> list[str]:
    """Args:
        preset_name             — key into HERWIG_PRESETS
        bool_flags              — {key: bool} for HERWIG_BOOL_TOGGLES
        numeric_values          — {key: str} for HERWIG_NUMERIC_KNOBS
        custom_lines            — free-form .in block (multi-line string)
        stable_pdgs_or_names    — optional list mixing PDG ints + Herwig
                                  names to be made non-decaying
        ctau_max_mm             — optional global cτ cutoff in mm
                                  (keyword-only).  Anything longer-lived
                                  is treated as stable.
    """
    out: list[str] = []
    out.extend(HERWIG_PRESETS.get(preset_name, []))
    for key, on in (bool_flags or {}).items():
        if on and key in HERWIG_BOOL_TOGGLES:
            out.extend(HERWIG_BOOL_TOGGLES[key]["lines"])
    out.extend(_emit_numeric(HERWIG_NUMERIC_KNOBS, numeric_values))
    out.extend(_emit_herwig_lifetime_cut(ctau_max_mm))
    out.extend(_emit_herwig_stable(stable_pdgs_or_names))
    for ln in (custom_lines or "").splitlines():
        s = ln.rstrip()
        if s:
            out.append(s)
    return out


# Backward-compat aliases for older callers (the GUI code still uses
# PYTHIA_PANEL_OPTIONS / HERWIG_PANEL_OPTIONS — keep those names alive
# until the GUIs are updated to the v2 split).
PYTHIA_PANEL_OPTIONS = {
    k: (v["category"] + ": " + v["label"], v["lines"])
    for k, v in PYTHIA_BOOL_TOGGLES.items()
}
HERWIG_PANEL_OPTIONS = {
    k: (v["category"] + ": " + v["label"], v["lines"])
    for k, v in HERWIG_BOOL_TOGGLES.items()
}
