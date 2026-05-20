/* **********************************************************************
 * CAP::HepMC3HistoryBuilder
 *
 * Part of the parton-tracking feature (Phase 1).
 *
 * Converts a HepMC3 GenEvent into a generator-agnostic EventHistory
 * DAG.  HepMC3 keeps the full particle/vertex graph, so ancestry is
 * always available — but the HepMC status convention is coarse (only
 * 1 = final, 2 = decayed, 3 = documentation, 4 = beam).  Stage
 * classification therefore leans on the graph itself: a hadron is a
 * "primary hadron" if its production vertex is fed by partons, and a
 * "decay product" if it is fed by hadrons.
 *
 * Only compiled when CAP_ENABLE_HEPMC3=ON (see CMakeLists.txt).  The
 * HepMC3 type is forward-declared so this header stays light.
 * ********************************************************************/
#ifndef CAP__HepMC3HistoryBuilder
#define CAP__HepMC3HistoryBuilder

#include "EventHistory.hpp"

namespace HepMC3 { class GenEvent; }

namespace CAP
{

class HepMC3HistoryBuilder
{
public:

  // Fill `out` from a HepMC3 event.  `out` is cleared first.
  //
  // NOTE: takes a non-const GenEvent& to match HepMC3's own iteration
  // API (GenEvent::particles()), exactly as CAP::HepMC3EventReader does.
  void build(HepMC3::GenEvent & ev, EventHistory & out) const;
};

} // namespace CAP

#endif // CAP__HepMC3HistoryBuilder
