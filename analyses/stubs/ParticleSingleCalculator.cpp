/* ----------------------------------------------------------------------
 * Implementation of CAP::ParticleSingleCalculator.  See header for design.
 *
 * execute() does the bare minimum: increment the executed counter and
 * call postProcess(), which is the EventProcessor base's hook that
 * delegates to calculateDerived() (defined in EventProcessorSingle.hpp,
 * line 154 in CAP8-original — byte-identical in our tree).  That method
 * walks (eventFilter × particleFilter) and for each combination invokes
 *   ParticleSingleDerivedHistos::calculateDerivedHistograms(n1)
 * which is the function in src/ParticleSingle/ParticleSingleDerivedHistos.cpp
 * (219 lines, byte-identical to original) that computes ρ1 etc.
 *
 * The histograms come from disk because the .ini sets
 * <task>:HISTOGRAM_1:IMPORT=1 and IMPORT:FILE_NAME=SingleGen.root.
 * The newly-computed derived histos are exported because
 * <task>:HISTOGRAM_2:EXPORT=1 and EXPORT:FILE_NAME=SingleDerivedGen.root.
 * --------------------------------------------------------------------*/
#include "ParticleSingleCalculator.hpp"
#include "PrintHelpers.hpp"
ClassImp(CAP::ParticleSingleCalculator);

namespace CAP {

ParticleSingleCalculator::ParticleSingleCalculator()
:
ParticleSingleAnalyzer()
{
  appendClassName("ParticleSingleCalculator");
  setMinimumReportLevel(Object::kInfo);
  setName("ParticleSingleCalculator");
  setTitle("ParticleSingleCalculator");
}

ParticleSingleCalculator::ParticleSingleCalculator(const ParticleSingleCalculator & source)
:
ParticleSingleAnalyzer(source)
{ }

ParticleSingleCalculator &
ParticleSingleCalculator::operator=(const ParticleSingleCalculator & rhs)
{
  if (this != &rhs)
    ParticleSingleAnalyzer::operator=(rhs);
  return *this;
}

void ParticleSingleCalculator::execute()
{
  // No event loop here — histograms come from disk via initialize()
  // when HISTOGRAM_1:IMPORT=1.  postProcess() invokes calculateDerived()
  // which fills ρ1 (and all single-particle derived projections).
  _taskExecuted.increment();
  postProcess();
}

} // namespace CAP
