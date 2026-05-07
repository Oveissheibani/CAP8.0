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
#include "PtPtHistos.hpp"
#include "NameHelpers.hpp"
#include "PrintHelpers.hpp"
#include "RootHistogramHelpers.hpp"
#include "Task.hpp"
#include "NameHelpers.hpp"

ClassImp(CAP::PtPtHistos);

namespace CAP
{

  PtPtHistos::PtPtHistos()
  :
  HistogramGroup(),
  h_evt(),
  h_f1_evt(),
  h_f2_evt(),
  h_f3_evt(),
  h_f4_evt(),
  h_Q1_evt(),
  h_Q1Q1_evt(),
  h_Q1Q1Q1_evt(),
  h_Q1Q1Q1Q1_evt(),
  h_Q2_evt(),
  h_Q2Q1_evt(),
  h_Q2Q1Q1_evt(),
  h_Q2Q2_evt(),
  h_Q3_evt(),
  h_Q3Q1_evt(),
  h_Q4_evt(),
  evtName("MULT"),
  evt_nbins(100),
  evt_min(0.0),
  evt_max(100.0)
  {
  appendClassName("PtPtHistos");
  setName("PtPtHistos");
  setTitle("PtPtHistos");
  }

  PtPtHistos::PtPtHistos(const PtPtHistos & source)
  :
  HistogramGroup(source),
  h_evt(),
  h_f1_evt(),
  h_f2_evt(),
  h_f3_evt(),
  h_f4_evt(),
  h_Q1_evt(),
  h_Q1Q1_evt(),
  h_Q1Q1Q1_evt(),
  h_Q1Q1Q1Q1_evt(),
  h_Q2_evt(),
  h_Q2Q1_evt(),
  h_Q2Q1Q1_evt(),
  h_Q2Q2_evt(),
  h_Q3_evt(),
  h_Q3Q1_evt(),
  h_Q4_evt(),
  evtName(source.evtName),
  evt_nbins(source.evt_nbins),
  evt_min(source.evt_min),
  evt_max(source.evt_max)
  {
  cloneB(source);
  }

  PtPtHistos & PtPtHistos::operator=(const PtPtHistos & rhs)
  {
  if (this!=&rhs)
    {
    HistogramGroup::operator=(rhs),
    cloneB(rhs);
    }
  return *this;
  }

  void PtPtHistos::cloneB(const PtPtHistos & source)
  {
  if (reportDebug(__FUNCTION__)) { printCR(); }
  h_evt          = safeCloneH1(source.h_evt);
  h_f1_evt       = safeCloneProfile(source.h_f1_evt);
  h_f2_evt       = safeCloneProfile(source.h_f2_evt);
  h_f3_evt       = safeCloneProfile(source.h_f3_evt);
  h_f4_evt       = safeCloneProfile(source.h_f4_evt);
  h_Q1_evt       = safeCloneProfile(source.h_Q1_evt);
  h_Q1Q1_evt     = safeCloneProfile(source.h_Q1Q1_evt);
  h_Q1Q1Q1_evt   = safeCloneProfile(source.h_Q1Q1Q1_evt);
  h_Q1Q1Q1Q1_evt = safeCloneProfile(source.h_Q1Q1Q1Q1_evt);
  h_Q2_evt       = safeCloneProfile(source.h_Q2_evt);
  h_Q2Q1_evt     = safeCloneProfile(source.h_Q2Q1_evt);
  h_Q2Q1Q1_evt   = safeCloneProfile(source.h_Q2Q1Q1_evt);
  h_Q2Q2_evt     = safeCloneProfile(source.h_Q2Q2_evt);
  h_Q3_evt       = safeCloneProfile(source.h_Q3_evt);
  h_Q3Q1_evt     = safeCloneProfile(source.h_Q3Q1_evt);
  h_Q4_evt       = safeCloneProfile(source.h_Q4_evt);
  }

  void PtPtHistos::configure(const String & taskName,
                             const String & objectType,
                             const Configuration & configuration,
                             unsigned int index)
  {
  // Same fixes as ParticlePair3DHistos / NuDynHistos: call the base
  // configure so _histogramBaseName is set, and read binning from
  // <task>:HISTOGRAM:* (analyzer-level) instead of <task>:HISTOGRAM_1:*.
  HistogramGroup::configure(taskName, objectType, configuration, index);
  String type = "HISTOGRAM";
  evtName   = configuration.valueString(createKey(taskName,type,"evtName"));
  evt_nbins = configuration.valueInt(   createKey(taskName,type,"evt_nbins"));
  evt_min   = configuration.valueDouble(createKey(taskName,type,"evt_min"));
  evt_max   = configuration.valueDouble(createKey(taskName,type,"evt_max"));
  if (reportDebug(__FUNCTION__))
    {
    printCR();
    printValue(createKey(taskName,type,"evtName"),evtName);
    printValue(createKey(taskName,type,"evt_nbins"),evt_nbins);
    printValue(createKey(taskName,type,"evt_min"),evt_min);
    printValue(createKey(taskName,type,"evt_min"),evt_max);
    printCR();
    }
  }


  void PtPtHistos::create()
  {
  if (reportDebug(__FUNCTION__)) { printCR();}
  String xTitle = "M";
  h_evt    = createHistogram(createName(_histogramBaseName,"mult"),evt_nbins,evt_min,evt_max,xTitle,"Counts");
  h_f1_evt = createProfile(createName(_histogramBaseName,"n1"),evt_nbins,evt_min,evt_max,xTitle,"n_{1}");
  h_f2_evt = createProfile(createName(_histogramBaseName,"n2"),evt_nbins,evt_min,evt_max,xTitle,"n_{2}");
  h_f3_evt = createProfile(createName(_histogramBaseName,"n3"),evt_nbins,evt_min,evt_max,xTitle,"n_{3}");
  h_f4_evt = createProfile(createName(_histogramBaseName,"n4"),evt_nbins,evt_min,evt_max,xTitle,"n_{4}");
  h_Q1_evt = createProfile(createName(_histogramBaseName,"Q1"),      evt_nbins,evt_min,evt_max,xTitle,"Q_{1}");
  h_Q1Q1_evt = createProfile(createName(_histogramBaseName,"Q1Q1"),    evt_nbins,evt_min,evt_max,xTitle,"Q_{1}^{2}");
  h_Q1Q1Q1_evt = createProfile(createName(_histogramBaseName,"Q1Q1Q1"),  evt_nbins,evt_min,evt_max,xTitle,"Q_{1}^{3}");
  h_Q1Q1Q1Q1_evt = createProfile(createName(_histogramBaseName,"Q1Q1Q1Q1"),evt_nbins,evt_min,evt_max,xTitle,"Q_{1}^{4}");
  h_Q2_evt = createProfile(createName(_histogramBaseName,"Q2"),      evt_nbins,evt_min,evt_max,xTitle,"Q_{2}^{2}");
  h_Q2Q1_evt = createProfile(createName(_histogramBaseName,"Q2Q1"),    evt_nbins,evt_min,evt_max,xTitle,"Q_{2}Q_{2}");
  h_Q2Q1Q1_evt = createProfile(createName(_histogramBaseName,"Q2Q1Q1"),  evt_nbins,evt_min,evt_max,xTitle,"Q_{2}Q_{1}^2");
  h_Q2Q2_evt =     createProfile(createName(_histogramBaseName,"Q2Q2"),    evt_nbins,evt_min,evt_max,xTitle,"Q_{2}^2");
  h_Q3_evt =       createProfile(createName(_histogramBaseName,"Q3"),      evt_nbins,evt_min,evt_max,xTitle,"Q_{3}");
  h_Q3Q1_evt =     createProfile(createName(_histogramBaseName,"Q3Q1"),    evt_nbins,evt_min,evt_max,xTitle,"Q_{3}Q_{1}");
  h_Q4_evt =       createProfile(createName(_histogramBaseName,"Q4"),      evt_nbins,evt_min,evt_max,xTitle,"Q_{4}");
  }

  void PtPtHistos::loadFrom(TFile & inputFile)
  {
  if (reportDebug(__FUNCTION__)) { printCR(); }
  h_f1_evt = loadProfile(inputFile,createName(_histogramBaseName,"n1"));
  h_f2_evt = loadProfile(inputFile,createName(_histogramBaseName,"n2"));
  h_f3_evt = loadProfile(inputFile,createName(_histogramBaseName,"n3"));
  h_f4_evt = loadProfile(inputFile,createName(_histogramBaseName,"n4"));
  h_Q1_evt =       loadProfile(inputFile,createName(_histogramBaseName,"Q1"));
  h_Q1Q1_evt =     loadProfile(inputFile,createName(_histogramBaseName,"Q1Q1"));
  h_Q1Q1Q1_evt =   loadProfile(inputFile,createName(_histogramBaseName,"Q1Q1Q1"));
  h_Q1Q1Q1Q1_evt = loadProfile(inputFile,createName(_histogramBaseName,"Q1Q1Q1Q1"));
  h_Q2_evt =       loadProfile(inputFile,createName(_histogramBaseName,"Q2"));
  h_Q2Q1_evt =     loadProfile(inputFile,createName(_histogramBaseName,"Q2Q1"));
  h_Q2Q1Q1_evt =   loadProfile(inputFile,createName(_histogramBaseName,"Q2Q1Q1"));
  h_Q2Q2_evt =     loadProfile(inputFile,createName(_histogramBaseName,"Q2Q2"));
  h_Q3_evt =       loadProfile(inputFile,createName(_histogramBaseName,"Q3"));
  h_Q3Q1_evt =     loadProfile(inputFile,createName(_histogramBaseName,"Q3Q1"));
  h_Q4_evt =       loadProfile(inputFile,createName(_histogramBaseName,"Q4"));
  }

  /// Implement Eqs. D4 - D7
  ///
  void PtPtHistos::fill(double mult __attribute__((unused)),
                        vector<double> & moments __attribute__((unused)))
  {

  }
  //  unsigned int nFilters = counts.size();
  //  ////unsigned int index = 0;
  //  for (unsigned int i1=0; i1<nFilters; i1++)
  //    {
  //    double n1 = counts[i1];
  //    double n2 = n1*(n1-1);
  //    double n3 = n2*(n1-2);
  //    double n4 = n3*(n1-3);
  //    h_f1_evt[i1]->Fill(mult,n1);
  //    h_f2_evt[i1]->Fill(mult,n2);
  //    h_f3_evt[i1]->Fill(mult,n3);
  //    h_f4_evt[i1]->Fill(mult,n4);
  //
  //    double Q1 = q1Sum[i1];
  //    double Q2 = q2Sum[i1];
  //    double Q3 = q3Sum[i1];
  //    double Q4 = q4Sum[i1];
  //    double Q1Q1     = Q1*Q1;
  //    double Q1Q1Q1   = Q1Q1*Q1;
  //    double Q1Q1Q1Q1 = Q1Q1Q1*Q1;
  //    double Q2Q1   = Q2*Q1;
  //    double Q2Q2   = Q2*Q2;
  //    double Q2Q1Q1 = Q2Q1*Q1;
  //    double Q3Q1   = Q3*Q1;
  //
  //    h_Q1_evt[i1]->Fill(mult,Q1);
  //    h_Q1Q1_evt[i1]->Fill(mult,Q1Q1);
  //    h_Q1Q1Q1_evt[i1]->Fill(mult,Q1Q1Q1);
  //    h_Q1Q1Q1Q1_evt[i1]->Fill(mult,Q1Q1Q1Q1);
  //    h_Q2_evt[i1]->Fill(mult,Q2);
  //    h_Q2Q1_evt[i1]->Fill(mult,Q2Q1);
  //    h_Q2Q1Q1_evt[i1]->Fill(mult,Q2Q1Q1);
  //    h_Q2Q2_evt[i1]->Fill(mult,Q2Q2);
  //    h_Q3_evt[i1]->Fill(mult,Q3);
  //    h_Q3Q1_evt[i1]->Fill(mult,Q3Q1);
  //    h_Q4_evt[i1]->Fill(mult,Q4);
  //    }

} // namespace CAP

