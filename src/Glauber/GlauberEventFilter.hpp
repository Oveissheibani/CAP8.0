#ifndef CAP__GlauberEventFilter
#define CAP__GlauberEventFilter
#include "Filter.hpp"
#include "GlauberEvent.hpp"
#include "ManagedObject.hpp"

namespace CAP
{
  class GlauberEventFilter
  :
  public Filter<GlauberEvent>,
  public ManagedObject
  {
  public:

  //!
  //! Filter types
  //!  kAny : all Glauber accepted.
  //!  kImpact: filtering based on impact parameter.
  //!  kNParts:  filtering based on number of participants.
  //!  kNColls:  filtering based on number of binary collisions.
  //!  kNSpecs: filtering based on number of spectators..
  //!
  enum GlauberEventFilterType { kAny, kImpact, kNParts, kNColls, kNSpecs};

  GlauberEventFilter();
  GlauberEventFilter(const GlauberEventFilter & source);
  GlauberEventFilter & operator=(const GlauberEventFilter & source);
  virtual ~GlauberEventFilter() {}
  virtual bool accept(const GlauberEvent & event);
  virtual void configure(const String & ownerName,
                         const String & typeName,
                         const Configuration & configuration,
                         unsigned int index);
  
  ClassDef(GlauberEventFilter,0)
  };

} // namespace CAP

#endif /* CAP__GlauberEventFilter */
