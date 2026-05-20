/* **********************************************************************
 * CAP::StageStatusCodes  —  Stage <-> generator-status-code mapping
 *
 * Part of the parton-tracking feature (Phase 2).
 *
 * Bridges the clean CAP::Stage taxonomy to the raw status-code zoo of
 * the event generators.  Two uses:
 *   - stageFromName()           parse an `analysis_stage` config string.
 *   - pythiaInternalStatusFor() the Pythia 8 internal-status magnitudes
 *                               of a stage, for a generator-level filter
 *                               that wants fine stage granularity.
 *
 * NOTE on Pythia status: the existing generator filter compares against
 * Particle::statusHepMC(), which only distinguishes final / decayed /
 * beam.  To separate ISR / FSR / pre-hadronization partons a filter
 * must instead use Particle::statusAbs() — the documented, stable
 * internal ranges returned here.
 *
 * Header-only, no dependencies beyond the C++ standard library.
 * ********************************************************************/
#ifndef CAP__StageStatusCodes
#define CAP__StageStatusCodes

#include <set>
#include <string>
#include <cctype>

#include "StageTaxonomy.hpp"

namespace CAP
{

// Parse a stage name (case-insensitive, whitespace-tolerant).  Accepts
// the canonical stageName() spellings plus a few friendly aliases.
// An unrecognised string yields Stage::Unknown.
inline Stage stageFromName(const std::string & raw)
{
  std::string s;
  for (char c : raw)
    if (!std::isspace(static_cast<unsigned char>(c)))
      s.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(c))));

  if (s.empty())                                  return Stage::Unknown;
  if (s == "final"   || s == "finalstate")        return Stage::FinalState;
  if (s == "primary" || s == "primaryhadrons" ||
      s == "predecay")                            return Stage::PrimaryHadrons;
  if (s == "decay"   || s == "decayproducts")     return Stage::DecayProducts;
  if (s == "partons" || s == "prehadronization" ||
      s == "partonsprehadronization")             return Stage::PartonsPreHadronization;
  if (s == "fsr"     || s == "shower")            return Stage::FSR;
  if (s == "isr")                                 return Stage::ISR;
  if (s == "mpi")                                 return Stage::MPI;
  if (s == "hard"    || s == "hardprocess")       return Stage::HardProcess;
  if (s == "beamremnants")                        return Stage::BeamRemnants;
  if (s == "beam")                                return Stage::Beam;
  return Stage::Unknown;
}

// The Pythia 8 internal-status magnitudes (Particle::statusAbs()) that
// correspond to a stage.  FinalState returns an empty set on purpose —
// "final" is the sign of the status, not a magnitude, so a filter must
// test Particle::isFinal() for it.
inline std::set<int> pythiaInternalStatusFor(Stage s)
{
  std::set<int> out;
  int lo = 0, hi = -1;
  switch (s)
    {
    case Stage::Beam:                    lo = 11; hi = 19; break;
    case Stage::HardProcess:             lo = 21; hi = 29; break;
    case Stage::MPI:                     lo = 31; hi = 39; break;
    case Stage::ISR:                     lo = 41; hi = 49; break;
    case Stage::FSR:                     lo = 51; hi = 59; break;
    case Stage::BeamRemnants:            lo = 61; hi = 69; break;
    case Stage::PartonsPreHadronization: lo = 71; hi = 79; break;
    case Stage::PrimaryHadrons:          lo = 81; hi = 89; break;
    case Stage::DecayProducts:           lo = 91; hi = 99; break;
    case Stage::FinalState:              return out;   // test isFinal()
    case Stage::Unknown:                 return out;
    }
  for (int v = lo; v <= hi; ++v) out.insert(v);
  return out;
}

} // namespace CAP

#endif // CAP__StageStatusCodes
