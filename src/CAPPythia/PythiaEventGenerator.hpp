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
#ifndef CAP__PythiaEventGenerator
#define CAP__PythiaEventGenerator
#include "Pythia.h"
#include "EventProcessor.hpp"
#include "Event.hpp"
#include "EventFilter.hpp"
#include "EventEfficiency.hpp"
#include "EventSmearer.hpp"
#include "Particle.hpp"
#include "ParticleFilter.hpp"
#include "ParticleEfficiency.hpp"
#include "ParticleSmearer.hpp"

#include <set>     // status-code keep set

namespace CAP
{

class PythiaEventGenerator
:
public EventProcessor
{
public:

  PythiaEventGenerator();
  PythiaEventGenerator(const PythiaEventGenerator & task);
  virtual ~PythiaEventGenerator() {}
  PythiaEventGenerator & operator=(const PythiaEventGenerator & task);
  virtual void setDefaultConfiguration();
  virtual void initialize();
  virtual void execute();
  virtual void finalize();

protected:

  Pythia8::Pythia * pythia;
  bool saveFinalOnly;
  bool savePhotons;
  bool saveNeutrinos;
  bool saveQuarks;
  bool saveGaugeBosons;
  // Status-code whitelist (HepMC convention from statusHepMC()).
  // Empty + saveFinalOnly=true → {1}; empty + saveFinalOnly=false → all.
  // Non-empty: explicit list.  Populated by initialize() from the
  // KeepStatuses config key.  Enables BF-per-stage analyses on Pythia.
  std::set<int> _keepStatuses;
  bool _keepAllStatuses = false;

  ClassDef(PythiaEventGenerator,0)
};

}

#endif /* CAP__PythiaEventGenerator */
