/* **********************************************************************
 * Copyright (C) 2019-2024, Claude Pruneau, Chun Shen
 * All rights reserved.
 *
 * Based on the ROOT package and environment
 * This code is adapted from Chun Shen's code
 * for the purpose of computing hadronic resonance
 * correlations..
 *
 * For the licensing terms see LICENSE.
 *
 * Author: Claude Pruneau,   04/01/2024
 *
 * *********************************************************************/
#include "ParticleDecayMode.hpp"
#include "ParticleDbExceptions.hpp"
#include "PrintHelpers.hpp"

//#include "SelectionGenerator.hpp"

ClassImp(CAP::ParticleDecayMode);

namespace CAP
{

ParticleDecayMode::ParticleDecayMode()
:
_branchingRatio(1.0),
_childrenPdgCodes(),
_childrenTypes()
{
  _childrenPdgCodes.clear();
  _childrenTypes.clear();
}

ParticleDecayMode::ParticleDecayMode(const ParticleDecayMode & source)
:
_branchingRatio(source._branchingRatio),
_childrenPdgCodes(source._childrenPdgCodes),
_childrenTypes(source._childrenTypes)
{  }

ParticleDecayMode::~ParticleDecayMode()
{
  _childrenPdgCodes.clear();
  _childrenTypes.clear();
}

ParticleDecayMode & ParticleDecayMode::operator=(const ParticleDecayMode & source)
{
  if (this!=&source)
    {
    _branchingRatio    = source._branchingRatio;
    _childrenPdgCodes  = source._childrenPdgCodes;
    _childrenTypes     = source._childrenTypes;
    }
  return *this;
}

// Assumes the mode was defined based on pdg codes
// build the table for types
void ParticleDecayMode::resolveTypes(ParticleDb & db)
{
  for (auto & code : _childrenPdgCodes)
    {
    ParticleType * type = db.findPdgCode(code);
    if (type)
      _childrenTypes.push_back(type);
    else
      throw ParticleDecayException("Unknown Particle Type",__FUNCTION__);
    }
}

const ParticleType & ParticleDecayMode::childType(unsigned int index) const
{
  if (index>=_childrenTypes.size())
    throw ParticleDecayException("Index out of bound",__FUNCTION__);
  return * _childrenTypes[index];
}

ParticleType & ParticleDecayMode::childType(unsigned int index)
{
  if (index>=_childrenTypes.size())
    throw ParticleDecayException("Index out of bound",__FUNCTION__);
  return * _childrenTypes[index];
}

  void   ParticleDecayMode::addChild(ParticleType* type)
  {
  _childrenPdgCodes.push_back(type->pdgCode());
  _childrenTypes.push_back(type);
  }


int ParticleDecayMode::childPdgCode(unsigned int index) const
{
  if (index<_childrenPdgCodes.size())
    return _childrenPdgCodes[index];
  else
    return -1;
}

int ParticleDecayMode::childPdgCode(unsigned int index)
{
  if (index<_childrenPdgCodes.size())
    return _childrenPdgCodes[index];
  else
    return -1;
}

void ParticleDecayMode::print()
{
  printValue("Br",_branchingRatio);
  for (unsigned int k=0; k<_childrenPdgCodes.size(); k++)
    printValue(_childrenTypes[k]->name(),_childrenPdgCodes[k]);
  printCR();
}

} // namespace CAP
