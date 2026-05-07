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
#include "ParticleFlowCalculator.hpp"
#include "MathConstants.hpp"
#include "NameHelpers.hpp"
#include "PrintHelpers.hpp"

ClassImp(CAP::ParticleFlowCalculator);

namespace CAP
{

ParticleFlowCalculator::ParticleFlowCalculator()
:
Calculator<ParticleFlowHistos,ParticleFlowDerivedHistos,1>()
{
  appendClassName("ParticleFlowCalculator");
  setName("ParticleFlowCalculator");
  setTitle("ParticleFlowCalculator");
}

ParticleFlowCalculator::ParticleFlowCalculator(const ParticleFlowCalculator & source)
:
Calculator<ParticleFlowHistos,ParticleFlowDerivedHistos,1>(source)
{   }

ParticleFlowCalculator & ParticleFlowCalculator::operator=(const ParticleFlowCalculator & rhs)
{
  if (this!=&rhs)
    {
    Calculator<ParticleFlowHistos,ParticleFlowDerivedHistos,1>::operator=(rhs);
    }
  return *this;
}

void ParticleFlowCalculator::setDefaultConfiguration()
{
  Calculator<ParticleFlowHistos,ParticleFlowDerivedHistos,1>::setDefaultConfiguration();
  addProperty( "HistogramBaseName","Flow");
  addProperty( "load",true);
  addProperty( "exportObject",true);
  addProperty( "CreateHistograms",true);

  addProperty( "n1_nbins",  100);
  addProperty( "n1_min",    0.0);
  addProperty( "n1_max",  100.0);

  addProperty( "etot_nbins",  100);
  addProperty( "etot_min",    0.0);
  addProperty( "etot_max",  100.0);

  addProperty( "pt_nbins",  100);
  addProperty( "pt_min",    0.0);
  addProperty( "pt_max",  100.0);

  addProperty( "eta_nbins",  20);
  addProperty( "eta_min",   -1.0);
  addProperty( "eta_max",    1.0);

  addProperty( "rapidity_nbins",  20);
  addProperty( "rapidity_min",   -1.0);
  addProperty( "rapidity_max",    1.0);

  addProperty( "phi_nbins",  36);
  addProperty( "phi_min",    0.0);
  addProperty( "phi_max",    CAP::MATH::twoPi());

  addProperty( "phiEta_nbins",    720);
  addProperty( "phiEtaPt_nbins",  7200);
  addProperty( "phiY_nbins",      720);
  addProperty( "phiYPt_nbins",    7200);
}

} // namespace CAP
