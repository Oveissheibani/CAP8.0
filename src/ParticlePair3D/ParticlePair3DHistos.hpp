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
#ifndef CAP__ParticlePair3DHistos
#define CAP__ParticlePair3DHistos
#include "HistogramGroup.hpp"
#include "Particle.hpp"
#include "ParticleDigit.hpp"



namespace CAP
{
class ParticlePair3DHistos : public HistogramGroup
{
public:

  friend class ParticlePair3DDerivedHistos;

  ParticlePair3DHistos();
  ParticlePair3DHistos(const ParticlePair3DHistos & group);
  ParticlePair3DHistos & operator=(const ParticlePair3DHistos & group);
  virtual ~ParticlePair3DHistos() {}
  virtual void configure(const String & taskName,
                               const String & objectType,
                               const Configuration & configuration,
                               unsigned int index);
  virtual void create();
  virtual void loadFrom(TFile & inputFile);
  virtual void fill(Particle & particle1, Particle & particle2, double weight);
  virtual void fillMultiplicity(double nPairs, double weigh=1.0);
  virtual void cloneB(const ParticlePair3DHistos & source);
  friend class ParticlePair3DDerivedHistos;
  friend class ParticlePair3DBFHistos;

protected:

  int    n2_nbins;
  int    Qinv_nbins;
  int    DeltaPs_nbins;
  int    DeltaPo_nbins;
  int    DeltaPl_nbins;
  int    pt_nbins;
  int    rapidity_nbins;
  double n2_min;
  double n2_max;
  double Qinv_min;
  double Qinv_max;
  double DeltaPs_min;
  double DeltaPs_max;
  double DeltaPo_min;
  double DeltaPo_max;
  double DeltaPl_min;
  double DeltaPl_max;
  double pt_min;
  double pt_max;
  double rapidity_minimum;
  double rapidity_maximum;

  TH1 * h_n2;
  TH1 * h_n2_Qinv;
  TH3 * h_n2_DeltaP;

  ClassDef(ParticlePair3DHistos,0)
};

} // namespace CAP

#endif /* CAP__ParticlePair3DHistos  */

