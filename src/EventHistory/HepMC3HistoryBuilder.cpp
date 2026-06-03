/* **********************************************************************
 * CAP::HepMC3HistoryBuilder — implementation.  See header for design.
 *
 * Phase-1 stage classification, by necessity, is coarser than the
 * Pythia path: HepMC status codes only distinguish final / decayed /
 * documentation / beam.  We recover granularity from the graph:
 *
 *   - status 4                       -> Beam
 *   - parton (quark/gluon):
 *        end vertex emits hadrons    -> PartonsPreHadronization
 *        otherwise                   -> FSR  (generic shower bucket)
 *   - hadron / lepton:
 *        produced from partons       -> PrimaryHadrons
 *        produced from hadrons       -> DecayProducts
 *
 * A later phase can sharpen this with generator-specific attributes.
 * ********************************************************************/
#include "HepMC3HistoryBuilder.hpp"

#include "HepMC3/GenEvent.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenVertex.h"
#include "HepMC3/FourVector.h"

#include <map>
#include <cstdlib>   // std::abs

namespace CAP
{

namespace
{
// Quarks (|pdg|<10) and gluons (21).  Diquarks (1103, 2101, ...) are
// deliberately not treated as partons at Phase 1 — they are rare in
// the analysis-relevant final-state ancestry and folding them in
// cleanly needs the generator-specific colour model.
bool pdgIsParton(int pdg)
{
  const int a = std::abs(pdg);
  return a < 10 || a == 21;
}

// The hadronization BOUNDARY pseudo-particles: a Herwig cluster (PDG 81), a
// generic cluster (91) and a Lund string (92).  These are neither real
// partons nor real hadrons — they are the object that fragments INTO the
// primary hadrons.  A hadron whose parent is one of these is a PRIMARY hadron
// (formed directly at hadronization), NOT a decay product.  Without this,
// every Herwig primary hadron — which always descends from a cluster — is
// mislabelled as feed-down, giving the spurious "Primary 0%" / inflated
// FromWeakDecay seen in the HepMC (Herwig) path.  Verified on real Herwig
// output: the dominant direct parent of primary pions is PDG 81.
bool pdgIsHadronizationBoundary(int pdg)
{
  const int a = std::abs(pdg);
  return a == 81 || a == 91 || a == 92;
}
} // anonymous namespace

// ----------------------------------------------------------------------
void HepMC3HistoryBuilder::build(HepMC3::GenEvent & ev,
                                 EventHistory &     out) const
{
  out.clear();

  // HepMC GenParticle::id() -> EventHistory node index.
  std::map<int,int> idToIndex;

  // Pass 1 — a node per HepMC particle (kinematics + production vertex).
  for (auto & p : ev.particles())
    {
    if (!p) continue;
    ParticleNode node;
    node.pdg     = p->pid();
    node.status  = p->status();
    node.isFinal = (p->status() == 1);

    const HepMC3::FourVector & m = p->momentum();
    node.px = m.px();  node.py = m.py();  node.pz = m.pz();  node.e = m.e();

    if (auto pv = p->production_vertex())
      {
      const HepMC3::FourVector & xv = pv->position();
      node.xProd = xv.x();  node.yProd = xv.y();
      node.zProd = xv.z();  node.tProd = xv.t();
      }

    // stage is assigned in pass 2 — it needs the graph.
    const int idx = out.addNode(node);
    idToIndex[p->id()] = idx;
    }

  // Pass 2 — links + stage classification.
  for (auto & p : ev.particles())
    {
    if (!p) continue;
    std::map<int,int>::const_iterator it = idToIndex.find(p->id());
    if (it == idToIndex.end()) continue;
    const int idx = it->second;

    // Parents = production-vertex inputs.  While we walk them, note
    // whether this particle was produced from partons or from hadrons.
    bool fromPartons = false;
    bool fromHadrons = false;
    if (auto pv = p->production_vertex())
      {
      for (auto & in : pv->particles_in())
        {
        if (!in) continue;
        std::map<int,int>::const_iterator pit = idToIndex.find(in->id());
        if (pit != idToIndex.end()) out.link(pit->second, idx);
        // A cluster / string parent is the hadronization BOUNDARY, not a
        // real hadron — treat it like a parton source so the hadron it
        // produces is classified PrimaryHadrons (formed at hadronization),
        // not DecayProducts.  This is the Herwig "Primary 0%" fix.
        if (pdgIsParton(in->pid()) || pdgIsHadronizationBoundary(in->pid()))
          fromPartons = true;
        else
          fromHadrons = true;
        }
      }

    ParticleNode & node = out.node(idx);
    const int st = p->status();

    if (st == 4)
      {
      node.stage = Stage::Beam;
      }
    else if (pdgIsParton(p->pid()))
      {
      bool endsInHadrons = false;
      if (auto endv = p->end_vertex())
        for (auto & ou : endv->particles_out())
          if (ou && !pdgIsParton(ou->pid())) endsInHadrons = true;
      node.stage = endsInHadrons ? Stage::PartonsPreHadronization
                                 : Stage::FSR;
      }
    else
      {
      if (fromPartons && !fromHadrons)
        node.stage = Stage::PrimaryHadrons;
      else if (fromHadrons)
        node.stage = Stage::DecayProducts;
      else
        node.stage = node.isFinal ? Stage::FinalState : Stage::Unknown;
      }
    }
}

} // namespace CAP
