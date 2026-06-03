/* **********************************************************************
 * CAP::PairProvenanceObservables  —  two-particle correlations split by
 *                                    pair ancestry
 *
 * Part of the parton-tracking feature (Phase 3d).
 *
 * This is the literal answer to "what fraction of the correlation comes
 * from the parents".  For every same-event pair of final hadrons it
 * decides where the pair's correlation comes from:
 *
 *   SameResonance  — both hadrons are decay products of ONE resonance
 *                    (rho -> pi pi, K* -> K pi, ...).  The textbook
 *                    resonance contamination of a same-event signal.
 *   SameWeakParent — both from one weak decay (K0S, Lambda, ...).
 *   SharedParton   — they share a pre-hadronization parton ancestor:
 *                    the correlation is inherited from the partonic
 *                    stage (string / jet structure).
 *   Unrelated      — no common ancestry: combinatorial background.
 *
 * The dEta / dPhi distribution of each class is accumulated separately,
 * so you can see e.g. the near-side resonance peak sitting on top of
 * the partonic correlation and the flat combinatorial floor.
 *
 * Pure C++14 — reuses Hist1D from ProvenanceObservables.  No ROOT.
 * Fully unit-testable.
 * ********************************************************************/
#ifndef CAP__PairProvenanceObservables
#define CAP__PairProvenanceObservables

#include <limits>
#include <map>
#include <string>
#include <vector>

#include "EventHistory.hpp"
#include "ProvenanceTagger.hpp"
#include "ProvenanceObservables.hpp"   // Hist1D

namespace CAP
{

// ----------------------------------------------------------------------
//  Pair ancestry classes.  Enum values 0..3 — used to index a counter.
// ----------------------------------------------------------------------
enum class PairClass
{
  SameResonance  = 0,
  SameWeakParent = 1,
  SharedParton   = 2,
  Unrelated      = 3
};

std::string pairClassName(PairClass c);

// Classify a pair from the two hadrons' provenance tags.  Order of
// precedence: a shared decay parent is the most specific relationship,
// then a shared parton ancestor, else unrelated.
PairClass classifyPair(const ProvenanceTag & a, const ProvenanceTag & b);

// ----------------------------------------------------------------------
//  The pair-correlation accumulator.
// ----------------------------------------------------------------------
class PairProvenanceObservables
{
public:

  // speciesPdg : |pdg| of the species to pair up (0 = every final hadron).
  // pt/eta cuts: per-pion acceptance window applied BEFORE pairing.
  // multLow / multHigh: event-multiplicity thresholds defining the
  //              Low / Mid / High bins.  Defaults (20, 80) are tuned for
  //              pp at 13 TeV soft QCD; the user should override for any
  //              other regime (e.g. heavy-ion).
  // spheroLow / spheroHigh: transverse-spherocity (S0) thresholds for the
  //              JetLike / Mid / Isotropic event-shape bins.  S0 in [0,1];
  //              low S0 ≈ pencil-like jet event, high S0 ≈ isotropic event.
  //              Defaults (0.3, 0.7) follow ALICE-style cuts.
  explicit PairProvenanceObservables(
      int    speciesPdg = 0,
      double ptMin  =  0.0,
      double ptMax  =  std::numeric_limits<double>::infinity(),
      double etaMin = -std::numeric_limits<double>::infinity(),
      double etaMax =  std::numeric_limits<double>::infinity(),
      int    multLow  = 20,
      int    multHigh = 80,
      double spheroLow  = 0.3,
      double spheroHigh = 0.7);

  // Multi-species overload — same set of pair histograms emitted for
  // each unordered species combination (sA, sB) drawn from the list,
  // tagged with suffix `_S<a>x<b>`.  Single-species mode (size 1) keeps
  // the legacy bare names.
  explicit PairProvenanceObservables(
      const std::vector<int> & speciesList,
      double ptMin  =  0.0,
      double ptMax  =  std::numeric_limits<double>::infinity(),
      double etaMin = -std::numeric_limits<double>::infinity(),
      double etaMax =  std::numeric_limits<double>::infinity(),
      int    multLow  = 20,
      int    multHigh = 80,
      double spheroLow  = 0.3,
      double spheroHigh = 0.7);

  const std::vector<int> & speciesList() const { return _species; }

  // Feed one event.
  void accumulate(const EventHistory & history,
                  const std::vector<ProvenanceTag> & tags);

  int  events() const { return _events; }
  long pairs()  const { return _pairs;  }
  long pairsInClass(PairClass c) const;

  const std::map<std::string,Hist1D> & histograms() const { return _hist; }

  // Per-class fraction of all studied pairs.
  std::string report() const;

private:

  Hist1D & H(const std::string & name);   // fetch-or-create

  // Returns the suffix "_S<a>x<b>" (single-species mode: "").
  std::string speciesPairSuffix(int a, int b) const;

  std::vector<int> _species;                 // canonicalised species list
  int    _speciesPdg;                        // legacy alias = _species[0]
  double _ptMin, _ptMax, _etaMin, _etaMax;   // acceptance window
  int    _multLow, _multHigh;                // multiplicity-bin cuts
  double _spheroLow, _spheroHigh;            // event-shape (S0) bin cuts
  int  _events = 0;
  long _pairs  = 0;
  long _count[4] = { 0, 0, 0, 0 };         // indexed by PairClass
  std::map<std::string,Hist1D> _hist;
};

} // namespace CAP

#endif // CAP__PairProvenanceObservables
