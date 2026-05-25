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
  int   leadPartonPdg = 0;        // flavour of the first parton ancestor
  int   hardPartonPdg = 0;        // ancestor flavour at the hard process
  Stage deepestStage  = Stage::Unknown;    // earliest stage reached

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
  // purpose of correlation-contamination studies?  Phase-3a uses a
  // curated list of the common contaminators (rho, omega, phi, K*,
  // Delta, ...); a later phase can defer to the CAP ParticleDb width /
  // lifetime instead of a hard-coded list.
  static bool isResonance(int pdg);
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

} // namespace CAP

#endif // CAP__ProvenanceTagger
