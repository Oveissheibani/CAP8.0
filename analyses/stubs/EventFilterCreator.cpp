/* **********************************************************************
 *  Stub implementation of CAP::EventFilterCreator — see header.
 * ******************************************************************** */
#include "EventFilterCreator.hpp"

ClassImp(CAP::EventFilterCreator);

namespace CAP
{

EventFilterCreator::EventFilterCreator()
:
EventProcessor()
{
  appendClassName("EventFilterCreator");
  setMinimumReportLevel(Object::kInfo);
  setName("EventFilterCreator");
  setTitle("EventFilterCreator");
}

EventFilterCreator::EventFilterCreator(const EventFilterCreator & source)
:
EventProcessor(source)
{ }

EventFilterCreator & EventFilterCreator::operator=(const EventFilterCreator & rhs)
{
  if (this != &rhs)
    {
    EventProcessor::operator=(rhs);
    }
  return *this;
}

void EventFilterCreator::execute()
{
  // Nothing per-tick — filters are made during configure().
  _taskExecuted.increment();
}

} // namespace CAP
