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
#ifndef CAP__EventPlaneRandomizerTask
#define CAP__EventPlaneRandomizerTask
#include "EventProcessor.hpp"

namespace CAP
{

class EventPlaneRandomizerTask
:
public EventProcessor
{
public:
  EventPlaneRandomizerTask();
  virtual ~EventPlaneRandomizerTask() {}
  virtual void execute();
  ClassDef(EventPlaneRandomizerTask,0)
};

} // namespace CAP

#endif /* CAP__EventPlaneRandomizerTask */
