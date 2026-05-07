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
#include "EventFilter.hpp"
#include "ManagedObjects.hpp"
//ClassImp(CAP::ManagedObjects<CAP::EventFilter>);
ClassImp(CAP::Filter<CAP::Event>);
ClassImp(CAP::EventFilter);

namespace CAP
{
  
  EventFilter::EventFilter()
  :
  Filter<Event>(),
  ManagedObject()
  { /* no ops */}
  
  EventFilter::EventFilter(const EventFilter & source)
  :
  Filter<Event>(source)
  { /* no ops */}
  
  EventFilter & EventFilter::operator=(const EventFilter & source)
  {
  if (this!=&source)
    {
    Filter<Event>::operator=(source);
    }
  return *this;
  }
  
  
  EventFilter::~EventFilter()
  { /* no ops */}
  
  //!
  //! accept/reject the given Event based on filter parameter
  //!
  bool EventFilter::accept(const Event & event)  const
  {
  if (nConditions()<1) return true;
  bool accepting = false;
  
  for (auto & condition : conditions())
    {
    int  subtype = condition->subtype();
    //    printValue("EventFilter::accept(const Event & event) subtype",subtype);
    //    printValue("EventFilter::accept(const Event & event) event.mult(0)",event.mult(0));
    switch (subtype)
      {
        case kMult0:  accepting = condition->accept( event.mult(0) );     break;
        case kMult1:  accepting = condition->accept( event.mult(1) );     break;
        case kMult2:  accepting = condition->accept( event.mult(2) );     break;
        case kMult3:  accepting = condition->accept( event.mult(3) );     break;
        case kMult4:  accepting = condition->accept( event.mult(4) );     break;
        case kMult5:  accepting = condition->accept( event.mult(5) );     break;
        case kEnergy0:  accepting = condition->accept( event.energy(0) );     break;
        case kEnergy1:  accepting = condition->accept( event.energy(1) );     break;
        case kEnergy2:  accepting = condition->accept( event.energy(2) );     break;
        case kEnergy3:  accepting = condition->accept( event.energy(3) );     break;
        case kEnergy4:  accepting = condition->accept( event.energy(4) );     break;
        case kEnergy5:  accepting = condition->accept( event.energy(5) );     break;
        case kSphero0:  accepting = condition->accept( event.spherocity(0) );     break;
        case kSphero1:  accepting = condition->accept( event.spherocity(1) );     break;
        case kSphero2:  accepting = condition->accept( event.spherocity(2) );     break;
        case kSphero3:  accepting = condition->accept( event.spherocity(3) );     break;
        case kSphero4:  accepting = condition->accept( event.spherocity(4) );     break;
        case kSphero5:  accepting = condition->accept( event.spherocity(5) );     break;
      }
    if (!accepting)  return false;
    }
  return true;
  }
  
  void EventFilter::configure(const String & ownerName,
                              const String & typeName,
                              const Configuration & configuration,
                              unsigned int index)
  {
  String type;
  String subtype;
  double minimum;
  double maximum;
  //  double minimum2;
  //  double maximum2;
  unsigned int nConditions;
  unsigned int subtypeIndex;
  
  nConditions = configuration.valueInt(createKey(ownerName,typeName,index,"CONDITION:N"));
  for (unsigned int iCondition=0; iCondition<nConditions; iCondition++)
    {
    type     = configuration.valueString(createKey(ownerName,typeName,index,"CONDITION",iCondition,"TYPE"));
    subtype  = configuration.valueString(createKey(ownerName,typeName,index,"CONDITION",iCondition,"SUBTYPE"));
    minimum  = configuration.valueDouble(createKey(ownerName,typeName,index,"CONDITION",iCondition,"MINIMUM"));
    maximum  = configuration.valueDouble(createKey(ownerName,typeName,index,"CONDITION",iCondition,"MAXIMUM"));
    
    if (type.EqualTo("DOUBLE_RANGE"))
      {
      if (subtype.EqualTo("MULT_0"))
        subtypeIndex = EventFilter::kMult0;
      else if (subtype.EqualTo("MULT_1"))
        subtypeIndex = EventFilter::kMult1;
      else if (subtype.EqualTo("MULT_2"))
        subtypeIndex = EventFilter::kMult2;
      else if (subtype.EqualTo("MULT_3"))
        subtypeIndex = EventFilter::kMult3;
      else if (subtype.EqualTo("MULT_4"))
        subtypeIndex = EventFilter::kMult4;
      else if (subtype.EqualTo("MULT_5"))
        subtypeIndex = EventFilter::kMult5;
      else if (subtype.EqualTo("ENERGY_0"))
        subtypeIndex = EventFilter::kEnergy0;
      else if (subtype.EqualTo("ENERGY_1"))
        subtypeIndex = EventFilter::kEnergy1;
      else if (subtype.EqualTo("ENERGY_2"))
        subtypeIndex = EventFilter::kEnergy2;
      else if (subtype.EqualTo("ENERGY_3"))
        subtypeIndex = EventFilter::kEnergy3;
      else if (subtype.EqualTo("ENERGY_4"))
        subtypeIndex = EventFilter::kEnergy4;
      else if (subtype.EqualTo("ENERGY_5"))
        subtypeIndex = EventFilter::kEnergy5;
      else if (subtype.EqualTo("SPHERO_0"))
        subtypeIndex = EventFilter::kSphero0;
      else if (subtype.EqualTo("SPHERO_1"))
        subtypeIndex = EventFilter::kSphero1;
      else if (subtype.EqualTo("SPHERO_2"))
        subtypeIndex = EventFilter::kSphero2;
      else if (subtype.EqualTo("SPHERO_3"))
        subtypeIndex = EventFilter::kSphero3;
      else if (subtype.EqualTo("SPHERO_4"))
        subtypeIndex = EventFilter::kSphero4;
      else if (subtype.EqualTo("SPHERO_5"))
        subtypeIndex = EventFilter::kSphero5;
      else
        throw FilterException(subtype,__FUNCTION__);
      addCondition(subtypeIndex,minimum,maximum);
      }
    else if (type.EqualTo("DOUBLE_DUAL_RANGE"))
      {
      if (subtype.EqualTo("MULT_0"))
        subtypeIndex = EventFilter::kMult0;
      else if (subtype.EqualTo("MULT_1"))
        subtypeIndex = EventFilter::kMult1;
      else if (subtype.EqualTo("MULT_2"))
        subtypeIndex = EventFilter::kMult2;
      else if (subtype.EqualTo("MULT_3"))
        subtypeIndex = EventFilter::kMult3;
      else if (subtype.EqualTo("MULT_4"))
        subtypeIndex = EventFilter::kMult4;
      else if (subtype.EqualTo("MULT_5"))
        subtypeIndex = EventFilter::kMult5;
      else if (subtype.EqualTo("ENERGY_0"))
        subtypeIndex = EventFilter::kEnergy0;
      else if (subtype.EqualTo("ENERGY_1"))
        subtypeIndex = EventFilter::kEnergy1;
      else if (subtype.EqualTo("ENERGY_2"))
        subtypeIndex = EventFilter::kEnergy2;
      else if (subtype.EqualTo("ENERGY_3"))
        subtypeIndex = EventFilter::kEnergy3;
      else if (subtype.EqualTo("ENERGY_4"))
        subtypeIndex = EventFilter::kEnergy4;
      else if (subtype.EqualTo("ENERGY_5"))
        subtypeIndex = EventFilter::kEnergy5;
      else if (subtype.EqualTo("SPHERO_0"))
        subtypeIndex = EventFilter::kSphero0;
      else if (subtype.EqualTo("SPHERO_1"))
        subtypeIndex = EventFilter::kSphero1;
      else if (subtype.EqualTo("SPHERO_2"))
        subtypeIndex = EventFilter::kSphero2;
      else if (subtype.EqualTo("SPHERO_3"))
        subtypeIndex = EventFilter::kSphero3;
      else if (subtype.EqualTo("SPHERO_4"))
        subtypeIndex = EventFilter::kSphero4;
      else if (subtype.EqualTo("SPHERO_5"))
        subtypeIndex = EventFilter::kSphero5;
      else
        throw FilterException(subtype,__FUNCTION__);
      addCondition(subtypeIndex,minimum,maximum); //,minimum2,maximum2);
      }
    else
      throw FilterException(type,__FUNCTION__);
    }
  }
  
  
  
} // namespace CAP
