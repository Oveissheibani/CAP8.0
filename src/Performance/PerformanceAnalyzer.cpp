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
#include "PerformanceAnalyzer.hpp"
#include "ParticlePerformanceHistos.hpp"
#include "PrintHelpers.hpp"
#include "NameHelpers.hpp"
#include "RootHistogramHelpers.hpp"

ClassImp(CAP::PerformanceAnalyzer);

namespace CAP
{

PerformanceAnalyzer::PerformanceAnalyzer()
:
EventProcessorSingle<ParticlePerformanceHistos,ParticlePerformanceDerivedHistos>()
{
  appendClassName("PerformanceAnalyzer");
  setMinimumReportLevel(Object::kInfo);
  setName("PerformanceAnalyzer");
  setTitle("PerformanceAnalyzer");
}

PerformanceAnalyzer::PerformanceAnalyzer(const PerformanceAnalyzer & source)
:
EventProcessorSingle<ParticlePerformanceHistos,ParticlePerformanceDerivedHistos>(source)
{   }

PerformanceAnalyzer & PerformanceAnalyzer::operator=(const PerformanceAnalyzer & rhs)
{
  if (this!=&rhs)
    {
    EventProcessorSingle<ParticlePerformanceHistos,ParticlePerformanceDerivedHistos>::operator=(rhs);
    }
  return *this;
}

void PerformanceAnalyzer::setDefaultConfiguration()
{
  EventProcessorSingle<ParticlePerformanceHistos,ParticlePerformanceDerivedHistos>::setDefaultConfiguration();
  String taskName = name();
  String objectType = "HISTOGRAM";
  _configuration.addProperty(createKey(taskName,objectType,"pt_nbins"),100);
  _configuration.addProperty(createKey(taskName,objectType,"pt_min"), 0.0);
  _configuration.addProperty(createKey(taskName,objectType,"pt_max"), 5.0);
  _configuration.addProperty(createKey(taskName,objectType,"dpt_nbins"), 100);
  _configuration.addProperty(createKey(taskName,objectType,"dpt_min"),  -1.0);
  _configuration.addProperty(createKey(taskName,objectType,"dpt_max"),   1.0);
  _configuration.addProperty(createKey(taskName,objectType,"phi_nbins"), 100);
  _configuration.addProperty(createKey(taskName,objectType,"phi_min"),  -1.0);
  _configuration.addProperty(createKey(taskName,objectType,"phi_max"),   1.0);
  _configuration.addProperty(createKey(taskName,objectType,"Dphi_nbins"), 100);
  _configuration.addProperty(createKey(taskName,objectType,"Dphi_min"),  -1.0);
  _configuration.addProperty(createKey(taskName,objectType,"Dphi_max"),   1.0);
  _configuration.addProperty(createKey(taskName,objectType,"eta_nbins"), 100);
  _configuration.addProperty(createKey(taskName,objectType,"eta_min"),  -1.0);
  _configuration.addProperty(createKey(taskName,objectType,"eta_max"),   1.0);
  _configuration.addProperty(createKey(taskName,objectType,"Deta_nbins"), 100);
  _configuration.addProperty(createKey(taskName,objectType,"Deta_min"),  -1.0);
  _configuration.addProperty(createKey(taskName,objectType,"Deta_max"),   1.0);
  _configuration.addProperty(createKey(taskName,objectType,"rapidity_nbins"), 100);
  _configuration.addProperty(createKey(taskName,objectType,"rapidity_min"),  -1.0);
  _configuration.addProperty(createKey(taskName,objectType,"rapidity_max"),  -1.0);
  _configuration.addProperty(createKey(taskName,objectType,"Drapidity_nbins"), 100);
  _configuration.addProperty(createKey(taskName,objectType,"Drapidity_min"),  -1.0);
  _configuration.addProperty(createKey(taskName,objectType,"Drapidity_max"),   1.0);
}

void PerformanceAnalyzer::execute()
{
  _taskExecuted.increment();
  Event & theEvent = event();
  if (!filterEvent(theEvent)) return;
  if (!filterParticles(theEvent.particles()))  return;
  unsigned int npf = nParticleFilters();
  unsigned int iEventFilter = 0;
  std::vector<ParticlePerformanceHistos*> & histograms = histograms_1();
  for (auto accepted : _acceptedParticleFilters)
    {
    if (!accepted) continue;
    unsigned int  baseSingle   = iEventFilter*npf;
    for (unsigned int iParticleFilter=0; iParticleFilter<npf;iParticleFilter++)
      {
      ParticlePerformanceHistos * histos = histograms[baseSingle+iParticleFilter];
      std::vector<Particle*> & accepted = acceptedParticles(iParticleFilter);
      for (auto & particle : accepted) histos->fill(*particle,1.0);
      }
    iEventFilter++;
    }
}

} // namespace CAP
