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
#include "JetSingleHistos.hpp"
#include "JetHelpers.hpp"
#include "NameHelpers.hpp"
#include "PrintHelpers.hpp"
#include "RootHistogramHelpers.hpp"
#include "Configuration.hpp"
#include "Task.hpp"

ClassImp(CAP::JetSingleHistos);

namespace CAP
{

  JetSingleHistos::JetSingleHistos()
  :
  HistogramGroup(),
  h_jet_n1_pt(nullptr),
  h_jet_n1_phi(nullptr),
  h_jet_n1_eta(nullptr),
  h_jet_n1_th(nullptr),
  h_jet_n1_jt(nullptr),
  h_jet_n1_z(nullptr),

  h_jet_n1p_pt(nullptr),
  h_jet_n1p_phi(nullptr),
  h_jet_n1p_eta(nullptr),
  h_jet_n1p_th(nullptr),
  h_jet_n1p_jt(nullptr),
  h_jet_n1p_z(nullptr),

  h_jet_n1m_pt(nullptr),
  h_jet_n1m_phi(nullptr),
  h_jet_n1m_eta(nullptr),
  h_jet_n1m_th(nullptr),
  h_jet_n1m_jt(nullptr),
  h_jet_n1m_z(nullptr),
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
  {  }

  JetSingleHistos::JetSingleHistos(const JetSingleHistos & source)
  :
  HistogramGroup(source),
  particleTypeList(nullptr),
  h_jet_n1_pt(nullptr),
  h_jet_n1_phi(nullptr),
  h_jet_n1_eta(nullptr),
  h_jet_n1_th(nullptr),
  h_jet_n1_jt(nullptr),
  h_jet_n1_z(nullptr),

  h_jet_n1p_pt(nullptr),
  h_jet_n1p_phi(nullptr),
  h_jet_n1p_eta(nullptr),
  h_jet_n1p_th(nullptr),
  h_jet_n1p_jt(nullptr),
  h_jet_n1p_z(nullptr),

  h_jet_n1m_pt(nullptr),
  h_jet_n1m_phi(nullptr),
  h_jet_n1m_eta(nullptr),
  h_jet_n1m_th(nullptr),
  h_jet_n1m_jt(nullptr),
  h_jet_n1m_z(nullptr),
  pt_nbins(source.pt_nbins),
  phi_nbins(source.phi_nbins),
  eta_nbins(source.eta_nbins),
  jt_nbins(source.jt_nbins),
  th_nbins(source.th_nbins),
  z_nbins(source.z_nbins),
  pt_min(source.pt_min),
  pt_max(source.pt_max),
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
  { }

  JetSingleHistos & JetSingleHistos::operator=(const JetSingleHistos & rhs)
  {
  if (this!=&rhs)
    {
    HistogramGroup::operator=(rhs);
    particleTypeList = rhs.particleTypeList;
    pt_nbins  = rhs.pt_nbins;
    phi_nbins = rhs.phi_nbins;
    eta_nbins = rhs.eta_nbins;
    jt_nbins  = rhs.jt_nbins;
    th_nbins  = rhs.th_nbins;
    z_nbins   = rhs.z_nbins;
    pt_min    = rhs.pt_min;
    pt_max    = rhs.pt_max;
    phi_min   = rhs.phi_min;
    phi_max   = rhs.phi_max;
    eta_min   = rhs.eta_min;
    eta_max   = rhs.eta_max;
    jt_min    = rhs.jt_min;
    jt_max    = rhs.jt_max;
    th_min    = rhs.th_min;
    th_max    = rhs.th_max;
    z_min     = rhs.z_min;
    z_max     = rhs.z_max;
    }
  return *this;
  }

  void JetSingleHistos::cloneB(const JetSingleHistos & source)
  {
  h_jet_n1_pt   = safeCloneH1(source.h_jet_n1_pt);
  h_jet_n1_phi  = safeCloneH1(source.h_jet_n1_phi);
  h_jet_n1_eta  = safeCloneH1(source.h_jet_n1_eta);
  h_jet_n1_th   = safeCloneH1(source.h_jet_n1_th);
  h_jet_n1_jt   = safeCloneH1(source.h_jet_n1_jt);
  h_jet_n1_z    = safeCloneH1(source.h_jet_n1_z);

  h_jet_n1p_pt  = safeCloneH1(source.h_jet_n1p_pt);
  h_jet_n1p_phi = safeCloneH1(source.h_jet_n1p_phi);
  h_jet_n1p_eta = safeCloneH1(source.h_jet_n1p_eta);
  h_jet_n1p_th  = safeCloneH1(source.h_jet_n1p_th);
  h_jet_n1p_jt  = safeCloneH1(source.h_jet_n1p_jt);
  h_jet_n1p_z   = safeCloneH1(source.h_jet_n1p_z);

  h_jet_n1m_pt  = safeCloneH1(source.h_jet_n1m_pt);
  h_jet_n1m_phi = safeCloneH1(source.h_jet_n1m_phi);
  h_jet_n1m_eta = safeCloneH1(source.h_jet_n1m_eta);
  h_jet_n1m_th  = safeCloneH1(source.h_jet_n1m_th);
  h_jet_n1m_jt  = safeCloneH1(source.h_jet_n1m_jt);
  h_jet_n1m_z   = safeCloneH1(source.h_jet_n1m_z);
  }

  void JetSingleHistos::configure(const String & taskName,
                                  const String & objectType,
                                  const Configuration & configuration,
                                  unsigned int index)
  {
  HistogramGroup::configure(taskName,objectType,configuration,index);
  pt_nbins   = configuration.valueInt(createKey(taskName,objectType,index,    "pt_nbins"));
  pt_min     = configuration.valueDouble(createKey(taskName,objectType,index, "pt_min"));
  pt_max     = configuration.valueDouble(createKey(taskName,objectType,index, "pt_max"));
  phi_nbins  = configuration.valueInt(createKey(taskName,objectType,index,    "phi_nbins"));
  phi_min    = configuration.valueDouble(createKey(taskName,objectType,index, "phi_min"));
  phi_max    = configuration.valueDouble(createKey(taskName,objectType,index, "phi_max"));
  eta_nbins  = configuration.valueInt(createKey(taskName,objectType,index,    "eta_nbins"));
  eta_min    = configuration.valueDouble(createKey(taskName,objectType,index, "eta_min"));
  eta_max    = configuration.valueDouble(createKey(taskName,objectType,index, "eta_max"));
  jt_nbins   = configuration.valueInt(createKey(taskName,objectType,index,    "jt_nbins"));
  jt_min     = configuration.valueDouble(createKey(taskName,objectType,index, "jt_min"));
  jt_max     = configuration.valueDouble(createKey(taskName,objectType,index, "jt_max"));
  th_nbins   = configuration.valueInt(createKey(taskName,objectType,index,    "th_nbins"));
  th_min     = configuration.valueDouble(createKey(taskName,objectType,index, "th_min"));
  th_max     = configuration.valueDouble(createKey(taskName,objectType,index, "th_max"));
  z_nbins    = configuration.valueInt(createKey(taskName,objectType,index,    "z_nbins"));
  z_min      = configuration.valueDouble(createKey(taskName,objectType,index, "z_min"));
  z_max      = configuration.valueDouble(createKey(taskName,objectType,index, "z_max"));
  if (reportDebug(__FUNCTION__))
    {
    printCR();
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
    printValue("z_nbins",     z_nbins);
    printValue("z_min",       z_min);
    printValue("z_max",       z_max);
    printCR();
    }
  }

  void JetSingleHistos::create()
  {
  if (reportDebug(__FUNCTION__)) { printCR();}
  h_jet_n1_pt    = createHistogram(createName(_histogramBaseName,"jet_n1_pt"),    pt_nbins,    pt_min,    pt_max,     "pt","N_1");
  h_jet_n1_phi   = createHistogram(createName(_histogramBaseName,"jet_n1_phi"),   phi_nbins,   phi_min,   phi_max,    "phi","N_1");
  h_jet_n1_eta   = createHistogram(createName(_histogramBaseName,"jet_n1_eta"),   eta_nbins,   eta_min,   eta_max,    "#eta","N_1");
  h_jet_n1_th    = createHistogram(createName(_histogramBaseName,"jet_n1_th"),    th_nbins,    th_min,    th_max,     "#theta","N_1");
  h_jet_n1_jt    = createHistogram(createName(_histogramBaseName,"jet_n1_jt"),    jt_nbins,    jt_min,    jt_max,     "j_{t}","N_1");
  h_jet_n1_z     = createHistogram(createName(_histogramBaseName,"jet_n1_z"),     z_nbins,     z_min,     z_max,      "p_{t}","N_1");

  h_jet_n1p_pt    = createHistogram(createName(_histogramBaseName,"jet_n1p_pt"),    pt_nbins,    pt_min,    pt_max,     "pt","N_1^{+}");
  h_jet_n1p_phi   = createHistogram(createName(_histogramBaseName,"jet_n1p_phi"),   phi_nbins,   phi_min,   phi_max,    "phi","N_1^{+}");
  h_jet_n1p_eta   = createHistogram(createName(_histogramBaseName,"jet_n1p_eta"),   eta_nbins,   eta_min,   eta_max,    "#eta","N_1^{+}");
  h_jet_n1p_th    = createHistogram(createName(_histogramBaseName,"jet_n1p_th"),    th_nbins,    th_min,    th_max,     "#theta","N_1^{+}");
  h_jet_n1p_jt    = createHistogram(createName(_histogramBaseName,"jet_n1p_jt"),    jt_nbins,    jt_min,    jt_max,     "j_{t}","N_1^{+}");
  h_jet_n1p_z     = createHistogram(createName(_histogramBaseName,"jet_n1p_z"),     z_nbins,     z_min,     z_max,      "p_{t}","N_1^{+}");

  h_jet_n1m_pt    = createHistogram(createName(_histogramBaseName,"jet_n1m_pt"),    pt_nbins,    pt_min,    pt_max,     "pt","N_1^{-}");
  h_jet_n1m_phi   = createHistogram(createName(_histogramBaseName,"jet_n1m_phi"),   phi_nbins,   phi_min,   phi_max,    "phi","N_1^{-}");
  h_jet_n1m_eta   = createHistogram(createName(_histogramBaseName,"jet_n1m_eta"),   eta_nbins,   eta_min,   eta_max,    "#eta","N_1^{-}");
  h_jet_n1m_th    = createHistogram(createName(_histogramBaseName,"jet_n1m_th"),    th_nbins,    th_min,    th_max,     "#theta","N_1^{-}");
  h_jet_n1m_jt    = createHistogram(createName(_histogramBaseName,"jet_n1m_jt"),    jt_nbins,    jt_min,    jt_max,     "j_{t}","N_1^{-}");
  h_jet_n1m_z     = createHistogram(createName(_histogramBaseName,"jet_n1m_z"),     z_nbins,     z_min,     z_max,      "p_{t}","N_1^{-}");
  }

  void JetSingleHistos::loadFrom(TFile & inputFile)
  {
  if (reportDebug(__FUNCTION__)) { printCR();}
  h_jet_n1_pt    = loadH1(inputFile,createName(_histogramBaseName,"jet_n1_pt"));
  h_jet_n1_phi   = loadH1(inputFile,createName(_histogramBaseName,"jet_n1_phi"));
  h_jet_n1_eta   = loadH1(inputFile,createName(_histogramBaseName,"jet_n1_eta"));
  h_jet_n1_th    = loadH1(inputFile,createName(_histogramBaseName,"jet_n1_th"));
  h_jet_n1_jt    = loadH1(inputFile,createName(_histogramBaseName,"jet_n1_jt"));
  h_jet_n1_z     = loadH1(inputFile,createName(_histogramBaseName,"jet_n1_z"));

  h_jet_n1p_pt    = loadH1(inputFile,createName(_histogramBaseName,"jet_n1p_pt"));
  h_jet_n1p_phi   = loadH1(inputFile,createName(_histogramBaseName,"jet_n1p_phi"));
  h_jet_n1p_eta   = loadH1(inputFile,createName(_histogramBaseName,"jet_n1p_eta"));
  h_jet_n1p_th    = loadH1(inputFile,createName(_histogramBaseName,"jet_n1p_th"));
  h_jet_n1p_jt    = loadH1(inputFile,createName(_histogramBaseName,"jet_n1p_jt"));
  h_jet_n1p_z     = loadH1(inputFile,createName(_histogramBaseName,"jet_n1p_z"));

  h_jet_n1m_pt    = loadH1(inputFile,createName(_histogramBaseName,"jet_n1m_pt"));
  h_jet_n1m_phi   = loadH1(inputFile,createName(_histogramBaseName,"jet_n1m_phi"));
  h_jet_n1m_eta   = loadH1(inputFile,createName(_histogramBaseName,"jet_n1m_eta"));
  h_jet_n1m_th    = loadH1(inputFile,createName(_histogramBaseName,"jet_n1m_th"));
  h_jet_n1m_jt    = loadH1(inputFile,createName(_histogramBaseName,"jet_n1m_jt"));
  h_jet_n1m_z     = loadH1(inputFile,createName(_histogramBaseName,"jet_n1m_z"));
  }

  void JetSingleHistos::fill(PseudoJet& jet)
  {
  //double jet_phi = jet.phi();
  double jet_pt  = jet.perp();
  //double jet_eta = jet.pseudorapidity();

  // Constituents of the passed Jet
  const std::vector<PseudoJet> & constituents = jet.constituents();
  for (const auto & part : constituents)
    {
    int pdgId = part.user_index();
    ParticleType * type = particleTypeList->findPdgCode(pdgId);
    double q = type->charge();

    double part_phi = part.phi();
    double part_pt  = part.perp();
    double part_eta = part.pseudorapidity();

    double part_z   = part_pt/jet_pt;
    double part_jt  = 0;
    double part_th  = 0;
    //calculateJtTheta(jet_px,jet_py,jet_pz,jet_p,part_px,part_py,part_pz,part_p,part_jt,part_th);
    h_jet_n1_pt->Fill(part_pt);
    h_jet_n1_phi->Fill(part_phi);
    h_jet_n1_eta->Fill(part_eta);
    h_jet_n1_th->Fill(part_th);
    h_jet_n1_jt->Fill(part_jt);
    h_jet_n1_z->Fill(part_z);

    if (q>0)
      {
      h_jet_n1p_pt->Fill(part_pt);
      h_jet_n1p_phi->Fill(part_phi);
      h_jet_n1p_eta->Fill(part_eta);
      h_jet_n1p_th->Fill(part_th);
      h_jet_n1p_jt->Fill(part_jt);
      h_jet_n1p_z->Fill(part_z);
      }
    else
      {
      h_jet_n1m_pt->Fill(part_pt);
      h_jet_n1m_phi->Fill(part_phi);
      h_jet_n1m_eta->Fill(part_eta);
      h_jet_n1m_th->Fill(part_th);
      h_jet_n1m_jt->Fill(part_jt);
      h_jet_n1m_z->Fill(part_z);
      }
    }
  }

} // namespace CAP





