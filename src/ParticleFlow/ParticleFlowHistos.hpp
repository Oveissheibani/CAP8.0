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
#ifndef CAP__ParticleFlowHistos
#define CAP__ParticleFlowHistos
#include "HistogramGroup.hpp"
#include "Particle.hpp"
#include "MathConstants.hpp"

namespace CAP
{

class ParticleFlowHistos : public HistogramGroup
{
public:
  ParticleFlowHistos();
  ParticleFlowHistos(const ParticleFlowHistos & source);
  ParticleFlowHistos & operator=(const ParticleFlowHistos & rhs);
  virtual ~ParticleFlowHistos(){ }

  virtual void configure(const String & taskName,
                               const String & objectType,
                               const Configuration & configuration,
                               unsigned int index);
  virtual void create();
  virtual void loadFrom(TFile & inputFile);
  virtual void fill(Particle & particle, double weight);

protected:

  virtual void cloneB(const ParticleFlowHistos & group);

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
  unsigned int phiEta_nbins;
  unsigned int phiEtaPt_nbins;
  unsigned int phiY_nbins;
  unsigned int phiYPt_nbins;
  
  TH1 * h_n1;

  ClassDef(ParticleFlowHistos,0)

};

} // namespace CAP

#endif /* CAP__ParticleFlowHistos  */



