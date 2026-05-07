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
#ifndef CAP__ParticlePairDerivedHistos
#define CAP__ParticlePairDerivedHistos
#include "MathBasicFunctions.hpp"
#include "ParticleSingleHistos.hpp"
#include "ParticleSingleDerivedHistos.hpp"
#include "ParticlePairHistos.hpp"
#include "HistogramGroup.hpp"

using CAP::MATH::absolute;

namespace CAP
{


//!
//! Two-Particle Correlation Function HistogramGroup
//!
class ParticlePairDerivedHistos : public  ParticlePairHistos
{
public:

  ParticlePairDerivedHistos();
  ParticlePairDerivedHistos(const ParticlePairDerivedHistos & source);
  ParticlePairDerivedHistos & operator=(const ParticlePairDerivedHistos & rhs);
  virtual ~ParticlePairDerivedHistos() {}
  virtual void configure(const String & taskName,
                               const String & objectType,
                               const Configuration & configuration,
                               unsigned int index);
  virtual void loadFrom(TFile & inputFile);
  virtual void create();
  virtual void calculateDerivedHistograms(ParticleSingleDerivedHistos & part1DerivedHistos,
                                          ParticleSingleDerivedHistos & part2DerivedHistos,
                                          ParticlePairHistos          & pairHistos);
  void calculateOmegaFactor(TH2 * h_DyDphi, vector<double> & omegaFactor);
  void calculateAverageYbar(TH2 * h_DyDphi, TH2 * h_DyDphi_Avg, vector<double> & omegaFactor);
  friend class ParticlePairBfHistos;

protected:

  virtual void cloneD(const ParticlePairDerivedHistos & source);

  TH2* h_n1n1_ptpt;
  TH2* h_C2_ptpt;
  TH2* h_C2N_ptpt;
  TH2* h_R2_ptpt;

  TH2* h_n1n1_phiphi;
  TH2* h_C2_phiphi;
  TH2* h_C2N_phiphi;
  TH2* h_R2_phiphi;
  TH2* h_P2_phiphi;
  TH2* h_G2_phiphi;
  TH2* h_DptDpt_phiphi;
  TH2* h_pt1pt1_phiphi;

  TH2* h_n1n1_etaeta;
  TH2* h_C2_etaeta;
  TH2* h_C2N_etaeta;
  TH2* h_R2_etaeta;
  TH2* h_G2_etaeta;
  TH2* h_P2_etaeta;
  TH2* h_DptDpt_etaeta;
  TH2* h_pt1pt1_etaeta;

  TH2* h_n1n1_yY;
  TH2* h_C2_yY;
  TH2* h_C2N_yY;
  TH2* h_R2_yY;
  TH2* h_G2_yY;
  TH2* h_P2_yY;
  TH2* h_DptDpt_yY;
  TH2* h_pt1pt1_yY;

  TH2* h_n1n1_DetaDphi;
  TH2* h_C2_DetaDphi;
  TH2* h_C2N_DetaDphi;
  TH2* h_C2O_DetaDphi;
  TH2* h_R2_DetaDphi;
  TH2* h_P2_DetaDphi;
  TH2* h_G2_DetaDphi;
  TH2* h_DptDpt_DetaDphi;
  TH2* h_pt1pt1_DetaDphi;

  TH2* h_n1n1_DetaDphi_shft;
  TH2* h_C2_DetaDphi_shft;
  TH2* h_C2N_DetaDphi_shft;
  TH2* h_C2O_DetaDphi_shft;
  TH2* h_R2_DetaDphi_shft;
  TH2* h_P2_DetaDphi_shft;
  TH2* h_G2_DetaDphi_shft;
  TH2* h_DptDpt_DetaDphi_shft;

  TH2* h_n1n1_DyDphi;
  TH2* h_C2_DyDphi;
  TH2* h_C2N_DyDphi;
  TH2* h_C2O_DyDphi;
  TH2* h_R2_DyDphi;
  TH2* h_P2_DyDphi;
  TH2* h_G2_DyDphi;
  TH2* h_DptDpt_DyDphi;
  TH2* h_pt1pt1_DyDphi;

  TH2* h_n1n1_DyDphi_shft;
  TH2* h_C2_DyDphi_shft;
  TH2* h_C2N_DyDphi_shft;
  TH2* h_C2O_DyDphi_shft;
  TH2* h_R2_DyDphi_shft;
  TH2* h_P2_DyDphi_shft;
  TH2* h_G2_DyDphi_shft;
  TH2* h_DptDpt_DyDphi_shft;
  
  ClassDef(ParticlePairDerivedHistos,1)

};

} // namespace CAP

#endif /* CAP__ParticlePairDerivedHistos */
