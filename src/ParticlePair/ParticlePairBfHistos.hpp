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
#ifndef CAP__ParticlePairBfHistos
#define CAP__ParticlePairBfHistos
#include "ParticlePairDerivedHistos.hpp"
#include "HistogramGroup.hpp"

namespace CAP
{

//!
//! Two-Particle Balance Functions
//!
class ParticlePairBfHistos : public  ParticlePairDerivedHistos
{
public:

  ParticlePairBfHistos();
  ParticlePairBfHistos(const ParticlePairBfHistos & source);
  ParticlePairBfHistos & operator=(const ParticlePairBfHistos & rhs);
  virtual ~ParticlePairBfHistos() {}
  virtual void configure(const String & taskName,
                               const String & objectType,
                               const Configuration & configuration,
                               unsigned int index);
  void create();
  void loadFrom(TFile & inputFile);
  
  virtual void calculateBfHistograms(ParticlePairDerivedHistos & pair12B,
                                     ParticlePairDerivedHistos & pair1B2B,
                                     ParticlePairDerivedHistos & pair1B2,
                                     ParticlePairDerivedHistos & pair12);

protected:

  virtual void cloneBF(const ParticlePairBfHistos & source);

  TH2* h_C2_CD_ptpt;
  TH2* h_C2_CI_ptpt;
  TH2* h_C2N_B12_ptpt;
  TH2* h_C2N_B21_ptpt;
  TH2* h_C2N_Bs_ptpt;
  TH2* h_R2_B12_ptpt;
  TH2* h_R2_B21_ptpt;
  TH2* h_R2_Bs_ptpt;

  TH2* h_C2_CD_phiphi;
  TH2* h_C2_CI_phiphi;
  TH2* h_C2N_B12_phiphi;
  TH2* h_C2N_B21_phiphi;
  TH2* h_C2N_Bs_phiphi;
  TH2* h_R2_B12_phiphi;
  TH2* h_R2_B21_phiphi;
  TH2* h_R2_Bs_phiphi;

  TH2* h_C2_CD_etaeta;
  TH2* h_C2_CI_etaeta;
  TH2* h_C2N_B12_etaeta;
  TH2* h_C2N_B21_etaeta;
  TH2* h_C2N_Bs_etaeta;
  TH2* h_R2_B12_etaeta;
  TH2* h_R2_B21_etaeta;
  TH2* h_R2_Bs_etaeta;

  TH2* h_C2_CD_yY;
  TH2* h_C2_CI_yY;
  TH2* h_C2N_B12_yY;
  TH2* h_C2N_B21_yY;
  TH2* h_C2N_Bs_yY;
  TH2* h_R2_B12_yY;
  TH2* h_R2_B21_yY;
  TH2* h_R2_Bs_yY;

  TH2* h_C2_CD_DetaDphi;
  TH2* h_C2_CI_DetaDphi;
  TH2* h_C2N_B12_DetaDphi;
  TH2* h_C2N_B21_DetaDphi;
  TH2* h_C2N_Bs_DetaDphi;
  TH2* h_R2_B12_DetaDphi;
  TH2* h_R2_B21_DetaDphi;
  TH2* h_R2_Bs_DetaDphi;

  TH2* h_C2_CD_DyDphi;
  TH2* h_C2_CI_DyDphi;
  TH2* h_C2N_B12_DyDphi;
  TH2* h_C2N_B21_DyDphi;
  TH2* h_C2N_Bs_DyDphi;
  TH2* h_R2_B12_DyDphi;
  TH2* h_R2_B21_DyDphi;
  TH2* h_R2_Bs_DyDphi;

  ClassDef(ParticlePairBfHistos,1)

};

} // namespace CAP

#endif /* CAP__ParticlePairBfHistos */
