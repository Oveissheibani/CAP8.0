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
#include "ParticlePairDerivedHistos.hpp"
#include "Task.hpp"
#include "NameHelpers.hpp"
#include "PrintHelpers.hpp"
#include "RootHistogramHelpers.hpp"
#include "Task.hpp"

ClassImp(CAP::ParticlePairDerivedHistos);

namespace CAP
{

ParticlePairDerivedHistos::ParticlePairDerivedHistos()
:
ParticlePairHistos(),
h_n1n1_ptpt(nullptr),
h_C2_ptpt(nullptr),
h_C2N_ptpt(nullptr),
h_R2_ptpt(nullptr),
h_n1n1_phiphi(nullptr),
h_C2_phiphi(nullptr),
h_C2N_phiphi(nullptr),
h_R2_phiphi(nullptr),
h_P2_phiphi(nullptr),
h_G2_phiphi(nullptr),
h_DptDpt_phiphi(nullptr),
h_pt1pt1_phiphi(nullptr),
h_n1n1_etaeta(nullptr),
h_C2_etaeta(nullptr),
h_C2N_etaeta(nullptr),
h_R2_etaeta(nullptr),
h_G2_etaeta(nullptr),
h_P2_etaeta(nullptr),
h_DptDpt_etaeta(nullptr),
h_pt1pt1_etaeta(nullptr),
h_n1n1_yY(nullptr),
h_C2_yY(nullptr),
h_C2N_yY(nullptr),
h_R2_yY(nullptr),
h_G2_yY(nullptr),
h_P2_yY(nullptr),
h_DptDpt_yY(nullptr),
h_pt1pt1_yY(nullptr),
h_n1n1_DetaDphi(nullptr),
h_C2_DetaDphi(nullptr),
h_C2N_DetaDphi(nullptr),
h_C2O_DetaDphi(nullptr),
h_R2_DetaDphi(nullptr),
h_P2_DetaDphi(nullptr),
h_G2_DetaDphi(nullptr),
h_DptDpt_DetaDphi(nullptr),
h_pt1pt1_DetaDphi(nullptr),
h_n1n1_DetaDphi_shft(nullptr),
h_C2_DetaDphi_shft(nullptr),
h_C2N_DetaDphi_shft(nullptr),
h_C2O_DetaDphi_shft(nullptr),
h_R2_DetaDphi_shft(nullptr),
h_P2_DetaDphi_shft(nullptr),
h_G2_DetaDphi_shft(nullptr),
h_DptDpt_DetaDphi_shft(nullptr),
h_n1n1_DyDphi(nullptr),
h_C2_DyDphi(nullptr),
h_C2N_DyDphi(nullptr),
h_C2O_DyDphi(nullptr),
h_R2_DyDphi(nullptr),
h_P2_DyDphi(nullptr),
h_G2_DyDphi(nullptr),
h_DptDpt_DyDphi(nullptr),
h_pt1pt1_DyDphi(nullptr),
h_n1n1_DyDphi_shft(nullptr),
h_C2_DyDphi_shft(nullptr),
h_C2N_DyDphi_shft(nullptr),
h_C2O_DyDphi_shft(nullptr),
h_R2_DyDphi_shft(nullptr),
h_P2_DyDphi_shft(nullptr),
h_G2_DyDphi_shft(nullptr),
h_DptDpt_DyDphi_shft(nullptr)
{
  appendClassName("ParticlePairDerivedHistos");
  setName("ParticlePairDerivedHistos");
  setTitle("ParticlePairDerivedHistos");
}

ParticlePairDerivedHistos::ParticlePairDerivedHistos(const ParticlePairDerivedHistos & source)
:
ParticlePairHistos(source),
h_n1n1_ptpt(nullptr),
h_C2_ptpt(nullptr),
h_C2N_ptpt(nullptr),
h_R2_ptpt(nullptr),
h_n1n1_phiphi(nullptr),
h_C2_phiphi(nullptr),
h_C2N_phiphi(nullptr),
h_R2_phiphi(nullptr),
h_P2_phiphi(nullptr),
h_G2_phiphi(nullptr),
h_DptDpt_phiphi(nullptr),
h_pt1pt1_phiphi(nullptr),
h_n1n1_etaeta(nullptr),
h_C2_etaeta(nullptr),
h_C2N_etaeta(nullptr),
h_R2_etaeta(nullptr),
h_G2_etaeta(nullptr),
h_P2_etaeta(nullptr),
h_DptDpt_etaeta(nullptr),
h_pt1pt1_etaeta(nullptr),
h_n1n1_yY(nullptr),
h_C2_yY(nullptr),
h_C2N_yY(nullptr),
h_R2_yY(nullptr),
h_G2_yY(nullptr),
h_P2_yY(nullptr),
h_DptDpt_yY(nullptr),
h_pt1pt1_yY(nullptr),
h_n1n1_DetaDphi(nullptr),
h_C2_DetaDphi(nullptr),
h_C2N_DetaDphi(nullptr),
h_C2O_DetaDphi(nullptr),
h_R2_DetaDphi(nullptr),
h_P2_DetaDphi(nullptr),
h_G2_DetaDphi(nullptr),
h_DptDpt_DetaDphi(nullptr),
h_pt1pt1_DetaDphi(nullptr),
h_n1n1_DetaDphi_shft(nullptr),
h_C2_DetaDphi_shft(nullptr),
h_C2N_DetaDphi_shft(nullptr),
h_C2O_DetaDphi_shft(nullptr),
h_R2_DetaDphi_shft(nullptr),
h_P2_DetaDphi_shft(nullptr),
h_G2_DetaDphi_shft(nullptr),
h_DptDpt_DetaDphi_shft(nullptr),
h_n1n1_DyDphi(nullptr),
h_C2_DyDphi(nullptr),
h_C2N_DyDphi(nullptr),
h_C2O_DyDphi(nullptr),
h_R2_DyDphi(nullptr),
h_P2_DyDphi(nullptr),
h_G2_DyDphi(nullptr),
h_DptDpt_DyDphi(nullptr),
h_pt1pt1_DyDphi(nullptr),
h_n1n1_DyDphi_shft(nullptr),
h_C2_DyDphi_shft(nullptr),
h_C2N_DyDphi_shft(nullptr),
h_C2O_DyDphi_shft(nullptr),
h_R2_DyDphi_shft(nullptr),
h_P2_DyDphi_shft(nullptr),
h_G2_DyDphi_shft(nullptr),
h_DptDpt_DyDphi_shft(nullptr)
{
  cloneD(source);
}

ParticlePairDerivedHistos & ParticlePairDerivedHistos::operator=(const ParticlePairDerivedHistos & rhs)
{
  if (this!=&rhs)
  {
    ParticlePairHistos::operator=(rhs);
    cloneD(rhs);
  }
  return *this;
}

void ParticlePairDerivedHistos::cloneD(const ParticlePairDerivedHistos & source)
{
  if (reportDebug(__FUNCTION__)) { printCR(); }
  ParticlePairHistos::cloneB(source);
  h_n1n1_ptpt  = safeCloneH2(source.h_n1n1_ptpt);
  h_C2_ptpt    = safeCloneH2(source.h_C2_ptpt);
  h_C2N_ptpt   = safeCloneH2(source.h_C2N_ptpt);
  h_R2_ptpt    = safeCloneH2(source.h_R2_ptpt);
  
  h_n1n1_phiphi  = safeCloneH2(source.h_n1n1_phiphi);
  h_C2_phiphi    = safeCloneH2(source.h_C2_phiphi);
  h_C2N_phiphi   = safeCloneH2(source.h_C2N_phiphi);
  h_R2_phiphi    = safeCloneH2(source.h_R2_phiphi);
  h_P2_phiphi    = safeCloneH2(source.h_P2_phiphi);
  h_G2_phiphi    = safeCloneH2(source.h_G2_phiphi);
  h_DptDpt_phiphi  = safeCloneH2(source.h_DptDpt_phiphi);
  h_pt1pt1_phiphi  = safeCloneH2(source.h_pt1pt1_phiphi);
  
  h_n1n1_etaeta    = safeCloneH2(source.h_n1n1_etaeta);
  h_C2_etaeta      = safeCloneH2(source.h_C2_etaeta);
  h_C2N_etaeta     = safeCloneH2(source.h_C2N_etaeta);
  h_R2_etaeta      = safeCloneH2(source.h_R2_etaeta);
  h_G2_etaeta      = safeCloneH2(source.h_G2_etaeta);
  h_P2_etaeta      = safeCloneH2(source.h_P2_etaeta);
  h_DptDpt_etaeta  = safeCloneH2(source.h_DptDpt_etaeta);
  h_pt1pt1_etaeta  = safeCloneH2( source.h_pt1pt1_etaeta);
  
  h_n1n1_yY    = safeCloneH2(source.h_n1n1_yY);
  h_C2_yY      = safeCloneH2(source.h_C2_yY);
  h_C2N_yY     = safeCloneH2(source.h_C2N_yY);
  h_R2_yY      = safeCloneH2(source.h_R2_yY);
  h_G2_yY      = safeCloneH2(source.h_G2_yY);
  h_P2_yY      = safeCloneH2(source.h_P2_yY);
  h_DptDpt_yY  = safeCloneH2(source.h_DptDpt_yY);
  h_pt1pt1_yY  = safeCloneH2(source.h_pt1pt1_yY);
  
  h_n1n1_DetaDphi    = safeCloneH2(source.h_n1n1_DetaDphi);
  h_C2_DetaDphi      = safeCloneH2(source.h_C2_DetaDphi);
  h_C2N_DetaDphi     = safeCloneH2(source.h_C2N_DetaDphi);
  h_C2O_DetaDphi     = safeCloneH2(source.h_C2O_DetaDphi);
  h_R2_DetaDphi      = safeCloneH2(source.h_R2_DetaDphi);
  h_P2_DetaDphi      = safeCloneH2(source.h_P2_DetaDphi);
  h_G2_DetaDphi      = safeCloneH2(source.h_G2_DetaDphi);
  h_DptDpt_DetaDphi  = safeCloneH2(source.h_DptDpt_DetaDphi);
  h_pt1pt1_DetaDphi  = safeCloneH2(source.h_pt1pt1_DetaDphi);
  
  h_C2_DetaDphi_shft  = safeCloneH2(source.h_C2_DetaDphi_shft);
  h_C2N_DetaDphi_shft = safeCloneH2(source.h_C2N_DetaDphi_shft);
  h_C2O_DetaDphi_shft = safeCloneH2(source.h_C2O_DetaDphi_shft);
  h_R2_DetaDphi_shft  = safeCloneH2(source.h_R2_DetaDphi_shft);
  h_P2_DetaDphi_shft  = safeCloneH2(source.h_P2_DetaDphi_shft);
  h_G2_DetaDphi_shft  = safeCloneH2(source.h_G2_DetaDphi_shft);
  h_DptDpt_DetaDphi_shft  = safeCloneH2(source.h_DptDpt_DetaDphi_shft);
  
  h_n1n1_DyDphi  = safeCloneH2(source.h_n1n1_DyDphi);
  h_C2_DyDphi    = safeCloneH2(source.h_C2_DyDphi);
  h_C2N_DyDphi   = safeCloneH2(source.h_C2N_DyDphi);
  h_C2O_DyDphi   = safeCloneH2(source.h_C2O_DyDphi);
  h_R2_DyDphi    = safeCloneH2(source.h_R2_DyDphi);
  h_P2_DyDphi    = safeCloneH2(source.h_P2_DyDphi);
  h_G2_DyDphi    = safeCloneH2(source.h_G2_DyDphi);
  h_DptDpt_DyDphi  = safeCloneH2(source.h_DptDpt_DyDphi);
  
  h_n1n1_DyDphi_shft   = safeCloneH2(source.h_n1n1_DyDphi_shft);
  h_C2_DyDphi_shft     = safeCloneH2(source.h_C2_DyDphi_shft);
  h_C2N_DyDphi_shft    = safeCloneH2(source.h_C2N_DyDphi_shft);
  h_C2O_DyDphi_shft    = safeCloneH2(source.h_C2O_DyDphi_shft);
  h_R2_DyDphi_shft     = safeCloneH2(source.h_R2_DyDphi_shft);
  h_P2_DyDphi_shft     = safeCloneH2(source.h_P2_DyDphi_shft);
  h_G2_DyDphi_shft     = safeCloneH2(source.h_G2_DyDphi_shft);
  h_DptDpt_DyDphi_shft = safeCloneH2(source.h_DptDpt_DyDphi_shft);
}

void ParticlePairDerivedHistos::configure(const String & taskName,
                                                const String & objectType,
                                                const Configuration & configuration,
                                                unsigned int index)
{
  if (reportDebug(__FUNCTION__)) { printCR(); }
  ParticlePairHistos::configure(taskName,objectType,configuration,index);
}

void ParticlePairDerivedHistos::create()
{
  if (reportDebug(__FUNCTION__)) { printCR(); }
  h_n1n1_ptpt            = createHistogram(createName(_histogramBaseName,"n1n1_ptpt"),   pt_nbins,  pt_min,  pt_max,  pt_nbins,  pt_min,   pt_max,   "p_{T,1}", "p_{T,2}", "<n_{1}><n_{2}>");
  h_C2N_ptpt             = createHistogram(createName(_histogramBaseName,"C2N_ptpt"),     pt_nbins,  pt_min,  pt_max,  pt_nbins,  pt_min,   pt_max,   "p_{T,1}", "p_{T,2}", "A_{2}");
  h_C2_ptpt              = createHistogram(createName(_histogramBaseName,"C2_ptpt"),     pt_nbins,  pt_min,  pt_max,  pt_nbins,  pt_min,   pt_max,   "p_{T,1}", "p_{T,2}", "C_{2}");
  h_R2_ptpt              = createHistogram(createName(_histogramBaseName,"R2_ptpt"),     pt_nbins,  pt_min,  pt_max,  pt_nbins,  pt_min,   pt_max,   "p_{T,1}", "p_{T,2}", "R_{2}");
  
  h_n1n1_phiphi          = createHistogram(createName(_histogramBaseName,"n1n1_phiphi"), phi_nbins, phi_min, phi_max, phi_nbins, phi_min,  phi_max,  "#varphi_{1}", "#varphi_{2}", "<n_{1}><n_{2}>");
  h_C2N_phiphi           = createHistogram(createName(_histogramBaseName,"C2N_phiphi"),   phi_nbins, phi_min, phi_max, phi_nbins, phi_min,  phi_max,  "#varphi_{1}", "#varphi_{2}", "A_{2}");
  h_C2_phiphi            = createHistogram(createName(_histogramBaseName,"C2_phiphi"),   phi_nbins, phi_min, phi_max, phi_nbins, phi_min,  phi_max,  "#varphi_{1}", "#varphi_{2}", "C_{2}");
  h_R2_phiphi            = createHistogram(createName(_histogramBaseName,"R2_phiphi"),   phi_nbins, phi_min, phi_max, phi_nbins, phi_min,  phi_max,  "#varphi_{1}", "#varphi_{2}", "R_{2}");
  
  if (p2_fill)
  {
    h_pt1pt1_phiphi      = createHistogram(createName(_histogramBaseName,"pt1pt1_phiphi"), phi_nbins, phi_min, phi_max, phi_nbins, phi_min,  phi_max,  "#varphi_{1}","#varphi_{2}",   "pt1pt1");
    h_DptDpt_phiphi      = createHistogram(createName(_histogramBaseName,"DptDpt_phiphi"), phi_nbins, phi_min, phi_max, phi_nbins, phi_min,  phi_max,  "#varphi_{1}", "#varphi_{2}",   "<#Delta p_{T} #Delta p_{T}>");
    h_P2_phiphi          = createHistogram(createName(_histogramBaseName,"P2_phiphi"),     phi_nbins, phi_min, phi_max, phi_nbins, phi_min,  phi_max,  "#varphi_{1}", "#varphi_{2}",   "P_{2}");
    h_G2_phiphi          = createHistogram(createName(_histogramBaseName,"G2_phiphi"),     phi_nbins, phi_min, phi_max, phi_nbins, phi_min,  phi_max,  "#varphi_{1}", "#varphi_{2}",   "G_{2}");
  }
  
  if (eta_fill)
  {
    h_n1n1_etaeta        = createHistogram(createName(_histogramBaseName,"n1n1_etaeta"),    eta_nbins,   eta_min,   eta_max,   eta_nbins,   eta_min,  eta_max,  "#eta_{1}", "#eta_{2}", "<n_{1}><n_{2}>");
    h_C2N_etaeta         = createHistogram(createName(_histogramBaseName,"C2N_etaeta"),      eta_nbins,   eta_min,   eta_max,   eta_nbins,   eta_min,  eta_max,  "#eta_{1}", "#eta_{2}", "A_{2}");
    h_C2_etaeta          = createHistogram(createName(_histogramBaseName,"C2_etaeta"),      eta_nbins,   eta_min,   eta_max,   eta_nbins,   eta_min,  eta_max,  "#eta_{1}", "#eta_{2}", "C_{2}");
    h_R2_etaeta          = createHistogram(createName(_histogramBaseName,"R2_etaeta"),      eta_nbins,   eta_min,   eta_max,   eta_nbins,   eta_min,  eta_max,  "#eta_{1}", "#eta_{2}", "R_{2}");
    
    h_n1n1_DetaDphi      = createHistogram(createName(_histogramBaseName,"n1n1_DetaDphi"),  Deta_nbins, Deta_min, Deta_max, Dphi_nbins, Dphi_min, Dphi_max, "#Delta#eta","#Delta#varphi", "<n_{1}><n_{1}>");
    h_C2_DetaDphi        = createHistogram(createName(_histogramBaseName,"C2_DetaDphi"),    Deta_nbins, Deta_min, Deta_max, Dphi_nbins, Dphi_min, Dphi_max, "#Delta#eta","#Delta#varphi", "C_{2}");
    h_C2N_DetaDphi       = createHistogram(createName(_histogramBaseName,"C2N_DetaDphi"),    Deta_nbins, Deta_min, Deta_max, Dphi_nbins, Dphi_min, Dphi_max, "#Delta#eta","#Delta#varphi", "A_{2}");
    h_C2O_DetaDphi       = createHistogram(createName(_histogramBaseName,"C2O_DetaDphi"),    Deta_nbins, Deta_min, Deta_max, Dphi_nbins, Dphi_min, Dphi_max, "#Delta#eta","#Delta#varphi", "D_{2}");
    h_R2_DetaDphi        = createHistogram(createName(_histogramBaseName,"R2_DetaDphi"),    Deta_nbins, Deta_min, Deta_max, Dphi_nbins, Dphi_min, Dphi_max, "#Delta#eta","#Delta#varphi", "R_{2}");
    
    h_C2_DetaDphi_shft   = createHistogram(createName(_histogramBaseName,"C2_DetaDphi_shft"),    Deta_nbins,  Deta_min,  Deta_max,  Dphi_nbins,  Dphi_min_shft, Dphi_max_shft, "#Delta#eta","#Delta#varphi", "C_{2}");
    h_C2N_DetaDphi_shft  = createHistogram(createName(_histogramBaseName,"C2N_DetaDphi_shft"),    Deta_nbins,  Deta_min,  Deta_max,  Dphi_nbins,  Dphi_min_shft, Dphi_max_shft, "#Delta#eta","#Delta#varphi", "A_{2}");
    h_C2O_DetaDphi_shft  = createHistogram(createName(_histogramBaseName,"C2O_DetaDphi_shft"),    Deta_nbins,  Deta_min,  Deta_max,  Dphi_nbins,  Dphi_min_shft, Dphi_max_shft, "#Delta#eta","#Delta#varphi", "D_{2}");
    h_R2_DetaDphi_shft   = createHistogram(createName(_histogramBaseName,"R2_DetaDphi_shft"),    Deta_nbins,  Deta_min,  Deta_max,  Dphi_nbins,  Dphi_min_shft, Dphi_max_shft, "#Delta#eta","#Delta#varphi", "R_{2}");
    
    if (p2_fill)
    {
      h_pt1pt1_etaeta        = createHistogram(createName(_histogramBaseName,"pt1pt1_etaeta"),  eta_nbins,   eta_min,   eta_max,   eta_nbins,   eta_min,  eta_max,            "#eta_{1}", "#eta_{2}",      "pt1pt1");
      h_DptDpt_etaeta        = createHistogram(createName(_histogramBaseName,"DptDpt_etaeta"),  eta_nbins,   eta_min,   eta_max,   eta_nbins,   eta_min,  eta_max,            "#eta_{1}", "#eta_{2}",      "<#Delta p_{T} #Delta p_{T}>");
      h_P2_etaeta            = createHistogram(createName(_histogramBaseName,"P2_etaeta"),      eta_nbins,   eta_min,   eta_max,   eta_nbins,   eta_min,  eta_max,            "#eta_{1}", "#eta_{2}",      "P_{2}");
      h_G2_etaeta            = createHistogram(createName(_histogramBaseName,"G2_etaeta"),      eta_nbins,   eta_min,   eta_max,   eta_nbins,   eta_min,  eta_max,            "#eta_{1}", "#eta_{2}",         "G_{2}");
      
      h_pt1pt1_DetaDphi      = createHistogram(createName(_histogramBaseName,"pt1pt1_DetaDphi"),Deta_nbins,  Deta_min,  Deta_max,  Dphi_nbins,  Dphi_min,      Dphi_max,      "#Delta#eta", "#Delta#varphi", "pt1pt1");
      h_DptDpt_DetaDphi      = createHistogram(createName(_histogramBaseName,"DptDpt_DetaDphi"),Deta_nbins,  Deta_min,  Deta_max,  Dphi_nbins,  Dphi_min,      Dphi_max,      "#Delta#eta",  "#Delta#varphi", "<#Delta p_{T}#Delta p_{T}>");
      h_P2_DetaDphi          = createHistogram(createName(_histogramBaseName,"P2_DetaDphi"),    Deta_nbins,  Deta_min,  Deta_max,  Dphi_nbins,  Dphi_min,      Dphi_max,      "#Delta#eta",  "#Delta#varphi", "P_{2}");
      h_G2_DetaDphi          = createHistogram(createName(_histogramBaseName,"G2_DetaDphi"),    Deta_nbins,  Deta_min,  Deta_max,  Dphi_nbins,  Dphi_min,      Dphi_max,      "#Delta#eta","#Delta#varphi",   "G_{2}");
      h_DptDpt_DetaDphi_shft = createHistogram(createName(_histogramBaseName,"DptDpt_DetaDphi_shft"),Deta_nbins,  Deta_min,  Deta_max,  Dphi_nbins,  Dphi_min_shft, Dphi_max_shft, "#Delta#eta",  "#Delta#varphi", "<#Delta p_{T} #Delta p_{T}>");
      h_P2_DetaDphi_shft     = createHistogram(createName(_histogramBaseName,"P2_DetaDphi_shft"),    Deta_nbins,  Deta_min,  Deta_max,  Dphi_nbins,  Dphi_min_shft, Dphi_max_shft, "#Delta#eta",  "#Delta#varphi", "P_{2}");
      h_G2_DetaDphi_shft     = createHistogram(createName(_histogramBaseName,"G2_DetaDphi_shft"),    Deta_nbins,  Deta_min,  Deta_max,  Dphi_nbins,  Dphi_min_shft, Dphi_max_shft, "#Delta#eta", "#Delta#varphi",  "G_{2}");
    }
  }
  
  if (rapidity_fill)
  {
    h_n1n1_yY           = createHistogram(createName(_histogramBaseName,"n1n1_yY"),      rapidity_nbins,   rapidity_minimum,   rapidity_maximum,   rapidity_nbins,   rapidity_minimum,  rapidity_maximum,                 "y_{1}",     "y_{2}", "<n_{1}><n_{1}>");
    h_C2_yY             = createHistogram(createName(_histogramBaseName,"C2_yY"),        rapidity_nbins,   rapidity_minimum,   rapidity_maximum,   rapidity_nbins,   rapidity_minimum,  rapidity_maximum,                 "y_{1}",    "y_{2}", "C_{2}");
    h_C2N_yY            = createHistogram(createName(_histogramBaseName,"C2N_yY"),        rapidity_nbins,   rapidity_minimum,   rapidity_maximum,   rapidity_nbins,   rapidity_minimum,  rapidity_maximum,                 "y_{1}",    "y_{2}", "A_{2}");
    h_R2_yY             = createHistogram(createName(_histogramBaseName,"R2_yY"),        rapidity_nbins,   rapidity_minimum,   rapidity_maximum,   rapidity_nbins,   rapidity_minimum,  rapidity_maximum,                 "y_{1}",    "y_{2}", "R_{2}");
    
    h_n1n1_DyDphi       = createHistogram(createName(_histogramBaseName,"n1n1_DyDphi"),  Drapidity_nbins,  Drapidity_min,  Drapidity_max,  Dphi_nbins,  Dphi_min,      Dphi_max,     "#Delta y",  "#Delta#varphi", "<n_{1}><n_{1}>");
    h_C2_DyDphi         = createHistogram(createName(_histogramBaseName,"C2_DyDphi"),    Drapidity_nbins,  Drapidity_min,  Drapidity_max,  Dphi_nbins,  Dphi_min,      Dphi_max,     "#Delta y",  "#Delta#varphi", "C_{2}");
    h_C2N_DyDphi        = createHistogram(createName(_histogramBaseName,"C2N_DyDphi"),   Drapidity_nbins,  Drapidity_min,  Drapidity_max,  Dphi_nbins,  Dphi_min,      Dphi_max,     "#Delta y",  "#Delta#varphi", "A_{2}");
    h_C2O_DyDphi        = createHistogram(createName(_histogramBaseName,"C2O_DyDphi"),   Drapidity_nbins,  Drapidity_min,  Drapidity_max,  Dphi_nbins,  Dphi_min,      Dphi_max,     "#Delta y",  "#Delta#varphi", "D_{2}");
    h_R2_DyDphi         = createHistogram(createName(_histogramBaseName,"R2_DyDphi"),    Drapidity_nbins,  Drapidity_min,  Drapidity_max,  Dphi_nbins,  Dphi_min,      Dphi_max,     "#Delta y",  "#Delta#varphi", "R_{2}");
    
    h_C2_DyDphi_shft    = createHistogram(createName(_histogramBaseName,"C2_DyDphi_shft"),  Drapidity_nbins,  Drapidity_min,  Drapidity_max,  Dphi_nbins,  Dphi_min_shft, Dphi_max_shft, "#Delta y", "#Delta#varphi", "C_{2}");
    h_C2N_DyDphi_shft   = createHistogram(createName(_histogramBaseName,"C2N_DyDphi_shft"), Drapidity_nbins,  Drapidity_min,  Drapidity_max,  Dphi_nbins,  Dphi_min_shft, Dphi_max_shft, "#Delta y", "#Delta#varphi", "A_{2}");
    h_C2O_DyDphi_shft   = createHistogram(createName(_histogramBaseName,"C2O_DyDphi_shft"), Drapidity_nbins,  Drapidity_min,  Drapidity_max,  Dphi_nbins,  Dphi_min_shft, Dphi_max_shft, "#Delta y", "#Delta#varphi", "D_{2}");
    h_R2_DyDphi_shft    = createHistogram(createName(_histogramBaseName,"R2_DyDphi_shft"),  Drapidity_nbins,  Drapidity_min,  Drapidity_max,  Dphi_nbins,  Dphi_min_shft, Dphi_max_shft, "#Delta y", "#Delta#varphi", "R_{2}");
    
    if (p2_fill)
    {
      h_pt1pt1_yY         = createHistogram(createName(_histogramBaseName,"pt1pt1_yY"),    rapidity_nbins,   rapidity_minimum,   rapidity_maximum,   rapidity_nbins,   rapidity_minimum,  rapidity_maximum,                  "y_{1}",    "y_{2}", "pt1pt1");
      h_DptDpt_yY         = createHistogram(createName(_histogramBaseName,"DptDpt_yY"),    rapidity_nbins,   rapidity_minimum,   rapidity_maximum,   rapidity_nbins,   rapidity_minimum,  rapidity_maximum,                  "y_{1}",    "y_{2}", "<#Delta p_{T} #Delta p_{T}>");
      h_P2_yY             = createHistogram(createName(_histogramBaseName,"P2_yY"),        rapidity_nbins,   rapidity_minimum,   rapidity_maximum,   rapidity_nbins,     rapidity_minimum,         rapidity_maximum,         "y_{1}",    "y_{2}",         "P_{2}");
      h_G2_yY             = createHistogram(createName(_histogramBaseName,"G2_yY"),        rapidity_nbins,   rapidity_minimum,   rapidity_maximum,   rapidity_nbins,   rapidity_minimum,  rapidity_maximum,                  "y_{1}",    "y_{2}", "G_{2}");
      
      h_pt1pt1_DyDphi     = createHistogram(createName(_histogramBaseName,"pt1pt1_DyDphi"),Drapidity_nbins,  Drapidity_min,  Drapidity_max,  Dphi_nbins,  Dphi_min,      Dphi_max,      "#Delta y", "#Delta#varphi", "pt1pt1");
      h_DptDpt_DyDphi     = createHistogram(createName(_histogramBaseName,"DptDpt_DyDphi"),Drapidity_nbins,  Drapidity_min,  Drapidity_max,  Dphi_nbins,  Dphi_min,      Dphi_max,      "#Delta y", "#Delta#varphi", "<#Delta p_{T}#Delta p_{T}>");
      h_P2_DyDphi         = createHistogram(createName(_histogramBaseName,"P2_DyDphi"),    Drapidity_nbins,  Drapidity_min,  Drapidity_max,  Dphi_nbins,  Dphi_min,      Dphi_max,      "#Delta y", "#Delta#varphi", "P_{2}");
      h_G2_DyDphi         = createHistogram(createName(_histogramBaseName,"G2_DyDphi"),    Drapidity_nbins,  Drapidity_min,  Drapidity_max,  Dphi_nbins,  Dphi_min,      Dphi_max,      "#Delta y", "#Delta#varphi", "G_{2}");
      
      h_DptDpt_DyDphi_shft= createHistogram(createName(_histogramBaseName,"DptDpt_DyDphi_shft"),Drapidity_nbins,  Drapidity_min,  Drapidity_max,  Dphi_nbins,  Dphi_min_shft, Dphi_max_shft, "#Delta y", "#Delta#varphi", "<#Delta p_{T}#Delta p_{T}>");
      h_P2_DyDphi_shft    = createHistogram(createName(_histogramBaseName,"P2_DyDphi_shft"),    Drapidity_nbins,  Drapidity_min,  Drapidity_max,  Dphi_nbins,  Dphi_min_shft, Dphi_max_shft, "#Delta y", "#Delta#varphi", "P_{2}");
      h_G2_DyDphi_shft    = createHistogram(createName(_histogramBaseName,"G2_DyDphi_shft"),    Drapidity_nbins,  Drapidity_min,  Drapidity_max,  Dphi_nbins,  Dphi_min_shft, Dphi_max_shft, "#Delta y", "#Delta#varphi", "G_{2}");
    }
  }
}

void ParticlePairDerivedHistos::loadFrom(TFile & inputFile)
{
  if (reportDebug(__FUNCTION__)) { printCR(); }
  h_n1n1_phiphi          = importH2(inputFile, CAP::createName(_histogramBaseName,"n1n1_phiphi"));
  h_C2_phiphi            = importH2(inputFile, CAP::createName(_histogramBaseName,"C2_phiphi"));
  h_C2N_phiphi           = importH2(inputFile, CAP::createName(_histogramBaseName,"C2N_phiphi"));
  h_R2_phiphi            = importH2(inputFile, CAP::createName(_histogramBaseName,"R2_phiphi"));
  
  h_n1n1_ptpt            = importH2(inputFile, CAP::createName(_histogramBaseName,"n1n1_ptpt"));
  h_C2_ptpt              = importH2(inputFile, CAP::createName(_histogramBaseName,"C2_ptpt"));
  h_C2N_ptpt             = importH2(inputFile, CAP::createName(_histogramBaseName,"C2N_ptpt"));
  h_R2_ptpt              = importH2(inputFile, CAP::createName(_histogramBaseName,"R2_ptpt"));
  
  if (p2_fill)
  {
    h_pt1pt1_phiphi      = importH2(inputFile, CAP::createName(_histogramBaseName,"pt1pt1_phiphi"));
    h_DptDpt_phiphi      = importH2(inputFile, CAP::createName(_histogramBaseName,"DptDpt_phiphi"));
    h_P2_phiphi          = importH2(inputFile, CAP::createName(_histogramBaseName,"P2_phiphi"));
    h_G2_phiphi          = importH2(inputFile, CAP::createName(_histogramBaseName,"G2_phiphi"));
  }
  
  if (eta_fill)
  {
    h_n1n1_etaeta        = importH2(inputFile, CAP::createName(_histogramBaseName,"n1n1_etaeta"));
    h_C2_etaeta          = importH2(inputFile, CAP::createName(_histogramBaseName,"C2_etaeta"));
    h_C2N_etaeta         = importH2(inputFile, CAP::createName(_histogramBaseName,"C2N_etaeta"));
    h_R2_etaeta          = importH2(inputFile, CAP::createName(_histogramBaseName,"R2_etaeta"));
    
    h_n1n1_DetaDphi      = importH2(inputFile, CAP::createName(_histogramBaseName,"n1n1_DetaDphi_shft"));
    h_C2_DetaDphi        = importH2(inputFile, CAP::createName(_histogramBaseName,"C2_DetaDphi_shft"));
    h_C2N_DetaDphi       = importH2(inputFile, CAP::createName(_histogramBaseName,"C2N_DetaDphi_shft"));
    h_R2_DetaDphi        = importH2(inputFile, CAP::createName(_histogramBaseName,"R2_DetaDphi_shft"));
    
    h_C2_DetaDphi_shft   = importH2(inputFile, CAP::createName(_histogramBaseName,"C2_DetaDphi_shft"));
    h_C2N_DetaDphi_shft  = importH2(inputFile, CAP::createName(_histogramBaseName,"C2N_DetaDphi_shft"));
    h_R2_DetaDphi_shft   = importH2(inputFile, CAP::createName(_histogramBaseName,"R2_DetaDphi_shft"));
    
    if (p2_fill)
    {
      h_pt1pt1_etaeta        = importH2(inputFile, CAP::createName(_histogramBaseName,"pt1pt1_etaeta"));
      h_DptDpt_etaeta        = importH2(inputFile, CAP::createName(_histogramBaseName,"DptDpt_etaeta"));
      h_P2_etaeta            = importH2(inputFile, CAP::createName(_histogramBaseName,"P2_etaeta"));
      h_G2_etaeta            = importH2(inputFile, CAP::createName(_histogramBaseName,"G2_etaeta"));
      
      h_pt1pt1_DetaDphi      = importH2(inputFile, CAP::createName(_histogramBaseName,"pt1pt1_DetaDphi_shft"));
      h_DptDpt_DetaDphi      = importH2(inputFile, CAP::createName(_histogramBaseName,"DptDpt_DetaDphi_shft"));
      h_P2_DetaDphi          = importH2(inputFile, CAP::createName(_histogramBaseName,"P2_DetaDphi_shft"));
      h_G2_DetaDphi          = importH2(inputFile, CAP::createName(_histogramBaseName,"G2_DetaDphi_shft"));
      h_DptDpt_DetaDphi_shft = importH2(inputFile, CAP::createName(_histogramBaseName,"DptDpt_DetaDphi_shft"));
      h_P2_DetaDphi_shft     = importH2(inputFile, CAP::createName(_histogramBaseName,"P2_DetaDphi_shft"));
      h_G2_DetaDphi_shft     = importH2(inputFile, CAP::createName(_histogramBaseName,"G2_DetaDphi_shft"));
    }
  }
  
  if (rapidity_fill)
  {
    h_n1n1_yY           = importH2(inputFile, CAP::createName(_histogramBaseName,"n1n1_yY"));
    h_C2_yY             = importH2(inputFile, CAP::createName(_histogramBaseName,"C2_yY"));
    h_C2N_yY            = importH2(inputFile,CAP::createName(_histogramBaseName,"C2N_yY"));
    h_R2_yY             = importH2(inputFile, CAP::createName(_histogramBaseName,"R2_yY"));
    
    h_n1n1_DyDphi       = importH2(inputFile, CAP::createName(_histogramBaseName,"n1n1_DyDphi"));
    h_C2_DyDphi         = importH2(inputFile, CAP::createName(_histogramBaseName,"C2_DyDphi"));
    h_C2N_DyDphi        = importH2(inputFile, CAP::createName(_histogramBaseName,"C2N_DyDphi"));
    h_C2O_DyDphi        = importH2(inputFile, CAP::createName(_histogramBaseName,"C2O_DyDphi"));
    h_R2_DyDphi         = importH2(inputFile, CAP::createName(_histogramBaseName,"R2_DyDphi"));
    
    h_C2_DyDphi_shft    = importH2(inputFile, CAP::createName(_histogramBaseName,"C2_DyDphi_shft"));
    h_C2N_DyDphi_shft   = importH2(inputFile, CAP::createName(_histogramBaseName,"C2N_DyDphi_shft"));
    h_C2O_DyDphi_shft   = importH2(inputFile, CAP::createName(_histogramBaseName,"C2_DyDphi_shft"));
    h_R2_DyDphi_shft    = importH2(inputFile, CAP::createName(_histogramBaseName,"R2_DyDphi_shft"));
    
    if (p2_fill)
    {
      h_pt1pt1_yY         = importH2(inputFile, CAP::createName(_histogramBaseName,"pt1pt1_yY"));
      h_DptDpt_yY         = importH2(inputFile, CAP::createName(_histogramBaseName,"DptDpt_yY"));
      h_P2_yY             = importH2(inputFile, CAP::createName(_histogramBaseName,"P2_yY"));
      h_G2_yY             = importH2(inputFile, CAP::createName(_histogramBaseName,"G2_yY"));
      
      h_pt1pt1_DyDphi     = importH2(inputFile, CAP::createName(_histogramBaseName,"pt1pt1_DyDphi_shft"));
      h_DptDpt_DyDphi     = importH2(inputFile, CAP::createName(_histogramBaseName,"DptDpt_DyDphi_shft"));
      h_P2_DyDphi         = importH2(inputFile, CAP::createName(_histogramBaseName,"P2_DyDphi_shft"));
      h_G2_DyDphi         = importH2(inputFile, CAP::createName(_histogramBaseName,"G2_DyDphi_shft"));
      
      h_DptDpt_DyDphi_shft= importH2(inputFile, CAP::createName(_histogramBaseName,"DptDpt_DyDphi_shft"));
      h_P2_DyDphi_shft    = importH2(inputFile, CAP::createName(_histogramBaseName,"P2_DyDphi_shft"));
      h_G2_DyDphi_shft    = importH2(inputFile, CAP::createName(_histogramBaseName,"G2_DyDphi_shft"));
    }
  }
}

void quickCopy(TH2 * source, TH2 * target)
{
  int sNx = source->GetNbinsX();
  int sNy = source->GetNbinsY();
  int tNx = target->GetNbinsX();
  int tNy = target->GetNbinsY();
  
  
  if (sNx!=tNx || sNy!=tNy)
  {
    String s = source->GetName();
    s += "-->";
    s += target->GetName();
    throw CAP::HistogramException(s,"sNx!=tNx || sNy!=tNy","ParticlePairDerivedHistos::quickCopy(TH2 * source, TH2 * target)");
  }
  for (int iX=1; iX<sNx; iX++)
  {
    for (int iY=1; iY<sNy; iY++)
    {
      double v  = source->GetBinContent(iX,iY);
      double ev = source->GetBinError(iX,iY);
      target->SetBinContent(iX,iY,v);
      target->SetBinError(iX,iY,ev);
    }
  }
}


// HistogramGroup from ParticlePairDerivedHistos must be normalized "per event" before calling this function
void ParticlePairDerivedHistos::
calculateDerivedHistograms(ParticleSingleDerivedHistos & part1Histos,
                           ParticleSingleDerivedHistos & part2Histos,
                           ParticlePairHistos          & pairHistos)
{
  double bincorrection = 1.0;
  double yield2;
  double avgPt1Phi;
  double avgPt2Phi;
  double scalingFactorPhi;
  double avgPt1Eta;
  double avgPt2Eta;
  double avgPt1Y;
  double avgPt2Y;
  double scalingFactorEta;
  double scalingFactorY;
  int ijNormalization = 0;
  
  if (!part1Histos.h_n1_pt )     throw HistogramException("LOGIC","!part1Histos.h_n1_pt",__FUNCTION__);
  if (!part2Histos.h_n1_pt )     throw HistogramException("LOGIC","!part2Histos.h_n1_pt",__FUNCTION__);
  if (!pairHistos.h_n2_ptpt )    throw HistogramException("LOGIC","!pairHistos.h_n2_ptpt",__FUNCTION__);
  if (!h_n1n1_ptpt )             throw HistogramException("LOGIC","!h_n1n1_ptpt",__FUNCTION__);
  if (!h_C2_ptpt )               throw HistogramException("LOGIC","!h_C2_ptpt",__FUNCTION__);
  if (!h_C2N_ptpt )              throw HistogramException("LOGIC","!h_C2N_ptpt",__FUNCTION__);
  if (!h_R2_ptpt )               throw HistogramException("LOGIC","!h_R2_ptpt",__FUNCTION__);
  
  yield2 = part2Histos.h_n1_pt->Integral();
  calculateN1N1_H1H1H2( part1Histos.h_n1_pt,   part2Histos.h_n1_pt,  h_n1n1_ptpt,1.0, 1.0);
  h_C2_ptpt->Add(pairHistos.h_n2_ptpt,h_n1n1_ptpt,1.0,-1.0);
  h_C2N_ptpt->Add(h_C2_ptpt);
  h_C2N_ptpt->Scale(1.0/yield2);
  calculateR2_H2H2H2(pairHistos.h_n2_ptpt,h_n1n1_ptpt,h_R2_ptpt,0, 1.0, 1.0);
  
  //
  // phi vs phi
  //
  yield2 = part2Histos.h_n1_phi->Integral();
  calculateN1N1_H1H1H2( part1Histos.h_n1_phi,  part2Histos.h_n1_phi, h_n1n1_phiphi,1.0, 1.0);
  h_C2_phiphi->Add(pairHistos.h_n2_phiphi,h_n1n1_phiphi,1.0,-1.0);
  h_C2N_phiphi->Add(h_C2_phiphi);
  h_C2N_phiphi->Scale(1.0/yield2);
  calculateR2_H2H2H2(pairHistos.h_n2_phiphi,h_n1n1_phiphi,h_R2_phiphi,0, 1.0, 1.0);
  
  if (p2_fill)
  {
    avgPt1Phi = avgValue(part1Histos.h_avg_pt_phi);
    avgPt2Phi = avgValue(part2Histos.h_avg_pt_phi);
    scalingFactorPhi = 1.0/avgPt1Phi/avgPt2Phi;
    h_P2_phiphi->Add(h_DptDpt_phiphi,h_DptDpt_phiphi,1.0,0.0);
    h_P2_phiphi->Scale(scalingFactorPhi);
  }
  
  
  if (eta_fill)
  {
    
    //
    // eta vs eta
    //
    yield2 = part2Histos.h_n1_eta->Integral();
    calculateN1N1_H1H1H2(part1Histos.h_n1_eta,part2Histos.h_n1_eta,h_n1n1_etaeta,1.0, 1.0);
    h_C2_etaeta->Add(pairHistos.h_n2_etaeta,h_n1n1_etaeta,1.0,-1.0);
    h_C2N_etaeta->Add(h_C2_etaeta);
    h_C2N_etaeta->Scale(1.0/yield2);
    calculateR2_H2H2H2(pairHistos.h_n2_etaeta,h_n1n1_etaeta,h_R2_etaeta,0, 1.0, 1.0);
    
    //
    // Deta vs Dphi
    //
    reduce_n1EtaPhiN1EtaPhiOntoN1N1DetaDphi(part1Histos.h_n1_phiEta,part2Histos.h_n1_phiEta,h_n1n1_DetaDphi,Deta_nbins,Dphi_nbins);
    h_C2_DetaDphi->Add(pairHistos.h_n2_DetaDphi,h_n1n1_DetaDphi, 1.0, -1.0);
    h_C2N_DetaDphi->Add(h_C2_DetaDphi);
    h_C2N_DetaDphi->Scale(1.0/yield2);
    calculateR2_H2H2H2(pairHistos.h_n2_DetaDphi,h_n1n1_DetaDphi,h_R2_DetaDphi,0,1.0,1.0);
    
    shiftY(*h_C2N_DetaDphi,*h_C2N_DetaDphi_shft,Dphi_nbins_shft);
    shiftY(*h_C2_DetaDphi,*h_C2_DetaDphi_shft,Dphi_nbins_shft);
    shiftY(*h_R2_DetaDphi,*h_R2_DetaDphi_shft,Dphi_nbins_shft);
    
    if (p2_fill)
    {
      avgPt1Eta = avgValue(part1Histos.h_avg_pt_eta);
      avgPt2Eta = avgValue(part2Histos.h_avg_pt_eta);
      scalingFactorEta = 1.0/avgPt1Eta/avgPt2Eta;
      h_P2_etaeta->Add(h_DptDpt_etaeta,h_DptDpt_etaeta,1.0,0.0);
      h_P2_etaeta->Scale(scalingFactorEta);
      shiftY(*h_DptDpt_DetaDphi, *h_DptDpt_DetaDphi_shft,Dphi_nbins_shft);
      
      /// calculate average pt base on h_avg_pt_eta histograms
      h_P2_DetaDphi->Add(h_DptDpt_DetaDphi,h_DptDpt_DetaDphi,1.0,0.0);
      h_P2_DetaDphi->Scale(scalingFactorEta);
      //symmetrizeDeltaEtaDeltaPhi(h_P2_DetaDphi,ijNormalization);
      shiftY(*h_P2_DetaDphi,*h_P2_DetaDphi_shft, Dphi_nbins_shft);

      calculateG2_H2H2H2H2( h_DptDpt_etaeta,   h_n1n1_etaeta,   h_pt1pt1_etaeta,   h_G2_etaeta,   ijNormalization, bincorrection, 1.0);
      calculateG2_H2H2H2H2( h_DptDpt_phiphi,   h_n1n1_phiphi,   h_pt1pt1_phiphi,   h_G2_phiphi,   ijNormalization, bincorrection, 1.0);
      calculateG2_H2H2H2H2( h_DptDpt_DetaDphi, h_n1n1_DetaDphi, h_pt1pt1_DetaDphi, h_G2_DetaDphi, ijNormalization, bincorrection, 1.0);
      shiftY(*h_G2_DetaDphi,*h_G2_DetaDphi_shft,Dphi_nbins_shft);
    }
  }
  
  if (rapidity_fill)
  {
    //
    // y vs y
    //
    yield2 = part2Histos.h_n1_y->Integral();
    calculateN1N1_H1H1H2(part1Histos.h_n1_y,part2Histos.h_n1_y,h_n1n1_yY,1.0, 1.0);
    h_C2_yY->Add(pairHistos.h_n2_yY,h_n1n1_yY,1.0,-1.0);
    h_C2N_yY->Add(h_C2_yY);
    h_C2N_yY->Scale(1.0/yield2);
    calculateR2_H2H2H2(pairHistos.h_n2_yY,h_n1n1_yY,h_R2_yY,0, 1.0, 1.0);
    
    //
    // Dy vs Dphi
    //
    //if (reportDebug(__FUNCTION__))  cout << "Calculate XXX_DyDphi" << endl;
    reduce_n1EtaPhiN1EtaPhiOntoN1N1DetaDphi(part1Histos.h_n1_phiY,part2Histos.h_n1_phiY,h_n1n1_DyDphi,Drapidity_nbins,Dphi_nbins);
    h_C2_DyDphi->Add(pairHistos.h_n2_DyDphi,h_n1n1_DyDphi, 1.0, -1.0);
    h_C2N_DyDphi->Add(h_C2_DyDphi);
    h_C2N_DyDphi->Scale(1.0/yield2);
    calculateR2_H2H2H2(pairHistos.h_n2_DyDphi,h_n1n1_DyDphi,h_R2_DyDphi,0,1.0,1.0);
    
    vector<double> omegaFactor;
    omegaFactor.clear();
    calculateOmegaFactor(h_C2N_DyDphi, omegaFactor);
    calculateAverageYbar(h_C2N_DyDphi, h_C2O_DyDphi, omegaFactor);
    
    //if (reportDebug(__FUNCTION__))  cout <<  " shift XXX_DyDphi" << endl;
    shiftY(*h_C2_DyDphi,  *h_C2_DyDphi_shft,    Dphi_nbins_shft);
    shiftY(*h_C2N_DyDphi, *h_C2N_DyDphi_shft,   Dphi_nbins_shft);
    shiftY(*h_C2O_DyDphi, *h_C2O_DyDphi_shft,   Dphi_nbins_shft);
    shiftY(*h_R2_DyDphi,  *h_R2_DyDphi_shft,    Dphi_nbins_shft);
    
    if (p2_fill)
    {
      avgPt1Y = avgValue(part1Histos.h_avg_pt_y);
      avgPt2Y = avgValue(part2Histos.h_avg_pt_y);
      scalingFactorY = 1.0/avgPt1Y/avgPt2Y;
      h_P2_yY->Add(h_DptDpt_yY,h_DptDpt_yY,1.0,0.0);
      h_P2_yY->Scale(scalingFactorY);
      //if (reportDebug(__FUNCTION__))  cout <<  " shiftY(*h_R2_DyDphi,*h_R2_DyDphi_shft,Dphi_nbins_shft)" << endl;
      shiftY(*h_DptDpt_DyDphi, *h_DptDpt_DyDphi_shft,Dphi_nbins_shft);
      
      /// calculate average pt base on h_avg_pt_eta histograms
      h_P2_DyDphi->Add(h_DptDpt_DyDphi,h_DptDpt_DyDphi,1.0,0.0);
      h_P2_DyDphi->Scale(scalingFactorY);
      //symmetrizeDeltaEtaDeltaPhi(h_P2_DetaDphi,ijNormalization);
      shiftY(*h_P2_DyDphi,*h_P2_DyDphi_shft, Dphi_nbins_shft);
      calculateG2_H2H2H2H2( h_DptDpt_yY,       h_n1n1_yY,       h_pt1pt1_yY,       h_G2_yY,       ijNormalization, bincorrection, 1.0);
      calculateG2_H2H2H2H2( h_DptDpt_phiphi,   h_n1n1_phiphi,   h_pt1pt1_phiphi,   h_G2_phiphi,   ijNormalization, bincorrection, 1.0);
      calculateG2_H2H2H2H2( h_DptDpt_DetaDphi, h_n1n1_DetaDphi, h_pt1pt1_DetaDphi, h_G2_DetaDphi, ijNormalization, bincorrection, 1.0);
      shiftY(*h_G2_DetaDphi,*h_G2_DetaDphi_shft,Dphi_nbins_shft);
    }
  }
}

void ParticlePairDerivedHistos::calculateOmegaFactor(TH2 * source, vector<double> & omegaFactor)
{
  int nY  = source->GetNbinsX(); // bins in delta rapidity
  double y0;
  y0 = 2.0*rapidity_maximumAcc;
  for (int iY=1; iY<=nY; iY++) // delta rapidity loop
  {
    double dy = source->GetXaxis()->GetBinCenter(iY);
    omegaFactor.push_back(1.0- absolute(dy/y0));
    //cout << " index : " << iY << "  dy: " << dy << " Omega: " << omegaFactor[iY] << endl;
  }
}


void ParticlePairDerivedHistos::calculateAverageYbar(TH2 * source,
                                                     TH2 * target,
                                                     vector<double> & omegaFactor)
{
  //  cout << "calculateAverageYbar(TH2 *source,TH2*target,vector<double>&omegaFactor)" << endl;
  double scale = 1.0;
  int nX = source->GetNbinsX();
  int nY = source->GetNbinsY();
  for (int iX=1; iX<=nX; iX++)
  {
    scale = 1.0/omegaFactor[iX-1];
    //if (iX>180  &&  iX<220) cout << " iX: " << iX << " omegaFactor: " << omegaFactor[iX-1] << " scale:" << scale << endl;
    for (int iY=1; iY<=nY; iY++)
    {
      double v  =  source->GetBinContent(iX,iY);
      double ev =  source->GetBinError(iX,iY);
      //if (iX>190  &&  iX<210) cout << " v: " << v << "+-" << ev << " v*scale:" << v*scale << "+-" << ev*scale << endl;
      target->SetBinContent(iX,iY,v*scale);
      target->SetBinError(iX,iY,ev*scale);
    }
  }
}


} // namespace CAP
