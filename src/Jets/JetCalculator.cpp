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
#include "JetCalculator.hpp"
#include "JetHistos.hpp"
#include "JetSingleHistos.hpp"
#include "JetPairHistos.hpp"
#include "JetHistosDerived.hpp"
#include "JetSingleHistosDerived.hpp"
#include "JetPairHistosDerived.hpp"
#include "fastjet/ClusterSequence.hh"
#include "NameHelpers.hpp"
#include "PrintHelpers.hpp"

ClassImp(CAP::JetCalculator);

namespace CAP
{

JetCalculator::JetCalculator()
:
Calculator(),
ManagedList<JetFilter>()
{
 appendClassName("JetCalculator");
 EventProcessorSingle<JetSingleHistos,JetSingleDerivedHistos>setName("JetCalculator");
  EventProcessorSingle<JetSingleHistos,JetSingleDerivedHistos>setTitle("JetCalculator");
}

JetCalculator::JetCalculator(const JetCalculator & source)
:
Calculator(source),
ManagedList<JetFilter>(source)
{   }

JetCalculator & JetCalculator::operator=(const JetCalculator & rhs)
{
  if (this!=&rhs)
    {
    Calculator::operator=(rhs);
    ManagedList<JetFilter>::operator=(rhs);
    }
  return *this;
}


void JetCalculator::setDefaultConfiguration()
{
  Task::setDefaultConfiguration();
  addProperty("HistogramBaseName","Jet");
  addProperty("load",true);
  addProperty("exportObject",true);
  addProperty("CreateHistograms",true);

  addProperty("JetRadius",      0.4);
  addProperty("JetPtMin",       10.0);
  addProperty("nBins_jet_n1",   100);
  addProperty("min_jet_n1",     0.0);
  addProperty("max_jet_n1",     100.0);
  addProperty("nBins_jet_p",    100);
  addProperty("min_jet_p",      0.0);
  addProperty("max_jet_p",      100.0);
  addProperty("nBins_jet_pt",   100);
  addProperty("min_jet_pt",     0.0);
  addProperty("max_jet_pt",     100.0);
  addProperty("nBins_jet_phi",  32);
  addProperty("min_jet_phi",    0.0);
  addProperty("max_jet_phi",    CAP::MATH::twoPi());
  addProperty("nBins_jet_eta",   20);
  addProperty("min_jet_eta",     -2.0);
  addProperty("max_jet_eta",     2.0);
  addProperty("nBins_jet_netQ",  20);
  addProperty("min_jet_netQ",    -10.0);
  addProperty("max_jet_netQ",    10.0);
  addProperty("nBins_p",    100);
  addProperty("min_p",      0.0);
  addProperty("max_p",      10.0);
  addProperty("pt_nbins",   100);
  addProperty("pt_min",     0.0);
  addProperty("pt_max",     10.0);
  addProperty("phi_nbins",  36);
  addProperty("phi_min",    0.0);
  addProperty("phi_max",    CAP::MATH::twoPi());
  addProperty("eta_nbins",  40);
  addProperty("eta_min",    -2.0);
  addProperty("eta_max",    2.0);
  addProperty("jt_nbins",   50);
  addProperty("jt_min",     0.0);
  addProperty("jt_max",     5.0);
  addProperty("th_nbins",   20);
  addProperty("th_min",     0.0);
  addProperty("th_max",     CAP::MATH::pi()/4.0);
  addProperty("z_nbins",   50);
  addProperty("z_min",     0.0);
  addProperty("z_max",     1.0);
}

void JetCalculator::loadFrom(TFile & inputFile,const Configuration & configuration)
{
  String bn = valueString("HistogramBaseName");
  JetHistos        * jetHistos;
  JetSingleHistos  * jetSingleHistos;
  JetPairHistos    * jetPairHistos;

  for (auto & eventFilter : eventFilters)
    {
    String efn = eventFilter->name();
    for (auto & particleFilter : particleFilters)
      {
      String pfn = particleFilter->name();
      for (auto & jetFilter : jetFilters)
        {
        String jfn = jetFilter->name();

        jetHistos = new JetHistos();
        jetHistos->setName(createName(_histogramBaseName,efn,pfn,jfn));
        jetHistos->setParentTask(this);
        jetHistos->loadFrom(inputFile);
        addList(jetHistos);

        jetSingleHistos = new JetSingleHistos();
        jetSingleHistos->setName(createName(_histogramBaseName,efn,pfn,jfn));
        jetSingleHistos->setParentTask(this);
        jetSingleHistos->loadFrom(inputFile);
        addList(jetSingleHistos);

        jetPairHistos = new JetPairHistos();
        jetPairHistos->setName(createName(_histogramBaseName,efn,pfn,jfn));
        jetPairHistos->setParentTask(this);
        jetPairHistos->loadFrom(inputFile);
        addList(jetPairHistos);
        }
      }
    }

}

void JetCalculator::create(const Configuration & configuration)
{
  String bn = valueString("HistogramBaseName");
  JetHistosDerived * jetHistosDerived;
  JetSingleHistosDerived * jetSingleHistosDerived;
  JetPairHistosDerived   * jetPairHistosDerived;

  for (auto & eventFilter : eventFilters)
    {
    String efn = eventFilter->name();
    for (auto & particleFilter : particleFilters)
      {
      String pfn = eventFilter->name();

      for (auto & jetFilter : jetFilters)
        {
        String jfn = jetFilter->name();

        jetHistosDerived = new JetHistosDerived();
        jetHistosDerived->setName(createName(_histogramBaseName,efn,pfn,jfn));
        jetHistosDerived->setParentTask(this);
        jetHistosDerived->create(const Configuration & configuration);
        addList(jetHistosDerived);

        jetSingleHistosDerived = new JetSingleHistosDerived();
        jetSingleHistosDerived->setName(createName(_histogramBaseName,efn,pfn,jfn));
        jetSingleHistosDerived->setParentTask(this);
        jetSingleHistosDerived->create(const Configuration & configuration);
        addList(jetSingleHistosDerived);

        jetPairHistosDerived = new JetPairHistosDerived();
        jetPairHistosDerived->setName(createName(_histogramBaseName,efn,pfn,jfn));
        jetPairHistosDerived->setParentTask(this);
        jetPairHistosDerived->create(const Configuration & configuration);
        addList(jetPairHistosDerived);
        }
      }
    }
}

void JetCalculator::execute()
{
  int index = 0;
  for (unsigned int iEventFilter=0;iEventFilter<eventFilters.size();iEventFilter++)
    {
    for (unsigned int iParticleFilter=0;iParticleFilter<particleFilters.size();iParticleFilter++)
      {
      for (unsigned int iJetFilter=0;iJetFilter<jetFilters.size();iJetFilter++)
        {
        JetHistos * jetHistos = static_cast<JetHistos*>(listAt(index));
        JetHistosDerived * jetHistosDerived = static_cast<JetHistosDerived*>(listAt(index));
        jetHistosDerived->calculateDerivedHistograms(*jetHistos);

        JetSingleHistos * jetSingleHistos = static_cast<JetSingleHistos*>(listAt(index));
        JetSingleHistosDerived * jetSingleHistosDerived = static_cast<JetSingleHistosDerived*>(listAt(index));
        jetSingleHistosDerived->calculateDerivedHistograms(*jetSingleHistos);

        JetPairHistos * jetPairHistos = static_cast<JetPairHistos*>(listAt(index));
        JetPairHistosDerived * jetPairHistosDerived = static_cast<JetPairHistosDerived*>(listAt(index));
        jetPairHistosDerived->calculateDerivedHistograms(*jetSingleHistos,*jetPairHistos);
        index++;
        }
      }
    }
}

void JetCalculator::scaleHistograms()
  {
  // no ops....
  }



} // namespace CAP
