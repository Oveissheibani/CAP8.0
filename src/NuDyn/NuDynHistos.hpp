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
#ifndef CAP__NuDynHistos
#define CAP__NuDynHistos
#include "HistogramGroup.hpp"
#include "ParticleFilter.hpp"

namespace CAP
{


//!
//! Calculate lowest moments up to order 4
//! The calculation is carried out for N distinct types (species) of particles (N distinct filters)
//!
//! Let the species be: a, b, c, etc
//! First order moments:  f_1(a), f_1(b), f_1(c), etc
//! Second order moments: f_2(a,a), f_2(a,b), f_2(a,c), .., f_2(b,b), f_2(b,c), etc
//! Third order moments: f_3(a,a,a), f_3(a,a,b), f_3(a,a,c), f(a,a,d), f(a,b,b), etc
//! Fourth order moments: f_4(a,a,a,a), f_(a,a,a,b), f_4(a,a,b,b), etc
//!
//!// ================================================================================
// Naming convention
// ================================================================================
// fk_i     : factorial moments of order "k" for particle "i" (i.e., accepted by filter 1)
// Fk_i.j  : factorial cumulants of order "k" for particles "i, .., j" (i.e., accepted by filter i.j)
// rk_i     : ratio of factorial moments of order "k" for particle "i" (i.e., accepted by filter 1)
//            to product of f1_i
//



class NuDynDerivedHistos;

class NuDynHistos : public HistogramGroup
{
public:

  NuDynHistos();
  NuDynHistos(const NuDynHistos & source);
  NuDynHistos & operator=(const NuDynHistos & rhs);
  virtual ~NuDynHistos(){ }

  virtual void configure(const String & taskName,
                               const String & objectType,
                               const Configuration & configuration,
                               unsigned int index);
  virtual void create();
  virtual void loadFrom(TFile & inputFile);
  virtual void fill(double mult, double n1_1, double n1_2, double n2_11, double n2_12, double n2_22);
  virtual void scaleObject(double factor);

  friend NuDynDerivedHistos;

protected:

  void cloneB(const NuDynHistos & source);


  TH1 * h_evt;
  TProfile* h_f1_1_evt;
  TProfile* h_f1_2_evt;
  TProfile* h_f2_11_evt;
  TProfile* h_f2_12_evt;
  TProfile* h_f2_22_evt;

  String evtName;
  int evt_nbins;
  double evt_min;
  double evt_max;


  ClassDef(NuDynHistos,0)
};

} // namespace CAP

#endif /* CAP__NuDynHistos  */



