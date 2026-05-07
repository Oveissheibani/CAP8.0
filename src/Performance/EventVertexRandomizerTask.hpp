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
#ifndef CAP__EventVertexRandomizerTask
#define CAP__EventVertexRandomizerTask
#include "EventProcessor.hpp"

namespace CAP
{

class EventVertexRandomizerTask
:
public EventProcessor
{
public:
  
  EventVertexRandomizerTask();
  virtual ~EventVertexRandomizerTask() {}
  virtual void setDefaultConfiguration();
  virtual void initialize();
  virtual void execute();
  
protected:
  
  double rConversion;
  double tConversion;
  double xAvg; //!< mean vertex position along the x-axis
  double yAvg; //!< mean vertex position along the y-axis
  double zAvg; //!< mean vertex position along the z-axis
  double tAvg; //!< mean vertex position along the t-axis
  double xRms; //!< rms of the vertex distribution along the x-axis
  double yRms; //!< rms of the vertex distribution along the y-axis
  double zRms; //!< rms of the vertex distribution along the z-axis
  double tRms; //!< rms of the vertex distribution along the t-axis
  
  ClassDef(EventVertexRandomizerTask,0)
};

} // namespace CAP

#endif /* CAP__EventVertexRandomizerTask */
