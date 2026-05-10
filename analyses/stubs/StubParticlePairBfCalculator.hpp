/* ----------------------------------------------------------------------
 * CAP::ParticlePairBfCalculator — REAL 1D Pair Balance-Function
 * calculator.  Replaces the earlier TFile::Cp pass-through stub.
 *
 * FILENAME NOTE: this file is named StubParticlePairBfCalculator.hpp on
 * purpose — the shipped header src/ParticlePair/ParticlePairBfCalculator.hpp
 * #includes the missing upstream Calculator.hpp and rootcling resolved
 * the shipped header before our stub when both shared the same name.
 * Renaming the file (not the C++ class) lets rootcling parse this
 * implementation.  Class name CAP::ParticlePairBfCalculator is
 * unchanged so the .ini and ROOT TClass lookups continue to work.
 *
 * ALGORITHM: transcribed from
 *   src/ParticlePair/BalanceFunctionCalculator.cpp lines 140-260
 * (Pruneau's calculate_CI / calculate_CD / calculate_BalFct).  Same
 * arithmetic, just inlined here with zero new physics.  See
 * MISSING_CLASSES.md "Balance-Function antiparticle convention" for
 * the index-offset rule that determines particle ↔ antiparticle pairs.
 * --------------------------------------------------------------------*/
#ifndef CAP_USER__ParticlePairBfCalculator
#define CAP_USER__ParticlePairBfCalculator

#include "Task.hpp"
#include "TString.h"

class TFile;
class TH1;
class TH2;

namespace CAP {

class ParticlePairBfCalculator : public Task
{
public:
  ParticlePairBfCalculator();
  ParticlePairBfCalculator(const ParticlePairBfCalculator & source);
  ParticlePairBfCalculator & operator=(const ParticlePairBfCalculator & rhs);
  virtual ~ParticlePairBfCalculator() {}

  virtual void execute();

private:
  // -------------------------------------------------------------------
  //  Pruneau's math, transcribed (BalanceFunctionCalculator.cpp:140-260)
  // -------------------------------------------------------------------
  // CI  = 0.25 * (obs_1_2 + obs_1Bar_2 + obs_1_2Bar + obs_1Bar_2Bar)
  // CD  = 0.50 * (obs_1Bar_2 + obs_1_2Bar - obs_1_2 - obs_1Bar_2Bar)
  // BalFct = (obs_US - obs_LS) / x_bin_width
  TH2 * compute_CI(TH2 * obs_1_2,    TH2 * obs_1Bar_2,
                   TH2 * obs_1_2Bar, TH2 * obs_1Bar_2Bar,
                   const TString & outName);
  TH2 * compute_CD(TH2 * obs_1_2,    TH2 * obs_1Bar_2,
                   TH2 * obs_1_2Bar, TH2 * obs_1Bar_2Bar,
                   const TString & outName);
  TH2 * compute_BalFct(TH2 * obs_US, TH2 * obs_LS,
                       const TString & outName);

  ClassDef(ParticlePairBfCalculator, 0)
};

} // namespace CAP

#endif
