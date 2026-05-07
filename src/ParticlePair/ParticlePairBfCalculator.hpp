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
#ifndef CAP__ParticlePairBfCalculator
#define CAP__ParticlePairBfCalculator
#include "Calculator.hpp"
#include "ParticlePairDerivedHistos.hpp"
#include "ParticlePairBfHistos.hpp"

namespace CAP
{
class ParticlePairBfCalculator
:
public CalculatorBF<ParticlePairDerivedHistos,ParticlePairBfHistos,2>
{
public:

  ParticlePairBfCalculator();
  ParticlePairBfCalculator(const ParticlePairBfCalculator & task);
  virtual ~ParticlePairBfCalculator() {}
  ParticlePairBfCalculator & operator=(const ParticlePairBfCalculator & task);
  virtual void setDefaultConfiguration();
  virtual void execute();

  ClassDef(ParticlePairBfCalculator,0)
};

} // namespace CAP

#endif /* CAP__ParticlePairCalculator */
