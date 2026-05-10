/* Stub of CAP::ParticleDbTask (alternative DB-loader name used by some
   shipped .ini files). Same shape as ParticleTypeTask; both ultimately
   need a real implementation that loads ParticleDb from disk. */
#ifndef CAP_USER__ParticleDbTask
#define CAP_USER__ParticleDbTask
#include "EventProcessor.hpp"
namespace CAP {
class ParticleDbTask : public EventProcessor {
public:
  ParticleDbTask();
  ParticleDbTask(const ParticleDbTask & source);
  ParticleDbTask & operator=(const ParticleDbTask & rhs);
  virtual ~ParticleDbTask() {}
  virtual void execute();    // override EventProcessor's abstract execute()
  virtual void initialize(); // load ParticleDb from disk after instantiation
  ClassDef(ParticleDbTask, 0)
};
}
#endif
