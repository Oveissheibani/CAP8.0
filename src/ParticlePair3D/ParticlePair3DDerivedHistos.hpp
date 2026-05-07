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
#ifndef CAP__ParticlePair3DDerivedHistos
#define CAP__ParticlePair3DDerivedHistos
#include "ParticleSingleDerivedHistos.hpp"
#include "ParticlePair3DHistos.hpp"

namespace CAP
{

//!
//! Two-Particle Correlation Function HistogramGroup
//!
class ParticlePair3DDerivedHistos : public  ParticlePair3DHistos
{
public:

  ParticlePair3DDerivedHistos();
  ParticlePair3DDerivedHistos(const ParticlePair3DDerivedHistos & group);
  ParticlePair3DDerivedHistos & operator=(const ParticlePair3DDerivedHistos & group);
  virtual ~ParticlePair3DDerivedHistos() {}
  virtual void cloneD(const ParticlePair3DDerivedHistos & group);
  virtual void configure(const String & taskName,
                               const String & objectType,
                               const Configuration & configuration,
                               unsigned int index);
  virtual void loadFrom(TFile & inputFile);
  virtual void create();
  virtual void calculateDerivedHistograms(ParticleSingleDerivedHistos & part1DerivedHistos,
                                          ParticleSingleDerivedHistos & part2DerivedHistos,
                                          ParticlePair3DHistos        & pairHistos,
                                          double bincorrection=1.0);
  virtual void setMasses(double m1, double m2);

  TH1 * h_n1n1_Qinv;
  TH3 * h_n1n1_DeltaP;
  TH1 * h_n1n1_DeltaPs;
  TH1 * h_n1n1_DeltaPo;
  TH1 * h_n1n1_DeltaPl;

  TH1 * h_n2_DeltaPs;
  TH1 * h_n2_DeltaPo;
  TH1 * h_n2_DeltaPl;

  
  TH1 * h_c2_Qinv;
  TH3 * h_c2_DeltaP;
  TH1 * h_c2_DeltaPs;
  TH1 * h_c2_DeltaPo;
  TH1 * h_c2_DeltaPl;

  
  TH1 * h_r2_Qinv;
  TH3 * h_r2_DeltaP;
  TH1 * h_r2_DeltaPs;
  TH1 * h_r2_DeltaPo;
  TH1 * h_r2_DeltaPl;

  TH1 * h_a12_Qinv;
  TH1 * h_a12_DeltaPs;
  TH1 * h_a12_DeltaPo;
  TH1 * h_a12_DeltaPl;

  TH1 * h_a21_Qinv;
  TH1 * h_a21_DeltaPs;
  TH1 * h_a21_DeltaPo;
  TH1 * h_a21_DeltaPl;

  TH1 * h_n1_1_pt;
  TH1 * h_n1_2_pt;
  TH1 * h_n1_1_y;
  TH1 * h_n1_2_y;

  TH1 * h_n1_1_pt_mc;
  TH1 * h_n1_2_pt_mc;
  TH1 * h_n1_1_y_mc;
  TH1 * h_n1_2_y_mc;

  TH1 * h_n1r_1_pt_mc;
  TH1 * h_n1r_2_pt_mc;
  TH1 * h_n1r_1_y_mc;
  TH1 * h_n1r_2_y_mc;

  TH2 * h_n1_1_ptY_mc;
  TH2 * h_n1_2_ptY_mc;
  TH2 * h_n1r_1_ptY_mc;
  TH2 * h_n1r_2_ptY_mc;

  double mass1;
  double mass2;

  ClassDef(ParticlePair3DDerivedHistos,1)

};

} // namespace CAP

#endif /* CAP__ParticlePair3DDerivedHistos */
