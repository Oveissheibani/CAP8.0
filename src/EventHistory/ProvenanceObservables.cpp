/* **********************************************************************
 * CAP::ProvenanceObservables — implementation.  See header for design.
 * ********************************************************************/
#include "ProvenanceObservables.hpp"

#include <cstdlib>   // std::abs
#include <sstream>
#include <iomanip>

namespace CAP
{

// ======================================================================
//  Hist1D
// ======================================================================
Hist1D::Hist1D(const std::string & n, const std::string & t,
               int nb, double a, double b)
: name(n), title(t), nbins(nb), lo(a), hi(b), counts(nb > 0 ? nb : 0, 0.0)
{ }

void Hist1D::fill(double x, double w)
{
  entries += 1.0;
  if (nbins <= 0 || hi <= lo) return;
  if (x <  lo) { underflow += w; return; }
  if (x >= hi) { overflow  += w; return; }
  int bin = static_cast<int>((x - lo) / (hi - lo) * nbins);
  if (bin < 0)        bin = 0;
  if (bin >= nbins)   bin = nbins - 1;
  counts[static_cast<size_t>(bin)] += w;
}

double Hist1D::binCenter(int i) const
{
  if (nbins <= 0) return 0.0;
  return lo + (static_cast<double>(i) + 0.5) * (hi - lo) / nbins;
}

double Hist1D::integral() const
{
  double s = 0.0;
  for (double c : counts) s += c;
  return s;
}

// ======================================================================
//  Provenance-class naming + classification
// ======================================================================
std::string originClassName(OriginClass c)
{
  switch (c)
    {
    case OriginClass::Primary:       return "Primary";
    case OriginClass::FromResonance: return "FromResonance";
    case OriginClass::FromWeakDecay: return "FromWeakDecay";
    case OriginClass::Unknown:       return "Unknown";
    }
  return "Unknown";
}

std::string partonClassName(PartonClass c)
{
  switch (c)
    {
    case PartonClass::LightQuark:  return "LightQuark";
    case PartonClass::Strange:     return "Strange";
    case PartonClass::Charm:       return "Charm";
    case PartonClass::Bottom:      return "Bottom";
    case PartonClass::Gluon:       return "Gluon";
    case PartonClass::NonPartonic: return "NonPartonic";
    }
  return "NonPartonic";
}

OriginClass classifyOrigin(const ProvenanceTag & t)
{
  if (t.isPrimaryHadron) return OriginClass::Primary;
  if (t.isFromDecay)
    return t.isFromResonance ? OriginClass::FromResonance
                             : OriginClass::FromWeakDecay;
  return OriginClass::Unknown;
}

PartonClass classifyParton(const ProvenanceTag & t)
{
  switch (std::abs(t.leadPartonPdg))
    {
    case 1: case 2: return PartonClass::LightQuark;
    case 3:         return PartonClass::Strange;
    case 4:         return PartonClass::Charm;
    case 5:         return PartonClass::Bottom;
    case 21:        return PartonClass::Gluon;
    default:        return PartonClass::NonPartonic;
    }
}

// ======================================================================
//  ProvenanceObservables
// ======================================================================
namespace
{
// Histogram axis defaults — soft-physics ranges.
const int    PT_NBINS  = 100;   const double PT_LO  = 0.0;  const double PT_HI  = 10.0;
const int    ETA_NBINS = 120;   const double ETA_LO = -6.0; const double ETA_HI =  6.0;
const int    MUL_NBINS = 200;   const double MUL_LO = 0.0;  const double MUL_HI = 600.0;
}

ProvenanceObservables::ProvenanceObservables(int speciesPdg)
: _speciesPdg(speciesPdg < 0 ? -speciesPdg : speciesPdg)
{ }

Hist1D & ProvenanceObservables::H(const std::string & name)
{
  std::map<std::string,Hist1D>::iterator it = _hist.find(name);
  if (it != _hist.end()) return it->second;

  // Lazily create with the right axis based on the name prefix.
  int nb; double lo, hi;
  if (name.rfind("pt_", 0) == 0)
    { nb = PT_NBINS;  lo = PT_LO;  hi = PT_HI;  }
  else if (name.rfind("eta_", 0) == 0)
    { nb = ETA_NBINS; lo = ETA_LO; hi = ETA_HI; }
  else if (name.rfind("n_parton_ancestors", 0) == 0)
    { nb = 11;  lo = -0.5; hi = 10.5; }   // integer-valued, bins per count
  else  // mult_*
    { nb = MUL_NBINS; lo = MUL_LO; hi = MUL_HI; }

  _hist[name] = Hist1D(name, name, nb, lo, hi);
  return _hist[name];
}

void ProvenanceObservables::accumulate(const EventHistory &               history,
                                       const std::vector<ProvenanceTag> & tags)
{
  _events++;

  // Per-event multiplicity counters, by origin class.
  double mAll = 0.0, mPrim = 0.0, mRes = 0.0, mWeak = 0.0;

  for (const ProvenanceTag & t : tags)
    {
    // Species filter: 0 == accept every final hadron.
    if (_speciesPdg != 0 && std::abs(t.finalPdg) != _speciesPdg) continue;

    // Kinematics come from the history node the tag points at.
    if (t.finalIndex < 0 || t.finalIndex >= history.size()) continue;
    const ParticleNode & node = history.node(t.finalIndex);
    const double pt  = node.pt();
    const double eta = node.eta();

    const OriginClass oc = classifyOrigin(t);
    const PartonClass pc = classifyParton(t);

    mAll++;
    H("pt_all").fill(pt);
    H("eta_all").fill(eta);

    H("pt_origin_"  + originClassName(oc)).fill(pt);
    H("eta_origin_" + originClassName(oc)).fill(eta);
    H("pt_parton_"  + partonClassName(pc)).fill(pt);

    // ancestry-depth diagnostic: how many pre-hadronization partons does
    // this final hadron actually descend from?  Tells the user whether
    // hadrons typically trace to a single parton (string endpoint) or
    // share several (gluon kinks, MPI merging).
    H("n_parton_ancestors")
      .fill(static_cast<double>(t.partonAncestorIndices.size()));

    if      (oc == OriginClass::Primary)       mPrim++;
    else if (oc == OriginClass::FromResonance) mRes++;
    else if (oc == OriginClass::FromWeakDecay) mWeak++;

    _totalStudied += 1.0;
    }

  H("mult_all").fill(mAll);
  H("mult_origin_Primary").fill(mPrim);
  H("mult_origin_FromResonance").fill(mRes);
  H("mult_origin_FromWeakDecay").fill(mWeak);
}

std::string ProvenanceObservables::report() const
{
  std::ostringstream os;
  os << std::fixed << std::setprecision(2);
  os << "ProvenanceObservables — " << _events << " event(s), "
     << static_cast<long>(_totalStudied) << " studied hadron(s)";
  if (_speciesPdg != 0) os << "  (species |pdg| = " << _speciesPdg << ")";
  os << "\n";

  const double tot = _totalStudied > 0.0 ? _totalStudied : 1.0;

  os << "  by origin:\n";
  const OriginClass ocs[] = { OriginClass::Primary, OriginClass::FromResonance,
                              OriginClass::FromWeakDecay, OriginClass::Unknown };
  for (OriginClass c : ocs)
    {
    std::map<std::string,Hist1D>::const_iterator it =
      _hist.find("pt_origin_" + originClassName(c));
    const double n = (it != _hist.end()) ? it->second.integral() : 0.0;
    os << "    " << std::setw(16) << std::left << originClassName(c)
       << std::setw(10) << std::right << static_cast<long>(n)
       << "   " << std::setw(6) << (100.0 * n / tot) << " %\n";
    }

  os << "  by parton flavour:\n";
  const PartonClass pcs[] = { PartonClass::LightQuark, PartonClass::Strange,
                              PartonClass::Charm,      PartonClass::Bottom,
                              PartonClass::Gluon,      PartonClass::NonPartonic };
  for (PartonClass c : pcs)
    {
    std::map<std::string,Hist1D>::const_iterator it =
      _hist.find("pt_parton_" + partonClassName(c));
    const double n = (it != _hist.end()) ? it->second.integral() : 0.0;
    os << "    " << std::setw(16) << std::left << partonClassName(c)
       << std::setw(10) << std::right << static_cast<long>(n)
       << "   " << std::setw(6) << (100.0 * n / tot) << " %\n";
    }
  return os.str();
}

} // namespace CAP
