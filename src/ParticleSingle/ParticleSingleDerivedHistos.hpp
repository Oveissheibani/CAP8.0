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
#ifndef CAP__ParticleSingleDerivedHistos
#define CAP__ParticleSingleDerivedHistos
#include "ParticleSingleHistos.hpp"

namespace CAP
{

class ParticleSingleDerivedHistos
:
public ParticleSingleHistos
{
public:

  ParticleSingleDerivedHistos();
  ParticleSingleDerivedHistos(const ParticleSingleDerivedHistos & group);
  ParticleSingleDerivedHistos & operator=(const ParticleSingleDerivedHistos & group);
  virtual ~ParticleSingleDerivedHistos() {}
  virtual void configure(const String & taskName,
                               const String & objectType,
                               const Configuration & configuration,
                               unsigned int index);
  virtual void create();
  virtual void loadFrom(TFile & inputFile);
  virtual void calculateDerivedHistograms(ParticleSingleHistos & baseHistos);
  friend class ParticlePairDerivedHistos;
  friend class ParticlePairBfHistos;

protected:
  virtual void cloneD(const ParticleSingleDerivedHistos & group);

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

  ClassDef(ParticleSingleDerivedHistos,0)

};

} // namespace CAP

#endif /* CAP__ParticleSingleDerivedHistos  */



