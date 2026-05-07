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
#include "ParticlePerformanceHistos.hpp"
#include "NameHelpers.hpp"
#include "PrintHelpers.hpp"
#include "RootHistogramHelpers.hpp"
#include "Task.hpp"
#include "NameHelpers.hpp"

ClassImp(CAP::ParticlePerformanceHistos);

namespace CAP
{

  ParticlePerformanceHistos::ParticlePerformanceHistos()
  :
  HistogramGroup(),
  h_n1_dpt(nullptr),
  h_n1_deta(nullptr),
  h_n1_dphi(nullptr),
  h_n1_dy(nullptr),
  p_n1_dptVsPt(nullptr),
  p_n1_dptVsEta(nullptr),
  p_n1_detaVsPt(nullptr),
  p_n1_detaVsEta(nullptr),
  p_n1_dyVsPt(nullptr),
  p_n1_dyVsEta(nullptr),
  p_n1_dphiVsPt(nullptr),
  p_n1_dphiVsEta(nullptr)
  {
  appendClassName("ParticlePerformanceHistos");
  setName("ParticlePerformanceHistos");
  setTitle("ParticlePerformanceHistos");
  }

  void ParticlePerformanceHistos::configure(const String & taskName,
                                            const String & objectType,
                                            const Configuration & configuration,
                                            unsigned int index)
  {
  int pt_nbins    = configuration.valueInt(createKey(taskName,objectType,index,"pt_nbins"));
  double pt_min   = configuration.valueDouble(createKey(taskName,objectType,index,"pt_min"));
  double pt_max   = configuration.valueDouble(createKey(taskName,objectType,index,"pt_max"));

  int dpt_nbins   = configuration.valueInt(createKey(taskName,objectType,index,"dpt_nbins"));
  double dpt_min  = configuration.valueDouble(createKey(taskName,objectType,index,"dpt_min"));
  double dpt_max  = configuration.valueDouble(createKey(taskName,objectType,index,"dpt_max"));

  int Dphi_nbins  = configuration.valueInt(createKey(taskName,objectType,index,"Dphi_nbins"));
  double Dphi_min = configuration.valueDouble(createKey(taskName,objectType,index,"Dphi_min"));
  double Dphi_max = configuration.valueDouble(createKey(taskName,objectType,index,,"Dphi_max"));

  int eta_nbins   = configuration.valueInt(createKey(taskName,objectType,index,"eta_nbins"));
  double eta_min  = configuration.valueDouble(createKey(taskName,objectType,index,"eta_min"));
  double eta_max  = configuration.valueDouble(createKey(taskName,objectType,index,"eta_max"));

  int Deta_nbins  = configuration.valueInt(createKey(taskName,objectType,index,"Deta_nbins"));
  double Deta_min = configuration.valueDouble(createKey(taskName,objectType,index,"Deta_min"));
  double Deta_max = configuration.valueDouble(createKey(taskName,objectType,index,"Deta_max"));

  int Drapidity_nbins    = configuration.valueInt(createKey(taskName,objectType,index,"Drapidity_nbins"));
  double Drapidity_min   = configuration.valueDouble(createKey(taskName,objectType,index,"Drapidity_min"));
  double Drapidity_max   = configuration.valueDouble(createKey(taskName,objectType,index,"Drapidity_max"));
  }

  // for now use the same boundaries for eta and y histogram
  void ParticlePerformanceHistos::create()
  {
  if (reportDebug(__FUNCTION__)) { printCR();}
  h_n1_dpt        = createHistogram(createName(_histogramBaseName,"n1_dpt"),  dpt_nbins,  dpt_min,  dpt_max,   "#Delta p_{T}", "N");
  h_n1_deta       = createHistogram(createName(_histogramBaseName,"n1_deta"), Deta_nbins, Deta_min, Deta_max,  "#Delta#eta",   "N");
  h_n1_dphi       = createHistogram(createName(_histogramBaseName,"n1_dphi"), Dphi_nbins, Dphi_min, Dphi_max,  "#Delta#varphi","N");
  h_n1_dy         = createHistogram(createName(_histogramBaseName,"n1_dy"),   Drapidity_nbins,   Drapidity_min,   Drapidity_max,    "#Delta y",     "N");
  p_n1_dptVsPt    = createProfile(createName(_histogramBaseName,"dptVsPt"),   pt_nbins,   pt_min,   pt_max,    "p_{T}","#Delta p_{T}");
  p_n1_detaVsPt   = createProfile(createName(_histogramBaseName,"detaVsPt"),  pt_nbins,   pt_min,   pt_max,    "p_{T}","#Delta#eta");
  p_n1_dyVsPt     = createProfile(createName(_histogramBaseName,"dyVsPt"),    pt_nbins,   pt_min,   pt_max,    "p_{T}","#Delta y");
  p_n1_dphiVsPt   = createProfile(createName(_histogramBaseName,"dphiVsPt"),  pt_nbins,   pt_min,   pt_max,    "p_{T}","#Delta#varphi");
  p_n1_dptVsEta   = createProfile(createName(_histogramBaseName,"dptVsEta"),  eta_nbins,  eta_min,  eta_max,   "#eta","#Delta p_{T}");
  p_n1_detaVsEta  = createProfile(createName(_histogramBaseName,"detaVsEta"), eta_nbins,  eta_min,  eta_max,   "#eta","#Delta#eta");
  p_n1_dyVsEta    = createProfile(createName(_histogramBaseName,"dyVsEta"),   eta_nbins,  eta_min,  eta_max,   "#eta","#Delta y");
  p_n1_dphiVsEta  = createProfile(createName(_histogramBaseName,"dphiVsEta"), eta_nbins,  eta_min,  eta_max,   "#eta","#Delta#varphi");

  }

  void ParticlePerformanceHistos::loadFrom(TFile & inputFile)
  {
  if (reportDebug(__FUNCTION__)) { printCR();}
  h_n1_dpt        = importH1(inputFile, createName(_histogramBaseName,"n1_dpPt"));
  h_n1_deta       = importH1(inputFile, createName(_histogramBaseName,"n1_deta"));
  h_n1_dphi       = importH1(inputFile, createName(_histogramBaseName,"n1_dphi"));
  h_n1_dy         = importH1(inputFile, createName(_histogramBaseName,"n1_dy"));
  p_n1_dptVsPt    = importProfile(inputFile, createName(_histogramBaseName,"dptVsPt"));
  p_n1_detaVsPt   = importProfile(inputFile, createName(_histogramBaseName,"detaVsPt"));
  p_n1_dyVsPt     = importProfile(inputFile, createName(_histogramBaseName,"dyVsPt"));
  p_n1_dphiVsPt   = importProfile(inputFile, createName(_histogramBaseName,"dphiVsPt"));
  p_n1_dptVsEta   = importProfile(inputFile, createName(_histogramBaseName,"dptVsEta"));
  p_n1_detaVsEta  = importProfile(inputFile, createName(_histogramBaseName,"detaVsEta"));
  p_n1_dyVsEta    = importProfile(inputFile, createName(_histogramBaseName,"dyVsEta"));
  p_n1_dphiVsEta  = importProfile(inputFile, createName(_histogramBaseName,"dphiVsEta"));
  }

  //!
  //! Fiil  histograms of this class with differences between reco and gen particles..
  //!
  void ParticlePerformanceHistos::fill(Particle & recoParticle, double weight)
  {
  Particle & genParticle  = *recoParticle.truth();
  VectorLorentz & recoMomentum = recoParticle.momentum();
  VectorLorentz & genMomentum  = genParticle.momentum();
  double recoPt   = recoMomentum.perp();
  double recoEta  = recoMomentum.pseudorapidity();
  double recoY    = recoMomentum.Y();
  double recoPhi  = recoMomentum.phi();
  double genPt    = genMomentum.perp();
  double genPhi   = genMomentum.phi();
  double genEta   = genMomentum.pseudorapidity();
  double genY     = genMomentum.Y();
  double dpt      = recoPt  - genPt;
  double dphi     = recoPhi - genPhi;
  double deta     = recoEta - genEta;
  double dy       = recoY   - genY;
  h_n1_dpt  ->Fill(dpt,weight);
  h_n1_deta ->Fill(deta,weight);
  h_n1_dphi ->Fill(dphi,weight);
  h_n1_dy   ->Fill(dy,weight);

  p_n1_dptVsPt  ->Fill(genPt, dpt,  weight);
  p_n1_detaVsPt ->Fill(genPt, deta, weight);
  p_n1_dyVsPt   ->Fill(genPt, dy,   weight);
  p_n1_dphiVsPt ->Fill(genPt, dphi, weight);
  p_n1_dptVsEta ->Fill(genEta,dpt,  weight);
  p_n1_detaVsEta->Fill(genEta,deta, weight);
  p_n1_dyVsEta  ->Fill(genEta,dy,   weight);
  p_n1_dphiVsEta->Fill(genEta,dphi, weight);
  }

} // namespace CAP
