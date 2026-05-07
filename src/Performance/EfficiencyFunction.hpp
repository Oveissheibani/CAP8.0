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
#ifndef CAP__EfficiencyFunction
#define CAP__EfficiencyFunction

namespace CAP
{

class EfficiencyFunction
{
public:
  
  EfficiencyFunction(double _eff0,
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
                     double _etaLowSigma2,
                     double _etaHighEdge,
                     double _etaHighSigma2);
  EfficiencyFunction(const EfficiencyFunction & f);
  EfficiencyFunction operator=(const EfficiencyFunction & f);
  ~EfficiencyFunction() {}
  double efficiency(double pt, double eta, double phi);
  bool accept(double pt, double eta, double phi);


protected:
  
  double eff0;
  double ptMin;
  double eff1Pt;
  double eff2Pt;
  double eff3Pt;
  double eff4Pt;
  double etaMin;
  double etaMax;
  double eff1Eta;
  double eff2Eta;
  double eff3Eta;
  double eff4Eta;
  double etaLowEdge;
  double etaLowSigma2;
  double etaHighEdge;
  double etaHighSigma2;
};


} // namespace CAP

#endif /* CAP__EfficiencyFunction */
