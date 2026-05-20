/* **********************************************************************
 * CAP::PythiaHistoryBuilder — implementation.  See header for design.
 * ********************************************************************/
#include "PythiaHistoryBuilder.hpp"

#include "Pythia8/Event.h"

namespace CAP
{

// ----------------------------------------------------------------------
Stage PythiaHistoryBuilder::stageFromPythiaStatus(int s)
{
  if (s >= 11 && s <= 19) return Stage::Beam;
  if (s >= 21 && s <= 29) return Stage::HardProcess;
  if (s >= 31 && s <= 39) return Stage::MPI;
  if (s >= 41 && s <= 49) return Stage::ISR;
  if (s >= 51 && s <= 59) return Stage::FSR;
  if (s >= 61 && s <= 69) return Stage::BeamRemnants;
  if (s >= 71 && s <= 79) return Stage::PartonsPreHadronization;
  if (s >= 81 && s <= 89) return Stage::PrimaryHadrons;
  if (s >= 91 && s <= 99) return Stage::DecayProducts;
  return Stage::Unknown;
}

// ----------------------------------------------------------------------
void PythiaHistoryBuilder::build(const Pythia8::Event & ev,
                                 EventHistory &         out) const
{
  out.clear();
  const int n = ev.size();

  // Pass 1 — one node per Pythia entry, in order, so that the
  // EventHistory node index equals the Pythia event-record index.
  // (Entry 0 is Pythia's bookkeeping "system" pseudo-particle; we keep
  // it so the indices line up — it carries Stage::Unknown and is
  // harmless to downstream queries.)
  for (int i = 0; i < n; ++i)
    {
    const Pythia8::Particle & p = ev[i];
    ParticleNode node;
    node.pdg     = p.id();
    node.status  = p.status();
    node.stage   = stageFromPythiaStatus(p.statusAbs());
    node.isFinal = p.isFinal();
    node.px      = p.px();
    node.py      = p.py();
    node.pz      = p.pz();
    node.e       = p.e();
    node.xProd   = p.xProd();
    node.yProd   = p.yProd();
    node.zProd   = p.zProd();
    node.tProd   = p.tProd();
    out.addNode(node);
    }

  // Pass 2 — copy the daughter links.  EventHistory::link() fills both
  // the parent->child and child->parent directions, so iterating
  // daughters alone reconstructs the whole DAG.
  for (int i = 0; i < n; ++i)
    {
    const std::vector<int> daughters = ev[i].daughterList();
    for (int d : daughters)
      if (d >= 0 && d < n) out.link(i, d);
    }
}

} // namespace CAP
