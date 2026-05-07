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
#ifndef CAP__ParticleDigit
#define CAP__ParticleDigit
#include "Aliases.hpp"
#include "Factory.hpp"

using namespace std;
namespace CAP
{
  //!
  //!Class ParticleDigit
  //!
  class ParticleDigit
  {
  public:

  ParticleDigit();
  ParticleDigit(unsigned int iY,
                unsigned int iEta,
                unsigned int iPhi,
                unsigned int iPt,
                float       pt,
                float       phi,
                float       eta,
                float       y,
                float       e);
  virtual ~ParticleDigit(){}
  ParticleDigit(const ParticleDigit& other);
  ParticleDigit & operator=(const ParticleDigit & other);
  void print(ostream & output);
  void reset();

  unsigned int _iY;
  unsigned int _iEta;
  unsigned int _iPhi;
  unsigned int _iPt;
  float _pt;
  float _phi;
  float _eta;
  float _y;
  float _e;

  static int _factorySize;
  static Factory<ParticleDigit> * _factory;
  static Factory<ParticleDigit> * factory();


  ClassDef(ParticleDigit,0)
  };

}

#endif /* CAP__ParticleDigit */
