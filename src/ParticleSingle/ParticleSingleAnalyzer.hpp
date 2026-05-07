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
#ifndef CAP__ParticleSingleAnalyzer
#define CAP__ParticleSingleAnalyzer
#include "EventProcessorSingle.hpp"
#include "ParticleSingleHistos.hpp"
#include "ParticleSingleDerivedHistos.hpp"

namespace CAP
{

class ParticleSingleAnalyzer
:
public EventProcessorSingle<ParticleSingleHistos,ParticleSingleDerivedHistos>
{
public:
  
  ParticleSingleAnalyzer();
  ParticleSingleAnalyzer(const ParticleSingleAnalyzer & analyzer);
  ParticleSingleAnalyzer & operator=(const ParticleSingleAnalyzer & analyzer);
  virtual ~ParticleSingleAnalyzer() {}
  virtual void setDefaultConfiguration();
  virtual void execute();

protected:

  ClassDef(ParticleSingleAnalyzer,0)
};

} // namespace CAP

#endif /* CAP__ParticleSingleAnalyzer */
