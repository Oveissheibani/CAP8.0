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
#include "GlobalDerivedHistos.hpp"
#include "Task.hpp"
#include "Configuration.hpp"
#include "NameHelpers.hpp"
#include "RootHistogramHelpers.hpp"

ClassImp(CAP::GlobalDerivedHistos);

namespace CAP
{

  GlobalDerivedHistos::GlobalDerivedHistos()
  :
  GlobalHistos(),
  h_ptAvgIncVsN()
  {   }

  void GlobalDerivedHistos::cloneD(const GlobalDerivedHistos & group)
  {
  h_ptAvgIncVsN  = safeCloneH1(group.h_ptAvgIncVsN);
  }

  void GlobalDerivedHistos::configure(const String & taskName,
                                            const String & objectType,
                                            const Configuration & configuration,
                                            unsigned int index)
  {
  if (reportDebug(__FUNCTION__)) { printCR();}
  GlobalHistos::configure(taskName,objectType,configuration,index);
  }

  // for now use the same boundaries for eta and y histogram
  void GlobalDerivedHistos::create()
  {
  if (reportDebug(__FUNCTION__)) { printCR();}
  h_ptAvgIncVsN = createHistogram(createName(_histogramBaseName,"n","ptAvgIncl"),n_nbins, n_min,  n_max,"n","ptAvgIncl");
  }

  void GlobalDerivedHistos::loadFrom(TFile & inputFile)
  {
  if (reportDebug(__FUNCTION__)) { printCR();}
  GlobalHistos::loadFrom(inputFile);
  h_ptAvgIncVsN = importH1(inputFile, createName(_histogramBaseName,"n","ptAvgIncl"));
  }

  //!
  //! Calculates derived histograms. This is currently limited to pt averages vs eta and phi.
  //!
  void GlobalDerivedHistos::calculateDerivedHistograms(const GlobalHistos & baseHistos)
  {
  unsigned int n = baseHistos.p_ptSumVsN->GetNbinsX();
  for (unsigned int k=1; k<n; k++)
    {
    double v  = baseHistos.p_ptSumVsN->GetBinContent(k);
    double ev = baseHistos.p_ptSumVsN->GetBinError(k);
    double n  = baseHistos.p_ptSumVsN->GetXaxis()->GetBinCenter(k);
    h_ptAvgIncVsN->SetBinContent(k,v/n);
    h_ptAvgIncVsN->SetBinError(k,ev/n);
    }
  }

} // namespace CAP

