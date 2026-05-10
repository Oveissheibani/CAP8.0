/* ----------------------------------------------------------------------
 * Implementation of CAP::ParticlePairCalculator.  See header for design.
 * --------------------------------------------------------------------*/
#include "ParticlePairCalculator.hpp"
#include "PrintHelpers.hpp"
ClassImp(CAP::ParticlePairCalculator);

namespace CAP {

ParticlePairCalculator::ParticlePairCalculator()
:
ParticlePairAnalyzer()
{
  appendClassName("ParticlePairCalculator");
  setMinimumReportLevel(Object::kInfo);
  setName("ParticlePairCalculator");
  setTitle("ParticlePairCalculator");
}

ParticlePairCalculator::ParticlePairCalculator(const ParticlePairCalculator & source)
:
ParticlePairAnalyzer(source)
{ }

ParticlePairCalculator &
ParticlePairCalculator::operator=(const ParticlePairCalculator & rhs)
{
  if (this != &rhs)
    ParticlePairAnalyzer::operator=(rhs);
  return *this;
}

void ParticlePairCalculator::execute()
{
  _taskExecuted.increment();
  postProcess();
}

} // namespace CAP
