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
#ifndef CAP__JetHistos
#define CAP__JetHistos
#include "HistogramGroup.hpp"
#include "fastjet/ClusterSequence.hh"
#include "MathConstants.hpp"
#include "ParticleDb.hpp"

using namespace fastjet;

namespace CAP
{

//class JetHistoDerived;

//!
//!Single particle histograms of particles within jets.
//!
class JetHistos : public HistogramGroup
{
public:
  friend class JetHistosDerived;

  JetHistos();
  JetHistos(const JetHistos & source);
  JetHistos & operator=(const JetHistos & rhs);
  ~JetHistos(){}

  virtual void configure(const String & taskName,
                               const String & objectType,
                               const Configuration & configuration,
                               unsigned int index);

  virtual void create();
  virtual void loadFrom(TFile & inputFile);
  virtual void fill(PseudoJet& J);
  virtual void scaleHistograms(double value);
  virtual double acceptedJets() const;

protected:

  virtual void   cloneB(const JetHistos & source);

  ParticleDb * particleTypeList;

  //
  // jet global properties
  //
  TH1 *h_jet_n1; // number of jets of the selected type
  TH1 *h_jet_pt;   // pt
  TH1 *h_jet_phi;  // phi
  TH1 *h_jet_eta;   // rapidity or eta
  TH2 *h_jet_etaPt; // rapidity vs pt
  TH1 *h_jet_n;    // multiplicity of constituent particles
  TH1 *h_jet_np;   // multiplicity of +ve constituent particles
  TH1 *h_jet_nm;   // multiplicity of -ve particles
  TH1 *h_jet_n0;   // multiplicity of neutral particles
  TH1 *h_jet_netQ; // multiplicity of neutral particles
  TProfile *p_jet_n;    // multiplicity of constituent particles vs jet p
  TProfile *p_jet_np;   // multiplicity of +ve constituent particles vs jet p
  TProfile *p_jet_nm;   // multiplicity of -ve particles vs jet p
  TProfile *p_jet_n0;   // multiplicity of neutral particles vs jet p
  TProfile *p_jet_npSq; // multiplicity of +ve constituent particles vs jet p
  TProfile *p_jet_nmSq; // multiplicity of -ve particles vs jet p
  TProfile *p_jet_n0Sq; // multiplicity of neutral particles vs jet p
  TProfile *p_jet_npnm; // multiplicity of pairs of +ve and -ve
  TProfile *p_jet_npn0; // multiplicity of pairs of +ve and 0
  TProfile *p_jet_nmn0; // multiplicity of neutral particles vs jet p
  TProfile *p_jet_netQ; // multiplicity of neutral particles vs jet p
  TProfile *p_jet_netQSq; // multiplicity of neutral particles vs jet p

  int jet_n1_nbins;
  int jet_p_nbins;
  int jet_pt_nbins;
  int jet_phi_nbins;
  int jet_eta_nbins;
  int jet_netQ_nbins;
  double jet_n1_min;
  double jet_p_min;
  double jet_pt_min;
  double jet_phi_min;
  double jet_eta_min;
  double jet_netQ_min;
  double jet_n1_max;
  double jet_p_max;
  double jet_pt_max;
  double jet_phi_max;
  double jet_eta_max;
  double jet_netQ_max;

  ClassDef(JetHistos,0)

};

} // namespace CAP
#endif // JetHistos_HPP
