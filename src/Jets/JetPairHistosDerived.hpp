/* **********************************************************************
 * Copyright (C) 2019-2024, Claude Pruneau, Akash Raj
 * All rights reserved.
 *
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 *
 * Author: Claude Pruneau, Akash Raj,  Nov 2024
 *
 * *********************************************************************/
#ifndef CAP__JetPairHistosDerived
#define CAP__JetPairHistosDerived
#include "JetSingleHistos.hpp"
#include "JetPairHistos.hpp"
#include "MathConstants.hpp"

//using namespace fastjet;

namespace CAP
{


class JetPairHistosDerived : public JetPairHistos
{
public:
  JetPairHistosDerived();
  JetPairHistosDerived(const JetPairHistosDerived & source);
  JetPairHistosDerived & operator=(const JetPairHistosDerived & rhs);
  ~JetPairHistosDerived(){}
  virtual void configure(const String & taskName,
                               const String & objectType,
                               const Configuration & configuration,
                               unsigned int index);
  virtual void create();
  virtual void loadFrom(TFile & inputFile);
  virtual void calculateDerivedHistograms(const JetSingleHistos & singleHistos, const JetPairHistos & pairHistos);


protected:

  virtual void cloneD(const JetPairHistosDerived & source);

  TH2 *  h_jet_a2pp_ptpt;
  TH2 *  h_jet_a2pp_phiphi;
  TH2 *  h_jet_a2pp_etaeta;
  TH2 *  h_jet_a2pp_thth;
  TH2 *  h_jet_a2pp_jtjt;
  TH2 *  h_jet_a2pp_zz;

  TH2 *  h_jet_a2mp_ptpt;
  TH2 *  h_jet_a2mp_phiphi;
  TH2 *  h_jet_a2mp_etaeta;
  TH2 *  h_jet_a2mp_thth;
  TH2 *  h_jet_a2mp_jtjt;
  TH2 *  h_jet_a2mp_zz;

  TH2 *  h_jet_a2pm_ptpt;
  TH2 *  h_jet_a2pm_phiphi;
  TH2 *  h_jet_a2pm_etaeta;
  TH2 *  h_jet_a2pm_thth;
  TH2 *  h_jet_a2pm_jtjt;
  TH2 *  h_jet_a2pm_zz;

  TH2 *  h_jet_a2mm_ptpt;
  TH2 *  h_jet_a2mm_phiphi;
  TH2 *  h_jet_a2mm_etaeta;
  TH2 *  h_jet_a2mm_thth;
  TH2 *  h_jet_a2mm_jtjt;
  TH2 *  h_jet_a2mm_zz;

  TH2 *  h_jet_b2pm_ptpt;
  TH2 *  h_jet_b2pm_phiphi;
  TH2 *  h_jet_b2pm_etaeta;
  TH2 *  h_jet_b2pm_thth;
  TH2 *  h_jet_b2pm_jtjt;
  TH2 *  h_jet_b2pm_zz;

  TH2 *  h_jet_b2mp_ptpt;
  TH2 *  h_jet_b2mp_phiphi;
  TH2 *  h_jet_b2mp_etaeta;
  TH2 *  h_jet_b2mp_thth;
  TH2 *  h_jet_b2mp_jtjt;
  TH2 *  h_jet_b2mp_zz;

  TH2 *  h_jet_b2s_ptpt;
  TH2 *  h_jet_b2s_phiphi;
  TH2 *  h_jet_b2s_etaeta;
  TH2 *  h_jet_b2s_thth;
  TH2 *  h_jet_b2s_jtjt;
  TH2 *  h_jet_b2s_zz;

  ClassDef(JetPairHistosDerived,0)

};

} //  namespace CAP

#endif
