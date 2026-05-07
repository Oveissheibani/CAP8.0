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
#include "JetPairHistosDerived.hpp"
#include "JetHelpers.hpp"
#include "NameHelpers.hpp"
#include "PrintHelpers.hpp"
#include "RootHistogramHelpers.hpp"
#include "Configuration.hpp"
#include "Task.hpp"

ClassImp(CAP::JetPairHistosDerived);

namespace CAP
{

JetPairHistosDerived::JetPairHistosDerived()
:
JetPairHistos(),
h_jet_a2pp_ptpt(nullptr),
 h_jet_a2pp_phiphi(nullptr),
 h_jet_a2pp_etaeta(nullptr),
 h_jet_a2pp_thth(nullptr),
 h_jet_a2pp_jtjt(nullptr),
 h_jet_a2pp_zz(nullptr),

 h_jet_a2mp_ptpt(nullptr),
 h_jet_a2mp_phiphi(nullptr),
 h_jet_a2mp_etaeta(nullptr),
 h_jet_a2mp_thth(nullptr),
 h_jet_a2mp_jtjt(nullptr),
 h_jet_a2mp_zz(nullptr),

 h_jet_a2pm_ptpt(nullptr),
 h_jet_a2pm_phiphi(nullptr),
 h_jet_a2pm_etaeta(nullptr),
 h_jet_a2pm_thth(nullptr),
 h_jet_a2pm_jtjt(nullptr),
 h_jet_a2pm_zz(nullptr),

 h_jet_a2mm_ptpt(nullptr),
 h_jet_a2mm_phiphi(nullptr),
 h_jet_a2mm_etaeta(nullptr),
 h_jet_a2mm_thth(nullptr),
 h_jet_a2mm_jtjt(nullptr),
 h_jet_a2mm_zz(nullptr),

 h_jet_b2pm_ptpt(nullptr),
 h_jet_b2pm_phiphi(nullptr),
 h_jet_b2pm_etaeta(nullptr),
 h_jet_b2pm_thth(nullptr),
 h_jet_b2pm_jtjt(nullptr),
 h_jet_b2pm_zz(nullptr),

 h_jet_b2mp_ptpt(nullptr),
 h_jet_b2mp_phiphi(nullptr),
 h_jet_b2mp_etaeta(nullptr),
 h_jet_b2mp_thth(nullptr),
 h_jet_b2mp_jtjt(nullptr),
 h_jet_b2mp_zz(nullptr),

 h_jet_b2s_ptpt(nullptr),
 h_jet_b2s_phiphi(nullptr),
 h_jet_b2s_etaeta(nullptr),
 h_jet_b2s_thth(nullptr),
 h_jet_b2s_jtjt(nullptr),
 h_jet_b2s_zz(nullptr)
{   }

JetPairHistosDerived::JetPairHistosDerived(const JetPairHistosDerived & group)
:
JetPairHistos(group),
h_jet_a2pp_ptpt(nullptr),
h_jet_a2pp_phiphi(nullptr),
h_jet_a2pp_etaeta(nullptr),
h_jet_a2pp_thth(nullptr),
h_jet_a2pp_jtjt(nullptr),
h_jet_a2pp_zz(nullptr),

h_jet_a2mp_ptpt(nullptr),
h_jet_a2mp_phiphi(nullptr),
h_jet_a2mp_etaeta(nullptr),
h_jet_a2mp_thth(nullptr),
h_jet_a2mp_jtjt(nullptr),
h_jet_a2mp_zz(nullptr),

h_jet_a2pm_ptpt(nullptr),
h_jet_a2pm_phiphi(nullptr),
h_jet_a2pm_etaeta(nullptr),
h_jet_a2pm_thth(nullptr),
h_jet_a2pm_jtjt(nullptr),
h_jet_a2pm_zz(nullptr),

h_jet_a2mm_ptpt(nullptr),
h_jet_a2mm_phiphi(nullptr),
h_jet_a2mm_etaeta(nullptr),
h_jet_a2mm_thth(nullptr),
h_jet_a2mm_jtjt(nullptr),
h_jet_a2mm_zz(nullptr),

h_jet_b2pm_ptpt(nullptr),
h_jet_b2pm_phiphi(nullptr),
h_jet_b2pm_etaeta(nullptr),
h_jet_b2pm_thth(nullptr),
h_jet_b2pm_jtjt(nullptr),
h_jet_b2pm_zz(nullptr),

h_jet_b2mp_ptpt(nullptr),
h_jet_b2mp_phiphi(nullptr),
h_jet_b2mp_etaeta(nullptr),
h_jet_b2mp_thth(nullptr),
h_jet_b2mp_jtjt(nullptr),
h_jet_b2mp_zz(nullptr),

h_jet_b2s_ptpt(nullptr),
h_jet_b2s_phiphi(nullptr),
h_jet_b2s_etaeta(nullptr),
h_jet_b2s_thth(nullptr),
h_jet_b2s_jtjt(nullptr),
h_jet_b2s_zz(nullptr)
{   }

JetPairHistosDerived & JetPairHistosDerived::operator=(const JetPairHistosDerived & group)
{
  if (this!=&group)
    {
    JetPairHistos::operator=(group);
    }
  return *this;
}

void JetPairHistosDerived::cloneD(const JetPairHistosDerived & source)
{
  h_jet_a2pp_ptpt   = safeCloneH2(source.h_jet_a2pp_ptpt);
  h_jet_a2pp_phiphi = safeCloneH2(source.h_jet_a2pp_phiphi);
  h_jet_a2pp_etaeta = safeCloneH2(source.h_jet_a2pp_etaeta);
  h_jet_a2pp_thth   = safeCloneH2(source.h_jet_a2pp_thth);
  h_jet_a2pp_jtjt   = safeCloneH2(source.h_jet_a2pp_jtjt);
  h_jet_a2pp_zz     = safeCloneH2(source.h_jet_a2pp_zz);

  h_jet_a2mp_ptpt   = safeCloneH2(source.h_jet_a2mp_ptpt);
  h_jet_a2mp_phiphi = safeCloneH2(source.h_jet_a2mp_phiphi);
  h_jet_a2mp_etaeta = safeCloneH2(source.h_jet_a2mp_etaeta);
  h_jet_a2mp_thth   = safeCloneH2(source.h_jet_a2mp_thth);
  h_jet_a2mp_jtjt   = safeCloneH2(source.h_jet_a2mp_jtjt);
  h_jet_a2mp_zz     = safeCloneH2(source.h_jet_a2mp_zz);

  h_jet_a2pm_ptpt   = safeCloneH2(source.h_jet_a2pm_ptpt);
  h_jet_a2pm_phiphi = safeCloneH2(source.h_jet_a2pm_phiphi);
  h_jet_a2pm_etaeta = safeCloneH2(source.h_jet_a2pm_etaeta);
  h_jet_a2pm_thth   = safeCloneH2(source.h_jet_a2pm_thth);
  h_jet_a2pm_jtjt   = safeCloneH2(source.h_jet_a2pm_jtjt);
  h_jet_a2pm_zz     = safeCloneH2(source.h_jet_a2pm_zz);

  h_jet_a2mm_ptpt   = safeCloneH2(source.h_jet_a2mm_ptpt);
  h_jet_a2mm_phiphi = safeCloneH2(source.h_jet_a2mm_phiphi);
  h_jet_a2mm_etaeta = safeCloneH2(source.h_jet_a2mm_etaeta);
  h_jet_a2mm_thth   = safeCloneH2(source.h_jet_a2mm_thth);
  h_jet_a2mm_jtjt   = safeCloneH2(source.h_jet_a2mm_jtjt);
  h_jet_a2mm_zz     = safeCloneH2(source.h_jet_a2mm_zz);

  h_jet_b2pm_ptpt   = safeCloneH2(source.h_jet_b2pm_ptpt);
  h_jet_b2pm_phiphi = safeCloneH2(source.h_jet_b2pm_phiphi);
  h_jet_b2pm_etaeta = safeCloneH2(source.h_jet_b2pm_etaeta);
  h_jet_b2pm_thth   = safeCloneH2(source.h_jet_b2pm_thth);
  h_jet_b2pm_jtjt   = safeCloneH2h_jet_b2pm_jtjt;
  h_jet_b2pm_zz     = safeCloneH2(source.h_jet_b2pm_zz);

  h_jet_b2mp_ptpt   = safeCloneH2(source.h_jet_b2mp_ptpt);
  h_jet_b2mp_phiphi = safeCloneH2(source.h_jet_b2mp_phiphi);
  h_jet_b2mp_etaeta = safeCloneH2(source.h_jet_b2mp_etaeta);
  h_jet_b2mp_thth   = safeCloneH2(source.h_jet_b2mp_thth);
  h_jet_b2mp_jtjt   = safeCloneH2(source.h_jet_b2mp_jtjt);
  h_jet_b2mp_zz     = safeCloneH2(source.h_jet_b2mp_zz);

  h_jet_b2s_ptpt    = safeCloneH2(source.h_jet_b2s_ptpt);
  h_jet_b2s_phiphi  = safeCloneH2(source.h_jet_b2s_phiphi);
  h_jet_b2s_etaeta  = safeCloneH2(source.h_jet_b2s_etaeta);
  h_jet_b2s_thth    = safeCloneH2(source.h_jet_b2s_thth);
  h_jet_b2s_jtjt    = safeCloneH2(source.h_jet_b2s_jtjt);
  h_jet_b2s_zz      = safeCloneH2(source.h_jet_b2s_zz);

}


void JetPairHistosDerived::configure(const String & taskName,
                                           const String & objectType,
                                           const Configuration & configuration,
                                           unsigned int index)
{
  JetPairHistos::configure(taskName,objectType,configuration,index);
}

void JetPairHistosDerived::create()
{
  if (reportDebug(__FUNCTION__)) { printCR();}
  h_jet_a2pp_ptpt   = createHistogram(createName(_histogramBaseName,"jet_a2pp_ptpt"),  pt_nbins,pt_min,pt_max,    pt_nbins,pt_min,pt_max,    "p_{T,1}",    "p_{T,2}",    "A_{2}^{++}");
  h_jet_a2pp_phiphi = createHistogram(createName(_histogramBaseName,"jet_a2pp_phiphi"),phi_nbins,phi_min,phi_max, phi_nbins,phi_min,phi_max, "#varphi_{1}","#varphi_{1}","A_{2}^{++}");
  h_jet_a2pp_etaeta = createHistogram(createName(_histogramBaseName,"jet_a2pp_etaet"), eta_nbins,eta_min,eta_max, eta_nbins,eta_min,eta_max, "#eta_{1}",   "#eta_{2}",   "A_{2}^{++}");
  h_jet_a2pp_thth   = createHistogram(createName(_histogramBaseName,"jet_a2pp_thth"),  th_nbins,th_min,th_max,    th_nbins,th_min,th_max,    "#theta_{1}", "#theta_{1}", "A_{2}^{++}");
  h_jet_a2pp_jtjt   = createHistogram(createName(_histogramBaseName,"jet_a2pp_jtjt"),  jt_nbins,jt_min,jt_max,    jt_nbins,jt_min,jt_max,    "j_{T,1}",    "j_{T,2}",    "A_{2}^{++}");
  h_jet_a2pp_zz     = createHistogram(createName(_histogramBaseName,"jet_a2pp_zz"),    z_nbins,z_min,z_max,       z_nbins,z_min,z_max,       "z_{1}",      "z_{2}",      "A_{2}^{++}");

  h_jet_a2mp_ptpt   = createHistogram(createName(_histogramBaseName,"jet_a2mp_ptpt"),  pt_nbins,pt_min,pt_max,    pt_nbins,pt_min,pt_max,    "p_{T,1}",    "p_{T,2}",    "A_{2}^{-+}");
  h_jet_a2mp_phiphi = createHistogram(createName(_histogramBaseName,"jet_a2mp_phiphi"),phi_nbins,phi_min,phi_max, phi_nbins,phi_min,phi_max, "#varphi_{1}","#varphi_{1}","A_{2}^{-+}");
  h_jet_a2mp_etaeta = createHistogram(createName(_histogramBaseName,"jet_a2mp_etaeta"),eta_nbins,eta_min,eta_max, eta_nbins,eta_min,eta_max, "#eta_{1}",   "#eta_{2}",   "A_{2}^{-+}");
  h_jet_a2mp_thth   = createHistogram(createName(_histogramBaseName,"jet_a2mp_thth"),  th_nbins,th_min,th_max,    th_nbins,th_min,th_max,    "#theta_{1}", "#theta_{1}", "A_{2}^{-+}");
  h_jet_a2mp_jtjt   = createHistogram(createName(_histogramBaseName,"jet_a2mp_jtjt"),  jt_nbins,jt_min,jt_max,    jt_nbins,jt_min,jt_max,    "j_{T,1}",    "j_{T,2}",    "A_{2}^{-+}");
  h_jet_a2mp_zz     = createHistogram(createName(_histogramBaseName,"jet_a2mp_zz"),    z_nbins,z_min,z_max,       z_nbins,z_min,z_max,       "z_{1}",      "z_{2}",      "A_{2}^{-+}");

  h_jet_a2pm_ptpt   = createHistogram(createName(_histogramBaseName,"jet_a2pm_ptpt"),  pt_nbins,pt_min,pt_max,    pt_nbins,pt_min,pt_max,    "p_{T,1}",    "p_{T,2}",    "A_{2}^{+-}");
  h_jet_a2pm_phiphi = createHistogram(createName(_histogramBaseName,"jet_a2pm_phiphi"),phi_nbins,phi_min,phi_max, phi_nbins,phi_min,phi_max, "#varphi_{1}","#varphi_{1}","A_{2}^{+-}");
  h_jet_a2pm_etaeta = createHistogram(createName(_histogramBaseName,"jet_a2pm_etaeta"),eta_nbins,eta_min,eta_max, eta_nbins,eta_min,eta_max, "#eta_{1}",   "#eta_{2}",   "A_{2}^{+-}");
  h_jet_a2pm_thth   = createHistogram(createName(_histogramBaseName,"jet_a2pm_thth"),  th_nbins,th_min,th_max,    th_nbins,th_min,th_max,    "#theta_{1}", "#theta_{1}", "A_{2}^{+-}");
  h_jet_a2pm_jtjt   = createHistogram(createName(_histogramBaseName,"jet_a2pm_jtjt"),  jt_nbins,jt_min,jt_max,    jt_nbins,jt_min,jt_max,    "j_{T,1}",    "j_{T,2}",    "A_{2}^{+-}");
  h_jet_a2pm_zz     = createHistogram(createName(_histogramBaseName,"jet_a2pm_zz"),    z_nbins,z_min,z_max,       z_nbins,z_min,z_max,       "z_{1}",      "z_{2}",      "A_{2}^{+-}");

  h_jet_a2mm_ptpt   = createHistogram(createName(_histogramBaseName,"jet_a2mm_ptpt"),  pt_nbins,pt_min,pt_max,    pt_nbins,pt_min,pt_max,    "p_{T,1}",    "p_{T,2}",    "A_{2}^{--}");
  h_jet_a2mm_phiphi = createHistogram(createName(_histogramBaseName,"jet_a2mm_phiphi"),phi_nbins,phi_min,phi_max, phi_nbins,phi_min,phi_max, "#varphi_{1}","#varphi_{1}","A_{2}^{--}");
  h_jet_a2mm_etaeta = createHistogram(createName(_histogramBaseName,"jet_a2mm_etaeta"),eta_nbins,eta_min,eta_max, eta_nbins,eta_min,eta_max, "#eta_{1}",   "#eta_{2}",   "A_{2}^{--}");
  h_jet_a2mm_thth   = createHistogram(createName(_histogramBaseName,"jet_a2mm_thth"),  th_nbins,th_min,th_max,    th_nbins,th_min,th_max,    "#theta_{1}", "#theta_{1}", "A_{2}^{--}");
  h_jet_a2mm_jtjt   = createHistogram(createName(_histogramBaseName,"jet_a2mm_jtjt"),  jt_nbins,jt_min,jt_max,    jt_nbins,jt_min,jt_max,    "j_{T,1}",    "j_{T,2}",    "A_{2}^{--}");
  h_jet_a2mm_zz     = createHistogram(createName(_histogramBaseName,"jet_a2mm_zz"),    z_nbins,z_min,z_max,       z_nbins,z_min,z_max,       "z_{1}",      "z_{2}",      "A_{2}^{--}");

  h_jet_b2pm_ptpt   = createHistogram(createName(_histogramBaseName,"jet_b2pm_ptpt"),  pt_nbins,pt_min,pt_max,    pt_nbins,pt_min,pt_max,    "p_{T,1}",    "p_{T,2}",    "B_{2}^{+-}");
  h_jet_b2pm_phiphi = createHistogram(createName(_histogramBaseName,"jet_b2pm_phiphi"),phi_nbins,phi_min,phi_max, phi_nbins,phi_min,phi_max, "#varphi_{1}","#varphi_{1}","B_{2}^{+-}");
  h_jet_b2pm_etaeta = createHistogram(createName(_histogramBaseName,"jet_b2pm_etaeta"),eta_nbins,eta_min,eta_max, eta_nbins,eta_min,eta_max, "#eta_{1}",   "#eta_{2}",   "B_{2}^{+-}");
  h_jet_b2pm_thth   = createHistogram(createName(_histogramBaseName,"jet_b2pm_thth"),  th_nbins,th_min,th_max,    th_nbins,th_min,th_max,    "#theta_{1}", "#theta_{1}", "B_{2}^{+-}");
  h_jet_b2pm_jtjt   = createHistogram(createName(_histogramBaseName,"jet_b2pm_jtjt"),  jt_nbins,jt_min,jt_max,    jt_nbins,jt_min,jt_max,    "j_{T,1}",    "j_{T,2}",    "B_{2}^{+-}");
  h_jet_b2pm_zz     = createHistogram(createName(_histogramBaseName,"jet_b2pm_zz"),    z_nbins,z_min,z_max,       z_nbins,z_min,z_max,       "z_{1}",      "z_{2}",      "B_{2}^{+-}");

  h_jet_b2mp_ptpt   = createHistogram(createName(_histogramBaseName,"jet_b2mp_ptpt"),  pt_nbins,pt_min,pt_max,    pt_nbins,pt_min,pt_max,    "p_{T,1}",    "p_{T,2}",    "B_{2}^{-+}");
  h_jet_b2mp_phiphi = createHistogram(createName(_histogramBaseName,"jet_b2mp_phiphi"),phi_nbins,phi_min,phi_max, phi_nbins,phi_min,phi_max, "#varphi_{1}","#varphi_{1}","B_{2}^{-+}");
  h_jet_b2mp_etaeta = createHistogram(createName(_histogramBaseName,"jet_b2mp_etaeta"),eta_nbins,eta_min,eta_max, eta_nbins,eta_min,eta_max, "#eta_{1}",   "#eta_{2}",   "B_{2}^{-+}");
  h_jet_b2mp_thth   = createHistogram(createName(_histogramBaseName,"jet_b2mp_thth"),  th_nbins,th_min,th_max,    th_nbins,th_min,th_max,    "#theta_{1}", "#theta_{1}", "B_{2}^{-+}");
  h_jet_b2mp_jtjt   = createHistogram(createName(_histogramBaseName,"jet_b2mp_jtjt"),  jt_nbins,jt_min,jt_max,    jt_nbins,jt_min,jt_max,    "j_{T,1}",    "j_{T,2}",    "B_{2}^{-+}");
  h_jet_b2mp_zz     = createHistogram(createName(_histogramBaseName,"jet_b2mp_zz"),    z_nbins,z_min,z_max,       z_nbins,z_min,z_max,       "z_{1}",      "z_{2}",      "B_{2}^{-+}");

  h_jet_b2s_ptpt    = createHistogram(createName(_histogramBaseName,"jet_b2s_ptpt "),  pt_nbins,pt_min,pt_max,    pt_nbins,pt_min,pt_max,    "p_{T,1}",    "p_{T,2}",    "B_{2}^{s}");
  h_jet_b2s_phiphi  = createHistogram(createName(_histogramBaseName,"jet_b2s_phiphi"), phi_nbins,phi_min,phi_max, phi_nbins,phi_min,phi_max, "#varphi_{1}","#varphi_{1}","B_{2}^{s}");
  h_jet_b2s_etaeta  = createHistogram(createName(_histogramBaseName,"jet_b2s_etaeta"), eta_nbins,eta_min,eta_max, eta_nbins,eta_min,eta_max, "#eta_{1}",   "#eta_{2}",   "B_{2}^{s}");
  h_jet_b2s_thth    = createHistogram(createName(_histogramBaseName,"jet_b2s_thth"),   th_nbins,th_min,th_max,    th_nbins,th_min,th_max,    "#theta_{1}", "#theta_{1}", "B_{2}^{s}");
  h_jet_b2s_jtjt    = createHistogram(createName(_histogramBaseName,"jet_b2s_jtjt"),   jt_nbins,jt_min,jt_max,    jt_nbins,jt_min,jt_max,    "j_{T,1}",    "j_{T,2}",    "B_{2}^{s}");
  h_jet_b2s_zz      = createHistogram(createName(_histogramBaseName,"jet_b2s_zz"),     z_nbins,z_min,z_max,       z_nbins,z_min,z_max,       "z_{1}",      "z_{2}",      "B_{2}^{s}");

}

void JetPairHistosDerived::loadFrom(TFile & inputFile)
{
  if (reportDebug(__FUNCTION__)) { printCR();}
  JetPairHistos::loadFrom(inputFile);
  h_jet_a2pp_ptpt   =  loadH2(inputFile,createName(_histogramBaseName,"jet_a2pp_ptpt"));
  h_jet_a2pp_phiphi =  loadH2(inputFile,createName(_histogramBaseName,"jet_a2pp_phiphi"));
  h_jet_a2pp_etaeta =  loadH2(inputFile,createName(_histogramBaseName,"jet_a2pp_etaet"));
  h_jet_a2pp_thth   =  loadH2(inputFile,createName(_histogramBaseName,"jet_a2pp_thth"));
  h_jet_a2pp_jtjt   =  loadH2(inputFile,createName(_histogramBaseName,"jet_a2pp_jtjt"));
  h_jet_a2pp_zz     =  loadH2(inputFile,createName(_histogramBaseName,"jet_a2pp_zz"));

  h_jet_a2mp_ptpt   =  loadH2(inputFile,createName(_histogramBaseName,"jet_a2mp_ptpt"));
  h_jet_a2mp_phiphi =  loadH2(inputFile,createName(_histogramBaseName,"jet_a2mp_phiphi"));
  h_jet_a2mp_etaeta =  loadH2(inputFile,createName(_histogramBaseName,"jet_a2mp_etaeta"));
  h_jet_a2mp_thth   =  loadH2(inputFile,createName(_histogramBaseName,"jet_a2mp_thth"));
  h_jet_a2mp_jtjt   =  loadH2(inputFile,createName(_histogramBaseName,"jet_a2mp_jtjt"));
  h_jet_a2mp_zz     =  loadH2(inputFile,createName(_histogramBaseName,"jet_a2mp_zz"));

  h_jet_a2pm_ptpt   =  loadH2(inputFile,createName(_histogramBaseName,"jet_a2pm_ptpt"));
  h_jet_a2pm_phiphi =  loadH2(inputFile,createName(_histogramBaseName,"jet_a2pm_phiphi"));
  h_jet_a2pm_etaeta =  loadH2(inputFile,createName(_histogramBaseName,"jet_a2pm_etaeta"));
  h_jet_a2pm_thth   =  loadH2(inputFile,createName(_histogramBaseName,"jet_a2pm_thth"));
  h_jet_a2pm_jtjt   =  loadH2(inputFile,createName(_histogramBaseName,"jet_a2pm_jtjt"));
  h_jet_a2pm_zz     =  loadH2(inputFile,createName(_histogramBaseName,"jet_a2pm_zz"));

  h_jet_a2mm_ptpt   =  loadH2(inputFile,createName(_histogramBaseName,"jet_a2mm_ptpt"));
  h_jet_a2mm_phiphi =  loadH2(inputFile,createName(_histogramBaseName,"jet_a2mm_phiphi"));
  h_jet_a2mm_etaeta =  loadH2(inputFile,createName(_histogramBaseName,"jet_a2mm_etaeta"));
  h_jet_a2mm_thth   =  loadH2(inputFile,createName(_histogramBaseName,"jet_a2mm_thth"));
  h_jet_a2mm_jtjt   =  loadH2(inputFile,createName(_histogramBaseName,"jet_a2mm_jtjt"));
  h_jet_a2mm_zz     =  loadH2(inputFile,createName(_histogramBaseName,"jet_a2mm_zz"));

  h_jet_b2pm_ptpt   =  loadH2(inputFile,createName(_histogramBaseName,"jet_b2pm_ptpt"));
  h_jet_b2pm_phiphi =  loadH2(inputFile,createName(_histogramBaseName,"jet_b2pm_phiphi"));
  h_jet_b2pm_etaeta =  loadH2(inputFile,createName(_histogramBaseName,"jet_b2pm_etaeta"));
  h_jet_b2pm_thth   =  loadH2(inputFile,createName(_histogramBaseName,"jet_b2pm_thth"));
  h_jet_b2pm_jtjt   =  loadH2(inputFile,createName(_histogramBaseName,"jet_b2pm_jtjt"));
  h_jet_b2pm_zz     =  loadH2(inputFile,createName(_histogramBaseName,"jet_b2pm_zz"));

  h_jet_b2mp_ptpt   =  loadH2(inputFile,createName(_histogramBaseName,"jet_b2mp_ptpt"));
  h_jet_b2mp_phiphi =  loadH2(inputFile,createName(_histogramBaseName,"jet_b2mp_phiphi"));
  h_jet_b2mp_etaeta =  loadH2(inputFile,createName(_histogramBaseName,"jet_b2mp_etaeta"));
  h_jet_b2mp_thth   =  loadH2(inputFile,createName(_histogramBaseName,"jet_b2mp_thth"));
  h_jet_b2mp_jtjt   =  loadH2(inputFile,createName(_histogramBaseName,"jet_b2mp_jtjt"));
  h_jet_b2mp_zz     =  loadH2(inputFile,createName(_histogramBaseName,"jet_b2mp_zz"));

  h_jet_b2s_ptpt    =  loadH2(inputFile,createName(_histogramBaseName,"jet_b2s_ptpt "));
  h_jet_b2s_phiphi  =  loadH2(inputFile,createName(_histogramBaseName,"jet_b2s_phiphi"));
  h_jet_b2s_etaeta  =  loadH2(inputFile,createName(_histogramBaseName,"jet_b2s_etaeta"));
  h_jet_b2s_thth    =  loadH2(inputFile,createName(_histogramBaseName,"jet_b2s_thth"));
  h_jet_b2s_jtjt    =  loadH2(inputFile,createName(_histogramBaseName,"jet_b2s_jtjt"));
  h_jet_b2s_zz      =  loadH2(inputFile,createName(_histogramBaseName,"jet_b2s_zz"));

}

void JetPairHistosDerived::calculateDerivedHistograms(const JetSingleHistos & singleHistos, const JetPairHistos & pairHistos)
{
  double pIntegral = singleHistos.h_jet_n1p_phi->Integral();
  double mIntegral = singleHistos.h_jet_n1m_phi->Integral();
  double pScale = (pIntegral>0) ? 1.0/pIntegral : -1.0;  // -1 indicate error or no statistics
  double mScale = (mIntegral>0) ? 1.0/mIntegral : -1.0;  // -1 indicate error or no statistics

  printString("jet_a2pp");
  h_jet_a2pp_ptpt  ->Add(pairHistos.h_jet_n2pp_ptpt);   h_jet_a2pp_ptpt->Scale(pScale);
  h_jet_a2pp_phiphi->Add(pairHistos.h_jet_n2pp_phiphi); h_jet_a2pp_phiphi->Scale(pScale);
  h_jet_a2pp_etaeta->Add(pairHistos.h_jet_n2pp_etaeta); h_jet_a2pp_etaeta->Scale(pScale);
  h_jet_a2pp_thth  ->Add(pairHistos.h_jet_n2pp_thth);   h_jet_a2pp_thth->Scale(pScale);
  h_jet_a2pp_jtjt  ->Add(pairHistos.h_jet_n2pp_jtjt);   h_jet_a2pp_jtjt->Scale(pScale);
  h_jet_a2pp_zz    ->Add(pairHistos.h_jet_n2pp_zz);     h_jet_a2pp_zz->Scale(pScale);

  printString("jet_a2mp");
  h_jet_a2mp_ptpt  ->Add(pairHistos.h_jet_n2pm_ptpt);   h_jet_a2mp_ptpt->Scale(pScale);
  h_jet_a2mp_phiphi->Add(pairHistos.h_jet_n2pm_phiphi); h_jet_a2mp_phiphi->Scale(pScale);
  h_jet_a2mp_etaeta->Add(pairHistos.h_jet_n2pm_etaeta); h_jet_a2mp_etaeta->Scale(pScale);
  h_jet_a2mp_thth  ->Add(pairHistos.h_jet_n2pm_thth);   h_jet_a2mp_thth->Scale(pScale);
  h_jet_a2mp_jtjt  ->Add(pairHistos.h_jet_n2pm_jtjt);   h_jet_a2mp_jtjt->Scale(pScale);
  h_jet_a2mp_zz    ->Add(pairHistos.h_jet_n2pm_zz);     h_jet_a2mp_zz->Scale(pScale);

  printString("jet_a2pm");
  h_jet_a2pm_ptpt  ->Add(pairHistos.h_jet_n2pm_ptpt);   h_jet_a2pm_ptpt->Scale(mScale);
  h_jet_a2pm_phiphi->Add(pairHistos.h_jet_n2pm_phiphi); h_jet_a2pm_phiphi->Scale(mScale);
  h_jet_a2pm_etaeta->Add(pairHistos.h_jet_n2pm_etaeta); h_jet_a2pm_etaeta->Scale(mScale);
  h_jet_a2pm_thth  ->Add(pairHistos.h_jet_n2pm_thth);   h_jet_a2pm_thth->Scale(mScale);
  h_jet_a2pm_jtjt  ->Add(pairHistos.h_jet_n2pm_jtjt);   h_jet_a2pm_jtjt->Scale(mScale);
  h_jet_a2pm_zz    ->Add(pairHistos.h_jet_n2pm_zz);     h_jet_a2pm_zz->Scale(mScale);

  printString("jet_a2mm");
  h_jet_a2mm_ptpt  ->Add(pairHistos.h_jet_n2mm_ptpt);   h_jet_a2mm_ptpt->Scale(mScale);
  h_jet_a2mm_phiphi->Add(pairHistos.h_jet_n2mm_phiphi); h_jet_a2mm_phiphi->Scale(mScale);
  h_jet_a2mm_etaeta->Add(pairHistos.h_jet_n2mm_etaeta); h_jet_a2mm_etaeta->Scale(mScale);
  h_jet_a2mm_thth  ->Add(pairHistos.h_jet_n2mm_thth);   h_jet_a2mm_thth->Scale(mScale);
  h_jet_a2mm_jtjt  ->Add(pairHistos.h_jet_n2mm_jtjt);   h_jet_a2mm_jtjt->Scale(mScale);
  h_jet_a2mm_zz    ->Add(pairHistos.h_jet_n2mm_zz);     h_jet_a2mm_zz->Scale(mScale);

  printString("jet_b2pm");
  h_jet_b2pm_ptpt   ->Add(h_jet_a2pm_ptpt,   h_jet_a2mm_ptpt,    1.0, -1.0);
  h_jet_b2pm_phiphi ->Add(h_jet_a2pm_phiphi, h_jet_a2mm_phiphi,  1.0, -1.0);
  h_jet_b2pm_etaeta ->Add(h_jet_a2pm_etaeta, h_jet_a2mm_etaeta,  1.0, -1.0);
  h_jet_b2pm_thth   ->Add(h_jet_a2pm_thth,   h_jet_a2mm_thth,    1.0, -1.0);
  h_jet_b2pm_jtjt   ->Add(h_jet_a2pm_jtjt,   h_jet_a2mm_jtjt,    1.0, -1.0);
  h_jet_b2pm_zz     ->Add(h_jet_a2pm_zz,     h_jet_a2mm_zz,      1.0, -1.0);

  printString("jet_b2mp");
  h_jet_b2mp_ptpt   ->Add(h_jet_a2mp_ptpt,   h_jet_a2pp_ptpt,    1.0, -1.0);
  h_jet_b2mp_phiphi ->Add(h_jet_a2mp_phiphi, h_jet_a2pp_phiphi,  1.0, -1.0);
  h_jet_b2mp_etaeta ->Add(h_jet_a2mp_etaeta, h_jet_a2pp_etaeta,  1.0, -1.0);
  h_jet_b2mp_thth   ->Add(h_jet_a2mp_thth,   h_jet_a2pp_thth,    1.0, -1.0);
  h_jet_b2mp_jtjt   ->Add(h_jet_a2mp_jtjt,   h_jet_a2pp_jtjt,    1.0, -1.0);
  h_jet_b2mp_zz     ->Add(h_jet_a2mp_zz,     h_jet_a2pp_zz,      1.0, -1.0);

  printString("jet_b2s");
  h_jet_b2s_ptpt   ->Add(h_jet_b2mp_ptpt,   h_jet_b2pm_ptpt,    0.5, 0.5);
  h_jet_b2s_phiphi ->Add(h_jet_b2mp_phiphi, h_jet_b2pm_phiphi,  0.5, 0.5);
  h_jet_b2s_etaeta ->Add(h_jet_b2mp_etaeta, h_jet_b2pm_etaeta,  0.5, 0.5);
  h_jet_b2s_thth   ->Add(h_jet_b2mp_thth,   h_jet_b2pm_thth,    0.5, 0.5);
  h_jet_b2s_jtjt   ->Add(h_jet_b2mp_jtjt,   h_jet_b2pm_jtjt,    0.5, 0.5);
  h_jet_b2s_zz     ->Add(h_jet_b2mp_zz,     h_jet_b2pm_zz,      0.5, 0.5);
}


} // namespace CAP
