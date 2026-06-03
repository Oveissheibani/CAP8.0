/* **********************************************************************
 * CAP::ProvenanceObservables — implementation.  See header for design.
 * ********************************************************************/
#include "ProvenanceObservables.hpp"

#include <algorithm>
#include <array>
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

// Map a bare parton PDG id to a flavour class.  Shared by the string-endpoint
// classifier (classifyParton) and the initiating-parton classifier so both
// agree on the LightQuark/Strange/.../Gluon mapping.
PartonClass partonClassOfPdg(int pdg)
{
  switch (std::abs(pdg))
    {
    case 1: case 2: return PartonClass::LightQuark;
    case 3:         return PartonClass::Strange;
    case 4:         return PartonClass::Charm;
    case 5:         return PartonClass::Bottom;
    case 21:        return PartonClass::Gluon;
    default:        return PartonClass::NonPartonic;
    }
}

// String-ENDPOINT flavour: the Lund-string endpoint parton this hadron
// fragmented from.  By construction these are quarks/diquarks — a GLUON is
// never a string endpoint, so PartonClass::Gluon is (correctly) unreachable
// here.  This answers "what flavour quark did the pion's string end on?".
PartonClass classifyParton(const ProvenanceTag & t)
{
  return partonClassOfPdg(t.leadPartonPdg);
}

// INITIATING-parton flavour: the topmost parton that started this hadron's
// lineage (the quark- vs GLUON-jet origin).  Unlike the string endpoint, this
// CAN be a gluon and at the LHC is gluon-dominated.  Uses the generator-
// agnostic initiatingPartonPdg computed by the tagger (works for both the
// Pythia status-code path and the Herwig/HepMC graph path).
PartonClass classifyInitiatingParton(const ProvenanceTag & t)
{
  return partonClassOfPdg(t.initiatingPartonPdg);
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

// Common ctor body: canonicalise the species list (positive PDGs, drop
// duplicates, collapse to {0} if 0 is present), then set the legacy
// _speciesPdg for code that still queries it.
static std::vector<int> _canonSpecies(const std::vector<int> & raw)
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

ProvenanceObservables::ProvenanceObservables(int speciesPdg,
                                              double ptMin,  double ptMax,
                                              double etaMin, double etaMax)
: _species(_canonSpecies({speciesPdg})),
  _speciesPdg(_species.front()),
  _ptMin (ptMin), _ptMax (ptMax),
  _etaMin(etaMin), _etaMax(etaMax)
{ }

ProvenanceObservables::ProvenanceObservables(const std::vector<int> & list,
                                              double ptMin,  double ptMax,
                                              double etaMin, double etaMax)
: _species(_canonSpecies(list)),
  _speciesPdg(_species.front()),
  _ptMin (ptMin), _ptMax (ptMax),
  _etaMin(etaMin), _etaMax(etaMax)
{ }

std::string ProvenanceObservables::speciesSuffix(int s) const
{
  // Single-species mode keeps the legacy bare histogram names so the
  // existing plotter and report don't have to know about the new axis.
  if (_species.size() <= 1) return "";
  return "_S" + std::to_string(s);
}

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
  else if (name.rfind("decay_chain_depth", 0) == 0)
    { nb = 21;  lo = -0.5; hi = 20.5; }   // integer-valued decay-chain depth
  else  // mult_*
    { nb = MUL_NBINS; lo = MUL_LO; hi = MUL_HI; }

  _hist[name] = Hist1D(name, name, nb, lo, hi);
  return _hist[name];
}

void ProvenanceObservables::accumulate(const EventHistory &               history,
                                       const std::vector<ProvenanceTag> & tags)
{
  _events++;

  // Per-event multiplicity counters, by origin class — keyed by species
  // suffix so multi-species mode gets per-species multiplicity spectra.
  std::map<std::string, std::array<double, 4>> mult;   // [all, prim, res, weak]

  for (const ProvenanceTag & t : tags)
    {
    // Species filter — multi-species capable.  In single-species mode
    // (`_species == {0}` or `_species == {pdg}`) this matches the
    // historical behaviour; in multi-species mode the hadron is routed
    // into the FIRST species in the list it matches and tagged with
    // that species' suffix.  Hadrons matching no species are skipped.
    int matched_species = -1;
    for (int s : _species)
      {
      if (s == 0 || std::abs(t.finalPdg) == s) { matched_species = s; break; }
      }
    if (matched_species < 0) continue;
    const std::string sfx = speciesSuffix(matched_species);

    // Kinematics come from the history node the tag points at.
    if (t.finalIndex < 0 || t.finalIndex >= history.size()) continue;
    const ParticleNode & node = history.node(t.finalIndex);
    const double pt  = node.pt();
    const double eta = node.eta();

    // Apply the kinematic acceptance window — audit fix #9.  A hadron
    // outside [ptMin, ptMax] x [etaMin, etaMax] is dropped before any
    // fill, so the species fraction *within* the window is what every
    // downstream plot shows.
    if (pt  < _ptMin  || pt  > _ptMax)  continue;
    if (eta < _etaMin || eta > _etaMax) continue;

    const OriginClass oc = classifyOrigin(t);
    const PartonClass pc = classifyParton(t);

    H("pt_all"  + sfx).fill(pt);
    H("eta_all" + sfx).fill(eta);

    H("pt_origin_"  + originClassName(oc) + sfx).fill(pt);
    H("eta_origin_" + originClassName(oc) + sfx).fill(eta);
    H("pt_parton_"  + partonClassName(pc) + sfx).fill(pt);

    // ---- INITIATING-parton flavour (gluon-capable) ------------------
    // The string-endpoint flavour above is never a gluon (Lund endpoints
    // are quarks).  To answer "did this hadron originate from a quark or
    // a GLUON?" we classify the generator-agnostic INITIATING parton —
    // the topmost parton in the lineage (tagger field initiatingPartonPdg),
    // which works for both the Pythia and the Herwig/HepMC paths.
    {
    const PartonClass ic = classifyInitiatingParton(t);
    H("pt_initparton_"  + partonClassName(ic) + sfx).fill(pt);
    H("eta_initparton_" + partonClassName(ic) + sfx).fill(eta);
    }

    // ancestry-depth diagnostic: how many pre-hadronization partons does
    // this final hadron actually descend from?  Tells the user whether
    // hadrons typically trace to a single parton (string endpoint) or
    // share several (gluon kinks, MPI merging).
    H("n_parton_ancestors" + sfx)
      .fill(static_cast<double>(t.partonAncestorIndices.size()));

    // Decay-chain depth: 0 = primary, 1 = direct decay, N = N-step cascade.
    H("decay_chain_depth" + sfx).fill(static_cast<double>(t.decayChainDepth));

    // ---- Heavy-flavour origin split (single-particle) ---------------
    // The bottom chain dominates if present; otherwise charm; else
    // "NoHeavyFlavour".  Mutually exclusive — easy to read as a stack.
    {
    const char * hf = t.fromBottomChain ? "FromBottomChain"
                     : t.fromCharmChain ? "FromCharmChain"
                     : "NoHeavyFlavour";
    H(std::string("pt_HF_")  + hf + sfx).fill(pt);
    H(std::string("eta_HF_") + hf + sfx).fill(eta);
    }

    // ---- Shower-origin split (single-particle) ----------------------
    // "FSR" includes hadrons whose ancestry touches both ISR and FSR
    // (the FSR is the last shower step before fragmentation).
    {
    const char * sh = t.fromFSR ? "FSR"
                    : t.fromISR ? "ISR"
                    : "NoShower";
    H(std::string("pt_Shower_")  + sh + sfx).fill(pt);
    H(std::string("eta_Shower_") + sh + sfx).fill(eta);
    }

    // ---- MPI presence split (single-particle) -----------------------
    // FromMPI: hadron descends from a secondary MPI scatter.  NoMPI:
    // descends only from primary hard scatter / beam remnants.
    {
    const char * mp = (t.mpiIndex >= 0) ? "FromMPI" : "NoMPI";
    H(std::string("pt_MPI_")  + mp + sfx).fill(pt);
    H(std::string("eta_MPI_") + mp + sfx).fill(eta);
    }

    auto & m = mult[sfx];
    m[0]++;                                                   // all
    if      (oc == OriginClass::Primary)       m[1]++;
    else if (oc == OriginClass::FromResonance) m[2]++;
    else if (oc == OriginClass::FromWeakDecay) m[3]++;

    _totalStudied += 1.0;
    }

  // Per-species multiplicity histograms.  Single-species mode uses
  // bare names (sfx == "") and matches legacy output exactly.
  for (const auto & kv : mult)
    {
    const std::string & suf = kv.first;
    const auto & m = kv.second;
    H("mult_all"                + suf).fill(m[0]);
    H("mult_origin_Primary"     + suf).fill(m[1]);
    H("mult_origin_FromResonance" + suf).fill(m[2]);
    H("mult_origin_FromWeakDecay" + suf).fill(m[3]);
    }
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

  const PartonClass pcs[] = { PartonClass::LightQuark, PartonClass::Strange,
                              PartonClass::Charm,      PartonClass::Bottom,
                              PartonClass::Gluon,      PartonClass::NonPartonic };

  os << "  by parton flavour (string endpoint):\n";
  for (PartonClass c : pcs)
    {
    std::map<std::string,Hist1D>::const_iterator it =
      _hist.find("pt_parton_" + partonClassName(c));
    const double n = (it != _hist.end()) ? it->second.integral() : 0.0;
    os << "    " << std::setw(16) << std::left << partonClassName(c)
       << std::setw(10) << std::right << static_cast<long>(n)
       << "   " << std::setw(6) << (100.0 * n / tot) << " %\n";
    }

  // Initiating (hard-scatter) parton flavour — the gluon-capable view.
  os << "  by initiating parton flavour:\n";
  for (PartonClass c : pcs)
    {
    std::map<std::string,Hist1D>::const_iterator it =
      _hist.find("pt_initparton_" + partonClassName(c));
    const double n = (it != _hist.end()) ? it->second.integral() : 0.0;
    os << "    " << std::setw(16) << std::left << partonClassName(c)
       << std::setw(10) << std::right << static_cast<long>(n)
       << "   " << std::setw(6) << (100.0 * n / tot) << " %\n";
    }
  return os.str();
}

} // namespace CAP
