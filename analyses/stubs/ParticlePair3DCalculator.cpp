#include "ParticlePair3DCalculator.hpp"
#include "PrintHelpers.hpp"
ClassImp(CAP::ParticlePair3DCalculator);

namespace CAP {

ParticlePair3DCalculator::ParticlePair3DCalculator()
:
ParticlePair3DAnalyzer()
{
  appendClassName("ParticlePair3DCalculator");
  setMinimumReportLevel(Object::kInfo);
  setName("ParticlePair3DCalculator");
  setTitle("ParticlePair3DCalculator");
}

ParticlePair3DCalculator::ParticlePair3DCalculator(const ParticlePair3DCalculator & source)
:
ParticlePair3DAnalyzer(source)
{ }

ParticlePair3DCalculator &
ParticlePair3DCalculator::operator=(const ParticlePair3DCalculator & rhs)
{
  if (this != &rhs)
    ParticlePair3DAnalyzer::operator=(rhs);
  return *this;
}

void ParticlePair3DCalculator::execute()
{
  _taskExecuted.increment();
  postProcess();
}

} // namespace CAP
