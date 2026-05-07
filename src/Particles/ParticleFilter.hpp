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
#ifndef CAP__ParticleFilter
#define CAP__ParticleFilter
#include "Particle.hpp"
#include "Filter.hpp"
#include "ManagedObject.hpp"
#include "Configuration.hpp"

namespace CAP
{

  class ParticleFilter
  :
  public Filter<Particle>,
  public ManagedObject
  {
  public:
  enum ParticleFilterType   { kNone, kPt, kPx, kPy, kPz, kEnergy, kEta, kRapidity, kPhi, kPdg,  kCharge, kStrange, kBaryon, kCharm, kBeauty, kLive, kEnabled, kEfficiency};

  ParticleFilter();
  ParticleFilter(const ParticleFilter & source);
  ParticleFilter & operator=(const ParticleFilter & rhs);
  virtual ~ParticleFilter() {}
  virtual bool accept(const Particle & particle) const;
  virtual void configure(const String & ownerName,
                         const String & typeName,
                         const Configuration & configuration,
                         unsigned int index);

  ClassDef(ParticleFilter,0)
  };

}

#endif /* CAP__ParticleFilter */

