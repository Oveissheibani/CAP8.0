/* **********************************************************************
 *  Stub implementation of CAP::RunAnalysis — see RunAnalysis.hpp.
 * ******************************************************************** */
#include "RunAnalysis.hpp"

ClassImp(CAP::RunAnalysis);

namespace CAP
{

RunAnalysis::RunAnalysis()
:
Task()
{
  appendClassName("RunAnalysis");
  setMinimumReportLevel(Object::kInfo);
  setName("RunAnalysis");
  setTitle("RunAnalysis");
}

RunAnalysis::RunAnalysis(const RunAnalysis & source)
:
Task(source)
{ }

RunAnalysis & RunAnalysis::operator=(const RunAnalysis & rhs)
{
  if (this != &rhs)
    {
    Task::operator=(rhs);
    }
  return *this;
}

void RunAnalysis::execute()
{
  // Drive every subtask's execute() once. EventIterator (typically the last
  // subtask) is responsible for the actual per-event loop, so all the work
  // happens inside that single call.
  executeSubTasks();
  _taskExecuted.increment();
}

void RunAnalysis::finalize()
{
  // Task::finalize() ALREADY calls finalizeSubTasks() (Task.cpp:113).
  // Calling it ourselves first would walk the tree twice — double-export
  // every .root file (truncating the real content with empty histograms
  // on the second pass) and double-finalize PythiaEventGenerator.
  Task::finalize();
}

} // namespace CAP
