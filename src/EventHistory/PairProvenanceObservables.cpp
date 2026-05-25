/* **********************************************************************
 * CAP::PairProvenanceObservables — implementation.  See header for design.
 * ********************************************************************/
#include "PairProvenanceObservables.hpp"

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
  struct P { const ProvenanceTag * tag; double eta; double phi; };
  std::vector<P> parts;
  parts.reserve(tags.size());
  for (const ProvenanceTag & t : tags)
    {
    if (_speciesPdg != 0 && std::abs(t.finalPdg) != _speciesPdg) continue;
    if (t.finalIndex < 0 || t.finalIndex >= history.size())      continue;
    const ParticleNode & node = history.node(t.finalIndex);
    P p; p.tag = &t; p.eta = node.eta(); p.phi = node.phi();
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

      H("deta_pair_All").fill(deta);
      H("dphi_pair_All").fill(dphi);
      H("deta_pair_" + pairClassName(pc)).fill(deta);
      H("dphi_pair_" + pairClassName(pc)).fill(dphi);

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
