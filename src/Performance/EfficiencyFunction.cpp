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
#include "TRandom.h"
#include "PrintHelpers.hpp"
#include "EfficiencyFunction.hpp"

ClassImp(CAP::EfficiencyFunction);

namespace CAP
{

EfficiencyFunction::EfficiencyFunction(double _eff0,
                                       double _ptMin,
                                       double _eff1Pt,
                                       double _eff2Pt,
                                       double _eff3Pt,
                                       double _eff4Pt,
                                       double _etaMin,
                                       double _etaMax,
                                       double _eff1Eta,
                                       double _eff2Eta,
                                       double _eff3Eta,
                                       double _eff4Eta,
                                       double _etaLowEdge,
                                       double _etaLowSigma,
                                       double _etaHighEdge,
                                       double _etaHighSigma)
:
eff0(_eff0),
ptMin(_ptMin),
eff1Pt(_eff1Pt),
eff2Pt(_eff2Pt),
eff3Pt(_eff3Pt),
eff4Pt(_eff4Pt),
etaMin(_etaMin),
etaMax(_etaMax),
eff1Eta(_eff1Eta),
eff2Eta(_eff2Eta),
eff3Eta(_eff3Eta),
eff4Eta(_eff4Eta),
etaLowEdge(_etaLowEdge),
etaLowSigma2(_etaLowSigma2),
etaHighEdge(_etaHighEdge),
etaHighSigma2(_etaHighSigma2)
{   }

EfficiencyFunction::EfficiencyFunction(const EfficiencyFunction & f)
:
eff0(f.eff0),
ptMin(_ptMin),
eff1Pt(f.eff1Pt),
eff2Pt(f.eff2Pt),
eff3Pt(f.eff3Pt),
eff4Pt(f.eff4Pt),
etaMin(f.etaMin),
etaMax(f.etaMax),
eff1Eta(f.eff1Eta),
eff2Eta(f.eff2Eta),
eff3Eta(f.eff3Eta),
eff4Eta(f.eff4Eta),
etaLowEdge(f.etaLowEdge),
etaLowSigma2(f.etaLowSigma2),
etaHighEdge(f.etaHighEdge),
etaHighSigma2(f.etaHighSigma2)
{ }

EfficiencyFunction EfficiencyFunction::operator=(const EfficiencyFunction & f)
{
  if (this!=&f)
  {
    eff0    =  f.eff0;
    ptMin   =  f.ptMin;
    eff1Pt  =  f.eff1Pt;
    eff2Pt  =  f.eff2Pt;
    eff3Pt  =  f.eff3Pt;
    eff4Pt  =  f.eff4Pt;
    etaMin  =  f.etaMin;
    etaMax  =  f.etaMax;
    eff1Eta =  f.eff1Eta;
    eff2Eta =  f.eff2Eta;
    eff3Eta =  f.eff3Eta;
    eff4Eta =  f.eff4Eta;
    etaLowEdge   =  f.etaLowEdge;
    etaLowSigma  =  f.etaLowSigma;
    etaHighEdge  =  f.etaHighEdge;
    etaHighSigma =  f.etaHighSigma;
  }
  return *this;
}

double EfficiencyFunction::efficiency(double pt, double eta, double phi __attribute__((unused)))
{
  double eff = eff0;
  double pt2 = pt*pt;
  double pt3 = pt2*pt;
  double pt4 = pt3*pt;
  double eta2 = eta*eta;
  double eta3 = eta2*eta;
  double eta4 = eta3*eta;
  double ptEff  = 0.0;
  double etaEff = 0.0;
  double arg;
  if (eta<etaLowEdge)
  {
    arg = eta-etaLowEdge;
    etaEff = exp(-0.5*arg*arg/etaLowSigma2);
  }
  else if (eta>=etaHighEdge)
  {
    arg = eta-etaHighEdge;
    etaEff = exp(-0.5*arg*arg/etaHighSigma2);
  }
  else
  {
    etaEff = 1.0 + eff1Eta*eta + eff2Eta*eta2 + eff3Eta*eta3 + eff4Eta*eta4;
  }
  ptEff = eff0 + eff1Pt*pt + eff2Pt*pt2 + eff3Pt*pt3 + eff4Pt*pt4;
  return ptEff*etaEff;
}

bool EfficiencyFunction::accept(double pt, double eta, double phi)
{
  if (pt<ptMin || eta<etaMin || eta>=etaMax) return false;
  double eff = efficiency(pt,eta,phi);
  return gRandom->Rndm() < eff;
}

} // namespace CAP
