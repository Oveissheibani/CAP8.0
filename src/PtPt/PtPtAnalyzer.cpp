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
#include "PtPtAnalyzer.hpp"
#include "PrintHelpers.hpp"
#include "NameHelpers.hpp"
#include "RootHistogramHelpers.hpp"

ClassImp(CAP::PtPtAnalyzer);

namespace CAP
{

PtPtAnalyzer::PtPtAnalyzer()
:
EventProcessorSingle<PtPtHistos,PtPtDerivedHistos>()
{
  appendClassName("PtPtAnalyzer");
  setMinimumReportLevel(Object::kInfo);
  setName("PtPtAnalyzer");
  setTitle("PtPtAnalyzer");
}

PtPtAnalyzer::PtPtAnalyzer(const PtPtAnalyzer & source)
:
EventProcessorSingle<PtPtHistos,PtPtDerivedHistos>(source)
{   }


void PtPtAnalyzer::setDefaultConfiguration()
{
  EventProcessorSingle<PtPtHistos,PtPtDerivedHistos>::setDefaultConfiguration();
  const String & taskName = name();
  String objectType = "HISTOGRAM";
  _configuration.addProperty(createKey(taskName,objectType,"evtName"),   String("ALL"));
  _configuration.addProperty(createKey(taskName,objectType,"evt_nbins"), 200);
  _configuration.addProperty(createKey(taskName,objectType,"evt_min"),   0.0);
  _configuration.addProperty(createKey(taskName,objectType,"evt_max"),   200.0);
}

void PtPtAnalyzer::execute()
{
  _taskExecuted.increment();
  Event & theEvent = event();
  if (!filterEvent(theEvent)) return;
  if (!filterParticles(theEvent.particles()))  return;
//  double mult = getEvtClassValue();
//  for (unsigned int iEventFilter=0; iEventFilter<nEventFilters();iEventFilter++)
//    {
//    if (!_acceptedParticleFilters[iEventFilter]) continue;
//    PtPtHistos * histos = static_cast<PtPtHistos *>(listAt(iEventFilter));
//    std::vector<double> & moments = particleMoments[iEventFilter];
//    histos->fill(mult,moments);
//    }
}

} // namespace CAP

