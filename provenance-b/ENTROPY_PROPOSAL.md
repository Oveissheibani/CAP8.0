# Entropy of the collision — a provenance-based study

Proposal for extending the provenance module to entropy / information-theoretic
observables. Drafted June 2026.

## 1. Why this is timely

Three active research lines, none of which has what we have (the full per-event
ancestry DAG with stage labels and parton tags in two generators):

1. **Kharzeev–Levin (KL) entanglement entropy.** Conjecture: the entanglement
   entropy of the partonic state equals the Shannon entropy of the final hadron
   multiplicity distribution, S = −Σ P(N) ln P(N) ≈ ln(xG(x)).
   Confirmed against H1 DIS data (Hentschinski & Kutak, EPJC 2022,
   arXiv:2110.06156, 2207.09430). Extended to pp at the LHC via maximum-entropy
   multiplicity distributions (arXiv:2505.23491) and "scaling entropy"
   universality (arXiv:2506.09899). High-multiplicity tails still disagree with
   generators (arXiv:2605.21052) — an open tension we can probe.

2. **Entropy inside Monte Carlo generators.** Hentschinski, Jung & Kutak
   (arXiv:2509.03400, Sep 2025) computed entropy production in DIS MC and found
   soft gluons dominate; entropy is mostly an *initial-state* effect there.
   They did DIS, initial-state focused, single generator family. Nobody has done
   the *stage-resolved* entropy budget of a full pp event, and nobody has
   compared string vs cluster hadronization as entropy producers.

3. **Entanglement in the Lund string.** Berges, Flörchinger & Venugopalan
   (arXiv:1812.08120) showed the expanding string has entanglement entropy
   extensive in rapidity, with a thermal reduced density matrix ("entanglement
   temperature"). Theory only — no event-record-level study.

4. **Mutual information in QCD.** Larkoski, Metodiev & Thaler
   (arXiv:1408.3122) used mutual information for quark/gluon *jet* tagging.
   Nobody has applied it at the soft-QCD event level to ask how much
   initial-state information survives hadronization.

**The gap we fill:** experiments see only the rim of the onion; theory papers
see only the initial state. Our EventHistory DAG sees every layer of every
event, in Pythia (strings) and Herwig (clusters), with mechanism toggles
(MPI, CR). That is exactly the ground truth needed for information-flow
questions.

## 2. Honest framing (important for the write-up)

Generators are classical. We never compute a true von Neumann entropy. We
compute exact **Shannon entropies and mutual informations** on generator
events, and we **test the KL duality** (does the classical multiplicity
entropy behave like the predicted entanglement entropy?). Keep the two
vocabularies separated; reviewers will check.

## 3. The observables, phased

### Phase 1 — post-processing only (days)

No C++ changes; scripts over existing summaries / dumps.

- **S_mult(Δy, √s)** = −Σ P(N) ln P(N) in rapidity windows. The KL test,
  in both generators, vs window size and energy. Compare to ln of expected
  parton number; check the S ∝ ln W (∝ ln 1/x) growth.
- **Forward–backward mutual information** I(N_F : N_B) from per-event
  hemisphere multiplicities. Classical proxy for rapidity-interval
  entanglement; string vs cluster should differ.
- **KL divergence between generators**: D_KL(Pythia‖Herwig) on the
  provenance-class and initiating-parton distributions we already publish —
  a single number per observable quantifying model distance.

Inputs: per-event multiplicity (already histogrammed) + the provenance-study
JSON dump (already has per-particle y, pT, tags).

### Phase 2 — entropy production along the onion (1–2 weeks)

Small accumulator beside ProvenanceObservables; the DAG walk exists.

- For each Stage ring (Hard → MPI → shower → PartonsPreHadronization →
  PrimaryHadrons → DecayProducts → FinalState): count objects and compute a
  coarse-grained momentum-space Shannon entropy (bin (y, pT), S = −Σ p ln p).
- Output: **entropy-vs-stage profile** per run. Where is entropy made —
  shower, hadronization, or decays?
- Mechanism ladder makes it surgical: MPI on/off → the MPI entropy jump;
  CR on/off → does colour reconnection *reduce* entropy? String vs cluster →
  two different entropy machines, same axes.
- Implementation: one pass over EventHistory nodes grouped by stage; a
  `stage_entropy` Hist1D per run; report section like the existing genealogy
  tables.

### Phase 3 — information recovery (the novel one)

Hadronization as a **noisy channel**: input = partonic state (we hold the
exact tags), output = final hadrons.

- **I(initiating parton flavour ; final hadron observables)** — bits of
  gluon-vs-quark origin surviving to the rim. Both variables already in the
  tag. Compare the channel "Lund string" vs the channel "cluster model":
  which hadronization destroys more information?
- **Before/after decay layer**: same mutual information computed at
  PrimaryHadrons vs FinalState — how much the resonance/weak feed-down
  scrambles on top of hadronization.
- **Operational bound**: train a simple classifier (final-state-only inputs →
  initiating parton); its accuracy lower-bounds recoverable information.
  Connects to jet tagging literature but at the soft-QCD event level — fresh.
- **I(N_MPI ; N_final)** in Pythia: recoverability of MPI activity from the
  final state (relevant to centrality estimators in small systems).

### Phase 4 — physics extensions (later)

- **Per-class thermodynamics**: Tsallis/exponential fits per provenance class.
  Are primary hadrons more "thermal" than feed-down? Experimentally
  inseparable; trivial for us. Connects to the Berges et al. "entanglement
  temperature".
- High-multiplicity tail: where generators disagree with data
  (arXiv:2605.21052), use the stage profile to localize *which* stage drives
  the tail.

## 4. Deliverable shape

Each phase is a self-contained result and every one is a Pythia-vs-Herwig
comparison for free (the compare-report machinery already exists). Phase 1+2
make a methods paper; phase 3 is the headline ("how much information survives
hadronization — string vs cluster").

## 5. Estimator caveats

- Plug-in Shannon entropy is biased low at finite N_events; use
  Miller–Madow correction or bootstrap. Matters for I(·;·) especially.
- Mutual information with continuous variables: discretize (k-NN estimators
  later if needed); report binning sensitivity.
- Multiplicity entropy needs the *distribution* P(N), so per-event N must be
  dumped, not just the mean — phase 1 uses the JSON dump path.

## References

- KL duality + H1: arXiv:2110.06156, arXiv:2207.09430
- Entropy in MC (DIS): arXiv:2509.03400
- pp multiplicity entropy: arXiv:2505.23491, arXiv:2506.09899, arXiv:2605.21052
- String entanglement: arXiv:1812.08120
- Mutual information in QCD: arXiv:1408.3122
- Dipole-cascade entropy: arXiv:2509.07898
