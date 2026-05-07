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
#ifndef CAP__ParticleDecayMode
#define CAP__ParticleDecayMode
#include <iostream>
#include <iomanip>
#include <vector>
#include "Aliases.hpp"
#include "ParticleType.hpp"
#include "ParticleDb.hpp"

using namespace std;

namespace CAP
{

  class ParticleType;
  class ParticleDb;

  class ParticleDecayMode
  {
  private:

  double _branchingRatio;
  vector<int> _childrenPdgCodes;
  vector<ParticleType*> _childrenTypes;

  public:
  ParticleDecayMode();
  ParticleDecayMode(const ParticleDecayMode & source);
  virtual ~ParticleDecayMode();
  ParticleDecayMode & operator=(const ParticleDecayMode & source);
  void resolveTypes(ParticleDb & collection);

  void   setBranchingRatio(double value) { _branchingRatio = value; }
  double branchingRatio() const { return _branchingRatio; }
  int    nChildren() const { return _childrenPdgCodes.size(); }
  void   addChild(int pdgCode) {  _childrenPdgCodes.push_back(pdgCode); }
  void   addChild(ParticleType* type);
  inline const vector<int> & childrenPdgCodes() const {  return _childrenPdgCodes; }
  inline vector<int> & childrenPdgCodes() {  return _childrenPdgCodes; }
  const vector<ParticleType*> & childrenTypes() const {   return _childrenTypes; }
  vector<ParticleType*> & childrenTypes() {   return _childrenTypes; }
  const ParticleType & childType(unsigned int index) const;
  ParticleType & childType(unsigned int index);
  int childPdgCode(unsigned int index) const ;
  int childPdgCode(unsigned int index);
  void print();

  ClassDef(ParticleDecayMode,0)
  };

} // namespace CAP

#endif  // CAP__ParticleDecayMode

