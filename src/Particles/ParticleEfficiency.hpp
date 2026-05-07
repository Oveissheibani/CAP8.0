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
#ifndef CAP__ParticleEfficiency
#define CAP__ParticleEfficiency
#include "Aliases.hpp"
#include "HistogramGroup.hpp"
#include "Particle.hpp"

namespace CAP
{

class ParticleEfficiency
:
public HistogramGroup
{
public:

  ParticleEfficiency();
  ParticleEfficiency(const ParticleEfficiency & source);
  ParticleEfficiency & operator=(const ParticleEfficiency & rhs);
  virtual ~ParticleEfficiency() {}
  virtual void configure(const String & ownerName,
                               const String & typeName,
                               const Configuration & configuration,
                               unsigned int index);
  virtual void create();
  virtual void loadFrom(TFile & inputFile);
  virtual bool accept(const Particle & particle);
  virtual bool accept(const VectorLorentz & momentum);
  virtual double efficiencyAt(const VectorLorentz & momentum);
  virtual double weightAt(const VectorLorentz & momentum);
  virtual void   calculateWeights();

protected:

  void cloneB(const ParticleEfficiency & source);

  TH2 * h_efficiency_etaPt;
  TH2 * h_weight_etaPt;

  int pt_nbins;
  int eta_nbins;
  int phi_nbins;
  double pt_min;
  double pt_max;
  double eta_min;
  double eta_max;
  double phi_min;
  double phi_max;

  ClassDef(ParticleEfficiency,0)
};

}

#endif /* CAP__ParticleEfficiency */

