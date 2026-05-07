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
#include "ParticlePerformanceDerivedHistos.hpp"
#include "NameHelpers.hpp"
#include "PrintHelpers.hpp"
#include "RootHistogramHelpers.hpp"
#include "Task.hpp"
#include "NameHelpers.hpp"

ClassImp(CAP::ParticlePerformanceDerivedHistos);

namespace CAP
{

ParticlePerformanceDerivedHistos::ParticlePerformanceDerivedHistos()
:
ParticlePerformanceHistos()
{
  appendClassName("ParticlePerformanceDerivedHistos");
  setName("ParticlePerformanceDerivedHistos");
  setTitle("ParticlePerformanceDerivedHistos");
}

void ParticlePerformanceDerivedHistos::configure(const String& taskName,
                                                       const String & objectType,
                                                       const Configuration & configuration,
                                                       unsigned int index)
{
  if (reportDebug(__FUNCTION__)) { printCR(); }
  ParticlePerformanceHistos::configure(taskName,objectType,configuration,index);
}

// for now use the same boundaries for eta and y histogram
void ParticlePerformanceDerivedHistos::create()
{
  if (reportDebug(__FUNCTION__)) { printCR(); }
}

void ParticlePerformanceDerivedHistos::loadFrom(TFile & inputFile)
{
  if (reportDebug(__FUNCTION__)) { printCR(); }
  ParticlePerformanceHistos::loadFrom(inputFile);
}

void ParticlePerformanceDerivedHistos::calculateDerivedHistograms(const ParticlePerformanceHistos & base __attribute__((unused)))
{
  if (reportDebug(__FUNCTION__)) { printCR(); }
}


} // namespace CAP
