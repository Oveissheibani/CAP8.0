/* **********************************************************************
 * CAP::ProvenanceTagger — implementation.  See header for design.
 * ********************************************************************/
#include "ProvenanceTagger.hpp"

#include <cstdlib>   // std::abs

namespace CAP
{

namespace
{
// Cap on the parent-chain walk length — guards against malformed graphs.
const int MAX_DECAY_DEPTH = 64;

// |pdg| ranges for charmed and bottom hadrons.  Conservative ranges that
// cover the standard PDG numbering scheme: meson 4xx / 5xx (3 digit) and
// baryon 4xxx / 5xxx (4 digit), excluding pure-quark codes.
bool isCharmHadron(int pdg)
{
  const int a = std::abs(pdg);
  // Charm mesons: 411..445, charm baryons: 4112..4444.
  return (a >= 411  && a <= 499) ||
         (a >= 4112 && a <= 4999);
}

bool isBottomHadron(int pdg)
{
  const int a = std::abs(pdg);
  // Bottom mesons: 511..555, bottom baryons: 5112..5554.
  return (a >= 511  && a <= 599) ||
         (a >= 5112 && a <= 5999);
}
} // namespace

// ----------------------------------------------------------------------
// Runtime resolver — installed by callers that have access to a live
// particle database (Pythia, HepMC3-with-PDT, CAP ParticleDb).  When
// non-null and it returns 0/1, that decision wins; -1 means "I don't
// know, please fall back".
// ----------------------------------------------------------------------
static ProvenanceTagger::ResonanceResolver s_resolver = nullptr;

void ProvenanceTagger::setResonanceResolver(
    ProvenanceTagger::ResonanceResolver fn)
{
  s_resolver = fn;
}

// ----------------------------------------------------------------------
//  Curated list of the common strong / EM-decaying resonances that
//  contaminate same-event correlations.  Compared on |pdg|.
//
//  This is the FALLBACK list used when no runtime resolver is installed
//  (e.g. tagging a HepMC3 file without a particle DB).  When Pythia is
//  linked, provenance-study installs a resolver at startup that queries
//  the Pythia particle database for the correct answer (cτ-based).
// ----------------------------------------------------------------------
bool ProvenanceTagger::isResonance(int pdg)
{
  if (s_resolver)
    {
    const int r = s_resolver(pdg);
    if (r == 0 || r == 1) return r == 1;
    // r == -1 falls through to the curated list below.
    }
  switch (std::abs(pdg))
    {
    // light unflavoured mesons
    case 113:  case 213:               // rho(770)
    case 223:                          // omega(782)
    case 333:                          // phi(1020)
    case 221:                          // eta
    case 331:                          // eta'
    case 9000221: case 9010221:        // f0(500)/sigma, f0(980)
    case 115:  case 215:               // a2(1320)
    // strange mesons
    case 313:  case 323:               // K*(892)
    case 315:  case 325:               // K*2(1430)
    // heavy-flavour vector mesons
    case 413:  case 423:  case 433:    // D*
    case 513:  case 523:  case 533:    // B*
    // baryon resonances
    case 1114: case 2114:
    case 2214: case 2224:              // Delta(1232)
    case 3114: case 3214: case 3224:   // Sigma*(1385)
    case 3314: case 3324:              // Xi*(1530)
      return true;
    default:
      return false;
    }
}

// ----------------------------------------------------------------------
ProvenanceTag ProvenanceTagger::tag(const EventHistory & h, int idx) const
{
  ProvenanceTag t;
  if (idx < 0 || idx >= h.size()) return t;

  const ParticleNode & node = h.node(idx);
  t.finalIndex      = idx;
  t.finalPdg        = node.pdg;
  t.productionStage = node.stage;
  t.isPrimaryHadron = (node.stage == Stage::PrimaryHadrons);
  t.isFromDecay     = (node.stage == Stage::DecayProducts);

  // Immediate decay parent: the first non-parton (hadronic) parent.
  if (t.isFromDecay)
    {
    for (int p : node.parents)
      {
      if (p < 0 || p >= h.size()) continue;
      const ParticleNode & parent = h.node(p);
      if (!parent.isParton())
        {
        t.parentHadronIndex = p;
        t.parentHadronPdg   = parent.pdg;
        t.isFromResonance   = isResonance(parent.pdg);
        t.resonancePdg      = t.isFromResonance ? parent.pdg : 0;
        break;
        }
      }
    }

  // Partonic ancestry — walk the DAG up to the requested stages.
  t.partonAncestorIndices =
    h.ancestorsAtStage(idx, Stage::PartonsPreHadronization);
  if (!t.partonAncestorIndices.empty())
    t.leadPartonPdg = h.node(t.partonAncestorIndices.front()).pdg;

  const std::vector<int> hard = h.ancestorsAtStage(idx, Stage::HardProcess);
  if (!hard.empty())
    {
    t.hardPartonPdg   = h.node(hard.front()).pdg;
    t.hardPartonIndex = hard.front();
    }

  // Generator-AGNOSTIC initiating parton: the TOPMOST parton in the lineage,
  // i.e. the first parton (walking up) whose own parents are NOT partons
  // (they are beams / the hard vertex).  For the Pythia path this lands on
  // the incoming hard-process parton; for the HepMC path (Herwig), which
  // tags no HardProcess stage, it lands on the parton at the top of the
  // shower.  Either way it CAN be a gluon — the quark-vs-gluon origin that
  // the string-endpoint leadPartonPdg can never express.
  {
  const int nN = h.size();
  std::vector<char> seen(static_cast<size_t>(nN), 0);
  if (idx >= 0 && idx < nN) seen[static_cast<size_t>(idx)] = 1;
  std::vector<int> frontier = node.parents;
  bool found = false;
  while (!frontier.empty() && !found)
    {
    std::vector<int> next;
    for (int p : frontier)
      {
      if (p < 0 || p >= nN || seen[static_cast<size_t>(p)]) continue;
      seen[static_cast<size_t>(p)] = 1;
      const ParticleNode & pn = h.node(p);
      if (pn.isParton())
        {
        bool hasPartonParent = false;
        for (int gp : pn.parents)
          if (gp >= 0 && gp < nN && h.node(gp).isParton())
            { hasPartonParent = true; break; }
        if (!hasPartonParent)            // topmost parton on this branch
          {
          t.initiatingPartonPdg = pn.pdg;
          found = true;
          break;
          }
        }
      for (int gp : pn.parents) next.push_back(gp);
      }
    frontier.swap(next);
    }
  }

  // How far back the chain reaches.
  if (!hard.empty())
    t.deepestStage = Stage::HardProcess;
  else if (!t.partonAncestorIndices.empty())
    t.deepestStage = Stage::PartonsPreHadronization;
  else
    t.deepestStage = node.stage;

  // ---- MPI ancestry --------------------------------------------------
  // Earliest MPI-stage ancestor: a parton emitted by a secondary
  // multi-parton-interaction scatter.  Pair-level "did these two hadrons
  // come from the same MPI?" reduces to comparing this index.
  const std::vector<int> mpiAncestors = h.ancestorsAtStage(idx, Stage::MPI);
  if (!mpiAncestors.empty())
    t.mpiIndex = mpiAncestors.front();

  // ---- ISR / FSR origin ---------------------------------------------
  // Did this hadron's ancestry walk through any ISR / FSR parton?  These
  // are not exclusive — a hadron from a hard-scatter recoil parton may
  // descend through both ISR and FSR.
  t.fromISR = !h.ancestorsAtStage(idx, Stage::ISR).empty();
  t.fromFSR = !h.ancestorsAtStage(idx, Stage::FSR).empty();

  // ---- Heavy-flavour chain + decay-chain depth ----------------------
  // Two independent walks:
  //
  //   1. decayChainDepth  — counts hadronic decay steps along the LEAD
  //      branch from this hadron to its first partonic ancestor.  A
  //      single integer; the lead-branch convention is fine because
  //      depth is a per-particle quantity.
  //
  //   2. fromCharmChain / fromBottomChain — must be a BREADTH-FIRST walk
  //      across EVERY hadronic ancestor branch, otherwise a heavy hadron
  //      hiding on a non-lead branch is missed (audit fix #5).  E.g. for
  //      a pi+ in a D+- -> K-* pi+ pi+ chain, the lead branch may be the
  //      pi-side while the charm hadron is the parent of the K-side.
  {
    // (1) lead-branch depth — kept identical to the prior implementation
    // so the decay_chain_depth histogram doesn't change shape.
    int cur   = idx;
    int depth = 0;
    while (depth < MAX_DECAY_DEPTH)
      {
      const ParticleNode & cn = h.node(cur);
      int nextParent = -1;
      for (int p : cn.parents)
        {
        if (p < 0 || p >= h.size()) continue;
        const ParticleNode & parent = h.node(p);
        if (parent.isParton()) continue;
        nextParent = p;
        break;
        }
      if (nextParent < 0) break;
      cur = nextParent;
      ++depth;
      }
    t.decayChainDepth = depth;

    // (2) BFS across every hadronic ancestor branch for heavy-flavour
    // detection.  Cycle-safe via a "seen" vector; bounded by the same
    // MAX_DECAY_DEPTH so a malformed graph cannot hang the tagger.
    std::vector<char> seen(static_cast<size_t>(h.size()), 0);
    std::vector<int>  frontier = h.node(idx).parents;
    int steps = 0;
    while (!frontier.empty() && steps < MAX_DECAY_DEPTH)
      {
      std::vector<int> next;
      for (int p : frontier)
        {
        if (p < 0 || p >= h.size())             continue;
        if (seen[static_cast<size_t>(p)])       continue;
        seen[static_cast<size_t>(p)] = 1;
        const ParticleNode & parent = h.node(p);
        if (parent.isParton()) continue;        // stop the walk at partons
        if (isCharmHadron(parent.pdg))  t.fromCharmChain  = true;
        if (isBottomHadron(parent.pdg)) t.fromBottomChain = true;
        // Early-out if both flags are already set.
        if (t.fromCharmChain && t.fromBottomChain) { frontier.clear(); break; }
        for (int gp : parent.parents) next.push_back(gp);
        }
      frontier.swap(next);
      ++steps;
      }
  }

  return t;
}

// ----------------------------------------------------------------------
std::vector<ProvenanceTag>
ProvenanceTagger::tagFinalState(const EventHistory & h) const
{
  std::vector<ProvenanceTag> tags;
  const std::vector<int> finals = h.finalState();
  tags.reserve(finals.size());
  for (int f : finals) tags.push_back(tag(h, f));
  return tags;
}

// ----------------------------------------------------------------------
bool sharesDecayParent(const ProvenanceTag & a, const ProvenanceTag & b)
{
  return a.parentHadronIndex >= 0 &&
         a.parentHadronIndex == b.parentHadronIndex;
}

// ----------------------------------------------------------------------
bool sharesPartonAncestor(const ProvenanceTag & a, const ProvenanceTag & b)
{
  for (int x : a.partonAncestorIndices)
    for (int y : b.partonAncestorIndices)
      if (x == y) return true;
  return false;
}

bool sharesHardProcessAncestor(const ProvenanceTag & a, const ProvenanceTag & b)
{
  return a.hardPartonIndex >= 0 && a.hardPartonIndex == b.hardPartonIndex;
}

// ----------------------------------------------------------------------
bool sharesMPIVertex(const ProvenanceTag & a, const ProvenanceTag & b)
{
  return a.mpiIndex >= 0 && a.mpiIndex == b.mpiIndex;
}

} // namespace CAP
