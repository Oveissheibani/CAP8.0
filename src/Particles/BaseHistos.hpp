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
#ifndef CAP__BaseHistos
#define CAP__BaseHistos
#include "HistogramGroup.hpp"
#include "Particle.hpp"
#include "MathConstants.hpp"

namespace CAP
{

class BaseHistos : public HistogramGroup
{
public:
  BaseHistos();
  BaseHistos(const BaseHistos & group);
  BaseHistos & operator=(const BaseHistos & rhs);
  virtual ~BaseHistos(){ }
  virtual void cloneB(const BaseHistos & source);
  ClassDef(BaseHistos,0)
};

} // namespace CAP

#endif /* CAP__BaseDerivedHistos  */



