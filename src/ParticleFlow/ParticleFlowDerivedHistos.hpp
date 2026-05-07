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
#ifndef CAP__ParticleFlowDerivedHistos
#define CAP__ParticleFlowDerivedHistos
#include "ParticleFlowHistos.hpp"

namespace CAP
{

class ParticleFlowDerivedHistos : public HistogramGroup
{
public:

  ParticleFlowDerivedHistos();
  ParticleFlowDerivedHistos(const ParticleFlowDerivedHistos & group);
  ParticleFlowDerivedHistos & operator=(const ParticleFlowDerivedHistos & group);
  virtual ~ParticleFlowDerivedHistos() {}
  virtual void create();
  virtual void loadFrom(TFile & inputFile,);
  virtual void calculateDerivedHistograms(ParticleFlowHistos & baseHistos);
  void cloneAll(const ParticleFlowDerivedHistos & group);


  ////////////////////////////////////////////////////////////////////////////
  // Data Members - HistogramGroup
  ////////////////////////////////////////////////////////////////////////////
  bool eta_fill;
  bool rapidity_fill;
  bool p2_fill;
  unsigned int n1_nbins;
  float        n1_min;
  float        n1_max;
  unsigned int pt_nbins;
  float        pt_min;
  float        pt_max;
  float        pt_scale;
  unsigned int phi_nbins;
  float        phi_min;
  float        phi_max;
  float        phi_scale;
  unsigned int eta_nbins;
  float        eta_min;
  float        eta_max;
  float        eta_range;
  unsigned int rapidity_nbins;
  float        rapidity_minimum;
  float        rapidity_maximum;
  float        range_y;

  TH1 * h_n1_phi;
  TH1 * h_n1_eta;
  TH1 * h_n1_y;

  TH1 * h_spt_phi;
  TH1 * h_spt_eta;
  TH1 * h_spt_y;
  TH1 * h_avg_pt_phi;
  TH1 * h_avg_pt_eta;
  TH2 * h_avg_pt_phiEta;
  TH1 * h_avg_pt_y;
  TH2 * h_avg_pt_phiY;

  ClassDef(ParticleFlowDerivedHistos,0)

};

} // namespace CAP

#endif /* CAP__ParticleFlowDerivedHistos  */



