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
#ifndef CAP__EventIterator
#define CAP__EventIterator
#include "EventProcessor.hpp"

namespace CAP
{

//!
//! This class implements a task iterator task, i.e., a task that repeated calls other tasks (subtasks) to carry out the same operation(s) on a sequence of events (event stream).
//!  Use the 'run' method to execute the job on all selected events. Internally, 'run' calls the 'execute' method repeatedly on each event in the event stream.
//!  Do not call the 'execute' method of this class directly unless you know what you are doing..
//!
//!  Optionally, the iterator may call a subsampleAnalysis method to carry out a sub-sample analysis of all the sub tasks operated by this iterator.
//!
class EventIterator
:
public EventProcessor
{
public:
  
  EventIterator();
  EventIterator(const EventIterator & task);
  EventIterator operator=(const EventIterator & task);
  virtual ~EventIterator() {}
  virtual void setDefaultConfiguration();
  virtual void configure(const Configuration & configuration);
  virtual void execute();
  long nEventsRequested() const { return _nEventsRequested; }
  long nEventsReport() const { return _nEventsReport; }

protected:

  long    _nEventsRequested;
  long    _nEventsReport;

  ClassDef(EventIterator,0)
};

}
#endif /* CAP__EventIterator */
