/* **********************************************************************
 * CAP::EntropyObservables  —  entropy / information content of the event
 *
 * Part of the parton-tracking feature (Phase 5 — entropy study).
 *
 * A SELF-CONTAINED, OPT-IN accumulator.  It is only instantiated when
 * provenance-study is run with --entropy; with the flag off nothing in
 * the existing provenance chain changes (no shared state, no edits to
 * ProvenanceObservables / PairProvenanceObservables).
 *
 * What it measures, per run:
 *
 *  1. MULTIPLICITY ENTROPY  S = -sum_N P(N) ln P(N)  (nats)
 *     of the charged final-state multiplicity in several |eta| windows.
 *     This is the quantity the Kharzeev–Levin duality conjecture equates
 *     with the entanglement entropy of the probed partonic state
 *     (arXiv:2110.06156, 2207.09430).  Computed from the per-event
 *     multiplicity distributions, with a plug-in estimator + Miller–
 *     Madow bias correction and a statistical error.
 *
 *  2. STAGE ENTROPY PROFILE — "entropy production along the onion".
 *     For each Stage ring of the EventHistory DAG (HardProcess -> MPI ->
 *     ISR/FSR -> PartonsPreHadronization -> PrimaryHadrons ->
 *     DecayProducts -> FinalState) the ensemble occupancy entropy of the
 *     stage's particles over a coarse (eta, pT) grid, plus the mean
 *     object count per event.  Shows WHERE in the event evolution the
 *     entropy is produced — shower, hadronization, or decays — and how
 *     MPI / CR / the hadronization model (string vs cluster) move it.
 *
 *  3. INFORMATION RECOVERY — hadronization as a noisy channel.
 *     Mutual information (bits, Miller–Madow corrected) between the
 *     initial-state tags the provenance tagger already computes and
 *     final-state observables:
 *        I(initiating-parton class ; hadron species)
 *        I(initiating-parton class ; hadron pT bin)
 *        I(origin class            ; hadron species)
 *     each also restricted to the Primary / Decay subsets, quantifying
 *     how much initial-state information SURVIVES hadronization and how
 *     much the decay layer erases on top.  Plus the forward–backward
 *     multiplicity mutual information I(N_F ; N_B) — the classical proxy
 *     for entanglement between rapidity intervals.
 *
 * IMPORTANT FRAMING — these are exact SHANNON entropies of classical
 * generator output.  The generator contains no quantum state; what the
 * multiplicity entropy enables is a TEST of the Kharzeev–Levin duality,
 * not a simulation of entanglement.  The report text repeats this.
 *
 * Pure C++14 — no ROOT (mirrors ProvenanceObservables: the runner
 * converts the Hist1D map to TH1D for output).  Fully unit-testable.
 * ********************************************************************/
#ifndef CAP__EntropyObservables
#define CAP__EntropyObservables

#include <map>
#include <string>
#include <utility>
#include <vector>

#include "EventHistory.hpp"
#include "ProvenanceTagger.hpp"
#include "ProvenanceObservables.hpp"   // Hist1D + Origin/Parton classes

namespace CAP
{

class EntropyObservables
{
public:

  EntropyObservables();

  // Feed one event: the history and the per-final-hadron tags (the same
  // two objects provenance-study already has in hand — no extra work in
  // the event loop beyond this call).
  void accumulate(const EventHistory & history,
                  const std::vector<ProvenanceTag> & tags);

  int events() const { return _events; }

  // Histograms for ROOT export.  Includes the per-window multiplicity
  // distributions (ent_mult_*), the hemisphere multiplicities
  // (ent_fb_*), and the finalized stage profile (ent_stage_S /
  // ent_stage_meanN — one bin per stage, in stageList() order).
  const std::map<std::string,Hist1D> & histograms() const;

  // Human-readable summary block, appended to the .root.txt summary.
  // Section markers all carry the unique "entropy:" prefix so the
  // report parser cannot confuse them with the existing genealogy
  // sections.
  std::string report() const;

  // The fixed stage order used by the profile (bin i of ent_stage_S).
  static const std::vector<Stage> & stageList();

  // --- estimators (public + static so they are unit-testable) ---------

  // Plug-in Shannon entropy (nats) of a discrete count vector, with the
  // Miller–Madow bias correction  S_MM = S_plugin + (K-1)/(2N)  where K
  // is the number of OCCUPIED cells.  Returns {S_MM, statistical error}.
  static std::pair<double,double> shannonEntropy(
      const std::vector<double> & counts);

  // Rényi-2 ("collision") entropy  S_2 = -ln sum p^2  (nats), using the
  // UNBIASED estimator of the power sum,
  //   sum p^2  ->  sum c(c-1) / [N(N-1)],
  // so unlike the Shannon plug-in this needs no bias correction.  S_2 is
  // what swap-operator / cold-atom protocols actually measure for
  // entanglement, and S_2 <= S_1 always (a useful internal cross-check).
  static double renyi2Entropy(const std::vector<double> & counts);

  // Mutual information (BITS) of a joint count table, Miller–Madow
  // corrected via  I = S_MM(X) + S_MM(Y) - S_MM(XY).  Returns
  // {I_MM_bits, plug-in bias estimate in bits = (Kx-1)(Ky-1)/(2N ln2)}.
  // The bias estimate is reported so the reader can judge whether the
  // sample is large enough for the quoted I to be meaningful.
  static std::pair<double,double> mutualInformation(
      const std::map<std::pair<int,int>,double> & joint);

private:

  Hist1D & H(const std::string & name);
  // Recompute the derived stage-profile histograms into _hist.
  // Idempotent; called by histograms() / report().
  void finalize() const;

  // Charged final-state selector (pdg-based; no particle DB needed).
  static bool isChargedFinal(int pdg);
  // Species group for the MI tables: 0 pi, 1 K, 2 p, 3 lepton, 4 other.
  static int  speciesGroup(int pdg);
  static int  ptBin(double pt);            // 9 bins, edges in .cpp

  int _events = 0;

  // --- multiplicity windows (half-widths; <0 means "full") ------------
  std::vector<double>      _etaWindows;    // 0.5, 1.0, 2.0, -1 (=full)
  std::vector<std::string> _windowNames;   // eta05, eta10, eta20, full
  // Exact per-window multiplicity moments, for <N> and the Kharzeev–
  // Levin maximal-entanglement test  S / ln<N>  (KL predicts -> 1).
  std::vector<double>      _winSum, _winSumSq;

  // --- forward/backward joint multiplicity (binned for the MI) --------
  // Joint counts over (N_F/FB_BIN, N_B/FB_BIN).  Binning keeps the
  // occupied-cell count K small enough that the plug-in MI bias
  // ~K/(2 N_events) stays negligible at typical run sizes.
  // One joint table PER eta-GAP: hemisphere hadrons must satisfy
  // |eta| > gap/2, so the MI-vs-gap curve measures how the shared
  // information decays with rapidity separation (long-range vs
  // short-range) — a string-vs-cluster discriminant.
  static const int FB_BIN = 4;
  std::vector<double> _fbGaps;                            // 0, 0.5, 1, 1.5, 2
  std::vector<std::map<std::pair<int,int>,double>> _fbJointByGap;

  // --- event-level information recovery --------------------------------
  // N_MPI = number of DISTINCT MPI scatters feeding the studied hadrons
  // (distinct mpiIndex >= 0 in the tags).  I(N_MPI ; N_ch) measures how
  // well the final multiplicity encodes the MPI activity — the
  // information-theoretic quality of a small-system centrality estimator.
  // Pythia-only physics: the HepMC path never assigns MPI tags, so for
  // Herwig the table is degenerate and I = 0 by construction.
  std::map<std::pair<int,int>,double> _jMpiMult;

  // --- per-stage occupancy over a coarse (eta, pT) grid ---------------
  static const int ETA_BINS = 40;          // eta in [-10, 10]
  static const int PT_BINS  = 30;          // pT  in [0, 6] GeV, overflow
                                           // folded into the last bin
  std::vector<std::vector<double>> _stageGrid;   // [stage][cell]
  std::vector<double>              _stageCount;  // total objects per stage
  // Sum over events of the PER-EVENT occupancy entropy at each stage.
  // <S_event> (intra-event spread) vs the ensemble S_occ (which also
  // contains event-to-event diversity); their difference is itself an
  // information measure of how much events differ from each other.
  std::vector<double>              _stageEventS;

  // --- information-recovery joint tables (counts) ---------------------
  std::map<std::pair<int,int>,double> _jPartonSpecies;        // all finals
  std::map<std::pair<int,int>,double> _jPartonSpeciesPrimary; // primary only
  std::map<std::pair<int,int>,double> _jPartonSpeciesDecay;   // decay only
  std::map<std::pair<int,int>,double> _jPartonPt;             // all finals
  std::map<std::pair<int,int>,double> _jOriginSpecies;        // all finals

  // mutable so finalize() can refresh the derived stage histograms from
  // const accessors (histograms() / report()).
  mutable std::map<std::string,Hist1D> _hist;
};

} // namespace CAP

#endif // CAP__EntropyObservables
