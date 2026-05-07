#include "ParticleTypeFilter.hpp"
#include "PrintHelpers.hpp"

ClassImp(CAP::Filter<CAP::ParticleType>);

namespace CAP
{

  ParticleTypeFilter::ParticleTypeFilter()
  :
  Filter<ParticleType>(),
  ManagedObject()
  { /* no ops */}

  ParticleTypeFilter::ParticleTypeFilter(const ParticleTypeFilter & source)
  :
  Filter<ParticleType>(source),
  ManagedObject(source)
  { /* no ops */}

  ParticleTypeFilter & ParticleTypeFilter::operator=(const ParticleTypeFilter & rhs)
  {
  if (this!=&rhs)
    {
    Filter<ParticleType>::operator=(rhs);
    ManagedObject::operator=(rhs);
    }
  return *this;
  }

  bool ParticleTypeFilter::accept(const ParticleType & type) const
  {
  if (nConditions()<1) return true;
  bool   accepting = false;
  for (auto & condition : conditions)
    {
    int filterType    = condition->conditionType;
    int filterSubType = condition->conditionSubtype;
    switch (filterType)
      {
        case kConditionBool:
        switch (filterSubType)
          {
            case kStable:   accepting = condition->accept(type.isStable()); break;
            case kEnabled:  accepting = condition->accept(type.isEnabled()); break;
            case kCharge:   accepting = condition->accept(type.isCharged()); break;
            case kStrange:  accepting = condition->accept(type.isStrange()); break;
            case kCharm:    accepting = condition->accept(type.isCharm()); break;
            case kBottom:   accepting = condition->accept(type.isBottom()); break;
            case kBaryon:   accepting = condition->accept(type.isBaryon()); break;
            case kLepton:   accepting = condition->accept(type.isLeptonn()); break;
            case kLeptonElectron: accepting = condition->accept(type.isLeptonElectron()); break;
            case kLeptonMuon:     accepting = condition->accept(type.isLeptonMuon()); break;
            case kLeptonTau:      accepting = condition->accept(type.isLeptonTau()); break;
            case kNeutrino:       accepting = condition->accept(type.isNeutrino()); break;
            case kPhoton:         accepting = condition->accept(type.isPhoton()); break;
            case kGluon:          accepting = condition->accept(type.isGluon()); break;
            case kWeakBoson:      accepting = condition->accept(type.isWeakBoson()); break;
            case kHiggs:          accepting = condition->accept(type.isHiggs()); break;
            case kQuark:          accepting = condition->accept(type.isQuark()); break;
          }
        break;

        case kConditionInteger:
        switch (filterSubType)
          {
            case kPDG:      accepting = condition->accept(type.pdgCode()); break;
            case kCharge:   accepting = condition->accept(type.charge()); break;
            case kStrange:  accepting = condition->accept(type.strangeness()); break;
            case kCharm:    accepting = condition->accept(type.charm()); break;
            case kBottom:   accepting = condition->accept(type.getBottomNumber()); break;
            case kBaryon:   accepting = condition->accept(type.baryonNumber()); break;
            case kLeptonElectron: accepting = condition->accept(type.leptonElectron()); break;
            case kLeptonMuon:     accepting = condition->accept(type.leptonMuon()); break;
            case kLeptonTau:      accepting = condition->accept(type.leptonTau()); break;
          }
        break;

        case kConditionIntegerRange:
        switch (filterSubType)
          {
            case kPDG:      accepting = condition->accept(type.pdgCode()); break;
            case kCharge:   accepting = condition->accept(type.charge()); break;
            case kStrange:  accepting = condition->accept(type.strangeness()); break;
            case kCharm:    accepting = condition->accept(type.charm()); break;
            case kBottom:   accepting = condition->accept(type.getBottomNumber()); break;
            case kBaryon:   accepting = condition->accept(type.baryonNumber()); break;
          }
        break;

        case kDoubleRange:
        case kDualRange:
        switch (filterSubType)
          {
            case kMass:     accepting = condition->accept(type.mass()); break;
            case kLifeTime: accepting = condition->accept(type.lifeTime()); break;
            case kWidth:    accepting = condition->accept(type.getWidth()); break;
          }
        break;

      }

    if (!accepting)  return false;
    }
  return true;
  }

  void ParticleTypeFilter::configure(const String & ownerName,
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
      if (subtype.EqualTo("PDG"))
        subtypeIndex = ParticleTypeFilter::kPDG;
      else if (subtype.EqualTo("MASS"))
        subtypeIndex = ParticleTypeFilter::kMass;
      else if (subtype.EqualTo("CHARGE"))
        subtypeIndex = ParticleTypeFilter::kCharge;
      else if (subtype.EqualTo("STRANGE"))
        subtypeIndex = ParticleTypeFilter::kStrange;
      else if (subtype.EqualTo("CHARM"))
        subtypeIndex = ParticleTypeFilter::kCharm;
      else if (subtype.EqualTo("BOTTOM"))
        subtypeIndex = ParticleTypeFilter::kBottom;
      else if (subtype.EqualTo("BARYON"))
        subtypeIndex = ParticleTypeFilter::kBaryon;
      else
        throw FilterException(subtype,__FUNCTION__);
      addCondition(subtypeIndex,minimum,maximum);
      }
    else if (type.EqualTo("DOUBLE_DUAL_RANGE"))
      {
      if (subtype.EqualTo("PDG"))
        subtypeIndex = ParticleTypeFilter::kPDG;
      else if (subtype.EqualTo("MASS"))
        subtypeIndex = ParticleTypeFilter::kMass;
      else if (subtype.EqualTo("CHARGE"))
        subtypeIndex = ParticleTypeFilter::kCharge;
      else if (subtype.EqualTo("STRANGE"))
        subtypeIndex = ParticleTypeFilter::kStrange;
      else if (subtype.EqualTo("CHARM"))
        subtypeIndex = ParticleTypeFilter::kCharm;
      else if (subtype.EqualTo("BOTTOM"))
        subtypeIndex = ParticleTypeFilter::kBottom;
      else if (subtype.EqualTo("BARYON"))
        subtypeIndex = ParticleTypeFilter::kBaryon;
      else
        throw FilterException(subtype,__FUNCTION__);
      addCondition(subtypeIndex,minimum,maximum,minimum2,maximum2);
      }
    else if (type.EqualTo("INTEGER"))
      {
      if (subtype.EqualTo("PDG"))
        subtypeIndex = ParticleTypeFilter::kPDG;
      else if (subtype.EqualTo("MASS"))
        subtypeIndex = ParticleTypeFilter::kMass;
      else if (subtype.EqualTo("CHARGE"))
        subtypeIndex = ParticleTypeFilter::kCharge;
      else if (subtype.EqualTo("STRANGE"))
        subtypeIndex = ParticleTypeFilter::kStrange;
      else if (subtype.EqualTo("CHARM"))
        subtypeIndex = ParticleTypeFilter::kCharm;
      else if (subtype.EqualTo("BOTTOM"))
        subtypeIndex = ParticleTypeFilter::kBottom;
      else if (subtype.EqualTo("BARYON"))
        subtypeIndex = ParticleTypeFilter::kBaryon;
      else
        throw FilterException(subtype,__FUNCTION__);
      addCondition(subtypeIndex,int(minimum));
      }
    else if (type.EqualTo("Bool"))
      {
      if (subtype.EqualTo("STABLE"))
        subtypeIndex = ParticleTypeFilter::kStable;
      else if (subtype.EqualTo("ENABLED"))
        subtypeIndex = ParticleTypeFilter::kEnabled;
      else if (subtype.EqualTo("STRANGE"))
        subtypeIndex = ParticleTypeFilter::kStrange;
      else if (subtype.EqualTo("CHARM"))
        subtypeIndex = ParticleTypeFilter::kCharm;
      else if (subtype.EqualTo("BOTTOM"))
        subtypeIndex = ParticleTypeFilter::kBottom;
      else if (subtype.EqualTo("BARYON"))
        subtypeIndex = ParticleTypeFilter::kBaryon;
      else if (subtype.EqualTo("LEPTON"))
        subtypeIndex = ParticleTypeFilter::kLepton;
      else if (subtype.EqualTo("NEUTRINO"))
        subtypeIndex = ParticleTypeFilter::kNeutrino;
      else if (subtype.EqualTo("PHOTON"))
        subtypeIndex = ParticleTypeFilter::kPhoton;
      else if (subtype.EqualTo("GLUON"))
        subtypeIndex = ParticleTypeFilter::kGluon;
      else if (subtype.EqualTo("WEAK_BOSON"))
        subtypeIndex = ParticleTypeFilter::kWeakBoson;
      else if (subtype.EqualTo("HIGGS"))
        subtypeIndex = ParticleTypeFilter::kHiggs;
      else if (subtype.EqualTo("QUARK"))
        subtypeIndex = ParticleTypeFilter::kQuark;
      else
        throw FilterException(subtype,__FUNCTION__);
      addCondition(subtypeIndex,bool(minimum));
      }
    else
      throw FilterException(type,__FUNCTION__);
    }
  }




} // namespace CAP
