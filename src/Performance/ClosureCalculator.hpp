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
#ifndef CAP__ClosureCalculator
#define CAP__ClosureCalculator
#include "Task.hpp"

namespace CAP
{

class ClosureCalculator
:
public Task
{
  
public:
  
  ClosureCalculator();
  virtual ~ClosureCalculator() {}
  virtual void setDefaultConfiguration();
  virtual void execute();

  ClassDef(ClosureCalculator,0)
};

} // namespace CAP

#endif /* CAP__ClosureCalculator */
