#include "FilterCreator.hpp"
ClassImp(CAP::FilterCreator);
namespace CAP {
FilterCreator::FilterCreator() : EventProcessor() {
  appendClassName("FilterCreator");
  setMinimumReportLevel(Object::kInfo);
  setName("FilterCreator"); setTitle("FilterCreator");
}
FilterCreator::FilterCreator(const FilterCreator & s) : EventProcessor(s) {}
FilterCreator & FilterCreator::operator=(const FilterCreator & r) {
  if (this != &r) EventProcessor::operator=(r); return *this;
}
void FilterCreator::execute() { _taskExecuted.increment(); }
}
