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
#include "ParticlePair3DHistos.hpp"
#include "MathConstants.hpp"
#include "RootHistogramHelpers.hpp"
#include "NameHelpers.hpp"
#include "PrintHelpers.hpp"
#include "Task.hpp"
#include "Configuration.hpp"
#include <cmath>      // std::isfinite — defensive guards for BF-per-stage

ClassImp(CAP::ParticlePair3DHistos);

namespace CAP
{

  ParticlePair3DHistos::ParticlePair3DHistos()
  :
  HistogramGroup(),
  n2_nbins(0),
  Qinv_nbins(0),
  DeltaPs_nbins(0),
  DeltaPo_nbins(0),
  DeltaPl_nbins(0),
  pt_nbins(0),
  rapidity_nbins(0),
  n2_min(0),
  n2_max(0),
  Qinv_min(0),
  Qinv_max(0),
  DeltaPs_min(0),
  DeltaPs_max(0),
  DeltaPo_min(0),
  DeltaPo_max(0),
  DeltaPl_min(0),
  DeltaPl_max(0),
  pt_min(0),
  pt_max(0),
  rapidity_minimum(0),
  rapidity_maximum(0),
  h_n2(nullptr),
  h_n2_Qinv(nullptr),
  h_n2_DeltaP(nullptr)
  {
  appendClassName("ParticlePair3DHistos");
  setName("ParticlePair3DHistos");
  setTitle("ParticlePair3DHistos");
  }

  ParticlePair3DHistos::ParticlePair3DHistos(const ParticlePair3DHistos & source)
  :
  HistogramGroup(source),
  n2_nbins(source.n2_nbins),
  Qinv_nbins(source.Qinv_nbins),
  DeltaPs_nbins(source.DeltaPs_nbins),
  DeltaPo_nbins(source.DeltaPo_nbins),
  DeltaPl_nbins(source.DeltaPl_nbins),
  pt_nbins(source.pt_nbins),
  rapidity_nbins(source.rapidity_nbins),
  n2_min(source.n2_min),
  n2_max(source.n2_max),
  Qinv_min(source.Qinv_min),
  Qinv_max(source.Qinv_max),
  DeltaPs_min(source.DeltaPs_min),
  DeltaPs_max(source.DeltaPs_max),
  DeltaPo_min(source.DeltaPo_min),
  DeltaPo_max(source.DeltaPo_max),
  DeltaPl_min(source.DeltaPl_min),
  DeltaPl_max(source.DeltaPl_max),
  pt_min(source.pt_min),
  pt_max(source.pt_max),
  rapidity_minimum(source.rapidity_minimum),
  rapidity_maximum(source.rapidity_maximum),
  h_n2(nullptr),
  h_n2_Qinv(nullptr),
  h_n2_DeltaP(nullptr)
  {
  cloneB(source);
  }

  ParticlePair3DHistos & ParticlePair3DHistos::operator=(const ParticlePair3DHistos & rhs)
  {
  if (this!=&rhs)
    {
    HistogramGroup::operator=(rhs),
    n2_nbins      = rhs.n2_nbins;
    Qinv_nbins    = rhs.Qinv_nbins;
    DeltaPs_nbins = rhs.DeltaPs_nbins;
    DeltaPo_nbins = rhs.DeltaPo_nbins;
    DeltaPl_nbins = rhs.DeltaPl_nbins;
    pt_nbins      = rhs.pt_nbins;
    rapidity_nbins       = rhs.rapidity_nbins;
    n2_min        = rhs.n2_min;
    n2_max        = rhs.n2_max;
    Qinv_min      = rhs.Qinv_min;
    Qinv_max      = rhs.Qinv_max;
    DeltaPs_min   = rhs.DeltaPs_min;
    DeltaPs_max   = rhs.DeltaPs_max;
    DeltaPo_min   = rhs.DeltaPo_min;
    DeltaPo_max   = rhs.DeltaPo_max;
    DeltaPl_min   = rhs.DeltaPl_min;
    DeltaPl_max   = rhs.DeltaPl_max;
    pt_min        = rhs.pt_min;
    pt_max        = rhs.pt_max;
    rapidity_minimum         = rhs.rapidity_minimum;
    rapidity_maximum         = rhs.rapidity_maximum;
    }
  return *this;
  }

  void ParticlePair3DHistos::configure(const String & taskName,
                                       const String & objectType,
                                       const Configuration & configuration,
                                       unsigned int index )
  {
  // Two fixes vs. the original code:
  //  (1) Call HistogramGroup::configure() so _histogramBaseName picks up
  //      the per-instance BASE_NAME key.  Without it histogram names
  //      come out as "NOTSET_n2" etc.
  //  (2) Read binning from the analyzer-level "HISTOGRAM:*" namespace
  //      (matching ParticlePairHistos / ParticleSingleHistos), so a
  //      single set of <task>:HISTOGRAM:n2_nbins keys configures every
  //      pair-3D histogram instance.
  HistogramGroup::configure(taskName, objectType, configuration, index);
  String type = "HISTOGRAM";
  n2_nbins       = configuration.valueInt(   createKey(taskName,type,"n2_nbins"));
  n2_min         = configuration.valueDouble(createKey(taskName,type,"n2_min"));
  n2_max         = configuration.valueDouble(createKey(taskName,type,"n2_max"));
  Qinv_nbins     = configuration.valueInt(   createKey(taskName,type,"Qinv_nbins"));
  Qinv_min       = configuration.valueDouble(createKey(taskName,type,"Qinv_min"));
  Qinv_max       = configuration.valueDouble(createKey(taskName,type,"Qinv_max"));
  DeltaPs_nbins  = configuration.valueInt(   createKey(taskName,type,"DeltaPs_nbins"));
  DeltaPo_nbins  = configuration.valueInt(   createKey(taskName,type,"DeltaPo_nbins"));
  DeltaPl_nbins  = configuration.valueInt(   createKey(taskName,type,"DeltaPl_nbins"));
  DeltaPs_min    = configuration.valueDouble(createKey(taskName,type,"DeltaPs_min"));
  DeltaPs_max    = configuration.valueDouble(createKey(taskName,type,"DeltaPs_max"));
  DeltaPo_min    = configuration.valueDouble(createKey(taskName,type,"DeltaPo_min"));
  DeltaPo_max    = configuration.valueDouble(createKey(taskName,type,"DeltaPo_max"));
  DeltaPl_min    = configuration.valueDouble(createKey(taskName,type,"DeltaPl_min"));
  DeltaPl_max    = configuration.valueDouble(createKey(taskName,type,"DeltaPl_max"));
  // pt / rapidity binning — inherited members used by
  // ParticlePair3DDerivedHistos::create() (n1_1_pt, n1_1_y, ...).
  // Stage 1 didn't need these because HISTOGRAM_2 N=0, but stage 2
  // (RunDerived) instantiates the derived histos and trips on n_x<1
  // unless we read these here.  Note: ParticlePair3DHistos.hpp declares
  // ONLY pt_* and rapidity_* (no phi_* / eta_* / fill flags) — those
  // exist in the 1D ParticlePairHistos but not in 3D.
  pt_nbins         = configuration.valueInt(   createKey(taskName,type,"pt_nbins"));
  pt_min           = configuration.valueDouble(createKey(taskName,type,"pt_min"));
  pt_max           = configuration.valueDouble(createKey(taskName,type,"pt_max"));
  rapidity_nbins   = configuration.valueInt(   createKey(taskName,type,"rapidity_nbins"));
  rapidity_minimum = configuration.valueDouble(createKey(taskName,type,"rapidity_min"));
  rapidity_maximum = configuration.valueDouble(createKey(taskName,type,"rapidity_max"));

  if (reportDebug(__FUNCTION__))
    {
    printValue(createKey(taskName,type,"n2_nbins"),n2_nbins);
    printValue(createKey(taskName,type,"n2_min"),n2_min);
    printValue(createKey(taskName,type,"n2_max"),n2_max);
    printValue(createKey(taskName,type,"Qinv_nbins"),Qinv_nbins);
    printValue(createKey(taskName,type,"Qinv_min"),Qinv_min);
    printValue(createKey(taskName,type,"Qinv_max"),Qinv_max);
    printValue(createKey(taskName,type,"DeltaPs_nbins"),DeltaPs_nbins);
    printValue(createKey(taskName,type,"DeltaPo_nbins"),DeltaPo_nbins);
    printValue(createKey(taskName,type,"DeltaPl_nbins"),DeltaPl_nbins);
    printValue(createKey(taskName,type,"DeltaPs_min"),DeltaPs_min);
    printValue(createKey(taskName,type,"DeltaPs_max"),DeltaPs_max);
    printValue(createKey(taskName,type,"DeltaPo_min"),DeltaPo_min);
    printValue(createKey(taskName,type,"DeltaPo_max"),DeltaPo_max);
    printValue(createKey(taskName,type,"DeltaPl_min"),DeltaPl_min);
    printValue(createKey(taskName,type,"DeltaPl_max"),DeltaPl_max);
    }
  }

  void ParticlePair3DHistos::create()
  {
  if (reportDebug(__FUNCTION__)) { printCR();}
  h_n2        = createHistogram(createName(_histogramBaseName,"n2"),n2_nbins,  n2_min,  n2_max, "n_{2}", "Yield");
  h_n2_Qinv   = createHistogram(createName(_histogramBaseName,"n2_Qinv"),Qinv_nbins,Qinv_min,Qinv_max, "Q_{inv}","n_{2}",2);
  h_n2_DeltaP = createHistogram(createName(_histogramBaseName,"n2_DeltaP"),
                                DeltaPs_nbins,  DeltaPs_min, DeltaPs_max,
                                DeltaPo_nbins,  DeltaPo_min, DeltaPo_max,
                                DeltaPl_nbins,  DeltaPl_min, DeltaPl_max,
                                "p_{s}","p_{o}", "p_{l}");
  }

  void ParticlePair3DHistos::loadFrom(TFile & inputFile)
  {
  if (reportDebug(__FUNCTION__)) { printCR();}
  h_n2        = importH1(inputFile, createName(_histogramBaseName,"n2"));
  h_n2_Qinv   = importH1(inputFile, createName(_histogramBaseName,"n2_Qinv"));
  h_n2_DeltaP = importH3(inputFile, createName(_histogramBaseName,"n2_DeltaP"));
  }

  void ParticlePair3DHistos::cloneB(const ParticlePair3DHistos & source)
  {
  if (reportDebug(__FUNCTION__)) { printCR(); }
  h_n2        = safeCloneH1(source.h_n2);
  h_n2_Qinv   = safeCloneH1(source.h_n2_Qinv);
  h_n2_DeltaP = safeCloneH3(source.h_n2_DeltaP);
  }


  void ParticlePair3DHistos::fill(Particle & particle1, Particle & particle2, double weight)
  {
  //decompose(double *pa,double *pb,double & qlong,double & qout,double & qside,double & qinv)
  // ====================================================================================
  // Method from Scott Pratt to do three dimensional qinv components
  // ====================================================================================
  double pt,s,Mlong,roots;
  double pa[4], pb[4], ptot[4],q[4];
  const int g[4]={1,-1,-1,-1};
  double qinv, qside, qlong, qout;

  VectorLorentz & momentumA = particle1.momentum();
  VectorLorentz & momentumB = particle2.momentum();
  pa[0] = momentumA.t();
  pa[1] = momentumA.x();
  pa[2] = momentumA.y();
  pa[3] = momentumA.z();

  pb[0] = momentumB.t();
  pb[1] = momentumB.x();
  pb[2] = momentumB.y();
  pb[3] = momentumB.z();

  qinv=0.0;
  s=0.0;
  for(int alpha=0;alpha<4;alpha++)
    {
    ptot[alpha]=pa[alpha]+pb[alpha];
    s+=g[alpha]*ptot[alpha]*ptot[alpha];
    q[alpha]=pa[alpha]-pb[alpha];
    qinv-=g[alpha]*q[alpha]*q[alpha];
    }
  pt=std::sqrt(ptot[1]*ptot[1]+ptot[2]*ptot[2]);
  // Defensive: pair-pT can be 0 for back-to-back partons → divisions
  // below would produce NaN/inf and corrupt the histograms.  Skip
  // such pairs.  Also guard against Mlong=0 (pair-rest-frame Mlong
  // can underflow for partonic kinematics).
  if (pt <= 0.0 || !std::isfinite(pt)) return;
  Mlong=std::sqrt(s+pt*pt);
  if (Mlong <= 0.0 || !std::isfinite(Mlong)) return;
  // qinv inside the loop accumulates -g[a]*q[a]*q[a]; it can become
  // slightly negative for off-shell partons → std::sqrt(NaN).  Guard.
  if (qinv < 0.0 || !std::isfinite(qinv)) return;
  roots=std::sqrt(s>=0.0 ? s : 0.0);

  qside=(ptot[1]*q[2]-ptot[2]*q[1])/pt;
  qlong=(ptot[0]*q[3]-ptot[3]*q[0])/Mlong;
  qout=(roots/Mlong)*(ptot[1]*q[1]+ptot[2]*q[2])/pt;
  qinv=std::sqrt(qinv);

  if (!std::isfinite(qside) || !std::isfinite(qlong) ||
      !std::isfinite(qout) || !std::isfinite(qinv)) return;

  h_n2_Qinv->Fill(qinv,weight);
  h_n2_DeltaP->Fill(qside,qout,qlong,weight);
  }


  void ParticlePair3DHistos::fillMultiplicity(double nPairs, double weight)
  {
  h_n2->Fill(nPairs, weight);
  }


}  //  namespace CAP



