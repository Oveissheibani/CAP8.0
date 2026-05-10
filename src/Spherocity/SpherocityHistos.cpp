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
#include "Particle.hpp"
#include "SpherocityHistos.hpp"
#include "GlobalHistos.hpp"
#include "NameHelpers.hpp"
#include "PrintHelpers.hpp"
#include "RootHistogramHelpers.hpp"
#include "Task.hpp"
#include "Configuration.hpp"

ClassImp(CAP::SpherocityHistos);

namespace CAP
{

  SpherocityHistos::SpherocityHistos()
  :
  HistogramGroup(),
  fillS0(true),
  fillS1(false),
  fillS1VsS0(false),
  h_s0(nullptr),
  h_s1(),
  h_s1VsS0()
  {
  appendClassName("SpherocityHistos");
  setName("SpherocityHistos");
  setTitle("SpherocityHistos");
  }

  SpherocityHistos::SpherocityHistos(const SpherocityHistos & source)
  :
  HistogramGroup(source),
  fillS0(source.fillS0),
  fillS1(source.fillS1),
  fillS1VsS0(source.fillS1VsS0),
  h_s0(),
  h_s1(),
  h_s1VsS0()
  {
  cloneB(source);
  }

  SpherocityHistos & SpherocityHistos::operator=(const SpherocityHistos & rhs)
  {
  if (this!=&rhs)
    {
    HistogramGroup::operator=(rhs);
    fillS0     = rhs.fillS0;
    fillS1     = rhs.fillS1;
    fillS1VsS0 = rhs.fillS1VsS0;
    cloneB(rhs);
    }
  return *this;
  }

  void SpherocityHistos::cloneB(const SpherocityHistos & source)
  {
  if (reportDebug(__FUNCTION__)) { printCR(); }
  // BUG FIX: previously this aliased pointers
  //     h_s0 = (TH1*) source.h_s0;
  // which made the copy share its histograms with `source`.  When
  // `source` (or the file owning its histos) was destroyed, the copy
  // had dangling pointers and saveTo() segfaulted in SetDirectory()/
  // Write() on freed memory.  Use safeCloneH1 (deep clone) so each
  // copy owns its own histogram — matches the pattern used by every
  // other *Histos class in CAP.
  h_s0     = safeCloneH1(source.h_s0);
  h_s1     = safeCloneH1(source.h_s1);
  h_s1VsS0 = safeCloneH2(source.h_s1VsS0);
  }

  void SpherocityHistos::configure(const String & taskName,
                                   const String & objectType,
                                   const Configuration & configuration,
                                   unsigned int index)
  {
  HistogramGroup::configure(taskName,objectType,configuration,index);
  // Read binning from <task>:HISTOGRAM:* (analyzer-level), not
  // <task>:HISTOGRAM_1:* (per-instance) — the rest of CAP and the .ini
  // composer share one binning block per analyzer.
  String type = "HISTOGRAM";
  fillS0     = configuration.valueBool(createKey(taskName,type,"FillS0"));
  fillS1     = configuration.valueBool(createKey(taskName,type,"FillS1"));
  fillS1VsS0 = configuration.valueBool(createKey(taskName,type,"FillS1VsS0"));
  spherocity_nbins = configuration.valueInt(   createKey(taskName,type,"spherocity_nbins"));
  spherocity_min   = configuration.valueDouble(createKey(taskName,type,"spherocity_min"));
  spherocity_max   = configuration.valueDouble(createKey(taskName,type,"spherocity_max"));

  if (reportDebug(__FUNCTION__))
    {
    printCR();
    printValue(createKey(taskName,type,"fillS0"),fillS0);
    printValue(createKey(taskName,type,"fillS1"),fillS1);
    printValue(createKey(taskName,type,"fillS1VsS0"),fillS1VsS0);
    printValue(createKey(taskName,type,"spherocity_nbins"),spherocity_nbins);
    printValue(createKey(taskName,type,"spherocity_min"),spherocity_min);
    printValue(createKey(taskName,type,"spherocity_max"),spherocity_max);
    }
  }

  void SpherocityHistos::create()
  {
  if (reportDebug(__FUNCTION__)) { printCR();}
  if (fillS0) h_s0 = createHistogram(createName(_histogramBaseName,"S0"), spherocity_nbins,spherocity_min,spherocity_max,"S_{0}","N");
  if (fillS1) h_s1 = createHistogram(createName(_histogramBaseName,"S1"), spherocity_nbins,spherocity_min,spherocity_max,"S_{1}","N");
  if (fillS1VsS0)
    h_s1VsS0  =  createHistogram(createName(_histogramBaseName,"S1VsS0"),
                                 spherocity_nbins,spherocity_min,spherocity_max,spherocity_nbins,spherocity_min,spherocity_max,
                                 "S_{0}","S_{1}","N");

  }

  void SpherocityHistos::loadFrom(TFile & inputFile)
  {
  if (fillS0) h_s0 = importH1(inputFile, createName(_histogramBaseName,"S0"));
  if (fillS1) h_s1 = importH1(inputFile, createName(_histogramBaseName,"S1"));
  if (fillS1VsS0) h_s1VsS0 =  importH2(inputFile, createName(_histogramBaseName,"S1VsS0"));
  }

  //!
  //! Fiil  single particle histograms of this class with the particles contained in the given list.
  //!
  void SpherocityHistos::fill(double s0, double s1, double weight)
  {
  if (fillS0) h_s0->Fill(s0,weight);
  if (fillS1) h_s1->Fill(s1,weight);
  if (fillS1VsS0) h_s1VsS0->Fill(s0,s1,weight);
  }

} // namespace CAP
