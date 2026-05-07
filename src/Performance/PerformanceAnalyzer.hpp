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
#ifndef CAP__PerformanceAnalyzer
#define CAP__PerformanceAnalyzer
#include "EventProcessorSingle.hpp"
#include "ParticlePerformanceHistos.hpp"
#include "ParticlePerformanceDerivedHistos.hpp"

namespace CAP
{

class PerformanceAnalyzer
:
public EventProcessorSingle<ParticlePerformanceHistos,ParticlePerformanceDerivedHistos>
{
public:
  
  PerformanceAnalyzer();
  PerformanceAnalyzer(const PerformanceAnalyzer & analyzer);
  PerformanceAnalyzer & operator=(const PerformanceAnalyzer & analyzer);
  virtual ~PerformanceAnalyzer() {}
  virtual void setDefaultConfiguration();
  virtual void execute();

protected:

  ClassDef(PerformanceAnalyzer,0)
};

} // namespace CAP

#endif /* CAP__PerformanceAnalyzer */
