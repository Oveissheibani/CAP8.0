/* **********************************************************************
 *  Stub implementation of CAP::ParticleTypeTask.
 *
 *  Original is missing from src/; this stub satisfies TClass::GetClass so
 *  the .ini load reaches the rest of the task tree. It does NOT yet load a
 *  particle database — that's the next step once the basic load path works.
 * ******************************************************************** */
#ifndef CAP_USER__ParticleTypeTask
#define CAP_USER__ParticleTypeTask

#include "EventProcessor.hpp"

namespace CAP
{

class ParticleTypeTask : public EventProcessor
{
public:
  ParticleTypeTask();
  ParticleTypeTask(const ParticleTypeTask & source);
  ParticleTypeTask & operator=(const ParticleTypeTask & rhs);
  virtual ~ParticleTypeTask() {}

  // EventProcessor::execute() is deliberately abstract (throws
  // NoImplementationException). This task does its work in configure() /
  // initialize() (loading the particle DB), so per-tick execute() is a no-op.
  virtual void execute();

  // Load the particle data + decay tables from $CAP_DATABASE_PATH/ParticleData
  // into the freshly-instantiated DB so analyzers can resolve PDG codes.
  virtual void initialize();

  ClassDef(ParticleTypeTask, 0)
};

} // namespace CAP

#endif // CAP_USER__ParticleTypeTask
