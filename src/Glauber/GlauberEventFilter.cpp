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
#include "GlauberEventFilter.hpp"
#include "NameHelpers.hpp"
#include "PrintHelpers.hpp"
ClassImp(CAP::Filter<CAP::GlauberEvent>);
ClassImp(CAP::GlauberEventFilter);

namespace CAP
{
  GlauberEventFilter::GlauberEventFilter()
  :
  Filter<GlauberEvent>(),
  ManagedObject()
  { /* no ops */}

  GlauberEventFilter::GlauberEventFilter(const GlauberEventFilter & source)
  :
  Filter<GlauberEvent>(source),
  ManagedObject(source)
  { /* no ops */}

  GlauberEventFilter & GlauberEventFilter::operator=(const GlauberEventFilter & rhs)
  {
  if (this!=&rhs)
    {
    Filter<GlauberEvent>::operator=(rhs);
    ManagedObject::operator=(rhs);
    }
  return *this;
  }

  //!
  //! accept/reject the given GlauberEvent based on filter parameter
  //!
  bool GlauberEventFilter::accept(const GlauberEvent & event)
  {
  if (nConditions()<1) return true;
  bool accepting = false;
  for (auto & condition : conditions())
    {
    int  subtype = condition->subtype();
    switch (subtype)
      {
        case kAny:     accepting = true; break;
        case kImpact:  accepting = condition->accept( event.impactParameter() );   break;
        case kNParts:  accepting = condition->accept( event.nParticipants() );     break;
        case kNColls:  accepting = condition->accept( event.nCollisions() ); break;
        case kNSpecs:  accepting = condition->accept( event.nSpectators() );       break;
      }
    if (!accepting)  return false;
    }
  return true;
  }

  void GlauberEventFilter::configure(const String & ownerName,
                                     const String & typeName,
                                     const Configuration & configuration,
                                     unsigned int index)
  {
  String type;
  String subtype;
  double minimum;
  double maximum;
  double minimum2;
  double maximum2;
  int    nConditions;
  nConditions = configuration.valueInt(createKey(ownerName,typeName,index,"CONDITION:N"));
  for (int iCondition=0; iCondition<nConditions; iCondition++)
    {
    type     = configuration.valueString(createKey(ownerName,typeName,index,"CONDITION",iCondition,"TYPE"));
    subtype  = configuration.valueString(createKey(ownerName,typeName,index,"CONDITION",iCondition,"SUBTYPE"));
    minimum  = configuration.valueDouble(createKey(ownerName,typeName,index,"CONDITION",iCondition,"MINIMUM"));
    maximum  = configuration.valueDouble(createKey(ownerName,typeName,index,"CONDITION",iCondition,"MAXIMUM"));
    if (type.EqualTo("DOUBLE_DUAL_RANGE"))
      {
      minimum2  = configuration.valueDouble(createKey(ownerName,typeName,index,"CONDITION",iCondition,"MINIMUM2"));
      maximum2  = configuration.valueDouble(createKey(ownerName,typeName,index,"CONDITION",iCondition,"MAXIMUM2"));
      }
    else
      {
      minimum2 = 0.0;
      maximum2 = 0.0;
      }
    int subtypeIndex;
    if (type.EqualTo("DOUBLE_RANGE"))
      {
      if (subtype.EqualTo("IMPACT"))
        subtypeIndex = GlauberEventFilter::kImpact;
      else if (subtype.EqualTo("NPARTS"))
        subtypeIndex = GlauberEventFilter::kNParts;
      else if (subtype.EqualTo("NCOLLS"))
        subtypeIndex = GlauberEventFilter::kNColls;
      else if (subtype.EqualTo("NSPECS"))
        subtypeIndex = GlauberEventFilter::kNSpecs;
      else
        throw FilterException(subtype,__FUNCTION__);
      addCondition(subtypeIndex,minimum,maximum);
      }
    else if (type.EqualTo("DOUBLE_DUAL_RANGE"))
      {
      if (subtype.EqualTo("IMPACT"))
        subtypeIndex = GlauberEventFilter::kImpact;
      else if (subtype.EqualTo("NPARTS"))
        subtypeIndex = GlauberEventFilter::kNParts;
      else if (subtype.EqualTo("NCOLLS"))
        subtypeIndex = GlauberEventFilter::kNColls;
      else if (subtype.EqualTo("NSPECS"))
        subtypeIndex = GlauberEventFilter::kNSpecs;
      else
        throw FilterException(subtype,__FUNCTION__);
      addCondition(subtypeIndex,minimum,maximum,minimum2,maximum2);
      }
    else if (type.EqualTo("INTEGER"))
      {
      throw FilterException(subtype,__FUNCTION__);
      }
    else if (type.EqualTo("BOOL"))
      {
      throw FilterException(subtype,__FUNCTION__);
      }
    else
      throw FilterException(type,__FUNCTION__);
    }
  }



} // namespace CAP
