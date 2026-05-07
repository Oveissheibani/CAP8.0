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
#ifndef CAP__GlobalDerivedHistos
#define CAP__GlobalDerivedHistos
#include "HistogramGroup.hpp"
#include "ParticleFilter.hpp"
#include "GlobalHistos.hpp"

namespace CAP
{

class GlobalDerivedHistos : public GlobalHistos
{
public://

  GlobalDerivedHistos();
  virtual ~GlobalDerivedHistos() {} 

  virtual void configure(const String & taskName,
                               const String & objectType,
                               const Configuration & configuration,
                               unsigned int index);
  virtual void create();
  virtual void loadFrom(TFile & inputFile);
  virtual void calculateDerivedHistograms(const GlobalHistos & baseHistos);

protected:
  
  void cloneD(const GlobalDerivedHistos & group);

  TH1* h_ptAvgIncVsN; //! Inclusive average pT vs multiplicity computed from ratio of ptSum profile average by average number of particles at given mul

  ClassDef(GlobalDerivedHistos,0)

};

}

#endif /* CAP__GlobalDerivedHistos  */



