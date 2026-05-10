/* Stub of CAP::ParticlePair3DBfCalculator — post-processing Calculator. */
#ifndef CAP_USER__ParticlePair3DBfCalculator
#define CAP_USER__ParticlePair3DBfCalculator
#include "Task.hpp"
namespace CAP {
class ParticlePair3DBfCalculator : public Task {
public:
  ParticlePair3DBfCalculator();
  ParticlePair3DBfCalculator(const ParticlePair3DBfCalculator & source);
  ParticlePair3DBfCalculator & operator=(const ParticlePair3DBfCalculator & rhs);
  virtual ~ParticlePair3DBfCalculator() {}
  virtual void execute();
  ClassDef(ParticlePair3DBfCalculator, 0)
};
}
#endif
