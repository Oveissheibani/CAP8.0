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
#include "ParticlePair3DDerivedHistos.hpp"
#include "RootHistogramHelpers.hpp"
#include "NameHelpers.hpp"
#include "PrintHelpers.hpp"
#include "TRandom.h"
#include "Task.hpp"

ClassImp(CAP::ParticlePair3DDerivedHistos);

namespace CAP
{
ParticlePair3DDerivedHistos::ParticlePair3DDerivedHistos()
:
ParticlePair3DHistos(),
h_n1n1_Qinv(nullptr),
h_n1n1_DeltaP(nullptr),
h_n1n1_DeltaPs(nullptr),
h_n1n1_DeltaPo(nullptr),
h_n1n1_DeltaPl(nullptr),
h_n2_DeltaPs(nullptr),
h_n2_DeltaPo(nullptr),
h_n2_DeltaPl(nullptr),
h_c2_Qinv(nullptr),
h_c2_DeltaP(nullptr),
h_c2_DeltaPs(nullptr),
h_c2_DeltaPo(nullptr),
h_c2_DeltaPl(nullptr),
h_r2_Qinv(nullptr),
h_r2_DeltaP(nullptr),
h_r2_DeltaPs(nullptr),
h_r2_DeltaPo(nullptr),
h_r2_DeltaPl(nullptr),
h_a12_Qinv(nullptr),
h_a12_DeltaPs(nullptr),
h_a12_DeltaPo(nullptr),
h_a12_DeltaPl(nullptr),
h_a21_Qinv(nullptr),
h_a21_DeltaPs(nullptr),
h_a21_DeltaPo(nullptr),
h_a21_DeltaPl(nullptr),
h_n1_1_pt(nullptr),
h_n1_2_pt(nullptr),
h_n1_1_y(nullptr),
h_n1_2_y(nullptr),
h_n1_1_pt_mc(nullptr),
h_n1_2_pt_mc(nullptr),
h_n1_1_y_mc(nullptr),
h_n1_2_y_mc(nullptr),
h_n1r_1_pt_mc(nullptr),
h_n1r_2_pt_mc(nullptr),
h_n1r_1_y_mc(nullptr),
h_n1r_2_y_mc(nullptr),
h_n1_1_ptY_mc(nullptr),
h_n1_2_ptY_mc(nullptr),
h_n1r_1_ptY_mc(nullptr),
h_n1r_2_ptY_mc(nullptr),
mass1(0.13957039),
mass2(0.13957039)
{
  appendClassName("ParticlePair3DDerivedHistos");
  setName("ParticlePair3DDerivedHistos");
  setTitle("ParticlePair3DDerivedHistos");
}

ParticlePair3DDerivedHistos::ParticlePair3DDerivedHistos(const ParticlePair3DDerivedHistos & source)
:
ParticlePair3DHistos(source),
h_n1n1_Qinv(nullptr),
h_n1n1_DeltaP(nullptr),
h_n1n1_DeltaPs(nullptr),
h_n1n1_DeltaPo(nullptr),
h_n1n1_DeltaPl(nullptr),
h_n2_DeltaPs(nullptr),
h_n2_DeltaPo(nullptr),
h_n2_DeltaPl(nullptr),
h_c2_Qinv(nullptr),
h_c2_DeltaP(nullptr),
h_c2_DeltaPs(nullptr),
h_c2_DeltaPo(nullptr),
h_c2_DeltaPl(nullptr),
h_r2_Qinv(nullptr),
h_r2_DeltaP(nullptr),
h_r2_DeltaPs(nullptr),
h_r2_DeltaPo(nullptr),
h_r2_DeltaPl(nullptr),
h_a12_Qinv(nullptr),
h_a12_DeltaPs(nullptr),
h_a12_DeltaPo(nullptr),
h_a12_DeltaPl(nullptr),
h_a21_Qinv(nullptr),
h_a21_DeltaPs(nullptr),
h_a21_DeltaPo(nullptr),
h_a21_DeltaPl(nullptr),
h_n1_1_pt(nullptr),
h_n1_2_pt(nullptr),
h_n1_1_y(nullptr),
h_n1_2_y(nullptr),
h_n1_1_pt_mc(nullptr),
h_n1_2_pt_mc(nullptr),
h_n1_1_y_mc(nullptr),
h_n1_2_y_mc(nullptr),
h_n1r_1_pt_mc(nullptr),
h_n1r_2_pt_mc(nullptr),
h_n1r_1_y_mc(nullptr),
h_n1r_2_y_mc(nullptr),
h_n1_1_ptY_mc(nullptr),
h_n1_2_ptY_mc(nullptr),
h_n1r_1_ptY_mc(nullptr),
h_n1r_2_ptY_mc(nullptr),
mass1(source.mass1),
mass2(source.mass2)
{
  cloneD(source);
}

ParticlePair3DDerivedHistos & ParticlePair3DDerivedHistos::operator=(const ParticlePair3DDerivedHistos & rhs)
{
  if (this!=&rhs)
  {
  ParticlePair3DHistos::operator=(rhs);
  cloneD(rhs);
  mass1 =  rhs.mass1;
  mass2 =  rhs.mass2;
  }
  return *this;
}

void ParticlePair3DDerivedHistos::configure(const String & taskName,
                                                  const String & objectType,
                                                  const Configuration & configuration,
                                                  unsigned int index)
{
  ParticlePair3DHistos::configure(taskName,objectType,configuration,index);
}

void ParticlePair3DDerivedHistos::create()
{
  if (reportDebug(__FUNCTION__)) { printCR(); }
  h_n2_DeltaPs= createHistogram(createName(_histogramBaseName,"n2_DeltaPs"),DeltaPs_nbins,DeltaPs_min,DeltaPs_max,"p_{s}","n_{2}",2);
  h_n2_DeltaPo= createHistogram(createName(_histogramBaseName,"n2_DeltaPo"),DeltaPo_nbins,DeltaPo_min,DeltaPo_max,"p_{o}","n_{2}",2);
  h_n2_DeltaPl= createHistogram(createName(_histogramBaseName,"n2_DeltaPl"),DeltaPl_nbins,DeltaPl_min,DeltaPl_max,"p_{l}","n_{2}",2);

  h_n1n1_Qinv   = createHistogram(createName(_histogramBaseName,"n1n1_Qinv"),Qinv_nbins,Qinv_min,Qinv_max, "Q_{inv}","n_{1}n_{1}",2);
  h_n1n1_DeltaP = createHistogram(createName(_histogramBaseName,"n1n1_DeltaP"),
                                  DeltaPs_nbins,DeltaPs_min,DeltaPs_max,
                                  DeltaPo_nbins,DeltaPo_min,DeltaPo_max,
                                  DeltaPl_nbins,DeltaPl_min,DeltaPl_max,
                                  "p_{s}","p_{o}", "p_{l}");

  h_n1n1_DeltaPs= createHistogram(createName(_histogramBaseName,"n1n1_DeltaPs"),DeltaPs_nbins,DeltaPs_min,DeltaPs_max,"p_{s}","n_{1}n_{1}",2);
  h_n1n1_DeltaPo= createHistogram(createName(_histogramBaseName,"n1n1_DeltaPo"),DeltaPo_nbins,DeltaPo_min,DeltaPo_max,"p_{o}","n_{1}n_{1}",2);
  h_n1n1_DeltaPl= createHistogram(createName(_histogramBaseName,"n1n1_DeltaPl"),DeltaPl_nbins,DeltaPl_min,DeltaPl_max,"p_{l}","n_{1}n_{1}",2);

  h_c2_Qinv   = createHistogram(createName(_histogramBaseName,"c2_Qinv"), Qinv_nbins,Qinv_min,Qinv_max, "Q_{inv}","c_{2}",2);
  h_c2_DeltaP = createHistogram(createName(_histogramBaseName,"c2_DeltaP"),
                                DeltaPs_nbins,DeltaPs_min,DeltaPs_max,
                                DeltaPo_nbins,DeltaPo_min,DeltaPo_max,
                                DeltaPl_nbins,DeltaPl_min,DeltaPl_max,
                                "p_{s}","p_{o}", "p_{l}");
  h_c2_DeltaPs= createHistogram(createName(_histogramBaseName,"c2_DeltaPs"),DeltaPs_nbins,DeltaPs_min,DeltaPs_max,"p_{s}","c_{2}",2);
  h_c2_DeltaPo= createHistogram(createName(_histogramBaseName,"c2_DeltaPo"),DeltaPo_nbins,DeltaPo_min,DeltaPo_max,"p_{o}","c_{2}",2);
  h_c2_DeltaPl= createHistogram(createName(_histogramBaseName,"c2_DeltaPl"),DeltaPl_nbins,DeltaPl_min,DeltaPl_max,"p_{l}","c_{2}",2);

  
  h_r2_Qinv   = createHistogram(createName(_histogramBaseName,"r2_Qinv"), Qinv_nbins,Qinv_min,Qinv_max, "Q_{inv}","r_{2}",2);
  h_r2_DeltaP = createHistogram(createName(_histogramBaseName,"r2_DeltaP"),
                                DeltaPs_nbins,DeltaPs_min,DeltaPs_max,
                                DeltaPo_nbins,DeltaPo_min,DeltaPo_max,
                                DeltaPl_nbins,DeltaPl_min,DeltaPl_max,
                                "p_{s}","p_{o}", "p_{l}");

  h_r2_DeltaPs= createHistogram(createName(_histogramBaseName,"r2_DeltaPs"),DeltaPs_nbins,DeltaPs_min,DeltaPs_max,"p_{s}","r_{2}",2);
  h_r2_DeltaPo= createHistogram(createName(_histogramBaseName,"r2_DeltaPo"),DeltaPo_nbins,DeltaPo_min,DeltaPo_max,"p_{o}","r_{2}",2);
  h_r2_DeltaPl= createHistogram(createName(_histogramBaseName,"r2_DeltaPl"),DeltaPl_nbins,DeltaPl_min,DeltaPl_max,"p_{l}","r_{2}",2);

  h_a12_Qinv      = createHistogram(createName(_histogramBaseName,"A12_Qinv"),    Qinv_nbins,Qinv_min,Qinv_max, "Q_{inv}","A_{12}",2);
  h_a12_DeltaPs   = createHistogram(createName(_histogramBaseName,"A12_DeltaPs"), DeltaPs_nbins,DeltaPs_min,DeltaPs_max, "p_{s}","A_{12}",2);
  h_a12_DeltaPo   = createHistogram(createName(_histogramBaseName,"A12_DeltaPo"), DeltaPo_nbins,DeltaPo_min,DeltaPo_max, "p_{o}","A_{12}",2);
  h_a12_DeltaPl   = createHistogram(createName(_histogramBaseName,"A12_DeltaPl"), DeltaPl_nbins,DeltaPl_min,DeltaPl_max, "p_{l}","A_{12}",2);
  h_a21_Qinv      = createHistogram(createName(_histogramBaseName,"A21_Qinv"),    Qinv_nbins,Qinv_min,Qinv_max, "Q_{inv}","A_{21}",2);
  h_a21_DeltaPs   = createHistogram(createName(_histogramBaseName,"A21_DeltaPs"), DeltaPs_nbins,DeltaPs_min,DeltaPs_max, "p_{s}","A_{21}",2);
  h_a21_DeltaPo   = createHistogram(createName(_histogramBaseName,"A21_DeltaPo"), DeltaPo_nbins,DeltaPo_min,DeltaPo_max, "p_{o}","A_{21}",2);
  h_a21_DeltaPl   = createHistogram(createName(_histogramBaseName,"A21_DeltaPl"), DeltaPl_nbins,DeltaPl_min,DeltaPl_max, "p_{l}","A_{21}",2);

  h_n1_1_pt       = createHistogram(createName(_histogramBaseName,"n1_1_pt"), pt_nbins, pt_min,pt_max,"p_{T}",   "#rho_{1}^{1}(p_{T})");
  h_n1_2_pt       = createHistogram(createName(_histogramBaseName,"n1_2_pt"), pt_nbins, pt_min,pt_max,"p_{T}",   "#rho_{1}^{2}(p_{T})");
  h_n1_1_y        = createHistogram(createName(_histogramBaseName,"n1_1_y"),  rapidity_nbins,rapidity_minimum,rapidity_maximum, "y",   "#rho_{1}^{1}(y)");
  h_n1_2_y        = createHistogram(createName(_histogramBaseName,"n1_2_y"),  rapidity_nbins,rapidity_minimum,rapidity_maximum, "y",   "#rho_{1}^{2}(y)");

  h_n1_1_pt_mc  = createHistogram(createName(_histogramBaseName,"n1_1_pt_mc"), pt_nbins,pt_min,pt_max, "p_{T}","N",2);
  h_n1_2_pt_mc  = createHistogram(createName(_histogramBaseName,"n1_2_pt_mc"), pt_nbins,pt_min,pt_max, "p_{T}","N",2);
  h_n1_1_y_mc   = createHistogram(createName(_histogramBaseName,"n1_1_y_mc"),  rapidity_nbins,rapidity_minimum,rapidity_maximum, "y","N",2);
  h_n1_2_y_mc   = createHistogram(createName(_histogramBaseName,"n1_2_y_mc"),  rapidity_nbins,rapidity_minimum,rapidity_maximum, "y","N",2);
  h_n1_1_ptY_mc = createHistogram(createName(_histogramBaseName,"n1_1_ptY_mc"),rapidity_nbins,rapidity_minimum,rapidity_maximum, pt_nbins,pt_min,pt_max, "y","p_{T}","N");
  h_n1_2_ptY_mc = createHistogram(createName(_histogramBaseName,"n1_2_ptY_mc"),rapidity_nbins,rapidity_minimum,rapidity_maximum, pt_nbins,pt_min,pt_max, "y","p_{T}","N");

  h_n1r_1_pt_mc  = createHistogram(createName(_histogramBaseName,"n1r_1_pt_mc"), pt_nbins,pt_min,pt_max, "p_{T}","N",2);
  h_n1r_2_pt_mc  = createHistogram(createName(_histogramBaseName,"n1r_2_pt_mc"), pt_nbins,pt_min,pt_max, "p_{T}","N",2);
  h_n1r_1_y_mc   = createHistogram(createName(_histogramBaseName,"n1r_1_y_mc"),  rapidity_nbins,rapidity_minimum,rapidity_maximum, "y","N",2);
  h_n1r_2_y_mc   = createHistogram(createName(_histogramBaseName,"n1r_2_y_mc"),  rapidity_nbins,rapidity_minimum,rapidity_maximum, "y","N",2);
  h_n1r_1_ptY_mc = createHistogram(createName(_histogramBaseName,"n1r_1_ptY_mc"),rapidity_nbins,rapidity_minimum,rapidity_maximum, pt_nbins,pt_min,pt_max, "y","p_{T}","N");
  h_n1r_2_ptY_mc = createHistogram(createName(_histogramBaseName,"n1r_2_ptY_mc"),rapidity_nbins,rapidity_minimum,rapidity_maximum, pt_nbins,pt_min,pt_max, "y","p_{T}","N");
  
}

void ParticlePair3DDerivedHistos::loadFrom(TFile & inputFile)
{
  if (reportDebug(__FUNCTION__)) { printCR(); }
  h_n2_DeltaPs = loadH1(inputFile,createName(_histogramBaseName,"n2_DeltaPs"));
  h_n2_DeltaPo = loadH1(inputFile,createName(_histogramBaseName,"n2_DeltaPo"));
  h_n2_DeltaPl = loadH1(inputFile,createName(_histogramBaseName,"n2_DeltaPl"));
  
  h_n1n1_Qinv   = loadH1(inputFile,createName(_histogramBaseName,"n1n1_Qinv"));
  h_n1n1_DeltaP = loadH3(inputFile,createName(_histogramBaseName,"n1n1_DeltaP"));
  h_n1n1_DeltaPs= loadH1(inputFile,createName(_histogramBaseName,"n1n1_DeltaPs"));
  h_n1n1_DeltaPo= loadH1(inputFile,createName(_histogramBaseName,"n1n1_DeltaPo"));
  h_n1n1_DeltaPl= loadH1(inputFile,createName(_histogramBaseName,"n1n1_DeltaPl"));
  
  h_c2_Qinv   = loadH1(inputFile,createName(_histogramBaseName,"c2_Qinv"));
  h_c2_DeltaP = loadH3(inputFile,createName(_histogramBaseName,"c2_DeltaP"));
  h_c2_DeltaPs= loadH1(inputFile,createName(_histogramBaseName,"c2_DeltaPs"));
  h_c2_DeltaPo= loadH1(inputFile,createName(_histogramBaseName,"c2_DeltaPo"));
  h_c2_DeltaPl= loadH1(inputFile,createName(_histogramBaseName,"c2_DeltaPl"));
  
  h_r2_Qinv   = loadH1(inputFile,createName(_histogramBaseName,"r2_Qinv"));
  h_r2_DeltaP = loadH3(inputFile,createName(_histogramBaseName,"r2_DeltaP"));
  h_r2_DeltaPs= loadH1(inputFile,createName(_histogramBaseName,"r2_DeltaPs"));
  h_r2_DeltaPo= loadH1(inputFile,createName(_histogramBaseName,"r2_DeltaPo"));
  h_r2_DeltaPl= loadH1(inputFile,createName(_histogramBaseName,"r2_DeltaPl"));
  
  h_a12_Qinv      = loadH1(inputFile,createName(_histogramBaseName,"A12_Qinv"));
  h_a12_DeltaPs   = loadH1(inputFile,createName(_histogramBaseName,"A12_DeltaPs"));
  h_a12_DeltaPo   = loadH1(inputFile,createName(_histogramBaseName,"A12_DeltaPo"));
  h_a12_DeltaPl   = loadH1(inputFile,createName(_histogramBaseName,"A12_DeltaPl"));
  h_a21_Qinv      = loadH1(inputFile,createName(_histogramBaseName,"A21_Qinv"));
  h_a21_DeltaPs   = loadH1(inputFile,createName(_histogramBaseName,"A21_DeltaPs"));
  h_a21_DeltaPo   = loadH1(inputFile,createName(_histogramBaseName,"A21_DeltaPo"));
  h_a21_DeltaPl   = loadH1(inputFile,createName(_histogramBaseName,"A21_DeltaPl"));

  h_n1_1_pt_mc  = loadH1(inputFile,createName(_histogramBaseName,"n1_1_pt_mc"));
  h_n1_2_pt_mc  = loadH1(inputFile,createName(_histogramBaseName,"n1_2_pt_mc"));
  h_n1_1_y_mc   = loadH1(inputFile,createName(_histogramBaseName,"n1_1_y_mc"));
  h_n1_2_y_mc   = loadH1(inputFile,createName(_histogramBaseName,"n1_2_y_mc"));


  h_n1_1_ptY_mc  = loadH2(inputFile,createName(_histogramBaseName,"n1_1_ptY_mc"));
  h_n1_2_ptY_mc  = loadH2(inputFile,createName(_histogramBaseName,"n1_2_ptY_mc"));
  h_n1r_1_ptY_mc = loadH2(inputFile,createName(_histogramBaseName,"n1r_1_ptY_mc"));
  h_n1r_2_ptY_mc = loadH2(inputFile,createName(_histogramBaseName,"n1r_2_ptY_mc"));
  
}

void ParticlePair3DDerivedHistos::cloneD(const ParticlePair3DDerivedHistos & source)
{
  if (reportDebug(__FUNCTION__)) { printCR(); }
  h_n1n1_Qinv    = safeCloneH1(source.h_n1n1_Qinv);
  h_n1n1_DeltaP  = safeCloneH3(source.h_n1n1_DeltaP);
  h_n1n1_DeltaPs = safeCloneH1(source.h_n1n1_DeltaPs);
  h_n1n1_DeltaPo = safeCloneH1(source.h_n1n1_DeltaPo);
  h_n1n1_DeltaPl = safeCloneH1(source.h_n1n1_DeltaPl);

  h_n2_DeltaPs   = safeCloneH1(source.h_n2_DeltaPs);
  h_n2_DeltaPo   = safeCloneH1(source.h_n2_DeltaPo);
  h_n2_DeltaPl   = safeCloneH1(source.h_n2_DeltaPl);

  h_c2_Qinv      = safeCloneH1(source.h_c2_Qinv);
  h_c2_DeltaP    = safeCloneH3(source.h_c2_DeltaP);
  h_c2_DeltaPs   = safeCloneH1(source.h_c2_DeltaPs);
  h_c2_DeltaPo   = safeCloneH1(source.h_c2_DeltaPo);
  h_c2_DeltaPl   = safeCloneH1(source.h_c2_DeltaPl);

  h_r2_Qinv      = safeCloneH1(source.h_r2_Qinv);
  h_r2_DeltaP    = safeCloneH3(source.h_r2_DeltaP);
  h_r2_DeltaPs   = safeCloneH1(source.h_r2_DeltaPs);
  h_r2_DeltaPo   = safeCloneH1(source.h_r2_DeltaPo);
  h_r2_DeltaPl   = safeCloneH1(source.h_r2_DeltaPl);

  h_a12_Qinv     = safeCloneH1(source.h_a12_Qinv);
  h_a12_DeltaPs  = safeCloneH1(source.h_a12_DeltaPs);
  h_a12_DeltaPo  = safeCloneH1(source.h_a12_DeltaPo);
  h_a12_DeltaPl  = safeCloneH1(source.h_a12_DeltaPl);

  h_a21_Qinv     = safeCloneH1(source.h_a21_Qinv);
  h_a21_DeltaPs  = safeCloneH1(source.h_a21_DeltaPs);
  h_a21_DeltaPo  = safeCloneH1(source.h_a21_DeltaPo);
  h_a21_DeltaPl  = safeCloneH1(source.h_a21_DeltaPl);

  h_n1_1_pt      = safeCloneH1(source.h_n1_1_pt);
  h_n1_2_pt      = safeCloneH1(source.h_n1_2_pt);
  h_n1_1_y       = safeCloneH1(source.h_n1_1_y);
  h_n1_2_y       = safeCloneH1(source.h_n1_2_y);

  h_n1_1_pt_mc   = safeCloneH1(source.h_n1_1_pt_mc);
  h_n1_2_pt_mc   = safeCloneH1(source.h_n1_2_pt_mc);
  h_n1_1_y_mc    = safeCloneH1(source.h_n1_1_y_mc);
  h_n1_2_y_mc    = safeCloneH1(source.h_n1_2_y_mc);

  h_n1r_1_pt_mc  = safeCloneH1(source.h_n1r_1_pt_mc);
  h_n1r_2_pt_mc  = safeCloneH1(source.h_n1r_2_pt_mc);
  h_n1r_1_y_mc   = safeCloneH1(source.h_n1r_1_y_mc);
  h_n1r_2_y_mc   = safeCloneH1(source.h_n1r_2_y_mc);

  h_n1_1_ptY_mc  = safeCloneH2(source.h_n1_1_ptY_mc);
  h_n1_2_ptY_mc  = safeCloneH2(source.h_n1_2_ptY_mc);
  h_n1r_1_ptY_mc = safeCloneH2(source.h_n1r_1_ptY_mc);
  h_n1r_2_ptY_mc = safeCloneH2(source.h_n1r_2_ptY_mc);
}

// HistogramGroup from ParticlePair3DDerivedHistos must be normalized "per event" before calling this function
void ParticlePair3DDerivedHistos::calculateDerivedHistograms(ParticleSingleDerivedHistos & part1Histos,
                                                             ParticleSingleDerivedHistos & part2Histos,
                                                             ParticlePair3DHistos        & pairHistos,
                                                             double bincorrection)
{
  if (reportDebug(__FUNCTION__)) { printCR(); }
  bool same = bincorrection>0.0;

  // to calculate n1n1, we fold n1 by n1 using an MC technique.
  double y1, pt1, mt1, phi1;
  double y2, pt2, mt2, phi2;
  double pt,s,Mlong,roots;
  double weight=1.0;
  double pa[4], pb[4], ptot[4],q[4];
  //  const double m1 = 0.13957039;
  //  const double m2 = 0.13957039;
  const double g[4]={1.0,-1.0,-1.0,-1.0};
  double qinv, qside, qlong, qout;

  TH1 * part1_n1 = part1Histos.h_n1;
  TH1 * part2_n1 = part2Histos.h_n1;

  TH2 * part1_ptY = part1Histos.h_n1_ptY;
  TH2 * part2_ptY = part2Histos.h_n1_ptY;

  // Defensive: any of these can be null when running with non-default
  // particle keep flags (partons / gauge bosons) where the
  // ParticleSingleHistos didn't allocate the corresponding histo
  // because its enable flag (e.g., ptVsRapidity_fill) was off, or
  // because the IMPORT loaded a file written without that observable.
  // Without these guards we'd SEGV on GetBinContent / Integral.
  if (!part1_n1 || !part2_n1)
    {
    if (reportInfo(__FUNCTION__))
      {
      printCR();
      printString("Pair3D: part1/2_n1 null — skipping pair");
      }
    return;
    }
  if (!part1_ptY || !part2_ptY)
    {
    if (reportInfo(__FUNCTION__))
      {
      printCR();
      printString("Pair3D: part1/2_ptY null (ptVsRapidity_fill off?) — skipping pair");
      }
    return;
    }
  if (!pairHistos.h_n2_Qinv || !pairHistos.h_n2_DeltaP)
    {
    if (reportInfo(__FUNCTION__))
      {
      printCR();
      printString("Pair3D: pairHistos h_n2_Qinv/h_n2_DeltaP null — skipping pair");
      }
    return;
    }

  double v1 = part1_n1->GetBinContent(2);
  part1_n1->SetBinContent(1,v1);
  double v2 = part2_n1->GetBinContent(2);
  part2_n1->SetBinContent(1,v2);
  double nParticle1_mean = part1_n1->GetMean();
  double nParticle2_mean = part2_n1->GetMean();
  double nParticle1 = part1_ptY->Integral();
  double nParticle2 = part2_ptY->Integral();
  printValue("nParticle1 (integral)",nParticle1);
  printValue("nParticle1_mean",nParticle1_mean);

  printValue("nParticle2 (integral)",nParticle1);
  printValue("nParticle2_mean",nParticle2_mean);

  int nPart1 = nParticle1;
  int nPart2 = nParticle2;
  Configuration & configuration = parentTask()->configuration();
  const String & bn  __attribute__((unused)) = name();
  const String & ptn = parentName();
  long nMcSimEvents  = configuration.valueInt(ptn,   "nMcSimEvents");
  double scale = 1.0/double(nMcSimEvents);
  printValue("nMcSimEvents",nMcSimEvents);
  printValue("scale",scale);

// projections
  TH1* hTemp;
  hTemp = part1_ptY->ProjectionX();
  h_n1_1_y->Add(hTemp);
  delete hTemp;

  hTemp = part2_ptY->ProjectionX();
  h_n1_2_y->Add(hTemp);
  delete hTemp;

  hTemp = part1_ptY->ProjectionY();
  h_n1_1_pt->Add(hTemp);
  delete hTemp;

  hTemp = part2_ptY->ProjectionY();
  h_n1_2_pt->Add(hTemp);
  delete hTemp;


  vector<double> evPhi1;
  vector<double> evPt1;
  vector<double> evY1;
  vector<double> evPhi2;
  vector<double> evPt2;
  vector<double> evY2;

  //nMcSimEvents = 10000;
  for (long iEvent=0; iEvent<nMcSimEvents; iEvent++)
    {
    // create event
    evPhi1.clear();
    evPt1.clear();
    evY1.clear();
    evPhi2.clear();
    evPt2.clear();
    evY2.clear();

    for (int iParticle1=0; iParticle1<nPart1; iParticle1++)
      {
      phi1 = CAP::MATH::twoPi() * gRandom->Rndm();
      part1_ptY->GetRandom2(y1,pt1);
      h_n1_1_pt_mc->Fill(pt1);
      h_n1_1_y_mc->Fill(y1);
      h_n1_1_ptY_mc->Fill(y1,pt1);
      evPhi1.push_back(phi1);
      evPt1.push_back(pt1);
      evY1.push_back(y1);
      }
    if (!same)
      {
      for (int iParticle2=0; iParticle2<nPart2; iParticle2++)
        {
        phi2 = CAP::MATH::twoPi() * gRandom->Rndm();
        part2_ptY->GetRandom2(y2,pt2);
        h_n1_2_pt_mc->Fill(pt2);
        h_n1_2_y_mc->Fill(y2);
        h_n1_2_ptY_mc->Fill(y2,pt2);
        evPhi2.push_back(phi2);
        evPt2.push_back(pt2);
        evY2.push_back(y2);
        }
      }
    else
      {
      evPhi2 = evPhi1;
      evPt2  = evPt1;
      evY2   = evY1;
      }

    for (unsigned long iParticle1=0; iParticle1<evPhi1.size(); iParticle1++)
      {
      phi1 = evPhi1[iParticle1];
      pt1  = evPt1[iParticle1];
      y1   = evY1[iParticle1];
      mt1  = std::sqrt(mass1*mass1+pt1*pt1);
      pa[0] = mt1*std::cosh(y1);
      pa[1] = pt1*std::cos(phi1);
      pa[2] = pt1*std::sin(phi1);
      pa[3] = mt1*std::sinh(y1);

      for (unsigned long iParticle2=0; iParticle2<evPhi2.size(); iParticle2++)
        {
        if (same  &&  iParticle1==iParticle2) continue;
        phi2 = evPhi2[iParticle2];
        pt2  = evPt2[iParticle2];
        y2   = evY2[iParticle2];
        mt2  = std::sqrt(mass2*mass2+pt2*pt2);
        pb[0] = mt2*std::cosh(y2);
        pb[1] = pt2*std::cos(phi2);
        pb[2] = pt2*std::sin(phi2);
        pb[3] = mt2*std::sinh(y2);
        qinv=0.0;
        s=0.0;
        for(int alpha=0;alpha<4;alpha++)
          {
          ptot[alpha]=pa[alpha]+pb[alpha];
          s+=g[alpha]*ptot[alpha]*ptot[alpha];
          q[alpha]=pa[alpha]-pb[alpha];
          qinv-=g[alpha]*q[alpha]*q[alpha];
          }
        pt=std::sqrt(ptot[1]*ptot[1]+ptot[2]*ptot[2]);
        Mlong=std::sqrt(s+pt*pt);
        roots=std::sqrt(s);

        qside=(ptot[1]*q[2]-ptot[2]*q[1])/pt;
        qlong=(ptot[0]*q[3]-ptot[3]*q[0])/Mlong;
        qout=(roots/Mlong)*(ptot[1]*q[1]+ptot[2]*q[2])/pt;
        qinv=std::sqrt(qinv);
        h_n1n1_Qinv->Fill(qinv,weight);
        h_n1n1_DeltaP->Fill(qside,qout,qlong,weight);
        }
      }
    }

  h_n1_1_pt_mc->Scale(scale);
  h_n1_1_y_mc->Scale(scale);
  h_n1_2_pt_mc->Scale(scale);
  h_n1_2_y_mc->Scale(scale);
  h_n1_1_ptY_mc->Scale(scale);
  h_n1_2_ptY_mc->Scale(scale);

  double nParticle1_mc = h_n1_1_pt_mc->Integral();
  double nParticle2_mc = h_n1_2_pt_mc->Integral();
  double tweak1;
  double tweak2;

  tweak1= nParticle1/nParticle1_mc;
  if (bincorrection>1)
    {
    nParticle2_mc = nParticle1_mc;
    tweak2 = tweak1;
    h_n1_2_y_mc->Add(h_n1_1_y_mc);
    h_n1_2_pt_mc->Add(h_n1_1_pt_mc);
    h_n1_2_ptY_mc->Add(h_n1_1_ptY_mc);
    }
  else
    tweak2= nParticle2/nParticle2_mc;

  printValue("nPart1",nPart1);
  printValue("nParticle1",nParticle1);
  printValue("nParticle1_mc",nParticle1_mc);
  printValue("nPart2",nPart2);
  printValue("nParticle2",nParticle2);
  printValue("nParticle2_mc",nParticle2_mc);
  printValue("tweak1",tweak1);
  printValue("tweak2",tweak2);

  h_n1_1_pt_mc->Scale(tweak1);
  h_n1_1_y_mc->Scale(tweak1);
  h_n1_1_ptY_mc->Scale(tweak1);

  h_n1_2_pt_mc->Scale(tweak2);
  h_n1_2_y_mc->Scale(tweak2);
  h_n1_2_ptY_mc->Scale(tweak2);

  h_n1r_1_pt_mc->Divide(h_n1_1_pt_mc, h_n1_1_pt);
  h_n1r_1_y_mc->Divide(h_n1_1_y_mc,   h_n1_1_y);
  h_n1r_1_ptY_mc->Divide(h_n1_1_ptY_mc, part1_ptY);

  h_n1r_2_pt_mc->Divide(h_n1_2_pt_mc, h_n1_2_pt);
  h_n1r_2_y_mc->Divide(h_n1_2_y_mc,   h_n1_2_y);
  h_n1r_2_ptY_mc->Divide(h_n1_2_ptY_mc, part2_ptY);

  h_n1n1_Qinv->Scale(scale);
  h_n1n1_DeltaP->Scale(scale);

  double n2_yield   = pairHistos.h_n2_Qinv->GetBinContent(5);
  double n1n1_yield = h_n1n1_Qinv->GetBinContent(5);

  double n2_yield_err   = pairHistos.h_n2_Qinv->GetBinError(5);
  double n1n1_yield_err = h_n1n1_Qinv->GetBinError(5);
  double tweak = n2_yield/n1n1_yield;
  printValue("n2_yield",n2_yield);
  printValue("n1n1_yield",n1n1_yield);
  printValue("n2_yield/n1n1_yield",tweak);
  printValue("n2_yield_err",n2_yield_err);
  printValue("n1n1_yield_err",n1n1_yield_err);

  h_n1n1_DeltaP->ProjectionX(createName(_histogramBaseName,"n2_DeltaPs"),1,DeltaPo_nbins,1,DeltaPl_nbins,"e");
  h_n1n1_DeltaP->ProjectionY(createName(_histogramBaseName,"n2_DeltaPo"),1,DeltaPs_nbins,1,DeltaPl_nbins,"e");
  h_n1n1_DeltaP->ProjectionZ(createName(_histogramBaseName,"n2_DeltaPl"),1,DeltaPs_nbins,1,DeltaPo_nbins,"e");

  pairHistos.h_n2_DeltaP->ProjectionX(createName(_histogramBaseName,"n2_DeltaPs"),1,DeltaPo_nbins,1,DeltaPl_nbins,"e");
  pairHistos.h_n2_DeltaP->ProjectionY(createName(_histogramBaseName,"n2_DeltaPo"),1,DeltaPs_nbins,1,DeltaPl_nbins,"e");
  pairHistos.h_n2_DeltaP->ProjectionZ(createName(_histogramBaseName,"n2_DeltaPl"),1,DeltaPs_nbins,1,DeltaPo_nbins,"e");

  calculateC2_H1H1H1(pairHistos.h_n2_Qinv,h_n1n1_Qinv,h_c2_Qinv);
  calculateC2_H3H3H3(pairHistos.h_n2_DeltaP,h_n1n1_DeltaP,h_c2_DeltaP);
  h_c2_DeltaP->ProjectionX(createName(_histogramBaseName,"c2_DeltaPs"),1,DeltaPo_nbins,1,DeltaPl_nbins,"e");
  h_c2_DeltaP->ProjectionY(createName(_histogramBaseName,"c2_DeltaPo"),1,DeltaPs_nbins,1,DeltaPl_nbins,"e");
  h_c2_DeltaP->ProjectionZ(createName(_histogramBaseName,"c2_DeltaPl"),1,DeltaPs_nbins,1,DeltaPo_nbins,"e");

  calculateR2_H1H1H1(pairHistos.h_n2_Qinv,h_n1n1_Qinv,h_r2_Qinv);

  double r2_yield = h_r2_Qinv->GetBinContent(5);
  double r2_yield_err = h_r2_Qinv->GetBinError(5);
  printValue("r2_yield",r2_yield);
  printValue("r2_yield_err",r2_yield_err);

  calculateR2_H3H3H3(pairHistos.h_n2_DeltaP,h_n1n1_DeltaP,h_r2_DeltaP);
  h_r2_DeltaP->ProjectionX(createName(_histogramBaseName,"r2_DeltaPs"),1,DeltaPo_nbins,1,DeltaPl_nbins,"e");
  h_r2_DeltaP->ProjectionY(createName(_histogramBaseName,"r2_DeltaPo"),1,DeltaPs_nbins,1,DeltaPl_nbins,"e");
  h_r2_DeltaP->ProjectionZ(createName(_histogramBaseName,"r2_DeltaPl"),1,DeltaPs_nbins,1,DeltaPo_nbins,"e");

  double invN1 = 1.0/nParticle1;
  double invN2 = 1.0/nParticle2;
  h_a12_Qinv    ->Add(h_c2_Qinv,invN2);
  h_a12_DeltaPs ->Add(h_c2_DeltaPs,invN2);
  h_a12_DeltaPo ->Add(h_c2_DeltaPo,invN2);
  h_a12_DeltaPl ->Add(h_c2_DeltaPl,invN2);
  h_a21_Qinv    ->Add(h_c2_Qinv,invN1);
  h_a21_DeltaPs ->Add(h_c2_DeltaPs,invN1);
  h_a21_DeltaPo ->Add(h_c2_DeltaPo,invN1);
  h_a21_DeltaPl ->Add(h_c2_DeltaPl,invN1);
}

//! set the masses to be used in the calculation of q_inv
//! The default value set at construction are the pion+ mass.
//!
void ParticlePair3DDerivedHistos::setMasses(double m1, double m2)
{
  mass1 = m1;
  mass2 = m2;
}


} //  namespace CAP
