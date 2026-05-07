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
#include "ParticleDecayerTask.hpp"
#include "PrintHelpers.hpp"
#include "NameHelpers.hpp"
#include "VectorLorentz.hpp"
#include "ParticleDbExceptions.hpp"

ClassImp(CAP::ParticleDecayerTask);

namespace CAP
{

ParticleDecayerTask::ParticleDecayerTask()
:
EventProcessor(),
ParticleDecayer()
{
  appendClassName("ParticleDecayerTask");
  setMinimumReportLevel(Object::kInfo);
  setName("ParticleDecayerTask");
  setTitle("ParticleDecayerTask");
 }

void ParticleDecayerTask::decayParticle(Particle & parent, Event & event)
{
  ParticleType & parentType      = parent.type();
  VectorLorentz & parentMomentum  = parent.momentum();
  VectorLorentz & parentPosition  = parent.position();
  ParticleDecayMode & decayMode   = parentType.generateDecayMode();
  int nChildren = decayMode.nChildren();
  switch (nChildren)
    {
      default:
      case 1:
      throw ParticleDecayException(nChildren,"Invalid number of children",__FUNCTION__);
      break;

      case 2:
      {
      Particle & child1 = Particle::factory().nextObject();
      Particle & child2 = Particle::factory().nextObject();
      ParticleType & childType1 = decayMode.childType(0); child1.setType(&childType1); child1.setLive(true);
      ParticleType & childType2 = decayMode.childType(1); child2.setType(&childType2); child2.setLive(true);
      VectorLorentz & p1 = child1.momentum();
      VectorLorentz & r1 = child1.position();
      VectorLorentz & p2 = child2.momentum();
      VectorLorentz & r2 = child2.position();
      decay2(parentType,
             parentMomentum,
             parentPosition,
             childType1,p1,r1,
             childType2,p2,r2);
      event.addParticle(&child1); child1.setLive(true);
      event.addParticle(&child2); child2.setLive(true);
      
      if (!child1.isStable() ) decayParticle(child1,event);
      if (!child2.isStable() ) decayParticle(child2,event);
      parent.setDecayed(true);
      break;
      }

      case 3:
      {
      Particle & child1 = Particle::factory().nextObject();
      Particle & child2 = Particle::factory().nextObject();
      Particle & child3 = Particle::factory().nextObject();
      ParticleType & childType1 = decayMode.childType(0); child1.setType(&childType1); child1.setLive(true);
      ParticleType & childType2 = decayMode.childType(1); child2.setType(&childType2); child2.setLive(true);
      ParticleType & childType3 = decayMode.childType(2); child3.setType(&childType3); child3.setLive(true);
      VectorLorentz & p1 = child1.momentum();
      VectorLorentz & r1 = child1.position();
      VectorLorentz & p2 = child2.momentum();
      VectorLorentz & r2 = child2.position();
      VectorLorentz & p3 = child3.momentum();
      VectorLorentz & r3 = child3.position();
      decay3(parentType,
             parentMomentum,
             parentPosition,
             childType1,p1,r1,
             childType2,p2,r2,
             childType3,p3,r3);
      event.addParticle(&child1); child1.setLive(true);
      event.addParticle(&child2); child2.setLive(true);
      event.addParticle(&child3); child3.setLive(true);
      if (!child1.isStable() ) decayParticle(child1,event);
      if (!child2.isStable() ) decayParticle(child2,event);
      if (!child3.isStable() ) decayParticle(child3,event);
      parent.setDecayed(true);
      break;
      }
      case 4: // skip those for now...
      {
      //        Particle * child1 = Particle::factory().nextObject();
      //        Particle * child2 = Particle::factory().nextObject();
      //        Particle * child3 = Particle::factory().nextObject();
      //        Particle * child4 = Particle::factory().nextObject();
      //        ParticleType  & childType1 = decayMode.childType(0); child1.setType(&childType1); child1.setLive(true);
      //        ParticleType  & childType2 = decayMode.childType(1); child2.setType(&childType2); child2.setLive(true);
      //        ParticleType  & childType3 = decayMode.childType(2); child3.setType(&childType3); child3.setLive(true);
      //        ParticleType  & childType4 = decayMode.childType(3); child4->setType(&childType4); child4->setLive(true);
      //        VectorLorentz & p1 = child1.momentum();
      //        VectorLorentz & r1 = child1.position();
      //        VectorLorentz & p2 = child2.momentum();
      //        VectorLorentz & r2 = child2.position();
      //        VectorLorentz & p3 = child3.momentum();
      //        VectorLorentz & r3 = child3.position();
      //        VectorLorentz & p4 = child4->momentum();
      //        VectorLorentz & r4 = child4->position();
      //        decay4(parentType,
      //                       parentMomentum,
      //                       parentPosition,
      //                       childType1,p1,r1,
      //                       childType2,p2,r2,
      //                       childType3,p3,r3,
      //                       childType4,p4,r4);
      //        events[0]->add(child1); child1.setLive(true);
      //        events[0]->add(child2); child2.setLive(true);
      //        events[0]->add(child3); child3.setLive(true);
      //        events[0]->add(child4); child4->setLive(true);
      //      if (!child1.isStable() ) decayParticle(child1,event);
      //      if (!child2.isStable() ) decayParticle(child2,event);
      //      if (!child3.isStable() ) decayParticle(child3,event);
      //      if (!child4->isStable() ) decayParticle(child4,event);
      //      parent.setDecayed(true);
      //
      break;
      }
    }
}


// ====================================================================
// Decay unstable particles in the event
// Decayed particles are retained in the event but their live
// flag is set to false.
// Children particles are added at the tail end of the event
// with a live flag  set to true.
// ====================================================================
void ParticleDecayerTask::execute()
{
  Event & ev = event();
  for (auto & particle : ev.particles())
    {
    if (particle->isLive() &&
        particle->isDecayEnabled()) decayParticle(*particle,ev);
    }
}

} // namespace CAP
