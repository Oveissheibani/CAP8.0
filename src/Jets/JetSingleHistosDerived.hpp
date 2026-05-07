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
#ifndef CAP__JetSingleHistosDerived
#define CAP__JetSingleHistosDerived
#include "JetSingleHistos.hpp"
#include "MathConstants.hpp"

using namespace fastjet;

namespace CAP
{

//!
//!Single particle histograms of particles within jets.
//!
class JetSingleHistosDerived : public JetSingleHistos
{
public:
  JetSingleHistosDerived();
  JetSingleHistosDerived(const JetSingleHistosDerived & group);
  JetSingleHistosDerived & operator=(const JetSingleHistosDerived & group);
	~JetSingleHistosDerived(){}
  virtual void configure(const String & taskName,
                               const String & objectType,
                               const Configuration & configuration,
                               unsigned int index);
  virtual void create();
  virtual void loadFrom(TFile & inputFile);
  virtual void calculateDerivedHistograms(const JetSingleHistos & jetSingleHistos);

protected:

  virtual void cloneD(const JetSingleHistosDerived & source);

  TH1 * h_jet_n1d_pt;
  TH1 * h_jet_n1d_phi;
  TH1 * h_jet_n1d_eta;
  TH1 * h_jet_n1d_th;
  TH1 * h_jet_n1d_jt;
  TH1 * h_jet_n1d_z;

  TH1 * h_jet_n1r_pt;
  TH1 * h_jet_n1r_phi;
  TH1 * h_jet_n1r_eta;
  TH1 * h_jet_n1r_th;
  TH1 * h_jet_n1r_jt;
  TH1 * h_jet_n1r_z;

  int pt_nbins;
  int phi_nbins;
  int eta_nbins;
  int jt_nbins;
  int th_nbins;
  int z_nbins;
  double pt_min;
  double pt_max;
  double phi_min;
  double phi_max;
  double eta_min;
  double eta_max;
  double jt_min;
  double jt_max;
  double th_min;
  double th_max;
  double z_min;
  double z_max;

  ClassDef(JetSingleHistosDerived,0)

};

} // namespace CAP
#endif // JetSingleHistos_HPP
