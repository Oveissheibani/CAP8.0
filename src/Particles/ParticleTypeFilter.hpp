#ifndef CAP__ParticleTypeFilter
#define CAP__ParticleTypeFilter
#include "ParticleType.hpp"
#include "Filter.hpp"
#include "ManagedObject.hpp"
#include "Configuration.hpp"

namespace CAP
{

  class ParticleTypeFilter
  :
  public Filter<ParticleType>,
  public ManagedObject
  {
  public:

  enum ParticleTypeFilterType
  { kStable, kEnabled, kPDG, kMass, kCharge, kStrange, kCharm, kBottom, kBaryon, kLepton, kNeutrino, kPhoton, kGluon, kWeakBoson, kHiggs, kLifeTime, kWidth };

  ParticleTypeFilter();
  ParticleTypeFilter(const ParticleTypeFilter & source);
  ParticleTypeFilter & operator=(const ParticleTypeFilter & rhs);
  virtual ~ParticleTypeFilter() {}
  virtual bool accept(const ParticleType & particleType) const;
  virtual void configure(const String & ownerName,
                               const String & typeName,
                               const Configuration & configuration,
                               unsigned int index);

  ClassDef(ParticleTypeFilter,0)
  };

}

#endif /* CAP__ParticleTypeFilter */

