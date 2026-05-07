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
#include "NameHelpers.hpp"
#include "ParticleFilter.hpp"
#include "PrintHelpers.hpp"

ClassImp(CAP::Filter<CAP::Particle>);
ClassImp(CAP::ParticleFilter);

namespace CAP
{
  
  ParticleFilter::ParticleFilter()
  :
  Filter<Particle>(),
  ManagedObject()
  { /* no ops */}
  
  ParticleFilter::ParticleFilter(const ParticleFilter & source)
  :
  Filter<Particle>(source),
  ManagedObject(source)
  { /* no ops */}
  
  ParticleFilter & ParticleFilter::operator=(const ParticleFilter & rhs)
  {
  if (this!=&rhs)
    {
    Filter<Particle>::operator=(rhs);
    ManagedObject::operator=(rhs);
    }
  return *this;
  }
  
  bool ParticleFilter::accept(const Particle & particle) const
  {
  if (nConditions()<1) return true;
  bool   accepting = false;
  for (auto & condition : conditions())
    {
    int subtype = condition->subtype();
    switch (subtype)
      {
        case kPt:       accepting = condition->accept(particle.momentum().perp());  break;
        case kPx:       accepting = condition->accept(particle.momentum().x());  break;
        case kPy:       accepting = condition->accept(particle.momentum().y());  break;
        case kPz:       accepting = condition->accept(particle.momentum().z());  break;
        case kEnergy:   accepting = condition->accept(particle.momentum().t());   break;
        case kEta:      accepting = condition->accept(particle.momentum().pseudorapidity()); break;
        case kRapidity: accepting = condition->accept(particle.momentum().rapidity()); break;
        case kPhi:      accepting = condition->accept(particle.momentum().phi()); break;
        case kPdg:      accepting = condition->accept(particle.type().pdgCode()); break;
        case kCharge:   accepting = condition->accept(particle.type().charge()); break;
        case kStrange:  accepting = condition->accept(particle.type().strangeness()); break;
        case kBaryon:   accepting = condition->accept(particle.type().baryonNumber()); break;
        case kCharm:    accepting = condition->accept(particle.type().charm()); break;
        //case kBeauty:   accepting = condition->accept(particle.type().getBeauty()); break;
        case kLive:     accepting = condition->accept(particle.isLive()); break;
        case kEnabled:  accepting = condition->accept(particle.type().isEnabled());
      }
    if (!accepting)  return false;
    }
  return true;
  }
  
  void ParticleFilter::configure(const String & ownerName,
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
      if (subtype.EqualTo("PT"))
        subtypeIndex = ParticleFilter::kPt;
      else if (subtype.EqualTo("PX"))
        subtypeIndex = ParticleFilter::kPx;
      else if (subtype.EqualTo("PY"))
        subtypeIndex = ParticleFilter::kPy;
      else if (subtype.EqualTo("PZ"))
        subtypeIndex = ParticleFilter::kPz;
      else if (subtype.EqualTo("ENERGY"))
        subtypeIndex = ParticleFilter::kEnergy;
      else if (subtype.EqualTo("ETA"))
        subtypeIndex = ParticleFilter::kEta;
      else if (subtype.EqualTo("RAPIDITY"))
        subtypeIndex = ParticleFilter::kRapidity;
      else if (subtype.EqualTo("PHI"))
        subtypeIndex = ParticleFilter::kPhi;
      else
        throw FilterException(subtype,__FUNCTION__);
      addCondition(subtypeIndex,minimum,maximum);
      }
    else if (type.EqualTo("DOUBLE_DUAL_RANGE"))
      {
      if (subtype.EqualTo("PT"))
        subtypeIndex = ParticleFilter::kPt;
      else if (subtype.EqualTo("PX"))
        subtypeIndex = ParticleFilter::kPx;
      else if (subtype.EqualTo("PY"))
        subtypeIndex = ParticleFilter::kPy;
      else if (subtype.EqualTo("PZ"))
        subtypeIndex = ParticleFilter::kPz;
      else if (subtype.EqualTo("ENERGY"))
        subtypeIndex = ParticleFilter::kEnergy;
      else if (subtype.EqualTo("ETA"))
        subtypeIndex = ParticleFilter::kEta;
      else if (subtype.EqualTo("RAPIDITY"))
        subtypeIndex = ParticleFilter::kRapidity;
      else if (subtype.EqualTo("PHI"))
        subtypeIndex = ParticleFilter::kPhi;
      else
        throw FilterException(subtype,__FUNCTION__);
      addCondition(subtypeIndex,minimum,maximum,minimum2,maximum2);
      }
    else if (type.EqualTo("INTEGER"))
      {
      if (subtype.EqualTo("PDG"))
        subtypeIndex = ParticleFilter::kPdg;
      else if (subtype.EqualTo("CHARGE"))
        subtypeIndex = ParticleFilter::kCharge;
      else if (subtype.EqualTo("BARYON"))
        subtypeIndex = ParticleFilter::kBaryon;
      else if (subtype.EqualTo("STRANGE"))
        subtypeIndex = ParticleFilter::kStrange;
      else if (subtype.EqualTo("CHARM"))
        subtypeIndex = ParticleFilter::kCharm;
      else if (subtype.EqualTo("CHARM"))
        subtypeIndex = ParticleFilter::kCharm;
      else
        throw FilterException(subtype,__FUNCTION__);
      addCondition(subtypeIndex,int(minimum));
      }
    else if (type.EqualTo("Bool"))
      {
      if (subtype.EqualTo("LIVE"))
        subtypeIndex = ParticleFilter::kLive;
      else if (subtype.EqualTo("ENABLED"))
        subtypeIndex = ParticleFilter::kEnabled;
      else
        throw FilterException(subtype,__FUNCTION__);
      addCondition(subtypeIndex,bool(minimum));
      }
    else
      throw FilterException(type,__FUNCTION__);
    }
  }
  
} // namespace CAP
