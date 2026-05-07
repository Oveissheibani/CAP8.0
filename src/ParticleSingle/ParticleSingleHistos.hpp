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
#ifndef CAP__ParticleSingleHistos
#define CAP__ParticleSingleHistos
#include "HistogramGroup.hpp"
#include "Particle.hpp"
#include "MathConstants.hpp"

namespace CAP
{

class ParticleSingleHistos : public HistogramGroup
{
public:
  ParticleSingleHistos();
  ParticleSingleHistos(const ParticleSingleHistos & source);
  ParticleSingleHistos & operator=(const ParticleSingleHistos & rhs);
  virtual ~ParticleSingleHistos(){ }
  virtual void cloneB(const ParticleSingleHistos & source);
  virtual void configure(const String & taskName,
                               const String & objectType,
                               const Configuration & configuration,
                               unsigned int index);
  virtual void create();
  virtual void loadFrom(TFile & inputFile);
  virtual void fill(Particle & particle, double weight);
  virtual void fillMultiplicity(double nAccepted, double totalEnergy, double weight);
  virtual void scaleObject(double factor);

  ////////////////////////////////////////////////////////////////////////////
  // Data Members - HistogramGroup
  ////////////////////////////////////////////////////////////////////////////
  bool eta_fill;
  bool rapidity_fill;
  bool p2_fill;
  bool ptVsRapidity_fill;
  bool pid_fill;

  int n1_nbins;
  int pt_nbins;
  int phi_nbins;
  int eta_nbins;
  int rapidity_nbins;
  double n1_min;
  double n1_max;
  double pt_min;
  double pt_max;
  double phi_min;
  double phi_max;
  double eta_min;
  double eta_max;
  double rapidity_minimum;
  double rapidity_maximum;

  TH1 * h_n1;
  TH1 * h_n1_eTotal;
  TH1 * h_n1_pt;
  TH1 * h_n1_ptXS;  // 1/pt dN/dptdy

  TH2 * h_n1_phiEta;
  TH2 * h_spt_phiEta;

  TH2 * h_n1_phiY;
  TH2 * h_spt_phiY;
  TH2 * h_n1_ptY;

  TH1 * h_pdgId;

  ClassDef(ParticleSingleHistos,0)

};

} // namespace CAP

#endif /* CAP__ParticleSingleHistos  */



