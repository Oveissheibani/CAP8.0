/* **********************************************************************
 * Copyright (C) 2019-2024, Claude Pruneau, Victor Gonzalez   
 * All rights reserved.
 *
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 *
 * Author: Claude Pruneau,   04/01/2024
 *
 * *********************************************************************/
#include "EventIterator.hpp"
#include "PrintHelpers.hpp"
#include "NameHelpers.hpp"

ClassImp(CAP::EventIterator);

namespace CAP
{

EventIterator::EventIterator()
:
EventProcessor(),
_nEventsRequested(1),
_nEventsReport(10)
{
  appendClassName("EventIterator");
  setMinimumReportLevel(Object::kInfo);
  setName("EventIterator");
  setTitle("EventIterator");
}

EventIterator::EventIterator(const EventIterator & source)
:
EventProcessor(source),
_nEventsRequested(source._nEventsRequested),
_nEventsReport(source._nEventsReport)
{  /* no ops */ }

EventIterator EventIterator::operator=(const EventIterator & rhs)
{
  if (this!=&rhs)
    {
    EventProcessor::operator=(rhs);
    _nEventsRequested    = rhs._nEventsRequested;
    _nEventsReport       = rhs._nEventsReport;
    }
  return *this;
}

void EventIterator::setDefaultConfiguration()
{
  EventProcessor::setDefaultConfiguration();
  const String & taskName = Task::name();
  _configuration.addProperty( createKey(taskName,"EVENT:REQUESTED:N"),_nEventsRequested);
  _configuration.addProperty( createKey(taskName,"EVENT:REPORT:N"),   _nEventsReport);
}

void EventIterator::configure(const Configuration & configuration)
{
  EventProcessor::configure(configuration);
  const String & taskName = Task::name();
  // Backward compat: shipped .ini files use the legacy keys
  // <task>:nEventsRequested / <task>:nEventsReport. The new key form is
  // <task>:EVENT:REQUESTED:N / <task>:EVENT:REPORT:N. Try the new keys
  // first, then fall back to the legacy keys when the new ones are absent
  // (valueInt returns 0 when a key is missing).
  _nEventsRequested = configuration.valueInt(createKey(taskName,"EVENT:REQUESTED:N"));
  if (_nEventsRequested <= 0)
    _nEventsRequested = configuration.valueInt(createKey(taskName,"nEventsRequested"));

  _nEventsReport    = configuration.valueInt(createKey(taskName,"EVENT:REPORT:N"));
  if (_nEventsReport <= 0)
    _nEventsReport = configuration.valueInt(createKey(taskName,"nEventsReport"));
  if (_nEventsReport <= 0)
    _nEventsReport = 1;   // sensible default — report every event

  if (reportDebug(__FUNCTION__))
    {
    printCR();
    printValue(createKey(taskName,"EVENT:REQUESTED:N"),_nEventsRequested);
    printValue(createKey(taskName,"EVENT:REPORT:N"),   _nEventsReport);
    }
}

void EventIterator::execute()
{
  if (reportDebug(__FUNCTION__)) { printCR(); }
  _timer.start();
  long iEvent = 0;
  try
  {
  while(iEvent<_nEventsRequested)
    {
    Particle::factory().reset();
    for (auto & event : events()) event->reset();
    for (auto & subTask : _subTasks) subTask->execute();
    if (iEvent%_nEventsReport == 0  &&  reportInfo(__FUNCTION__)) printValue("iEvent",iEvent);
    iEvent++;
    }
  }
  catch (CAP::EndOfDataException & exception)
  {
  if (reportWarning(__FUNCTION__))
    {
    printCR();
    printValue("EVENT:REQUESTED:N",_nEventsRequested);
    printValue("EVENT:INDEX",iEvent);
    printString("File Stream Ended");
    printLine();
    }
  _timer.stop();
  printCR();
  printLine();
  _timer.print();
  printLine();
  printCR();
  }
}


} // namespace CAP
