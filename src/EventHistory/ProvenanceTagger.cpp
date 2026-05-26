/* **********************************************************************
 * CAP::ProvenanceTagger — implementation.  See header for design.
 * ********************************************************************/
#include "ProvenanceTagger.hpp"

#include <cstdlib>   // std::abs

namespace CAP
{

// ----------------------------------------------------------------------
//  Curated list of the common strong / EM-decaying resonances that
//  contaminate same-event correlations.  Compared on |pdg|.
//
//  This is deliberately a hard-coded list at Phase 3a: it is explicit,
//  reviewable, and needs no particle database.  A later phase can
//  replace it with a width / lifetime lookup via the CAP ParticleDb.
// ----------------------------------------------------------------------
bool ProvenanceTagger::isResonance(int pdg)
{
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

  // How far back the chain reaches.
  if (!hard.empty())
    t.deepestStage = Stage::HardProcess;
  else if (!t.partonAncestorIndices.empty())
    t.deepestStage = Stage::PartonsPreHadronization;
  else
    t.deepestStage = node.stage;

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

} // namespace CAP
