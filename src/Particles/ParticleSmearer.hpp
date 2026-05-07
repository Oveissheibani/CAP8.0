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
#ifndef CAP__ParticleSmearer
#define CAP__ParticleSmearer
#include "HistogramGroup.hpp"
#include "Particle.hpp"
#include "MathConstants.hpp"
#include "Configuration.hpp"

namespace CAP
{

class ParticleSmearer : public HistogramGroup
{
public:
  ParticleSmearer();
  ParticleSmearer(const ParticleSmearer & group);
  ParticleSmearer & operator=(const ParticleSmearer & group);
  virtual ~ParticleSmearer(){ }
  virtual void configure(const String & ownerName,
                               const String & typeName,
                               const Configuration & configuration,
                               unsigned int index);
  virtual void create();
  virtual void loadFrom(TFile & inputFile);
  virtual void smear(const Particle & source, Particle & target);
  virtual void cloneB(const ParticleSmearer & source);

protected:

  int    n_nbins;
  int    pt_nbins;
  int    phi_nbins;
  int    eta_nbins;
  double pt_min;
  double pt_max;
  double phi_min;
  double phi_max;
  double eta_min;
  double eta_max;

  TH2 * h_pt_bias;
  TH2 * h_pt_rms;
  TH2 * h_eta_bias;
  TH2 * h_eta_rms;
  TH2 * h_phi_bias;
  TH2 * h_phi_rms;
  std::vector<int> pids;
  std::vector<TH2*> h_prob_species;
  unsigned int nSpecies;

  ClassDef(ParticleSmearer,0)

};

} // namespace CAP

#endif /* CAP__ParticleSmearer  */



