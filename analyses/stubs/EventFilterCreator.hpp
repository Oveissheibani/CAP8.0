/* **********************************************************************
 *  Stub implementation of CAP::EventFilterCreator.
 *
 *  Same status as ParticleTypeTask: this stub gets the class registered
 *  with ROOT so the .ini load can complete. The full job (parsing the
 *  EventFilterCreator:EventFilter:Filter<k>:* blocks and building real
 *  EventFilter objects) is reserved for a follow-on patch.
 * ******************************************************************** */
#ifndef CAP_USER__EventFilterCreator
#define CAP_USER__EventFilterCreator

#include "EventProcessor.hpp"

namespace CAP
{

class EventFilterCreator : public EventProcessor
{
public:
  EventFilterCreator();
  EventFilterCreator(const EventFilterCreator & source);
  EventFilterCreator & operator=(const EventFilterCreator & rhs);
  virtual ~EventFilterCreator() {}

  // No per-tick work; filters are instantiated during configure().
  virtual void execute();

  ClassDef(EventFilterCreator, 0)
};

} // namespace CAP

#endif // CAP_USER__EventFilterCreator
