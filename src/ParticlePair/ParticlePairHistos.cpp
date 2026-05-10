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
#include "ParticlePairHistos.hpp"
#include "MathConstants.hpp"
#include "PrintHelpers.hpp"
#include "Task.hpp"
#include "Configuration.hpp"
#include "NameHelpers.hpp"
#include "RootHistogramHelpers.hpp"
#include <cmath>      // std::isfinite — defensive guards for BF-per-stage
#include <limits>     // quiet_NaN
#include <exception>

ClassImp(CAP::ParticlePairHistos);

namespace CAP
{
  ParticlePairHistos::ParticlePairHistos()
  :
  HistogramGroup(),
  eta_fill(false),
  rapidity_fill(false),
  p2_fill(false),
  n2_nbins(100),
  pt_nbins(100),
  phi_nbins(72),
  eta_nbins(200),
  rapidity_nbins(200),
  Dphi_nbins(0),
  Deta_nbins(0),
  Drapidity_nbins(0),
  Dphi_nbins_shft(0),
  n2_min(0.0),
  n2_max(10.0),
  pt_min(0.0),
  pt_max(10.0),
  phi_min(0),
  phi_max(2.0*3.14159265359),
  eta_min(-5.0),
  eta_max(5.0),
  rapidity_minimum(-10.0),
  rapidity_maximum(10.0),
  phi_scale(0),
  rapidity_scale(0),
  eta_scale(0),
  pt_scale(0),
  Dphi_width(0),
  Dphi_min(0),
  Dphi_max(0),
  Deta_min(0),
  Deta_max(0),
  Drapidity_min(0),
  Drapidity_max(0),
  Dphi_min_shft(0),
  Dphi_max_shft(0),
  rapidity_maximumAcc(0),
  h_n2(nullptr),
  h_n2_ptpt(nullptr),
  h_n2_etaeta(nullptr),
  h_DptDpt_etaeta(nullptr),
  h_n2_phiphi(nullptr),
  h_DptDpt_phiphi(nullptr),
  h_n2_yY(nullptr),
  h_DptDpt_yY(nullptr),
  h_n2_DetaDphi(nullptr),
  h_DptDpt_DetaDphi(nullptr),
  h_n2_DyDphi(nullptr),
  h_DptDpt_DyDphi(nullptr)
  {
  appendClassName("ParticlePairHistos");
  setName("ParticlePairHistos");
  setTitle("ParticlePairHistos");
  }

  ParticlePairHistos::ParticlePairHistos(const ParticlePairHistos & source)
  :
  HistogramGroup(),
  eta_fill(source.eta_fill),
  rapidity_fill(source.rapidity_fill),
  p2_fill(source.p2_fill),
  n2_nbins(source.n2_nbins),
  pt_nbins(source.pt_nbins),
  phi_nbins(source.phi_nbins),
  eta_nbins(source.eta_nbins),
  rapidity_nbins(source.rapidity_nbins),
  Dphi_nbins(source.Dphi_nbins),
  Deta_nbins(source.Deta_nbins),
  Drapidity_nbins(source.Drapidity_nbins),
  Dphi_nbins_shft(source.Dphi_nbins_shft),
  n2_min(source.n2_min),
  n2_max(source.n2_max),
  pt_min(source.pt_min),
  pt_max(source.pt_max),
  phi_min(source.phi_min),
  phi_max(source.phi_max),
  eta_min(source.eta_min),
  eta_max(source.eta_max),
  rapidity_minimum(source.rapidity_minimum),
  rapidity_maximum(source.rapidity_maximum),
  phi_scale(source.phi_scale),
  rapidity_scale(source.rapidity_scale),
  eta_scale(source.eta_scale),
  pt_scale(source.pt_scale),
  Dphi_width(source.Dphi_width),
  Dphi_min(source.Dphi_min),
  Dphi_max(source.Dphi_max),
  Deta_min(source.Deta_min),
  Deta_max(source.Deta_max),
  Drapidity_min(source.Drapidity_min),
  Drapidity_max(source.Drapidity_max),
  Dphi_min_shft(source.Dphi_min_shft),
  Dphi_max_shft(source.Dphi_max_shft),
  rapidity_maximumAcc(source.rapidity_maximumAcc),
  h_n2(nullptr),
  h_n2_ptpt(nullptr),
  h_n2_etaeta(nullptr),
  h_DptDpt_etaeta(nullptr),
  h_n2_phiphi(nullptr),
  h_DptDpt_phiphi(nullptr),
  h_n2_yY(nullptr),
  h_DptDpt_yY(nullptr),
  h_n2_DetaDphi(nullptr),
  h_DptDpt_DetaDphi(nullptr),
  h_n2_DyDphi(nullptr),
  h_DptDpt_DyDphi(nullptr)
  {
  cloneB(source);
  }

  ParticlePairHistos & ParticlePairHistos::operator=(const ParticlePairHistos & source)
  {
  if (reportDebug(__FUNCTION__)) { printCR(); }
  if (this!=&source)
    {
    HistogramGroup::operator=(source);
    eta_fill      =  source.eta_fill;
    rapidity_fill =  source.rapidity_fill;
    p2_fill       =  source.p2_fill;
    n2_nbins      =  source.n2_nbins;
    pt_nbins      =  source.pt_nbins;
    phi_nbins     =  source.phi_nbins;
    eta_nbins     =  source.eta_nbins;
    rapidity_nbins     =  source.rapidity_nbins;
    Dphi_nbins  =  source.Dphi_nbins;
    Deta_nbins  =  source.Deta_nbins;
    Drapidity_nbins    =  source.Drapidity_nbins;
    Dphi_nbins_shft = source.Dphi_nbins_shft;
    n2_min      =  source.n2_min;
    n2_max      =  source.n2_max;
    pt_min      =  source.pt_min;
    pt_max      =  source.pt_max;
    phi_min     =  source.phi_min;
    phi_max     =  source.phi_max;
    eta_min     =  source.eta_min;
    eta_max     =  source.eta_max;
    rapidity_minimum       =  source.rapidity_minimum;
    rapidity_maximum       =  source.rapidity_maximum;
    phi_scale   =  source.phi_scale;
    rapidity_scale     =  source.rapidity_scale;
    eta_scale   =  source.eta_scale;
    pt_scale    =  source.pt_scale;
    Dphi_width  =  source.Dphi_width;
    Dphi_min    =  source.Dphi_min;
    Dphi_max    =  source.Dphi_max;
    Deta_min    =  source.Deta_min;
    Deta_max    =  source.Deta_max;
    Drapidity_min       =  source.Drapidity_min;
    Drapidity_max       =  source.Drapidity_max;
    Dphi_min_shft       =  source.Dphi_min_shft;
    Dphi_max_shft       =  source.Dphi_max_shft;
    rapidity_maximumAcc =  source.rapidity_maximumAcc;
    cloneB(source);
    }
  return *this;
  }

  void ParticlePairHistos::cloneB(const ParticlePairHistos & source)
  {
  if (reportDebug(__FUNCTION__)) { printCR(); }
  h_n2              = safeCloneH1(source.h_n2);
  h_n2_ptpt         = safeCloneH2(source.h_n2_ptpt);
  h_n2_etaeta       = safeCloneH2(source.h_n2_etaeta);
  h_DptDpt_etaeta   = safeCloneH2(source.h_DptDpt_etaeta);
  h_n2_phiphi       = safeCloneH2(source.h_n2_phiphi);
  h_DptDpt_phiphi   = safeCloneH2(source.h_DptDpt_phiphi);
  h_n2_yY           = safeCloneH2(source.h_n2_yY);
  h_DptDpt_yY       = safeCloneH2(source.h_DptDpt_yY);
  h_n2_DetaDphi     = safeCloneH2(source.h_n2_DetaDphi);
  h_DptDpt_DetaDphi = safeCloneH2(source.h_DptDpt_DetaDphi);
  h_n2_DyDphi       = safeCloneH2(source.h_n2_DyDphi);
  h_DptDpt_DyDphi   = safeCloneH2(source.h_DptDpt_DyDphi);
  }

  void ParticlePairHistos::configure(const String & taskName,
                                     const String & type,
                                     const Configuration & configuration,
                                     unsigned int index)
  {
  String objectType = "HISTOGRAM";
  HistogramGroup::configure(taskName,type,configuration,index);
  eta_fill         = configuration.valueBool(createKey(taskName,objectType,"eta_fill"));
  rapidity_fill    = configuration.valueBool(createKey(taskName,objectType,"rapidity_fill"));
  p2_fill          = configuration.valueBool(createKey(taskName,objectType,"p2_fill"));
  n2_nbins         = configuration.valueInt( createKey(taskName,objectType,"n2_nbins"));
  n2_min           = configuration.valueDouble(createKey(taskName,objectType,"n2_min"));
  n2_max           = configuration.valueDouble(createKey(taskName,objectType,"n2_max"));
  pt_nbins         = configuration.valueInt(createKey(taskName,objectType,"pt_nbins"));
  pt_min           = configuration.valueDouble(createKey(taskName,objectType,"pt_min"));
  pt_max           = configuration.valueDouble(createKey(taskName,objectType,"pt_max"));
  phi_nbins        = configuration.valueInt(createKey(taskName,objectType,"phi_nbins"));
  phi_min          = configuration.valueDouble(createKey(taskName,objectType,"phi_min"));
  phi_max          = configuration.valueDouble(createKey(taskName,objectType,"phi_max"));
  eta_nbins        = configuration.valueInt(createKey(taskName,objectType,"eta_nbins"));
  eta_min          = configuration.valueDouble(createKey(taskName,objectType,"eta_min"));
  eta_max          = configuration.valueDouble(createKey(taskName,objectType,"eta_max"));
  rapidity_nbins   = configuration.valueInt(createKey(taskName,objectType,"rapidity_nbins"));
  rapidity_minimum = configuration.valueDouble(createKey(taskName,objectType,"rapidity_min"));
  rapidity_maximum = configuration.valueDouble(createKey(taskName,objectType,"rapidity_max"));
  phi_scale        = double(phi_nbins)/(phi_max-phi_min);
  rapidity_scale   = double(rapidity_nbins)/(rapidity_maximum-rapidity_minimum);
  eta_scale        = double(eta_nbins)/(eta_max-eta_min);
  pt_scale         = double(pt_nbins)/(pt_max-pt_min);

  Dphi_nbins  = phi_nbins;
  Deta_nbins  = 2*eta_nbins-1;
  Drapidity_nbins    = 2*rapidity_nbins-1;
  Dphi_width  = CAP::MATH::twoPi()/double(Dphi_nbins);
  Dphi_min    = -Dphi_width/2.0;
  Dphi_max    = CAP::MATH::twoPi() - Dphi_width/2.;
  Deta_min    = -(eta_max-eta_min);
  Deta_max    = eta_max-eta_min;
  Drapidity_min      = -(rapidity_maximum-rapidity_minimum);
  Drapidity_max      = rapidity_maximum-rapidity_minimum;

  Dphi_nbins_shft = phi_nbins/4;
  Dphi_min_shft   = Dphi_min - CAP::MATH::twoPi()/4.0;
  Dphi_max_shft   = Dphi_max - CAP::MATH::twoPi()/4.0;
  rapidity_maximumAcc = 4.0;

  if (reportInfo(__FUNCTION__))
    {
    printCR();
    printValue("eta_fill",eta_fill);
    printValue("rapidity_fill",rapidity_fill);
    printValue("p2_fill",p2_fill);
    printValue("n2_nbins",n2_nbins);
    printValue("n2_min",n2_min);
    printValue("n2_max",n2_max);
    printValue("pt_nbins",pt_nbins);
    printValue("pt_min",pt_min);
    printValue("pt_max",pt_max);
    printValue("pt_scale",pt_scale);
    printValue("phi_nbins",phi_nbins);
    printValue("phi_min",phi_min);
    printValue("phi_max",phi_max);
    printValue("phi_scale",phi_scale);
    printValue("eta_nbins",eta_nbins);
    printValue("eta_min",eta_min);
    printValue("eta_max",eta_max);
    printValue("eta_scale",eta_scale);
    printValue("rapidity_nbins",rapidity_nbins);
    printValue("rapidity_min",rapidity_minimum);
    printValue("rapidity_max",rapidity_maximum);
    printValue("rapidity_scale",rapidity_scale);
    printValue("Dphi_nbins",phi_nbins);
    printValue("Dphi_min",Dphi_min);
    printValue("Dphi_max",Dphi_max);
    printValue("Dphi_width",Dphi_width);
    printValue("Deta_nbins",eta_nbins);
    printValue("Deta_min",Deta_min);
    printValue("Deta_max",Deta_max);
    printValue("Drapidity_nbins",Drapidity_nbins);
    printValue("Drapidity_min",Drapidity_min);
    printValue("Drapidity_max",Drapidity_max);
    }
  }

  void ParticlePairHistos::create()
  {
  if (reportInfo(__FUNCTION__)) { printCR();}
  h_n2          = createHistogram(createName(_histogramBaseName,"n2"),         n2_nbins,  n2_min,  n2_max, "n_{2}", "Yield");
  h_n2_ptpt     = createHistogram(createName(_histogramBaseName,"n2_ptpt"),    pt_nbins,  pt_min,  pt_max, pt_nbins, pt_min, pt_max,   "p_{T,1}",  "p_{T,2}", "N_{2}");
  if (!h_n2_ptpt) throw Exception("!h_n2_ptpt",__FUNCTION__);


  h_n2_phiphi   = createHistogram(createName(_histogramBaseName,"n2_phiphi"),  phi_nbins, phi_min, phi_max, phi_nbins, phi_min, phi_max, "#varphi_{1}", "#varphi_{2}", "N_{2}");

  if (p2_fill)
    {
    h_DptDpt_phiphi = createHistogram(createName(_histogramBaseName,"ptpt_phiphi"),   phi_nbins, phi_min, phi_max, phi_nbins, phi_min, phi_max, "#varphi_{1}", "#varphi_{2}", "p_{T}xp_{T}");
    }

  if (eta_fill)
    {
    h_n2_etaeta   = createHistogram(createName(_histogramBaseName,"n2_etaeta"),   eta_nbins,  eta_min, eta_max, eta_nbins, eta_min, eta_max, "#eta_{1}", "#eta_{2}", "N_{2}");
    h_n2_DetaDphi = createHistogram(createName(_histogramBaseName,"n2_DetaDphi"), Deta_nbins, Deta_min, Deta_max, Dphi_nbins, Dphi_min, Dphi_max, "#Delta#eta", "#Delta#phi", "N_{2}");
    if (p2_fill)
      {
      h_DptDpt_etaeta = createHistogram(createName(_histogramBaseName,"ptpt_etaeta"), eta_nbins, eta_min, eta_max, eta_nbins, eta_min, eta_max, "#eta_{1}", "#eta_{2}", "p_{T}xp_{T}");
      h_DptDpt_DetaDphi = createHistogram(createName(_histogramBaseName,"ptpt_DetaDphi"),Deta_nbins, Deta_min, Deta_max, Dphi_nbins, Dphi_min, Dphi_max, "#Delta#eta", "#Delta#phi", "ptpt");
      }
    }

  if (rapidity_fill)
    {
    h_n2_yY     = createHistogram(createName(_histogramBaseName,"n2_yY"),     rapidity_nbins,  rapidity_minimum,  rapidity_maximum,  rapidity_nbins, rapidity_minimum, rapidity_maximum, "y_{1}","y_{2}", "N_{2}");
    h_n2_DyDphi = createHistogram(createName(_histogramBaseName,"n2_DyDphi"), Drapidity_nbins, Drapidity_min, Drapidity_max, Dphi_nbins, Dphi_min, Dphi_max, "#Delta y", "#Delta#phi", "N_{2}");
    if (p2_fill)
      {
      h_DptDpt_yY    = createHistogram(createName(_histogramBaseName,"ptpt_yY"),  rapidity_nbins,  rapidity_minimum, rapidity_maximum, rapidity_nbins, rapidity_minimum, rapidity_maximum, "y_{1}","y_{2}", "p_{T}xp_{T}");
      h_DptDpt_DyDphi = createHistogram(createName(_histogramBaseName,"ptpt_DyDphi"),Drapidity_nbins, Drapidity_min, Drapidity_max, Dphi_nbins, Dphi_min, Dphi_max, "#Delta y", "#Delta#phi", "ptpt");
      }
    }
  }

  void ParticlePairHistos::loadFrom(TFile & inputFile)
  {
  if (reportDebug(__FUNCTION__)) { printCR();}
  h_n2          = importH1(inputFile,createName(_histogramBaseName,"n2"));
  h_n2_ptpt     = importH2(inputFile,createName(_histogramBaseName,"n2_ptpt"));
  h_n2_phiphi   = importH2(inputFile,createName(_histogramBaseName,"n2_phiphi"));

  if (p2_fill)
    {
    h_DptDpt_phiphi = importH2(inputFile,createName(_histogramBaseName,"ptpt_phiphi"));
    }

  if (eta_fill)
    {
    h_n2_etaeta   = importH2(inputFile,createName(_histogramBaseName,"n2_etaeta"));
    h_n2_DetaDphi = importH2(inputFile,createName(_histogramBaseName,"n2_DetaDphi"));
    if (p2_fill)
      {
      h_DptDpt_etaeta   = importH2(inputFile,createName(_histogramBaseName,"ptpt_etaeta"));
      h_DptDpt_DetaDphi = importH2(inputFile,createName(_histogramBaseName,"ptpt_DetaDphi"));
      }
    }
  if (rapidity_fill)
    {
    h_n2_yY     = importH2(inputFile,createName(_histogramBaseName,"n2_yY"));
    h_n2_DyDphi = importH2(inputFile,createName(_histogramBaseName,"n2_DyDphi"));
    if (p2_fill)
      {
      h_DptDpt_yY     = importH2(inputFile,createName(_histogramBaseName,"ptpt_yY"));
      h_DptDpt_DyDphi = importH2(inputFile,createName(_histogramBaseName,"ptpt_DyDphi"));
      }
    }
  }


  void ParticlePairHistos::fill(Particle & particle1, Particle & particle2, double weight)
  {
  int iPhi1, iPt1, iEta1, iY1;
  int iPhi2, iPt2, iEta2, iY2;
  int iGptpt;
  int iGphiphi;
  int iGetaeta;
  int iGYY;
  int iGDeltaEtaDeltaPhi;
  int iGDeltaYDeltaPhi;

  VectorLorentz & momentum1 = particle1.momentum();
  double pt1   = momentum1.perp();
  // Defensive: NaN/inf pt would crash binning math.  Allows safe
  // BF-per-stage runs on partons / exotic species.
  if (!std::isfinite(pt1)) return;
  iPt1 = ptBinFor(pt1);
  double phi1  = momentum1.phi();   if (phi1<0.0) phi1 += CAP::MATH::twoPi(); iPhi1 = phiBinFor(phi1);
  // pseudorapidity()/rapidity() can throw or return inf for partons
  // with E ≤ |pz|.  Catch and substitute NaN; the bin lookup will
  // return the under/overflow bin which we filter via iEta1==0/iY1==0.
  double eta1, y1;
  try { eta1 = momentum1.pseudorapidity(); }
  catch (...) { eta1 = std::numeric_limits<double>::quiet_NaN(); }
  iEta1 = (eta_fill && std::isfinite(eta1)) ? getEtaBinFor(eta1) : 0;
  try { y1 = momentum1.rapidity(); }
  catch (...) { y1 = std::numeric_limits<double>::quiet_NaN(); }
  iY1 = (rapidity_fill && std::isfinite(y1)) ? getYBinFor(y1) : 0;
  //  printValue("ParticlePairHistos::fill() eta_fill",eta_fill);
  //  printValue("ParticlePairHistos::fill() rapidity_fill",rapidity_fill);
  //  printValue("ParticlePairHistos::fill() pt1",pt1);
  //  printValue("ParticlePairHistos::fill() phi1",phi1);
  //  printValue("ParticlePairHistos::fill() eta1",eta1);
  //  printValue("ParticlePairHistos::fill() y1",y1);
  //  printValue("ParticlePairHistos::fill() iPt1",iPt1);
  //  printValue("ParticlePairHistos::fill() iEta1",iEta1);
  //  printValue("ParticlePairHistos::fill() iY1",iY1);
  //  printValue("ParticlePairHistos::fill() iPhi1",iPhi1);
  VectorLorentz & momentum2 = particle2.momentum();
  double pt2   = momentum2.perp();
  if (!std::isfinite(pt2)) return;
  iPt2 = ptBinFor(pt2);
  double phi2  = momentum2.phi();   if (phi2<0.0) phi2 += CAP::MATH::twoPi(); iPhi2 = phiBinFor(phi2);
  double eta2, y2;
  try { eta2 = momentum2.pseudorapidity(); }
  catch (...) { eta2 = std::numeric_limits<double>::quiet_NaN(); }
  iEta2 = (eta_fill && std::isfinite(eta2)) ? getEtaBinFor(eta2) : 0;
  try { y2 = momentum2.rapidity(); }
  catch (...) { y2 = std::numeric_limits<double>::quiet_NaN(); }
  iY2 = (rapidity_fill && std::isfinite(y2)) ? getYBinFor(y2) : 0;

  if (iPt1==0  || iPt2==0)  return;
  if (iPhi1==0 || iPhi1==0) return;
  if (iEta1==0  &&  iY1==0) return;
  if (iEta2==0  &&  iY2==0) return;
  int iDeltaEta  = iEta1-iEta2 + eta_nbins-1;
  int iDeltaY    = iY1-iY2 + rapidity_nbins-1;
  int iDeltaPhi  = iPhi1-iPhi2;
  if (iDeltaPhi < 0) iDeltaPhi += phi_nbins;
  //cout <<  "iDeltaY:" << iDeltaY << " iDeltaPhi: " << iDeltaPhi << endl;
  // Removed two leftover developer overrides here that unconditionally set
  // `p2_fill = false; rapidity_fill = true;`.  The rapidity_fill=true
  // override was a real bug: it forced the rapidity branch (line ~439)
  // to run for every pair, but the matching histograms (h_n2_yY, ...)
  // are only allocated by create() when rapidity_fill is already true at
  // creation time.  Result: a guaranteed nullptr-deref segfault on the
  // first pair when the user configured rapidity_fill=0.  Keep the
  // configured values intact instead.
  //  printValue("ParticlePairHistos::fill() iPt1",iPt1);
  //  printValue("ParticlePairHistos::fill() iPt2",iPt2);
  if (!h_n2_ptpt) throw Exception("!h_n2_ptpt",__FUNCTION__);
  if (!h_n2_phiphi) throw Exception("!h_n2_phiphi",__FUNCTION__);
  // printValue("ParticlePairHistos::fill()",6);
  iGptpt   = h_n2_ptpt->GetBin(iPt1,iPt2);
  //printValue("ParticlePairHistos::fill()",7);
  iGphiphi = h_n2_phiphi->GetBin(iPhi1,iPhi2);
  //printValue("ParticlePairHistos::fill()",8);
  h_n2_ptpt   ->AddBinContent(iGptpt,    weight);  h_n2_ptpt  ->SetEntries(h_n2_ptpt  ->GetEntries()+1);
  h_n2_phiphi ->AddBinContent(iGphiphi,  weight);  h_n2_phiphi->SetEntries(h_n2_phiphi->GetEntries()+1);
  //printValue("ParticlePairHistos::fill()",9);

  if (p2_fill)
    {
    //printValue("ParticlePairHistos::fill()",10);

    h_DptDpt_phiphi->AddBinContent(iGphiphi,weight*pt1*pt2);
    h_DptDpt_phiphi->SetEntries(h_DptDpt_phiphi->GetEntries()+1);
    }
  //printValue("ParticlePairHistos::fill()",11);

  if (eta_fill  &&  iEta1!=0  &&  iEta2!=0 )
    {
    //printValue("ParticlePairHistos::fill()",12);

    iGetaeta           = h_n2_etaeta->GetBin(iEta1,iEta2);
    iGDeltaEtaDeltaPhi = h_n2_DetaDphi->GetBin(iDeltaEta+1,iDeltaPhi+1);
    h_n2_etaeta->AddBinContent(iGetaeta,weight);             h_n2_etaeta  ->SetEntries(h_n2_etaeta  ->GetEntries()+1);
    h_n2_DetaDphi->AddBinContent(iGDeltaEtaDeltaPhi,weight); h_n2_DetaDphi->SetEntries(h_n2_DetaDphi->GetEntries()+1);

    if (p2_fill)
      {
      h_DptDpt_etaeta   ->AddBinContent(iGetaeta,           weight*pt1*pt2); h_DptDpt_etaeta->SetEntries(h_DptDpt_etaeta    ->GetEntries()+1);
      h_DptDpt_DetaDphi ->AddBinContent(iGDeltaEtaDeltaPhi, weight*pt1*pt2); h_DptDpt_DetaDphi->SetEntries(h_DptDpt_DetaDphi->GetEntries()+1);
      }
    }
  //printValue("ParticlePairHistos::fill()",13);

  if (rapidity_fill  &&  iY1!=0  &&  iY2!=0 )
    {
    //printValue("ParticlePairHistos::fill()",14);

    iGYY             = h_n2_yY->GetBin(iY1,iY2);
    iGDeltaYDeltaPhi = h_n2_DyDphi->GetBin(iDeltaY+1,iDeltaPhi+1);
    h_n2_yY      ->AddBinContent(iGYY,weight);              h_n2_yY      ->SetEntries(h_n2_yY      ->GetEntries()+1);
    h_n2_DyDphi  ->AddBinContent(iGDeltaYDeltaPhi,weight);  h_n2_DyDphi  ->SetEntries(h_n2_DyDphi  ->GetEntries()+1);
    if (p2_fill)
      {
      //printValue("ParticlePairHistos::fill()",15);

      h_DptDpt_yY     ->AddBinContent(iGYY, weight*pt1*pt2);               h_DptDpt_yY ->SetEntries(   h_DptDpt_yY     ->GetEntries()+1);
      h_DptDpt_DyDphi ->AddBinContent(iGDeltaEtaDeltaPhi, weight*pt1*pt2); h_DptDpt_DyDphi->SetEntries(h_DptDpt_DyDphi->GetEntries()+1);
      }
    }
  //printValue("ParticlePairHistos::fill()",20);

  }

  void ParticlePairHistos::fillMultiplicity(double nPairs, double weight)
  {
  h_n2->Fill(nPairs, weight);
  }

  void ParticlePairHistos::scaleObject(double factor)
  {
  h_n2->Scale(factor);
  h_n2_ptpt->Scale(factor);
  h_n2_phiphi->Scale(factor);
  if (p2_fill) h_DptDpt_phiphi->Scale(factor);
  if (eta_fill)
    {
    h_n2_etaeta->Scale(factor);
    h_n2_DetaDphi->Scale(factor);
    if (p2_fill)
      {
      h_DptDpt_etaeta->Scale(factor);
      h_DptDpt_DetaDphi->Scale(factor);
      }
    }
  if (rapidity_fill)
    {
    h_n2_yY->Scale(factor);
    h_n2_DyDphi->Scale(factor);
    if (p2_fill)
      {
      h_DptDpt_yY->Scale(factor);
      h_DptDpt_DyDphi->Scale(factor);
      }
    }
  }

} // namespace CAP
