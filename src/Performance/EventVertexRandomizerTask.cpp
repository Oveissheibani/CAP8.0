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
#include "EventVertexRandomizerTask.hpp"
#include "PrintHelpers.hpp"
#include "NameHelpers.hpp"

ClassImp(CAP::EventVertexRandomizerTask);

namespace CAP
{
EventVertexRandomizerTask::EventVertexRandomizerTask()
:
EventProcessor(),
rConversion(0), tConversion(0),
xAvg(0), yAvg(0), zAvg(0), tAvg(0),
xRms(0), yRms(0), zRms(0), tRms(0)
{
  appendClassName("EventVertexRandomizerTask");
  setMinimumReportLevel(Object::kInfo);
  setName("EventVertexRandomizerTask");
  setTitle("EventVertexRandomizerTask");
}

//!
//! vertex position and size supplied in micro-meters.
//! vertex time is nanosecond relative to nominal crossing time
//! convert to fm for internal use: 1  micro-meter = 1E9 fm
//!
void EventVertexRandomizerTask::setDefaultConfiguration()
{
  EventProcessor::setDefaultConfiguration();
  const String & taskName = name();
  _configuration.addProperty(createKey(taskName,"EventsUseStream0"),true);
  _configuration.addProperty(createKey(taskName,"rConversion"),1.0E9);
  _configuration.addProperty(createKey(taskName,"tConversion"),1.0E9);
  _configuration.addProperty(createKey(taskName,"xAvg"),0.0);
  _configuration.addProperty(createKey(taskName,"yAvg"),0.0);
  _configuration.addProperty(createKey(taskName,"zAvg"),0.0);
  _configuration.addProperty(createKey(taskName,"tAvg"),0.0);
  _configuration.addProperty(createKey(taskName,"xRms"),100.0);
  _configuration.addProperty(createKey(taskName,"yRms"),100.0);
  _configuration.addProperty(createKey(taskName,"zRms"),5.0);
  _configuration.addProperty(createKey(taskName,"tRms"),10.0);
}

void EventVertexRandomizerTask::initialize()
{
  EventProcessor::initialize();
  const String & taskName = Task::name();
  rConversion = _configuration.valueDouble(createKey(taskName,"rConversion"));
  tConversion = _configuration.valueDouble(createKey(taskName,"tConversion"));
  xAvg = _configuration.valueDouble(createKey(taskName,"xAvg"));
  yAvg = _configuration.valueDouble(createKey(taskName,"yAvg"));
  zAvg = _configuration.valueDouble(createKey(taskName,"zAvg"));
  tAvg = _configuration.valueDouble(createKey(taskName,"tAvg"));
  xRms = _configuration.valueDouble(createKey(taskName,"xRms"));
  yRms = _configuration.valueDouble(createKey(taskName,"yRms"));
  zRms = _configuration.valueDouble(createKey(taskName,"zRms"));
  tRms = _configuration.valueDouble(createKey(taskName,"tRms"));
}

void EventVertexRandomizerTask::execute()
{
  double eventX = gRandom->Gaus(rConversion*xAvg, rConversion*xRms);
  double eventY = gRandom->Gaus(rConversion*yAvg, rConversion*yRms);
  double eventZ = gRandom->Gaus(rConversion*zAvg, rConversion*zRms);
  double eventT = gRandom->Gaus(tConversion*tAvg, tConversion*tRms);
  for (auto & particle : event().particles()) particle->shift(eventX,eventY,eventZ,eventT);
}

} // namespace CAP
