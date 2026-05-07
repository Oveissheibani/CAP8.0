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
#ifndef CAP__BaseDerivedHistos
#define CAP__BaseDerivedHistos
#include "BaseHistos.hpp"

namespace CAP
{

class BaseDerivedHistos : public BaseHistos
{
public:
  BaseDerivedHistos();
  BaseDerivedHistos(const BaseDerivedHistos & group);
  BaseDerivedHistos & operator=(const BaseDerivedHistos & group);
  virtual ~BaseDerivedHistos(){ }
  virtual void calculateDerivedHistograms(const BaseHistos & base  __attribute__((unused))) {}

protected:

  virtual void cloneD(const BaseDerivedHistos & group  __attribute__((unused))) {}

  ClassDef(BaseDerivedHistos,0)
};

} // namespace CAP

#endif /* CAP__BaseDerivedHistos  */
