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
#include "NameHelpers.hpp"
#include "PrintHelpers.hpp"
#include "Task.hpp"
#include "Configuration.hpp"
#include "NameHelpers.hpp"
#include "RootHistogramHelpers.hpp"
#include <cmath>      // std::isfinite — defensive guards against partons / NaN momenta
#include <limits>     // quiet_NaN
#include <exception>

ClassImp(CAP::ParticleSingleHistos);

namespace CAP
{

  ParticleSingleHistos::ParticleSingleHistos()
  :
  HistogramGroup(),
  eta_fill(0),
  rapidity_fill(0),
  p2_fill(0),
  ptVsRapidity_fill(0),
  pid_fill(0),
  n1_nbins(0),
  pt_nbins(0),
  phi_nbins(0),
  eta_nbins(0),
  rapidity_nbins(0),
  n1_min(0),
  n1_max(0),
  pt_min(0),
  pt_max(0),
  phi_min(0),
  phi_max(6.28319),
  eta_min(0),
  eta_max(0),
  rapidity_minimum(0),
  rapidity_maximum(0),
  h_n1(nullptr),
  h_n1_eTotal(nullptr),
  h_n1_pt(nullptr),
  h_n1_ptXS(nullptr),
  h_n1_phiEta(nullptr),
  h_spt_phiEta(nullptr),
  h_n1_phiY(nullptr),
  h_spt_phiY(nullptr),
  h_n1_ptY(nullptr),
  h_pdgId(nullptr)
  {
  appendClassName("ParticleSingleHistos");
  setName("ParticleSingleHistos");
  setTitle("ParticleSingleHistos");
  }

  ParticleSingleHistos::ParticleSingleHistos(const ParticleSingleHistos & source )
  :
  HistogramGroup(source ),
  eta_fill(source.eta_fill ),
  rapidity_fill(source.rapidity_fill ),
  p2_fill(source.p2_fill ),
  ptVsRapidity_fill(source.ptVsRapidity_fill ),
  pid_fill(source.pid_fill ),
  n1_nbins(source.n1_nbins),
  pt_nbins(source.pt_nbins),
  phi_nbins(source.phi_nbins),
  eta_nbins(source.eta_nbins),
  rapidity_nbins(source.rapidity_nbins),
  n1_min(source.n1_min),
  n1_max(source.n1_max),
  pt_min(source.pt_min),
  pt_max(source.pt_max),
  phi_min(source.phi_min),
  phi_max(source.phi_max),
  eta_min(source.eta_min),
  eta_max(source.eta_max),
  rapidity_minimum(source.rapidity_minimum),
  rapidity_maximum(source.rapidity_maximum),
  h_n1(nullptr),
  h_n1_eTotal(nullptr),
  h_n1_pt(nullptr),
  h_n1_ptXS(nullptr),
  h_n1_phiEta(nullptr),
  h_spt_phiEta(nullptr),
  h_n1_phiY(nullptr),
  h_spt_phiY(nullptr),
  h_n1_ptY(nullptr),
  h_pdgId(nullptr)
  {
  cloneB(source );
  }

  ParticleSingleHistos & ParticleSingleHistos::operator=(const ParticleSingleHistos & rhs)
  {
  if (this!=&rhs)
    {
    HistogramGroup::operator=(rhs);
    eta_fill           = rhs.eta_fill;
    rapidity_fill      = rhs.rapidity_fill;
    p2_fill            = rhs.p2_fill;
    ptVsRapidity_fill  = rhs.ptVsRapidity_fill;
    pid_fill           = rhs.pid_fill;
    n1_nbins           = rhs.n1_nbins;
    pt_nbins           = rhs.pt_nbins;
    phi_nbins          = rhs.phi_nbins;
    eta_nbins          = rhs.eta_nbins;
    rapidity_nbins     = rhs.rapidity_nbins;
    n1_min             = rhs.n1_min;
    n1_max             = rhs.n1_max;
    pt_min             = rhs.pt_min;
    pt_max             = rhs.pt_max;
    phi_min            = rhs.phi_min;
    phi_max            = rhs.phi_max;
    eta_min            = rhs.eta_min;
    eta_max            = rhs.eta_max;
    rapidity_minimum   = rhs.rapidity_minimum;
    rapidity_maximum   = rhs.rapidity_maximum;
    cloneB(rhs);
    }
  return *this;
  }

  void ParticleSingleHistos::cloneB(const ParticleSingleHistos & source )
  {
  if (reportStart(__FUNCTION__)) { printCR(); }
  h_n1         = safeCloneH1(source.h_n1);
  h_n1_eTotal  = safeCloneH1(source.h_n1_eTotal);
  h_n1_pt      = safeCloneH1(source.h_n1_pt);
  h_n1_ptXS    = safeCloneH1(source.h_n1_ptXS) ;
  h_n1_phiEta  = safeCloneH2(source.h_n1_phiEta) ;
  h_spt_phiEta = safeCloneH2(source.h_spt_phiEta) ;
  h_n1_phiY    = safeCloneH2(source.h_n1_phiY);
  h_spt_phiY   = safeCloneH2(source.h_spt_phiY);
  h_n1_ptY     = safeCloneH2(source.h_n1_ptY);
  h_pdgId      = safeCloneH1(source.h_pdgId);
  if (reportEnd(__FUNCTION__)) { printCR(); }
  }

  void ParticleSingleHistos::configure(const String & taskName,
                                       const String & objectType,
                                       const Configuration & configuration,
                                       unsigned int index)
  {
  HistogramGroup::configure(taskName,objectType,configuration,index);
  String type;
  if (objectType.Contains("HISTOGRAM"))
    type = "HISTOGRAM";
  else
    type = objectType;
  eta_fill          = configuration.valueBool(   createKey(taskName,type,"eta_fill"));
  rapidity_fill     = configuration.valueBool(   createKey(taskName,type,"rapidity_fill"));
  p2_fill           = configuration.valueBool(   createKey(taskName,type,"p2_fill"));
  pid_fill          = configuration.valueBool(   createKey(taskName,type,"FillPid"));
  ptVsRapidity_fill = configuration.valueBool(   createKey(taskName,type,"FillPtvsY"));

  n1_nbins          = configuration.valueInt(    createKey(taskName,type,"n1_nbins"));
  pt_nbins          = configuration.valueInt(    createKey(taskName,type,"pt_nbins"));
  phi_nbins         = configuration.valueInt(    createKey(taskName,type,"phi_nbins"));
  eta_nbins         = configuration.valueInt(    createKey(taskName,type,"eta_nbins"));
  rapidity_nbins    = configuration.valueInt(    createKey(taskName,type,"rapidity_nbins"));

  n1_min   = configuration.valueDouble(   createKey(taskName,type,"n1_min"));
  n1_max   = configuration.valueDouble(   createKey(taskName,type,"n1_max"));
  pt_min   = configuration.valueDouble(   createKey(taskName,type,"pt_min"));
  pt_max   = configuration.valueDouble(   createKey(taskName,type,"pt_max"));
  phi_min  = configuration.valueDouble(   createKey(taskName,type,"phi_min"));
  phi_max  = configuration.valueDouble(   createKey(taskName,type,"phi_max"));
  eta_min  = configuration.valueDouble(   createKey(taskName,type,"eta_min"));
  eta_max  = configuration.valueDouble(   createKey(taskName,type,"eta_max"));
  rapidity_minimum    = configuration.valueDouble(   createKey(taskName,type,"rapidity_min"));
  rapidity_maximum    = configuration.valueDouble(   createKey(taskName,type,"rapidity_max"));


  if (reportInfo(__FUNCTION__))
    {
    printCR();
    printValue(createKey(taskName,type,"eta_fill"),eta_fill);
    printValue(createKey(taskName,type,"rapidity_fill"),rapidity_fill  );
    printValue(createKey(taskName,type,"p2_fill"),p2_fill );
    printValue(createKey(taskName,type,"FillPtvsY"),ptVsRapidity_fill );
    printValue(createKey(taskName,type,"n1_nbins"),n1_nbins);
    printValue(createKey(taskName,type,"n1_min"),n1_min);
    printValue(createKey(taskName,type,"n1_max"),n1_max);
    printValue(createKey(taskName,type,"pt_nbins"),pt_nbins);
    printValue(createKey(taskName,type,"pt_min"),pt_min);
    printValue(createKey(taskName,type,"pt_max"),pt_max);
    printValue(createKey(taskName,type,"phi_nbins"),phi_nbins);
    printValue(createKey(taskName,type,"phi_min"),phi_min);
    printValue(createKey(taskName,type,"phi_max"),phi_max);
    printValue(createKey(taskName,type,"eta_nbins"),eta_nbins);
    printValue(createKey(taskName,type,"eta_min"),eta_min);
    printValue(createKey(taskName,type,"eta_max"),eta_max);
    printValue(createKey(taskName,type,"rapidity_nbins"),rapidity_nbins);
    printValue(createKey(taskName,type,"rapidity_min"),rapidity_minimum);
    printValue(createKey(taskName,type,"rapidity_max"),rapidity_maximum);
    printLine();
    printCR();
    }
  }

  // for now use the same boundaries for eta and y histogram
  void ParticleSingleHistos::create()
  {
  if (reportDebug(__FUNCTION__)) { printCR();}
  h_n1         = createHistogram(createName(_histogramBaseName,"n1"),           n1_nbins,  n1_min,  n1_max,  "n_1","N",2);
  h_n1_eTotal  = createHistogram(createName(_histogramBaseName,"n1_eTotal"),    n1_nbins,  n1_min,  10.0*n1_max,  "n1_eTotal","N",2);
  h_n1_pt      = createHistogram(createName(_histogramBaseName,"n1_pt"),        pt_nbins,  pt_min,  pt_max,  "p_{T}","N",2);
  h_n1_ptXS    = createHistogram(createName(_histogramBaseName,"n1_ptXS"),      pt_nbins,  pt_min,  pt_max,  "p_{T}","1/p_{T} dN/p_{T}",2);
  if (eta_fill)
    {
    h_n1_phiEta  = createHistogram(createName(_histogramBaseName,"n1_phiEta"),  eta_nbins, eta_min, eta_max, phi_nbins, phi_min, phi_max, "#eta", "#varphi","N",2);
    if (p2_fill)
      {
      h_spt_phiEta  = createHistogram(createName(_histogramBaseName,"spt_phiEta"),  eta_nbins, eta_min, eta_max, phi_nbins, phi_min, phi_max, "#eta", "#varphi","N",2);
      }
    }
  if (rapidity_fill)
    {
    h_n1_phiY  = createHistogram(createName(_histogramBaseName,"n1_phiY"),     rapidity_nbins,   rapidity_minimum,   rapidity_maximum, phi_nbins, phi_min, phi_max, "y", "#varphi","N",2);
    if (p2_fill)
      {
      h_spt_phiY  = createHistogram(createName(_histogramBaseName,"spt_phiY"),  rapidity_nbins,   rapidity_minimum,   rapidity_maximum, phi_nbins, phi_min, phi_max, "y", "#varphi","N",2);
      }
    }
  if (ptVsRapidity_fill)
    {
    h_n1_ptY  = createHistogram(createName(_histogramBaseName,"n1_ptY"),     rapidity_nbins,   rapidity_minimum,   rapidity_maximum, pt_nbins, pt_min, pt_max, "y", "p_{T}","N",2);
    }
  if (pid_fill)
    h_pdgId  = createHistogram(createName(_histogramBaseName,"n1_indexId"),  6000,  -3000.5, 2999.5, "Index", "N");
  if (reportDebug(__FUNCTION__)) { printCR(); }
  }

  void ParticleSingleHistos::loadFrom(TFile & inputFile)
  {
  if (reportDebug(__FUNCTION__)) { printCR();}
  h_n1         = importH1(inputFile,  createName(_histogramBaseName,"n1"));
  h_n1_eTotal  = importH1(inputFile,  createName(_histogramBaseName,"n1_eTotal"));
  h_n1_pt      = importH1(inputFile,  createName(_histogramBaseName,"n1_pt"));
  h_n1_ptXS    = importH1(inputFile,  createName(_histogramBaseName,"n1_ptXS"));

  if (eta_fill)
    {
    h_n1_phiEta  = importH2(inputFile,createName(_histogramBaseName,"n1_phiEta"));
    if (p2_fill)
      {
      h_spt_phiEta  = importH2(inputFile,createName(_histogramBaseName,"spt_phiEta"));
      }
    }
  if (rapidity_fill)
    {
    h_n1_phiY  = importH2(inputFile,createName(_histogramBaseName,"n1_phiY"));
    if (p2_fill)
      {
      h_spt_phiY  = importH2(inputFile,  createName(_histogramBaseName,"spt_phiY"));
      }
    }
  if (ptVsRapidity_fill)
    {
    h_n1_ptY  = importH2(inputFile,  createName(_histogramBaseName,"n1_ptY"));
    }

  if (pid_fill)
    h_pdgId  = importH1(inputFile,  createName(_histogramBaseName,"n1_indexId"));
  }

  //!
  //! Fiil  single particle histograms of this class with the particles contained in the given list.
  //!
  void ParticleSingleHistos::fill(Particle & particle, double weight)
  {
  // Defensive: skip particles whose 4-momentum is broken or whose
  // type pointer is null.  Allows CAP to safely process partons,
  // gauge bosons, and any other "exotic" PDG that may flow in via
  // the BF-per-stage workflow (status != 1, etc.).
  VectorLorentz & momentum = particle.momentum();
  float pt   = momentum.perp();
  if (!std::isfinite(pt)) return;
  // pseudorapidity() can return ±inf for pt=0; rapidity() THROWS a
  // MathException("arg<=0") when (E+pz)/(E-pz) <= 0 (partons exactly
  // along beam axis, or off-shell with E < |pz|).  Both are common
  // when KeepStatuses includes 23/51/52 (partons).  Compute defensively.
  float eta;
  try { eta = momentum.pseudorapidity(); }
  catch (...) { eta = std::numeric_limits<float>::quiet_NaN(); }
  float phi  = momentum.phi();
  if (phi<0) phi += CAP::MATH::twoPi();
  float rapidity;
  try { rapidity = momentum.rapidity(); }
  catch (...) { rapidity = std::numeric_limits<float>::quiet_NaN(); }
  h_n1_pt  ->Fill(pt,weight);
  // ptXS divides by pt — guard against pt==0 (partons exactly along
  // beam axis can have pt=0).
  if (pt > 0.0) h_n1_ptXS->Fill(pt,weight/pt);
  if (eta_fill)
    {
    if (std::isfinite(eta))
      {
      h_n1_phiEta->Fill(eta,phi,weight);
      if (p2_fill) h_spt_phiEta->Fill(eta,phi,weight*pt);
      }
    }
  if (rapidity_fill)
    {
    if (std::isfinite(rapidity))
      {
      h_n1_phiY->Fill(rapidity,phi,weight);
      if (p2_fill) h_spt_phiY->Fill(rapidity,phi,weight*pt);
      }
    }
  if (ptVsRapidity_fill)
    {
    if (std::isfinite(rapidity))
      h_n1_ptY->Fill(rapidity,pt);
    }
  if (pid_fill)
    {
    // type() dereferences _type — null check first.
    if (particle.typePtr())
      {
      int pdgIndex = particle.type().pdgCode();
      h_pdgId->Fill(pdgIndex);
      }
    }
  }

  //!
  //! Fiil the global histogram for the multiplicity and the total energy of an event.
  //! Call this function only once per event.
  //!
  void ParticleSingleHistos::fillMultiplicity(double nAccepted, double totalEnergy, double weight)
  {
  h_n1->Fill(nAccepted, weight);
  h_n1_eTotal->Fill(totalEnergy, weight);
  }

  void ParticleSingleHistos::scaleObject(double factor)
  {
  h_n1->Scale(factor);
  h_n1_eTotal->Scale(factor);
  h_n1_pt->Scale(factor);
  h_n1_ptXS->Scale(factor);  // 1/pt dN/dptdy
  if (eta_fill)
    {
    h_n1_phiEta->Scale(factor);
    if (p2_fill) h_spt_phiEta->Scale(factor);
    }
  if (rapidity_fill)
    {
    h_n1_phiY->Scale(factor);
    if (p2_fill) h_spt_phiY->Scale(factor);
    }
  if (ptVsRapidity_fill)
    {
    h_n1_ptY->Scale(factor);
    }
  if (pid_fill)
    {
    h_pdgId->Scale(factor);
    }


  }


} // namespace CAP
