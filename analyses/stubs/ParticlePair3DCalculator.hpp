/* ----------------------------------------------------------------------
 * CAP::ParticlePair3DCalculator — thin subclass of ParticlePair3DAnalyzer.
 *
 * Stage-2 (RunDerived) Task.  Imports the 3D pair n2 histos written by
 * stage-1 ParticlePair3DAnalyzer, then triggers calculateDerived() via
 * postProcess() — that's Pruneau's untouched code in
 * src/ParticlePair3D/ParticlePair3DDerivedHistos.cpp (596 lines).
 *
 * Loyalty note: zero new physics — renamed Analyzer that drives
 * postProcess() instead of an event loop.
 * --------------------------------------------------------------------*/
#ifndef CAP_USER__ParticlePair3DCalculator
#define CAP_USER__ParticlePair3DCalculator
#include "ParticlePair3DAnalyzer.hpp"
namespace CAP {
class ParticlePair3DCalculator : public ParticlePair3DAnalyzer {
public:
  ParticlePair3DCalculator();
  ParticlePair3DCalculator(const ParticlePair3DCalculator & source);
  ParticlePair3DCalculator & operator=(const ParticlePair3DCalculator & rhs);
  virtual ~ParticlePair3DCalculator() {}
  virtual void execute();
  ClassDef(ParticlePair3DCalculator, 0)
};
}
#endif
