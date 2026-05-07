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
#ifndef CAP__ParticleFlowCalculator
#define CAP__ParticleFlowCalculator
#include "Calculator.hpp"
#include "ParticleFlowHistos.hpp"
#include "ParticleFlowDerivedHistos.hpp"

namespace CAP
{
class ParticleFlowCalculator
:
public Calculator<ParticleFlowHistos,ParticleFlowDerivedHistos,1>
{
public:
  
  ParticleFlowCalculator();
  ParticleFlowCalculator(const ParticleFlowCalculator & calculator);
  ParticleFlowCalculator & operator=(const ParticleFlowCalculator & calculator);
  virtual ~ParticleFlowCalculator() {}
  virtual void setDefaultConfiguration();

  ClassDef(ParticleFlowCalculator,0)
};

} // namespace CAP

#endif /* CAP__ParticleFlowCalculator */
