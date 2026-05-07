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
#include "ParticlePair3DAnalyzer.hpp"
#include "PrintHelpers.hpp"
#include "NameHelpers.hpp"
#include "RootHistogramHelpers.hpp"
#include "MathConstants.hpp"

ClassImp(CAP::ParticlePair3DAnalyzer);

namespace CAP
{

ParticlePair3DAnalyzer::ParticlePair3DAnalyzer()
:
EventProcessorPair<ParticlePair3DHistos,ParticlePair3DDerivedHistos,ParticlePair3DBfHistos,ParticleSingleDerivedHistos>()
{
  appendClassName("ParticlePair3DAnalyzer");
  setMinimumReportLevel(Object::kInfo);
  setName("ParticlePair3DAnalyzer");
  setTitle("ParticlePair3DAnalyzer");
}

ParticlePair3DAnalyzer::ParticlePair3DAnalyzer(const ParticlePair3DAnalyzer & source)
:
EventProcessorPair<ParticlePair3DHistos,ParticlePair3DDerivedHistos,ParticlePair3DBfHistos,ParticleSingleDerivedHistos>(source)
{   }

ParticlePair3DAnalyzer & ParticlePair3DAnalyzer::operator=(const ParticlePair3DAnalyzer & rhs)
{
  if (this!=&rhs)
    {
    EventProcessorPair<ParticlePair3DHistos,ParticlePair3DDerivedHistos,ParticlePair3DBfHistos,ParticleSingleDerivedHistos>::operator=(rhs);
    }
  return *this;
}


void ParticlePair3DAnalyzer::setDefaultConfiguration()
{
  if (reportDebug(__FUNCTION__)) { printCR(); }
  EventProcessorPair<ParticlePair3DHistos,ParticlePair3DDerivedHistos,ParticlePair3DBfHistos,ParticleSingleDerivedHistos>::setDefaultConfiguration();
  const String & taskName = name();
  String objectType = "HISTOGRAM";
  _configuration.addProperty(createKey(taskName,objectType,"n1_nbins"),   100);
  _configuration.addProperty(createKey(taskName,objectType,"n1_min"),     0.0);
  _configuration.addProperty(createKey(taskName,objectType,"n1_max"),     100.0);
  _configuration.addProperty(createKey(taskName,objectType,"etot_nbins"), 100);
  _configuration.addProperty(createKey(taskName,objectType,"etot_min"),   0.0);
  _configuration.addProperty(createKey(taskName,objectType,"etot_max"),   100.0);
  _configuration.addProperty(createKey(taskName,objectType,"pt_nbins"),   100);
  _configuration.addProperty(createKey(taskName,objectType,"pt_min"),     0.0);
  _configuration.addProperty(createKey(taskName,objectType,"pt_max"),     100.0);
  _configuration.addProperty(createKey(taskName,objectType,"rapidity_nbins"),    200);
  _configuration.addProperty(createKey(taskName,objectType,"rapidity_min"),      -10.0);
  _configuration.addProperty(createKey(taskName,objectType,"rapidity_max"),      10.0);
  _configuration.addProperty(createKey(taskName,objectType,"phi_nbins"),   36);
  _configuration.addProperty(createKey(taskName,objectType,"phi_min"),     0.0);
  _configuration.addProperty(createKey(taskName,objectType,"phi_max"),     CAP::MATH::twoPi());
  _configuration.addProperty(createKey(taskName,objectType,"phiEta_nbins"),     720);
  _configuration.addProperty(createKey(taskName,objectType,"phiEtaPt_nbins"),   7200);
  _configuration.addProperty(createKey(taskName,objectType,"phiY_nbins"),       720);
  _configuration.addProperty(createKey(taskName,objectType,"phiYPt_nbins"),     7200);
  _configuration.addProperty(createKey(taskName,objectType,"eta_fill"),          false);
  _configuration.addProperty(createKey(taskName,objectType,"rapidity_fill"),            true);
  _configuration.addProperty(createKey(taskName,objectType,"p2_fill"),           false);
  _configuration.addProperty(createKey(taskName,objectType,"FillPid"),          false);
  _configuration.addProperty(createKey(taskName,objectType,"FillPtvsY"),        true); // needed for n1n1 calculation
  _configuration.addProperty(createKey(taskName,objectType,"Qinv_nbins"),      90);     // q invariant
  _configuration.addProperty(createKey(taskName,objectType,"Qinv_min"),        0.0);
  _configuration.addProperty(createKey(taskName,objectType,"Qinv_max"),        1.8);
  _configuration.addProperty(createKey(taskName,objectType,"DeltaPs_nbins"),   60);     // q invariant side
  _configuration.addProperty(createKey(taskName,objectType,"DeltaPs_min"),     0.0);
  _configuration.addProperty(createKey(taskName,objectType,"DeltaPs_max"),     1.2);
  _configuration.addProperty(createKey(taskName,objectType,"DeltaPo_nbins"),   60);     // q invariant out
  _configuration.addProperty(createKey(taskName,objectType,"DeltaPo_min"),     0.0);
  _configuration.addProperty(createKey(taskName,objectType,"DeltaPo_max"),     1.2);
  _configuration.addProperty(createKey(taskName,objectType,"DeltaPl_nbins"),   60);     // q invariant long
  _configuration.addProperty(createKey(taskName,objectType,"DeltaPl_min"),     0.0);
  _configuration.addProperty(createKey(taskName,objectType,"DeltaPl_max"),     1.2);
}

void ParticlePair3DAnalyzer::execute()
{
  _taskExecuted.increment();;
  Event & theEvent = event();
  if (!filterEvent(theEvent)) return;
  if (! filterParticles(theEvent.particles()))  return;
  unsigned int iEventFilter = 0;
  unsigned int nPFsq = nParticleFilters()*nParticleFilters();
  std::vector<ParticlePair3DHistos*> & histograms = histograms_1();

  for (auto accepted : _acceptedEventFilters)
    {
    if (!accepted) continue;
    for (unsigned int iParticleFilter1=0; iParticleFilter1<nParticleFilters();iParticleFilter1++)
      {
      unsigned int basePair = iEventFilter*nPFsq + iParticleFilter1*nParticleFilters();
      std::vector<Particle*> & particlesSelected1 = acceptedParticles(iParticleFilter1);
      //std::vector<double>   & weightSelected1    = acceptedWeights(iParticleFilter1);
      //bool useWeights = 0; //weightSelected1.size()>0;
      for (unsigned int iParticleFilter2=0; iParticleFilter2<nParticleFilters();iParticleFilter2++)
        {
        bool same = iParticleFilter1==iParticleFilter2;
        std::vector<Particle*> & particlesSelected2 = acceptedParticles(iParticleFilter2);
       //std::vector<double>   & weightSelected2    = acceptedWeights(iParticleFilter2);
        ParticlePair3DHistos * histosPair = histograms[basePair+iParticleFilter2];
        int n1 = particlesSelected1.size();
        int n2 = particlesSelected2.size();
        for (int i1=0; i1<n1; i1++)
          {
          for (int i2=0; i2<n1; i2++)
            {
            if (!same || i1!=i2)
              {
              double weight = 1.0; //useWeights ? weightSelected1[i1]*weightSelected2[i2] : 1.0;
              histosPair->fill(*(particlesSelected1[i1]),*(particlesSelected1[i2]),weight);
              }
            }
          }
        if (same)
          histosPair->fillMultiplicity(double(n1*(n1-1)));
        else
          histosPair->fillMultiplicity(double(n1*n2));
        }
      }
    iEventFilter++;
    }
}

double ParticlePair3DAnalyzer::getMassFor(ParticleFilter* pf __attribute__((unused)))
{
  int pdgCode = 211; //pidFor(pf);
  ParticleDb & adb = db();
  ParticleType * particleType = adb.findPdgCode(pdgCode);
  return particleType->mass();
}


} // namespace CAP
