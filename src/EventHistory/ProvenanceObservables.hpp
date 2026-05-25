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
PartonClass classifyParton(const ProvenanceTag & t);

// ----------------------------------------------------------------------
//  The accumulator.
// ----------------------------------------------------------------------
class ProvenanceObservables
{
public:

  // speciesPdg : |pdg| of the species to study (e.g. 211 for pi+/-).
  //              0 means "every final-state hadron".
  explicit ProvenanceObservables(int speciesPdg = 0);

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

  int    _speciesPdg;
  int    _events       = 0;
  double _totalStudied = 0.0;             // # studied hadrons, all events
  std::map<std::string,Hist1D> _hist;
};

} // namespace CAP

#endif // CAP__ProvenanceObservables
