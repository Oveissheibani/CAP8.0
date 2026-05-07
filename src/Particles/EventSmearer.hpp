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
#ifndef CAP__EventSmearer
#define CAP__EventSmearer
#include "HistogramGroup.hpp"
#include "Event.hpp"
#include "MathConstants.hpp"

namespace CAP
{

class EventSmearer : public HistogramGroup
{
public:
  EventSmearer();
  EventSmearer(const EventSmearer & group);
  EventSmearer & operator=(const EventSmearer & group);
  virtual ~EventSmearer(){ }
  virtual void configure(const String & ownerName,
                               const String & typeName,
                               const Configuration & configuration,
                               unsigned int index);
  virtual void smear(const Event & source, Event & target);

protected:

  virtual void cloneB(const EventSmearer & source);

  ClassDef(EventSmearer,0)

};

} // namespace CAP

#endif /* CAP__EventSmearer  */



