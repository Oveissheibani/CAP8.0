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
#ifndef CAP__PtPtDerivedHistos
#define CAP__PtPtDerivedHistos
#include "HistogramGroup.hpp"
#include "PtPtHistos.hpp"

namespace CAP
{

class PtPtDerivedHistos : public PtPtHistos
{
public:

  PtPtDerivedHistos();
  PtPtDerivedHistos(const PtPtDerivedHistos & source);
  PtPtDerivedHistos & operator=(const PtPtDerivedHistos & rhs);
  virtual ~PtPtDerivedHistos() {}
  virtual void configure(const String & taskName,
                               const String & objectType,
                               const Configuration & configuration,
                               unsigned int index);

  virtual void create();
  virtual void loadFrom(TFile & inputFile);
  virtual void calculateDerivedHistograms(const PtPtHistos & baseHistos);

protected:
  void cloneD(const PtPtDerivedHistos & group);
  TH1 * h_F1_evt;
  TH1 * h_F2_evt;
  TH1 * h_F3_evt;
  TH1 * h_F4_evt;
  TH1 * h_R2_evt;
  TH1 * h_R3_evt;
  TH1 * h_R4_evt;

  TH1 * h_q1_evt;
  TH1 * h_q1q1_evt;
  TH1 * h_q1q1q1_evt;
  TH1 * h_q1q1q1q1_evt;
  TH1 * h_dq1dq1_evt;
  TH1 * h_dq1dq1dq1_evt;
  TH1 * h_dq1dq1dq1dq1_evt;
  
  vector<ParticleFilter*> particleFilters;
  ClassDef(PtPtDerivedHistos,0)
};

} // namespace CAP

#endif /* CAP__PtPtDerivedHistos  */



