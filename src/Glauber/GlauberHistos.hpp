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
#ifndef CAP__GlauberHistos
#define CAP__GlauberHistos
#include "TObject.h"
#include "Task.hpp"
#include "Exceptions.hpp"
#include "ManagedObjects.hpp"
#include "GlauberEvent.hpp"
#include "HistogramGroup.hpp"
#include "MathConstants.hpp"

namespace CAP
{

class GlauberHistos : public HistogramGroup
{
public:
  GlauberHistos();
  GlauberHistos(const GlauberHistos & group);
  GlauberHistos & operator=(const GlauberHistos & group);
  virtual ~GlauberHistos(){ }
  virtual void cloneAll(const GlauberHistos & group);
  virtual void create();
  virtual void loadFrom(TFile & inputFile);
  virtual void fill(GlauberEvent & event, double weight=1.0);
  virtual void configure(const String & taskName,
                         const String & objectType,
                         const Configuration & configuration,
                         unsigned int index);

  double calculateLength();
  double calculateDensityAt(const GlauberNucleus & nucleus, double x, double y, double rMaxSq);
  double calculateDensity(NuclearDistribution & dist, double x, double y);
  void   calculateArea(GlauberEvent & event);
  void   calculateAreaAt(const GlauberNucleus & nucleus, TH2* area, double mx, double my, double rMaxSq);
protected:

  bool _fillBasics;
  bool _fillMoments;
  bool _fillSmeared;
  bool _fillArea;
  bool _fillEvents;
  int  _fillEventsN;
  int  _fillEventsIndex;
  int  _nOrders;
  
  int _nParts_nBins;
  int _nBinaries_nBins;
  int _nSpect_nBins;
  int _impact_nBins;
  int _cs_nBins;
  int _psi_nBins;
  int _ecc_nBins;
  int _rN_nBins;
  int _psiN_nBins;
  int _eccN_nBins;
  int _area_nBins;

  float _nParts_min;
  float _nBinaries_min;
  float _nSpect_min;
  float _impact_min;
  float _cs_min;
  float _psi_min;
  float _ecc_min;
  float _rN_min;
  float _psiN_min;
  float _eccN_min;
  float _area_min;

  float _nParts_max;
  float _nBinaries_max;
  float _nSpect_max;
  float _impact_max;
  float _cs_max;
  float _psi_max;
  float _ecc_max;
  float _rN_max;
  float _psiN_max;
  float _eccN_max;
  float _area_max;




  TH1 * h_impactPar;
  TH1 * h_xSection;
  TH1 * h_nParts;
  TH1 * h_nPartsA;
  TH1 * h_nPartsB;
  TH1 * h_nPartsProton;
  TH1 * h_nPartsProtonA;
  TH1 * h_nPartsProtonB;
  TH1 * h_nPartsNeutron;
  TH1 * h_nPartsNeutronA;
  TH1 * h_nPartsNeutronB;

  TH1 * h_nBinaries;
  TH1 * h_nBinariesA;
  TH1 * h_nBinariesB;

  TH1 * h_nSpect;
  TH1 * h_nSpectA;
  TH1 * h_nSpectB;
  TH1 * h_nSpectProton;
  TH1 * h_nSpectProtonA;
  TH1 * h_nSpectProtonB;
  TH1 * h_nSpectNeutron;
  TH1 * h_nSpectNeutronA;
  TH1 * h_nSpectNeutronB;

  TH1 * h_nPP;
  TH1 * h_nPN;
  TH1 * h_nNP;
  TH1 * h_nNN;
  TH1 * h_nHard;

  TH2 * h_nPartsVsImpact;
  TH2 * h_nBinariesVsImpact;
  TH2 * h_nBinariesVsNParts;

  TProfile * h_nPartsAvgVsImpact;
  TProfile * h_nPartsAAvgVsImpact;
  TProfile * h_nPartsBAvgVsImpact;
  TProfile * h_nPartsProtonAvgVsImpact;
  TProfile * h_nPartsProtonAAvgVsImpact;
  TProfile * h_nPartsProtonBAvgVsImpact;
  TProfile * h_nPartsNeutronAvgVsImpact;
  TProfile * h_nPartsNeutronAAvgVsImpact;
  TProfile * h_nPartsNeutronBAvgVsImpact;

  TProfile * h_nBinariesAvgVsImpact;
  TProfile * h_nBinariesAAvgVsImpact;
  TProfile * h_nBinariesBAvgVsImpact;

  TProfile * h_nSpectAvgVsImpact;
  TProfile * h_nSpectAAvgVsImpact;
  TProfile * h_nSpectBAvgVsImpact;
  TProfile * h_nSpectProtonAvgVsImpact;
  TProfile * h_nSpectProtonAAvgVsImpact;
  TProfile * h_nSpectProtonBAvgVsImpact;
  TProfile * h_nSpectNeutronAvgVsImpact;
  TProfile * h_nSpectNeutronAAvgVsImpact;
  TProfile * h_nSpectNeutronBAvgVsImpact;

  TProfile * h_nPPAvgVsImpact;
  TProfile * h_nPNAvgVsImpact;
  TProfile * h_nNPAvgVsImpact;
  TProfile * h_nNNAvgVsImpact;
  TProfile * h_nHardAvgVsImpact;


// sys == system == participants

  TH1 * h_sysMeanX;
  TH1 * h_sysMeanY;
  TH1 * h_sysVarX;
  TH1 * h_sysVarY;
  TH1 * h_sysVarXY;
  TH1 * h_sysRmsX;
  TH1 * h_sysRmsY;
  TH1 * h_sysEccX;
  TH1 * h_sysEccY;
  TH1 * h_sysEcc;
  TH1 * h_sysPsi;

  TProfile * h_sysMeanXVsImpact;
  TProfile * h_sysMeanYVsImpact;
  TProfile * h_sysVarXVsImpact;
  TProfile * h_sysVarYVsImpact;
  TProfile * h_sysVarXYVsImpact;
  TProfile * h_sysRmsXVsImpact;
  TProfile * h_sysRmsYVsImpact;
  TProfile * h_sysEccXVsImpact;
  TProfile * h_sysEccYVsImpact;
  TProfile * h_sysEccVsImpact;
  TProfile * h_sysPsiVsImpact;

  std::vector<TH1*> h_sysCosPhiN;
  std::vector<TH1*> h_sysSinPhiN;
  std::vector<TH1*> h_sysRnN;
  std::vector<TH1*> h_sysPsiN;
  std::vector<TH1*> h_sysEccN;
  std::vector<TProfile*> h_sysCosPhiNVsImpact;
  std::vector<TProfile*> h_sysSinPhiNVsImpact;
  std::vector<TProfile*> h_sysRnNVsImpact;
  std::vector<TProfile*> h_sysPsiNVsImpact;
  std::vector<TProfile*> h_sysEccNVsImpact;

  TH2 * h_areaA;
  TH2 * h_areaB;
  TH1 * h_areaOverlap;
  TH1 * h_areaTotal;
  TProfile * h_areaAVsImpact;
  TProfile * h_areaBVsImpact;
  TProfile * h_areaOverlapVsImpact;
  TProfile * h_areaTotalVsImpact;

  std::vector<TH2*> h_events;


  ClassDef(GlauberHistos,0)
};

} // namespace CAP

#endif /* CAP__GlauberHistos  */



