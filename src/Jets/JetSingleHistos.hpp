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
#ifndef CAP__JetSingleHistos
#define CAP__JetSingleHistos
#include "HistogramGroup.hpp"
#include "fastjet/ClusterSequence.hh"
#include "MathConstants.hpp"
#include "ParticleDb.hpp"

using namespace fastjet;

namespace CAP
{

//!
//!Single particle histograms of particles within jets.
//!
class JetSingleHistos : public HistogramGroup
{
public:

  friend class JetSingleHistosDerived;
  friend class JetPairHistosDerived;

	JetSingleHistos();
  JetSingleHistos(const JetSingleHistos & source);
  JetSingleHistos & operator=(const JetSingleHistos & rhs);
	~JetSingleHistos(){}
  virtual void configure(const String & taskName,
                               const String & objectType,
                               const Configuration & configuration,
                               unsigned int index);
  virtual void create();
  virtual void loadFrom(TFile & inputFile);
  virtual void fill(PseudoJet& J);

  protected:

  virtual void  cloneB(const JetSingleHistos & group);

  ParticleDb * particleTypeList;

  TH1 *h_jet_n1_pt;    // pt
  TH1 *h_jet_n1_phi;   // phi - azimuth relative to z axis
  TH1 *h_jet_n1_eta;   // rapidity or eta
  TH1 *h_jet_n1_th;    // angle relative to jet axis
  TH1 *h_jet_n1_jt;    // particle momentum perp to jet axis.
  TH1 *h_jet_n1_z;     // part_p/jet_p

  TH1 *h_jet_n1p_pt;    // pt
  TH1 *h_jet_n1p_phi;   // phi - azimuth relative to z axis
  TH1 *h_jet_n1p_eta;   // rapidity or eta
  TH1 *h_jet_n1p_th;    // angle relative to jet axis
  TH1 *h_jet_n1p_jt;    // particle momentum perp to jet axis.
  TH1 *h_jet_n1p_z;     // part_p/jet_p

  TH1 *h_jet_n1m_pt;    // pt
  TH1 *h_jet_n1m_phi;   // phi - azimuth relative to z axis
  TH1 *h_jet_n1m_eta;   // rapidity or eta
  TH1 *h_jet_n1m_th;    // angle relative to jet axis
  TH1 *h_jet_n1m_jt;    // particle momentum perp to jet axis.
  TH1 *h_jet_n1m_z;     // part_p/jet_p

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

  ClassDef(JetSingleHistos,0)

};

} // namespace CAP
#endif // JetSingleHistos_HPP
