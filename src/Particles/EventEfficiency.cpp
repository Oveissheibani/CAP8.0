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
#include "EventEfficiency.hpp"
#include "NameHelpers.hpp"
#include "PrintHelpers.hpp"
#include "RootHistogramHelpers.hpp"
#include "Task.hpp"

ClassImp(CAP::EventEfficiency);

namespace CAP
{

EventEfficiency::EventEfficiency()
:
HistogramGroup(),
h_efficiency_mult(nullptr),
h_weight_mult(nullptr),
nBins_mult(0),
min_mult(0),
max_mult(0)
{ /* no ops */}

EventEfficiency::EventEfficiency(const EventEfficiency & source)
:
HistogramGroup(source),
h_efficiency_mult(nullptr),
h_weight_mult(nullptr),
nBins_mult(source.nBins_mult),
min_mult(source.min_mult),
max_mult(source.max_mult)
{ /* no ops */}

EventEfficiency & EventEfficiency::operator=(const EventEfficiency & rhs)
{
  if (this!=&rhs)
    {
    EventEfficiency::operator=(rhs);
    cloneB(rhs);
    nBins_mult = rhs.nBins_mult;
    min_mult   = rhs.min_mult;
    max_mult   = rhs.max_mult;
    }
  return *this;
}

double EventEfficiency::efficiencyAt(const Event & event __attribute__((unused)))
{
  //int    ix  = h_efficiency_mult->GetXaxis()->FindBin(momentum.perp());
  return 1.0; //h_efficiency_mult->GetBinContent(ix);
}

double EventEfficiency::weightAt(const Event & event  __attribute__((unused)) )
{
  return 1.0;
}

bool EventEfficiency::accept(const Event & event __attribute__((unused)))
{
  return true;
}

void EventEfficiency::calculateWeights()
{
  if (reportDebug(__FUNCTION__)) { printCR(); }
  if (h_weight_mult) delete h_weight_mult;
  if (h_efficiency_mult)
    {
    h_weight_mult = (TH1*) h_efficiency_mult->Clone();
    unsigned int n = h_weight_mult->GetNbinsX();
    for (unsigned int k=1; k<n; k++)
      {
      double v = h_weight_mult->GetBinContent(k);
      double ev = h_weight_mult->GetBinError(k);
      if (v>0  &&  v<1.0)
        {
        double w = 1.0/v;
        double ew = w*ev/v;
        h_weight_mult->SetBinContent(k,w);
        h_weight_mult->SetBinError(k,ew);
        }
      else
        {
        h_weight_mult->SetBinContent(k,0.0);
        h_weight_mult->SetBinError(k,0.0);
        }
      }
    }
}


void EventEfficiency::cloneB(const EventEfficiency & source)
{
  if (reportDebug(__FUNCTION__)) { printCR(); }
  h_efficiency_mult = safeCloneH1(source.h_efficiency_mult);
  h_weight_mult     = safeCloneH1(source.h_efficiency_mult);
}

void EventEfficiency::configure(const String & ownerName,
                                      const String & typeName,
                                      const Configuration & configuration,
                                      unsigned int index)
{
  HistogramGroup::configure(ownerName,typeName,configuration,index);
  nBins_mult = configuration.valueInt(   createKey(ownerName,typeName,index,"nBins_mult"));
  min_mult   = configuration.valueDouble(createKey(ownerName,typeName,index,"Min_mult"));
  max_mult   = configuration.valueDouble(createKey(ownerName,typeName,index,"Max_mult"));
  if (reportDebug(__FUNCTION__))
    {
    printCR();
    printValue("Histo Base Name", baseName());
    printValue("nBins_mult",   nBins_mult);
    printValue("Min_mult",     min_mult);
    printValue("Max_mult",     max_mult);
    printCR();
    }
}

// for now use the same boundaries for eta and y histogram
void EventEfficiency::create()
{
  if (reportDebug(__FUNCTION__)) { printCR(); }
  h_efficiency_mult  = createHistogram(createName(baseName(),"MULT"),nBins_mult,min_mult,max_mult,"mult","#epsilon",2);
}

void EventEfficiency::loadFrom(TFile & inputFile)
{
  if (reportDebug(__FUNCTION__)) { printCR();}
  h_efficiency_mult  = importH2(inputFile,createName(baseName(),"MULT"));
}

} // namespace CAP
