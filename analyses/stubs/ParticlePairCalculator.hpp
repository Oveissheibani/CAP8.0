/* ----------------------------------------------------------------------
 * CAP::ParticlePairCalculator — thin subclass of ParticlePairAnalyzer.
 *
 * Stage-2 (RunDerived) Task that imports the n2 histograms produced by
 * stage-1 ParticlePairAnalyzer, then calls postProcess() which
 * delegates to EventProcessorPair::calculateDerived().  That base
 * method walks (eventFilter × particleFilter1 × particleFilter2) and
 * for each cell calls
 *   ParticlePairDerivedHistos::calculateDerivedHistograms(
 *       singleDerived_1, singleDerived_2, basePair)
 * which is the function in
 *   src/ParticlePair/ParticlePairDerivedHistos.cpp
 * (664 lines, byte-identical to original) that computes R2_*, C2_*,
 * R2 vs Δη/Δφ, etc.
 *
 * The single-derived histos (ρ1) come from a peer
 * ParticleSingleCalculator that runs *before* this one in the same
 * RunDerived block — they're shared via the static ManagedObjects
 * pool keyed by (typeName, name) with Owner=1 in Single Calculator
 * and Owner=0 in this one's HISTOGRAM_4 block.
 *
 * Loyalty note: this class adds NO physics.  It is a renamed Analyzer
 * whose execute() bypasses event filling and triggers calculateDerived().
 * --------------------------------------------------------------------*/
#ifndef CAP_USER__ParticlePairCalculator
#define CAP_USER__ParticlePairCalculator
#include "ParticlePairAnalyzer.hpp"
namespace CAP {
class ParticlePairCalculator : public ParticlePairAnalyzer {
public:
  ParticlePairCalculator();
  ParticlePairCalculator(const ParticlePairCalculator & source);
  ParticlePairCalculator & operator=(const ParticlePairCalculator & rhs);
  virtual ~ParticlePairCalculator() {}
  virtual void execute();
  ClassDef(ParticlePairCalculator, 0)
};
}
#endif
