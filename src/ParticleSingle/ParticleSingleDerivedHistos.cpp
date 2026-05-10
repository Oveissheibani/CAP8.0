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
#include "ParticleSingleDerivedHistos.hpp"
#include "PrintHelpers.hpp"
#include "RootHistogramHelpers.hpp"
#include "Task.hpp"
#include "NameHelpers.hpp"
#include "RootHistogramHelpers.hpp"
#include "Configuration.hpp"

ClassImp(CAP::ParticleSingleDerivedHistos);

namespace CAP
{

ParticleSingleDerivedHistos::ParticleSingleDerivedHistos()
:
ParticleSingleHistos(),
h_n1_phi(nullptr),
h_n1_eta(nullptr),
h_n1_y(nullptr),
h_spt_phi(nullptr),
h_spt_eta(nullptr),
h_spt_y(nullptr),
h_avg_pt_phi(nullptr),
h_avg_pt_eta(nullptr),
h_avg_pt_phiEta(nullptr),
h_avg_pt_y(nullptr),
h_avg_pt_phiY(nullptr)
{
  appendClassName("ParticleSingleDerivedHistos");
  setName("ParticleSingleDerivedHistos");
  setTitle("ParticleSingleDerivedHistos");
}

ParticleSingleDerivedHistos::ParticleSingleDerivedHistos(const ParticleSingleDerivedHistos & source)
:
ParticleSingleHistos(source),
h_n1_phi(nullptr),
h_n1_eta(nullptr),
h_n1_y(nullptr),
h_spt_phi(nullptr),
h_spt_eta(nullptr),
h_spt_y(nullptr),
h_avg_pt_phi(nullptr),
h_avg_pt_eta(nullptr),
h_avg_pt_phiEta(nullptr),
h_avg_pt_y(nullptr),
h_avg_pt_phiY(nullptr)
{
  cloneD(source);
}

ParticleSingleDerivedHistos & ParticleSingleDerivedHistos::operator=(const ParticleSingleDerivedHistos & source)
{
  if (this!=&source)
    {
    ParticleSingleHistos::operator=(source);
    cloneD(source);
    }
  return *this;
}

void ParticleSingleDerivedHistos::cloneD(const ParticleSingleDerivedHistos & source)
{
  if (reportStart(__FUNCTION__)) { printCR(); }
  ParticleSingleHistos::cloneB(source);
  h_n1_phi        = safeCloneH1(source.h_n1_phi);
  h_n1_eta        = safeCloneH1(source.h_n1_eta);
  h_n1_y          = safeCloneH1(source.h_n1_y);
  h_spt_phi       = safeCloneH1(source.h_spt_phi);
  h_spt_eta       = safeCloneH1(source.h_spt_eta);
  h_spt_y         = safeCloneH1(source.h_spt_y);
  h_avg_pt_phi    = safeCloneH1(source.h_avg_pt_phi);
  h_avg_pt_eta    = safeCloneH1(source.h_avg_pt_eta);
  h_avg_pt_phiEta = safeCloneH2(source.h_avg_pt_phiEta);
  h_avg_pt_y      = safeCloneH1(source.h_avg_pt_y);
  h_avg_pt_phiY   = safeCloneH2(source.h_avg_pt_phiY);
}

void ParticleSingleDerivedHistos::configure(const String & taskName,
                                                  const String & objectType,
                                                  const Configuration & configuration,
                                                  unsigned int index)
{
  ParticleSingleHistos::configure(taskName,objectType,configuration,index);
}

void ParticleSingleDerivedHistos::create()
{
  if (reportDebug(__FUNCTION__)) { printCR();}
  h_n1_phi = createHistogram(createName(_histogramBaseName,"n1_phi"), phi_nbins, phi_min, phi_max, "#varphi","#rho_{1}(#varphi)");
  if (p2_fill)
    {
    h_spt_phi = createHistogram(createName(_histogramBaseName,"sum_pt_phi"), phi_nbins, phi_min, phi_max, "#varphi","#sum p_{T}");
    h_avg_pt_phi = createHistogram(createName(_histogramBaseName,"avg_pt_phi"),     phi_nbins, phi_min, phi_max, "#varphi","<p_{T}>");
    }

  if (eta_fill)
    {
    h_n1_eta = createHistogram(createName(_histogramBaseName,"n1_eta"),     eta_nbins, eta_min, eta_max, "#eta",   "#rho_{1}(#eta)");
    if (p2_fill)
      {
      h_spt_eta     = createHistogram(createName(_histogramBaseName,"sum_pt_eta"),  eta_nbins, eta_min, eta_max, "#eta",   "#sum p_{T}");
      h_avg_pt_eta     = createHistogram(createName(_histogramBaseName,"avg_pt_eta"),     eta_nbins, eta_min, eta_max, "#eta",   "<p_{T}>");
      h_avg_pt_phiEta  = createHistogram(createName(_histogramBaseName,"avg_pt_phiEta"),  eta_nbins, eta_min, eta_max, phi_nbins, phi_min, phi_max, "#eta", "#varphi","N");
      }
    }
  if (rapidity_fill)
    {
    h_n1_y = createHistogram(createName(_histogramBaseName,"n1_y"),     rapidity_nbins, rapidity_minimum,   rapidity_maximum, "y",   "#rho_{1}(y)");
    if (p2_fill)
      {
      h_spt_y         = createHistogram(createName(_histogramBaseName,"sum_pt_y"),     rapidity_nbins, rapidity_minimum,   rapidity_maximum, "y",   "#sum p_{T}");
      h_avg_pt_y      = createHistogram(createName(_histogramBaseName,"avg_pt_y"),     rapidity_nbins, rapidity_minimum,   rapidity_maximum, "y",   "<p_{T}>");
      h_avg_pt_phiY   = createHistogram(createName(_histogramBaseName,"avg_pt_phiY"),  rapidity_nbins, rapidity_minimum,   rapidity_maximum, phi_nbins, phi_min, phi_max, "y", "#varphi","N");
      }
    }
}

void ParticleSingleDerivedHistos::loadFrom(TFile & inputFile)
{
  if (reportDebug(__FUNCTION__)) { printCR();}
  h_n1_phi      = importH1(inputFile,  createName(_histogramBaseName,"n1_phi"));
  h_spt_phi     = importH1(inputFile,  createName(_histogramBaseName,"sum_pt_phi"));
  h_avg_pt_phi  = importH1(inputFile,  createName(_histogramBaseName,"avg_pt_phi"));

  if (eta_fill)
    {
    h_n1_eta     = importH1(inputFile,  createName(_histogramBaseName,"n1_eta"));
    if (p2_fill)
      {
      h_spt_eta         = importH1(inputFile,  createName(_histogramBaseName,"sum_eta"));
      h_avg_pt_eta      = importH1(inputFile,  createName(_histogramBaseName,"avg_pt_eta"));
      h_avg_pt_phiEta   = importH2(inputFile,  createName(_histogramBaseName,"avg_pt_phiEta"));
      }
    }
  if (rapidity_fill)
    {
    h_n1_y     = importH1(inputFile,  createName(_histogramBaseName,"n1_y"));
    if (p2_fill)
      {
      h_spt_y         = importH1(inputFile,  createName(_histogramBaseName,"spt_y"));
      h_avg_pt_y      = importH1(inputFile,  createName(_histogramBaseName,"avg_pt_y"));
      h_avg_pt_phiY   = importH2(inputFile,  createName(_histogramBaseName,"avg_pt_phiY"));
      }
    }
}

//!
//! Calculates derived histograms. This is currently limited to pt averages vs eta and phi.
//!
void ParticleSingleDerivedHistos::calculateDerivedHistograms(ParticleSingleHistos & baseHistos)
{
  if (reportDebug(__FUNCTION__)) { printCR(); }
  // ----------------------------------------------------------------
  //  CRITICAL: clone the base h_n1_pt / h_n1 into THIS derived
  //  object so downstream consumers (in particular the Pair
  //  Calculator's calculateDerivedHistograms which reads
  //  part1Histos.h_n1_pt) see a populated histogram instead of
  //  the nullptr left over from create() (which only allocates
  //  the strictly-derived members like h_n1_phi/eta/y).
  //  Without this clone, the Pair Calculator throws
  //      HistogramException("LOGIC", "!part1Histos.h_n1_pt", ...)
  //  the moment it tries to compute N1*N1 in pT.
  // ----------------------------------------------------------------
  // 1D base histos
  if (baseHistos.h_n1_pt && !h_n1_pt)
    h_n1_pt    = (TH1*) baseHistos.h_n1_pt->Clone();
  if (baseHistos.h_n1    && !h_n1)
    h_n1       = (TH1*) baseHistos.h_n1->Clone();
  if (baseHistos.h_n1_eTotal && !h_n1_eTotal)
    h_n1_eTotal = (TH1*) baseHistos.h_n1_eTotal->Clone();
  if (baseHistos.h_n1_ptXS   && !h_n1_ptXS)
    h_n1_ptXS  = (TH1*) baseHistos.h_n1_ptXS->Clone();
  // 2D base histos used downstream by ParticlePairDerivedHistos::
  // calculateDerivedHistograms (reduce_n1EtaPhiN1EtaPhi... etc.).
  if (baseHistos.h_n1_phiEta && !h_n1_phiEta)
    h_n1_phiEta = (TH2*) baseHistos.h_n1_phiEta->Clone();
  if (baseHistos.h_spt_phiEta && !h_spt_phiEta)
    h_spt_phiEta = (TH2*) baseHistos.h_spt_phiEta->Clone();
  if (baseHistos.h_n1_phiY && !h_n1_phiY)
    h_n1_phiY = (TH2*) baseHistos.h_n1_phiY->Clone();
  if (baseHistos.h_spt_phiY && !h_spt_phiY)
    h_spt_phiY = (TH2*) baseHistos.h_spt_phiY->Clone();
  if (baseHistos.h_n1_ptY && !h_n1_ptY)
    h_n1_ptY = (TH2*) baseHistos.h_n1_ptY->Clone();
  if (baseHistos.h_pdgId && !h_pdgId)
    h_pdgId = (TH1*) baseHistos.h_pdgId->Clone();

  TH1* hTemp;
  if (baseHistos.h_n1_phiEta)
    {
    hTemp = baseHistos.h_n1_phiEta->ProjectionY();
    h_n1_phi->Add(hTemp);
    delete hTemp;

    hTemp = baseHistos.h_n1_phiEta->ProjectionX();
    h_n1_eta->Add(hTemp);
    delete hTemp;

    if (p2_fill  &&  baseHistos.h_spt_phiEta)
      {
      hTemp = baseHistos.h_spt_phiEta->ProjectionY();
      h_spt_phi->Add(hTemp);
      delete hTemp;
      hTemp = baseHistos.h_spt_phiEta->ProjectionX();
      h_spt_eta->Add(hTemp);
      delete hTemp;

      calculateInclusiveAvg(h_spt_phi,h_n1_phi,h_avg_pt_phi);
      calculateInclusiveAvg(h_spt_eta,h_n1_eta,h_avg_pt_eta);
      calculateInclusiveAvgH2(baseHistos.h_spt_phiEta,baseHistos.h_n1_phiEta,h_avg_pt_phiEta);
      }
    }

  if (baseHistos.h_n1_phiY)
    {
    hTemp = baseHistos.h_n1_phiY->ProjectionY();
    h_n1_phi->Reset();
    h_n1_phi->Add(hTemp);
    delete hTemp;

    hTemp = baseHistos.h_n1_phiY->ProjectionX();
    h_n1_y->Add(hTemp);
    delete hTemp;

    if (p2_fill  &&  baseHistos.h_spt_phiY)
      {
      hTemp = baseHistos.h_spt_phiY->ProjectionY();
      h_spt_phi->Add(hTemp);
      delete hTemp;
      hTemp = baseHistos.h_spt_phiY->ProjectionX();
      h_spt_y->Add(hTemp);
      delete hTemp;

      calculateInclusiveAvg(h_spt_phi,h_n1_phi,h_avg_pt_phi);
      calculateInclusiveAvg(h_spt_y,h_n1_y,h_avg_pt_y);
      calculateInclusiveAvgH2(baseHistos.h_spt_phiY,baseHistos.h_n1_phiY,h_avg_pt_phiY);
      }
    }
}

} // namespace cap

