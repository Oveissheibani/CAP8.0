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
#ifndef CAP__JetPairMassHistos
#define CAP__JetPairMassHistos
#include "HistogramGroup.hpp"
#include "fastjet/ClusterSequence.hh"
#include "MathConstants.hpp"
#include "ParticleDb.hpp"

using namespace fastjet;

namespace CAP
{

class JetPairMassHistos : public HistogramGroup
{
public:

  friend class JetPairMassHistosDerived;


  JetPairMassHistos();
  JetPairMassHistos(const JetPairMassHistos & source);
  JetPairMassHistos & operator=(const JetPairMassHistos & rhs);
  ~JetPairMassHistos(){}
  virtual void configure(const String & taskName,
                               const String & objectType,
                               const Configuration & configuration,
                               unsigned int index);
  virtual void create();
  virtual void loadFrom(TFile & inputFile);

  virtual void fill(PseudoJet& J1, PseudoJet& J2);
  virtual void scaleHistograms(double scale);
  virtual void setParticleTypeList(ParticleDb * selectedParticleTypeList)
  {
  particleTypeList = selectedParticleTypeList;
  }

protected:

  virtual void cloneB(const JetPairMassHistos & source);

  ParticleDb * particleTypeList;
  
  TH2 *h_jetpair_n2_ptpt;      // jet1 pt vs. jet2 pt
  TH2 *h_jetpair_n2_phiphi;    // jet1 phi vs. jet2 phi
  TH2 *h_jetpair_n2_etaeta;    // jet1 eta vs. jet2 eta
  TH2 *h_jetpair_n2_DetaDphi;  // jet1, jet2 Deta, Dphi
  TH1 *h_jetpair_n2_mass;      // di-jet mass.

  int pt_nbins;
  int phi_nbins;
  int eta_nbins;
  int Deta_nbins;
  int Dphi_nbins;
  int mass_nbins;

  double pt_min;
  double pt_max;
  double phi_min;
  double phi_max;
  double eta_min;
  double eta_max;
  double Deta_min;
  double Deta_max;
  double Dphi_min;
  double Dphi_max;

  double mass_min;
  double mass_max;

  ClassDef(JetPairMassHistos,0)

};

} //  namespace CAP

#endif
