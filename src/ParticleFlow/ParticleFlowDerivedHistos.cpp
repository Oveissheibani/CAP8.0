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
#include "ParticleFlowDerivedHistos.hpp"
#include "PrintHelpers.hpp"
#include "RootHistogramHelpers.hpp"
#include "Task.hpp"
#include "Configuration.hpp"
#include "NameHelpers.hpp"

ClassImp(CAP::ParticleFlowDerivedHistos);

namespace CAP
{

ParticleFlowDerivedHistos::ParticleFlowDerivedHistos()
:
HistogramGroup(),
eta_fill(false),
rapidity_fill(false),
p2_fill(false),
n1_nbins(0),
n1_min(0.0),
n1_max(0.0),
pt_nbins(0),
pt_min(0.0),
pt_max(0.0),
pt_scale(0.0),
phi_nbins(0),
phi_min(0.0),
phi_max(0.0),
phi_scale(0.0),
eta_nbins(0),
eta_min(0.0),
eta_max(0.0),
eta_range(0.0),
rapidity_nbins(0),
rapidity_minimum(0.0),
rapidity_maximum(0.0),
range_y(0.0),
h_n1_phi(nullptr),
h_n1_eta(nullptr),
h_n1_y(nullptr),
h_spt_phi(nullptr),
h_spt_eta(nullptr),
h_spt_y(nullptr),
h_avg_pt_phi(nullptr),
h_avg_pt_eta(nullptr),
h_avg_pt_phiEta(nullptr),
h_avg_pt_y(nullptr),
h_avg_pt_phiY(nullptr)
{
  appendClassName("ParticleFlowDerivedHistos");
  setName("ParticleFlowDerivedHistos");
  setTitle("ParticleFlowDerivedHistos");
}

ParticleFlowDerivedHistos::ParticleFlowDerivedHistos(const ParticleFlowDerivedHistos & group)
:
HistogramGroup(group),
eta_fill(group.eta_fill ),
rapidity_fill(group.rapidity_fill ),
p2_fill(group.p2_fill ),
n1_nbins(group.n1_nbins ),
n1_min(group.n1_min ),
n1_max(group.n1_max ),
pt_nbins(group.pt_nbins ),
pt_min(group.pt_min ),
pt_max(group.pt_max ),
pt_scale(group.pt_scale ),
phi_nbins(group.phi_nbins ),
phi_min(group.phi_min ),
phi_max(group.phi_max ),
phi_scale(group.phi_scale ),
eta_nbins(group.eta_nbins ),
eta_min(group.eta_min ),
eta_max(group.eta_max ),
eta_range(group.eta_range ),
rapidity_nbins(group.rapidity_nbins ),
rapidity_minimum(group.rapidity_minimum ),
rapidity_maximum(group.rapidity_maximum ),
range_y(group.range_y ),
h_n1_phi(nullptr),
h_n1_eta(nullptr),
h_n1_y(nullptr),
h_spt_phi(nullptr),
h_spt_eta(nullptr),
h_spt_y(nullptr),
h_avg_pt_phi(nullptr),
h_avg_pt_eta(nullptr),
h_avg_pt_phiEta(nullptr),
h_avg_pt_y(nullptr),
h_avg_pt_phiY(nullptr)
{
  cloneAll(group);
}

ParticleFlowDerivedHistos & ParticleFlowDerivedHistos::operator=(const ParticleFlowDerivedHistos & group)
{
  if (this!=&group)
    {
    HistogramGroup::operator=(group);
    eta_fill   = group.eta_fill;
    rapidity_fill     = group.rapidity_fill;
    p2_fill    = group.p2_fill;
    n1_nbins  = group.n1_nbins;
    n1_min    = group.n1_min;
    n1_max    = group.n1_max;
    pt_nbins  = group.pt_nbins;
    pt_min    = group.pt_min;
    pt_max    = group.pt_max;
    pt_scale  = group.pt_scale;
    phi_nbins = group.phi_nbins;
    phi_min   = group.phi_min;
    phi_max   = group.phi_max;
    phi_scale = group.phi_scale;
    eta_nbins = group.eta_nbins;
    eta_min   = group.eta_min;
    eta_max   = group.eta_max;
    eta_range = group.eta_range;
    rapidity_nbins   = group.rapidity_nbins;
    rapidity_minimum     = group.rapidity_minimum;
    rapidity_maximum     = group.rapidity_maximum;
    range_y   = group.range_y;
    cloneAll(group);
    }
  return *this;
}

void ParticleFlowDerivedHistos::cloneAll(const ParticleFlowDerivedHistos & group)
{
  // only clone those that exist...
  if (group.h_n1_phi) h_n1_phi       = (TH1*) group.h_n1_phi->Clone();
  if (group.h_n1_eta) h_n1_eta       = (TH1*) group.h_n1_eta->Clone();
  if (group.h_n1_y) h_n1_y           = (TH1*) group.h_n1_y->Clone();
  if (group.h_spt_phi) h_spt_phi     = (TH1*) group.h_spt_phi->Clone();
  if (group.h_spt_eta) h_spt_eta     = (TH1*) group.h_spt_eta->Clone();
  if (group.h_spt_y) h_spt_y         = (TH1*) group.h_spt_y->Clone();
  if (group.h_avg_pt_phi) h_avg_pt_phi       = (TH1*) group.h_avg_pt_phi->Clone();
  if (group.h_avg_pt_eta) h_avg_pt_eta       = (TH1*) group.h_avg_pt_eta->Clone();
  if (group.h_avg_pt_phiEta) h_avg_pt_phiEta = (TH2*) group.h_avg_pt_phiEta->Clone();
  if (group.h_avg_pt_y) h_avg_pt_y           = (TH1*) group.h_avg_pt_y->Clone();
  if (group.h_avg_pt_phiY) h_avg_pt_phiY     = (TH2*) group.h_avg_pt_phiY->Clone();
}


void ParticleFlowDerivedHistos::create()
{
  if ( reportStart(__FUNCTION__))  {/* no ops */};
}

void ParticleFlowDerivedHistos::loadFrom(TFile & inputFile)
{
}

//!
//! Calculates derived histograms. This is currently limited to pt averages vs eta and phi.
//!
void ParticleFlowDerivedHistos::calculateDerivedHistograms(ParticleFlowHistos & baseHistos)
{
  String bn = parentName();
  TH1* hTemp;

  if (baseHistos.h_n1_phiEta)
    {
    if (reportDebug(__FUNCTION__)) cout << "Compute h_n1_phi" << endl;
    hTemp = baseHistos.h_n1_phiEta->ProjectionY();
    h_n1_phi->Add(hTemp);
    delete hTemp;

    if (reportDebug(__FUNCTION__)) cout << "Compute h_n1_eta" << endl;
    hTemp = baseHistos.h_n1_phiEta->ProjectionX();
    h_n1_eta->Add(hTemp);
    delete hTemp;

    if (p2_fill)
      {

      hTemp = baseHistos.h_spt_phiEta->ProjectionY();
      h_spt_phi->Add(hTemp);
      delete hTemp;


      calculateInclusiveAvg(h_spt_phi, h_n1_phi, h_avg_pt_phi);


      hTemp = baseHistos.h_spt_phiEta->ProjectionX();
      h_spt_eta->Add(hTemp);
      delete hTemp;


      calculateInclusiveAvg(h_spt_eta, h_n1_eta, h_avg_pt_eta);
      }
    }

  if (baseHistos.h_n1_phiY)
    {

    hTemp = baseHistos.h_n1_phiY->ProjectionY();
    h_n1_phi->Reset();
    h_n1_phi->Add(hTemp);
    delete hTemp;


    hTemp = baseHistos.h_n1_phiY->ProjectionX();
    h_n1_y->Add(hTemp);
    delete hTemp;

    if (p2_fill)
      {
      if (baseHistos.h_spt_phiY)
        {

        hTemp = baseHistos.h_spt_phiY->ProjectionY();
        h_spt_phi->Add(hTemp);
        delete hTemp;


        calculateInclusiveAvg(h_spt_phi, h_n1_phi, h_avg_pt_phi);
        }


      hTemp = baseHistos.h_spt_phiY->ProjectionX();
      h_spt_y->Add(hTemp);
      delete hTemp;


      calculateInclusiveAvg(h_spt_y, h_n1_y, h_avg_pt_y);
      }
    }

}

} // namespace cap

