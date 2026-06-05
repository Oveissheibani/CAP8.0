/* **********************************************************************
 * CAP::EntropyObservables — implementation.  See the header for the
 * physics rationale and the estimator definitions.
 * ********************************************************************/
#include "EntropyObservables.hpp"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <set>
#include <sstream>

namespace CAP
{

// ---------------------------------------------------------------------------
//  construction
// ---------------------------------------------------------------------------
EntropyObservables::EntropyObservables()
{
  // |eta| half-windows for the multiplicity-entropy scan; -1 = full.
  _etaWindows  = { 0.5, 1.0, 2.0, -1.0 };
  _windowNames = { "eta05", "eta10", "eta20", "full" };
  _winSum.assign(_etaWindows.size(), 0.0);
  _winSumSq.assign(_etaWindows.size(), 0.0);

  // eta-gap scan for the forward/backward mutual information.
  _fbGaps = { 0.0, 0.5, 1.0, 1.5, 2.0 };
  _fbJointByGap.assign(_fbGaps.size(),
                       std::map<std::pair<int,int>,double>());

  const std::size_t nStages = stageList().size();
  _stageGrid.assign(nStages,
                    std::vector<double>(static_cast<std::size_t>(ETA_BINS) *
                                        static_cast<std::size_t>(PT_BINS), 0.0));
  _stageCount.assign(nStages, 0.0);
  _stageEventS.assign(nStages, 0.0);
}

const std::vector<Stage> & EntropyObservables::stageList()
{
  static const std::vector<Stage> S = {
    Stage::HardProcess, Stage::MPI, Stage::ISR, Stage::FSR,
    Stage::BeamRemnants, Stage::PartonsPreHadronization,
    Stage::PrimaryHadrons, Stage::DecayProducts, Stage::FinalState };
  return S;
}

// ---------------------------------------------------------------------------
//  small classifiers
// ---------------------------------------------------------------------------
bool EntropyObservables::isChargedFinal(int pdg)
{
  switch (std::abs(pdg))
    {
    case 11: case 13: case 15:                       // e, mu, tau
    case 211: case 321: case 2212:                   // pi, K, p
    case 3222: case 3112: case 3312: case 3334:      // Sigma+-, Xi-, Omega-
      return true;
    default:
      return false;
    }
}

int EntropyObservables::speciesGroup(int pdg)
{
  switch (std::abs(pdg))
    {
    case 211:           return 0;   // pion
    case 321:           return 1;   // kaon
    case 2212:          return 2;   // proton
    case 11: case 13:   return 3;   // lepton
    default:            return 4;   // other
    }
}

int EntropyObservables::ptBin(double pt)
{
  static const double edges[] = { 0.25, 0.5, 0.75, 1.0, 1.5, 2.0, 3.0, 5.0 };
  const int n = static_cast<int>(sizeof(edges) / sizeof(edges[0]));
  for (int i = 0; i < n; ++i)
    if (pt < edges[i]) return i;
  return n;                                          // >= 5 GeV
}

// ---------------------------------------------------------------------------
//  estimators
// ---------------------------------------------------------------------------
std::pair<double,double>
EntropyObservables::shannonEntropy(const std::vector<double> & counts)
{
  double N = 0.0;
  long   K = 0;
  for (double c : counts)
    if (c > 0.0) { N += c; ++K; }
  if (N <= 0.0 || K <= 1) return std::make_pair(0.0, 0.0);

  double S = 0.0, S2 = 0.0;
  for (double c : counts)
    {
    if (c <= 0.0) continue;
    const double p = c / N;
    const double l = std::log(p);
    S  -= p * l;
    S2 += p * l * l;
    }
  // Miller–Madow bias correction: the plug-in estimator is biased LOW by
  // ~(K-1)/(2N); add it back.  K = occupied cells.
  const double Smm = S + static_cast<double>(K - 1) / (2.0 * N);
  // Statistical error of the plug-in estimator:
  //   Var(S) ≈ ( sum p ln^2 p - S^2 ) / N
  const double var = (S2 - S * S) / N;
  const double err = var > 0.0 ? std::sqrt(var) : 0.0;
  return std::make_pair(Smm, err);
}

double EntropyObservables::renyi2Entropy(const std::vector<double> & counts)
{
  double N = 0.0;
  for (double c : counts) if (c > 0.0) N += c;
  if (N <= 1.0) return 0.0;
  // Unbiased estimator of the power sum  sum_i p_i^2:
  //   E[ sum c(c-1) ] = N(N-1) sum p^2   for multinomial counts.
  double s = 0.0;
  for (double c : counts) if (c > 0.0) s += c * (c - 1.0);
  const double p2 = s / (N * (N - 1.0));
  if (p2 <= 0.0) return 0.0;
  return -std::log(p2);
}

std::pair<double,double>
EntropyObservables::mutualInformation(
    const std::map<std::pair<int,int>,double> & joint)
{
  if (joint.empty()) return std::make_pair(0.0, 0.0);

  std::map<int,double> mx, my;
  double N = 0.0;
  for (const auto & kv : joint)
    {
    mx[kv.first.first]  += kv.second;
    my[kv.first.second] += kv.second;
    N += kv.second;
    }
  if (N <= 0.0) return std::make_pair(0.0, 0.0);

  auto entropyOf = [](const std::map<int,double> & m)
    {
    std::vector<double> c;
    c.reserve(m.size());
    for (const auto & kv : m) c.push_back(kv.second);
    return shannonEntropy(c);
    };
  std::vector<double> cj;
  cj.reserve(joint.size());
  for (const auto & kv : joint) cj.push_back(kv.second);

  const double Sx  = entropyOf(mx).first;
  const double Sy  = entropyOf(my).first;
  const double Sxy = shannonEntropy(cj).first;

  const double LN2 = std::log(2.0);
  const double I   = (Sx + Sy - Sxy) / LN2;          // bits, MM-corrected

  // Residual plug-in bias scale (bits) — quote it so the reader can judge
  // whether the sample is big enough: bias ~ (Kx-1)(Ky-1)/(2N ln2).
  const double Kx = static_cast<double>(mx.size());
  const double Ky = static_cast<double>(my.size());
  const double bias = (Kx - 1.0) * (Ky - 1.0) / (2.0 * N * LN2);
  return std::make_pair(I, bias);
}

// ---------------------------------------------------------------------------
//  accumulation
// ---------------------------------------------------------------------------
Hist1D & EntropyObservables::H(const std::string & name)
{
  std::map<std::string,Hist1D>::iterator it = _hist.find(name);
  if (it != _hist.end()) return it->second;
  // Multiplicity-style axes: one unit per bin, generous range.
  Hist1D h(name, name, 600, 0.0, 600.0);
  return _hist.insert(std::make_pair(name, h)).first->second;
}

void EntropyObservables::accumulate(const EventHistory & history,
                                    const std::vector<ProvenanceTag> & tags)
{
  ++_events;

  // ---- charged final-state multiplicities ------------------------------
  std::vector<int> nW(_etaWindows.size(), 0);
  std::vector<int> nFg(_fbGaps.size(), 0), nBg(_fbGaps.size(), 0);
  const std::vector<int> finals = history.finalState();
  for (int idx : finals)
    {
    const ParticleNode & n = history.node(idx);
    if (!isChargedFinal(n.pdg)) continue;
    const double eta = n.eta();
    for (std::size_t w = 0; w < _etaWindows.size(); ++w)
      if (_etaWindows[w] < 0.0 || std::fabs(eta) < _etaWindows[w]) ++nW[w];
    for (std::size_t g = 0; g < _fbGaps.size(); ++g)
      {
      if (eta >  _fbGaps[g] / 2.0) ++nFg[g];
      if (eta < -_fbGaps[g] / 2.0) ++nBg[g];
      }
    }
  for (std::size_t w = 0; w < _etaWindows.size(); ++w)
    {
    H("ent_mult_" + _windowNames[w]).fill(nW[w] + 0.5);
    _winSum[w]   += nW[w];
    _winSumSq[w] += static_cast<double>(nW[w]) * nW[w];
    }
  H("ent_fb_NF").fill(nFg[0] + 0.5);
  H("ent_fb_NB").fill(nBg[0] + 0.5);
  for (std::size_t g = 0; g < _fbGaps.size(); ++g)
    _fbJointByGap[g][std::make_pair(nFg[g] / FB_BIN, nBg[g] / FB_BIN)] += 1.0;

  // ---- per-stage occupancy over the coarse (eta, pT) grid --------------
  const std::vector<Stage> & stages = stageList();
  // Per-event occupancy (sparse) so we can also accumulate the PER-EVENT
  // entropy <S_event> next to the ensemble occupancy entropy.
  std::vector<std::map<int,double>> evtGrid(stages.size());
  for (int i = 0; i < history.size(); ++i)
    {
    const ParticleNode & n = history.node(i);
    for (std::size_t s = 0; s < stages.size(); ++s)
      {
      // The FinalState ring is "what a detector records" = the isFinal
      // flag, NOT the production-stage label (a final pion produced at
      // hadronization carries productionStage PrimaryHadrons).
      const bool inRing = (stages[s] == Stage::FinalState)
                          ? n.isFinal
                          : (n.stage == stages[s]);
      if (!inRing) continue;
      double eta = n.eta();
      if (eta < -10.0) eta = -10.0;
      if (eta >= 10.0) eta =  9.999;
      int be = static_cast<int>((eta + 10.0) / 20.0 * ETA_BINS);
      if (be < 0) be = 0;
      if (be >= ETA_BINS) be = ETA_BINS - 1;
      double pt = n.pt();
      int bp = static_cast<int>(pt / 6.0 * PT_BINS);
      if (bp < 0) bp = 0;
      if (bp >= PT_BINS) bp = PT_BINS - 1;             // overflow folded
      const int cell = be * PT_BINS + bp;
      _stageGrid[s][static_cast<std::size_t>(cell)] += 1.0;
      _stageCount[s] += 1.0;
      evtGrid[s][cell] += 1.0;
      }
    }
  for (std::size_t s = 0; s < stages.size(); ++s)
    {
    if (evtGrid[s].empty()) continue;
    std::vector<double> c;
    c.reserve(evtGrid[s].size());
    for (const auto & kv : evtGrid[s]) c.push_back(kv.second);
    _stageEventS[s] += shannonEntropy(c).first;
    }

  // ---- information-recovery joint tables -------------------------------
  std::set<int> mpiVertices;
  for (std::size_t k = 0; k < tags.size(); ++k)
    {
    const ProvenanceTag & t = tags[k];
    if (t.finalIndex < 0 || t.finalIndex >= history.size()) continue;
    const ParticleNode & n = history.node(t.finalIndex);

    const int sp = speciesGroup(t.finalPdg);
    const int pb = ptBin(n.pt());
    const int pc = static_cast<int>(classifyInitiatingParton(t));
    const int oc = static_cast<int>(classifyOrigin(t));

    _jPartonSpecies[std::make_pair(pc, sp)] += 1.0;
    _jPartonPt     [std::make_pair(pc, pb)] += 1.0;
    _jOriginSpecies[std::make_pair(oc, sp)] += 1.0;
    if (t.isPrimaryHadron)
      _jPartonSpeciesPrimary[std::make_pair(pc, sp)] += 1.0;
    else if (t.isFromDecay)
      _jPartonSpeciesDecay  [std::make_pair(pc, sp)] += 1.0;
    if (t.mpiIndex >= 0) mpiVertices.insert(t.mpiIndex);
    }

  // Event-level: how well does the final multiplicity encode the MPI
  // activity?  N_MPI = distinct MPI scatters among the studied hadrons;
  // N_ch = total studied hadrons (binned as in the FB tables).
  const int nMpi = static_cast<int>(mpiVertices.size());
  const int nCh  = static_cast<int>(tags.size());
  _jMpiMult[std::make_pair(nMpi, nCh / FB_BIN)] += 1.0;
  H("ent_nmpi").fill(nMpi + 0.5);
}

// ---------------------------------------------------------------------------
//  finalisation — derived stage-profile histograms
// ---------------------------------------------------------------------------
void EntropyObservables::finalize() const
{
  const std::vector<Stage> & stages = stageList();
  const int nS = static_cast<int>(stages.size());

  Hist1D hS   ("ent_stage_S",
               "occupancy entropy per stage [nats]",     nS, 0.0, nS);
  Hist1D hN   ("ent_stage_meanN",
               "mean object count per stage",            nS, 0.0, nS);
  Hist1D hTot ("ent_stage_total",
               "occupancy entropy + ln(mean count) [nats]", nS, 0.0, nS);

  for (int s = 0; s < nS; ++s)
    {
    const std::pair<double,double> e =
        shannonEntropy(_stageGrid[static_cast<std::size_t>(s)]);
    const double meanN = _events > 0
        ? _stageCount[static_cast<std::size_t>(s)] / _events : 0.0;
    hS.counts[static_cast<std::size_t>(s)] = e.first;
    hN.counts[static_cast<std::size_t>(s)] = meanN;
    // Coarse "total entropy" proxy: S_occ + ln<N>  (the ideal-gas style
    // decomposition into a where-in-phase-space term and a how-many term).
    hTot.counts[static_cast<std::size_t>(s)] =
        meanN > 0.0 ? e.first + std::log(meanN) : 0.0;
    hS.entries = hN.entries = hTot.entries = nS;
    }
  // Per-event mean entropy at each stage (intra-event spread only).
  Hist1D hEvt ("ent_stage_Sevent",
               "mean per-event occupancy entropy per stage [nats]",
               nS, 0.0, nS);
  for (int s = 0; s < nS; ++s)
    hEvt.counts[static_cast<std::size_t>(s)] =
        _events > 0 ? _stageEventS[static_cast<std::size_t>(s)] / _events : 0.0;
  hEvt.entries = nS;

  // Forward/backward mutual information vs eta gap (bits per gap point).
  const int nG = static_cast<int>(_fbGaps.size());
  Hist1D hGap ("ent_fb_mi_gap",
               "I(N_F;N_B) [bits] vs eta gap", nG, -0.25,
               -0.25 + 0.5 * nG);          // bin centers at 0, 0.5, 1, ...
  for (int g = 0; g < nG; ++g)
    hGap.counts[static_cast<std::size_t>(g)] =
        mutualInformation(_fbJointByGap[static_cast<std::size_t>(g)]).first;
  hGap.entries = nG;

  _hist["ent_stage_S"]      = hS;
  _hist["ent_stage_meanN"]  = hN;
  _hist["ent_stage_total"]  = hTot;
  _hist["ent_stage_Sevent"] = hEvt;
  _hist["ent_fb_mi_gap"]    = hGap;
}

const std::map<std::string,Hist1D> & EntropyObservables::histograms() const
{
  finalize();
  return _hist;
}

// ---------------------------------------------------------------------------
//  text report — markers all carry the unique "entropy:" prefix
// ---------------------------------------------------------------------------
std::string EntropyObservables::report() const
{
  finalize();
  std::ostringstream os;
  os << std::fixed << std::setprecision(4);
  os << "EntropyObservables — " << _events
     << " event(s)  [charged final-state particles]\n";

  // ---- 1. multiplicity entropy -----------------------------------------
  // Columns: S (Shannon, MM-corrected), stat err, S2 (Renyi-2, unbiased),
  // <N>, and the Kharzeev-Levin maximal-entanglement ratio S/ln<N> (-> 1
  // for a maximally entangled state).
  os << "  entropy: multiplicity entropy (nats):\n";
  static const char * windowLabels[] =
      { "|eta|<0.5", "|eta|<1.0", "|eta|<2.0", "full" };
  for (std::size_t w = 0; w < _windowNames.size(); ++w)
    {
    std::map<std::string,Hist1D>::const_iterator it =
        _hist.find("ent_mult_" + _windowNames[w]);
    if (it == _hist.end()) continue;
    std::vector<double> c = it->second.counts;
    c.push_back(it->second.overflow);          // overflow is one more cell
    const std::pair<double,double> e = shannonEntropy(c);
    const double s2    = renyi2Entropy(c);
    const double meanN = _events > 0 ? _winSum[w] / _events : 0.0;
    const double ratio = meanN > 1.0 ? e.first / std::log(meanN) : 0.0;
    os << "    " << std::setw(16) << std::left << windowLabels[w]
       << std::setw(9) << std::right << e.first
       << "   err "   << e.second
       << "   S2 "    << s2
       << "   meanN " << meanN
       << "   S/lnN " << ratio << "\n";
    }

  // ---- 2. stage entropy profile ----------------------------------------
  os << "  entropy: stage profile (occupancy nats | mean count | total nats"
        " | per-event nats):\n";
  const std::vector<Stage> & stages = stageList();
  const Hist1D & hS = _hist["ent_stage_S"];
  const Hist1D & hN = _hist["ent_stage_meanN"];
  const Hist1D & hT = _hist["ent_stage_total"];
  const Hist1D & hE = _hist["ent_stage_Sevent"];
  for (std::size_t s = 0; s < stages.size(); ++s)
    {
    if (hN.counts[s] <= 0.0) continue;         // stage absent in this source
    os << "    " << std::setw(24) << std::left << stageName(stages[s])
       << std::setw(9)  << std::right << hS.counts[s]
       << std::setw(12) << hN.counts[s]
       << std::setw(12) << hT.counts[s]
       << std::setw(12) << hE.counts[s] << "\n";
    }

  // ---- 3. information recovery -------------------------------------------
  os << "  entropy: information recovery (bits):\n";
  // FB mutual information vs eta gap: how the shared information decays
  // with rapidity separation (long-range vs short-range correlation).
  for (std::size_t g = 0; g < _fbGaps.size(); ++g)
    {
    if (_fbJointByGap[g].empty()) continue;
    const std::pair<double,double> mi = mutualInformation(_fbJointByGap[g]);
    std::ostringstream name;
    name << "I(NF;NB)|gap=" << std::setprecision(1) << std::fixed
         << _fbGaps[g];
    os << "    " << std::setw(32) << std::left << name.str()
       << std::setw(9) << std::right << std::setprecision(4) << mi.first
       << "   bias " << mi.second << "\n";
    }
  struct Row { const char * name;
               const std::map<std::pair<int,int>,double> * j; };
  const Row rows[] = {
    { "I(NMPI;Nch)",                   &_jMpiMult              },
    { "I(initParton;species)",         &_jPartonSpecies        },
    { "I(initParton;species)|Primary", &_jPartonSpeciesPrimary },
    { "I(initParton;species)|Decay",   &_jPartonSpeciesDecay   },
    { "I(initParton;pT)",              &_jPartonPt             },
    { "I(origin;species)",             &_jOriginSpecies        },
  };
  for (const Row & r : rows)
    {
    if (r.j->empty()) continue;
    const std::pair<double,double> mi = mutualInformation(*r.j);
    os << "    " << std::setw(32) << std::left << r.name
       << std::setw(9) << std::right << mi.first
       << "   bias " << mi.second << "\n";
    }
  return os.str();
}

} // namespace CAP
