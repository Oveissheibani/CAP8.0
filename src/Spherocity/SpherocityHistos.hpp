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
#ifndef CAP__SpherocityHistos
#define CAP__SpherocityHistos
#include "HistogramGroup.hpp"
#include "ParticleFilter.hpp"

namespace CAP
{
      
class SpherocityHistos: public HistogramGroup
{
public:
  SpherocityHistos();
  SpherocityHistos(const SpherocityHistos & group);
  SpherocityHistos & operator=(const SpherocityHistos & group);
  virtual ~SpherocityHistos() {}
  virtual void configure(const String & taskName,
                               const String & objectType,
                               const Configuration & configuration,
                               unsigned int index);
  virtual void create();
  virtual void loadFrom(TFile & file);
  virtual void fill(double s0, double s1, double weight);

protected:

  void cloneB(const SpherocityHistos & group);

  bool  fillS0;
  bool  fillS1;
  bool  fillS1VsS0;
  int spherocity_nbins;
  double spherocity_min;
  double spherocity_max;

  TH1 * h_s0;
  TH1 * h_s1;
  TH2 * h_s1VsS0;

  ClassDef(SpherocityHistos, 1);
};

} // namespace CAP


#endif
