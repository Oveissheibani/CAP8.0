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
#ifndef CAP__EventMerger
#define CAP__EventMerger
#include "EventProcessor.hpp"

namespace CAP
{

//!
//! Task to merge events from different input streams into one output stream
//!
class EventMerger
:
public EventProcessor
{
public:
  
  EventMerger();
  EventMerger(const EventMerger & task);
  EventMerger operator=(const EventMerger & task);
  virtual ~EventMerger() {}
  virtual void setDefaultConfiguration();
  virtual void configure(const Configuration & configuration);
  virtual void execute();

  const std::vector<unsigned int> & nEventsPerStream() { return _nEventsPerStream; }

protected:

  std::vector<unsigned int> _nEventsPerStream;

  ClassDef(EventMerger,0)
};

}
#endif /* CAP__EventMerger */
