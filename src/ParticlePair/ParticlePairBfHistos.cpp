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

#include "ParticleSingleHistos.hpp"
#include "ParticleSingleDerivedHistos.hpp"
#include "ParticlePairHistos.hpp"
#include "HistogramGroup.hpp"
#include "ParticlePairBfHistos.hpp"
#include "RootFileHelpers.hpp"
#include "RootHistogramHelpers.hpp"
#include "NameHelpers.hpp"
#include "PrintHelpers.hpp"
#include "TRandom.h"
#include "Task.hpp"
#include "Configuration.hpp"

ClassImp(CAP::ParticlePairBfHistos);

namespace CAP
{

ParticlePairBfHistos::ParticlePairBfHistos()
:
ParticlePairDerivedHistos(),
h_C2_CD_ptpt(nullptr),
h_C2_CI_ptpt(nullptr),
h_C2N_B12_ptpt(nullptr),
h_C2N_B21_ptpt(nullptr),
h_C2N_Bs_ptpt(nullptr),
h_R2_B12_ptpt(nullptr),
h_R2_B21_ptpt(nullptr),
h_R2_Bs_ptpt(nullptr),

h_C2_CD_phiphi(nullptr),
h_C2_CI_phiphi(nullptr),
h_C2N_B12_phiphi(nullptr),
h_C2N_B21_phiphi(nullptr),
h_C2N_Bs_phiphi(nullptr),
h_R2_B12_phiphi(nullptr),
h_R2_B21_phiphi(nullptr),
h_R2_Bs_phiphi(nullptr),

h_C2_CD_etaeta(nullptr),
h_C2_CI_etaeta(nullptr),
h_C2N_B12_etaeta(nullptr),
h_C2N_B21_etaeta(nullptr),
h_C2N_Bs_etaeta(nullptr),
h_R2_B12_etaeta(nullptr),
h_R2_B21_etaeta(nullptr),
h_R2_Bs_etaeta(nullptr),

h_C2_CD_yY(nullptr),
h_C2_CI_yY(nullptr),
h_C2N_B12_yY(nullptr),
h_C2N_B21_yY(nullptr),
h_C2N_Bs_yY(nullptr),
h_R2_B12_yY(nullptr),
h_R2_B21_yY(nullptr),
h_R2_Bs_yY(nullptr),

h_C2_CD_DetaDphi(nullptr),
h_C2_CI_DetaDphi(nullptr),
h_C2N_B12_DetaDphi(nullptr),
h_C2N_B21_DetaDphi(nullptr),
h_C2N_Bs_DetaDphi(nullptr),
h_R2_B12_DetaDphi(nullptr),
h_R2_B21_DetaDphi(nullptr),
h_R2_Bs_DetaDphi(nullptr),

h_C2_CD_DyDphi(nullptr),
h_C2_CI_DyDphi(nullptr),
h_C2N_B12_DyDphi(nullptr),
h_C2N_B21_DyDphi(nullptr),
h_C2N_Bs_DyDphi(nullptr),
h_R2_B12_DyDphi(nullptr),
h_R2_B21_DyDphi(nullptr),
h_R2_Bs_DyDphi(nullptr)
{
  appendClassName("ParticlePairBfHistos");
  setName("ParticlePairBfHistos");
  setTitle("ParticlePairBfHistos");
}

ParticlePairBfHistos::ParticlePairBfHistos(const ParticlePairBfHistos & source)
:
ParticlePairDerivedHistos(source),
h_C2_CD_ptpt(nullptr),
h_C2_CI_ptpt(nullptr),
h_C2N_B12_ptpt(nullptr),
h_C2N_B21_ptpt(nullptr),
h_C2N_Bs_ptpt(nullptr),
h_R2_B12_ptpt(nullptr),
h_R2_B21_ptpt(nullptr),
h_R2_Bs_ptpt(nullptr),

h_C2_CD_phiphi(nullptr),
h_C2_CI_phiphi(nullptr),
h_C2N_B12_phiphi(nullptr),
h_C2N_B21_phiphi(nullptr),
h_C2N_Bs_phiphi(nullptr),
h_R2_B12_phiphi(nullptr),
h_R2_B21_phiphi(nullptr),
h_R2_Bs_phiphi(nullptr),

h_C2_CD_etaeta(nullptr),
h_C2_CI_etaeta(nullptr),
h_C2N_B12_etaeta(nullptr),
h_C2N_B21_etaeta(nullptr),
h_C2N_Bs_etaeta(nullptr),
h_R2_B12_etaeta(nullptr),
h_R2_B21_etaeta(nullptr),
h_R2_Bs_etaeta(nullptr),

h_C2_CD_yY(nullptr),
h_C2_CI_yY(nullptr),
h_C2N_B12_yY(nullptr),
h_C2N_B21_yY(nullptr),
h_C2N_Bs_yY(nullptr),
h_R2_B12_yY(nullptr),
h_R2_B21_yY(nullptr),
h_R2_Bs_yY(nullptr),

h_C2_CD_DetaDphi(nullptr),
h_C2_CI_DetaDphi(nullptr),
h_C2N_B12_DetaDphi(nullptr),
h_C2N_B21_DetaDphi(nullptr),
h_C2N_Bs_DetaDphi(nullptr),
h_R2_B12_DetaDphi(nullptr),
h_R2_B21_DetaDphi(nullptr),
h_R2_Bs_DetaDphi(nullptr),

h_C2_CD_DyDphi(nullptr),
h_C2_CI_DyDphi(nullptr),
h_C2N_B12_DyDphi(nullptr),
h_C2N_B21_DyDphi(nullptr),
h_C2N_Bs_DyDphi(nullptr),
h_R2_B12_DyDphi(nullptr),
h_R2_B21_DyDphi(nullptr),
h_R2_Bs_DyDphi(nullptr)
{

}

ParticlePairBfHistos & ParticlePairBfHistos::operator=(const ParticlePairBfHistos & rhs)
{
  if (this!=&rhs)
    {
    ParticlePairDerivedHistos::operator=(rhs);
    cloneBF(rhs);
    }
  return *this;
}

void ParticlePairBfHistos::configure(const String & taskName,
                                           const String & objectType,
                                           const Configuration & configuration,
                                           unsigned int index)
{
  if (reportDebug(__FUNCTION__)) { printCR();}
  ParticlePairDerivedHistos::configure(taskName,objectType,configuration,index);
}

void ParticlePairBfHistos::create()
{
  if (reportDebug(__FUNCTION__)) { printCR();}
  h_C2_CD_ptpt    = createHistogram(createName(_histogramBaseName,"C2_CD_ptpt"),    pt_nbins,pt_min,pt_max,pt_nbins,pt_min,pt_max, "p_{T,1}","p_{T,2}", "C_{2}^{CD}");
  h_C2_CI_ptpt    = createHistogram(createName(_histogramBaseName,"C2_CI_ptpt"),    pt_nbins,pt_min,pt_max,pt_nbins,pt_min,pt_max, "p_{T,1}","p_{T,2}", "C_{2}^{CI}");
  h_C2N_B12_ptpt  = createHistogram(createName(_histogramBaseName,"C2N_B12_ptpt"),  pt_nbins,pt_min,pt_max,pt_nbins,pt_min,pt_max, "p_{T,1}","p_{T,2}", "B_{C2N}^{12}");
  h_C2N_B21_ptpt  = createHistogram(createName(_histogramBaseName,"C2N_B21_ptpt"),  pt_nbins,pt_min,pt_max,pt_nbins,pt_min,pt_max, "p_{T,1}","p_{T,2}", "B_{C2N}^{21}");
  h_C2N_Bs_ptpt   = createHistogram(createName(_histogramBaseName,"C2N_Bs_ptpt"),   pt_nbins,pt_min,pt_max,pt_nbins,pt_min,pt_max, "p_{T,1}","p_{T,2}", "B_{C2N}^{s}");
  h_R2_B12_ptpt   = createHistogram(createName(_histogramBaseName,"R2_B12_ptpt"),   pt_nbins,pt_min,pt_max,pt_nbins,pt_min,pt_max, "p_{T,1}","p_{T,2}", "B_{R2}^{12}");
  h_R2_B21_ptpt   = createHistogram(createName(_histogramBaseName,"R2_B21_ptpt"),   pt_nbins,pt_min,pt_max,pt_nbins,pt_min,pt_max, "p_{T,1}","p_{T,2}", "B_{R2}^{21}");
  h_R2_Bs_ptpt    = createHistogram(createName(_histogramBaseName,"R2_Bs_ptpt"),    pt_nbins,pt_min,pt_max,pt_nbins,pt_min,pt_max, "p_{T,1}","p_{T,2}", "B_{R2}^{s}");

  h_C2_CD_phiphi   = createHistogram(createName(_histogramBaseName,"C2_CD_phiphi"),   phi_nbins,phi_min,phi_max,phi_nbins,phi_min,phi_max, "#varphi_{1}","#varphi_{2}", "C_{2}^{CD}");
  h_C2_CI_phiphi   = createHistogram(createName(_histogramBaseName,"C2_CI_phiphi"),   phi_nbins,phi_min,phi_max,phi_nbins,phi_min,phi_max, "#varphi_{1}","#varphi_{2}", "C_{2}^{CI}");
  h_C2N_B12_phiphi = createHistogram(createName(_histogramBaseName,"C2N_B12_phiphi"), phi_nbins,phi_min,phi_max,phi_nbins,phi_min,phi_max, "#varphi_{1}","#varphi_{2}", "B_{C2N}^{12}");
  h_C2N_B21_phiphi = createHistogram(createName(_histogramBaseName,"C2N_B21_phiphi"), phi_nbins,phi_min,phi_max,phi_nbins,phi_min,phi_max, "#varphi_{1}","#varphi_{2}", "B_{C2N}^{21}");
  h_C2N_Bs_phiphi  = createHistogram(createName(_histogramBaseName,"C2N_Bs_phiphi"),  phi_nbins,phi_min,phi_max,phi_nbins,phi_min,phi_max, "#varphi_{1}","#varphi_{2}", "B_{C2N}^{s}");
  h_R2_B12_phiphi  = createHistogram(createName(_histogramBaseName,"R2_B12_phiphi"),  phi_nbins,phi_min,phi_max,phi_nbins,phi_min,phi_max, "#varphi_{1}","#varphi_{2}", "B_{R2}^{12}");
  h_R2_B21_phiphi  = createHistogram(createName(_histogramBaseName,"R2_B21_phiphi"),  phi_nbins,phi_min,phi_max,phi_nbins,phi_min,phi_max, "#varphi_{1}","#varphi_{2}", "B_{R2}^{21}");
  h_R2_Bs_phiphi   = createHistogram(createName(_histogramBaseName,"R2_Bs_phiphi"),   phi_nbins,phi_min,phi_max,phi_nbins,phi_min,phi_max, "#varphi_{1}","#varphi_{2}", "B_{R2}^{s}");

if (eta_fill)
  {
  h_C2_CD_etaeta   = createHistogram(createName(_histogramBaseName,"C2_CD_etaeta"),   eta_nbins,eta_min,eta_max,eta_nbins,eta_min,eta_max, "#eta_{1}","#eta_{2}", "C_{2}^{CD}");
  h_C2_CI_etaeta   = createHistogram(createName(_histogramBaseName,"C2_CI_etaeta"),   eta_nbins,eta_min,eta_max,eta_nbins,eta_min,eta_max, "#eta_{1}","#eta_{2}", "C_{2}^{CI}");
  h_C2N_B12_etaeta = createHistogram(createName(_histogramBaseName,"C2N_B12_etaeta"), eta_nbins,eta_min,eta_max,eta_nbins,eta_min,eta_max, "#eta_{1}","#eta_{2}", "B_{C2N}^{12}");
  h_C2N_B21_etaeta = createHistogram(createName(_histogramBaseName,"C2N_B21_etaeta"), eta_nbins,eta_min,eta_max,eta_nbins,eta_min,eta_max, "#eta_{1}","#eta_{2}", "B_{C2N}^{21}");
  h_C2N_Bs_etaeta  = createHistogram(createName(_histogramBaseName,"C2N_Bs_etaeta"),  eta_nbins,eta_min,eta_max,eta_nbins,eta_min,eta_max, "#eta_{1}","#eta_{2}", "B_{C2N}^{s}");
  h_R2_B12_etaeta  = createHistogram(createName(_histogramBaseName,"R2_B12_etaeta"),  eta_nbins,eta_min,eta_max,eta_nbins,eta_min,eta_max, "#eta_{1}","#eta_{2}", "B_{R2}^{12}");
  h_R2_B21_etaeta  = createHistogram(createName(_histogramBaseName,"R2_B21_etaeta"),  eta_nbins,eta_min,eta_max,eta_nbins,eta_min,eta_max, "#eta_{1}","#eta_{2}", "B_{R2}^{21}");
  h_R2_Bs_etaeta   = createHistogram(createName(_histogramBaseName,"R2_Bs_etaeta"),   eta_nbins,eta_min,eta_max,eta_nbins,eta_min,eta_max, "#eta_{1}","#eta_{2}", "B_{R2}^{s}");

  h_C2_CD_DetaDphi    = createHistogram(createName(_histogramBaseName,"C2_CD_DetaDphi"),    Deta_nbins,Deta_min,Deta_max,Dphi_nbins,Dphi_min_shft,Dphi_max_shft, "#Delta#eta","#Delta#phi", "C_{2}^{CD}");
  h_C2_CI_DetaDphi    = createHistogram(createName(_histogramBaseName,"C2_CI_DetaDphi"),    Deta_nbins,Deta_min,Deta_max,Dphi_nbins,Dphi_min_shft,Dphi_max_shft, "#Delta#eta","#Delta#phi", "C_{2}^{CI}");
  h_C2N_B12_DetaDphi  = createHistogram(createName(_histogramBaseName,"C2N_B12_DetaDphi"),  Deta_nbins,Deta_min,Deta_max,Dphi_nbins,Dphi_min_shft,Dphi_max_shft, "#Delta#eta","#Delta#phi", "B_{C2N}^{12}");
  h_C2N_B21_DetaDphi  = createHistogram(createName(_histogramBaseName,"C2N_B21_DetaDphi"),  Deta_nbins,Deta_min,Deta_max,Dphi_nbins,Dphi_min_shft,Dphi_max_shft, "#Delta#eta","#Delta#phi", "B_{C2N}^{21}");
  h_C2N_Bs_DetaDphi   = createHistogram(createName(_histogramBaseName,"C2N_Bs_DetaDphi"),   Deta_nbins,Deta_min,Deta_max,Dphi_nbins,Dphi_min_shft,Dphi_max_shft, "#Delta#eta","#Delta#phi", "B_{C2N}^{s}");
  h_R2_B12_DetaDphi   = createHistogram(createName(_histogramBaseName,"R2_B12_DetaDphi"),   Deta_nbins,Deta_min,Deta_max,Dphi_nbins,Dphi_min_shft,Dphi_max_shft, "#Delta#eta","#Delta#phi", "B_{R2}^{12}");
  h_R2_B21_DetaDphi   = createHistogram(createName(_histogramBaseName,"R2_B21_DetaDphi"),   Deta_nbins,Deta_min,Deta_max,Dphi_nbins,Dphi_min_shft,Dphi_max_shft, "#Delta#eta","#Delta#phi", "B_{R2}^{21}");
  h_R2_Bs_DetaDphi    = createHistogram(createName(_histogramBaseName,"R2_Bs_DetaDphi"),    Deta_nbins,Deta_min,Deta_max,Dphi_nbins,Dphi_min_shft,Dphi_max_shft, "#Delta#eta","#Delta#phi", "B_{R2}^{s}");
  }

  if (rapidity_fill)
    {
    h_C2_CD_yY     = createHistogram(createName(_histogramBaseName,"C2_CD_yY"),    rapidity_nbins,rapidity_minimum,rapidity_maximum,rapidity_nbins,rapidity_minimum,rapidity_maximum, "y_{1}","y_{2}", "C_{2}^{CD}");
    h_C2_CI_yY     = createHistogram(createName(_histogramBaseName,"C2_CI_yY"),    rapidity_nbins,rapidity_minimum,rapidity_maximum,rapidity_nbins,rapidity_minimum,rapidity_maximum, "y_{1}","y_{2}", "C_{2}^{CI}");
    h_C2N_B12_yY   = createHistogram(createName(_histogramBaseName,"C2N_B12_yY"),  rapidity_nbins,rapidity_minimum,rapidity_maximum,rapidity_nbins,rapidity_minimum,rapidity_maximum, "y_{1}","y_{2}", "B_{C2N}^{12}");
    h_C2N_B21_yY   = createHistogram(createName(_histogramBaseName,"C2N_B21_yY"),  rapidity_nbins,rapidity_minimum,rapidity_maximum,rapidity_nbins,rapidity_minimum,rapidity_maximum, "y_{1}","y_{2}", "B_{C2N}^{21}");
    h_C2N_Bs_yY    = createHistogram(createName(_histogramBaseName,"C2N_Bs_yY"),   rapidity_nbins,rapidity_minimum,rapidity_maximum,rapidity_nbins,rapidity_minimum,rapidity_maximum, "y_{1}","y_{2}", "B_{C2N}^{s}");
    h_R2_B12_yY    = createHistogram(createName(_histogramBaseName,"R2_B12_yY"),   rapidity_nbins,rapidity_minimum,rapidity_maximum,rapidity_nbins,rapidity_minimum,rapidity_maximum, "y_{1}","y_{2}", "B_{R2}^{12}");
    h_R2_B21_yY    = createHistogram(createName(_histogramBaseName,"R2_B21_yY"),   rapidity_nbins,rapidity_minimum,rapidity_maximum,rapidity_nbins,rapidity_minimum,rapidity_maximum, "y_{1}","y_{2}", "B_{R2}^{21}");
    h_R2_Bs_yY     = createHistogram(createName(_histogramBaseName,"R2_Bs_yY"),    rapidity_nbins,rapidity_minimum,rapidity_maximum,rapidity_nbins,rapidity_minimum,rapidity_maximum, "y_{1}","y_{2}", "B_{R2}^{s}");

    h_C2_CD_DyDphi     = createHistogram(createName(_histogramBaseName,"C2_CD_DyDphi"),    Drapidity_nbins,Drapidity_min,Drapidity_max,Dphi_nbins,Dphi_min_shft,Dphi_max_shft, "#Delta y","#Delta#phi", "C_{2}^{CD}");
    h_C2_CI_DyDphi     = createHistogram(createName(_histogramBaseName,"C2_CI_DyDphi"),    Drapidity_nbins,Drapidity_min,Drapidity_max,Dphi_nbins,Dphi_min_shft,Dphi_max_shft, "#Delta y","#Delta#phi", "C_{2}^{CI}");
    h_C2N_B12_DyDphi   = createHistogram(createName(_histogramBaseName,"C2N_B12_DyDphi"),  Drapidity_nbins,Drapidity_min,Drapidity_max,Dphi_nbins,Dphi_min_shft,Dphi_max_shft, "#Delta y","#Delta#phi", "B_{C2N}^{12}");
    h_C2N_B21_DyDphi   = createHistogram(createName(_histogramBaseName,"C2N_B21_DyDphi"),  Drapidity_nbins,Drapidity_min,Drapidity_max,Dphi_nbins,Dphi_min_shft,Dphi_max_shft, "#Delta y","#Delta#phi", "B_{C2N}^{21}");
    h_C2N_Bs_DyDphi    = createHistogram(createName(_histogramBaseName,"C2N_Bs_DyDphi"),   Drapidity_nbins,Drapidity_min,Drapidity_max,Dphi_nbins,Dphi_min_shft,Dphi_max_shft, "#Delta y","#Delta#phi", "B_{C2N}^{s}");
    h_R2_B12_DyDphi    = createHistogram(createName(_histogramBaseName,"R2_B12_DyDphi"),   Drapidity_nbins,Drapidity_min,Drapidity_max,Dphi_nbins,Dphi_min_shft,Dphi_max_shft, "#Delta y","#Delta#phi", "B_{R2}^{12}");
    h_R2_B21_DyDphi    = createHistogram(createName(_histogramBaseName,"R2_B21_DyDphi"),   Drapidity_nbins,Drapidity_min,Drapidity_max,Dphi_nbins,Dphi_min_shft,Dphi_max_shft, "#Delta y","#Delta#phi", "B_{R2}^{21}");
    h_R2_Bs_DyDphi     = createHistogram(createName(_histogramBaseName,"R2_Bs_DyDphi"),    Drapidity_nbins,Drapidity_min,Drapidity_max,Dphi_nbins,Dphi_min_shft,Dphi_max_shft, "#Delta y","#Delta#phi", "B_{R2}^{s}");
    }
}


void ParticlePairBfHistos::loadFrom(TFile & inputFile)
{
  if (reportDebug(__FUNCTION__)) { printCR(); }
  ParticlePairDerivedHistos::loadFrom(inputFile);
  h_C2_CD_ptpt    = loadH2(inputFile,createName(_histogramBaseName,"C2_CD_ptpt"));
  h_C2_CI_ptpt    = loadH2(inputFile,createName(_histogramBaseName,"C2_CI_ptpt"));
  h_C2N_B12_ptpt  = loadH2(inputFile,createName(_histogramBaseName,"C2N_B12_ptpt"));
  h_C2N_B21_ptpt  = loadH2(inputFile,createName(_histogramBaseName,"C2N_B21_ptpt"));
  h_C2N_Bs_ptpt   = loadH2(inputFile,createName(_histogramBaseName,"C2N_Bs_ptpt"));
  h_R2_B12_ptpt   = loadH2(inputFile,createName(_histogramBaseName,"R2_B12_ptpt"));
  h_R2_B21_ptpt   = loadH2(inputFile,createName(_histogramBaseName,"R2_B21_ptpt"));
  h_R2_Bs_ptpt    = loadH2(inputFile,createName(_histogramBaseName,"R2_Bs_ptpt"));

  h_C2_CD_phiphi   = loadH2(inputFile,createName(_histogramBaseName,"C2_CD_phiphi"));
  h_C2_CI_phiphi   = loadH2(inputFile,createName(_histogramBaseName,"C2_CI_phiphi"));
  h_C2N_B12_phiphi = loadH2(inputFile,createName(_histogramBaseName,"C2N_B12_phiphi"));
  h_C2N_B21_phiphi = loadH2(inputFile,createName(_histogramBaseName,"C2N_B21_phiphi"));
  h_C2N_Bs_phiphi  = loadH2(inputFile,createName(_histogramBaseName,"C2N_Bs_phiphi"));
  h_R2_B12_phiphi  = loadH2(inputFile,createName(_histogramBaseName,"R2_B12_phiphi"));
  h_R2_B21_phiphi  = loadH2(inputFile,createName(_histogramBaseName,"R2_B21_phiphi"));
  h_R2_Bs_phiphi   = loadH2(inputFile,createName(_histogramBaseName,"R2_Bs_phiphi"));

  if (eta_fill)
    {
    h_C2_CD_etaeta   = loadH2(inputFile,createName(_histogramBaseName,"C2_CD_etaeta"));
    h_C2_CI_etaeta   = loadH2(inputFile,createName(_histogramBaseName,"C2_CI_etaeta"));
    h_C2N_B12_etaeta = loadH2(inputFile,createName(_histogramBaseName,"C2N_B12_etaeta"));
    h_C2N_B21_etaeta = loadH2(inputFile,createName(_histogramBaseName,"C2N_B21_etaeta"));
    h_C2N_Bs_etaeta  = loadH2(inputFile,createName(_histogramBaseName,"C2N_Bs_etaeta"));
    h_R2_B12_etaeta  = loadH2(inputFile,createName(_histogramBaseName,"R2_B12_etaeta"));
    h_R2_B21_etaeta  = loadH2(inputFile,createName(_histogramBaseName,"R2_B21_etaeta"));
    h_R2_Bs_etaeta   = loadH2(inputFile,createName(_histogramBaseName,"R2_Bs_etaeta"));

    h_C2_CD_DetaDphi    = loadH2(inputFile,createName(_histogramBaseName,"C2_CD_DetaDphi_shft"));
    h_C2_CI_DetaDphi    = loadH2(inputFile,createName(_histogramBaseName,"C2_CI_DetaDphi_shft"));
    h_C2N_B12_DetaDphi  = loadH2(inputFile,createName(_histogramBaseName,"C2N_B12_DetaDphi_shft"));
    h_C2N_B21_DetaDphi  = loadH2(inputFile,createName(_histogramBaseName,"C2N_B21_DetaDphi_shft"));
    h_C2N_Bs_DetaDphi   = loadH2(inputFile,createName(_histogramBaseName,"C2N_Bs_DetaDphi_shft"));
    h_R2_B12_DetaDphi   = loadH2(inputFile,createName(_histogramBaseName,"R2_B12_DetaDphi_shft"));
    h_R2_B21_DetaDphi   = loadH2(inputFile,createName(_histogramBaseName,"R2_B21_DetaDphi_shft"));
    h_R2_Bs_DetaDphi    = loadH2(inputFile,createName(_histogramBaseName,"R2_Bs_DetaDphi_shft"));
    }

  if (rapidity_fill)
    {
    h_C2_CD_yY     = loadH2(inputFile,createName(_histogramBaseName,"C2_CD_yY"));
    h_C2_CI_yY     = loadH2(inputFile,createName(_histogramBaseName,"C2_CI_yY"));
    h_C2N_B12_yY   = loadH2(inputFile,createName(_histogramBaseName,"C2N_B12_yY"));
    h_C2N_B21_yY   = loadH2(inputFile,createName(_histogramBaseName,"C2N_B21_yY"));
    h_C2N_Bs_yY    = loadH2(inputFile,createName(_histogramBaseName,"C2N_Bs_yY"));
    h_R2_B12_yY    = loadH2(inputFile,createName(_histogramBaseName,"R2_B12_yY"));
    h_R2_B21_yY    = loadH2(inputFile,createName(_histogramBaseName,"R2_B21_yY"));
    h_R2_Bs_yY     = loadH2(inputFile,createName(_histogramBaseName,"R2_Bs_yY"));

    h_C2_CD_DyDphi     = loadH2(inputFile,createName(_histogramBaseName,"C2_CD_DyDphi_shft"));
    h_C2_CI_DyDphi     = loadH2(inputFile,createName(_histogramBaseName,"C2_CI_DyDphi_shft"));
    h_C2N_B12_DyDphi   = loadH2(inputFile,createName(_histogramBaseName,"C2N_B12_DyDphi_shft"));
    h_C2N_B21_DyDphi   = loadH2(inputFile,createName(_histogramBaseName,"C2N_B21_DyDphi_shft"));
    h_C2N_Bs_DyDphi    = loadH2(inputFile,createName(_histogramBaseName,"C2N_Bs_DyDphi_shft"));
    h_R2_B12_DyDphi    = loadH2(inputFile,createName(_histogramBaseName,"R2_B12_DyDphi_shft"));
    h_R2_B21_DyDphi    = loadH2(inputFile,createName(_histogramBaseName,"R2_B21_DyDphi_shft"));
    h_R2_Bs_DyDphi     = loadH2(inputFile,createName(_histogramBaseName,"R2_Bs_DyDphi_shft"));
    }
}

void ParticlePairBfHistos::cloneBF(const ParticlePairBfHistos & source)
{
  h_C2_CD_ptpt    = safeCloneH2(source.h_C2_CD_ptpt);
  h_C2_CI_ptpt    = safeCloneH2(source.h_C2_CI_ptpt);
  h_C2N_B12_ptpt  = safeCloneH2(source.h_C2N_B12_ptpt);
  h_C2N_B21_ptpt  = safeCloneH2(source.h_C2N_B21_ptpt);
  h_C2N_Bs_ptpt   = safeCloneH2(source.h_C2N_Bs_ptpt);
  h_R2_B12_ptpt   = safeCloneH2(source.h_R2_B12_ptpt);
  h_R2_B21_ptpt   = safeCloneH2(source.h_R2_B21_ptpt);
  h_R2_Bs_ptpt    = safeCloneH2(source.h_R2_Bs_ptpt);

  h_C2_CD_phiphi   = safeCloneH2(source.h_C2_CD_phiphi);
  h_C2_CI_phiphi   = safeCloneH2(source.h_C2_CI_phiphi);
  h_C2N_B12_phiphi = safeCloneH2(source.h_C2N_B12_phiphi);
  h_C2N_B21_phiphi = safeCloneH2(source.h_C2N_B21_phiphi);
  h_C2N_Bs_phiphi  = safeCloneH2(source.h_C2N_Bs_phiphi);
  h_R2_B12_phiphi  = safeCloneH2(source.h_R2_B12_phiphi);
  h_R2_B21_phiphi  = safeCloneH2(source.h_R2_B21_phiphi);
  h_R2_Bs_phiphi   = safeCloneH2(source.h_R2_Bs_phiphi);

  h_C2_CD_etaeta   = safeCloneH2(source.h_C2_CD_etaeta);
  h_C2_CI_etaeta   = safeCloneH2(source.h_C2_CI_etaeta);
  h_C2N_B12_etaeta = safeCloneH2(source.h_C2N_B12_etaeta);
  h_C2N_B21_etaeta = safeCloneH2(source.h_C2N_B21_etaeta);
  h_C2N_Bs_etaeta  = safeCloneH2(source.h_C2N_Bs_etaeta);
  h_R2_B12_etaeta  = safeCloneH2(source.h_R2_B12_etaeta);
  h_R2_B21_etaeta  = safeCloneH2(source.h_R2_B21_etaeta);
  h_R2_Bs_etaeta   = safeCloneH2(source.h_R2_Bs_etaeta);

  h_C2_CD_DetaDphi    = safeCloneH2(source.h_C2_CD_DetaDphi);
  h_C2_CI_DetaDphi    = safeCloneH2(source.h_C2_CI_DetaDphi);
  h_C2N_B12_DetaDphi  = safeCloneH2(source.h_C2N_B12_DetaDphi);
  h_C2N_B21_DetaDphi  = safeCloneH2(source.h_C2N_B21_DetaDphi);
  h_C2N_Bs_DetaDphi   = safeCloneH2(source.h_C2N_Bs_DetaDphi);
  h_R2_B12_DetaDphi   = safeCloneH2(source.h_R2_B12_DetaDphi);
  h_R2_B21_DetaDphi   = safeCloneH2(source.h_R2_B21_DetaDphi);
  h_R2_Bs_DetaDphi    = safeCloneH2(source.h_R2_Bs_DetaDphi);

  h_C2_CD_yY     = safeCloneH2(source.h_C2_CD_yY);
  h_C2_CI_yY     = safeCloneH2(source.h_C2_CI_yY);
  h_C2N_B12_yY   = safeCloneH2(source.h_C2N_B12_yY);
  h_C2N_B21_yY   = safeCloneH2(source.h_C2N_B21_yY);
  h_C2N_Bs_yY    = safeCloneH2(source.h_C2N_Bs_yY);
  h_R2_B12_yY    = safeCloneH2(source.h_R2_B12_yY);
  h_R2_B21_yY    = safeCloneH2(source.h_R2_B21_yY);
  h_R2_Bs_yY     = safeCloneH2(source.h_R2_Bs_yY);

  h_C2_CD_DyDphi     = safeCloneH2(source.h_C2_CD_DyDphi);
  h_C2_CI_DyDphi     = safeCloneH2(source.h_C2_CI_DyDphi);
  h_C2N_B12_DyDphi   = safeCloneH2(source.h_C2N_B12_DyDphi);
  h_C2N_B21_DyDphi   = safeCloneH2(source.h_C2N_B21_DyDphi);
  h_C2N_Bs_DyDphi    = safeCloneH2(source.h_C2N_Bs_DyDphi);
  h_R2_B12_DyDphi    = safeCloneH2(source.h_R2_B12_DyDphi);
  h_R2_B21_DyDphi    = safeCloneH2(source.h_R2_B21_DyDphi);
  h_R2_Bs_DyDphi     = safeCloneH2(source.h_R2_Bs_DyDphi);
}


//!
//! HistogramGroup from ParticlePairBfHistos must be normalized "per event" before calling this function
//!
void ParticlePairBfHistos::calculateBfHistograms(ParticlePairDerivedHistos & pair21B,
                                                 ParticlePairDerivedHistos & pair2B1B,
                                                 ParticlePairDerivedHistos & pair2B1,
                                                 ParticlePairDerivedHistos & pair21)
{
  if (reportDebug(__FUNCTION__)) { printCR(); }
  if (pair21B.h_C2_ptpt)
    {
    h_C2_CD_ptpt    ->Add(pair21B.h_C2_ptpt,pair2B1B.h_C2_ptpt,1.0,-1.0);
    h_C2_CD_ptpt    ->Add(pair2B1.h_C2_ptpt,+1.0);
    h_C2_CD_ptpt    ->Add(pair21.h_C2_ptpt, -1.0);
    printString("ParticlePairBfHistos::calculateBfHistograms -- 21 --");
    h_C2_CI_ptpt    ->Add(pair2B1.h_C2_ptpt,pair2B1B.h_C2_ptpt,1.0,+1.0);
    h_C2_CI_ptpt    ->Add(pair2B1.h_C2_ptpt);
    h_C2_CI_ptpt    ->Add(pair21.h_C2_ptpt);
    printString("ParticlePairBfHistos::calculateBfHistograms -- 3 --");
    h_C2N_B12_ptpt  ->Add(pair21B.h_C2_ptpt,pair2B1B.h_C2_ptpt,1.0,-1.0);
    h_C2N_B21_ptpt  ->Add(pair2B1.h_C2_ptpt,pair21.h_C2_ptpt,1.0,-1.0);
    h_C2N_Bs_ptpt   ->Add(h_C2N_B12_ptpt,h_C2N_B21_ptpt,0.5,0.5);
    printString("ParticlePairBfHistos::calculateBfHistograms -- 4 --");
    h_R2_B12_ptpt   ->Add(pair21B.h_R2_ptpt,pair2B1B.h_R2_ptpt,1.0,-1.0);
    h_R2_B21_ptpt   ->Add(pair2B1.h_R2_ptpt,pair21.h_R2_ptpt,1.0,-1.0);
    h_R2_Bs_ptpt    ->Add(h_R2_B12_ptpt,h_R2_B21_ptpt,0.5,0.5);
    }
  printString("ParticlePairBfHistos::calculateBfHistograms -- 5 --");

  if (pair21B.h_C2_phiphi)
    {
    h_C2_CD_phiphi   ->Add(pair21B.h_C2_phiphi,pair2B1B.h_C2_phiphi,1.0,-1.0);
    h_C2_CD_phiphi   ->Add(pair2B1.h_C2_phiphi,+1.0);
    h_C2_CD_phiphi   ->Add(pair21.h_C2_phiphi, -1.0);
    printString("ParticlePairBfHistos::calculateBfHistograms -- 6 --");
    h_C2_CI_phiphi   ->Add(pair21B.h_C2_phiphi,pair2B1B.h_C2_phiphi,1.0,1.0);
    h_C2_CI_phiphi   ->Add(pair2B1.h_C2_phiphi,+1.0);
    h_C2_CI_phiphi   ->Add(pair21.h_C2_phiphi, +1.0);
    h_C2N_B12_phiphi ->Add(pair21B.h_C2_phiphi,pair2B1B.h_C2_phiphi,1.0,-1.0);
    h_C2N_B21_phiphi ->Add(pair2B1.h_C2_phiphi,pair21.h_C2_phiphi,1.0,-1.0);
    h_C2N_Bs_phiphi  ->Add(h_C2N_B12_phiphi,h_C2N_B21_phiphi,0.5,0.5);
    h_R2_B12_phiphi  ->Add(pair21B.h_R2_phiphi,pair2B1B.h_R2_phiphi,1.0,-1.0);
    h_R2_B21_phiphi  ->Add(pair2B1.h_R2_phiphi,pair21.h_R2_phiphi,1.0,-1.0);
    h_R2_Bs_phiphi   ->Add(h_R2_B12_phiphi,h_R2_B21_phiphi,0.5,0.5);
    }
  printString("ParticlePairBfHistos::calculateBfHistograms -- 7 --");

  if (pair21B.h_C2_etaeta)
    {
    h_C2_CD_etaeta   ->Add(pair21B.h_C2_etaeta,pair2B1B.h_C2_etaeta,1.0,-1.0);
    h_C2_CD_etaeta   ->Add(pair2B1.h_C2_etaeta,+1.0);
    h_C2_CD_etaeta   ->Add(pair21.h_C2_etaeta, -1.0);
    h_C2_CI_etaeta   ->Add(pair21B.h_C2_etaeta,pair2B1B.h_C2_etaeta,1.0,1.0);
    h_C2_CI_etaeta   ->Add(pair2B1.h_C2_etaeta,+1.0);
    h_C2_CI_etaeta   ->Add(pair21.h_C2_etaeta, +1.0);
    printString("ParticlePairBfHistos::calculateBfHistograms -- 8 --");
    h_C2N_B12_etaeta ->Add(pair21B.h_C2_etaeta,pair2B1B.h_C2_etaeta,1.0,-1.0);
    h_C2N_B21_etaeta ->Add(pair2B1.h_C2_etaeta,pair21.h_C2_etaeta,1.0,-1.0);
    h_C2N_Bs_etaeta  ->Add(h_C2N_B12_etaeta,h_C2N_B21_etaeta,0.5,0.5);
    h_R2_B12_etaeta  ->Add(pair21B.h_R2_etaeta,pair2B1B.h_R2_etaeta,1.0,-1.0);
    h_R2_B21_etaeta  ->Add(pair2B1.h_R2_etaeta,pair21.h_R2_etaeta,1.0,-1.0);
    h_R2_Bs_etaeta   ->Add(h_R2_B12_etaeta,h_R2_B21_etaeta,0.5,0.5);
    }
  printString("ParticlePairBfHistos::calculateBfHistograms -- 9 --");

  if (pair21B.h_C2_yY)
    {
    h_C2_CD_yY   ->Add(pair21B.h_C2_yY,pair2B1B.h_C2_yY,1.0,-1.0);
    h_C2_CD_yY   ->Add(pair2B1.h_C2_yY,+1.0);
    h_C2_CD_yY   ->Add(pair21.h_C2_yY, -1.0);
    h_C2_CI_yY   ->Add(pair21B.h_C2_yY,pair2B1B.h_C2_yY,1.0,1.0);
    h_C2_CI_yY   ->Add(pair2B1.h_C2_yY,+1.0);
    h_C2_CI_yY   ->Add(pair21.h_C2_yY, +1.0);
    h_C2N_B12_yY ->Add(pair21B.h_C2_yY,pair2B1B.h_C2_yY,1.0,-1.0);
    h_C2N_B21_yY ->Add(pair2B1.h_C2_yY,pair21.h_C2_yY,1.0,-1.0);
    h_C2N_Bs_yY  ->Add(h_C2N_B12_yY,h_C2N_B21_yY,0.5,0.5);
    h_R2_B12_yY  ->Add(pair21B.h_R2_yY,pair2B1B.h_R2_yY,1.0,-1.0);
    h_R2_B21_yY  ->Add(pair2B1.h_R2_yY,pair21.h_R2_yY,1.0,-1.0);
    h_R2_Bs_yY   ->Add(h_R2_B12_yY,h_R2_B21_yY,0.5,0.5);
    }
  printString("ParticlePairBfHistos::calculateBfHistograms -- 10 --");

  if (pair21B.h_C2_DetaDphi)
    {
    h_C2_CD_DetaDphi   ->Add(pair21B.h_C2_DetaDphi_shft, pair2B1B.h_C2_DetaDphi_shft,1.0,-1.0);
    h_C2_CD_DetaDphi   ->Add(pair2B1.h_C2_DetaDphi_shft, +1.0);
    h_C2_CD_DetaDphi   ->Add(pair21.h_C2_DetaDphi_shft,  -1.0);
    h_C2_CI_DetaDphi   ->Add(pair21B.h_C2_DetaDphi_shft, pair2B1B.h_C2_DetaDphi_shft,1.0,1.0);
    h_C2_CI_DetaDphi   ->Add(pair2B1.h_C2_DetaDphi_shft, +1.0);
    h_C2_CI_DetaDphi   ->Add(pair21.h_C2_DetaDphi_shft,  +1.0);
    h_C2N_B12_DetaDphi ->Add(pair21B.h_C2_DetaDphi_shft, pair2B1B.h_C2_DetaDphi_shft,1.0,-1.0);
    h_C2N_B21_DetaDphi ->Add(pair2B1.h_C2_DetaDphi_shft, pair21.h_C2_DetaDphi_shft,1.0,-1.0);
    h_C2N_Bs_DetaDphi  ->Add(h_C2N_B12_DetaDphi,         h_C2N_B21_DetaDphi,0.5,0.5);
    h_R2_B12_DetaDphi  ->Add(pair21B.h_R2_DetaDphi_shft, pair2B1B.h_R2_DetaDphi_shft,1.0,-1.0);
    h_R2_B21_DetaDphi  ->Add(pair2B1.h_R2_DetaDphi_shft, pair21.h_R2_DetaDphi_shft,1.0,-1.0);
    h_R2_Bs_DetaDphi   ->Add(h_R2_B12_DetaDphi,          h_R2_B21_DetaDphi,0.5,0.5);
    }
  printString("ParticlePairBfHistos::calculateBfHistograms -- 11 --");

  if (pair21B.h_C2_DyDphi)
    {
    h_C2_CD_DyDphi   ->Add(pair21B.h_C2_DyDphi_shft, pair2B1B.h_C2_DyDphi_shft,1.0,-1.0);
    h_C2_CD_DyDphi   ->Add(pair2B1.h_C2_DyDphi_shft, +1.0);
    h_C2_CD_DyDphi   ->Add(pair21.h_C2_DyDphi_shft,  -1.0);
    h_C2_CI_DyDphi   ->Add(pair21B.h_C2_DyDphi_shft, pair2B1B.h_C2_DyDphi_shft,1.0,1.0);
    h_C2_CI_DyDphi   ->Add(pair2B1.h_C2_DyDphi_shft, +1.0);
    h_C2_CI_DyDphi   ->Add(pair21.h_C2_DyDphi_shft,  +1.0);
    h_C2N_B12_DyDphi ->Add(pair21B.h_C2_DyDphi_shft, pair2B1B.h_C2_DyDphi_shft,1.0,-1.0);
    h_C2N_B21_DyDphi ->Add(pair2B1.h_C2_DyDphi_shft, pair21.h_C2_DyDphi_shft,1.0,-1.0);
    h_C2N_Bs_DyDphi  ->Add(h_C2N_B12_DyDphi,         h_C2N_B21_DyDphi,0.5,0.5);
    h_R2_B12_DyDphi  ->Add(pair21B.h_R2_DyDphi_shft, pair2B1B.h_R2_DyDphi_shft,1.0,-1.0);
    h_R2_B21_DyDphi  ->Add(pair2B1.h_R2_DyDphi_shft, pair21.h_R2_DyDphi_shft,1.0,-1.0);
    h_R2_Bs_DyDphi   ->Add(h_R2_B12_DyDphi,          h_R2_B21_DyDphi,0.5,0.5);
    }
}


} //  namespace CAP
