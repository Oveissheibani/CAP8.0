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
#ifndef CAP__SubSampleStatCalculator
#define CAP__SubSampleStatCalculator
#include "Task.hpp"

using namespace std;

namespace CAP
{

class SubSampleStatCalculator
:
public Task
{
public:
  
  SubSampleStatCalculator();
  SubSampleStatCalculator(const SubSampleStatCalculator & src);
  SubSampleStatCalculator & operator=(const SubSampleStatCalculator & rhs);
  virtual ~SubSampleStatCalculator() {}
  virtual void setDefaultConfiguration();
  virtual void execute();

protected:

  Accountant _totalTaskExecuted;
  Accountant _eventAccepted;
  Accountant _totalEventAccepted;
  String _importPath;
  String _importFile;
  String _exportPath;
  String _exportFile;

  ClassDef(SubSampleStatCalculator,0)
};

} // namespace CAP

#endif /* CAP__SubSampleStatCalculator */
