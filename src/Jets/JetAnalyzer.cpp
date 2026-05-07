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
#include "JetAnalyzer.hpp"
#include "JetHistos.hpp"
#include "JetSingleHistos.hpp"
#include "JetPairHistos.hpp"
#include "PrintHelpers.hpp"
#include "fastjet/ClusterSequence.hh"
#include "NameHelpers.hpp"
#include "RootHistogramHelpers.hpp"

ClassImp(CAP::JetAnalyzer);

namespace CAP
{

JetAnalyzer::JetAnalyzer()
:
EventProcessorSingle<JetSingleHistos,JetSingleDerivedHistos>(),
ManagedList<JetFilter>()
{
  appendClassName("JetAnalyzer");
  setMinimumReportLevel(Object::kInfo);
  setName("JetAnalyzer");
  setTitle("JetAnalyzer");
}

JetAnalyzer::JetAnalyzer(const JetAnalyzer & source)
:
EventProcessorSingle<JetSingleHistos,JetSingleDerivedHistos>(source),
ManagedList<JetFilter>(source)
{   }

JetAnalyzer & JetAnalyzer::operator=(const JetAnalyzer & rhs)
{
  if (this!=&rhs)
    {
    EventProcessorSingle<JetSingleHistos,JetSingleDerivedHistos>::operator=(rhs);
    ManagedList<JetFilter>::operator=(rhs);
    }
  return *this;
}


void JetAnalyzer::setDefaultConfiguration()
{
  EventProcessorSingle<JetSingleHistos,JetSingleDerivedHistos>::setDefaultConfiguration();
  const String & taskName = name();
  _configuration.addProperty(createKey(taskName,"HistogramBaseName"),"Jet");
  _configuration.addProperty(createKey(taskName,"load",false);
  _configuration.addProperty(createKey(taskName,"exportObject",true);
  _configuration.addProperty(createKey(taskName,"CreateHistograms",true);
  _configuration.addProperty(createKey(taskName,"JetRadius"),      0.4);
  _configuration.addProperty(createKey(taskName,"JetPtMin"),       10.0);
  _configuration.addProperty(createKey(taskName,"nBins_jet_n1"),   100);
  _configuration.addProperty(createKey(taskName,"min_jet_n1"),     0.0);
  _configuration.addProperty(createKey(taskName,"max_jet_n1"),     100.0);
  _configuration.addProperty(createKey(taskName,"nBins_jet_p"),    100);
  _configuration.addProperty(createKey(taskName,"min_jet_p"),      0.0);
  _configuration.addProperty(createKey(taskName,"max_jet_p"),      100.0);
  _configuration.addProperty(createKey(taskName,"nBins_jet_pt"),   100);
  _configuration.addProperty(createKey(taskName,"min_jet_pt"),     0.0);
  _configuration.addProperty(createKey(taskName,"max_jet_pt"),     100.0);
  _configuration.addProperty(createKey(taskName,"nBins_jet_phi"),  32);
  _configuration.addProperty(createKey(taskName,"min_jet_phi"),    0.0);
  _configuration.addProperty(createKey(taskName,"max_jet_phi"),    CAP::MATH::twoPi());
  _configuration.addProperty(createKey(taskName,"nBins_jet_eta"),   20);
  _configuration.addProperty(createKey(taskName,"min_jet_eta"),     -2.0);
  _configuration.addProperty(createKey(taskName,"max_jet_eta"),     2.0);
  _configuration.addProperty(createKey(taskName,"nBins_jet_netQ"),  20);
  _configuration.addProperty(createKey(taskName,"min_jet_netQ"),    -10.0);
  _configuration.addProperty(createKey(taskName,"max_jet_netQ"),    10.0);
  _configuration.addProperty(createKey(taskName,"nBins_p"),    100);
  _configuration.addProperty(createKey(taskName,"min_p"),      0.0);
  _configuration.addProperty(createKey(taskName,"max_p"),      10.0);
  _configuration.addProperty(createKey(taskName,"pt_nbins"),   100);
  _configuration.addProperty(createKey(taskName,"pt_min"),     0.0);
  _configuration.addProperty(createKey(taskName,"pt_max"),     10.0);
  _configuration.addProperty(createKey(taskName,"phi_nbins"),  36);
  _configuration.addProperty(createKey(taskName,"phi_min"),    0.0);
  _configuration.addProperty(createKey(taskName,"phi_max"),    CAP::MATH::twoPi());
  _configuration.addProperty(createKey(taskName,"eta_nbins"),  40);
  _configuration.addProperty(createKey(taskName,"eta_min"),    -2.0);
  _configuration.addProperty(createKey(taskName,"eta_max"),    2.0);
  _configuration.addProperty(createKey(taskName,"jt_nbins"),   50);
  _configuration.addProperty(createKey(taskName,"jt_min"),     0.0);
  _configuration.addProperty(createKey(taskName,"jt_max"),     5.0);
  _configuration.addProperty(createKey(taskName,"th_nbins"),   20);
  _configuration.addProperty(createKey(taskName,"th_min"),     0.0);
  _configuration.addProperty(createKey(taskName,"th_max"),     CAP::MATH::pi()/4.0);
  _configuration.addProperty(createKey(taskName,"z_nbins"),   50);
  _configuration.addProperty(createKey(taskName,"z_min"),     0.0);
  _configuration.addProperty(createKey(taskName,"z_max"),     1.0);
}

void JetAnalyzer::initialize()
{
  jetFilters = ManagedList<JetFilter>::getObjects();
  nJetFilters() = jetFilters.size();
  Analyzer::initialize();
}

void JetAnalyzer::create(const Configuration & configuration)
{
  String bn = configuration.valueString(createKey(taskName,"HistogramBaseName"));
  jetRadius = configuration.valueDouble(createKey(taskName,"JetRadius"));
  jetPtMin  = configuration.valueDouble(createKey(taskName,"JetPtMin"));
  JetHistos * jetHistos;
  JetSingleHistos * jetSingleHistos;
  JetPairHistos * jetPairHistos;

  for (auto & eventFilter : eventFilters)
    {
    String efn = eventFilter->name();
    for (auto & jetFilter : jetFilters)
      {
      String jfn = jetFilter->name();

      jetHistos = new JetHistos();
      jetHistos->setName(createName(_histogramBaseName,efn,jfn));
      jetHistos->setParentTask(this);
      jetHistos->create(const Configuration & configuration);
      addList(jetHistos);

      jetSingleHistos = new JetSingleHistos();
      jetSingleHistos->setName(createName(_histogramBaseName,efn,jfn));
      jetSingleHistos->setParentTask(this);
      jetSingleHistos->create(const Configuration & configuration);
      addList(jetSingleHistos);

      jetPairHistos = new JetPairHistos();
      jetPairHistos->setName(createName(_histogramBaseName,efn,jfn));
      jetPairHistos->setParentTask(this);
      jetPairHistos->create(const Configuration & configuration);
      addList(jetPairHistos);
      }
    }
}

void JetAnalyzer::execute()
{
  Event & theEvent = event();
  if (!filterEvent(theEvent)) return;
  if (! filterParticles(theEvent.particles()))  return;
  std::vector<PseudoJet> pseudoJetsInput;
  pseudoJetsInput.clear();

  for (auto accepted : _acceptedEventFilters)
    {
    if (!accepted) continue;
    unsigned int  baseSingle   = iEventFilter*nParticleFilters();
    for (unsigned int iParticleFilter=0; iParticleFilter<nParticleFilters();iParticleFilter++)
      {
      ParticleSingleHistos * histosSingle = (ParticleSingleHistos *) listAt(baseSingle+iParticleFilter);
      std::vector<Particle*> & particlesSelected = acceptedParticles(iParticleFilter);


  for (auto & particle : acceptedParticles) // acceptedParticles is set in  filterParticles(theEvent.particles())
    {
    int pid = particle->type().pdgCode();
    VectorLorentz & momentum = particle->momentum();
//    printValue("k",k);
//    printValue("px",momentum.x());
//    printValue("py",momentum.y());
//    printValue("pz",momentum.z());
//    printValue("e",momentum.t());
//    printValue("pid",pid);
    PseudoJet pseudoJet (momentum.x(),momentum.y(),momentum.z(),momentum.t());
    pseudoJet.set_user_index(pid);
    pseudoJetsInput.push_back(pseudoJet);
    //k++;
    }
//  printValue("jetRadius",jetRadius);
//  printValue("jetPtMin",jetPtMin);

  JetDefinition jetDef(antikt_algorithm,jetRadius);
  ClusterSequence * clusterSequence = new ClusterSequence(pseudoJetsInput, jetDef);
  std::vector<PseudoJet> clusteredJets = sorted_by_pt( clusterSequence->inclusive_jets(jetPtMin) );
//printJets (clusteredJets);
//  printValue("jetRadius",jetRadius);
//  printValue("clusteredJets.size()",clusteredJets.size());
  unsigned int iEventFilter = 0;
  for (auto accepted : _acceptedEventFilters)
    {
    if (!accepted)
      {
      printString("EVENT NOT ACCEPTED");
      continue;
      }
    unsigned int  baseSingle   = iEventFilter*nJetFilters();
    for (unsigned int iJetFilter=0; iJetFilter<nJetFilters();iJetFilter++)
      {
      int index = baseSingle+iJetFilter;
      JetHistos & jetHistos = (JetHistos & ) listAt(index);
      JetSingleHistos & jetSingleHistos = (JetSingleHistos & ) listAt(1,index);
      JetPairHistos & jetPairHistos     = (JetPairHistos & ) listAt(2,index);
      for (auto jet : clusteredJets)
        {
        if (jetFilters[iJetFilter]->accept(jet))
          {
          jetHistos.fill(jet);
          jetSingleHistos.fill(jet);
          jetPairHistos.fill(jet);
          }
        }
      }
    iEventFilter++;
    }
  _taskExecuted.increment();;
  delete clusterSequence;
}

//!
//!Jet histograms are scaled by the number of jets accepted - instead of the number of events accepted.
//!
void JetAnalyzer::scaleHistograms()
{
  if (reportStart(__FUNCTION__)) { /* no ops */ };

  for (int iEventFilter=0; iEventFilter<nEventFilters(); iEventFilter++)
  {
    unsigned int  baseSingle   = iEventFilter*nJetFilters();
    for (int iJetFilter=0; iJetFilter<nJetFilters(); iJetFilter++)
    {
      int index = baseSingle+iJetFilter;
      JetHistos & jetHistos = (JetHistos & ) listAt(index);
      double jetAccepted = jetHistos.acceptedJets();
      double scale = 1.0;
      if (jetAccepted>1)
        scale = 1.0/jetAccepted;
      jetHistos.scaleHistograms(scale);
      JetSingleHistos & jetSingleHistos = (JetSingleHistos & ) listAt(1,index);
      jetSingleHistos.scaleHistograms(scale);
      JetPairHistos & jetPairHistos     = (JetPairHistos & ) listAt(2,index);
      jetPairHistos.scaleHistograms(scale);
    }
  }
  if (reportEnd(__FUNCTION__)) {/* no ops */};
}

//void JetAnalyzer::printJets (const vector<fastjet::PseudoJet> & jets)
//{
//  // sort jets into increasing pt
//  vector<fastjet::PseudoJet> sorted_jets = sorted_by_pt(jets);
//
//  // label the columns
//  printf("%5s %15s %15s %15s %15s\n","jet #", "rapidity",
//         "phi", "pt", "n constituents");
//
//  // print out the details for each jet
//  for (unsigned int i = 0; i < sorted_jets.size(); i++)
//    {
//    // the following is not super efficient since it creates an
//    // intermediate constituents vector
//    int n_constituents = sorted_jets[i].constituents().size();
//    printf("%5u %15.8f %15.8f %15.8f %8u\n",
//           i, sorted_jets[i].rap(), sorted_jets[i].phi(),
//           sorted_jets[i].perp(), n_constituents);
//  }
//}


    //!
//! Loop on particle filters to setup the jet finder.
    //!
    bool JetAnalyzer::filterJets(std::vector< std::vector<Particle*> > acceptedParticles)
      {
      bool analyzeEvent = false;
      for (auto & particles : acceptedParticles)
        {
        bool useThisFilter = false;
        for (auto & particle : particles)
          {
          int pid = particle->type().pdgCode();
          VectorLorentz & momentum = particle->momentum();
          PseudoJet pseudoJet (momentum.x(),momentum.y(),momentum.z(),momentum.t());
          pseudoJet.set_user_index(pid);
          pseudoJetsInput.push_back(pseudoJet);
          }
          acceptedParticleFilters[iParticleFilter] = useThisFilter;
        if (useThisFilter) analyzeEvent = true;
        iParticleFilter++;
        }
      return analyzeEvent;
      }









} // namespace CAP
