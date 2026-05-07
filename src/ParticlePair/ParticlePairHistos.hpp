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
#ifndef CAP__ParticlePairHistos
#define CAP__ParticlePairHistos
#include "HistogramGroup.hpp"
#include "Particle.hpp"


namespace CAP
{

class ParticlePairHistos : public HistogramGroup
{
public:

  friend class ParticlePairDerivedHistos;

  ParticlePairHistos();
  ParticlePairHistos(const ParticlePairHistos & group);
  ParticlePairHistos & operator=(const ParticlePairHistos & group);
  virtual ~ParticlePairHistos() {}
  virtual void configure(const String & taskName,
                               const String & objectType,
                               const Configuration & configuration,
                               unsigned int index);
  virtual void create();
  virtual void loadFrom(TFile & inputFile);
  virtual void fill(Particle & particle1, Particle & particle2, double weight=1.0);
  virtual void fillMultiplicity(double nPairs, double weigh=1.0);
  virtual void scaleObject(double factor);

  inline int ptBinFor(float v) const
  {
  int index = 0; // indicates a value out of bounds
  if (v<pt_min || v>=pt_max) return index;
  index = 1+int(pt_scale*(v-pt_min));
  return index;
  }

  inline int phiBinFor(float v) const
  {
  int index = 0; // indicates a value out of bounds
  if (v<phi_min || v>=phi_max) return index;
  index = 1+int(phi_scale*(v-phi_min));
  return index;
  }

  inline int getEtaBinFor(float v) const
  {
  int index = 0; // indicates a value out of bounds
  if (v<eta_min || v>=eta_max) return index;
  index = 1+int(eta_scale*(v-eta_min));
  return index;
  }

  inline int getYBinFor(float v) const
  {
  int index = 0; // indicates a value out of bounds
  if (v<rapidity_minimum || v>=rapidity_maximum) return index;
  index = 1+int(rapidity_scale*(v-rapidity_minimum));
  return index;
  }

protected:

  virtual void cloneB(const ParticlePairHistos & source);

  bool eta_fill;
  bool rapidity_fill;
  bool p2_fill;
  int n2_nbins;
  int pt_nbins;
  int phi_nbins;
  int eta_nbins;
  int rapidity_nbins;
  int Dphi_nbins;
  int Deta_nbins;
  int Drapidity_nbins;
  int Dphi_nbins_shft;

  double n2_min,  n2_max;
  double pt_min,  pt_max;
  double phi_min, phi_max;
  double eta_min, eta_max;
  double rapidity_minimum, rapidity_maximum;
  double phi_scale;
  double rapidity_scale;
  double eta_scale;
  double pt_scale;
  double Dphi_width;
  double Dphi_min;
  double Dphi_max;
  double Deta_min;
  double Deta_max;
  double Drapidity_min;
  double Drapidity_max;
  double Dphi_min_shft;
  double Dphi_max_shft;
  double rapidity_maximumAcc;

  TH1 * h_n2;
  TH2 * h_n2_ptpt;
  TH2 * h_n2_etaeta;
  TH2 * h_DptDpt_etaeta;
  TH2 * h_n2_phiphi;
  TH2 * h_DptDpt_phiphi;
  TH2 * h_n2_yY;
  TH2 * h_DptDpt_yY;
  TH2 * h_n2_DetaDphi;
  TH2 * h_DptDpt_DetaDphi;
  TH2 * h_n2_DyDphi;
  TH2 * h_DptDpt_DyDphi;

  ClassDef(ParticlePairHistos,0)
};

} // namespace CAP

#endif /* CAP__ParticlePairHistos  */
