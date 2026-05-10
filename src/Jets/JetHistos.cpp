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
#include "JetHistos.hpp"
#include "JetHelpers.hpp"
#include "PrintHelpers.hpp"
#include "RootHistogramHelpers.hpp"
#include "ParticleType.hpp"
#include "Configuration.hpp"
#include "NameHelpers.hpp"
#include "Task.hpp"

ClassImp(CAP::JetHistos);

namespace CAP
{
  JetHistos::JetHistos()
  :
  HistogramGroup(),
  particleTypeList(nullptr),
  h_jet_count(nullptr),
  h_jet_pt(nullptr),
  h_jet_eta(nullptr),
  h_jet_etaPt(nullptr),
  h_jet_n(nullptr),
  h_jet_np(nullptr),
  h_jet_nm(nullptr),
  h_jet_n0(nullptr),
  h_jet_netQ(nullptr),
  p_jet_n(nullptr),
  p_jet_np(nullptr),
  p_jet_nm(nullptr),
  p_jet_n0(nullptr),
  p_jet_npSq(nullptr),
  p_jet_nmSq(nullptr),
  p_jet_n0Sq(nullptr),
  p_jet_npnm(nullptr),
  p_jet_npn0(nullptr),
  p_jet_nmn0(nullptr),
  p_jet_netQ(nullptr),
  p_jet_netQSq(nullptr),
  jet_n1_nbins(0),
  jet_p_nbins(0),
  jet_pt_nbins(0),
  jet_phi_nbins(0),
  jet_eta_nbins(0),
  jet_netQ_nbins(0),
  jet_n1_min(0),
  jet_p_min(0),
  jet_pt_min(0),
  jet_phi_min(0),
  jet_eta_min(0),
  jet_netQ_min(0),
  jet_n1_max(0),
  jet_p_max(0),
  jet_pt_max(0),
  jet_phi_max(0),
  jet_eta_max(0),
  jet_netQ_max(0)
  { }


  JetHistos::JetHistos(const JetHistos & source)
  :
  HistogramGroup(source),
  particleTypeList(nullptr),
  h_jet_count(nullptr),
  h_jet_pt(nullptr),
  h_jet_eta(nullptr),
  h_jet_etaPt(nullptr),
  h_jet_n(nullptr),
  h_jet_np(nullptr),
  h_jet_nm(nullptr),
  h_jet_n0(nullptr),
  h_jet_netQ(nullptr),
  p_jet_n(nullptr),
  p_jet_np(nullptr),
  p_jet_nm(nullptr),
  p_jet_n0(nullptr),
  p_jet_npSq(nullptr),
  p_jet_nmSq(nullptr),
  p_jet_n0Sq(nullptr),
  p_jet_npnm(nullptr),
  p_jet_npn0(nullptr),
  p_jet_nmn0(nullptr),
  p_jet_netQ(nullptr),
  p_jet_netQSq(nullptr),
  jet_n1_nbins(source.jet_n1_nbins),
  jet_p_nbins(source.jet_p_nbins),
  jet_pt_nbins(source.jet_pt_nbins),
  jet_phi_nbins(source.jet_phi_nbins),
  jet_eta_nbins(source.jet_eta_nbins),
  jet_netQ_nbins(source.jet_netQ_nbins),
  jet_n1_min(source.jet_n1_min),
  jet_p_min(source.jet_p_min),
  jet_pt_min(source.jet_pt_min),
  jet_phi_min(source.jet_phi_min),
  jet_eta_min(source.jet_eta_min),
  jet_netQ_min(source.jet_netQ_min),
  jet_n1_max(source.jet_n1_max),
  jet_p_max(source.jet_p_max),
  jet_pt_max(source.jet_pt_max),
  jet_phi_max(source.jet_phi_max),
  jet_eta_max(source.jet_eta_max),
  jet_netQ_max(source.jet_netQ_max)
  {
  cloneB();
  }

  JetHistos & JetHistos::operator=(const JetHistos & rhs)
  {
  if (this!=&rhs)
    {
    HistogramGroup::operator=(rhs);
    jet_n1_nbins= rhs.jet_n1_nbins;
    jet_p_nbins= rhs.jet_p_nbins;
    jet_pt_nbins= rhs.jet_pt_nbins;
    jet_phi_nbins= rhs.jet_phi_nbins;
    jet_eta_nbins= rhs.jet_eta_nbins;
    jet_netQ_nbins= rhs.jet_netQ_nbins;
    jet_n1_min= rhs.jet_n1_min;
    jet_p_min= rhs.jet_p_min;
    jet_pt_min= rhs.jet_pt_min;
    jet_phi_min= rhs.jet_phi_min;
    jet_eta_min= rhs.jet_eta_min;
    jet_netQ_min= rhs.jet_netQ_min;
    jet_n1_max= rhs.jet_n1_max;
    jet_p_max= rhs.jet_p_max;
    jet_pt_max= rhs.jet_pt_max;
    jet_phi_max= rhs.jet_phi_max;
    jet_eta_max= rhs.jet_eta_max;
    jet_netQ_max= rhs.jet_netQ_max);
    }
  return *this;
  }

  void JetHistos::cloneB(const JetHistos & source)
  {
  h_jet_n1 = safeCloneH1(source.h_jet_n1);
  h_jet_pt = safeCloneH1(source.h_jet_pt);
  h_jet_phi = safeCloneH1(source.h_jet_phi);
  h_jet_eta = safeCloneH1(source.h_jet_eta);
  h_jet_etaPt = safeCloneH2(source.h_jet_etaPt);
  h_jet_n = safeCloneH1(source.h_jet_n);
  h_jet_np = safeCloneH1(source.h_jet_np);
  h_jet_nm = safeCloneH1(source.h_jet_nm);
  h_jet_n0 = safeCloneH1(source.h_jet_n0);
  h_jet_netQ = safeCloneH1(source.h_jet_netQ);
  p_jet_n = safeCloneProfile(source.p_jet_n);
  p_jet_np = safeCloneProfile(source.p_jet_np);
  p_jet_nm = safeCloneProfile(source.p_jet_nm);
  p_jet_n0 = safeCloneProfile(source.p_jet_n0);
  p_jet_npSq = safeCloneProfile(source.p_jet_npSq);
  p_jet_nmSq = safeCloneProfile(source.p_jet_nmSq);
  p_jet_n0Sq = safeCloneProfile(source.p_jet_n0Sq);
  p_jet_npnm = safeCloneProfile(source.p_jet_npnm);
  p_jet_npn0 = safeCloneProfile(source.p_jet_npn0);
  p_jet_nmn0 = safeCloneProfile(source.p_jet_nmn0);
  p_jet_netQ = safeCloneProfile(source.p_jet_netQ);
  p_jet_netQSq = safeCloneProfile(source.p_jet_netQSq);
  }

  void JetHistos::configure(const String & taskName,
                            const String & objectType,
                            const Configuration & configuration,
                            unsigned int index)
  {
  if (reportDebug(__FUNCTION__)) { printCR();}
  HistogramGroup::configure(taskName,objectType,configuration,index);
  jet_n1_nbins    = configuration.valueInt(   createKey(taskName,objectType,index,"jet_n1_nbins"));
  jet_p_nbins     = configuration.valueInt(   createKey(taskName,objectType,index,"jet_p_nbins"));
  jet_pt_nbins    = configuration.valueInt(   createKey(taskName,objectType,index,"jet_pt_nbins"));
  jet_phi_nbins   = configuration.valueInt(   createKey(taskName,objectType,index,"jet_phi_nbins"));
  jet_eta_nbins   = configuration.valueInt(   createKey(taskName,objectType,index,"jet_eta_nbins"));
  jet_netQ_nbins  = configuration.valueInt(   createKey(taskName,objectType,index,"jet_netQ_nbins"));
  jet_n1_min   = configuration.valueDouble(createKey(taskName,objectType,index,"jet_n1_min"));
  jet_p_min    = configuration.valueDouble(createKey(taskName,objectType,index,"jet_p_min"));
  jet_pt_min   = configuration.valueDouble(createKey(taskName,objectType,index,"jet_pt_min"));
  jet_phi_min  = configuration.valueDouble(createKey(taskName,objectType,index,"jet_phi_min"));
  jet_eta_min  = configuration.valueDouble(createKey(taskName,objectType,index,"jet_eta_min"));
  jet_netQ_min = configuration.valueDouble(createKey(taskName,objectType,index,"jet_netQ_min"));
  jet_n1_max   = configuration.valueDouble(createKey(taskName,objectType,index,"jet_n1_max"));
  jet_p_max    = configuration.valueDouble(createKey(taskName,objectType,index,"jet_p_max"));
  jet_pt_max   = configuration.valueDouble(createKey(taskName,objectType,index,"jet_pt_max"));
  jet_phi_max  = configuration.valueDouble(createKey(taskName,objectType,index,"jet_phi_max"));
  jet_eta_max  = configuration.valueDouble(createKey(taskName,objectType,index,"jet_eta_max"));
  jet_netQ_max = configuration.valueDouble(createKey(taskName,objectType,index,"jet_netQ_max"));
  }

  void JetHistos::create()
  {
  if (reportDebug(__FUNCTION__)) { printCR();}
  h_jet_n1     = createHistogram(createName(_histogramBaseName,"jet_n1"),     jet_n1_nbins,  jet_n1_min,   jet_n1_max,   "jet","Count");
  h_jet_pt     = createHistogram(createName(_histogramBaseName,"jet_pt"),     jet_pt_nbins,  jet_pt_min,   jet_pt_max,   "pt (GeV/c)","N");
  h_jet_phi    = createHistogram(createName(_histogramBaseName,"jet_phi"),    jet_phi_nbins, jet_phi_min,  jet_phi_max,  "#varphi","N");
  h_jet_eta    = createHistogram(createName(_histogramBaseName,"jet_eta"),    jet_eta_nbins, jet_eta_min,  jet_eta_max,  "#eta","N");
  h_jet_etaPt  = createHistogram(createName(_histogramBaseName,"jet_etaPt"),  jet_eta_nbins, jet_eta_min,  jet_eta_max,  jet_pt_nbins, jet_pt_min, jet_pt_max,"#eta_{jet}", "p (GeV/c)","N");
  h_jet_n      = createHistogram(createName(_histogramBaseName,"jet_n"),      jet_n_nbins,   jet_n_min,    jet_n_max,    "n_{part}","N");
  h_jet_np     = createHistogram(createName(_histogramBaseName,"jet_np"),     jet_np_nbins,  jet_np_min,   jet_np_max,   "n_{+}","N");
  h_jet_nm     = createHistogram(createName(_histogramBaseName,"jet_nm"),     jet_nm_nbins,  jet_nm_min,   jet_nm_max,   "n_{-}","N");
  h_jet_n0     = createHistogram(createName(_histogramBaseName,"jet_n0"),     jet_n0_nbins,  jet_n0_min,   jet_n0_max,   "n_{0}","N");
  h_jet_netQ   = createHistogram(createName(_histogramBaseName,"jet_netQ"),   jet_netQ_nbins,jet_netQ_min, jet_netQ_max, "Q","N");
  p_jet_n      = createProfile(createName(_histogramBaseName,"jet_n_p"),      jet_p_nbins,   jet_p_min,    jet_p_max,    "p_{T} (GeV/c)","<n_{part}>");
  p_jet_np     = createProfile(createName(_histogramBaseName,"jet_np_p"),     jet_p_nbins,   jet_p_min,    jet_p_max,    "p_{T} (GeV/c)","<n_{+}>");
  p_jet_nm     = createProfile(createName(_histogramBaseName,"jet_nm_p"),     jet_p_nbins,   jet_p_min,    jet_p_max,    "p_{T} (GeV/c)","<n_{-}>");
  p_jet_n0     = createProfile(createName(_histogramBaseName,"jet_n0_p"),     jet_p_nbins,   jet_p_min,    jet_p_max,    "p_{T} (GeV/c)","<n_{0}>");
  p_jet_npSq   = createProfile(createName(_histogramBaseName,"jet_npSq_p"),   jet_p_nbins,   jet_p_min,    jet_p_max,    "p_{T} (GeV/c)","<n_{+}(n_{+}-1)>");
  p_jet_nmSq   = createProfile(createName(_histogramBaseName,"jet_nmSq_p"),   jet_p_nbins,   jet_p_min,    jet_p_max,    "p_{T} (GeV/c)","<n_{-}(n_{-}-1)>");
  p_jet_n0Sq   = createProfile(createName(_histogramBaseName,"jet_n0Sq_p"),   jet_p_nbins,   jet_p_min,    jet_p_max,    "p_{T} (GeV/c)","<n_{0}(n_{0}-1)>");
  p_jet_npnm   = createProfile(createName(_histogramBaseName,"jet_npnm_p"),   jet_p_nbins,   jet_p_min,    jet_p_max,    "p_{T} (GeV/c)","<n_{p}n_{m}>");
  p_jet_npn0   = createProfile(createName(_histogramBaseName,"jet_npn0_p"),   jet_p_nbins,   jet_p_min,    jet_p_max,    "p_{T} (GeV/c)","<n_{p}n_{0}>");
  p_jet_nmn0   = createProfile(createName(_histogramBaseName,"jet_nmn0_p"),   jet_p_nbins,   jet_p_min,    jet_p_max,    "p_{T} (GeV/c)","<n_{m}n_{0}>");
  p_jet_netQ   = createProfile(createName(_histogramBaseName,"jet_netQ_p"),   jet_p_nbins,   jet_p_min,    jet_p_max,    "p_{T} (GeV/c)","<Q>");
  p_jet_netQSq = createProfile(createName(_histogramBaseName,"jet_netQSq_p"), jet_p_nbins,   jet_p_min,    jet_p_max,    "p_{T} (GeV/c)","<Q^2>");
  }

  void JetHistos::loadFrom(TFile & inputFile)
  {
  if (reportDebug(__FUNCTION__)) { printCR();}
  h_jet_count  = loadH1(inputFile,createName(_histogramBaseName,"jet_count"));
  h_jet_pt     = loadH1(inputFile,createName(_histogramBaseName,"jet_pt"));
  h_jet_phi    = loadH1(inputFile,createName(_histogramBaseName,"jet_phi"));
  h_jet_eta    = loadH1(inputFile,createName(_histogramBaseName,"jet_eta"));
  h_jet_etaPt  = loadH2(inputFile,createName(_histogramBaseName,"jet_etaPt"));
  h_jet_n      = loadH1(inputFile,createName(_histogramBaseName,"jet_n"));
  h_jet_np     = loadH1(inputFile,createName(_histogramBaseName,"jet_np"));
  h_jet_nm     = loadH1(inputFile,createName(_histogramBaseName,"jet_nm"));
  h_jet_n0     = loadH1(inputFile,createName(_histogramBaseName,"jet_n0"));
  h_jet_netQ   = loadH1(inputFile,createName(_histogramBaseName,"jet_netQ"));
  p_jet_n      = loadProfile(inputFile,createName(_histogramBaseName,"jet_n_p"));
  p_jet_np     = loadProfile(inputFile,createName(_histogramBaseName,"jet_np_p"));
  p_jet_nm     = loadProfile(inputFile,createName(_histogramBaseName,"jet_nm_p"));
  p_jet_n0     = loadProfile(inputFile,createName(_histogramBaseName,"jet_n0_p"));
  p_jet_npSq   = loadProfile(inputFile,createName(_histogramBaseName,"jet_npSq_p"));
  p_jet_nmSq   = loadProfile(inputFile,createName(_histogramBaseName,"jet_nmSq_p"));
  p_jet_n0Sq   = loadProfile(inputFile,createName(_histogramBaseName,"jet_n0Sq_p"));
  p_jet_npnm   = loadProfile(inputFile,createName(_histogramBaseName,"jet_npnm_p"));
  p_jet_npn0   = loadProfile(inputFile,createName(_histogramBaseName,"jet_npn0_p"));
  p_jet_nmn0   = loadProfile(inputFile,createName(_histogramBaseName,"jet_nmn0_p"));
  p_jet_netQ   = loadProfile(inputFile,createName(_histogramBaseName,"jet_netQ_p"));
  p_jet_netQSq = loadProfile(inputFile,createName(_histogramBaseName,"jet_netQSq_p"));
  }

  void JetHistos::fill(PseudoJet& jet)
  {
  double jet_phi = jet.phi();
  double jet_pt  = jet.perp();
  double jet_eta = jet.pseudorapidity();
  h_jet_count->Fill(1.0);
  h_jet_pt->Fill(jet_pt);
  h_jet_phi->Fill(jet_phi);
  h_jet_eta->Fill(jet_eta);
  h_jet_etaPt->Fill(jet_eta, jet_pt);

  double n  = 0;
  double np = 0;
  double nm = 0;
  double n0 = 0;
  double netQ = 0;
  double q;
  int pdgId;

  // Constituents of the passed Jet
  vector<PseudoJet> constituents = jet.constituents();
  n = jet.constituents().size();
  for (const auto & constituent : constituents)
    {
    pdgId = constituent.user_index();
    ParticleType * type = particleTypeList->findPdgCode(pdgId);
    q = type->charge();
    netQ += q;
    if (q>0) np += 1.0;
    else if (q<0) nm += 1.0;
    else n0 += 1.0;
    }

  h_jet_n->Fill(n);
  h_jet_np->Fill(np);
  h_jet_nm->Fill(nm);
  h_jet_n0->Fill(n0);
  h_jet_netQ->Fill(netQ);
  p_jet_n->Fill(jet_pt,n);
  p_jet_np->Fill(jet_pt,np);
  p_jet_nm->Fill(jet_pt,nm);
  p_jet_n0->Fill(jet_pt,n0);
  p_jet_npSq->Fill(jet_pt,np*(np-1.0));
  p_jet_nmSq->Fill(jet_pt,nm*(nm-1.0));
  p_jet_n0Sq->Fill(jet_pt,n0*(n0-1.0));
  p_jet_npnm->Fill(jet_pt,np*nm);
  p_jet_npn0->Fill(jet_pt,nm*n0);
  p_jet_nmn0->Fill(jet_pt,nm*n0);
  p_jet_netQ->Fill(jet_pt,netQ);
  p_jet_netQSq->Fill(jet_pt,netQ*netQ);

  //  printValue("jet_n",n);
  //  printValue("jet_np",np);
  //  printValue("jet_nm",nm);
  //  printValue("jet_n0",n0);
  //  printValue("np*(np-1.0)",np*(np-1.0));
  //  printValue("nm*(nm-1.0)",nm*(nm-1.0));
  //  printValue("n0*(n0-1.0)",n0*(n0-1.0));
  //  printValue("nm*np",nm*np);
  //  printValue("netQ",netQ);
  //  printValue("netQSq",netQ*netQ);

  }

  double JetHistos::acceptedJets() const
  {
  return h_jet_count->GetBinContent(1);
  }

  void JetHistos::scaleHistograms(double scale)
  {
  h_jet_pt->Scale(scale);
  h_jet_phi->Scale(scale);
  h_jet_eta->Scale(scale);
  h_jet_etaPt->Scale(scale);
  h_jet_n->Scale(scale);
  h_jet_np->Scale(scale);
  h_jet_nm->Scale(scale);
  h_jet_n0->Scale(scale);
  h_jet_netQ->Scale(scale);
  }

} // namespace CAP
