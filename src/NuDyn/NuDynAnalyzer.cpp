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
#include "NuDynAnalyzer.hpp"
#include "NuDynHistos.hpp"
#include "PrintHelpers.hpp"
#include "NameHelpers.hpp"

ClassImp(CAP::NuDynAnalyzer);

namespace CAP
{

NuDynAnalyzer::NuDynAnalyzer()
:
EventProcessorSingle<NuDynHistos,NuDynDerivedHistos>()
{
  appendClassName("NuDynAnalyzer");
  setMinimumReportLevel(Object::kInfo);
  setName("NuDynAnalyzer");
  setTitle("NuDynAnalyzer");
}

NuDynAnalyzer::NuDynAnalyzer(const NuDynAnalyzer & source)
:
EventProcessorSingle<NuDynHistos,NuDynDerivedHistos>(source)
{   }

NuDynAnalyzer & NuDynAnalyzer::operator=(const NuDynAnalyzer & rhs)
{
  if (this!=&rhs)
    {
    EventProcessorSingle<NuDynHistos,NuDynDerivedHistos>::operator=(rhs);
    }
  return *this;
}

void NuDynAnalyzer::setDefaultConfiguration()
{
  if (reportDebug(__FUNCTION__)) { printCR(); }
  EventProcessorSingle<NuDynHistos,NuDynDerivedHistos>::setDefaultConfiguration();
  const String & taskName = name();
  String objectType = "HISTOGRAM";
  _configuration.addProperty(createKey(taskName,objectType,"evtName"), String("Mult0"));
  _configuration.addProperty(createKey(taskName,objectType,"evt_nbins"),200);
  _configuration.addProperty(createKey(taskName,objectType,"evt_min"),0.0);
  _configuration.addProperty(createKey(taskName,objectType,"evt_max"),200.0);
}

void NuDynAnalyzer::execute()
{
  _taskExecuted.increment();;
  Event & theEvent = event();
  if (!filterEvent(theEvent)) return;
  if (!filterParticles(theEvent.particles()))  return;
  double mult = getEvtClassValue();
  //double weight = 1.0;
  unsigned int nef = nEventFilters();
  unsigned int npf = nParticleFilters();
  unsigned int eventFilterBaseIndex;
  unsigned int particleFilterBaseIndex;
  std::vector<NuDynHistos*> & histograms = histograms_1();

  for (unsigned int iEventFilter=0; iEventFilter<nef;iEventFilter++)
    {
    if (!_acceptedEventFilters[iEventFilter]) continue;
    eventFilterBaseIndex = iEventFilter*npf*npf;
    std::vector<double> multiplicities;
    for (unsigned int iParticleFilter1=0; iParticleFilter1<npf;iParticleFilter1++)
      {
      std::vector<double> & moments1 = moments(iParticleFilter1);
      double n1_1 = moments1[0];
      double n2_11 = n1_1*(n1_1-1);
      particleFilterBaseIndex = eventFilterBaseIndex + iParticleFilter1*npf;
      for (unsigned int iParticleFilter2=0; iParticleFilter2<iParticleFilter1;iParticleFilter2++)
        {
        std::vector<double> & moments2 = moments(iParticleFilter2);
        double n1_2 = moments2[0];
        double n2_12 = n1_1*n1_2;
        double n2_22 = n1_2*(n1_2-1);
        histograms[particleFilterBaseIndex+iParticleFilter2]->fill(mult,n1_1,n1_2,n2_11,n2_12,n2_22);
        }
      }
    }
}

} // namespace CAP

