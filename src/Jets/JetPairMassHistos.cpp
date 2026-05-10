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
#include "JetPairMassHistos.hpp"
#include "JetHelpers.hpp"
#include "NameHelpers.hpp"
#include "PrintHelpers.hpp"
#include "RootHistogramHelpers.hpp"
#include "Configuration.hpp"
#include "Task.hpp"

ClassImp(CAP::JetPairMassHistos);

namespace CAP
{

  JetPairMassHistos::JetPairMassHistos()
  :
  HistogramGroup(),
  particleTypeList(nullptr),
  h_jetpair_n2_ptpt(nullptr),
  h_jetpair_n2_phiphi(nullptr),
  h_jetpair_n2_etaeta(nullptr),
  h_jetpair_n2_mass(nullptr),
  pt_nbins(0),
  phi_nbins(0),
  eta_nbins(0),
  Deta_nbins(0),
  Dphi_nbins(0),
  mass_nbins(0),
  pt_min(0),
  pt_max(0),
  phi_min(0),
  phi_max(0),
  eta_min(0),
  eta_max(0),
  Deta_min(0),
  Deta_max(0),
  Dphi_min(0),
  Dphi_max(0),
  mass_min(0),
  mass_max(0)
  { }

  JetPairMassHistos::JetPairMassHistos(const JetPairMassHistos & source)
  :
  HistogramGroup(source),
  particleTypeList(nullptr),
  h_jetpair_n2_ptpt(nullptr),
  h_jetpair_n2_phiphi(nullptr),
  h_jetpair_n2_etaeta(nullptr),
  h_jetpair_n2_DetaDphi(nullptr),
  h_jetpair_n2_mass(nullptr),
  pt_nbins(source.pt_nbins),
  phi_nbins(source.phi_nbins),
  eta_nbins(source.eta_nbins),
  Deta_nbins(source.Deta_nbins),
  Dphi_nbins(source.Dphi_nbins),
  mass_nbins(source.mass_nbins),
  pt_min(source.pt_min),
  pt_max(source.pt_max),
  phi_min(source.phi_min),
  phi_max(source.phi_max),
  eta_min(source.eta_min),
  eta_max(source.eta_max),
  Deta_min(source.Deta_min),
  Deta_max(source.Deta_max),
  Dphi_min(source.Dphi_min),
  Dphi_max(source.Dphi_max),
  mass_min(source.mass_min),
  mass_max(source.mass_max)
  {
  cloneB(source);
  }

  JetPairMassHistos & JetPairMassHistos::operator=(const JetPairMassHistos & rhs)
  {
  if (this!=&rhs)
    {
    HistogramGroup::operator=(rhs);
    cloneB(rhs);
    pt_nbins    = rhs.pt_nbins;
    phi_nbins   = rhs.phi_nbins;
    eta_nbins   = rhs.eta_nbins;
    Deta_nbins  = rhs.Deta_nbins;
    Dphi_nbins  = rhs.Dphi_nbins;
    mass_nbins  = rhs.mass_nbins;
    pt_min      = rhs.pt_min;
    pt_max      = rhs.pt_max;
    phi_min     = rhs.phi_min;
    phi_max     = rhs.phi_max;
    eta_min     = rhs.eta_min;
    eta_max     = rhs.eta_max;
    Deta_min    = rhs.Deta_min;
    Deta_max    = rhs.Deta_max;
    Dphi_min    = rhs.Dphi_min;
    Dphi_max    = rhs.Dphi_max;
    mass_min    = rhs.mass_min;
    mass_max    = rhs.mass_max;
    }
  return *this;
  }

  void JetPairMassHistos::cloneD(const JetPairMassHistos & source)
  {
  h_jetpair_n2_ptpt      = safeCloneH1(source.h_jetpair_n2_ptpt);
  h_jetpair_n2_phiphi    = safeCloneH1(source.h_jetpair_n2_phiphi);
  h_jetpair_n2_etaeta    = safeCloneH1(source.h_jetpair_n2_etaeta);
  h_jetpair_n2_DetaDphi  = safeCloneH1(source.h_jetpair_n2_DetaDphi);
  h_jetpair_n2_mass      = safeCloneH1(source.h_jetpair_n2_mass);
  }

  void JetPairMassHistos::configure(const String & taskName,
                                    const String & objectType,
                                    const Configuration & configuration,
                                    unsigned int index)
  {
  HistogramGroup::configure(taskName,objectType,configuration,index);
  pt_nbins   = configuration.valueInt(createKey(taskName,objectType,index,"pt_nbins"));
  pt_min     = configuration.valueDouble(createKey(taskName,objectType,index,"pt_min"));
  pt_max     = configuration.valueDouble(createKey(taskName,objectType,index,"pt_max"));
  phi_nbins  = configuration.valueInt(createKey(taskName,objectType,index,"phi_nbins"));
  phi_min    = configuration.valueDouble(createKey(taskName,objectType,index,"phi_min"));
  phi_max    = configuration.valueDouble(createKey(taskName,objectType,index,"phi_max"));
  eta_nbins  = configuration.valueInt(createKey(taskName,objectType,index,"eta_nbins"));
  eta_min    = configuration.valueDouble(createKey(taskName,objectType,index,"eta_min"));
  eta_max    = configuration.valueDouble(createKey(taskName,objectType,index,"eta_max"));
  Deta_nbins = configuration.valueInt(createKey(taskName,objectType,index,"Deta_nbins"));
  Deta_min   = configuration.valueDouble(createKey(taskName,objectType,index,"Deta_min"));
  Deta_max   = configuration.valueDouble(createKey(taskName,objectType,index,"Deta_max"));
  Dphi_nbins = configuration.valueInt(createKey(taskName,objectType,index,"Dphi_nbins"));
  Dphi_min   = configuration.valueDouble(createKey(taskName,objectType,index,"Dphi_min"));
  Dphi_max   = configuration.valueDouble(createKey(taskName,objectType,index,"Dphi_max"));
  mass_nbins = configuration.valueInt(createKey(taskName,objectType,index,"mass_nbins"));
  mass_min   = configuration.valueDouble(createKey(taskName,objectType,index,"mass_min"));
  mass_max   = configuration.valueDouble(createKey(taskName,objectType,index,"mass_max"));
  if (reportDebug(__FUNCTION__))
    {
    printCR();
    printValue("Histo Base Name",    bn);
    printValue("pt_nbins",pt_nbins);
    printValue("pt_min",  pt_min);
    printValue("pt_max",  pt_max);
    printValue("phi_nbins",phi_nbins);
    printValue("phi_min", phi_min);
    printValue("phi_max", phi_max);
    printValue("eta_nbins",eta_nbins);
    printValue("eta_min", eta_min);
    printValue("eta_max", eta_max);
    printValue("Deta_nbins",Deta_nbins);
    printValue("Deta_min", Deta_min);
    printValue("Deta_max", Deta_max);
    printValue("Dphi_nbins",Dphi_nbins);
    printValue("Dphi_min", Dphi_min);
    printValue("Dphi_max", Dphi_max);
    printValue("nBins_mass",mass_nbins);
    printValue("min_mass",  mass_min);
    printValue("max_mass",  mass_max);
    printLine();
    printCR();
    }
  }

  void JetPairMassHistos::create()
  {
  if (reportDebug(__FUNCTION__)) { printCR();}
  h_jetpair_n2_ptpt     = createHistogram(createName(_histogramBaseName,"jetpair_n2_ptpt"),pt_nbins,pt_min,pt_max, pt_nbins,pt_min,pt_max,"p_{T,1}","p_{T,2}","N_{2}");
  h_jetpair_n2_phiphi   = createHistogram(createName(_histogramBaseName,"jetpair_n2_phiphi"),phi_nbins,phi_min,phi_max, phi_nbins,phi_min,phi_max,"#varphi_{1}","#varphi_{2}","N_{2}");
  h_jetpair_n2_etaeta   = createHistogram(createName(_histogramBaseName,"jetpair_n2_etaeta"),eta_nbins,eta_min,eta_max, eta_nbins,eta_min,eta_max,"#eta_{1}","#eta_{2}","N_{2}");
  h_jetpair_n2_DetaDphi = createHistogram(createName(_histogramBaseName,"jetpair_n2_DetaDphi"),Deta_nbins,Deta_min,Deta_max,Dphi_nbins,Dphi_min,Dphi_maxh,"#Delta#eta","#Delta#phi","N_{2}");
  h_jetpair_n2_mass     = createHistogram(createName(_histogramBaseName,"jetpair_n2_mass"),nBins_mass,min_mass,max_mass,"mass","N_{2}");


  }

  void JetPairMassHistos::loadFrom(TFile & inputFile)
  {
  if (reportDebug(__FUNCTION__)) { printCR();}
  h_jetpair_n2_ptpt      = loadH2(inputFile,createName(_histogramBaseName,"jetpair_n2_ptpt"));
  h_jetpair_n2_phiphi    = loadH2(inputFile,createName(_histogramBaseName,"jetpair_n2_phiphi"));
  h_jetpair_n2_etaeta    = loadH2(inputFile,createName(_histogramBaseName,"jetpair_n2_etaeta"));
  h_jetpair_n2_thth      = loadH2(inputFile,createName(_histogramBaseName,"jetpair_n2_thth"));
  h_jetpair_n2_jtjt      = loadH2(inputFile,createName(_histogramBaseName,"jetpair_n2_jtjt"));
  h_jetpair_n2_zz        = loadH2(inputFile,createName(_histogramBaseName,"jetpair_n2_zz"));

  h_jetpair_n2pp_ptpt    = loadH2(inputFile,createName(_histogramBaseName,"jetpair_n2pp_ptpt"));
  h_jetpair_n2pp_phiphi  = loadH2(inputFile,createName(_histogramBaseName,"jetpair_n2pp_phiphi"));
  h_jetpair_n2pp_etaeta  = loadH2(inputFile,createName(_histogramBaseName,"jetpair_n2pp_etaeta"));
  h_jetpair_n2pp_thth    = loadH2(inputFile,createName(_histogramBaseName,"jetpair_n2pp_thth"));
  h_jetpair_n2pp_jtjt    = loadH2(inputFile,createName(_histogramBaseName,"jetpair_n2pp_jtjt"));
  h_jetpair_n2pp_zz      = loadH2(inputFile,createName(_histogramBaseName,"jetpair_n2pp_zz"));

  h_jetpair_n2pm_ptpt    = loadH2(inputFile,createName(_histogramBaseName,"jetpair_n2pm_ptpt"));
  h_jetpair_n2pm_phiphi  = loadH2(inputFile,createName(_histogramBaseName,"jetpair_n2pm_phiphi"));
  h_jetpair_n2pm_etaeta  = loadH2(inputFile,createName(_histogramBaseName,"jetpair_n2pm_etaeta"));
  h_jetpair_n2pm_thth    = loadH2(inputFile,createName(_histogramBaseName,"jetpair_n2pm_thth"));
  h_jetpair_n2pm_jtjt    = loadH2(inputFile,createName(_histogramBaseName,"jetpair_n2pm_jtjt"));
  h_jetpair_n2pm_zz      = loadH2(inputFile,createName(_histogramBaseName,"jetpair_n2pm_zz"));

  h_jetpair_n2mm_ptpt    = loadH2(inputFile,createName(_histogramBaseName,"jetpair_n2mm_ptpt"));
  h_jetpair_n2mm_phiphi  = loadH2(inputFile,createName(_histogramBaseName,"jetpair_n2mm_phiphi"));
  h_jetpair_n2mm_etaeta  = loadH2(inputFile,createName(_histogramBaseName,"jetpair_n2mm_etaeta"));
  h_jetpair_n2mm_thth    = loadH2(inputFile,createName(_histogramBaseName,"jetpair_n2mm_thth"));
  h_jetpair_n2mm_jtjt    = loadH2(inputFile,createName(_histogramBaseName,"jetpair_n2mm_jtjt"));
  h_jetpair_n2mm_zz      = loadH2(inputFile,createName(_histogramBaseName,"jetpair_n2mm_zz"));
  }

  void JetPairMassHistos::fill(PseudoJet& jet)
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

      h_jetpair_n2_ptpt->Fill(part1_pt,part2_pt);
      h_jetpair_n2_phiphi->Fill(part1_phi,part2_phi);
      h_jetpair_n2_etaeta->Fill(part1_eta,part2_eta);
      h_jetpair_n2_thth->Fill(part1_th,part2_th);
      h_jetpair_n2_jtjt->Fill(part1_jt,part2_jt);
      h_jetpair_n2_zz->Fill(part1_z,part2_z);

      if (q1>0  &&  q2>0)
        {
        h_jetpair_n2pp_ptpt->Fill(part1_pt,part2_pt);
        h_jetpair_n2pp_phiphi->Fill(part1_phi,part2_phi);
        h_jetpair_n2pp_etaeta->Fill(part1_eta,part2_eta);
        h_jetpair_n2pp_thth->Fill(part1_th,part2_th);
        h_jetpair_n2pp_jtjt->Fill(part1_jt,part2_jt);
        h_jetpair_n2pp_zz->Fill(part1_z,part2_z);
        }
      else if (q1>0  &&  q2<0)
        {
        h_jetpair_n2pm_ptpt->Fill(part1_pt,part2_pt);
        h_jetpair_n2pm_phiphi->Fill(part1_phi,part2_phi);
        h_jetpair_n2pm_etaeta->Fill(part1_eta,part2_eta);
        h_jetpair_n2pm_thth->Fill(part1_th,part2_th);
        h_jetpair_n2pm_jtjt->Fill(part1_jt,part2_jt);
        h_jetpair_n2pm_zz->Fill(part1_z,part2_z);
        }
      else if (q1<0  &&  q2>0)
        {
        h_jetpair_n2pm_ptpt->Fill(part2_pt,part1_pt);
        h_jetpair_n2pm_phiphi->Fill(part2_phi,part1_phi);
        h_jetpair_n2pm_etaeta->Fill(part2_eta,part1_eta);
        h_jetpair_n2pm_thth->Fill(part2_th,part1_th);
        h_jetpair_n2pm_jtjt->Fill(part2_jt,part1_jt);
        h_jetpair_n2pm_zz->Fill(part2_z,part1_z);
        }
      else if (q1<0  &&  q2<0)
        {
        h_jetpair_n2mm_ptpt->Fill(part1_pt,part2_pt);
        h_jetpair_n2mm_phiphi->Fill(part1_phi,part2_phi);
        h_jetpair_n2mm_etaeta->Fill(part1_eta,part2_eta);
        h_jetpair_n2mm_thth->Fill(part1_th,part2_th);
        h_jetpair_n2mm_jtjt->Fill(part1_jt,part2_jt);
        h_jetpair_n2mm_zz->Fill(part1_z,part2_z);
        }
      }
    }
  }

  void JetPairMassHistos::scaleHistograms(double scale)
  {
  h_jetpair_n2_ptpt->Scale(scale);
  h_jetpair_n2_phiphi->Scale(scale);
  h_jetpair_n2_etaeta->Scale(scale);
  h_jetpair_n2_thth->Scale(scale);
  h_jetpair_n2_jtjt->Scale(scale);
  h_jetpair_n2_zz->Scale(scale);

  h_jetpair_n2pp_ptpt->Scale(scale);
  h_jetpair_n2pp_phiphi->Scale(scale);
  h_jetpair_n2pp_etaeta->Scale(scale);
  h_jetpair_n2pp_thth->Scale(scale);
  h_jetpair_n2pp_jtjt->Scale(scale);
  h_jetpair_n2pp_zz->Scale(scale);

  h_jetpair_n2pm_ptpt->Scale(scale);
  h_jetpair_n2pm_phiphi->Scale(scale);
  h_jetpair_n2pm_etaeta->Scale(scale);
  h_jetpair_n2pm_thth->Scale(scale);
  h_jetpair_n2pm_jtjt->Scale(scale);
  h_jetpair_n2pm_zz->Scale(scale);

  h_jetpair_n2mm_ptpt->Scale(scale);
  h_jetpair_n2mm_phiphi->Scale(scale);
  h_jetpair_n2mm_etaeta->Scale(scale);
  h_jetpair_n2mm_thth->Scale(scale);
  h_jetpair_n2mm_jtjt->Scale(scale);
  h_jetpair_n2mm_zz->Scale(scale);

  }

} // namespace CAP
