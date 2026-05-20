/* **********************************************************************
 * CAP::Stage  —  event-evolution stage taxonomy
 *
 * Part of the parton-tracking feature (Phase 1).
 *
 * A single, generator-agnostic enumeration of the stages an event
 * passes through, from the hard scattering (earliest / most partonic)
 * through the parton shower, hadronization and decays, down to the
 * final state a detector would record.
 *
 * The numeric values increase with "time": a SMALLER value means an
 * EARLIER, deeper-in-the-history stage.  This lets analysis code ask
 * "how deep can I go?" with a plain integer comparison.
 *
 * Each generator's own status-code zoo is mapped onto this common
 * taxonomy by the per-generator history builders
 * (PythiaHistoryBuilder, HepMC3HistoryBuilder), so everything
 * downstream is generator-agnostic.
 *
 * Header-only, no dependencies.
 * ********************************************************************/
#ifndef CAP__StageTaxonomy
#define CAP__StageTaxonomy

#include <string>

namespace CAP
{

enum class Stage : int
{
  Unknown                 =   0,
  Beam                    =  10,   // incoming beam particles
  HardProcess             =  20,   // partons of the primary hard scatter
  MPI                     =  30,   // multi-parton-interaction partons
  ISR                     =  40,   // initial-state radiation
  FSR                     =  50,   // final-state radiation (parton shower)
  BeamRemnants            =  60,   // beam-remnant partons
  PartonsPreHadronization =  70,   // partons entering strings / clusters
  PrimaryHadrons          =  80,   // hadrons as first formed, before decays
  DecayProducts           =  90,   // hadrons / leptons produced in decays
  FinalState              = 100    // status==1 — what a detector records
};

// Human-readable name of a stage.
inline std::string stageName(Stage s)
{
  switch (s)
    {
    case Stage::Beam:                    return "Beam";
    case Stage::HardProcess:             return "HardProcess";
    case Stage::MPI:                     return "MPI";
    case Stage::ISR:                     return "ISR";
    case Stage::FSR:                     return "FSR";
    case Stage::BeamRemnants:            return "BeamRemnants";
    case Stage::PartonsPreHadronization: return "PartonsPreHadronization";
    case Stage::PrimaryHadrons:          return "PrimaryHadrons";
    case Stage::DecayProducts:           return "DecayProducts";
    case Stage::FinalState:              return "FinalState";
    case Stage::Unknown:                 return "Unknown";
    }
  return "Unknown";
}

// True for stages made of quarks / gluons (i.e. pre-hadronization).
inline bool isPartonic(Stage s)
{
  return s == Stage::HardProcess  || s == Stage::MPI ||
         s == Stage::ISR          || s == Stage::FSR ||
         s == Stage::BeamRemnants || s == Stage::PartonsPreHadronization;
}

// True for stages made of hadrons.
inline bool isHadronic(Stage s)
{
  return s == Stage::PrimaryHadrons || s == Stage::DecayProducts ||
         s == Stage::FinalState;
}

// Monotonic ordering value: earlier (deeper) stages compare smaller.
inline int stageOrder(Stage s) { return static_cast<int>(s); }

} // namespace CAP

#endif // CAP__StageTaxonomy
