/* **********************************************************************
 *  Stub implementation of CAP::ParticleFilterCreator — see header.
 * ******************************************************************** */
#include "ParticleFilterCreator.hpp"

ClassImp(CAP::ParticleFilterCreator);

namespace CAP
{

ParticleFilterCreator::ParticleFilterCreator()
:
EventProcessor()
{
  appendClassName("ParticleFilterCreator");
  setMinimumReportLevel(Object::kInfo);
  setName("ParticleFilterCreator");
  setTitle("ParticleFilterCreator");
}

ParticleFilterCreator::ParticleFilterCreator(const ParticleFilterCreator & source)
:
EventProcessor(source)
{ }

ParticleFilterCreator & ParticleFilterCreator::operator=(const ParticleFilterCreator & rhs)
{
  if (this != &rhs)
    {
    EventProcessor::operator=(rhs);
    }
  return *this;
}

void ParticleFilterCreator::execute()
{
  _taskExecuted.increment();
}

} // namespace CAP
