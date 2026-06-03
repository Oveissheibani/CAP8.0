/* **********************************************************************
 * CAP::ProvenanceObservables  —  observables decomposed by particle origin
 *
 * Part of the parton-tracking feature (Phase 3b).
 *
 * Accumulates single-particle and event-global observables over many
 * events, split by where each final hadron came from:
 *
 *   by origin class : Primary  (direct hadronization product)
 *                     FromResonance   (rho, K*, Delta, ... decay)
 *                     FromWeakDecay   (K0S, Lambda, ... decay)
 *   by parton class : the flavour of the pre-hadronization parton the
 *                     hadron descends from (light / s / c / b / gluon)
 *
 * This answers, directly from the histograms: what fraction of the
 * yield is direct vs feed-down, and how the spectrum of each origin
 * class differs — the single-particle / global half of the "what
 * happened before, during and after hadronization" question.
 *
 * Pure C++14 — no ROOT.  A thin runner converts the in-memory Hist1D
 * objects to ROOT TH1D for output.  Fully unit-testable.
 * ********************************************************************/
#ifndef CAP__ProvenanceObservables
#define CAP__ProvenanceObservables

#include <limits>
#include <map>
#include <string>
#include <vector>

#include "EventHistory.hpp"
#include "ProvenanceTagger.hpp"

namespace CAP
{

// ----------------------------------------------------------------------
//  Minimal 1-D histogram — pure C++, no ROOT dependency.
// ----------------------------------------------------------------------
struct Hist1D
{
  std::string name;
  std::string title;
  int    nbins = 0;
  double lo = 0.0;
  double hi = 0.0;
  std::vector<double> counts;       // size == nbins
  double underflow = 0.0;
  double overflow  = 0.0;
  double entries   = 0.0;

  Hist1D() = default;
  Hist1D(const std::string & n, const std::string & t,
         int nb, double a, double b);

  void   fill(double x, double w = 1.0);
  double binCenter(int i) const;
  double integral() const;          // sum of in-range bins
};

// ----------------------------------------------------------------------
//  Provenance classes.
// ----------------------------------------------------------------------
enum class OriginClass { Primary, FromResonance, FromWeakDecay, Unknown };
enum class PartonClass { LightQuark, Strange, Charm, Bottom, Gluon, NonPartonic };

std::string originClassName(OriginClass c);
std::string partonClassName(PartonClass c);

OriginClass classifyOrigin(const ProvenanceTag & t);
// String-ENDPOINT parton flavour (Lund endpoint; never a gluon).
PartonClass classifyParton(const ProvenanceTag & t);
// INITIATING hard-scatter parton flavour (gluon-capable: quark- vs gluon-jet
// origin).  See ProvenanceObservables.cpp for the string-endpoint vs
// initiating distinction.
PartonClass classifyInitiatingParton(const ProvenanceTag & t);

// ----------------------------------------------------------------------
//  The accumulator.
// ----------------------------------------------------------------------
class ProvenanceObservables
{
public:

  // speciesPdg : |pdg| of the species to study (e.g. 211 for pi+/-).
  //              0 means "every final-state hadron".
  // ptMin/ptMax  : transverse-momentum acceptance window (defaults are
  //                wide-open — i.e. no cut).  A hadron whose pT falls
  //                outside [ptMin, ptMax] is dropped entirely.
  // etaMin/etaMax: pseudorapidity acceptance window (defaults wide-open).
  explicit ProvenanceObservables(
      int    speciesPdg = 0,
      double ptMin  =  0.0,
      double ptMax  =  std::numeric_limits<double>::infinity(),
      double etaMin = -std::numeric_limits<double>::infinity(),
      double etaMax =  std::numeric_limits<double>::infinity());

  // Multi-species overload — accepts a list of |pdg| values.  Each
  // selected hadron is routed into species-tagged histograms, e.g.
  // `pt_origin_Primary_S211` for pi+/-.  When the list has exactly one
  // entry the histogram names match the legacy single-species format
  // (no suffix) so existing downstream plots keep working.  A 0 anywhere
  // in the list collapses to the legacy `all-hadrons` mode.
  explicit ProvenanceObservables(
      const std::vector<int> & speciesList,
      double ptMin  =  0.0,
      double ptMax  =  std::numeric_limits<double>::infinity(),
      double etaMin = -std::numeric_limits<double>::infinity(),
      double etaMax =  std::numeric_limits<double>::infinity());

  // Effective species list after canonicalisation (0 collapses).
  const std::vector<int> & speciesList() const { return _species; }

  // Feed one event: the history and the per-final-hadron tags.
  void accumulate(const EventHistory & history,
                  const std::vector<ProvenanceTag> & tags);

  int    events()       const { return _events; }
  double totalStudied() const { return _totalStudied; }

  // Read access to every histogram (the runner iterates this to export).
  const std::map<std::string,Hist1D> & histograms() const { return _hist; }

  // Human-readable summary: per-class fractions of the studied yield.
  std::string report() const;

private:

  Hist1D & H(const std::string & name);   // fetch-or-create

  // Per-species suffix used in histogram names.  Empty in single-species
  // mode for backward compatibility with existing plots.
  std::string speciesSuffix(int s) const;

  std::vector<int> _species;              // canonicalised species list
  int    _speciesPdg;                     // legacy, == _species[0] when size==1
  double _ptMin, _ptMax, _etaMin, _etaMax;   // acceptance window
  int    _events       = 0;
  double _totalStudied = 0.0;             // # studied hadrons, all events
  std::map<std::string,Hist1D> _hist;
};

} // namespace CAP

#endif // CAP__ProvenanceObservables
