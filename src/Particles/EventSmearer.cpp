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
#include "EventSmearer.hpp"
#include "NameHelpers.hpp"
#include "PrintHelpers.hpp"
#include "Task.hpp"
#include "Configuration.hpp"
#include "NameHelpers.hpp"
#include "RootHistogramHelpers.hpp"

ClassImp(CAP::EventSmearer);

namespace CAP
{

EventSmearer::EventSmearer()
:
HistogramGroup()
{
  appendClassName("EventSmearer");
  setMinimumReportLevel(Object::kInfo);
  setName("EventSmearer");
  setTitle("EventSmearer");
}

EventSmearer::EventSmearer(const EventSmearer & source)
:
HistogramGroup(source)
{
  cloneB(source);
}

EventSmearer & EventSmearer::operator=(const EventSmearer & rhs)
{
  if (this!=&rhs)
    {
    HistogramGroup::operator=(rhs);
    cloneB(rhs);
    }
  return *this;
}

void EventSmearer::cloneB(const EventSmearer & source __attribute__((unused)) )
{
  if (reportDebug(__FUNCTION__)) { printCR(); }
}

void EventSmearer::configure(const String & ownerName,
                                   const String & typeName,
                                   const Configuration & configuration,
                                   unsigned int index)
{
  if (reportDebug(__FUNCTION__)) { printCR(); }
  HistogramGroup::configure(ownerName,typeName,configuration,index);
}


void EventSmearer::smear(const Event & source __attribute__((unused)),
                         Event & target  __attribute__((unused)))
{

}

} // namespace CAP
