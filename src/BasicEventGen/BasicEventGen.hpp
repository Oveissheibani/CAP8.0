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
#ifndef CAP__BasicEventGen
#define CAP__BasicEventGen
#include "EventProcessor.hpp"
#include "ParticleDb.hpp"

namespace CAP
{

class BasicEventGen
:
public EventProcessor
{
public:

  BasicEventGen();
  BasicEventGen(const BasicEventGen & source);
  virtual ~BasicEventGen() {}
  BasicEventGen & operator=(const BasicEventGen & rhs);
  virtual void setDefaultConfiguration();
  virtual void initialize();
  virtual void execute();

protected:

  int    pdgSelected;
  double multMin;
  double multMax;
  double rapidityMin;
  double rapidityMax;
  double ptSlope;
  bool   setSeed;
  int    seedValue;
  std::vector<double> mass;
  std::vector<double> massSq;
  std::vector<ParticleType*> types;

  ClassDef(BasicEventGen,0)
};

}

#endif /* CAP__BasicEventGen */
