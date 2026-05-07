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
#ifndef CAP__JetFilter
#define CAP__JetFilter
#include "Filter.hpp"
#include "fastjet/PseudoJet.hh"
#include "Particle.hpp"

namespace CAP
{

class JetFilter : public Filter<fastjet::PseudoJet>
{
public:

  JetFilter();
  JetFilter(const JetFilter & otherFilter);
  JetFilter & operator=(const JetFilter & otherFilter);
  virtual ~JetFilter() {}
  virtual bool accept(const fastjet::PseudoJet & pseudoJet);

  enum JetFilterType   { kNone, kP, kPt, kEta, kMult };

  ClassDef(JetFilter,0)
};

}

#endif /* CAP__JetFilter */

