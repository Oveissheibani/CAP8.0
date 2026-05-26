/* **********************************************************************
 * CAP::PairProvenanceObservables — implementation.  See header for design.
 * ********************************************************************/
#include "PairProvenanceObservables.hpp"

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
PairProvenanceObservables::PairProvenanceObservables(int speciesPdg)
: _speciesPdg(speciesPdg < 0 ? -speciesPdg : speciesPdg)
{ }

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
  // The four-momentum is kept so we can form the pair invariant mass.
  struct P
  {
    const ProvenanceTag * tag;
    double eta, phi;
    double px, py, pz, e;
  };
  std::vector<P> parts;
  parts.reserve(tags.size());
  for (const ProvenanceTag & t : tags)
    {
    if (_speciesPdg != 0 && std::abs(t.finalPdg) != _speciesPdg) continue;
    if (t.finalIndex < 0 || t.finalIndex >= history.size())      continue;
    const ParticleNode & node = history.node(t.finalIndex);
    P p;
    p.tag = &t;
    p.eta = node.eta();  p.phi = node.phi();
    p.px  = node.px;     p.py  = node.py;
    p.pz  = node.pz;     p.e   = node.e;
    parts.push_back(p);
    }

  // Every unordered pair.
  const size_t n = parts.size();
  for (size_t i = 0; i < n; ++i)
    {
    for (size_t j = i + 1; j < n; ++j)
      {
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

      H("deta_pair_All").fill(deta);
      H("dphi_pair_All").fill(dphi);
      H("mass_pair_All").fill(m);
      H("deta_pair_" + pairClassName(pc)).fill(deta);
      H("dphi_pair_" + pairClassName(pc)).fill(dphi);
      H("mass_pair_" + pairClassName(pc)).fill(m);

      // ---- Same-Sign / Opposite-Sign breakdown --------------------------
      // Applies to every class; this is the direct balance-function split.
      const std::string sign = pairChargeTag(parts[i].tag->finalPdg,
                                             parts[j].tag->finalPdg);
      H("deta_pair_All_"             + sign).fill(deta);
      H("dphi_pair_All_"             + sign).fill(dphi);
      H("mass_pair_All_"             + sign).fill(m);
      H("deta_pair_" + pairClassName(pc) + "_" + sign).fill(deta);
      H("dphi_pair_" + pairClassName(pc) + "_" + sign).fill(dphi);
      H("mass_pair_" + pairClassName(pc) + "_" + sign).fill(m);

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
      H("common_ancestor_depth").fill(static_cast<double>(stageOrder(commonStage)));

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
