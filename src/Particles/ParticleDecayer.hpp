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
#ifndef CAP__ParticleDecayer
#define CAP__ParticleDecayer
#include <iostream>
#include <iomanip>
#include "TRandom.h"
#include "Particle.hpp"
#include "VectorLorentz.hpp"

using namespace std;

namespace CAP
{

class ParticleDecayer
{
public:
  ParticleDecayer();
  virtual ~ParticleDecayer() {}
  void setRandomGenerator(TRandom * _random) { random = _random; }
  TRandom * getRandomGenerator() { return random; }

  void decay2(ParticleType  & parentType,
              VectorLorentz & parentMomentum,
              VectorLorentz & parentPosition,
              ParticleType  & childType1,
              VectorLorentz & p1,
              VectorLorentz & r1,
              ParticleType  & childType2,
              VectorLorentz & p2,
              VectorLorentz & r2);

  void decay3(ParticleType  & parentType,
              VectorLorentz & parentMomentum,
              VectorLorentz & parentPosition,
              ParticleType  & childType1,
              VectorLorentz & p1,
              VectorLorentz & r1,
              ParticleType  & childType2,
              VectorLorentz & p2,
              VectorLorentz & r2,
              ParticleType  & childType3,
              VectorLorentz & p3,
              VectorLorentz & r3);

  void decay4(ParticleType  & parentType,
              VectorLorentz & parentMomentum,
              VectorLorentz & parentPosition,
              ParticleType  & childType1,
              VectorLorentz & p1,
              VectorLorentz & r1,
              ParticleType  & childType2,
              VectorLorentz & p2,
              VectorLorentz & r2,
              ParticleType  & childType3,
              VectorLorentz & p3,
              VectorLorentz & r3,
              ParticleType  & childType4,
              VectorLorentz & p4,
              VectorLorentz & r4
              );

  void  decay2(Particle & parent,
               Particle & child1,
               Particle & child2);
  void  decay3(Particle & parent,
               Particle & child1,
               Particle & child2,
               Particle & child3);
  void  decay4(Particle & parent,
               Particle & child1,
               Particle & child2,
               Particle & child3,
               Particle & child4);

protected:

  static double   twoPi;

  TRandom * random;



  ClassDef(ParticleDecayer,0)
};

}

#endif  // ParticleDecayer

