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
#ifndef CAP__PtPtHistos
#define CAP__PtPtHistos
#include "HistogramGroup.hpp"
#include "ParticleFilter.hpp"
#include "Configuration.hpp"

namespace CAP
{

class PtPtDerivedHistos;

class PtPtHistos : public HistogramGroup
{
public:

  PtPtHistos();
  PtPtHistos(const PtPtHistos & source);
  PtPtHistos & operator=(const PtPtHistos & rhs);
  virtual ~PtPtHistos(){ }
  virtual void configure(const String & taskName,
                               const String & objectType,
                               const Configuration & configuration,
                               unsigned int index);
  virtual void create();
  virtual void loadFrom(TFile & inputFile);
  virtual void fill(double mult,vector<double> & moments);
  friend PtPtDerivedHistos;

protected:
  void cloneB(const PtPtHistos & group);

  TH1 * h_evt;
  TProfile* h_f1_evt;
  TProfile* h_f2_evt;
  TProfile* h_f3_evt;
  TProfile* h_f4_evt;
  TProfile* h_Q1_evt;
  TProfile* h_Q1Q1_evt;
  TProfile* h_Q1Q1Q1_evt;
  TProfile* h_Q1Q1Q1Q1_evt;
  TProfile* h_Q2_evt;
  TProfile* h_Q2Q1_evt;
  TProfile* h_Q2Q1Q1_evt;
  TProfile* h_Q2Q2_evt;
  TProfile* h_Q3_evt;
  TProfile* h_Q3Q1_evt;
  TProfile* h_Q4_evt;


  String evtName;
  int    evt_nbins;
  double evt_min;
  double evt_max;


  ClassDef(PtPtHistos,0)
};

} // namespace CAP

#endif /* CAP__PtPtHistos  */



