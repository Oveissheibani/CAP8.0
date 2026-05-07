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
#include "ParticlePairFilter.hpp"
#include "Aliases.hpp"
#include "PrintHelpers.hpp"

ClassImp(CAP::ParticlePairFilter);

namespace CAP
{

ParticlePairFilter::ParticlePairFilter()
:
Filter<Particle>()
{
  // no ops
}


bool ParticlePairFilter::accept(const Particle & particle1, const Particle & particle2)
{
//  ParticleType & type1 = particle1.type();
//  ParticleType & type2 = particle2.type();
  if (particle1.isDecayed() )          return false;
  if (particle2.isDecayed() )          return false;
  return true;
}
} // namespace CAP
