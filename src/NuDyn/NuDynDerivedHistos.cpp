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
#include "NuDynDerivedHistos.hpp"
#include "NameHelpers.hpp"
#include "PrintHelpers.hpp"
#include "RootHistogramHelpers.hpp"
#include "Task.hpp"
#include "Configuration.hpp"
#include "NameHelpers.hpp"

ClassImp(CAP::NuDynDerivedHistos);

namespace CAP
{

NuDynDerivedHistos::NuDynDerivedHistos()
:
NuDynHistos(),
h_F2_11_evt(nullptr),
h_F2_12_evt(nullptr),
h_F2_22_evt(nullptr),
h_R2_11_evt(nullptr),
h_R2_12_evt(nullptr),
h_R2_22_evt(nullptr),
h_nudyn_evt(nullptr)
{
  appendClassName("NuDynDerivedHistos");
  setName("NuDynDerivedHistos");
  setTitle("NuDynDerivedHistos");
}

NuDynDerivedHistos::NuDynDerivedHistos(const NuDynDerivedHistos & source)
:
NuDynHistos(),
h_F2_11_evt(nullptr),
h_F2_12_evt(nullptr),
h_F2_22_evt(nullptr),
h_R2_11_evt(nullptr),
h_R2_12_evt(nullptr),
h_R2_22_evt(nullptr),
h_nudyn_evt(nullptr)
{
  cloneD(source);
}

NuDynDerivedHistos & NuDynDerivedHistos::operator=(const NuDynDerivedHistos & rhs)
{
  if (this!=&rhs)
    {
    NuDynHistos::operator=(rhs);
    cloneD(rhs);
    }
  return *this;
}

void NuDynDerivedHistos::cloneD(const NuDynDerivedHistos & source)
{
  if (reportDebug(__FUNCTION__)) { printCR(); }
  h_F2_11_evt    = safeCloneH1(source.h_F2_11_evt);
  h_F2_12_evt    = safeCloneH1(source.h_F2_12_evt);
  h_F2_22_evt    = safeCloneH1(source.h_F2_22_evt);
  h_R2_11_evt    = safeCloneH1(source.h_R2_11_evt);
  h_R2_12_evt    = safeCloneH1(source.h_R2_12_evt);
  h_R2_22_evt    = safeCloneH1(source.h_R2_22_evt);
  h_nudyn_evt    = safeCloneH1(source.h_nudyn_evt);
}

void NuDynDerivedHistos::configure(const String & taskName,
                                         const String & objectType,
                                         const Configuration & configuration,
                                         unsigned int index)
{
  NuDynHistos::configure(taskName,objectType,configuration,index);
}

// for now use the same boundaries for eta and y histogram
void NuDynDerivedHistos::create()
{
  if (reportDebug(__FUNCTION__)) { printCR();}
  h_F2_11_evt = createHistogram(createName(_histogramBaseName,"F2_11"), evt_nbins,evt_min,evt_max,"evtClass","F2_{11}");
  h_F2_12_evt = createHistogram(createName(_histogramBaseName,"F2_12"), evt_nbins,evt_min,evt_max,"evtClass","F2_{12}");
  h_F2_22_evt = createHistogram(createName(_histogramBaseName,"F2_22"), evt_nbins,evt_min,evt_max,"evtClass","F2_{22}");
  h_R2_11_evt = createHistogram(createName(_histogramBaseName,"R2_11"), evt_nbins,evt_min,evt_max,"evtClass","R2_{11}");
  h_R2_12_evt = createHistogram(createName(_histogramBaseName,"R2_12"), evt_nbins,evt_min,evt_max,"evtClass","R2_{12}");
  h_R2_22_evt = createHistogram(createName(_histogramBaseName,"R2_22"), evt_nbins,evt_min,evt_max,"evtClass","R2_{22}");
  h_nudyn_evt = createHistogram(createName(_histogramBaseName,"Nu_dyn"),evt_nbins,evt_min,evt_max,"evtClass","#nu_{Dyn}");
}

void NuDynDerivedHistos::loadFrom(TFile & inputFile)
{
  if (reportDebug(__FUNCTION__)) { printCR(); }
  NuDynHistos::loadFrom(inputFile);
  h_F2_11_evt = importH1(inputFile,createName(_histogramBaseName,"F2_11"));
  h_F2_12_evt = importH1(inputFile,createName(_histogramBaseName,"F2_12"));
  h_F2_22_evt = importH1(inputFile,createName(_histogramBaseName,"F2_22"));
  h_R2_11_evt = importH1(inputFile,createName(_histogramBaseName,"R2_11"));
  h_R2_12_evt = importH1(inputFile,createName(_histogramBaseName,"R2_12"));
  h_R2_22_evt = importH1(inputFile,createName(_histogramBaseName,"R2_22"));
  h_nudyn_evt = importH1(inputFile,createName(_histogramBaseName,"Nu_dyn"));
}

void NuDynDerivedHistos::calculateDerivedHistograms(const NuDynHistos & source)
{
  if (reportDebug(__FUNCTION__)) { printCR(); }
  calculateF2R2(source.h_f1_1_evt,source.h_f1_1_evt,source.h_f2_11_evt,h_F2_11_evt,h_R2_11_evt);
  calculateF2R2(source.h_f1_1_evt,source.h_f1_2_evt,source.h_f2_12_evt,h_F2_12_evt,h_R2_12_evt);
  calculateF2R2(source.h_f1_2_evt,source.h_f1_2_evt,source.h_f2_22_evt,h_F2_12_evt,h_R2_22_evt);
  calculateNudyn(h_R2_11_evt,h_R2_12_evt,h_R2_12_evt,h_R2_22_evt,h_nudyn_evt);
}

} // namespace CAP
