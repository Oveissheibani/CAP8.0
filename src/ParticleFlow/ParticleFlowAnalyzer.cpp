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
#include "ParticleFlowAnalyzer.hpp"
#include "PrintHelpers.hpp"
#include "NameHelpers.hpp"
#include "RootHistogramHelpers.hpp"

ClassImp(CAP::ParticleFlowAnalyzer);

namespace CAP
{

ParticleFlowAnalyzer::ParticleFlowAnalyzer()
:
EventProcessorSingle<ParticleFlowHistos,ParticleFlowDerivedHistos>()
{
  appendClassName("ParticleFlowAnalyzer");
  setMinimumReportLevel(Object::kInfo);
  setName("ParticleFlowAnalyzer");
  setTitle("ParticleFlowAnalyzer");
}

ParticleFlowAnalyzer::ParticleFlowAnalyzer(const ParticleFlowAnalyzer & source)
:
EventProcessorSingle<ParticleFlowHistos,ParticleFlowDerivedHistos>(source)
{   }

ParticleFlowAnalyzer & ParticleFlowAnalyzer::operator=(const ParticleFlowAnalyzer & rhs)
{
  if (this!=&rhs)
    {
    EventProcessorSingle<ParticleFlowHistos,ParticleFlowDerivedHistos>:operator=(rhs);
    }
  return *this;
}

void ParticleFlowAnalyzer::setDefaultConfiguration()
{
  EventProcessorSingle<ParticleFlowHistos,ParticleFlowDerivedHistos>::setDefaultConfiguration();
  const String & taskName = name();
  String objectType = "HISTOGRAM";
  _configuration.addProperty(createKey(taskName,objectType,"n1_nbins"),  100);
  _configuration.addProperty(createKey(taskName,objectType,"n1_min"),    0.0);
  _configuration.addProperty(createKey(taskName,objectType,"n1_max"),  100.0);

  _configuration.addProperty(createKey(taskName,objectType,"etot_nbins"),  100);
  _configuration.addProperty(createKey(taskName,objectType,"etot_min"),    0.0);
  _configuration.addProperty(createKey(taskName,objectType,"etot_max"),  100.0);

  _configuration.addProperty(createKey(taskName,objectType,"pt_nbins"),  100);
  _configuration.addProperty(createKey(taskName,objectType,"pt_min"),    0.0);
  _configuration.addProperty(createKey(taskName,objectType,"pt_max"),  100.0);

  _configuration.addProperty(createKey(taskName,objectType,"eta_nbins"),  20);
  _configuration.addProperty(createKey(taskName,objectType,"eta_min"),   -1.0);
  _configuration.addProperty(createKey(taskName,objectType,"eta_max"),    1.0);

  _configuration.addProperty(createKey(taskName,objectType,"rapidity_nbins"),  20);
  _configuration.addProperty(createKey(taskName,objectType,"rapidity_min"),   -1.0);
  _configuration.addProperty(createKey(taskName,objectType,"rapidity_max"),    1.0);

  _configuration.addProperty(createKey(taskName,objectType,"phi_nbins"),  36);
  _configuration.addProperty(createKey(taskName,objectType,"phi_min"),    0.0);
  _configuration.addProperty(createKey(taskName,objectType,"phi_max"),    CAP::MATH::twoPi());

  _configuration.addProperty(createKey(taskName,objectType,"phiEta_nbins"),    720);
  _configuration.addProperty(createKey(taskName,objectType,"phiEtaPt_nbins"),  7200);
  _configuration.addProperty(createKey(taskName,objectType,"phiY_nbins"),      720);
  _configuration.addProperty(createKey(taskName,objectType,"phiYPt_nbins"),    7200);
  _configuration.addProperty(createKey(taskName,objectType,"eta_fill"),         true);
  _configuration.addProperty(createKey(taskName,objectType,"rapidity_fill"),           false);
  _configuration.addProperty(createKey(taskName,objectType,"p2_fill"),          false);
  _configuration.addProperty(createKey(taskName,objectType,"FillPid"),         false);
}

void ParticleFlowAnalyzer::execute()
{
  _taskExecuted.increment();;
  Event & theEvent = event();
  if (!filterEvent(theEvent)) return;
  if (! filterParticles(theEvent.particles()))  return;
  unsigned int iEventFilter = 0;
  std::vector<ParticleFlowHistos*> histograms = histograms_1();
  for (auto accepted : _acceptedParticleFilters)
    {
    if (!accepted) continue;
    unsigned int  baseFlow   = iEventFilter*nParticleFilters();
    for (unsigned int iParticleFilter=0; iParticleFilter<nParticleFilters();iParticleFilter++)
      {
      ParticleFlowHistos * histosFlow = histograms[baseFlow+iParticleFilter];
      std::vector<Particle*> & particlesSelected = acceptedParticles[iParticleFilter];
      std::vector<double>   & weightsSelected   = acceptedWeights[iParticleFilter];
      double multiplicityAccepted = 0.0;
      double energyAccepted = 0.0;
      for (auto & particle : particlesSelected)
        {
        multiplicityAccepted++;
        energyAccepted += particle->momentum().t();
        histosFlow.fill(*particle,1.0);
        }
      histosFlow.fillMultiplicity(multiplicityAccepted,energyAccepted,1.0);
      }
    iEventFilter++;
    }
}

} // namespace CAP
