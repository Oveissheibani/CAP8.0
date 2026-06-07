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
    // ----- hadronization interface (string vs cluster) ------------------
    {"had_npartons",
     "The INPUT side of hadronization: how many partons reach the "
     "string/cluster stage per event.  Mostly an MPI+shower statement -- "
     "more scatters and more radiation mean more partons to hadronize."},
    {"had_nprimary",
     "The OUTPUT side of hadronization: primary hadrons per event, before "
     "any decay.  Compare against had_npartons mentally: the gap between "
     "the two IS the fragmentation multiplicity."},
    {"had_ratio",
     "Read the peak position: it is the average number of primary hadrons "
     "each incoming parton turns into.  This is the single most direct "
     "fingerprint of the fragmentation mechanism."},
    {"compare_had_npartons",
     "Expect a REAL difference here: Herwig's cluster model force-splits "
     "every gluon into a quark-antiquark pair before hadronization, so it "
     "feeds more, lighter objects into the hadronization step than "
     "Pythia's string picture.  This plot makes that model difference "
     "directly visible."},
    {"compare_had_nprimary",
     "Primary-hadron multiplicity head-to-head, decay-free.  The string "
     "typically yields more primary hadrons per event; differences here "
     "propagate into every multiplicity observable downstream."},
    {"compare_had_ratio",
     "THE string-vs-cluster plot: hadrons produced per parton entering "
     "hadronization.  One long string fragments into many hadrons (high "
     "ratio, broad); one cluster decays into about two (ratio clustered "
     "low).  If you show one hadronization comparison, show this one."},
    {"compare_mult_origin_Primary",
     "Same comparison restricted to the studied species inside the "
     "acceptance window -- the version of the primary-multiplicity "
     "difference that feeds the visible final state."},
    // ----- fragmentation systems (opt-in --systems runs) ----------------
    {"frag_system_mass",
     "THE model-assumption plot.  The cluster model is BUILT on low "
     "cluster masses (peak at 1-3 GeV); the string picture allows heavy "
     "systems.  Both axes are the same by construction (primary-hadron "
     "sum), so any difference is the model, not the bookkeeping."},
    {"frag_nhad_vs_mass",
     "The SCALING test, stronger than any shape: Lund predicts a straight "
     "ln(m^2) rise of hadrons-per-system; clusters stay near 2 at every "
     "mass because heavy clusters fission first.  If the two curves "
     "separate the way the models say, the system reconstruction is doing "
     "its job."},
    {"frag_charge_ordering",
     "Compare the BLUE (OS same-system) curve against the others at small "
     "Delta-y.  An OS excess that exists ONLY within systems is local "
     "charge conservation in the breaks -- the microscopic origin of "
     "balance functions.  Cross-system curves are the control: no "
     "constraint, no excess."},
    {"frag_lambda",
     "Watch this one across the CR ladder rung: colour reconnection is "
     "DEFINED as the mechanism that minimises total string length, so "
     "lambda must drop when CR turns on.  If it does not, the CR "
     "implementation is not doing what its name claims."},
    {"compare_frag_system_mass",
     "One axis, two hadronization philosophies.  Read the peak position "
     "(cluster mass scale vs string masses) and the tail (how much heavy-"
     "system phase space each model populates)."},
    {"compare_frag_nhad_per_system",
     "The cluster ~2-hadron peak against the string's broad fragmentation "
     "multiplicity -- the single most direct picture of how differently "
     "the two models convert partons into hadrons."},
    {"compare_frag_y_span",
     "Each model's long-range budget: how far one system's hadrons reach "
     "in rapidity.  Long spans seed long-range correlations; if the "
     "generators differ here they MUST differ in forward-backward "
     "correlations too."},
    // ----- per-stage kinematics + column graphs --------------------------
    {"frag_stage_y",
     "The anatomy of one collision in a single plot: how each evolution "
     "stage populates rapidity.  Watch the spread grow from the hard "
     "scatter outward; the gap between PreHadronization and "
     "PrimaryHadrons is what hadronization does to the rapidity "
     "structure."},
    {"frag_stage_dphi",
     "Azimuthal correlations stage by stage.  Partonic rings carry the "
     "back-to-back memory of the scatters; watch it dilute through the "
     "shower and survive (or not) into the primary hadrons."},
    {"compare_stagekin_y_PrimaryHadrons",
     "In the mechanism ladder this overlay is THE colour-reconnection "
     "plot: CR creates no particles, so its entire action is the change "
     "of this curve between the +MPI and +MPI+CR shades of one colour "
     "family."},
    {"compare_stagekin_dphi_PrimaryHadrons",
     "Same CR logic in azimuth: compare the +MPI and +MPI+CR shades "
     "within one colour family -- any reshaping is colour reconnection "
     "rearranging the strings/clusters before they decay."},
    {"compare_bars_origin",
     "The origin table as columns: each class is a group, each bar one "
     "configuration.  Faster to read than the table for spotting which "
     "configuration moves which class."},
    {"compare_bars_initparton",
     "The gluon-vs-quark origin table as columns.  The gluon group is "
     "the one to watch across configurations."},
    {"compare_bars_pair",
     "Pair-ancestry fractions as columns (log scale -- the classes span "
     "orders of magnitude).  SharedParton is the physics-rich group."},
    // ----- event multiplicity / centrality (three concepts) -------------
    {"nmult_hadrons",
     "The TOTAL hadronic yield: charged and neutral, all species.  Compare "
     "mentally against nmult_charged (about 2/3 of hadrons are charged) "
     "and event_multiplicity (one species, windowed) to keep the three "
     "concepts apart."},
    {"nmult_charged",
     "What experiments can actually count.  The ratio to nmult_hadrons is "
     "set by isospin (roughly 2/3) and strange/neutral feed-down."},
    {"nmult_charged_eta10",
     "The standard quoted N_ch: central tracker window.  This is the axis "
     "to compare against published dN_ch/deta numbers."},
    {"nmult_charged_fwd",
     "The CENTRALITY ESTIMATOR axis.  Slice this distribution into "
     "percentiles to define centrality classes; it is measured FORWARD "
     "precisely so the class definition does not auto-correlate with "
     "central observables."},
    {"nmult_nmpi",
     "Model-truth activity: how many MPI scatters actually happened "
     "(distinct MPI ancestors of the studied hadrons).  Experiments can "
     "never see this directly -- only its estimator (forward N_ch).  "
     "Pythia only; zero by construction for HepMC input."},
    {"nmult_nch_vs_nmpi",
     "Read the slope and the spread: a steep, tight, linear rise means "
     "central N_ch is a faithful centrality estimator; flattening at "
     "high N_MPI is saturation -- the multiplicity stops resolving the "
     "underlying activity.  This is the model-level justification (or "
     "indictment) of multiplicity-based centrality in pp."},
    // ----- entropy / information figures (opt-in --entropy runs) --------
    {"entropy_mult",
     "Each curve is the charged-multiplicity distribution P(N) in one "
     "pseudorapidity window.  WIDTH is the message: the Shannon entropy "
     "quoted in the entropy table is large when the distribution is wide "
     "and flat, small when it is narrow.  Watch the entropy grow as the "
     "window opens — the analogue of the entanglement-entropy growth with "
     "the size of the probed region in the Kharzeev-Levin picture."},
    {"entropy_fb",
     "Forward and backward hemisphere multiplicities.  By symmetry the two "
     "curves should lie on top of each other; the physics is NOT in either "
     "curve alone but in their joint distribution — the mutual information "
     "I(NF;NB) in the information-recovery table.  I > 0 means knowing the "
     "forward multiplicity tells you about the backward one (long-range "
     "correlation; the classical proxy for rapidity-interval entanglement)."},
    {"entropy_stage_S",
     "One point per evolution stage, in time order.  The VALUE is how "
     "spread out that stage's particles are over (eta, pT); the STEP from "
     "one stage to the next is the phase-space entropy produced by that "
     "step of the event evolution.  Expect the biggest steps at the shower "
     "and at hadronization; decays mostly reshuffle."},
    {"entropy_stage_meanN",
     "Mean object count per stage — the 'how many' half of the entropy "
     "budget (the profile plot is the 'where' half).  Hadronization is "
     "the big multiplier: one string/cluster system becomes many hadrons."},
    {"entropy_stage_total",
     "Occupancy entropy plus ln(mean N) — a coarse total-entropy proxy "
     "combining spread and count.  Read the jump between consecutive "
     "stages as the entropy produced by that step; this is the single "
     "plot to show for 'where is the entropy of the collision made'."},
    {"compare_ent_mult_eta05",
     "Same multiplicity distribution, both generators.  The wider "
     "distribution carries the larger Shannon entropy (see the comparison "
     "entropy table for the numbers).  Differences here are differences "
     "in how the string and cluster models populate the central region."},
    {"compare_ent_mult_full",
     "Full-acceptance multiplicity distribution, both generators.  The "
     "entropy difference between generators is dominated by the width "
     "difference visible here."},
    {"compare_ent_stage_S",
     "Entropy-production profile, both generators, NOT normalized — these "
     "are entropy values, not distributions.  Where the curves separate is "
     "where the two models produce entropy differently: a string-vs-cluster "
     "difference at PrimaryHadrons is a hadronization-model statement, a "
     "difference at ISR/FSR is a shower statement."},
    {"compare_ent_stage_meanN",
     "Mean object count per stage for both generators.  The PrimaryHadrons "
     "step shows directly which hadronization model fragments into more "
     "pieces; FinalState shows the net multiplicity after decays."},
    {"compare_ent_stage_total",
     "Total-entropy proxy per stage (occupancy + ln mean count), both "
     "generators.  The single most compact comparison of WHERE each "
     "generator makes its entropy."},
    {"entropy_kno",
     "The KNO test: if the rescaled curves from the different eta windows "
     "collapse onto one shape, multiplicity fluctuations are scale-"
     "invariant and the entropy obeys S = ln<N> + const.  A collapse here "
     "plus S/ln<N> near constant in the entropy table is the internal "
     "consistency check of the Kharzeev-Levin reading."},
    {"entropy_fb_mi_gap",
     "Read the trend, not single points: I at gap 0 is ALL the shared "
     "forward/backward information; as the gap removes the central region, "
     "what survives is genuinely LONG-RANGE shared information.  A slow "
     "decay means long-range correlation (string-like); a fast decay means "
     "the information is short-range.  Always compare each point against "
     "its bias estimate in the table before quoting it."},
    {"entropy_nmpi",
     "Sanity distribution for the MPI-activity recovery: how many distinct "
     "MPI scatters contribute tagged hadrons per event.  In Herwig/HepMC "
     "input this is identically zero (no MPI tags in the graph), so "
     "I(N_MPI;N_ch) is meaningful for Pythia only."},
    {"compare_ent_fb_mi_gap",
     "The headline information-flow comparison: which hadronization model "
     "carries more forward/backward shared information, and how fast each "
     "loses it with rapidity separation.  Differences at LARGE gap are the "
     "interesting ones -- they cannot come from short-range decay "
     "kinematics."},
  };
  return M;
