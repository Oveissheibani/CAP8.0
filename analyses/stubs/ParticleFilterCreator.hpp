/* **********************************************************************
 *  Stub implementation of CAP::ParticleFilterCreator.
 *  Same status as EventFilterCreator: registers the class for ROOT lookup.
 * ******************************************************************** */
#ifndef CAP_USER__ParticleFilterCreator
#define CAP_USER__ParticleFilterCreator

#include "EventProcessor.hpp"

namespace CAP
{

class ParticleFilterCreator : public EventProcessor
{
public:
  ParticleFilterCreator();
  ParticleFilterCreator(const ParticleFilterCreator & source);
  ParticleFilterCreator & operator=(const ParticleFilterCreator & rhs);
  virtual ~ParticleFilterCreator() {}

  virtual void execute();        // no-op — filters built in configure()

  ClassDef(ParticleFilterCreator, 0)
};

} // namespace CAP

#endif // CAP_USER__ParticleFilterCreator
