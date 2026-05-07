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
#include "ParticleSmearer.hpp"
#include "NameHelpers.hpp"
#include "PrintHelpers.hpp"
#include "Task.hpp"
#include "Configuration.hpp"
#include "NameHelpers.hpp"
#include "RootHistogramHelpers.hpp"

ClassImp(CAP::ParticleSmearer);

namespace CAP
{

ParticleSmearer::ParticleSmearer()
:
HistogramGroup(),
pt_nbins(0),
phi_nbins(0),
eta_nbins(0),
pt_min(0),
pt_max(0),
phi_min(0),
phi_max(0),
eta_min(0),
eta_max(0),
h_pt_bias(nullptr),
h_pt_rms(nullptr),
h_eta_bias(nullptr),
h_eta_rms(nullptr),
h_phi_bias(nullptr),
h_phi_rms(nullptr),
pids(),
h_prob_species(),
nSpecies(0)
{
  appendClassName("ParticleSmearer");
  setName("ParticleSmearer");
  setTitle("ParticleSmearer");
}

ParticleSmearer::ParticleSmearer(const ParticleSmearer & source)
:
HistogramGroup(source),
pt_nbins(source.pt_nbins),
phi_nbins(source.phi_nbins),
eta_nbins(source.eta_nbins),
pt_min(source.pt_min),
pt_max(source.pt_max),
phi_min(source.phi_min),
phi_max(source.phi_max),
eta_min(source.eta_min),
eta_max(source.eta_max),
h_pt_bias(nullptr),
h_pt_rms(nullptr),
h_eta_bias(nullptr),
h_eta_rms(nullptr),
h_phi_bias(nullptr),
h_phi_rms(nullptr),
pids(source.pids),
h_prob_species(),
nSpecies(source.nSpecies)
{
  cloneB(source);
}

ParticleSmearer & ParticleSmearer::operator=(const ParticleSmearer & rhs)
{
  if (this!=&rhs)
    {
    HistogramGroup::operator=(rhs);
    pt_nbins  = rhs.pt_nbins;
    phi_nbins = rhs.phi_nbins;
    eta_nbins = rhs.eta_nbins;
    pt_min    = rhs.pt_min;
    pt_max    = rhs.pt_max;
    phi_min   = rhs.phi_min;
    phi_max   = rhs.phi_max;
    eta_min   = rhs.eta_min;
    eta_max   = rhs.eta_max;
    cloneB(rhs);
    }
  return *this;
}

void ParticleSmearer::cloneB(const ParticleSmearer & source)
{
  if (reportDebug(__FUNCTION__)) { printCR(); }
  h_pt_bias  = safeCloneH2(source.h_pt_bias);
  h_pt_rms   = safeCloneH2(source.h_pt_rms);
  h_eta_bias = safeCloneH2(source.h_eta_bias);
  h_eta_rms  = safeCloneH2(source.h_eta_rms);
  h_phi_bias = safeCloneH2(source.h_phi_bias);
  h_phi_rms  = safeCloneH2(source.h_phi_rms);
}

void ParticleSmearer::configure(const String & taskName,
                                      const String & typeName,
                                      const Configuration & configuration,
                                      unsigned int index)
{
  if (reportDebug(__FUNCTION__)) { printCR(); }
  HistogramGroup::configure(taskName,typeName,configuration,index);
  n_nbins   = configuration.valueInt(   createKey(taskName,typeName,index,"n_nbins"));
  nSpecies  = configuration.valueBool(  createKey(taskName,typeName,index,"nSpecies"));
  pt_nbins  = configuration.valueInt(   createKey(taskName,typeName,index,"pt_nbins"));
  phi_nbins = configuration.valueInt(   createKey(taskName,typeName,index,"phi_nbins"));
  eta_nbins = configuration.valueInt(   createKey(taskName,typeName,index,"eta_nbins"));
  pt_min    = configuration.valueDouble(createKey(taskName,typeName,index,"pt_min"));
  pt_max    = configuration.valueDouble(createKey(taskName,typeName,index,"pt_max"));
  phi_min   = configuration.valueDouble(createKey(taskName,typeName,index,"phi_min"));
  phi_max   = configuration.valueDouble(createKey(taskName,typeName,index, "phi_max"));
  eta_min   = configuration.valueDouble(createKey(taskName,typeName,index,"eta_min"));
  eta_max   = configuration.valueDouble(createKey(taskName,typeName,index,"eta_max"));

  if (reportDebug(__FUNCTION__))
    {
    printCR();
    printLine();
    printValue("pt_nbins",    pt_nbins);
    printValue("pt_min",      pt_min);
    printValue("pt_max",      pt_max);
    printValue("eta_nbins",   eta_nbins);
    printValue("eta_min",     eta_min);
    printValue("eta_max",     eta_max);
    printLine();
    printCR();
    }
}

// for now use the same boundaries for eta and y histogram
void ParticleSmearer::create()
{
  if (reportDebug(__FUNCTION__)) { printCR();}
  h_pt_bias  = createHistogram("PART_BIAS_PT_ALL_ALL_etaPt",  eta_nbins, eta_min, eta_max, pt_nbins,  pt_min,  pt_max,  "#eta", "p_{T}","pt_{bias}", 2);
  h_pt_rms   = createHistogram("PART_RMS_PT_ALL_ALL_etaPt",   eta_nbins, eta_min, eta_max, pt_nbins,  pt_min,  pt_max,  "#eta", "p_{T}","pt_{rms}",  2);
  h_phi_bias = createHistogram("PART_BIAS_PHI_ALL_ALL_etaPt", eta_nbins, eta_min, eta_max, pt_nbins,  pt_min,  pt_max,  "#eta", "p_{T}","phi_{bias}",2);
  h_phi_rms  = createHistogram("PART_RMS_PHI_ALL_ALL_etaPt",  eta_nbins, eta_min, eta_max, pt_nbins,  pt_min,  pt_max,  "#eta", "p_{T}","phi_{rms}", 2);
  h_eta_bias = createHistogram("PART_BIAS_ETA_ALL_ALL_etaPt", eta_nbins, eta_min, eta_max, pt_nbins,  pt_min,  pt_max,  "#eta", "p_{T}","eta_{bias}",2);
  h_eta_rms  = createHistogram("PART_RMS_ETA_ALL_ALL_etaPt",  eta_nbins, eta_min, eta_max, pt_nbins,  pt_min,  pt_max,  "#eta", "p_{T}","eta_{rms}", 2);
}

void ParticleSmearer::loadFrom(TFile & inputFile)
{
  if (reportDebug(__FUNCTION__)) { printCR();}
  h_pt_bias  = importH2(inputFile,"PART_BIAS_PT_ALL_ALL_etaPt");
  h_pt_rms   = importH2(inputFile,"PART_RMS_PT_ALL_ALL_etaPt");
  h_eta_bias = importH2(inputFile,"PART_BIAS_ETA_ALL_ALL_etaPt");
  h_eta_rms  = importH2(inputFile,"PART_RMS_ETA_ALL_ALL_etaPt");
  h_phi_bias = importH2(inputFile,"PART_BIAS_PHI_ALL_ALL_etaPt");
  h_phi_rms  = importH2(inputFile,"PART_RMS_PHI_ALL_ALL_etaPt");
  eta_nbins  = h_pt_bias->GetNbinsX();
  pt_nbins   = h_pt_bias->GetNbinsY();
//  for (unsigned int k=0; k<nSpecies; k++)
//    {
//    String s = "PID"; s += k;
//    int pid = configuration.valueBool(ptn,"s");
//    pids.push_back(pid);
//    s = "ProbPid"; s += k;
//    TH2 * probHisto = importH2(inputFile,createName(_histogramBaseName,s));
//    }
}

void ParticleSmearer::smear(const Particle & source, Particle & target)
{
  const VectorLorentz & in  = source.momentum();
  double pt   = in.perp();
  double phi  = in.phi();
  double eta  = in.pseudorapidity();
  double mass = in.modulus();
  int    etaBin     = h_pt_bias->GetXaxis()->FindBin(eta);
  int    ptBin      = h_pt_bias->GetYaxis()->FindBin(pt);
  double pt_bias    = h_pt_bias->GetBinContent(etaBin,ptBin);
  double pt_rms     = h_pt_rms->GetBinContent(etaBin,ptBin);
  double smearedPt  = gRandom->Gaus(pt+pt_bias,pt_rms);
  if (smearedPt<0.001) smearedPt = 0.001;
  double eta_bias   = h_eta_bias->GetBinContent(etaBin,ptBin);
  double eta_rms    = h_eta_rms->GetBinContent(etaBin,ptBin);
  double smearedEta = gRandom->Gaus(eta+eta_bias,eta_rms);
  double phi_bias   = h_phi_bias->GetBinContent(etaBin,ptBin);
  double phi_rms    = h_phi_rms->GetBinContent(etaBin,ptBin);
  double smearedPhi = gRandom->Gaus(phi+phi_bias,phi_rms);
  VectorLorentz & out = target.momentum();
  out.setMPhiPtY(mass,smearedPhi,smearedPt,smearedEta);
}

} // namespace CAP
