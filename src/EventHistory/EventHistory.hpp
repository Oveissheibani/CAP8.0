/* **********************************************************************
 * CAP::EventHistory  —  generator-agnostic event-history (provenance) DAG
 *
 * Part of the parton-tracking feature (Phase 1).
 *
 * An EventHistory is a directed acyclic graph of every particle in an
 * event, from the hard scatter to the final state.  Each node carries
 * its PDG id, four-momentum, production vertex, a CAP::Stage label and
 * index links to its parents and children.
 *
 * It is filled, once per event, by a per-generator builder:
 *   - PythiaHistoryBuilder   — from a Pythia 8 Event record
 *   - HepMC3HistoryBuilder   — from a HepMC3 GenEvent
 * and then queried by analysis code, e.g. "give me every particle at
 * the PrimaryHadrons stage" or "walk this final hadron back to its
 * parent partons".
 *
 * Phase 1 is infrastructure only — no CAP analysis task uses this yet,
 * so building this module cannot change any existing CAP behaviour.
 *
 * Deliberately ROOT-free and dependency-free: the core is plain C++14
 * so it compiles everywhere and is trivial to unit-test.
 * ********************************************************************/
#ifndef CAP__EventHistory
#define CAP__EventHistory

#include <vector>
#include <string>
#include <cmath>
#include <cstdlib>

#include "StageTaxonomy.hpp"

namespace CAP
{

// ----------------------------------------------------------------------
//  One node of the event-history DAG.
//
//  Parent / child links are stored as integer indices into the owning
//  EventHistory's node vector — not pointers — so the whole structure
//  stays trivially copyable and is easy to serialise later.
// ----------------------------------------------------------------------
struct ParticleNode
{
  int   pdg     = 0;               // PDG id
  int   status  = 0;               // raw generator status (signed)
  Stage stage   = Stage::Unknown;  // production stage on the CAP taxonomy
  bool  isFinal = false;           // survives into the final state

  double px = 0.0, py = 0.0, pz = 0.0, e = 0.0;          // 4-momentum [GeV]
  double xProd = 0.0, yProd = 0.0, zProd = 0.0, tProd = 0.0; // prod. vertex

  std::vector<int> parents;        // node indices of direct ancestors
  std::vector<int> children;       // node indices of direct descendants

  double pt()   const { return std::sqrt(px*px + py*py); }
  double pmag() const { return std::sqrt(px*px + py*py + pz*pz); }
  double mass() const
  {
    const double m2 = e*e - px*px - py*py - pz*pz;
    return m2 > 0.0 ? std::sqrt(m2) : 0.0;
  }
  // Pseudorapidity, with guards for exactly-forward / backward tracks.
  double eta() const
  {
    const double p = pmag();
    if (p <= 0.0)        return  0.0;
    if (p - pz <= 0.0)   return  20.0;
    if (p + pz <= 0.0)   return -20.0;
    return 0.5 * std::log((p + pz) / (p - pz));
  }
  // Quarks (|pdg|<10) and gluons (21).  Diquarks are intentionally left
  // out at Phase 1 — see HepMC3HistoryBuilder for the rationale.
  bool isParton() const { return std::abs(pdg) < 10 || pdg == 21; }
};

// ----------------------------------------------------------------------
//  Capability summary — how much provenance a given event/source has.
//
//  A skimmed HepMC3 file may only carry final-state + decayed hadrons;
//  detector data carries no provenance at all.  The GUI uses this to
//  grey out stages a source cannot support.
// ----------------------------------------------------------------------
struct HistoryCapability
{
  Stage deepestStage      = Stage::Unknown; // earliest stage present
  bool  hasPartons        = false;
  bool  hasPrimaryHadrons = false;
  bool  hasDecayChain     = false;
  bool  hasGraphLinks     = false;          // parent/child links present

  std::string describe() const;
};

// ----------------------------------------------------------------------
//  The event-history DAG.
// ----------------------------------------------------------------------
class EventHistory
{
public:

  EventHistory()  = default;
  ~EventHistory() = default;

  void clear()              { _nodes.clear(); }
  int  size()  const        { return static_cast<int>(_nodes.size()); }
  bool empty() const        { return _nodes.empty(); }

  // Append a node; returns its index.
  int  addNode(const ParticleNode & n);

  ParticleNode &       node(int i)       { return _nodes[i]; }
  const ParticleNode & node(int i) const { return _nodes[i]; }
  const std::vector<ParticleNode> & nodes() const { return _nodes; }

  // Link an already-added parent to an already-added child.  Fills both
  // directions.  Out-of-range indices and duplicates are ignored.
  void link(int parentIndex, int childIndex);

  // ---- queries -------------------------------------------------------

  // Indices of every node whose production stage == s.
  std::vector<int> collectStage(Stage s) const;

  // Indices of every final-state node.
  std::vector<int> finalState() const;

  // Walk the parents of node i upward; return the indices of the first
  // ancestors reached whose stage == s.  Because the graph is a DAG a
  // node can have several such ancestors.  Cycle-safe.
  std::vector<int> ancestorsAtStage(int i, Stage s) const;

  // Earliest (deepest) stage present in the event.
  Stage deepestStage() const;

  // What this event supports.
  HistoryCapability capability() const;

private:

  std::vector<ParticleNode> _nodes;
};

} // namespace CAP

#endif // CAP__EventHistory
