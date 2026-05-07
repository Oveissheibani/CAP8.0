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
#include "GlobalAnalyzer.hpp"
#include "PrintHelpers.hpp"
#include "NameHelpers.hpp"
#include "RootHistogramHelpers.hpp"

ClassImp(CAP::GlobalAnalyzer);

namespace CAP
{

GlobalAnalyzer::GlobalAnalyzer()
:
  EventProcessorSingle<GlobalHistos,GlobalDerivedHistos>()
{
  appendClassName("GlobalAnalyzer");
  setMinimumReportLevel(Object::kInfo);
  setName("GlobalAnalyzer");
  setTitle("GlobalAnalyzer");
}

GlobalAnalyzer::GlobalAnalyzer(const GlobalAnalyzer & source)
:
  EventProcessorSingle<GlobalHistos,GlobalDerivedHistos>(source)
{   }

GlobalAnalyzer & GlobalAnalyzer::operator=(const GlobalAnalyzer & rhs)
{
  if (this!=&rhs)
    {
    EventProcessorSingle<GlobalHistos,GlobalDerivedHistos>::operator=(rhs);
    }
  return *this;
}

void GlobalAnalyzer::setDefaultConfiguration()
{
  EventProcessorSingle<GlobalHistos,GlobalDerivedHistos>::setDefaultConfiguration();
  const String & taskName = name();
  String objectType = "HISTOGRAM";
  _configuration.addProperty(createKey(taskName,objectType,"Fill2D"),     false);
  _configuration.addProperty(createKey(taskName,objectType,"n_nbins"),     500);
  _configuration.addProperty(createKey(taskName,objectType,"n_min"),       0.0);
  _configuration.addProperty(createKey(taskName,objectType,"n_max"),       5000.0);
  _configuration.addProperty(createKey(taskName,objectType,"e_nbins"),     500);
  _configuration.addProperty(createKey(taskName,objectType,"e_min"),       0.0);
  _configuration.addProperty(createKey(taskName,objectType,"e_max"),       10000.0);
  _configuration.addProperty(createKey(taskName,objectType,"q_nbins"),     200);
  _configuration.addProperty(createKey(taskName,objectType,"q_min"),       -100.0);
  _configuration.addProperty(createKey(taskName,objectType,"q_max"),       100.0);
  _configuration.addProperty(createKey(taskName,objectType,"s_nbins"),     200);
  _configuration.addProperty(createKey(taskName,objectType,"s_min"),       -100.0);
  _configuration.addProperty(createKey(taskName,objectType,"s_max"),       100.0);
  _configuration.addProperty(createKey(taskName,objectType,"b_nbins"),     200);
  _configuration.addProperty(createKey(taskName,objectType,"b_min"),       -100.0);
  _configuration.addProperty(createKey(taskName,objectType,"b_max"),       100.0);
  _configuration.addProperty(createKey(taskName,objectType,"pt_nbinsSum"), 100);
  _configuration.addProperty(createKey(taskName,objectType,"pt_minSum"),   0.0);
  _configuration.addProperty(createKey(taskName,objectType,"pt_maxSum"),   10000.0);
  _configuration.addProperty(createKey(taskName,objectType,"pt_nbinsAvg"), 100);
  _configuration.addProperty(createKey(taskName,objectType,"pt_minAvg"),   0.0);
  _configuration.addProperty(createKey(taskName,objectType,"pt_maxAvg"),   10.0);
}

void GlobalAnalyzer::execute()
{
  Event & theEvent = event();
  if (!filterEvent(theEvent)) return;
  if (!filterParticles(theEvent.particles()))  return;

  double n ,e, q, s, b, ptSum;
  unsigned int baseIndex = 0;
  unsigned int index = 0;
  theEvent.resetMult();
  theEvent.resetEnergy();
  theEvent.resetCharge();
  theEvent.resetStrangeness();
  theEvent.resetBaryon();
  theEvent.resetPtSum();
  std::vector<GlobalHistos*> histograms = histograms_1();
  //printValue("GlobalAnalyzer::execute()",4);
  for (unsigned int iEventFilter = 0;iEventFilter<nEventFilters();iEventFilter++)
    {
    //printValue("GlobalAnalyzer::execute() iEventFilter",iEventFilter);
    baseIndex = iEventFilter*nParticleFilters();
    if (_acceptedEventFilters[iEventFilter])
      {
      for (unsigned int iParticleFilter = 0; iParticleFilter<nParticleFilters(); iParticleFilter++)
        {
        //printValue("GlobalAnalyzer::execute() iParticleFilter",iParticleFilter);
        if (_acceptedParticleFilters[iParticleFilter])
          {
          index = baseIndex+iParticleFilter;
         // std::vector<Particle*> & particlesSelected = acceptedParticles(iParticleFilter);
          std::vector<Particle*> & particlesSelected = acceptedParticles(iParticleFilter);
          n = 0;
          e = 0;
          q = 0;
          s = 0;
          b = 0;
          ptSum = 0;
          for (auto & particle : particlesSelected)
            {
            ParticleType & type = particle->type();
            VectorLorentz & momentum = particle->momentum();
            n++;
            e += momentum.t();
            q += type.charge();
            s += type.strangeness();
            b += type.baryonNumber();
            ptSum += momentum.perp();
            }
          theEvent.setMult(iEventFilter,n);
          theEvent.setEnergy(iEventFilter,e);
          theEvent.setCharge(iEventFilter,q);
          theEvent.setStrangeness(iEventFilter,s);
          theEvent.setBaryon(iEventFilter,b);
          theEvent.setPtSum(iEventFilter,ptSum);
          GlobalHistos * histos =  histograms[index];
          histos->fill(n,ptSum,e,q,s,b,1.0);
          }
        }
      }
    }
  _taskExecuted.increment();;
}


} // namespace CAP
