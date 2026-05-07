#include "GlobalCalculator.hpp"
#include "PrintHelpers.hpp"
ClassImp(CAP::GlobalCalculator);

namespace CAP {

GlobalCalculator::GlobalCalculator()
:
GlobalAnalyzer()
{
  appendClassName("GlobalCalculator");
  setMinimumReportLevel(Object::kInfo);
  setName("GlobalCalculator");
  setTitle("GlobalCalculator");
}

GlobalCalculator::GlobalCalculator(const GlobalCalculator & source)
:
GlobalAnalyzer(source)
{ }

GlobalCalculator & GlobalCalculator::operator=(const GlobalCalculator & rhs)
{
  if (this != &rhs)
    GlobalAnalyzer::operator=(rhs);
  return *this;
}

void GlobalCalculator::execute()
{
  _taskExecuted.increment();
  postProcess();
}

} // namespace CAP
