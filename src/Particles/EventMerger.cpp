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
#include "EventMerger.hpp"
#include "PrintHelpers.hpp"
#include "NameHelpers.hpp"

ClassImp(CAP::EventMerger);

namespace CAP
{

  EventMerger::EventMerger()
  :
  EventProcessor(),
  _nEventsPerStream()
  {
  appendClassName("EventMerger");
  setMinimumReportLevel(Object::kInfo);
  setName("EventMerger");
  setTitle("EventMerger");
  }

  EventMerger::EventMerger(const EventMerger & source)
  :
  EventProcessor(source),
  _nEventsPerStream(source._nEventsPerStream)
  {  /* no ops */ }

  EventMerger EventMerger::operator=(const EventMerger & rhs)
  {
  if (this!=&rhs)
    {
    EventProcessor::operator=(rhs);
    _nEventsPerStream = rhs._nEventsPerStream;
    }
  return *this;
  }

  void EventMerger::setDefaultConfiguration()
  {
  EventProcessor::setDefaultConfiguration();
  //const String & taskName = Task::name();
  //_configuration.addProperty( createKey(taskName,"INPUT_STREAM_0:MULT"),1);
  //_configuration.addProperty( createKey(taskName,"INPUT_STREAM_1:MULT"),1);
  }

  void EventMerger::configure(const Configuration & configuration)
  {
  EventProcessor::configure(configuration);
  const String & taskName = Task::name();
  unsigned int nSubs = nSubTasks();
  if (reportDebug(__FUNCTION__))
    {
    printCR();
    printValue(createKey(taskName,"SUBTASKS:N"),nSubs);
    }
  for (unsigned int iTask=0; iTask<nSubs; iTask++)
    {
    unsigned int mult = configuration.valueInt(createKey(taskName,"INPUT_STREAM",iTask,"MULT"));
    _nEventsPerStream.push_back(mult);
    }
  }

  void EventMerger::execute()
  {
  //if (reportDebug(__FUNCTION__)) { printCR(); }
  Particle::factory().reset();
  for (unsigned int iTask=0; iTask<nSubTasks(); iTask++)
    {
    for (unsigned int iEvent=0; iEvent<_nEventsPerStream[iTask]; iEvent++) _subTasks[iTask]->execute();
    }
  }


} // namespace CAP
