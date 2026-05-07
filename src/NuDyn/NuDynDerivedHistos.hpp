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
#ifndef CAP__NuDynDerivedHistos
#define CAP__NuDynDerivedHistos
#include "HistogramGroup.hpp"
#include "NuDynHistos.hpp"
#include "ParticleFilter.hpp"

namespace CAP
{

class NuDynDerivedHistos : public NuDynHistos
{
public:

  NuDynDerivedHistos();
  NuDynDerivedHistos(const NuDynDerivedHistos & source);
  NuDynDerivedHistos & operator=(const NuDynDerivedHistos & rhs);
  virtual ~NuDynDerivedHistos() {}
  virtual void configure(const String & taskName,
                               const String & objectType,
                               const Configuration & configuration,
                               unsigned int index);
  virtual void create();
  virtual void loadFrom(TFile & inputFile);
  virtual void calculateDerivedHistograms(const NuDynHistos & baseHistos);

protected:

  void cloneD(const NuDynDerivedHistos & group);
  TH1 * h_F2_11_evt;
  TH1 * h_F2_12_evt;
  TH1 * h_F2_22_evt;
  TH1 * h_R2_11_evt;
  TH1 * h_R2_12_evt;
  TH1 * h_R2_22_evt;
  TH1 * h_nudyn_evt;

  ClassDef(NuDynDerivedHistos,0)
};

} // namespace CAP

#endif /* CAP__NuDynDerivedHistos  */



