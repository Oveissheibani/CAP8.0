/* =====================================================================
 *  provenance-report-captions.inc  —  data-only side of
 *  figureInterpretations().  Included once by provenance-report.cpp.
 *
 *  Keep the rest of provenance-report.cpp focused on structure /
 *  rendering; the prose lives here.  Edit captions in this file alone
 *  to update the "How to read this" notes attached to each figure.
 *
 *  Each entry is keyed by the figure file STEM (basename without
 *  extension).  Stems matched verbatim.  Falls through to empty if
 *  the figure name is not listed — the descriptive caption from the
 *  plotter manifest then stands alone.
 * ==================================================================== */
  static const std::map<std::string,std::string> M = {
    // ----- single-particle spectra -------------------------------------
    {"origin_pt",
     "What to look at: the 'primary' curve (blue) is the spectrum you would "
     "have if no hadrons decayed — the direct hadronization product.  The "
     "'from resonance' curve (red) sits ON TOP at low pT — that is where "
     "feed-down piles up.  At higher pT the primary curve takes over because "
     "decay kinematics dump the daughters at low pT.  The total 'all pions' "
     "(black) is just the sum."},
    {"origin_eta",
     "Read shape, not normalization.  All three origin classes give a wide, "
     "roughly flat distribution because pp -> X is rapidity-translation "
     "invariant.  Small deviations of the resonance curve from the primary "
     "curve at large |eta| flag a kinematic bias of the decay daughters."},
    {"parton_pt",
     "The light-quark line (blue) dominates everywhere.  The strange line "
     "rises with MPI / colour reconnection.  The gluon-ancestry line is "
     "essentially zero in pure-shower runs and grows visibly once MPI is "
     "switched on — that single observation is one of the strongest pieces "
     "of evidence in this report that MPI is what gives soft pions their "
     "gluonic component."},
    {"n_parton_ancestors",
     "Bin 1 dominates: most pions trace to a SINGLE pre-hadronization "
     "parton (string endpoint).  A tail at 2+ means several partons "
     "contributed — string gluon kinks, MPI merging, or join-up at the "
     "hadronization vertex.  Bin 0 should be empty in a healthy run; "
     "non-zero entries there mean the tagger missed the partonic ancestry."},
    {"decay_chain_depth",
     "Depth 0 = primary, depth 1 = single decay (e.g. rho -> pi pi), "
     "depth 2 = a two-step cascade (e.g. D -> K pi, K -> ...).  Long tails "
     "are the signature of heavy-flavour cascades and K0S / Lambda chains."},
    {"event_multiplicity",
     "Per-event count of the studied species.  The Low / Mid / High "
     "multiplicity bins used elsewhere in the report split this distribution "
     "at 20 and 80 entries per event."},
    {"common_ancestor_depth",
     "Per-pair: the earliest shared-ancestor stage.  Bin 0 = no shared "
     "ancestor (Unrelated).  Higher bins are deeper sharing: 20 = same "
     "hard-process parton, 70 = same pre-hadronization parton, 80 = same "
     "primary-hadron parent.  Single-plot summary of how genealogically "
     "close pion pairs are."},

    // ----- pair classes (the main 2-particle decompositions) -----------
    {"pair_dphi",
     "Read each colour as a story.  SameResonance (red) is a sharp near-side "
     "spike at Delta-phi = 0 (the two pions of one resonance fly close in "
     "azimuth).  SharedParton (blue) is a broader near-side peak — the "
     "jet-like correlation.  Unrelated (grey) is the flat combinatorial "
     "floor.  Same-weak-parent is tiny and lives under the spike.  Y-axis "
     "is log because the classes span four orders of magnitude."},
    {"pair_deta",
     "Same colour scheme as pair_dphi.  SameResonance is sharp around "
     "Delta-eta = 0 (back-to-back in eta is just as likely as same-side).  "
     "SharedParton has a broader gaussian-like peak.  Unrelated is flat: "
     "no correlation in pseudorapidity."},
    {"pair_mass",
     "This is the validation plot.  SameResonance pairs cluster at the "
     "rho(770), K*(892), omega(782), phi(1020), Delta(1232) masses.  If "
     "those peaks are visible you can trust the resonance tagger."},
    {"pair_dphi_shapes",
     "Same as pair_dphi but every class normalized to unit area, so you "
     "see SHAPE, not yield.  Two correlations with the same shape but "
     "different normalizations look identical here.  Use this when the "
     "log-axis spans are distracting in pair_dphi."},
    {"pair_deta_shapes",
     "Shape comparison of pair_deta.  Unrelated is flat by construction; "
     "all other classes show a peak whose width tells you the typical "
     "Delta-eta of correlated pion pairs in that ancestry."},

    // ----- charge / sign decompositions -------------------------------
    {"pair_dphi_SS",
     "Same-sign pairs only.  The balance function uses this minus pair_dphi_OS.  "
     "Resonances mostly decay into opposite-sign pairs so SameResonance(SS) "
     "is smaller than SameResonance(OS)."},
    {"pair_dphi_OS",
     "Opposite-sign pairs.  Carries most of the resonance signal.  "
     "OS minus SS at Delta-phi=0 is a direct measurement of "
     "resonance-driven near-side correlation."},
    {"pair_mass_OS",
     "OS pairs only — the rho / K* / omega / phi peaks live here, on top "
     "of the SharedParton continuum."},
    {"pair_mass_SS",
     "SS pairs lack the resonance peaks (charge conservation).  This is "
     "the cleanest view of the partonic + combinatorial background under "
     "the OS resonance peaks."},

    // ----- resonance subtypes -----------------------------------------
    {"pair_mass_resonance",
     "Each curve peaks at the mass of its parent resonance.  Rho near 0.77, "
     "omega near 0.78, K* near 0.89, phi near 1.02 GeV.  Direct, visible "
     "validation of the resonance classifier."},
    {"pair_dphi_resonance",
     "Per-resonance Delta-phi.  Lighter resonances (rho, omega) give wider "
     "near-side peaks than heavier ones (phi) because the decay opening "
     "angle scales with the kinematics."},

    // ----- shared-parton sharing depth --------------------------------
    {"pair_dphi_sharedparton_depth",
     "Splits SharedParton into 'same hard-process parton' (jet-like, "
     "narrow) and 'shower / MPI-only sharing' (softer, broader).  The two "
     "look dramatically different in Delta-phi width."},

    // ----- MPI relationship -------------------------------------------
    {"pair_dphi_mpi",
     "Pair-level MPI tag.  SameMPI pairs (red) came from ONE secondary "
     "scatter, CrossMPI (orange) from TWO different scatters, OneSideMPI "
     "(blue) from MPI + primary, NoMPI (green) from primary only.  In a "
     "pure-shower run only NoMPI is non-empty; in a +MPI run all four "
     "populate."},
    {"pair_dphi_sharedparton_mpi",
     "THE diagnostic for the MPI-dilution mystery.  When MPI is on the "
     "SharedParton fraction drops from ~40 percent to ~16 percent.  This "
     "plot asks: did that correlation MOVE into the cross-MPI bucket, or "
     "did it actually go away?  If the SameMPI line keeps the original "
     "shape, the correlation survived per-MPI; if CrossMPI grows large, "
     "the partonic correlation is being washed across independent "
     "scatters."},
    {"pair_mass_mpi",
     "Pair mass split by MPI relationship.  Resonance peaks live in "
     "OneSideMPI / NoMPI rather than SameMPI — most resonances decay "
     "inside one MPI scatter or in primary."},

    // ----- shower lineage --------------------------------------------
    {"pair_dphi_shower",
     "Pair-level shower-stage tag.  BothFSR pairs (red) come from a "
     "common FSR shower (jet-like).  MixedShower (orange) mixes ISR "
     "and FSR sides.  BothISR (blue) is rare in soft-QCD events.  "
     "NoShower is empty unless the pair has no shower ancestry at all."},

    // ----- heavy-flavour ---------------------------------------------
    {"pair_dphi_hf",
     "Heavy-flavour decay-chain content of the pair.  Pure-light pairs "
     "(NoHeavyFlavour) dominate.  BothCharm / BothBottom are the "
     "feed-down components — small but distinct in shape because charm / "
     "bottom decays give characteristically wide pion pairs."},

    // ----- multiplicity bins -----------------------------------------
    {"pair_dphi_lowmult",
     "Low-multiplicity events (N < 20 studied hadrons).  Few MPI scatters, "
     "so the SharedParton-from-hard-scattering line dominates over its "
     "MPI counterpart."},
    {"pair_dphi_midmult",
     "Mid-multiplicity (20 to 80 studied hadrons)."},
    {"pair_dphi_highmult",
     "High-multiplicity (>= 80 studied hadrons).  Multi-MPI events shift "
     "the SharedParton component into the cross-MPI line, and the "
     "Unrelated background grows."},

    // ----- decay-chain depth ------------------------------------------
    {"pair_decay_depth_pairclass",
     "Per-pair max decay-chain depth, by pair class.  SameResonance pairs "
     "are mostly at depth 1; long cascades populate the right-hand tail."},

    // ----- single-particle splits ------------------------------------
    {"sp_pt_hf",
     "Pion pT from heavy-flavour cascades vs not.  Charm / bottom feed-down "
     "is small in number but extends pT-coverage upward."},
    {"sp_pt_shower",
     "Pion pT by which shower the ancestry walks through.  FSR-touching "
     "ancestries dominate the bulk; pure-ISR ancestries are rare."},
    {"sp_pt_mpi",
     "Pion pT split by MPI presence.  In a pure-shower run only NoMPI is "
     "non-zero; once MPI is on the FromMPI fraction grows with pT."},

    // ----- fraction (local class-composition) plots -------------------
    {"pair_dphi_fraction",
     "Per-bin class fraction relative to all pairs.  Near Delta-phi = 0 the "
     "SameResonance fraction can be much larger than its global percentage "
     "even though it's a tiny global slice."},
    {"pair_deta_fraction",
     "Per-bin class composition vs Delta-eta.  Useful for seeing where in "
     "phase space each class dominates."},
    {"origin_pt_fraction",
     "Per-bin origin composition vs pT.  Feed-down dominates at low pT, "
     "primary at high pT."},
    {"parton_pt_fraction",
     "Per-bin parton-flavour composition vs pT.  Gluon-ancestry pions "
     "become a meaningful fraction once MPI is on."},

    // ----- ladder figures -------------------------------------------
    {"ladder_dphi_pair_SharedParton",
     "Same plot, four rungs of Pythia mechanism.  Look for shape changes "
     "and overall normalization shifts between rungs.  Big shift between "
     "shower and shower+MPI is the SharedParton-dilution effect."},
    {"ladder_dphi_pair_Unrelated",
     "Unrelated correlation across the ladder.  Should stay roughly flat "
     "but grow in normalization as MPI multiplies the combinatorial pair "
     "count."},
    {"ladder_pt_origin_FromResonance",
     "Resonance feed-down across the ladder.  Each mechanism shifts the "
     "available resonance population slightly."},
    {"ladder_pt_parton_Strange",
     "Strange-ancestry pT spectrum across the ladder.  Shoots up between "
     "shower and shower+MPI because MPI gives sea-strange quarks new "
     "accessible scatters."},
    {"ladder_dphi_pair_MPI_SameMPI",
     "Same-MPI pair correlation across the ladder.  Empty for pure shower, "
     "grows for every later rung."},
    {"ladder_dphi_pair_MPI_CrossMPI",
     "Cross-MPI pair correlation across the ladder.  Tells you how much "
     "of the combinatorial floor is accidental MPI overlap."},
    {"ladder_dphi_pair_SharedParton_SameMPI",
     "SharedParton pairs whose parton ancestry lives inside ONE MPI.  Use "
     "this to answer 'is the partonic correlation surviving inside each "
     "MPI scatter, or is it being spread across them?'"},
    {"ladder_pt_HF_FromBottomChain",
     "Bottom-chain pion pT across the ladder."},
    {"ladder_pt_HF_FromCharmChain",
     "Charm-chain pion pT across the ladder."},
    {"ladder_event_multiplicity",
     "Event multiplicity across the ladder.  MPI / CR / rope each push "
     "the average multiplicity up; the size of the shift quantifies the "
     "mechanism's impact."},

    // ----- flavor-mixing decompositions --------------------------------
    {"pair_dphi_flavmix",
     "Pair correlation split by joint hard-parton flavour.  Same-quark "
     "(red): both partons share |PDG|, the cleanest 'same vertex' case.  "
     "Same-generation (orange): different flavour within u/d, c/s, or "
     "t/b.  Cross-generation (blue): different generations such as "
     "u-s.  With-gluon (green) dominates in soft QCD once MPI is on "
     "(MPI scatters are mostly gg).  No-parton (grey) is the MPI-only "
     "/ shower-only / beam-remnant tail.  Reading these together "
     "answers 'how much flavour mixing do we have'."},
    {"pair_deta_flavmix",
     "Pseudorapidity-gap counterpart of pair_dphi_flavmix.  Same-quark "
     "and same-generation pairs concentrate near Delta-eta = 0 because "
     "they typically come from one shower; cross-generation pairs are "
     "broader because they mix scatters."},
    {"pair_mass_flavmix",
     "Pair mass by flavour.  Same-quark pairs tend to be light (pion-"
     "pair mass scale); with-gluon and cross-generation extend higher "
     "in mass.  Validation: same-quark + same-gen together should "
     "account for the majority of resonance-peak pairs."},
    {"pair_dphi_sharedparton_flavmix",
     "Same axis but restricted to pairs that ALREADY share a pre-"
     "hadronization parton.  The genuinely-partonic correlation, "
     "decomposed by what flavour content drives it.  This is the "
     "direct answer to 'how much of the pair correlation comes from "
     "flavour content vs from kinematics alone'."},
    {"pair_deta_sharedparton_flavmix",
     "SharedParton Delta-eta by flavour mixing."},
    {"pair_mass_sharedparton_flavmix",
     "SharedParton pair mass by flavour mixing.  Same-quark peaks at "
     "the resonance masses; with-gluon is the broad continuum."},

    // ----- event-shape (spherocity) binned correlations -----------------
    {"pair_dphi_jetlike",
     "Pair correlation in jet-like events (low transverse spherocity).  "
     "Look for an enhanced SharedParton near-side peak — these are the "
     "events where one or two hard partons dominate the topology."},
    {"pair_dphi_midshape",
     "Mid-shape events.  Mixed-topology behaviour; useful as a sanity "
     "check between the JetLike and Isotropic extremes."},
    {"pair_dphi_isotropic",
     "Pair correlation in isotropic events (high transverse spherocity).  "
     "Many soft scatters, no leading jet — the partonic correlation is "
     "diluted and the Unrelated combinatorial floor dominates."},
    {"pair_deta_jetlike",
     "Delta-eta in jet-like events.  Narrow same-side peak from "
     "shared-parton fragmentation."},
    {"pair_deta_midshape",
     "Delta-eta in mid-shape events."},
    {"pair_deta_isotropic",
     "Delta-eta in isotropic events.  The combinatorial floor is "
     "essentially flat in Delta-eta."},
    {"event_spherocity",
     "Per-event transverse spherocity distribution.  Pencil-like (jet) "
     "events live near 0, isotropic events near 1.  Use this to "
     "validate that the bin thresholds split the sample roughly evenly "
     "(or in the proportions you intended)."},
  };
  return M;
