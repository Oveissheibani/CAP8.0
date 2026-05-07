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
#ifndef CAP__ParticlePair3DBfHistos
#define CAP__ParticlePair3DBfHistos
#include "ParticleSingleHistos.hpp"
#include "ParticleSingleDerivedHistos.hpp"
#include "ParticlePair3DHistos.hpp"
#include "ParticlePair3DDerivedHistos.hpp"

namespace CAP
{
//!
//! Two-Particle 3D Correlation Function HistogramGroup
//!
class ParticlePair3DBfHistos : public  ParticlePair3DDerivedHistos
{
public:

  ParticlePair3DBfHistos();
  ParticlePair3DBfHistos(const ParticlePair3DBfHistos & group);
  ParticlePair3DBfHistos & operator=(const ParticlePair3DBfHistos & group);
  virtual ~ParticlePair3DBfHistos() {}
  virtual void configure(const String & taskName,
                               const String & objectType,
                               const Configuration & configuration,
                               unsigned int index);
  virtual void loadFrom(TFile & inputFile);
  virtual void create();
  virtual void calculateBfHistograms(ParticlePair3DDerivedHistos & pair12B,
                                     ParticlePair3DDerivedHistos & pair1B2B,
                                     ParticlePair3DDerivedHistos & pair1B2,
                                     ParticlePair3DDerivedHistos & pair12);
  virtual void setMasses(double m1, double m2);
  virtual void cloneBF(const ParticlePair3DBfHistos & source);

protected:

  TH1 * h_B12_Qinv;
  TH1 * h_B12_DeltaPs;
  TH1 * h_B12_DeltaPo;
  TH1 * h_B12_DeltaPl;

  TH1 * h_B21_Qinv;
  TH1 * h_B21_DeltaPs;
  TH1 * h_B21_DeltaPo;
  TH1 * h_B21_DeltaPl;

  TH1 * h_Bs_Qinv;
  TH1 * h_Bs_DeltaPs;
  TH1 * h_Bs_DeltaPo;
  TH1 * h_Bs_DeltaPl;

  double mass1;
  double mass2;

  ClassDef(ParticlePair3DBfHistos,1)

};

} // namespace CAP

#endif /* CAP__ParticlePair3DDerivedHistos */
