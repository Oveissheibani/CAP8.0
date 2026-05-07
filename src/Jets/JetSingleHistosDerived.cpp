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
#include "JetSingleHistosDerived.hpp"
#include "JetHelpers.hpp"
#include "NameHelpers.hpp"
#include "PrintHelpers.hpp"
#include "RootHistogramHelpers.hpp"
#include "fastjet/ClusterSequence.hh"
#include "Configuration.hpp"
#include "Task.hpp"

ClassImp(CAP::JetSingleHistosDerived);

namespace CAP
{

JetSingleHistosDerived::JetSingleHistosDerived()
:
JetSingleHistos(),
h_jet_n1d_pt(nullptr), // differences
h_jet_n1d_phi(nullptr),
h_jet_n1d_eta(nullptr),
h_jet_n1d_th(nullptr),
h_jet_n1d_jt(nullptr),
h_jet_n1d_z(nullptr),
h_jet_n1r_pt(nullptr), // ratios
h_jet_n1r_phi(nullptr),
h_jet_n1r_eta(nullptr),
h_jet_n1r_th(nullptr),
h_jet_n1r_jt(nullptr),
h_jet_n1r_z(nullptr),
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

JetSingleHistosDerived::JetSingleHistosDerived(const JetSingleHistosDerived & source)
:
JetSingleHistos(group),
h_jet_n1d_pt(nullptr),
h_jet_n1d_phi(nullptr),
h_jet_n1d_eta(nullptr),
h_jet_n1d_th(nullptr),
h_jet_n1d_jt(nullptr),
h_jet_n1d_z(nullptr),
h_jet_n1r_pt(nullptr), // ratios
h_jet_n1r_phi(nullptr),
h_jet_n1r_eta(nullptr),
h_jet_n1r_th(nullptr),
h_jet_n1r_jt(nullptr),
h_jet_n1r_z(nullptr),
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
{  }

JetSingleHistosDerived & JetSingleHistosDerived::operator=(const JetSingleHistosDerived & rhs)
{
  if (this!=&rhs)
  {
  JetSingleHistos::operator=(rhs);
  cloneD(rhs);
  pt_nbins    = rhs.pt_nbins;
  phi_nbins   = rhs.phi_nbins;
  eta_nbins   = rhs.eta_nbins;
  jt_nbins    = rhs.jt_nbins;
  th_nbins    = rhs.th_nbins;
  z_nbins     = rhs.z_nbins;
  pt_min      = rhs.pt_min;
  pt_max      = rhs.pt_max;
  phi_min     = rhs.phi_min;
  phi_max     = rhs.phi_max;
  eta_min     = rhs.eta_min;
  eta_max     = rhs.eta_max;
  jt_min      = rhs.jt_min;
  jt_max      = rhs.jt_max;
  th_min      = rhs.th_min;
  th_max      = rhs.th_max;
  z_min       = rhs.z_min;
  z_max       = rhs.z_max;
  }
  return *this;
}

void JetSingleHistosDerived::cloneD(const JetSingleHistosDerived & source)
{
  h_jet_n1d_pt  = safeCloneH1(source.h_jet_n1d_pt);
  h_jet_n1d_phi = safeCloneH1(source.h_jet_n1d_phi);
  h_jet_n1d_eta = safeCloneH1(source.h_jet_n1d_eta);
  h_jet_n1d_th  = safeCloneH1(source.h_jet_n1d_th);
  h_jet_n1d_jt  = safeCloneH1(source.h_jet_n1d_jt);
  h_jet_n1d_z   = safeCloneH1(source.h_jet_n1d_z);

  h_jet_n1r_pt  = safeCloneH1(source.h_jet_n1r_pt);
  h_jet_n1r_phi = safeCloneH1(source.h_jet_n1r_phi);
  h_jet_n1r_eta = safeCloneH1(source.h_jet_n1r_eta);
  h_jet_n1r_th  = safeCloneH1(source.h_jet_n1r_th);
  h_jet_n1r_jt  = safeCloneH1(source.h_jet_n1r_jt);
  h_jet_n1r_z   = safeCloneH1(source.h_jet_n1r_z);
}

void JetSingleHistosDerived::configure(const String & taskName,
                                             const String & objectType,
                                             const Configuration & configuration,
                                             unsigned int index)
{
  JetSingleHistos::configure(taskName,objectType,configuration,index);
  pt_nbins   = configuration.valueInt(createKey(taskName,objectType,index,"pt_nbins"));
  pt_min     = configuration.valueDouble(createKey(taskName,objectType,index,"pt_min"));
  pt_max     = configuration.valueDouble(createKey(taskName,objectType,index,"pt_max"));
  phi_nbins  = configuration.valueInt(createKey(taskName,objectType,index,"phi_nbins"));
  phi_min    = configuration.valueDouble(createKey(taskName,objectType,index,"phi_min"));
  phi_max    = configuration.valueDouble(createKey(taskName,objectType,index,"phi_max"));
  eta_nbins  = configuration.valueInt(createKey(taskName,objectType,index,"eta_nbins"));
  eta_min    = configuration.valueDouble(createKey(taskName,objectType,index,"eta_min"));
  eta_max    = configuration.valueDouble(createKey(taskName,objectType,index,"eta_max"));
  jt_nbins   = configuration.valueInt(createKey(taskName,objectType,index,"jt_nbins"));
  jt_min     = configuration.valueDouble(createKey(taskName,objectType,index,"jt_min"));
  jt_max     = configuration.valueDouble(createKey(taskName,objectType,index,"jt_max"));
  th_nbins   = configuration.valueInt(createKey(taskName,objectType,index,"th_nbins"));
  th_min     = configuration.valueDouble(createKey(taskName,objectType,index,"th_min"));
  th_max     = configuration.valueDouble(createKey(taskName,objectType,index,"th_max"));
  z_nbins    = configuration.valueInt(createKey(taskName,objectType,index,"z_nbins"));
  z_min      = configuration.valueDouble(createKey(taskName,objectType,index,"z_min"));
  z_max      = configuration.valueDouble(createKey(taskName,objectType,index,"z_max"));

  if (reportDebug(__FUNCTION__))
    {
    printCR();
    printValue("Histo Base Name",bn);
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


void JetSingleHistosDerived::create()
{
  if (reportDebug(__FUNCTION__)) { printCR();}
  // difference (+)-(-)
  h_jet_n1d_pt    = createHistogram(createName(_histogramBaseName,"jet_n1d_pt"),    pt_nbins,    pt_min,    pt_max,     "pt","N_1^{+}-N_1^{+}");
  h_jet_n1d_phi   = createHistogram(createName(_histogramBaseName,"jet_n1d_phi"),   phi_nbins,   phi_min,   phi_max,    "phi","N_1^{+}-N_1^{+}");
  h_jet_n1d_eta   = createHistogram(createName(_histogramBaseName,"jet_n1d_eta"),   eta_nbins,   eta_min,   eta_max,    "#eta","N_1^{+}-N_1^{+}");
  h_jet_n1d_th    = createHistogram(createName(_histogramBaseName,"jet_n1d_th"),    th_nbins,    th_min,    th_max,     "#theta","N_1^{+}-N_1^{+}");
  h_jet_n1d_jt    = createHistogram(createName(_histogramBaseName,"jet_n1d_jt"),    jt_nbins,    jt_min,    jt_max,     "j_{t}","N_1^{+}-N_1^{+}");
  h_jet_n1d_z     = createHistogram(createName(_histogramBaseName,"jet_n1d_z"),     z_nbins,     z_min,     z_max,      "p_{t}","N_1^{+}-N_1^{+}");

  // ratios (+)/(-)
  h_jet_n1r_pt    = createHistogram(createName(_histogramBaseName,"jet_n1r_pt"),    pt_nbins,    pt_min,    pt_max,     "pt","N_1^{+}/N_1^{+}");
  h_jet_n1r_phi   = createHistogram(createName(_histogramBaseName,"jet_n1r_phi"),   phi_nbins,   phi_min,   phi_max,    "phi","N_1^{+}/N_1^{+}");
  h_jet_n1r_eta   = createHistogram(createName(_histogramBaseName,"jet_n1r_eta"),   eta_nbins,   eta_min,   eta_max,    "#eta","N_1^{+}/N_1^{+}");
  h_jet_n1r_th    = createHistogram(createName(_histogramBaseName,"jet_n1r_th"),    th_nbins,    th_min,    th_max,     "#theta","N_1^{+}/N_1^{+}");
  h_jet_n1r_jt    = createHistogram(createName(_histogramBaseName,"jet_n1r_jt"),    jt_nbins,    jt_min,    jt_max,     "j_{t}","N_1^{+}/N_1^{+}");
  h_jet_n1r_z     = createHistogram(createName(_histogramBaseName,"jet_n1r_z"),     z_nbins,     z_min,     z_max,      "p_{t}","N_1^{+}/N_1^{+}");

}

void JetSingleHistosDerived::loadFrom(TFile & inputFile)
{
  if (reportDebug(__FUNCTION__)) { printCR();}
  JetSingleHistos::loadFrom(inputFile,taskName,objectType,configuration,index);
  h_jet_n1d_pt    = loadH1(inputFile,createName(_histogramBaseName,"jet_n1d_pt"));
  h_jet_n1d_phi   = loadH1(inputFile,createName(_histogramBaseName,"jet_n1d_phi"));
  h_jet_n1d_eta   = loadH1(inputFile,createName(_histogramBaseName,"jet_n1d_eta"));
  h_jet_n1d_th    = loadH1(inputFile,createName(_histogramBaseName,"jet_n1d_th"));
  h_jet_n1d_jt    = loadH1(inputFile,createName(_histogramBaseName,"jet_n1d_jt"));
  h_jet_n1d_z     = loadH1(inputFile,createName(_histogramBaseName,"jet_n1d_z"));

  h_jet_n1r_pt    = loadH1(inputFile,createName(_histogramBaseName,"jet_n1r_pt"));
  h_jet_n1r_phi   = loadH1(inputFile,createName(_histogramBaseName,"jet_n1r_phi"));
  h_jet_n1r_eta   = loadH1(inputFile,createName(_histogramBaseName,"jet_n1r_eta"));
  h_jet_n1r_th    = loadH1(inputFile,createName(_histogramBaseName,"jet_n1r_th"));
  h_jet_n1r_jt    = loadH1(inputFile,createName(_histogramBaseName,"jet_n1r_jt"));
  h_jet_n1r_z     = loadH1(inputFile,createName(_histogramBaseName,"jet_n1r_z"));
}

void JetSingleHistosDerived::calculateDerivedHistograms(const JetSingleHistos & s)
{
  h_jet_n1d_pt   ->Add(s.h_jet_n1p_pt,   s.h_jet_n1m_pt, 1.0, -10.0);
  h_jet_n1d_phi  ->Add(s.h_jet_n1p_phi,  s.h_jet_n1m_phi,1.0, -10.0);
  h_jet_n1d_eta  ->Add(s.h_jet_n1p_eta,  s.h_jet_n1m_eta,1.0, -10.0);
  h_jet_n1d_th   ->Add(s.h_jet_n1p_th,   s.h_jet_n1m_th, 1.0, -10.0);
  h_jet_n1d_jt   ->Add(s.h_jet_n1p_jt,   s.h_jet_n1m_jt, 1.0, -10.0);
  h_jet_n1d_z    ->Add(s.h_jet_n1p_z,    s.h_jet_n1m_z,  1.0, -10.0);

  // ratios (+)/(-)
  h_jet_n1r_pt  ->Add(s.h_jet_n1p_pt);  h_jet_n1r_pt  ->Divide(s.h_jet_n1m_pt);
  h_jet_n1r_phi ->Add(s.h_jet_n1p_phi); h_jet_n1r_phi ->Divide(s.h_jet_n1m_phi);
  h_jet_n1r_eta ->Add(s.h_jet_n1p_eta); h_jet_n1r_eta ->Divide(s.h_jet_n1m_eta);
  h_jet_n1r_th  ->Add(s.h_jet_n1p_th);  h_jet_n1r_th  ->Divide(s.h_jet_n1m_th);
  h_jet_n1r_jt  ->Add(s.h_jet_n1p_jt);  h_jet_n1r_jt  ->Divide(s.h_jet_n1m_jt);
  h_jet_n1r_z   ->Add(s.h_jet_n1p_z);   h_jet_n1r_z   ->Divide(s.h_jet_n1m_z);
}


} // namespace CAP





