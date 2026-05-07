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
#include "ParticleFlowHistos.hpp"
#include "PrintHelpers.hpp"
#include "Task.hpp"
#include "Configuration.hpp"
#include "NameHelpers.hpp"
#include "RootHistogramHelpers.hpp"

ClassImp(CAP::ParticleFlowHistos);

namespace CAP
{

ParticleFlowHistos::ParticleFlowHistos()
:
HistogramGroup()
h_n1(nullptr)
{
  appendClassName("ParticleFlowHistos");
  setName("ParticleFlowHistos");
  setTitle("ParticleFlowHistos");
}

ParticleFlowHistos::ParticleFlowHistos(const ParticleFlowHistos & source)
:
HistogramGroup(source),
h_n1(nullptr)
{
  cloneB(source);
}

ParticleFlowHistos & ParticleFlowHistos::operator=(const ParticleFlowHistos & rhs)
{
  if (this!=& rhs)
    {
    HistogramGroup::operator=( rhs);
    cloneB( rhs);
    }
  return *this;
}

void ParticleFlowHistos::cloneB(const ParticleFlowHistos & group)
{
  if (reportStart(__FUNCTION__)) { /* noops*/ };

  // ony clone those that exist...
  if (group.h_n1) h_n1   = (TH1*) group.h_n1->Clone();
}

void ParticleFlowHistos::configure(const String & taskName,
                                         const String & objectType,
                                         const Configuration & configuration,
                                         unsigned int index)
{
  HistogramGroup::configure(taskName,objectType,configuration,index);
}

// for now use the same boundaries for eta and y histogram
void ParticleFlowHistos::create()
{
  if (reportStart(__FUNCTION__)) { /* noops*/ };
}

void ParticleFlowHistos::loadFrom(TFile & inputFile)
{
  if (reportStart(__FUNCTION__)) { /* noops*/ };
}


//!
//! Fiil  Flow particle histograms of this class with the particles contained in the given list.
//!
void ParticleFlowHistos::fill(Particle & particle, double weight)
{
  VectorLorentz & momentum = particle.momentum();
  float pt   = momentum.perp();
  float eta  = momentum.pseudorapidity();
  float phi  = momentum.phi();
  float rapidity = momentum.rapidity();
  if (phi<0) phi += CAP::MATH::twoPi();
}

} // namespace CAP
