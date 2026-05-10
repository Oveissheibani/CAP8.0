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
#include "ParticlePairAnalyzer.hpp"
#include "PrintHelpers.hpp"
#include "NameHelpers.hpp"
#include "PrintHelpers.hpp"
#include "RootHistogramHelpers.hpp"
#include "MathConstants.hpp"

ClassImp(CAP::ParticlePairAnalyzer);

namespace CAP
{

ParticlePairAnalyzer::ParticlePairAnalyzer()
:
EventProcessorPair<ParticlePairHistos,ParticlePairDerivedHistos,ParticlePairBfHistos,ParticleSingleDerivedHistos>()
{
  appendClassName("ParticlePairAnalyzer");
  setName("ParticlePairAnalyzer");
  setMinimumReportLevel(Object::kInfo);
  setName("ParticlePairAnalyzer");
  setTitle("ParticlePairAnalyzer");
}

ParticlePairAnalyzer::ParticlePairAnalyzer(const ParticlePairAnalyzer & source)
:
EventProcessorPair<ParticlePairHistos,ParticlePairDerivedHistos,ParticlePairBfHistos,ParticleSingleDerivedHistos>(source)
{   }

ParticlePairAnalyzer & ParticlePairAnalyzer::operator=(const ParticlePairAnalyzer & rhs)
{
  if (this!=&rhs)
    {
    EventProcessorPair<ParticlePairHistos,ParticlePairDerivedHistos,ParticlePairBfHistos,ParticleSingleDerivedHistos>::operator=(rhs);
    }
  return *this;
}

void ParticlePairAnalyzer::setDefaultConfiguration()
{
  EventProcessorPair<ParticlePairHistos,ParticlePairDerivedHistos,ParticlePairBfHistos,ParticleSingleDerivedHistos>::setDefaultConfiguration();
  const String & taskName = name();
  String objectType = "HISTOGRAM";
  _configuration.addProperty(createKey(taskName,objectType,"eta_fill"),       false);
  _configuration.addProperty(createKey(taskName,objectType,"rapidity_fill"),  true);
  _configuration.addProperty(createKey(taskName,objectType,"p2_fill"),        false);
  _configuration.addProperty(createKey(taskName,objectType,"FillPid"),        false);
  _configuration.addProperty(createKey(taskName,objectType,"n1_nbins"),       100);
  _configuration.addProperty(createKey(taskName,objectType,"n1_min"),         0.0);
  _configuration.addProperty(createKey(taskName,objectType,"n1_max"),         100.0);
  _configuration.addProperty(createKey(taskName,objectType,"etot_nbins"),     100);
  _configuration.addProperty(createKey(taskName,objectType,"etot_min"),       0.0);
  _configuration.addProperty(createKey(taskName,objectType,"etot_max"),       100.0);
  _configuration.addProperty(createKey(taskName,objectType,"pt_nbins"),       18);
  _configuration.addProperty(createKey(taskName,objectType,"pt_min"),         0.20);
  _configuration.addProperty(createKey(taskName,objectType,"pt_max"),         2.00);
  _configuration.addProperty(createKey(taskName,objectType,"phi_nbins"),      72);
  _configuration.addProperty(createKey(taskName,objectType,"phi_min"),        0.0);
  _configuration.addProperty(createKey(taskName,objectType,"phi_max"),        CAP::MATH::twoPi());
  _configuration.addProperty(createKey(taskName,objectType,"eta_nbins"),      20);
  _configuration.addProperty(createKey(taskName,objectType,"eta_min"),        -1.0);
  _configuration.addProperty(createKey(taskName,objectType,"eta_max"),        1.0);
  _configuration.addProperty(createKey(taskName,objectType,"rapidity_nbins"), 20);
  _configuration.addProperty(createKey(taskName,objectType,"rapidity_min"),    -1.0);
  _configuration.addProperty(createKey(taskName,objectType,"rapidity_max"),     1.0);
  _configuration.addProperty(createKey(taskName,objectType,"n2_nbins"),         100);
  _configuration.addProperty(createKey(taskName,objectType,"n2_min"),           0.0);
  _configuration.addProperty(createKey(taskName,objectType,"n2_max"),          1000.0);
  _configuration.addProperty(createKey(taskName,objectType,"Dphi_nbins"),      36);
  _configuration.addProperty(createKey(taskName,objectType,"Dphi_min"),        0.0);
  _configuration.addProperty(createKey(taskName,objectType,"Dphi_max"),        CAP::MATH::twoPi());
  _configuration.addProperty(createKey(taskName,objectType,"Dphi_width"),      CAP::MATH::twoPi());
  _configuration.addProperty(createKey(taskName,objectType,"Dphi_nbins_shft"),  36);
  _configuration.addProperty(createKey(taskName,objectType,"Dphi_min_shft"),   0.0);
  _configuration.addProperty(createKey(taskName,objectType,"Dphi_max_shft"),   0.0);
  _configuration.addProperty(createKey(taskName,objectType,"Deta_nbins"),       39);
  _configuration.addProperty(createKey(taskName,objectType,"Deta_min"),       -2.0);
  _configuration.addProperty(createKey(taskName,objectType,"Deta_max"),        2.0);
  _configuration.addProperty(createKey(taskName,objectType,"Deta_width"),       4.0/39.0);
  _configuration.addProperty(createKey(taskName,objectType,"Drapidity_nbins"), 39);
  _configuration.addProperty(createKey(taskName,objectType,"Drapidity_min"),   -2.0);
  _configuration.addProperty(createKey(taskName,objectType,"Drapidity_max"),    2.0);
  _configuration.addProperty(createKey(taskName,objectType,"Drapidity_width"), 4.0/39.0);
  _configuration.addProperty(createKey(taskName,objectType,"DeltaP_nbins"),     10);
  _configuration.addProperty(createKey(taskName,objectType,"DeltaP_min"),       -4.0);
  _configuration.addProperty(createKey(taskName,objectType,"DeltaP_max"),      4.0);
}

void ParticlePairAnalyzer::execute()
{
  //if (reportInfo(__FUNCTION__)) { printCR(); }
  _taskExecuted.increment();
  //printValue("CALLED COUNT",taskExecuted.valueAt());
  Event & theEvent = event();
  if (reportDebug0(__FUNCTION__))
    {
    printCR();
    printValue("event.particles().size()",theEvent.particles().size());
    }
  if (!filterEvent(theEvent)) return;
  if (!filterParticles(theEvent.particles()))  return;
  unsigned int iEventFilter = 0;
  unsigned int nPFs  = nParticleFilters();
  unsigned int nPFsq = nPFs*nPFs;
  unsigned int index;
  std::vector<ParticlePairHistos*> & pairHistograms = histograms_1();
  double weight = 1.0;
  for (auto accepted : acceptedEventFilters())
    {
    if (!accepted) continue;
    for (unsigned int iParticleFilter1=0; iParticleFilter1<nPFs;iParticleFilter1++)
      {
      unsigned int basePair = iEventFilter*nPFsq + iParticleFilter1*nPFs;
      std::vector<Particle*> & particlesSelected1 = acceptedParticles(iParticleFilter1);
      //std::vector<double>   & weightSelected1    = acceptedWeights(iParticleFilter1);
      //bool useWeights = weightSelected1.size()>0;
      for (unsigned int iParticleFilter2=0; iParticleFilter2<nParticleFilters();iParticleFilter2++)
        {
        bool same = iParticleFilter1==iParticleFilter2;
        std::vector<Particle*> & particlesSelected2 = acceptedParticles(iParticleFilter2);
        //std::vector<double>   & weightSelected2    = acceptedWeights(iParticleFilter2);
        index = basePair+iParticleFilter2;
        // Original code had `if (index>3) throw` — a leftover developer's
        // assertion that capped the analyzer at 1 EF x 2 PF.  Replaced
        // with a proper bounds check against the actual histogram count.
        if (index >= pairHistograms.size())
          throw Exception("pair histogram index out of range", __FUNCTION__);
        ParticlePairHistos * histosPair = pairHistograms[index];
        if (!histosPair) throw Exception("!histosPair",__FUNCTION__);
        int n1 = particlesSelected1.size();
        int n2 = particlesSelected2.size();
       for (int i1=0; i1<n1; i1++)
          {
          Particle & particle1 = *(particlesSelected1[i1]);
          for (int i2=0; i2<n2; i2++)
            {
            if (!same || i1!=i2)
              {
              Particle & particle2 = *(particlesSelected2[i2]);
              histosPair->fill(particle1,particle2,weight);
              }
            }
          }
        if (same)
          histosPair->fillMultiplicity(double(n1*(n1-1)));
        else
          histosPair->fillMultiplicity(double(n1*n2));
        }
      }
    //printValue("MYSTERY",15);

    iEventFilter++;
    }
}

} // namespace CAP
