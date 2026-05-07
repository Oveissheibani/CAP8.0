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
#ifndef CAP__MeasurementSimulator
#define CAP__MeasurementSimulator
#include "EventProcessor.hpp"

namespace CAP
{
class MeasurementSimulator
:
public EventProcessor
{
public:
  
  MeasurementSimulator();
  MeasurementSimulator(const MeasurementSimulator & source);
  MeasurementSimulator & operator=(const MeasurementSimulator & rhs);
  virtual ~MeasurementSimulator() {}
  virtual void setDefaultConfiguration();
  virtual void execute();

  ClassDef(MeasurementSimulator,0)
};

} // namespace CAP


#endif /* CAP__MeasurementSimulator */
