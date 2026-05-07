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
#ifndef CAP__EventFilter
#define CAP__EventFilter
#include "Event.hpp"
#include "Filter.hpp"
#include "ManagedObject.hpp"

namespace CAP
{
  class EventFilter
  :
  public Filter<Event>,
  public ManagedObject
  {
  public:
  EventFilter();
  EventFilter(const EventFilter & source);
  EventFilter & operator=(const EventFilter & source);
  virtual ~EventFilter();
  virtual bool accept(const Event & event) const;
  virtual void configure(const String & ownerName,
                         const String & typeName,
                         const Configuration & configuration,
                         unsigned int index);
  
  enum EventFilterType
  { kAll,
    kMult0, kMult1, kMult2, kMult3, kMult4, kMult5,
    kEnergy0,kEnergy1,kEnergy2,kEnergy3,kEnergy4,kEnergy5,
    kCharge0,kCharge1,kCharge2,kCharge3,kCharge4,kCharge5,
    kStrange0,kStrange1,kStrange2,kStrange3,kStrange4,kStrange5,
    kBaryon0,kBaryon1,kBaryon2,kBaryon3,kBaryon4,kBaryon5,
    kSphero0,kSphero1,kSphero2,kSphero3,kSphero4,kSphero5
  };
  
  ClassDef(EventFilter,0)
  };
  
} // namespace CAP

#endif /* CAP__EventFilter */
