/* **********************************************************************
 * CAP::FragmentationSystems — string / cluster anatomy from the DAG
 *
 * Part of the parton-tracking feature (Phase 6 — fragmentation systems).
 *
 * A SELF-CONTAINED, OPT-IN accumulator (provenance-study --systems).
 * With the flag off nothing in the existing chain changes.
 *
 * THE OBJECT.  A "fragmentation system" is the generator-agnostic unit
 * of hadronization: the connected component of primary hadrons that
 * share a hadronization source.  Each PRIMARY hadron (pre-decay) is
 * keyed by its nearest ancestors that are partons or explicit
 * hadronization objects (Herwig cluster, PDG 81); primaries sharing a
 * key are merged (union-find).  In Pythia a component is one Lund
 * STRING system (the primaries' parents are the string's partons); in
 * Herwig it is one CLUSTER.  System kinematics are computed from the
 * SUM OF ITS PRIMARY HADRONS, which both models conserve exactly —
 * so masses are comparable across generators by construction.
 *
 * WHAT IT MEASURES.
 *  1. System mass spectrum   — the cluster model's central assumption
 *     (low, peaked) vs the string picture (heavy, broad), head-to-head.
 *  2. Hadrons per system     — cluster ~2 by construction, string many.
 *  3. <n_had> vs system mass — Lund predicts n ~ ln(m^2); clusters stay
 *     near 2 because heavy clusters FISSION before decaying.
 *  4. Rapidity span per system — string hadrons spread along the string
 *     axis (the mechanistic origin of long-range correlations); cluster
 *     decays are local.
 *  5. Lambda measure  sum_i ln(max(m_i^2, m0^2)/m0^2)  per event — the
 *     quantity colour reconnection exists to MINIMIZE; its drop on the
 *     +CR ladder rung makes CR's action (not just its consequences)
 *     visible.
 *  6. Charge ordering — string breaks make q-qbar pairs, so rapidity-
 *     NEIGHBOURING hadrons of one system should carry opposite charges;
 *     compared against cross-system pairs (no such constraint).
 *  7. Local pT compensation — cos(delta-phi) of neighbouring same-
 *     system hadrons; break kicks anti-correlate neighbours.
 *  8. Baryon pairing — diquark breaks put B and Bbar close in rapidity
 *     WITHIN one system; strangeness locality measured the same way.
 *  9. Herwig-only: explicit cluster nodes (PDG 81) expose the fission
 *     chain — top-cluster vs decaying-cluster mass spectra.  These
 *     histograms simply stay empty on the Pythia path.
 *
 * Pure C++14, no ROOT (runner converts Hist1D to TH1D).  Unit-testable.
 * ********************************************************************/
#ifndef CAP__FragmentationSystems
#define CAP__FragmentationSystems

#include <map>
#include <string>
#include <vector>

#include "EventHistory.hpp"
#include "ProvenanceObservables.hpp"   // Hist1D

namespace CAP
{

class FragmentationSystems
{
public:

  FragmentationSystems() = default;

  // Feed one event.  Needs only the history (primaries are read from the
  // PrimaryHadrons stage, BEFORE decays) — no tags required.
  void accumulate(const EventHistory & history);

  int events() const { return _events; }

  const std::map<std::string,Hist1D> & histograms() const { return _hist; }

  // Text block for the .root.txt summary; sections carry the unique
  // "fragmentation:" marker prefix for the report parser.
  std::string report() const;

  // --- small classifiers (public + static: unit-testable) -------------
  // Electric charge sign: +1 / -1 / 0 (curated list of generator-stable
  // and primary species; antiparticles handled by the pdg sign).
  static int  chargeSign(int pdg);
  // Baryon (4-digit baryon codes); sign of baryon number = sign(pdg).
  static bool isBaryon(int pdg);
  // Carries a strange valence quark (a '3' among the quark digits).
  static bool hasStrangeQuark(int pdg);
  // True rapidity y (not eta), with guards for E ~ |pz|.
  static double rapidity(const ParticleNode & n);

private:

  Hist1D & H(const std::string & name);

  // Per-primary system keys: indices of the nearest ancestors that are
  // partons or hadronization-boundary objects (|pdg| 81/91/92).
  std::vector<int> systemKeys(const EventHistory & h, int idx) const;

  int _events = 0;
  std::map<std::string,Hist1D> _hist;
};

} // namespace CAP

#endif // CAP__FragmentationSystems
