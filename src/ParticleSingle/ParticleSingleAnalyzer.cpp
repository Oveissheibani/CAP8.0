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
#include "ParticleSingleAnalyzer.hpp"
#include "PrintHelpers.hpp"
#include "NameHelpers.hpp"
#include "RootHistogramHelpers.hpp"

ClassImp(CAP::ParticleSingleAnalyzer);

namespace CAP
{

ParticleSingleAnalyzer::ParticleSingleAnalyzer()
:
EventProcessorSingle<ParticleSingleHistos,ParticleSingleDerivedHistos>()
{
  appendClassName("ParticleSingleAnalyzer");
  setMinimumReportLevel(Object::kInfo);
  setName("ParticleSingleAnalyzer");
  setTitle("ParticleSingleAnalyzer");
}

ParticleSingleAnalyzer::ParticleSingleAnalyzer(const ParticleSingleAnalyzer & source)
:
EventProcessorSingle<ParticleSingleHistos,ParticleSingleDerivedHistos>(source)
{   }

ParticleSingleAnalyzer & ParticleSingleAnalyzer::operator=(const ParticleSingleAnalyzer & rhs)
{
  if (this!=&rhs)
    {
    EventProcessorSingle<ParticleSingleHistos,ParticleSingleDerivedHistos>::operator=(rhs);
    }
  return *this;
}

void ParticleSingleAnalyzer::setDefaultConfiguration()
{
  if (reportDebug(__FUNCTION__)) { printCR();}
  EventProcessorSingle<ParticleSingleHistos,ParticleSingleDerivedHistos>::setDefaultConfiguration();
  String taskName = name();
  String objectType = "HISTOGRAM";
  if (reportInfo(__FUNCTION__))
    {
    printCR();
    printValue("taskName",taskName);
    printValue("objectType",objectType);
    }
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
  _configuration.addProperty(createKey(taskName,objectType,"eta_fill"),        true);
  _configuration.addProperty(createKey(taskName,objectType,"rapidity_fill"),   false);
  _configuration.addProperty(createKey(taskName,objectType,"p2_fill"),         false);
  if (reportEnd(__FUNCTION__))
    {
    printCR();
    printValue("TRACE OK",1);
    }

}

void ParticleSingleAnalyzer::execute()
{
  _taskExecuted.increment();
  Event & theEvent = event();
  if (!filterEvent(theEvent)) return;
  if (!filterParticles(theEvent.particles()))  return;
  unsigned int baseSingle;
  unsigned int index;
  double weight;
  bool usePartEff = particleEfficiencies().size()>0;
  const std::vector<bool> & acceptedEF = acceptedEventFilters();
  const std::vector<bool> & acceptedPF = acceptedParticleFilters();
  std::vector<ParticleSingleHistos*> & histograms = histograms_1();
  if (histograms.size() < 1)
    throw Exception("no histograms",__FUNCTION__);
  std::vector<ParticleEfficiency*> & particleEff = particleEfficiencies();
  unsigned int nef = acceptedEF.size();
  unsigned int npf = acceptedPF.size();
  for (unsigned int iEventFilter = 0;iEventFilter<nef;iEventFilter++)
    {
    if (!acceptedEF[iEventFilter]) continue;
    baseSingle = iEventFilter*nParticleFilters();
    for (unsigned int iParticleFilter=0; iParticleFilter<npf;iParticleFilter++)
      {
      if (!acceptedPF[iParticleFilter]) continue;
      index = baseSingle + iParticleFilter;
      ParticleSingleHistos * histos = histograms[index];
      std::vector<Particle*> & accParticles = acceptedParticles(iParticleFilter);
      double multiplicityAccepted = 0.0;
      double energyAccepted = 0.0;
      for (unsigned int k=0; k<accParticles.size(); k++)
        {
        Particle * particle = accParticles[k];
        multiplicityAccepted++;
        VectorLorentz mm = particle->momentum();
        energyAccepted += mm.t();
        if (usePartEff)
          weight = particleEff[k]->weightAt(mm);
        else
          weight = 1.0;
        histos->fill(*particle,weight);
        }
      histos->fillMultiplicity(multiplicityAccepted,energyAccepted,1.0);
      }
    iEventFilter++;
    }
}

} // namespace CAP
