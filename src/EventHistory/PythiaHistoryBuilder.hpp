/* **********************************************************************
 * CAP::PythiaHistoryBuilder
 *
 * Part of the parton-tracking feature (Phase 1).
 *
 * Converts a Pythia 8 event record into a generator-agnostic
 * EventHistory DAG.  Pythia provides the richest provenance available:
 * the complete mother/daughter graph plus fine-grained status codes
 * that distinguish hard process, ISR, FSR, beam remnants,
 * pre-hadronization partons, primary hadrons and decay products.
 *
 * Only compiled when CAP_ENABLE_PYTHIA=ON (see CMakeLists.txt).  The
 * Pythia type is forward-declared here so this header stays light;
 * the concrete include lives in the .cpp.
 * ********************************************************************/
#ifndef CAP__PythiaHistoryBuilder
#define CAP__PythiaHistoryBuilder

#include "EventHistory.hpp"
#include "StageTaxonomy.hpp"

namespace Pythia8 { class Event; }

namespace CAP
{

class PythiaHistoryBuilder
{
public:

  // Fill `out` from `pythiaEvent`.  `out` is cleared first.  The node
  // index of each particle equals its Pythia event-record index, so
  // the mother/daughter links copy across directly.
  void build(const Pythia8::Event & pythiaEvent, EventHistory & out) const;

  // Map the absolute value of a Pythia internal status code to a
  // CAP::Stage.  Pythia's status ranges:
  //   11-19 beam      21-29 hard process   31-39 MPI
  //   41-49 ISR       51-59 FSR            61-69 beam remnants
  //   71-79 partons pre-hadronization      81-89 primary hadrons
  //   91-99 decay products
  static Stage stageFromPythiaStatus(int statusAbs);
};

} // namespace CAP

#endif // CAP__PythiaHistoryBuilder
