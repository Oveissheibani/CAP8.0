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
#include "ParticlePairBfCalculator.hpp"
#include "ParticlePairDerivedHistos.hpp"
#include "ParticlePairBfHistos.hpp"
#include "PrintHelpers.hpp"
#include "NameHelpers.hpp"

ClassImp(CAP::ParticlePairBfCalculator);

namespace CAP
{

ParticlePairBfCalculator::ParticlePairBfCalculator()
:
CalculatorBF<ParticlePairDerivedHistos,ParticlePairBfHistos,2>()
{
  appendClassName("ParticlePairBfCalculator");
  setName("ParticlePairBfCalculator");
  setTitle("ParticlePairBfCalculator");
}

ParticlePairBfCalculator::ParticlePairBfCalculator(const ParticlePairBfCalculator & source)
:
CalculatorBF<ParticlePairDerivedHistos,ParticlePairBfHistos,2>(source)
{   }

ParticlePairBfCalculator & ParticlePairBfCalculator::operator=(const ParticlePairBfCalculator & rhs)
{
  if (this!=&rhs)
    {
    CalculatorBF<ParticlePairDerivedHistos,ParticlePairBfHistos,2>::operator=(rhs);
    }
  return *this;
}


void ParticlePairBfCalculator::setDefaultConfiguration()
{
  if (reportDebug(__FUNCTION__)) { printCR(); }
  CalculatorBF<ParticlePairDerivedHistos,ParticlePairBfHistos,2>::setDefaultConfiguration();
  addProperty("HistogramBaseName", "Pair");
  addProperty("load",true);
  addProperty("exportObject",true);
  addProperty("CreateHistograms",true);
  addProperty("DeltaP_nbins",   10);
  addProperty("DeltaP_min",   -4.0);
  addProperty("DeltaP_max",    4.0);
  addProperty("binCorrPP",     1.0);
}

//void ParticlePairBfCalculator::create(const Configuration & configuration)
//{
//  String bn = valueString( "HistogramBaseName");
//  HistogramGroup * histos;
//    printCR();
//    printValue("Creating BF HistogramGroup",bn);
//    printValue("NEventFilters",eventFilters.size());
//    printValue("NParticleFilters",particleFilters.size());
//    printCR();
//  int half = nParticleFilters()/2;
////  for (auto & eventFilter : eventFilters)
////    {
////    String efn = eventFilter->name();
////    for (int iParticleFilter1 = 0;iParticleFilter1<half;iParticleFilter1++)
////      {
////      ParticleFilter* particleFilter1 = particleFilters[iParticleFilter1];
////      String pfn1 = particleFilter1->name();
////      for (int iParticleFilter2 = 0; iParticleFilter2<half;iParticleFilter2++)
////        {
////        ParticleFilter* particleFilter2 = particleFilters[iParticleFilter2];
////        String pfn2 = particleFilter2->name();
////        histos = ParticlePairBfHistos[0];
////        histos->create(const Configuration & configuration);
////        }
////      }
////    }
//}



} // namespace CAP
