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
#include "JetPairHistos.hpp"
#include "NameHelpers.hpp"
#include "JetHelpers.hpp"
#include "PrintHelpers.hpp"
#include "RootHistogramHelpers.hpp"
#include "Configuration.hpp"
#include "NameHelpers.hpp"
#include "Task.hpp"

ClassImp(CAP::JetPairHistos);

namespace CAP
{

JetPairHistos::JetPairHistos()
:
HistogramGroup(),
particleTypeList(nullptr),
h_jet_n2_ptpt(nullptr),
h_jet_n2_phiphi(nullptr),
h_jet_n2_etaeta(nullptr),
h_jet_n2_thth(nullptr),
h_jet_n2_jtjt(nullptr),
h_jet_n2_zz(nullptr),
h_jet_n2pm_ptpt(nullptr),
h_jet_n2pm_phiphi(nullptr),
h_jet_n2pm_etaeta(nullptr),
h_jet_n2pm_thth(nullptr),
h_jet_n2pm_jtjt(nullptr),
h_jet_n2pm_zz(nullptr),
h_jet_n2mm_ptpt(nullptr),
h_jet_n2mm_phiphi(nullptr),
h_jet_n2mm_etaeta(nullptr),
h_jet_n2mm_thth(nullptr),
h_jet_n2mm_jtjt(nullptr),
h_jet_n2mm_zz(nullptr),
pt_nbins(0),
phi_nbins(0),
eta_nbins(0),
jt_nbins(0),
th_nbins(0),
z_nbins(0),
pt_min(0),
pt_max(0),
phi_min(0),
phi_max(0),
eta_min(0),
eta_max(0),
jt_min(0),
jt_max(0),
th_min(0),
th_max(0),
z_min(0),
z_max(0)
{ }

JetPairHistos::JetPairHistos(const JetPairHistos & source)
:
HistogramGroup(source),
particleTypeList(nullptr),
h_jet_n2_ptpt(nullptr),
h_jet_n2_phiphi(nullptr),
h_jet_n2_etaeta(nullptr),
h_jet_n2_thth(nullptr),
h_jet_n2_jtjt(nullptr),
h_jet_n2_zz(nullptr),
h_jet_n2pm_ptpt(nullptr),
h_jet_n2pm_phiphi(nullptr),
h_jet_n2pm_etaeta(nullptr),
h_jet_n2pm_thth(nullptr),
h_jet_n2pm_jtjt(nullptr),
h_jet_n2pm_zz(nullptr),
h_jet_n2mm_ptpt(nullptr),
h_jet_n2mm_phiphi(nullptr),
h_jet_n2mm_etaeta(nullptr),
h_jet_n2mm_thth(nullptr),
h_jet_n2mm_jtjt(nullptr),
pt_nbins(source.pt_nbins),
phi_nbins(source.phi_nbins),
eta_nbins(source.eta_nbins),
jt_nbins(source.jt_nbins),
th_nbins(source.th_nbins),
z_nbins(source.z_nbins),
pt_min(source.pt_min),
pt_max(source.t_max),
phi_min(source.phi_min),
phi_max(source.phi_max),
eta_min(source.eta_min),
eta_max(source.eta_max),
jt_min(source.jt_min),
jt_max(source.jt_max),
th_min(source.th_min),
th_max(source.th_max),
z_min(source.z_min),
z_max(source.z_max)
{
  cloneB(rhs);
}

JetPairHistos & JetPairHistos::operator=(const JetPairHistos & rhs)
{
  if (this!=&rhs)
    {
    HistogramGroup::operator=(rhs);
    cloneB(rhs);
    pt_nbins   = rhs.pt_nbins;
    phi_nbins  = rhs.phi_nbins;
    eta_nbins  = rhs.eta_nbins;
    jt_nbins   = rhs.jt_nbins;
    th_nbins   = rhs.th_nbins;
    z_nbins    = rhs.z_nbins;
    pt_min     = rhs.pt_min;
    pt_max     = rhs.t_max;
    phi_min    = rhs.phi_min;
    phi_max    = rhs.phi_max;
    eta_min    = rhs.eta_min;
    eta_max    = rhs.eta_max;
    jt_min     = rhs.jt_min;
    jt_max     = rhs.jt_max;
    th_min     = rhs.th_min;
    th_max     = rhs.th_max;
    z_min      = rhs.z_min;
    z_max      = rhs.z_max;
    }
  return *this;
}

void JetPairHistos::cloneB(const JetPairHistos & source)
{
  h_jet_n2_ptpt   = safeCloneH2(source.h_jet_n2_ptpt);
  h_jet_n2_phiphi = safeCloneH2(source.h_jet_n2_phiphi);
  h_jet_n2_etaeta = safeCloneH2(source.h_jet_n2_etaeta);
  h_jet_n2_thth   = safeCloneH2(source.h_jet_n2_thth);
  h_jet_n2_jtjt   = safeCloneH2(source.h_jet_n2_jtjt);
  h_jet_n2_zz     = safeCloneH2(source.h_jet_n2_zz);

  h_jet_n2pp_ptpt   = safeCloneH2(source.h_jet_n2pp_ptpt);
  h_jet_n2pp_phiphi = safeCloneH2(source.h_jet_n2pp_phiphi);
  h_jet_n2pp_etaeta = safeCloneH2(source.h_jet_n2pp_etaeta);
  h_jet_n2pp_thth   = safeCloneH2(source.h_jet_n2pp_thth);
  h_jet_n2pp_jtjt   = safeCloneH2(source.h_jet_n2pp_jtjt);
  h_jet_n2pp_zz     = safeCloneH2(source.h_jet_n2pp_zz);

  h_jet_n2pm_ptpt   = safeCloneH2(source.h_jet_n2pm_ptpt);
  h_jet_n2pm_phiphi = safeCloneH2(source.h_jet_n2pm_phiphi);
  h_jet_n2pm_etaeta = safeCloneH2(source.h_jet_n2pm_etaeta);
  h_jet_n2pm_thth   = safeCloneH2(source.h_jet_n2pm_thth);
  h_jet_n2pm_jtjt   = safeCloneH2(source.h_jet_n2pm_jtjt);
  h_jet_n2pm_zz     = safeCloneH2(source.h_jet_n2pm_zz);

  h_jet_n2mm_ptpt   = safeCloneH2(source.h_jet_n2mm_ptpt);
  h_jet_n2mm_phiphi = safeCloneH2(source.h_jet_n2mm_phiphi);
  h_jet_n2mm_etaeta = safeCloneH2(source.h_jet_n2mm_etaeta);
  h_jet_n2mm_thth   = safeCloneH2(source.h_jet_n2mm_thth);
  h_jet_n2mm_jtjt   = safeCloneH2(source.h_jet_n2mm_jtjt);
  h_jet_n2mm_zz     = safeCloneH2(source.h_jet_n2mm_zz);
}


void JetPairHistos::configure(const String & taskName,
                                    const String & objectType,
                                    const Configuration & configuration,
                                    unsigned int index)
{
  if (reportDebug(__FUNCTION__)) { printCR();}
  HistogramGroup::configure(taskName,objectType,configuration,index);
  pt_nbins   = configuration.valueInt(createKey(taskName,objectType,index,    "pt_nbins"));
  phi_nbins  = configuration.valueInt(createKey(taskName,objectType,index,    "phi_nbins"));
  eta_nbins  = configuration.valueInt(createKey(taskName,objectType,index,    "eta_nbins"));
  jt_nbins   = configuration.valueInt(createKey(taskName,objectType,index,    "jt_nbins"));
  th_nbins   = configuration.valueInt(createKey(taskName,objectType,index,    "th_nbins"));
  z_nbins    = configuration.valueInt(createKey(taskName,objectType,index,    "z_nbins"));
  pt_min     = configuration.valueDouble(createKey(taskName,objectType,index, "pt_min"));
  pt_max     = configuration.valueDouble(createKey(taskName,objectType,index, "pt_max"));
  phi_min    = configuration.valueDouble(createKey(taskName,objectType,index, "phi_min"));
  phi_max    = configuration.valueDouble(createKey(taskName,objectType,index, "phi_max"));
  eta_min    = configuration.valueDouble(createKey(taskName,objectType,index, "eta_min"));
  eta_max    = configuration.valueDouble(createKey(taskName,objectType,index, "eta_max"));
  jt_min     = configuration.valueDouble(createKey(taskName,objectType,index, "jt_min"));
  jt_max     = configuration.valueDouble(createKey(taskName,objectType,index, "jt_max"));
  th_min     = configuration.valueDouble(createKey(taskName,objectType,index, "th_min"));
  th_max     = configuration.valueDouble(createKey(taskName,objectType,index, "th_max"));
  z_min      = configuration.valueDouble(createKey(taskName,objectType,index, "z_min"));
  z_max      = configuration.valueDouble(createKey(taskName,objectType,index, "z_max"));

  if (reportDebug(__FUNCTION__))
    {
    printCR();
    printLine();
    printValue("Histo Base Name",    bn);
    printValue("pt_nbins",    pt_nbins);
    printValue("pt_min",      pt_min);
    printValue("pt_max",      pt_max);
    printValue("phi_nbins",   phi_nbins);
    printValue("phi_min",     phi_min);
    printValue("phi_max",     phi_max);
    printValue("eta_nbins",   eta_nbins);
    printValue("eta_min",     eta_min);
    printValue("eta_max",     eta_max);
    printValue("jt_nbins",    jt_nbins);
    printValue("jt_min",      jt_min);
    printValue("jt_max",      jt_max);
    printValue("th_nbins",    th_nbins);
    printValue("th_min",      th_min);
    printValue("th_max",      th_max);
    printCR();
    }
}

void JetPairHistos::create()
{
  if (reportDebug(__FUNCTION__)) { printCR();}
  h_jet_n2_ptpt    = createHistogram(createName(_histogramBaseName,"jet_n2_ptpt"),pt_nbins,pt_min,pt_max, pt_nbins,pt_min,pt_max,"p_{T,1}","p_{T,2}","N_{2}");
  h_jet_n2_phiphi  = createHistogram(createName(_histogramBaseName,"jet_n2_phiphi"),phi_nbins,phi_min,phi_max, phi_nbins,phi_min,phi_max,"#varphi_{1}","#varphi_{2}","N_{2}");
  h_jet_n2_etaeta  = createHistogram(createName(_histogramBaseName,"jet_n2_etaeta"),eta_nbins,eta_min,eta_max, eta_nbins,eta_min,eta_max,"#eta_{1}","#eta_{2}","N_{2}");
  h_jet_n2_thth    = createHistogram(createName(_histogramBaseName,"jet_n2_thth"),th_nbins,th_min,th_max, th_nbins,th_min,th_max,"#theta_{1}","#theta_{2}","N_{2}");
  h_jet_n2_jtjt    = createHistogram(createName(_histogramBaseName,"jet_n2_jtjt"),jt_nbins,jt_min,jt_max, jt_nbins,jt_min,jt_max,"j_{T,1}","j_{T,2}","N_{2}");
  h_jet_n2_zz      = createHistogram(createName(_histogramBaseName,"jet_n2_zz"),z_nbins,z_min,z_max, z_nbins,z_min,z_max,"z_{1}","z_{2}","N_{2}");

  h_jet_n2pp_ptpt    = createHistogram(createName(_histogramBaseName,"jet_n2pp_ptpt"),pt_nbins,pt_min,pt_max, pt_nbins,pt_min,pt_max,"p_{T,1}","p_{T,2}","N_{2}^{++}");
  h_jet_n2pp_phiphi  = createHistogram(createName(_histogramBaseName,"jet_n2pp_phiphi"),phi_nbins,phi_min,phi_max, phi_nbins,phi_min,phi_max,"#varphi_{1}","#varphi_{2}","N_{2}^{++}");
  h_jet_n2pp_etaeta  = createHistogram(createName(_histogramBaseName,"jet_n2pp_etaeta"),eta_nbins,eta_min,eta_max, eta_nbins,eta_min,eta_max,"#eta_{1}","#eta_{2}","N_{2}^{++}");
  h_jet_n2pp_thth    = createHistogram(createName(_histogramBaseName,"jet_n2pp_thth"),th_nbins,th_min,th_max, th_nbins,th_min,th_max,"#theta_{1}","#theta_{2}","N_{2}^{++}");
  h_jet_n2pp_jtjt    = createHistogram(createName(_histogramBaseName,"jet_n2pp_jtjt"),jt_nbins,jt_min,jt_max, jt_nbins,jt_min,jt_max,"j_{T,1}","j_{T,2}","N_{2}^{++}");
  h_jet_n2pp_zz      = createHistogram(createName(_histogramBaseName,"jet_n2pp_zz"),z_nbins,z_min,z_max, z_nbins,z_min,z_max,"z_{1}","z_{2}","N_{2}^{++}");

  h_jet_n2pm_ptpt    = createHistogram(createName(_histogramBaseName,"jet_n2pm_ptpt"),pt_nbins,pt_min,pt_max, pt_nbins,pt_min,pt_max,"p_{T,1}","p_{T,2}","N_{2}^{+-}");
  h_jet_n2pm_phiphi  = createHistogram(createName(_histogramBaseName,"jet_n2pm_phiphi"),phi_nbins,phi_min,phi_max, phi_nbins,phi_min,phi_max,"#varphi_{1}","#varphi_{2}","N_{2}^{+-}");
  h_jet_n2pm_etaeta  = createHistogram(createName(_histogramBaseName,"jet_n2pm_etaeta"),eta_nbins,eta_min,eta_max, eta_nbins,eta_min,eta_max,"#eta_{1}","#eta_{2}","N_{2}^{+-}");
  h_jet_n2pm_thth    = createHistogram(createName(_histogramBaseName,"jet_n2pm_thth"),th_nbins,th_min,th_max, th_nbins,th_min,th_max,"#theta_{1}","#theta_{2}","N_{2}^{+-}");
  h_jet_n2pm_jtjt    = createHistogram(createName(_histogramBaseName,"jet_n2pm_jtjt"),jt_nbins,jt_min,jt_max, jt_nbins,jt_min,jt_max,"j_{T,1}","j_{T,2}","N_{2}^{+-}");
  h_jet_n2pm_zz      = createHistogram(createName(_histogramBaseName,"jet_n2pm_zz"),z_nbins,z_min,z_max, z_nbins,z_min,z_max,"z_{1}","z_{2}","N_{2}^{+-}");

  h_jet_n2mm_ptpt    = createHistogram(createName(_histogramBaseName,"jet_n2mm_ptpt"),pt_nbins,pt_min,pt_max, pt_nbins,pt_min,pt_max,"p_{T,1}","p_{T,2}","N_{2}^{--}");
  h_jet_n2mm_phiphi  = createHistogram(createName(_histogramBaseName,"jet_n2mm_phiphi"),phi_nbins,phi_min,phi_max, phi_nbins,phi_min,phi_max,"#varphi_{1}","#varphi_{2}","N_{2}^{--}");
  h_jet_n2mm_etaeta  = createHistogram(createName(_histogramBaseName,"jet_n2mm_etaeta"),eta_nbins,eta_min,eta_max, eta_nbins,eta_min,eta_max,"#eta_{1}","#eta_{2}","N_{2}^{--}");
  h_jet_n2mm_thth    = createHistogram(createName(_histogramBaseName,"jet_n2mm_thth"),th_nbins,th_min,th_max, th_nbins,th_min,th_max,"#theta_{1}","#theta_{2}","N_{2}^{--}");
  h_jet_n2mm_jtjt    = createHistogram(createName(_histogramBaseName,"jet_n2mm_jtjt"),jt_nbins,jt_min,jt_max, jt_nbins,jt_min,jt_max,"j_{T,1}","j_{T,2}","N_{2}^{--}");
  h_jet_n2mm_zz      = createHistogram(createName(_histogramBaseName,"jet_n2mm_zz"),z_nbins,z_min,z_max, z_nbins,z_min,z_max,"z_{1}","z_{2}","N_{2}^{--}");
}

void JetPairHistos::loadFrom(TFile & inputFile)
{
  if (reportDebug(__FUNCTION__)) { printCR();}
  HistogramGroup::loadFrom(inputFile);
  h_jet_n2_ptpt      = loadH2(inputFile,createName(_histogramBaseName,"jet_n2_ptpt"));
  h_jet_n2_phiphi    = loadH2(inputFile,createName(_histogramBaseName,"jet_n2_phiphi"));
  h_jet_n2_etaeta    = loadH2(inputFile,createName(_histogramBaseName,"jet_n2_etaeta"));
  h_jet_n2_thth      = loadH2(inputFile,createName(_histogramBaseName,"jet_n2_thth"));
  h_jet_n2_jtjt      = loadH2(inputFile,createName(_histogramBaseName,"jet_n2_jtjt"));
  h_jet_n2_zz        = loadH2(inputFile,createName(_histogramBaseName,"jet_n2_zz"));

  h_jet_n2pp_ptpt    = loadH2(inputFile,createName(_histogramBaseName,"jet_n2pp_ptpt"));
  h_jet_n2pp_phiphi  = loadH2(inputFile,createName(_histogramBaseName,"jet_n2pp_phiphi"));
  h_jet_n2pp_etaeta  = loadH2(inputFile,createName(_histogramBaseName,"jet_n2pp_etaeta"));
  h_jet_n2pp_thth    = loadH2(inputFile,createName(_histogramBaseName,"jet_n2pp_thth"));
  h_jet_n2pp_jtjt    = loadH2(inputFile,createName(_histogramBaseName,"jet_n2pp_jtjt"));
  h_jet_n2pp_zz      = loadH2(inputFile,createName(_histogramBaseName,"jet_n2pp_zz"));

  h_jet_n2pm_ptpt    = loadH2(inputFile,createName(_histogramBaseName,"jet_n2pm_ptpt"));
  h_jet_n2pm_phiphi  = loadH2(inputFile,createName(_histogramBaseName,"jet_n2pm_phiphi"));
  h_jet_n2pm_etaeta  = loadH2(inputFile,createName(_histogramBaseName,"jet_n2pm_etaeta"));
  h_jet_n2pm_thth    = loadH2(inputFile,createName(_histogramBaseName,"jet_n2pm_thth"));
  h_jet_n2pm_jtjt    = loadH2(inputFile,createName(_histogramBaseName,"jet_n2pm_jtjt"));
  h_jet_n2pm_zz      = loadH2(inputFile,createName(_histogramBaseName,"jet_n2pm_zz"));

  h_jet_n2mm_ptpt    = loadH2(inputFile,createName(_histogramBaseName,"jet_n2mm_ptpt"));
  h_jet_n2mm_phiphi  = loadH2(inputFile,createName(_histogramBaseName,"jet_n2mm_phiphi"));
  h_jet_n2mm_etaeta  = loadH2(inputFile,createName(_histogramBaseName,"jet_n2mm_etaeta"));
  h_jet_n2mm_thth    = loadH2(inputFile,createName(_histogramBaseName,"jet_n2mm_thth"));
  h_jet_n2mm_jtjt    = loadH2(inputFile,createName(_histogramBaseName,"jet_n2mm_jtjt"));
  h_jet_n2mm_zz      = loadH2(inputFile,createName(_histogramBaseName,"jet_n2mm_zz"));
}

void JetPairHistos::fill(PseudoJet& jet)
{
  //double jet_phi = jet.phi();
  double jet_pt  = jet.perp();
  //double jet_eta = jet.pseudorapidity();
  int pdgId1, pdgId2;
  double q1, q2;

  // Constituents of the passed Jet
  const std::vector<PseudoJet> & constituents = jet.constituents();
  for (const auto & part1 : constituents)
    {
    pdgId1 = part1.user_index();
    ParticleType * type1 = particleTypeList->findPdgCode(pdgId1);
    q1 = type1->charge();

    double part1_phi = part1.phi();
    double part1_pt  = part1.perp();
    double part1_eta = part1.pseudorapidity();
    double part1_z   = part1_pt/jet_pt;
    double part1_jt  = 0;
    double part1_th  = 0;
    //calculateJtTheta(jet_px,jet_py,jet_pz,jet_p,part1_px,part1_py,part1_pz,part1_p,part1_jt,part1_th);
    for (const auto & part2 : constituents)
      {
      if (part1==part2) continue;

      pdgId2 = part2.user_index();
      ParticleType * type2 = particleTypeList->findPdgCode(pdgId2);
      q2 = type2->charge();

      double part2_phi = part2.phi();
      double part2_pt  = part2.perp();
      double part2_eta = part2.pseudorapidity();
      double part2_z   = part2_pt/jet_pt;
      double part2_jt  = 0;
      double part2_th  = 0;
      //calculateJtTheta(jet_px,jet_py,jet_pz,jet_p,part1_px,part2_py,part2_pz,part2_p,part2_jt,part2_th);

      h_jet_n2_ptpt->Fill(part1_pt,part2_pt);
      h_jet_n2_phiphi->Fill(part1_phi,part2_phi);
      h_jet_n2_etaeta->Fill(part1_eta,part2_eta);
      h_jet_n2_thth->Fill(part1_th,part2_th);
      h_jet_n2_jtjt->Fill(part1_jt,part2_jt);
      h_jet_n2_zz->Fill(part1_z,part2_z);

      if (q1>0  &&  q2>0)
        {
        h_jet_n2pp_ptpt->Fill(part1_pt,part2_pt);
        h_jet_n2pp_phiphi->Fill(part1_phi,part2_phi);
        h_jet_n2pp_etaeta->Fill(part1_eta,part2_eta);
        h_jet_n2pp_thth->Fill(part1_th,part2_th);
        h_jet_n2pp_jtjt->Fill(part1_jt,part2_jt);
        h_jet_n2pp_zz->Fill(part1_z,part2_z);
        }
      else if (q1>0  &&  q2<0)
        {
        h_jet_n2pm_ptpt->Fill(part1_pt,part2_pt);
        h_jet_n2pm_phiphi->Fill(part1_phi,part2_phi);
        h_jet_n2pm_etaeta->Fill(part1_eta,part2_eta);
        h_jet_n2pm_thth->Fill(part1_th,part2_th);
        h_jet_n2pm_jtjt->Fill(part1_jt,part2_jt);
        h_jet_n2pm_zz->Fill(part1_z,part2_z);
        }
      else if (q1<0  &&  q2>0)
        {
        h_jet_n2pm_ptpt->Fill(part2_pt,part1_pt);
        h_jet_n2pm_phiphi->Fill(part2_phi,part1_phi);
        h_jet_n2pm_etaeta->Fill(part2_eta,part1_eta);
        h_jet_n2pm_thth->Fill(part2_th,part1_th);
        h_jet_n2pm_jtjt->Fill(part2_jt,part1_jt);
        h_jet_n2pm_zz->Fill(part2_z,part1_z);
        }
      else if (q1<0  &&  q2<0)
        {
        h_jet_n2mm_ptpt->Fill(part1_pt,part2_pt);
        h_jet_n2mm_phiphi->Fill(part1_phi,part2_phi);
        h_jet_n2mm_etaeta->Fill(part1_eta,part2_eta);
        h_jet_n2mm_thth->Fill(part1_th,part2_th);
        h_jet_n2mm_jtjt->Fill(part1_jt,part2_jt);
        h_jet_n2mm_zz->Fill(part1_z,part2_z);
        }
      }
    }
}

void JetPairHistos::scaleHistograms(double scale)
{
  h_jet_n2_ptpt->Scale(scale);
  h_jet_n2_phiphi->Scale(scale);
  h_jet_n2_etaeta->Scale(scale);
  h_jet_n2_thth->Scale(scale);
  h_jet_n2_jtjt->Scale(scale);
  h_jet_n2_zz->Scale(scale);

  h_jet_n2pp_ptpt->Scale(scale);
  h_jet_n2pp_phiphi->Scale(scale);
  h_jet_n2pp_etaeta->Scale(scale);
  h_jet_n2pp_thth->Scale(scale);
  h_jet_n2pp_jtjt->Scale(scale);
  h_jet_n2pp_zz->Scale(scale);

  h_jet_n2pm_ptpt->Scale(scale);
  h_jet_n2pm_phiphi->Scale(scale);
  h_jet_n2pm_etaeta->Scale(scale);
  h_jet_n2pm_thth->Scale(scale);
  h_jet_n2pm_jtjt->Scale(scale);
  h_jet_n2pm_zz->Scale(scale);

  h_jet_n2mm_ptpt->Scale(scale);
  h_jet_n2mm_phiphi->Scale(scale);
  h_jet_n2mm_etaeta->Scale(scale);
  h_jet_n2mm_thth->Scale(scale);
  h_jet_n2mm_jtjt->Scale(scale);
  h_jet_n2mm_zz->Scale(scale);

}

} // namespace CAP
