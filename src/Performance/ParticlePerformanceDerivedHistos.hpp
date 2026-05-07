/* **********************************************************************
 * Copyright (C) 2019-2024, Claude Pruneau, Victor Gonzalez   
 * All rights reserved.
 *
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 *
 * Author: Claude Pruneau,   04/01/2024
 *
 * *********************************************************************/
#ifndef CAP__ParticlePerformanceDerivedHistos
#define CAP__ParticlePerformanceDerivedHistos
#include "ParticlePerformanceHistos.hpp"

namespace CAP
{
class ParticlePerformanceDerivedHistos : public ParticlePerformanceHistos
{
public:
  
  ParticlePerformanceDerivedHistos();
  virtual ~ParticlePerformanceDerivedHistos() {}
  virtual void configure(const String& taskName,
                               const String & objectType,
                               const Configuration & configuration,
                               unsigned int index);
  virtual void create();
  virtual void loadFrom(TFile & inputFile);
  virtual void calculateDerivedHistograms(const ParticlePerformanceHistos & base);

  ClassDef(ParticlePerformanceDerivedHistos,0)
};

} // namespace CAP

#endif /* CAP__ParticlePerformanceDerivedHistos  */



