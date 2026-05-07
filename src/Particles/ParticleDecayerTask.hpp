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
#ifndef CAP__ParticleDecayerTask
#define CAP__ParticleDecayerTask
#include "EventProcessor.hpp"
#include "Event.hpp"
#include "EventFilter.hpp"
#include "EventEfficiency.hpp"
#include "EventSmearer.hpp"
#include "Particle.hpp"
#include "ParticleFilter.hpp"
#include "ParticleEfficiency.hpp"
#include "ParticleSmearer.hpp"
#include "ParticleDecayer.hpp"

namespace CAP
{

//!
//! Task handles the decay of all decayable particles in the incoming event stream. At execution time, i.e., for each event encountered,
//! each particle of the input stream is examined to determine whether it should be decayed. Particle deemed decayable are passed to the
//! decay handler (decayer) and decayed.  Currently, the decayer handles two and three particle decays. Decayed particles are kept in the
//! event stream but tagged as decayed. Child particles are inserted in the stream and tagged as "live" (undecayed).  Child particles that
//! are themselves decayable are decayed iteratively. 
//!
class ParticleDecayerTask
:
public EventProcessor,
public ParticleDecayer
{
public:

  ParticleDecayerTask();
  virtual ~ParticleDecayerTask() {}
  virtual void execute();
  virtual void decayParticle(Particle & particle, Event & event);

  ClassDef(ParticleDecayerTask,0)
};

} // namespace CAP

#endif /* CAP__ParticleDecayerTask */
