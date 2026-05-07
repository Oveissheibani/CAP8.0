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
#include "PtPtDerivedHistos.hpp"
#include "NameHelpers.hpp"
#include "PrintHelpers.hpp"
#include "RootHistogramHelpers.hpp"
#include "Task.hpp"
#include "Configuration.hpp"
#include "NameHelpers.hpp"

ClassImp(CAP::PtPtDerivedHistos);

namespace CAP
{

PtPtDerivedHistos::PtPtDerivedHistos()
:
PtPtHistos(),
h_F1_evt(),
h_F2_evt(),
h_F3_evt(),
h_F4_evt(),
h_q1_evt(),
h_q1q1_evt(),
h_q1q1q1_evt(),
h_q1q1q1q1_evt(),
h_dq1dq1_evt(),
h_dq1dq1dq1_evt(),
h_dq1dq1dq1dq1_evt()
{
  appendClassName("PtPtDerivedHistos");
  setName("PtPtDerivedHistos");
  setTitle("PtPtDerivedHistos");
}

PtPtDerivedHistos::PtPtDerivedHistos(const PtPtDerivedHistos & source)
:
PtPtHistos(),
h_F1_evt(),
h_F2_evt(),
h_F3_evt(),
h_F4_evt(),
h_q1_evt(),
h_q1q1_evt(),
h_q1q1q1_evt(),
h_q1q1q1q1_evt(),
h_dq1dq1_evt(),
h_dq1dq1dq1_evt(),
h_dq1dq1dq1dq1_evt()
{
  cloneD(source);
}

PtPtDerivedHistos & PtPtDerivedHistos::operator=(const PtPtDerivedHistos & rhs)
{
  if (this!=&rhs)
    {
    PtPtHistos::operator=(rhs);
    cloneD(rhs);
    }
  return *this;
}

void PtPtDerivedHistos::cloneD(const PtPtDerivedHistos & source )
{
  h_F1_evt = safeCloneH1(source.h_F1_evt);
  h_F2_evt = safeCloneH1(source.h_F2_evt);
  h_F3_evt = safeCloneH1(source.h_F3_evt);
  h_F4_evt = safeCloneH1(source.h_F4_evt);
  h_R2_evt = safeCloneH1(source.h_R2_evt);
  h_R3_evt = safeCloneH1(source.h_R3_evt);
  h_R4_evt = safeCloneH1(source.h_R4_evt);

  h_q1_evt = safeCloneH1(source.h_q1_evt);
  h_q1q1_evt = safeCloneH1(source.h_q1q1_evt);
  h_q1q1q1_evt = safeCloneH1(source.h_q1q1q1_evt);
  h_q1q1q1q1_evt = safeCloneH1(source.h_q1q1q1q1_evt);
  h_dq1dq1_evt = safeCloneH1(source.h_dq1dq1_evt);
  h_dq1dq1dq1_evt = safeCloneH1(source.h_dq1dq1dq1_evt);
  h_dq1dq1dq1dq1_evt = safeCloneH1(source.h_dq1dq1dq1dq1_evt);
}

void PtPtDerivedHistos::configure(const String & taskName,
                                        const String & objectType,
                                        const Configuration & configuration,
                                        unsigned int index)
{
  if (reportDebug(__FUNCTION__)) { printCR();}
  PtPtHistos::configure(taskName,objectType,configuration,index);
}


// for now use the same boundaries for eta and y histogram
void PtPtDerivedHistos::create()
{
  if (reportStart(__FUNCTION__)) { printCR(); }
  String xTitle = "EvtClass";
  h_F1_evt = createHistogram(createName(_histogramBaseName,"F1"),evt_nbins,evt_min,evt_max,xTitle,"F_{1}");
  h_F2_evt = createHistogram(createName(_histogramBaseName,"F2"),evt_nbins,evt_min,evt_max,xTitle,"F_{2}");
  h_F3_evt = createHistogram(createName(_histogramBaseName,"F3"),evt_nbins,evt_min,evt_max,xTitle,"F_{3}");
  h_F4_evt = createHistogram(createName(_histogramBaseName,"F4"),evt_nbins,evt_min,evt_max,xTitle,"F_{4}");
  h_R2_evt = createHistogram(createName(_histogramBaseName,"R2"),evt_nbins,evt_min,evt_max,xTitle,"R_{2}");
  h_R3_evt = createHistogram(createName(_histogramBaseName,"R3"),evt_nbins,evt_min,evt_max,xTitle,"R_{3}");
  h_R4_evt = createHistogram(createName(_histogramBaseName,"R4"),evt_nbins,evt_min,evt_max,xTitle,"R_{4}");

  h_q1_evt =           createHistogram(createName(_histogramBaseName,"q1"),          evt_nbins,evt_min,evt_max,xTitle,"q_{1}");
  h_q1q1_evt =         createHistogram(createName(_histogramBaseName,"q1q1"),        evt_nbins,evt_min,evt_max,xTitle,"q_{1}q_{1}");
  h_q1q1q1_evt =       createHistogram(createName(_histogramBaseName,"q1q1q1"),      evt_nbins,evt_min,evt_max,xTitle,"q_{1}q_{1}q_{1}");
  h_q1q1q1q1_evt =     createHistogram(createName(_histogramBaseName,"q1q1q1q1"),    evt_nbins,evt_min,evt_max,xTitle,"q_{1}q_{1}q_{1}q_{1}");
  h_dq1dq1_evt =       createHistogram(createName(_histogramBaseName,"dq1dq1"),      evt_nbins,evt_min,evt_max,xTitle,"#Delta q_{1}#Delta q_{1}");
  h_dq1dq1dq1_evt =    createHistogram(createName(_histogramBaseName,"dq1dq1dq1"),   evt_nbins,evt_min,evt_max,xTitle,"#Delta q_{1}#Delta q_{1}#Delta q_{1}");
  h_dq1dq1dq1dq1_evt = createHistogram(createName(_histogramBaseName,"dq1dq1dq1dq1"),evt_nbins,evt_min,evt_max,xTitle,"#Delta q_{1}#Delta q_{1}#Delta q_{1}#Delta q_{1}");
}

void PtPtDerivedHistos::loadFrom(TFile & inputFile)
{
  if (reportDebug(__FUNCTION__)) { printCR();}
  PtPtHistos::loadFrom(inputFile);
  h_F1_evt           = importProfile(inputFile,createName(_histogramBaseName,"F1"));
  h_F2_evt           = importProfile(inputFile,createName(_histogramBaseName,"F2"));
  h_F3_evt           = importProfile(inputFile,createName(_histogramBaseName,"F3"));
  h_F4_evt           = importProfile(inputFile,createName(_histogramBaseName,"F4"));
  h_q1_evt           = importProfile(inputFile,createName(_histogramBaseName,"q1"));
  h_q1q1_evt         = importProfile(inputFile,createName(_histogramBaseName,"q1q1"));
  h_q1q1q1_evt       = importProfile(inputFile,createName(_histogramBaseName,"q1q1q1"));
  h_q1q1q1q1_evt     = importProfile(inputFile,createName(_histogramBaseName,"q1q1q1q1"));
  h_dq1dq1_evt       = importProfile(inputFile,createName(_histogramBaseName,"dq1dq1"));
  h_dq1dq1dq1_evt    = importProfile(inputFile,createName(_histogramBaseName,"dq1dq1dq1"));
  h_dq1dq1dq1dq1_evt = importProfile(inputFile,createName(_histogramBaseName,"dq1dq1dq1dq1"));
}

void PtPtDerivedHistos::calculateDerivedHistograms(const PtPtHistos & src)
{
  calculateF1(src.h_f1_evt, h_F1_evt);
  calculateF2R2(src.h_f1_evt, src.h_f1_evt,
                src.h_f2_evt,h_F2_evt,h_R2_evt);
  calculateF3R3(src.h_f1_evt,src.h_f1_evt,src.h_f1_evt,
                src.h_f2_evt,src.h_f2_evt,src.h_f2_evt,
                src.h_f3_evt,h_F3_evt,h_R3_evt);
  calculateF4R4(src.h_f1_evt,src.h_f1_evt,src.h_f1_evt,src.h_f1_evt,
                src.h_f2_evt,src.h_f2_evt,src.h_f2_evt,
                src.h_f2_evt,src.h_f2_evt,src.h_f2_evt,
                src.h_f3_evt,src.h_f3_evt,src.h_f3_evt,src.h_f3_evt,
                src.h_f4_evt,h_F4_evt,h_R4_evt);
}

} // namespace CAP
