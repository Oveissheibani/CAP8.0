/* ----------------------------------------------------------------------
 * CAP::ParticleSingleCalculator — thin subclass of ParticleSingleAnalyzer.
 *
 * Stage-2 (RunDerived) Task that imports the n1 histograms produced by
 * stage-1 ParticleSingleAnalyzer, then calls postProcess() which
 * delegates to EventProcessorSingle::calculateDerived().  That base
 * method (Pruneau's untouched code in EventProcessorSingle.hpp) walks
 * the (eventFilter × particleFilter) grid and for each cell calls
 *   ParticleSingleDerivedHistos::calculateDerivedHistograms(n1_base)
 * which fills the per-species single-particle density ρ1 and its
 * projections.  finalize() exports the ρ1 histos to the file named in
 * HISTOGRAM_2:EXPORT:FILE_NAME.
 *
 * Loyalty note: this class adds NO physics.  It is a renamed Analyzer
 * whose execute() bypasses event filling and triggers calculateDerived().
 * --------------------------------------------------------------------*/
#ifndef CAP_USER__ParticleSingleCalculator
#define CAP_USER__ParticleSingleCalculator
#include "ParticleSingleAnalyzer.hpp"
namespace CAP {
class ParticleSingleCalculator : public ParticleSingleAnalyzer {
public:
  ParticleSingleCalculator();
  ParticleSingleCalculator(const ParticleSingleCalculator & source);
  ParticleSingleCalculator & operator=(const ParticleSingleCalculator & rhs);
  virtual ~ParticleSingleCalculator() {}
  virtual void execute();
  ClassDef(ParticleSingleCalculator, 0)
};
}
#endif
