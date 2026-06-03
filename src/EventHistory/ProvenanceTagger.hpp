/* **********************************************************************
 * CAP::ProvenanceTagger  —  trace a final hadron back to its origin
 *
 * Part of the parton-tracking feature (Phase 3a).
 *
 * A ProvenanceTag is the complete origin record of one final-state
 * hadron.  Read together, the fields answer the physics questions the
 * feature exists for:
 *
 *   before hadronization : which partons does this hadron descend from,
 *                          and how far back does the chain reach?
 *                          -> partonAncestorIndices, leadPartonPdg,
 *                             hardPartonPdg, deepestStage
 *   at hadronization     : was the hadron formed directly from a
 *                          string / cluster?            -> isPrimaryHadron
 *   after hadronization  : was it instead produced in a decay, of what
 *                          parent, and was that parent a short-lived
 *                          resonance?  -> isFromDecay, parentHadronPdg,
 *                                         isFromResonance, resonancePdg
 *
 * The two free pair-helpers (sharesDecayParent / sharesPartonAncestor)
 * are what an analysis uses to split a correlation into the part that
 * is genuinely ancestry-driven and the part that is not.
 *
 * Pure C++14 — depends only on EventHistory.  No ROOT, no generators.
 * ********************************************************************/
#ifndef CAP__ProvenanceTagger
#define CAP__ProvenanceTagger

#include <vector>

#include "EventHistory.hpp"
#include "StageTaxonomy.hpp"

namespace CAP
{

// ----------------------------------------------------------------------
//  The origin record of one final-state hadron.
// ----------------------------------------------------------------------
struct ProvenanceTag
{
  int   finalIndex = -1;          // node index of the final-state hadron
  int   finalPdg   = 0;

  // --- hadronization vs decay ---
  Stage productionStage = Stage::Unknown;
  bool  isPrimaryHadron = false;  // formed directly at hadronization
  bool  isFromDecay     = false;  // produced in a subsequent decay

  // --- immediate decay parent (valid only when isFromDecay) ---
  int   parentHadronIndex = -1;   // node index of the parent hadron
  int   parentHadronPdg   = 0;
  bool  isFromResonance   = false;// parent is a short-lived resonance
  int   resonancePdg      = 0;    // == parentHadronPdg when isFromResonance

  // --- partonic ancestry ---
  std::vector<int> partonAncestorIndices;  // pre-hadronization partons
  int   leadPartonPdg     = 0;    // flavour of the first parton ancestor
  int   hardPartonPdg     = 0;    // ancestor flavour at the hard process
                                  // (Pythia status-code path only)
  // Generator-AGNOSTIC initiating parton: flavour of the TOPMOST parton in
  // the lineage — the parton whose own parents are NOT partons (beam / hard
  // vertex).  For the Pythia path this is the incoming hard-process parton;
  // for the HepMC path (Herwig), which never tags a HardProcess stage, it is
  // the parton at the top of the shower.  Unlike leadPartonPdg (the Lund
  // string ENDPOINT, which is a quark by construction and never a gluon) this
  // CAN be a gluon, and answers the physics question "did this hadron
  // originate from a quark or a GLUON?".  Works for BOTH generators.
  int   initiatingPartonPdg = 0;
  int   hardPartonIndex   = -1;   // node index of the hard-process ancestor
                                  // (allows "same hard parton" check at the
                                  // pair level: two hadrons sharing the same
                                  // hard parton came from one hard scatter)
  Stage deepestStage      = Stage::Unknown;    // earliest stage reached

  // --- MPI ancestry (Phase 3e) ---
  // Which multi-parton-interaction scatter does this hadron descend from?
  // mpiIndex is the node index of the EARLIEST MPI-stage parton in the
  // ancestry; -1 if the hadron has no MPI ancestor (came from the primary
  // hard scatter or beam remnants).  Two hadrons sharing mpiIndex came
  // from the same secondary MPI scatter — useful for separating
  // "correlation inherited from one MPI scatter" from "correlation across
  // independent MPI scatters".
  int   mpiIndex          = -1;   // node index of the MPI-stage ancestor

  // --- shower-origin tag (Phase 3e) ---
  // Did the deepest reachable pre-hadronization ancestor sit in the ISR
  // shower, the FSR shower, both, or neither?  Distinguishes hadrons
  // whose ancestry traces through initial-state radiation from those
  // whose ancestry traces through final-state radiation.
  bool  fromISR           = false;
  bool  fromFSR           = false;

  // --- heavy-flavour decay-chain tag (Phase 3e) ---
  // Walks up the hadronic parent chain looking for a charm or bottom
  // hadron.  fromCharmChain is true if any hadronic ancestor (including
  // the immediate decay parent) carries a charmed-hadron PDG; similarly
  // for fromBottomChain.  These overlap with isFromDecay / isFromResonance
  // but cleanly identify the "feed-down from heavy flavour" component
  // that would otherwise be hidden inside FromWeakDecay or FromResonance.
  bool  fromCharmChain    = false;
  bool  fromBottomChain   = false;

  // --- decay-chain depth (Phase 3e) ---
  // Number of hadronic decay steps from this hadron back to its first
  // non-hadronic (partonic) ancestor.  0 for a primary hadron, 1 for the
  // direct decay product of a primary, etc.  Caps at a reasonable depth
  // to avoid runaway walks on malformed graphs.
  int   decayChainDepth   = 0;

  bool hasPartonicOrigin() const { return !partonAncestorIndices.empty(); }
};

// ----------------------------------------------------------------------
//  Computes ProvenanceTags from a filled EventHistory.
// ----------------------------------------------------------------------
class ProvenanceTagger
{
public:

  // Tag one node — typically a final-state hadron, but any node works.
  ProvenanceTag tag(const EventHistory & history, int nodeIndex) const;

  // Tag every final-state node in the event.
  std::vector<ProvenanceTag> tagFinalState(const EventHistory & history) const;

  // Is `pdg` a short-lived (strong / EM-decaying) resonance, for the
  // purpose of correlation-contamination studies?  Two-tier resolution:
  //
  //   1. If a runtime resolver has been installed via
  //      setResonanceResolver() — typically by provenance-study at
  //      startup, querying Pythia's particle database — that resolver is
  //      consulted FIRST.  This is the data-driven path: classify a PDG
  //      as a resonance iff its width is non-trivial (i.e. cτ < user-
  //      chosen threshold).
  //
  //   2. Otherwise fall back to the curated PDG list — the original
  //      Phase-3a behaviour, which works without Pythia / a ParticleDb
  //      linked (e.g. HepMC3 paths).
  //
  // Both calls take an absolute PDG since Pythia and the curated list
  // are antiparticle-symmetric.
  static bool isResonance(int pdg);

  // Install / clear a runtime resolver.  The resolver returns 1 for
  // "yes, resonance", 0 for "no", and -1 for "don't know — fall back".
  // The function pointer is global by design: ProvenanceTagger is a
  // value type and tagger instances are cheap; resolution is global.
  using ResonanceResolver = int (*)(int pdg);
  static void setResonanceResolver(ResonanceResolver fn);
};

// ----------------------------------------------------------------------
//  Pair-level helpers — the basis of "how much of the correlation is
//  ancestry-driven".  Built directly from two tags.
// ----------------------------------------------------------------------

// True when both final hadrons came from the SAME decaying parent —
// e.g. the two pions of one rho.  This is the textbook resonance
// contamination of a same-event two-particle correlation.
bool sharesDecayParent(const ProvenanceTag & a, const ProvenanceTag & b);

// True when the two final hadrons share at least one pre-hadronization
// parton ancestor — i.e. their correlation is, at least in part,
// inherited from the partonic stage.
bool sharesPartonAncestor(const ProvenanceTag & a, const ProvenanceTag & b);

// True when both hadrons descend from the SAME hard-process parton —
// the deepest possible ancestry sharing.  Distinguishes hard-scattering
// (jet-like) sharing from soft / shower / MPI-only sharing.
bool sharesHardProcessAncestor(const ProvenanceTag & a, const ProvenanceTag & b);

// True when both hadrons descend from the SAME multi-parton-interaction
// scatter.  When false but both carry mpiIndex >= 0, the pair was made
// of fragments from TWO different MPI scatters (cross-MPI).
bool sharesMPIVertex(const ProvenanceTag & a, const ProvenanceTag & b);

} // namespace CAP

#endif // CAP__ProvenanceTagger
