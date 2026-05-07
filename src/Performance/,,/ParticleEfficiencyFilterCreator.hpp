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
#ifndef CAP__ParticleEfficiencyFilterCreator
#define CAP__ParticleEfficiencyFilterCreator
#include "EventProcessor.hpp"
#include "Event.hpp"
#include "EventFilter.hpp"
#include "EventEfficiency.hpp"
#include "EventSmearer.hpp"
#include "Particle.hpp"
#include "ParticleFilter.hpp"
#include "ParticleEfficiency.hpp"
#include "ParticleSmearer.hpp"
#include "BaseHistos.hpp"
#include "BaseDerivedHistos.hpp"

namespace CAP
{

class ParticleEfficiencyFilterCreator
:
public EventProcessor<Event,EventFilter,EventEfficiency,EventSmearer,Particle,ParticleFilter,ParticleEfficiency,
ParticleStearer,BaseHistos,BaseDerivedHistos>
{
public:
  
  ParticleEfficiencyFilterCreator();
  ParticleEfficiencyFilterCreator(const ParticleEfficiencyFilterCreator & source);
  ParticleEfficiencyFilterCreator & operator=(const ParticleEfficiencyFilterCreator & rhs);
  virtual ~ParticleEfficiencyFilterCreator() {}

  virtual void initialize();

  virtual ParticleEfficiencyFilter * createFilter(const String & name,
                                                  const String & title,
                                                  const vector<String>  & conditionTypes,
                                                  const vector<String>  & conditionSubtypes,
                                                  const vector<double>  & conditionMinima,
                                                  const vector<double>  & conditionMaxima,
                                                  const vector<double>  & conditionMinima2,
                                                  const vector<double>  & conditionMaxima2,
                                                  bool  verbose=true);

  ClassDef(ParticleEfficiencyFilterCreator,0)
};

}

#endif /* CAP_FilterCreator */


