/* **********************************************************************
 *  Stub implementation of CAP::RunAnalysis.
 *
 *  RunAnalysis is the canonical top-level container task referenced by
 *  every shipped projects ini file and by every ini emitted by the
 *  analyses/builder/build-ini-gui. The original implementation is missing
 *  from src/; this stub exists so ROOT's TClass::GetClass("CAP::RunAnalysis")
 *  succeeds and the ini load can walk the task tree.
 *
 *  Behaviour: it inherits Task. Task::initialize already recurses into
 *  subtasks, but Task::execute does NOT — so RunAnalysis explicitly drives
 *  its children's execute() and finalize() (see RunAnalysis.cpp).
 * ******************************************************************** */
#ifndef CAP_USER__RunAnalysis
#define CAP_USER__RunAnalysis

#include "Task.hpp"

namespace CAP
{

class RunAnalysis : public Task
{
public:

  RunAnalysis();
  RunAnalysis(const RunAnalysis & source);
  RunAnalysis & operator=(const RunAnalysis & rhs);
  virtual ~RunAnalysis() {}

  // Task::execute() in the base class does not recurse into subtasks
  // (only initialize() does). RunAnalysis is the top-level container, so
  // it must explicitly drive its children's execute()/finalize() — otherwise
  // EventIterator never runs and no events are processed.
  // (override keyword omitted to avoid -Winconsistent-missing-override
  // warnings caused by ROOT's ClassDef-generated members below.)
  virtual void execute();
  virtual void finalize();

  ClassDef(RunAnalysis, 0)
};

} // namespace CAP

#endif // CAP_USER__RunAnalysis
