/* **********************************************************************
 * CAP::PairProvenanceObservables — implementation.  See header for design.
 * ********************************************************************/
#include "PairProvenanceObservables.hpp"

#include <algorithm> // std::max
#include <cmath>     // std::sqrt
#include <cstdlib>   // std::abs
#include <sstream>
#include <iomanip>

namespace CAP
{

namespace
{
const double PI = 3.14159265358979323846;

// Wrap an azimuthal difference into the half-open range [-pi, pi) — the
// conventional dPhi range.  Half-open so a difference of exactly pi maps
// to -pi (an in-range bin) instead of falling into histogram overflow.
double wrapPi(double d)
{
  while (d >=  PI) d -= 2.0 * PI;
  while (d <  -PI) d += 2.0 * PI;
  return d;
}

// dEta / dPhi histogram axes.
const int    DETA_NBINS = 80;   const double DETA_LO = -8.0;  const double DETA_HI = 8.0;
const int    DPHI_NBINS = 72;   const double DPHI_LO = -PI;   const double DPHI_HI = PI;

// Group a resonance PDG into the major contributors that show up in pion
// pairs.  Anything unrecognised falls into "Other".
std::string classifyResonanceType(int pdg)
{
  switch (std::abs(pdg))
    {
    case 113:  case 213:                                    return "Rho";
    case 223:                                               return "Omega";
    case 333:                                               return "Phi";
    case 313:  case 323:                                    return "KStar";
    case 221:                                               return "Eta";
    case 331:                                               return "EtaPrime";
    case 1114: case 2114: case 2214: case 2224:             return "Delta";
    default:                                                return "Other";
    }
}

// Charge tag derived from two final-state PDGs.  Same sign (both > 0 or
// both < 0) -> "SS"; opposite -> "OS".
std::string pairChargeTag(int pdgA, int pdgB)
{
  return ((pdgA > 0) == (pdgB > 0)) ? "SS" : "OS";
}

// MPI relationship at the pair level.  When both hadrons have an MPI
// ancestor and it's the same one -> SameMPI; when both have one but
// different -> CrossMPI; when only one carries an MPI ancestor ->
// OneSideMPI; neither -> NoMPI (both came from primary hard scatter or
// beam remnants).
std::string pairMPITag(const ProvenanceTag & a, const ProvenanceTag & b)
{
  const bool aMpi = a.mpiIndex >= 0;
  const bool bMpi = b.mpiIndex >= 0;
  if (aMpi && bMpi) return (a.mpiIndex == b.mpiIndex) ? "SameMPI" : "CrossMPI";
  if (aMpi || bMpi) return "OneSideMPI";
  return "NoMPI";
}

// Pair-level shower lineage.  Each hadron is tagged by which showers its
// ancestry walks through; the pair tag summarises:
//   BothISR     — both hadrons walk through ISR (initial-state radiation)
//   BothFSR     — both walk through FSR (final-state radiation only)
//   MixedShower — one ISR-only, one FSR-only (or any mismatch)
//   NoShower    — neither hadron has any ISR/FSR ancestor
// A hadron whose ancestry walks through both ISR and FSR is treated as
// FSR for the purposes of pair classification (the most-final shower).
std::string pairShowerTag(const ProvenanceTag & a, const ProvenanceTag & b)
{
  auto leading = [](const ProvenanceTag & t) -> int {
    // 1 = FSR-touching, 2 = ISR-only, 0 = no shower
    if (t.fromFSR)  return 1;
    if (t.fromISR)  return 2;
    return 0;
  };
  const int la = leading(a);
  const int lb = leading(b);
  if (la == 0 && lb == 0) return "NoShower";
  if (la == 1 && lb == 1) return "BothFSR";
  if (la == 2 && lb == 2) return "BothISR";
  return "MixedShower";
}

// Heavy-flavour pair tag.  Bottom outranks charm (b -> c -> light).
std::string pairHeavyFlavourTag(const ProvenanceTag & a, const ProvenanceTag & b)
{
  const bool ab = a.fromBottomChain, bb = b.fromBottomChain;
  const bool ac = a.fromCharmChain,  bc = b.fromCharmChain;
  if (ab && bb)            return "BothBottom";
  if (ab || bb)            return "OneBottom";
  if (ac && bc)            return "BothCharm";
  if (ac || bc)            return "OneCharm";
  return "NoHeavyFlavour";
}

// Multiplicity bin label.  Cuts are taken from the instance (see
// _multLow / _multHigh) so the user can pick thresholds appropriate to
// the beam, energy, and selection — defaults are 20 / 80, tuned for pp
// at 13 TeV soft QCD.
std::string multBinTag(int mult, int low, int high)
{
  if (mult <  low)  return "LowMult";
  if (mult <  high) return "MidMult";
  return              "HighMult";
}

// Flavor-mixing classification of a pair using the two hadrons' hard-
// process parton ancestors.  Five mutually-exclusive categories — the
// minimum that surfaces the physics-relevant cases without exploding
// into a 25-cell joint table:
//
//   SameQuark   — both partons share |PDG| AND are quarks (1..6).
//                 Equal-flavour vertex (e.g. u u, s s, c c).
//   SameGen     — different |PDG| but both partons are quarks AND in the
//                 same generation: (u,d), (c,s), (t,b).
//   CrossGen    — both quarks, different generations: (u,s), (d,c), ...
//   WithGluon   — at least one parton is a gluon (PDG 21).  This is the
//                 dominant case in soft QCD once MPI is on, because MPI
//                 scatters are mostly gg.
//   Partonless  — at least one hadron has no hard-process parton ancestor
//                 (hardPartonPdg == 0).  Lives in MPI-only / beam-remnant-
//                 only / pure-shower lineages.
//
// The categorisation is by ABSOLUTE PDG: a u-ubar pair (which would be
// the "opposite-sign" version of u-u) still classifies as SameQuark,
// because at the hard-process level both endpoints are u-flavoured.  If
// you want the charge-sign breakdown, that's already in pair_*_SS / _OS.
// Transverse spherocity S_0 — pT-weighted, in [0, 1].  Low (~0) = pencil-
// like / back-to-back jet event; high (~1) = isotropic event.  Computed
// from the user-supplied (px, py) list, which by construction here is
// the studied-species + acceptance-window-filtered hadron list.  Cost:
// O(N * STEPS) per call; STEPS = 100 matches the existing CAP analyzer.
double transverseSpherocity(const std::vector<double> & px,
                            const std::vector<double> & py)
{
  const std::size_t N = px.size();
  if (N < 2) return 1.0;            // sentinel: cannot define S0
  double denom = 0.0;
  for (std::size_t i = 0; i < N; ++i)
    denom += std::sqrt(px[i]*px[i] + py[i]*py[i]);
  if (!(denom > 0.0)) return 1.0;
  const int    nSteps = 100;
  const double PI = 3.14159265358979323846;
  const double dphi = PI / static_cast<double>(nSteps);
  double sMin = 1.0e10;
  for (int k = 0; k < nSteps; ++k)
    {
    const double a  = k * dphi;
    const double nx = std::cos(a);
    const double ny = std::sin(a);
    double num = 0.0;
    for (std::size_t i = 0; i < N; ++i)
      num += std::fabs(ny * px[i] - nx * py[i]);
    const double r  = num / denom;
    const double r2 = r * r;
    if (r2 < sMin) sMin = r2;
    }
  // Clamp to [0, 1] — numerical drift can sneak above the upper edge.
  if (sMin < 0.0) sMin = 0.0;
  if (sMin > 1.0) sMin = 1.0;
  return sMin;
}

// Three-way event-shape bin label, tuned via instance thresholds.
std::string spheroBinTag(double s0, double low, double high)
{
  if (s0 < low)  return "JetLike";
  if (s0 < high) return "MidShape";
  return           "Isotropic";
}

std::string pairFlavourMixTag(int pdgA, int pdgB)
{
  auto gen = [](int pdg) -> int {
    const int a = std::abs(pdg);
    if (a == 1 || a == 2) return 1;     // u, d
    if (a == 3 || a == 4) return 2;     // s, c
    if (a == 5 || a == 6) return 3;     // b, t
    return 0;
  };
  if (pdgA == 0 || pdgB == 0) return "Partonless";
  if (std::abs(pdgA) == 21 || std::abs(pdgB) == 21) return "WithGluon";
  if (std::abs(pdgA) == std::abs(pdgB)) return "SameQuark";
  if (gen(pdgA) == gen(pdgB))           return "SameGen";
  return "CrossGen";
}
} // namespace

// ----------------------------------------------------------------------
std::string pairClassName(PairClass c)
{
  switch (c)
    {
    case PairClass::SameResonance:  return "SameResonance";
    case PairClass::SameWeakParent: return "SameWeakParent";
    case PairClass::SharedParton:   return "SharedParton";
    case PairClass::Unrelated:      return "Unrelated";
    }
  return "Unrelated";
}

// ----------------------------------------------------------------------
PairClass classifyPair(const ProvenanceTag & a, const ProvenanceTag & b)
{
  // A shared decaying parent is the most specific relationship.
  if (sharesDecayParent(a, b))
    return a.isFromResonance ? PairClass::SameResonance
                             : PairClass::SameWeakParent;
  // Otherwise, do they descend from a common pre-hadronization parton?
  if (sharesPartonAncestor(a, b))
    return PairClass::SharedParton;
  return PairClass::Unrelated;
}

// ======================================================================
//  PairProvenanceObservables
// ======================================================================
// Match ProvenanceObservables.cpp's canonicalisation rule.
static std::vector<int> _canonSpeciesPair(const std::vector<int> & raw)
{
  std::vector<int> out;
  bool has_wild = false;
  for (int p : raw)
    {
    const int a = (p < 0 ? -p : p);
    if (a == 0) { has_wild = true; continue; }
    if (std::find(out.begin(), out.end(), a) == out.end()) out.push_back(a);
    }
  if (has_wild || out.empty()) return std::vector<int>{0};
  return out;
}

PairProvenanceObservables::PairProvenanceObservables(int speciesPdg,
                                                     double ptMin,  double ptMax,
                                                     double etaMin, double etaMax,
                                                     int multLow, int multHigh,
                                                     double spheroLow, double spheroHigh)
: _species(_canonSpeciesPair({speciesPdg})),
  _speciesPdg(_species.front()),
  _ptMin (ptMin), _ptMax (ptMax),
  _etaMin(etaMin), _etaMax(etaMax),
  _multLow (multLow), _multHigh(multHigh),
  _spheroLow(spheroLow), _spheroHigh(spheroHigh)
{ }

PairProvenanceObservables::PairProvenanceObservables(const std::vector<int> & list,
                                                     double ptMin,  double ptMax,
                                                     double etaMin, double etaMax,
                                                     int multLow, int multHigh,
                                                     double spheroLow, double spheroHigh)
: _species(_canonSpeciesPair(list)),
  _speciesPdg(_species.front()),
  _ptMin (ptMin), _ptMax (ptMax),
  _etaMin(etaMin), _etaMax(etaMax),
  _multLow (multLow), _multHigh(multHigh),
  _spheroLow(spheroLow), _spheroHigh(spheroHigh)
{ }

std::string PairProvenanceObservables::speciesPairSuffix(int a, int b) const
{
  if (_species.size() <= 1) return "";
  // Canonical order — smaller PDG first.
  const int p1 = std::min(a, b), p2 = std::max(a, b);
  return "_S" + std::to_string(p1) + "x" + std::to_string(p2);
}

Hist1D & PairProvenanceObservables::H(const std::string & name)
{
  std::map<std::string,Hist1D>::iterator it = _hist.find(name);
  if (it != _hist.end()) return it->second;

  int nb; double lo, hi;
  if (name.rfind("deta_", 0) == 0)
    { nb = DETA_NBINS; lo = DETA_LO; hi = DETA_HI; }
  else if (name.rfind("mass_", 0) == 0)
    { nb = 150; lo = 0.0; hi = 3.0; }      // pair invariant mass, 20 MeV bins
  else if (name.rfind("common_ancestor_depth", 0) == 0)
    { nb = 11; lo = -5.0; hi = 105.0; }    // one bin per Stage value
  else if (name.rfind("pair_decay_depth", 0) == 0)
    { nb = 21; lo = -0.5; hi = 20.5; }     // integer-valued decay-chain depth
  else if (name.rfind("event_multiplicity", 0) == 0)
    { nb = 200; lo = 0.0; hi = 600.0; }    // studied hadrons per event
  else if (name.rfind("event_spherocity", 0) == 0)
    { nb = 100; lo = 0.0; hi = 1.0; }      // transverse spherocity S0
  else  // dphi_*
    { nb = DPHI_NBINS; lo = DPHI_LO; hi = DPHI_HI; }

  _hist[name] = Hist1D(name, name, nb, lo, hi);
  return _hist[name];
}

long PairProvenanceObservables::pairsInClass(PairClass c) const
{
  const int i = static_cast<int>(c);
  return (i >= 0 && i < 4) ? _count[i] : 0;
}

void PairProvenanceObservables::accumulate(const EventHistory &               history,
                                           const std::vector<ProvenanceTag> & tags)
{
  _events++;

  // Collect the studied final hadrons with their kinematics, once.
  // Each part remembers WHICH species in the configured list it
  // matched (0 in single-mode "all"); the pair loop uses that to pick
  // a per-species-pair histogram suffix.
  struct P
  {
    const ProvenanceTag * tag;
    int    species;        // |pdg| matched, or 0 in single-mode "all"
    double eta, phi;
    double px, py, pz, e;
  };
  std::vector<P> parts;
  parts.reserve(tags.size());
  for (const ProvenanceTag & t : tags)
    {
    int matched = -1;
    for (int s : _species)
      {
      if (s == 0 || std::abs(t.finalPdg) == s) { matched = s; break; }
      }
    if (matched < 0) continue;
    if (t.finalIndex < 0 || t.finalIndex >= history.size()) continue;
    const ParticleNode & node = history.node(t.finalIndex);
    const double pt  = node.pt();
    const double eta = node.eta();
    if (pt  < _ptMin  || pt  > _ptMax)  continue;
    if (eta < _etaMin || eta > _etaMax) continue;
    P p;
    p.tag = &t;
    p.species = matched;
    p.eta = eta;         p.phi = node.phi();
    p.px  = node.px;     p.py  = node.py;
    p.pz  = node.pz;     p.e   = node.e;
    parts.push_back(p);
    }

  // Event multiplicity = # studied hadrons in this event.  Used to bin
  // every pair into a coarse Low / Mid / High class so that the pair
  // observables can be inspected as a function of activity.
  const int nMult = static_cast<int>(parts.size());
  H("event_multiplicity").fill(static_cast<double>(nMult));
  const std::string mb = multBinTag(nMult, _multLow, _multHigh);

  // Transverse spherocity S0 for this event — computed once over the
  // studied-species (px, py) list and used to bin every pair into
  // JetLike / MidShape / Isotropic.  The S0 value is also written to a
  // diagnostic histogram so the user can see its distribution and check
  // the bin thresholds are sensible for their sample.
  std::vector<double> pxList, pyList;
  pxList.reserve(parts.size());
  pyList.reserve(parts.size());
  for (const P & p : parts) { pxList.push_back(p.px); pyList.push_back(p.py); }
  const double s0 = transverseSpherocity(pxList, pyList);
  H("event_spherocity").fill(s0);
  const std::string sb = spheroBinTag(s0, _spheroLow, _spheroHigh);

  // Every unordered pair.
  const size_t n = parts.size();
  for (size_t i = 0; i < n; ++i)
    {
    for (size_t j = i + 1; j < n; ++j)
      {
      // Per-pair species-combination suffix.  Single-species mode emits
      // bare names (suffix == ""); multi-species mode tags every fill
      // with "_S<a>x<b>" so pi-pi, pi-K, K-K, etc. stay separable.
      const std::string sps =
          speciesPairSuffix(parts[i].species, parts[j].species);
      auto Hs = [&](const std::string & name) -> Hist1D & {
        return H(name + sps);
      };
      const PairClass pc = classifyPair(*parts[i].tag, *parts[j].tag);
      const double deta = parts[i].eta - parts[j].eta;
      const double dphi = wrapPi(parts[i].phi - parts[j].phi);

      // Pair invariant mass — for SameResonance pairs this clusters at
      // the rho / K* / omega / phi / Delta masses, giving a direct,
      // visible check that the resonance tagger is working.
      const double Ep = parts[i].e  + parts[j].e;
      const double xp = parts[i].px + parts[j].px;
      const double yp = parts[i].py + parts[j].py;
      const double zp = parts[i].pz + parts[j].pz;
      const double m2 = Ep*Ep - xp*xp - yp*yp - zp*zp;
      const double m  = m2 > 0.0 ? std::sqrt(m2) : 0.0;

      Hs("deta_pair_All").fill(deta);
      Hs("dphi_pair_All").fill(dphi);
      Hs("mass_pair_All").fill(m);
      Hs("deta_pair_" + pairClassName(pc)).fill(deta);
      Hs("dphi_pair_" + pairClassName(pc)).fill(dphi);
      Hs("mass_pair_" + pairClassName(pc)).fill(m);

      // ---- Same-Sign / Opposite-Sign breakdown --------------------------
      // Applies to every class; this is the direct balance-function split.
      const std::string sign = pairChargeTag(parts[i].tag->finalPdg,
                                             parts[j].tag->finalPdg);
      Hs("deta_pair_All_"             + sign).fill(deta);
      Hs("dphi_pair_All_"             + sign).fill(dphi);
      Hs("mass_pair_All_"             + sign).fill(m);
      Hs("deta_pair_" + pairClassName(pc) + "_" + sign).fill(deta);
      Hs("dphi_pair_" + pairClassName(pc) + "_" + sign).fill(dphi);
      Hs("mass_pair_" + pairClassName(pc) + "_" + sign).fill(m);

      // ---- Resonance-type sub-classes (only when SameResonance) --------
      // Names the peaks visible in the mass overlay: rho, omega, phi, K*, ...
      if (pc == PairClass::SameResonance)
        {
        const std::string rt =
          classifyResonanceType(parts[i].tag->resonancePdg);
        H("dphi_pair_SameResonance_" + rt).fill(dphi);
        H("deta_pair_SameResonance_" + rt).fill(deta);
        H("mass_pair_SameResonance_" + rt).fill(m);
        }

      // ---- SharedParton depth — hard-process vs shower-only ------------
      // HardProcessShared: both pions descend from the SAME hard-process
      // parton (jet-like).  ShowerOnlyShared: share only at the soft /
      // shower / MPI level.
      if (pc == PairClass::SharedParton)
        {
        const std::string depth =
          sharesHardProcessAncestor(*parts[i].tag, *parts[j].tag)
            ? "HardProcessShared" : "ShowerOnlyShared";
        H("dphi_pair_SharedParton_" + depth).fill(dphi);
        H("deta_pair_SharedParton_" + depth).fill(deta);
        H("mass_pair_SharedParton_" + depth).fill(m);
        }

      // ---- Common-ancestor depth (single 1D histogram) -----------------
      // For each pair, the EARLIEST stage at which they share an ancestor.
      // Stage::Unknown = no shared ancestor (Unrelated).
      Stage commonStage = Stage::Unknown;
      if (sharesHardProcessAncestor(*parts[i].tag, *parts[j].tag))
        commonStage = Stage::HardProcess;
      else if (sharesPartonAncestor(*parts[i].tag, *parts[j].tag))
        commonStage = Stage::PartonsPreHadronization;
      else if (sharesDecayParent(*parts[i].tag, *parts[j].tag))
        commonStage = Stage::PrimaryHadrons;
      Hs("common_ancestor_depth").fill(static_cast<double>(stageOrder(commonStage)));

      // ---- MPI vertex relationship -------------------------------------
      // SameMPI: both descend from one secondary scatter.  CrossMPI: each
      // from a different MPI scatter.  OneSideMPI: only one carries an MPI
      // ancestor.  NoMPI: both came from the primary hard scatter or beam
      // remnants only.
      const std::string mpiTag =
        pairMPITag(*parts[i].tag, *parts[j].tag);
      Hs("deta_pair_MPI_" + mpiTag).fill(deta);
      Hs("dphi_pair_MPI_" + mpiTag).fill(dphi);
      Hs("mass_pair_MPI_" + mpiTag).fill(m);

      // Refine SharedParton further with the MPI relationship — this
      // directly answers "is the partonic correlation from one MPI or
      // from accidental cross-MPI?"
      if (pc == PairClass::SharedParton)
        {
        H("deta_pair_SharedParton_" + mpiTag).fill(deta);
        H("dphi_pair_SharedParton_" + mpiTag).fill(dphi);
        H("mass_pair_SharedParton_" + mpiTag).fill(m);
        }

      // ---- Shower-lineage relationship --------------------------------
      // BothISR / BothFSR / MixedShower / NoShower at the pair level.
      const std::string shTag =
        pairShowerTag(*parts[i].tag, *parts[j].tag);
      Hs("deta_pair_Shower_" + shTag).fill(deta);
      Hs("dphi_pair_Shower_" + shTag).fill(dphi);
      Hs("mass_pair_Shower_" + shTag).fill(m);

      // ---- Heavy-flavour decay-chain relationship --------------------
      // BothBottom, OneBottom, BothCharm, OneCharm, NoHeavyFlavour.
      const std::string hfTag =
        pairHeavyFlavourTag(*parts[i].tag, *parts[j].tag);
      Hs("deta_pair_HF_" + hfTag).fill(deta);
      Hs("dphi_pair_HF_" + hfTag).fill(dphi);
      Hs("mass_pair_HF_" + hfTag).fill(m);

      // ---- Decay-chain depth ------------------------------------------
      // Per-pair: the deeper of the two hadrons' decay-chain depths.
      // 0 = both primary; large = at least one in a long cascade.
      const int dDepth = std::max(parts[i].tag->decayChainDepth,
                                  parts[j].tag->decayChainDepth);
      Hs("pair_decay_depth_max").fill(static_cast<double>(dDepth));
      Hs("pair_decay_depth_max_" + pairClassName(pc))
        .fill(static_cast<double>(dDepth));

      // ---- Multiplicity-binned pair observables -----------------------
      // Split every pair-class fill into the event's multiplicity bin.
      // This lets the user see how the ancestry breakdown evolves with
      // event activity.
      Hs("deta_pair_All_"             + mb).fill(deta);
      Hs("dphi_pair_All_"             + mb).fill(dphi);
      Hs("mass_pair_All_"             + mb).fill(m);
      Hs("deta_pair_" + pairClassName(pc) + "_" + mb).fill(deta);
      Hs("dphi_pair_" + pairClassName(pc) + "_" + mb).fill(dphi);
      Hs("mass_pair_" + pairClassName(pc) + "_" + mb).fill(m);

      // ---- Flavor-mixing classification -------------------------------
      // The joint hard-parton flavour pair, collapsed to 5 categories
      // (see pairFlavourMixTag).  Answers: how much of the pair
      // correlation lives in same-flavour vertices vs cross-flavour vs
      // gluon-involved vs partonless.
      const std::string fv = pairFlavourMixTag(
          parts[i].tag->hardPartonPdg, parts[j].tag->hardPartonPdg);
      Hs("deta_pair_FlavMix_" + fv).fill(deta);
      Hs("dphi_pair_FlavMix_" + fv).fill(dphi);
      Hs("mass_pair_FlavMix_" + fv).fill(m);

      // Refine SharedParton specifically — when the pair correlation
      // is genuinely partonic, what flavour content drives it?
      if (pc == PairClass::SharedParton)
        {
        H("deta_pair_SharedParton_Flav_" + fv).fill(deta);
        H("dphi_pair_SharedParton_Flav_" + fv).fill(dphi);
        H("mass_pair_SharedParton_Flav_" + fv).fill(m);
        }

      // ---- Event-shape (transverse spherocity) bin --------------------
      // Sets up the same All / per-pair-class breakdown we use for
      // multiplicity, but binned by event topology.  JetLike events
      // (low S0) should show enhanced SharedParton correlation;
      // Isotropic events (high S0) should be dominated by Unrelated.
      Hs("deta_pair_All_"             + sb).fill(deta);
      Hs("dphi_pair_All_"             + sb).fill(dphi);
      Hs("mass_pair_All_"             + sb).fill(m);
      Hs("deta_pair_" + pairClassName(pc) + "_" + sb).fill(deta);
      Hs("dphi_pair_" + pairClassName(pc) + "_" + sb).fill(dphi);
      Hs("mass_pair_" + pairClassName(pc) + "_" + sb).fill(m);

      _pairs++;
      _count[static_cast<int>(pc)]++;
      }
    }
}

std::string PairProvenanceObservables::report() const
{
  std::ostringstream os;
  os << std::fixed << std::setprecision(2);
  os << "PairProvenanceObservables — " << _events << " event(s), "
     << _pairs << " same-event pair(s)";
  if (_speciesPdg != 0) os << "  (species |pdg| = " << _speciesPdg << ")";
  os << "\n  pair correlation by ancestry:\n";

  const double tot = _pairs > 0 ? static_cast<double>(_pairs) : 1.0;
  const PairClass classes[] = { PairClass::SameResonance,
                                PairClass::SameWeakParent,
                                PairClass::SharedParton,
                                PairClass::Unrelated };
  for (PairClass c : classes)
    {
    const long n = _count[static_cast<int>(c)];
    os << "    " << std::setw(16) << std::left << pairClassName(c)
       << std::setw(14) << std::right << n
       << "   " << std::setw(6) << (100.0 * n / tot) << " %\n";
    }
  os << "  (SameResonance + SameWeakParent + SharedParton = the part of "
        "the\n   same-event correlation that is ancestry-driven.)\n";
  return os.str();
}

} // namespace CAP
