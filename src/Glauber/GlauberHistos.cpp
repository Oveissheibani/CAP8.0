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
#include "GlauberHistos.hpp"
#include "MathConstants.hpp"
#include "NameHelpers.hpp"
#include "PrintHelpers.hpp"
#include "RootHistogramHelpers.hpp"
#include "VectorLorentz.hpp"
#include "TEllipse.h"
#include "TCanvas.h"

ClassImp(CAP::GlauberHistos);

using CAP::MATH::pi;

namespace CAP
{

  GlauberHistos::GlauberHistos()
  :
  HistogramGroup(),
  _fillBasics(true),
  _fillMoments(false),
  _fillSmeared(false),
  _fillArea(false),
  _drawEvents(false),
  _drawEventsN(100),
  _drawEventsIndex(0),
  _nOrders(4),
  _nParts_nBins(100),
  _nBinaries_nBins(100),
  _nSpect_nBins(100),
  _impact_nBins(50),
  _cs_nBins(20),
  _psi_nBins(20),
  _ecc_nBins(20),
  _rN_nBins(20),
  _psiN_nBins(20),
  _eccN_nBins(20),

  _nParts_min(0.0),
  _nBinaries_min(0.0),
  _nSpect_min(0.0),
  _impact_min(0.0),
  _cs_min(-1.0),
  _psi_min(-pi()/2.0),
  _ecc_min(-1.0),
  _rN_min(0.0),
  _psiN_min(-pi()/2.0),
  _eccN_min(-1.0),

  _nParts_max(500.0),
  _nBinaries_max(1000.0),
  _nSpect_max(500.0),
  _impact_max(12.0),
  _cs_max(1.0),
  _psi_max(pi()/2.0),
  _ecc_max(1.0),
  _rN_max(12.0),
  _psiN_max(pi()/2.0),
  _eccN_max(1.0),

  h_impactPar(nullptr),
  h_xSection(nullptr),
  h_nParts(nullptr),
  h_nPartsA(nullptr),
  h_nPartsB(nullptr),
  h_nPartsProton(nullptr),
  h_nPartsProtonA(nullptr),
  h_nPartsProtonB(nullptr),
  h_nPartsNeutron(nullptr),
  h_nPartsNeutronA(nullptr),
  h_nPartsNeutronB(nullptr),

  h_nBinaries(nullptr),
  h_nBinariesA(nullptr),
  h_nBinariesB(nullptr),
//  h_nBinariesProton(nullptr),
//  h_nBinariesProtonA(nullptr),
//  h_nBinariesProtonB(nullptr),
//  h_nBinariesNeutron(nullptr),
//  h_nBinariesNeutronA(nullptr),
//  h_nBinariesNeutronB(nullptr),

  h_nSpect(nullptr),
  h_nSpectA(nullptr),
  h_nSpectB(nullptr),
  h_nSpectProton(nullptr),
  h_nSpectProtonA(nullptr),
  h_nSpectProtonB(nullptr),
  h_nSpectNeutron(nullptr),
  h_nSpectNeutronA(nullptr),
  h_nSpectNeutronB(nullptr),

  h_nPP(nullptr),
  h_nPN(nullptr),
  h_nNP(nullptr),
  h_nNN(nullptr),
  h_nHard(nullptr),

  h_nPartsVsImpact(),
  h_nBinariesVsImpact(),
  h_nBinariesVsNParts(nullptr),

  h_nPartsAvgVsImpact(),
  h_nPartsAAvgVsImpact(),
  h_nPartsBAvgVsImpact(),
  h_nPartsProtonAvgVsImpact(),
  h_nPartsProtonAAvgVsImpact(),
  h_nPartsProtonBAvgVsImpact(),
  h_nPartsNeutronAvgVsImpact(),
  h_nPartsNeutronAAvgVsImpact(),
  h_nPartsNeutronBAvgVsImpact(),

  h_nBinariesAvgVsImpact(),
  h_nBinariesAAvgVsImpact(),
  h_nBinariesBAvgVsImpact(),
//  h_nBinariesProtonAvgVsImpact(),
//  h_nBinariesProtonAAvgVsImpact(),
//  h_nBinariesProtonBAvgVsImpact(),
//  h_nBinariesNeutronAvgVsImpact(),
//  h_nBinariesNeutronAAvgVsImpact(),
//  h_nBinariesNeutronBAvgVsImpact(),

  h_nSpectAvgVsImpact(nullptr),
  h_nSpectAAvgVsImpact(nullptr),
  h_nSpectBAvgVsImpact(nullptr),
  h_nSpectProtonAvgVsImpact(nullptr),
  h_nSpectProtonAAvgVsImpact(nullptr),
  h_nSpectProtonBAvgVsImpact(nullptr),
  h_nSpectNeutronAvgVsImpact(nullptr),
  h_nSpectNeutronAAvgVsImpact(nullptr),
  h_nSpectNeutronBAvgVsImpact(nullptr),

  h_nPPAvgVsImpact(nullptr),
  h_nPNAvgVsImpact(nullptr),
  h_nNPAvgVsImpact(nullptr),
  h_nNNAvgVsImpact(nullptr),
  h_nHardAvgVsImpact(nullptr),

  h_sysMeanX(nullptr),
  h_sysMeanY(nullptr),
  h_sysVarX(nullptr),
  h_sysVarY(nullptr),
  h_sysVarXY(nullptr),
  h_sysRmsX(nullptr),
  h_sysRmsY(nullptr),
  h_sysEccX(nullptr),
  h_sysEccY(nullptr),
  h_sysEcc(nullptr),
  h_sysPsi(nullptr),

  h_sysMeanXVsImpact(nullptr),
  h_sysMeanYVsImpact(nullptr),
  h_sysVarXVsImpact(nullptr),
  h_sysVarYVsImpact(nullptr),
  h_sysVarXYVsImpact(nullptr),
  h_sysRmsXVsImpact(nullptr),
  h_sysRmsYVsImpact(nullptr),
  h_sysEccXVsImpact(nullptr),
  h_sysEccYVsImpact(nullptr),
  h_sysEccVsImpact(nullptr),
  h_sysPsiVsImpact(nullptr),

  h_sysCosPhiN(),
  h_sysSinPhiN(),
  h_sysRnN(),
  h_sysPsiN(),
  h_sysEccN(),

  h_sysCosPhiNVsImpact(),
  h_sysSinPhiNVsImpact(),
  h_sysRnNVsImpact(),
  h_sysPsiNVsImpact(),
  h_sysEccNVsImpact(),

  h_areaA(nullptr),
  h_areaB(nullptr),
  h_areaOverlap(nullptr),
  h_areaTotal(nullptr),
  h_areaAVsImpact(nullptr),
  h_areaBVsImpact(nullptr),
  h_areaOverlapVsImpact(nullptr),
  h_areaTotalVsImpact(nullptr)
  {
  appendClassName("GlauberHistos");
  }

  GlauberHistos::GlauberHistos(const GlauberHistos & src)
  :
  HistogramGroup(src),
  _fillBasics(src._fillBasics),
  _fillMoments(src._fillMoments),
  _fillSmeared(src._fillSmeared),
  _nOrders(src._nOrders),
  _nParts_nBins(src._nParts_nBins),
  _nBinaries_nBins(src._nBinaries_nBins),
  _nSpect_nBins(src._nSpect_nBins),
  _impact_nBins(src._impact_nBins),
  _cs_nBins(src._cs_nBins),
  _psi_nBins(src._psi_nBins),
  _ecc_nBins(src._ecc_nBins),
  _rN_nBins(src._rN_nBins),
  _psiN_nBins(src._psiN_nBins),
  _eccN_nBins(src._eccN_nBins),

  _nParts_min(src._nParts_min),
  _nBinaries_min(src._nBinaries_min),
  _nSpect_min(src._nSpect_min),
  _impact_min(src._impact_min),
  _cs_min(-src._cs_min),
  _psi_min(src._psi_min),
  _ecc_min(src._ecc_min),
  _rN_min(src._rN_min),
  _psiN_min(src._psiN_min),
  _eccN_min(src._eccN_min),

  _nParts_max(src._nParts_max),
  _nBinaries_max(src._nBinaries_max),
  _nSpect_max(src._nSpect_max),
  _impact_max(src._impact_max),
  _cs_max(src._cs_max),
  _psi_max(src._psi_max),
  _ecc_max(src._ecc_max),
  _rN_max(src._rN_max),
  _psiN_max(src._psiN_max),
  _eccN_max(src._eccN_max),

  h_impactPar(nullptr),
  h_xSection(nullptr),
  h_nParts(nullptr),
  h_nPartsA(nullptr),
  h_nPartsB(nullptr),
  h_nPartsProton(nullptr),
  h_nPartsProtonA(nullptr),
  h_nPartsProtonB(nullptr),
  h_nPartsNeutron(nullptr),
  h_nPartsNeutronA(nullptr),
  h_nPartsNeutronB(nullptr),

  h_nBinaries(nullptr),
  h_nBinariesA(nullptr),
  h_nBinariesB(nullptr),

  h_nSpect(nullptr),
  h_nSpectA(nullptr),
  h_nSpectB(nullptr),
  h_nSpectProton(nullptr),
  h_nSpectProtonA(nullptr),
  h_nSpectProtonB(nullptr),
  h_nSpectNeutron(nullptr),
  h_nSpectNeutronA(nullptr),
  h_nSpectNeutronB(nullptr),

  h_nPP(nullptr),
  h_nPN(nullptr),
  h_nNP(nullptr),
  h_nNN(nullptr),
  h_nHard(nullptr),

  h_nPartsVsImpact(),
  h_nBinariesVsImpact(),
  h_nBinariesVsNParts(nullptr),

  h_nPartsAvgVsImpact(),
  h_nPartsAAvgVsImpact(),
  h_nPartsBAvgVsImpact(),
  h_nPartsProtonAvgVsImpact(),
  h_nPartsProtonAAvgVsImpact(),
  h_nPartsProtonBAvgVsImpact(),
  h_nPartsNeutronAvgVsImpact(),
  h_nPartsNeutronAAvgVsImpact(),
  h_nPartsNeutronBAvgVsImpact(),

  h_nBinariesAvgVsImpact(),
  h_nBinariesAAvgVsImpact(),
  h_nBinariesBAvgVsImpact(),

  h_nSpectAvgVsImpact(),
  h_nSpectAAvgVsImpact(),
  h_nSpectBAvgVsImpact(),
  h_nSpectProtonAvgVsImpact(),
  h_nSpectProtonAAvgVsImpact(),
  h_nSpectProtonBAvgVsImpact(),
  h_nSpectNeutronAvgVsImpact(),
  h_nSpectNeutronAAvgVsImpact(),
  h_nSpectNeutronBAvgVsImpact(),

  h_nPPAvgVsImpact(),
  h_nPNAvgVsImpact(),
  h_nNPAvgVsImpact(),
  h_nNNAvgVsImpact(),
  h_nHardAvgVsImpact(),

  h_sysMeanX(nullptr),
  h_sysMeanY(nullptr),
  h_sysVarX(nullptr),
  h_sysVarY(nullptr),
  h_sysVarXY(nullptr),
  h_sysRmsX(nullptr),
  h_sysRmsY(nullptr),
  h_sysEccX(nullptr),
  h_sysEccY(nullptr),
  h_sysEcc(nullptr),
  h_sysPsi(nullptr),

  h_sysMeanXVsImpact(nullptr),
  h_sysMeanYVsImpact(nullptr),
  h_sysVarXVsImpact(nullptr),
  h_sysVarYVsImpact(nullptr),
  h_sysVarXYVsImpact(nullptr),
  h_sysRmsXVsImpact(nullptr),
  h_sysRmsYVsImpact(nullptr),
  h_sysEccXVsImpact(nullptr),
  h_sysEccYVsImpact(nullptr),
  h_sysEccVsImpact(nullptr),
  h_sysPsiVsImpact(nullptr),

  h_sysCosPhiN(),
  h_sysSinPhiN(),
  h_sysRnN(),
  h_sysPsiN(),
  h_sysEccN(),

  h_sysCosPhiNVsImpact(),
  h_sysSinPhiNVsImpact(),
  h_sysRnNVsImpact(),
  h_sysPsiNVsImpact(),
  h_sysEccNVsImpact(),

  h_areaA(nullptr),
  h_areaB(nullptr),
  h_areaOverlap(nullptr),
  h_areaTotal(nullptr),
  h_areaAVsImpact(nullptr),
  h_areaBVsImpact(nullptr),
  h_areaOverlapVsImpact(nullptr),
  h_areaTotalVsImpact(nullptr)
  {
  cloneAll(src);
  }

  GlauberHistos & GlauberHistos::operator=(const GlauberHistos & rhs)
  {
  if (this!=&rhs)
    {
    HistogramGroup::operator=(rhs);
    cloneAll(rhs);
    _fillBasics  = rhs._fillBasics;
    _fillMoments = rhs._fillMoments;
    _fillSmeared = rhs._fillSmeared;
    _fillArea    = rhs._fillArea;
    _nOrders     = rhs._nOrders;
    }
  return *this;
  }

  void GlauberHistos::configure(const String & taskName,
                                const String & objectType,
                                const Configuration & configuration,
                                unsigned int index)
  {
  HistogramGroup::configure(taskName,objectType,configuration,index);

  printValue("GlauberHistos::configure() taskName",taskName);
  printValue("GlauberHistos::configure() objectType",objectType);
  String type;
  if (objectType.Contains("HISTOGRAM"))
    type ="HISTOGRAM";
  else
    type = objectType;


  _fillBasics    = configuration.valueBool(   createKey(taskName,type,"fillBasics"));
  _fillMoments   = configuration.valueBool(   createKey(taskName,type,"fillMoments"));
  _fillSmeared   = configuration.valueBool(   createKey(taskName,type,"fillSmeared"));
  _drawEvents    = configuration.valueBool(   createKey(taskName,type,"drawEvents"));
  _drawEventsN   = configuration.valueInt(    createKey(taskName,type,"drawEvents:N"));
  _drawEventsIndex = 0;

  _nOrders       = configuration.valueInt(    createKey(taskName,type,"nOrders"));

  _nParts_nBins    = configuration.valueInt( createKey(taskName,type,"nParts_nBins"));
  _nBinaries_nBins = configuration.valueInt( createKey(taskName,type,"nBinaries_nBins"));
  _nSpect_nBins    = configuration.valueInt( createKey(taskName,type,"nSpect_nBins"));
  _impact_nBins    = configuration.valueInt( createKey(taskName,type,"impact_nBins"));
  _cs_nBins        = configuration.valueInt( createKey(taskName,type,"cs_nBins"));
  _psi_nBins       = configuration.valueInt( createKey(taskName,type,"psi_nBins"));
  _ecc_nBins       = configuration.valueInt( createKey(taskName,type,"ecc_nBins"));
  _rN_nBins        = configuration.valueInt( createKey(taskName,type,"rn_nBins"));
  _psiN_nBins      = configuration.valueInt( createKey(taskName,type,"psiN_nBins"));
  _eccN_nBins      = configuration.valueInt( createKey(taskName,type,"eccN_nBins"));
  _area_nBins      = configuration.valueInt( createKey(taskName,type,"area_nBins"));

  _nParts_min      = configuration.valueDouble( createKey(taskName,type,"nParts_min"));
  _nBinaries_min   = configuration.valueDouble( createKey(taskName,type,"nBinaries_min"));
  _nSpect_min      = configuration.valueDouble( createKey(taskName,type,"nSpect_min"));
  _impact_min      = configuration.valueDouble( createKey(taskName,type,"impact_min"));
  _cs_min          = configuration.valueDouble( createKey(taskName,type,"cs_min"));
  _psi_min         = configuration.valueDouble( createKey(taskName,type,"psi_min"));
  _ecc_min         = configuration.valueDouble( createKey(taskName,type,"ecc_min"));
  _rN_min          = configuration.valueDouble( createKey(taskName,type,"rn_min"));
  _psiN_min        = configuration.valueDouble( createKey(taskName,type,"psiN_min"));
  _eccN_min        = configuration.valueDouble( createKey(taskName,type,"eccN_min"));
  _area_min        = configuration.valueDouble( createKey(taskName,type,"area_min"));

  _nParts_max      = configuration.valueDouble( createKey(taskName,type,"nParts_max"));
  _nBinaries_max   = configuration.valueDouble( createKey(taskName,type,"nBinaries_max"));
  _nSpect_max      = configuration.valueDouble( createKey(taskName,type,"nSpect_max"));
  _impact_max      = configuration.valueDouble( createKey(taskName,type,"impact_max"));
  _cs_max          = configuration.valueDouble( createKey(taskName,type,"cs_max"));
  _psi_max         = configuration.valueDouble( createKey(taskName,type,"psi_max"));
  _ecc_max         = configuration.valueDouble( createKey(taskName,type,"ecc_max"));
  _rN_max          = configuration.valueDouble( createKey(taskName,type,"rn_max"));
  _psiN_max        = configuration.valueDouble( createKey(taskName,type,"psiN_max"));
  _eccN_max        = configuration.valueDouble( createKey(taskName,type,"eccN_max"));
  _area_max        = configuration.valueDouble( createKey(taskName,type,"area_max"));

  printValue("_fillBasics",_fillBasics);
  printValue("_fillMoments",_fillMoments);
  printValue("_fillSmeared",_fillSmeared);
  printValue("_drawEvents",_drawEvents);
  printValue("_drawEventsN",_drawEventsN);
  printValue("_drawEventsIndex",_drawEventsIndex);

  printValue("_nParts_nBins",_nParts_nBins);
  printValue("_nBinaries_nBins",_nBinaries_nBins);
  printValue("_nSpect_nBins",_nSpect_nBins);
  printValue("_impact_nBins",_impact_nBins);
  printValue("_cs_nBins",_cs_nBins);
  printValue("_psi_nBins",_psi_nBins);
  printValue("_ecc_nBins",_ecc_nBins);
  printValue("_rN_nBins",_rN_nBins);
  printValue("_psiN_nBins",_psiN_nBins);
  printValue("_eccN_nBins",_eccN_nBins);
  printValue("_area_nBins",_area_nBins);

  printValue("_nParts_min",_nParts_min);
  printValue("_nBinaries_min",_nBinaries_min);
  printValue("_nSpect_min",_nSpect_min);
  printValue("_impact_min",_impact_min);
  printValue("_cs_min",_cs_min);
  printValue("_psi_min",_psi_min);
  printValue("_ecc_min",_ecc_min);
  printValue("_rN_min",_rN_min);
  printValue("_psiN_min",_psiN_min);
  printValue("_eccN_min",_eccN_min);
  printValue("_area_min",_area_min);

  printValue("_nParts_max",_nParts_max);
  printValue("_nBinaries_max",_nBinaries_max);
  printValue("_nSpect_max",_nSpect_max);
  printValue("_impact_max",_impact_max);
  printValue("_cs_max",_cs_max);
  printValue("_psi_max",_psi_max);
  printValue("_ecc_max",_ecc_max);
  printValue("_rN_max",_rN_max);
  printValue("_psiN_max",_psiN_max);
  printValue("_eccN_max",_eccN_max);
  printValue("_area_max",_area_max);
 }

  void GlauberHistos::cloneAll(const GlauberHistos & src)
  {
  if (reportStart(__FUNCTION__)) { printCR(); }
  h_impactPar      = safeCloneH1(src.h_impactPar);
  h_nParts         = safeCloneH1(src.h_nParts);
  h_nPartsA        = safeCloneH1(src.h_nPartsA);
  h_nPartsB        = safeCloneH1(src.h_nPartsB);
  h_nPartsProton   = safeCloneH1(src.h_nPartsProton);
  h_nPartsProtonA  = safeCloneH1(src.h_nPartsProtonA);
  h_nPartsProtonB  = safeCloneH1(src.h_nPartsProtonB);
  h_nPartsNeutron  = safeCloneH1(src.h_nPartsNeutron);
  h_nPartsNeutronA = safeCloneH1(src.h_nPartsNeutronA);
  h_nPartsNeutronB = safeCloneH1(src.h_nPartsNeutronB);

  h_nBinaries = safeCloneH1(src.h_nBinaries);
  h_nBinariesA = safeCloneH1(src.h_nBinariesA);
  h_nBinariesB = safeCloneH1(src.h_nBinariesB);

  h_nSpect = safeCloneH1(src.h_nSpect);
  h_nSpectA = safeCloneH1(src.h_nSpectA);
  h_nSpectB = safeCloneH1(src.h_nSpectB);
  h_nSpectProton = safeCloneH1(src.h_nSpectProton);
  h_nSpectProtonA = safeCloneH1(src.h_nSpectProtonA);
  h_nSpectProtonB = safeCloneH1(src.h_nSpectProtonB);
  h_nSpectNeutron = safeCloneH1(src.h_nSpectNeutron);
  h_nSpectNeutronA = safeCloneH1(src.h_nSpectNeutronA);
  h_nSpectNeutronB = safeCloneH1(src.h_nSpectNeutronB);

  h_nPP = safeCloneH1(src.h_nPP);
  h_nPN = safeCloneH1(src.h_nPN);
  h_nNP = safeCloneH1(src.h_nNP);
  h_nNN = safeCloneH1(src.h_nNN);
  h_nHard = safeCloneH1(src.h_nHard);

  h_nPartsVsImpact = safeCloneH2(src.h_nPartsVsImpact);
  h_nBinariesVsImpact = safeCloneH2(src.h_nBinariesVsImpact);
  h_nBinariesVsNParts = safeCloneH2(src.h_nBinariesVsNParts);

  h_nPartsAvgVsImpact = safeCloneProfile(src.h_nPartsAvgVsImpact);
  h_nPartsAAvgVsImpact = safeCloneProfile(src.h_nPartsAAvgVsImpact);
  h_nPartsBAvgVsImpact = safeCloneProfile(src.h_nPartsBAvgVsImpact);
  h_nPartsProtonAvgVsImpact = safeCloneProfile(src.h_nPartsProtonAvgVsImpact);
  h_nPartsProtonAAvgVsImpact = safeCloneProfile(src.h_nPartsProtonAAvgVsImpact);
  h_nPartsProtonBAvgVsImpact = safeCloneProfile(src.h_nPartsProtonBAvgVsImpact);
  h_nPartsNeutronAvgVsImpact = safeCloneProfile(src.h_nPartsNeutronAvgVsImpact);
  h_nPartsNeutronAAvgVsImpact = safeCloneProfile(src.h_nPartsNeutronAAvgVsImpact);
  h_nPartsNeutronBAvgVsImpact = safeCloneProfile(src.h_nPartsNeutronBAvgVsImpact);

  h_nBinariesAvgVsImpact = safeCloneProfile(src.h_nBinariesAvgVsImpact);
  h_nBinariesAAvgVsImpact = safeCloneProfile(src.h_nBinariesAAvgVsImpact);
  h_nBinariesBAvgVsImpact = safeCloneProfile(src.h_nBinariesBAvgVsImpact);

  h_nSpectAvgVsImpact = safeCloneProfile(src.h_nSpectAvgVsImpact);
  h_nSpectAAvgVsImpact = safeCloneProfile(src.h_nSpectAAvgVsImpact);
  h_nSpectBAvgVsImpact = safeCloneProfile(src.h_nSpectBAvgVsImpact);
  h_nSpectProtonAvgVsImpact = safeCloneProfile(src.h_nSpectProtonAvgVsImpact);
  h_nSpectProtonAAvgVsImpact = safeCloneProfile(src.h_nSpectProtonAAvgVsImpact);
  h_nSpectProtonBAvgVsImpact = safeCloneProfile(src.h_nSpectProtonBAvgVsImpact);
  h_nSpectNeutronAvgVsImpact = safeCloneProfile(src.h_nSpectNeutronAvgVsImpact);
  h_nSpectNeutronAAvgVsImpact = safeCloneProfile(src.h_nSpectNeutronAAvgVsImpact);
  h_nSpectNeutronBAvgVsImpact = safeCloneProfile(src.h_nSpectNeutronBAvgVsImpact);

  h_nPPAvgVsImpact = safeCloneProfile(src.h_nPPAvgVsImpact);
  h_nPNAvgVsImpact = safeCloneProfile(src.h_nPNAvgVsImpact);
  h_nNPAvgVsImpact = safeCloneProfile(src.h_nNPAvgVsImpact);
  h_nNNAvgVsImpact = safeCloneProfile(src.h_nNNAvgVsImpact);
  h_nHardAvgVsImpact = safeCloneProfile(src.h_nHardAvgVsImpact);
  if (reportEnd(__FUNCTION__)) { printCR(); }
  }

  // for now use the same boundaries for eta and y histogram
  void GlauberHistos::create()
  {
  if (reportInfo(__FUNCTION__)) { /* noops*/ };

  if (_fillBasics)
    {
    h_impactPar      = createHistogram(createName(_histogramBaseName,"impact"),        _impact_nBins,     _impact_min,  _impact_max,"b (fm)","N",2);
    h_xSection       = createHistogram(createName(_histogramBaseName,"xSection"),      _impact_nBins,     _impact_min,  _impact_max,"b (fm)","N(bdb)",2);
    h_nParts         = createHistogram(createName(_histogramBaseName,"nParts"),        _nParts_nBins,     _nParts_min,     _nParts_max,"N_{part}","N",2);
    h_nPartsA        = createHistogram(createName(_histogramBaseName,"nPartsA"),       _nParts_nBins,     _nParts_min,     _nParts_max,"N_{part,A}","N",2);
    h_nPartsB        = createHistogram(createName(_histogramBaseName,"nPartsB"),       _nParts_nBins,     _nParts_min,     _nParts_max,"N_{part,B}","N",2);
    h_nPartsProton   = createHistogram(createName(_histogramBaseName,"nPartsProton"),  _nParts_nBins,     _nParts_min,     _nParts_max,"N_{part}^{p}","N",2);
    h_nPartsProtonA  = createHistogram(createName(_histogramBaseName,"nPartsProtonA"), _nParts_nBins,     _nParts_min,     _nParts_max,"N_{part,A}^{p}","N",2);
    h_nPartsProtonB  = createHistogram(createName(_histogramBaseName,"nPartsProtonB"), _nParts_nBins,     _nParts_min,     _nParts_max,"N_{part,B}^{p}","N",2);
    h_nPartsNeutron  = createHistogram(createName(_histogramBaseName,"nPartsNeutron"), _nParts_nBins,     _nParts_min,     _nParts_max,"N_{part}^{n}","N",2);
    h_nPartsNeutronA = createHistogram(createName(_histogramBaseName,"nPartsNeutronA"),_nParts_nBins,     _nParts_min,     _nParts_max,"N_{part,A}^{n}","N",2);
    h_nPartsNeutronB = createHistogram(createName(_histogramBaseName,"nPartsNeutronB"),_nParts_nBins,     _nParts_min,     _nParts_max,"N_{part,B}^{n}","N",2);

    h_nBinaries         = createHistogram(createName(_histogramBaseName,"nBinaries"), _nBinaries_nBins,  _nBinaries_min,  _nBinaries_max,"N_{bin}","N",2);
    h_nBinariesA        = createHistogram(createName(_histogramBaseName,"nBinariesA"),_nBinaries_nBins,  _nBinaries_min,  _nBinaries_max,"N_{bin,A}","N",2);
    h_nBinariesB        = createHistogram(createName(_histogramBaseName,"nBinariesB"),_nBinaries_nBins,  _nBinaries_min,  _nBinaries_max,"N_{bin,B}","N",2);

    h_nBinariesAvgVsImpact         = createProfile(createName(_histogramBaseName,"nBinariesAvgVsImpact"),   _impact_nBins,  _impact_min,  _impact_max,"b (fm)","<N_{bin}>");
    h_nBinariesAAvgVsImpact        = createProfile(createName(_histogramBaseName,"nBinariesAAvgVsImpact"),  _impact_nBins,  _impact_min,  _impact_max,"b (fm)","<N_{bin,A}>");
    h_nBinariesBAvgVsImpact        = createProfile(createName(_histogramBaseName,"nBinariesBAvgVsImpact"),  _impact_nBins,  _impact_min,  _impact_max,"b (fm)","<N_{bin,B}>");

    h_nPartsVsImpact     = createHistogram(createName(_histogramBaseName,"nPartsVsImpact"),        _impact_nBins,  _impact_min,  _impact_max, _nParts_nBins,    _nParts_min,   _nParts_max,"b (fm)","N_{part}","N",2);
    h_nBinariesVsImpact  = createHistogram(createName(_histogramBaseName,"nBinariesVsImpact"),     _impact_nBins,  _impact_min,  _impact_max, _nBinaries_nBins, _nBinaries_min,_nBinaries_max,"b (fm)","N_{bins}","N",2);

    h_nPartsAvgVsImpact     = createProfile(createName(_histogramBaseName,"nPartsAvgVsImpact"),    _impact_nBins,  _impact_min,  _impact_max,"b (fm)","<N_{part}>");
    h_nPartsAAvgVsImpact    = createProfile(createName(_histogramBaseName,"nPartsAAvgVsImpact"),   _impact_nBins,  _impact_min,  _impact_max,"b (fm)","<N_{part,A}>");
    h_nPartsBAvgVsImpact    = createProfile(createName(_histogramBaseName,"nPartsBAvgVsImpact"),   _impact_nBins,  _impact_min,  _impact_max,"b (fm)","<N_{part,B}>");
    h_nPartsProtonAvgVsImpact    = createProfile(createName(_histogramBaseName,"nPartsProtonAvgVsImpact"),     _impact_nBins,  _impact_min,  _impact_max,"b (fm)","<N_{part}^{p}>");
    h_nPartsProtonAAvgVsImpact   = createProfile(createName(_histogramBaseName,"nPartsProtonAAvgVsImpact"),    _impact_nBins,  _impact_min,  _impact_max,"b (fm)","<N_{part,A}^{p}>");
    h_nPartsProtonBAvgVsImpact   = createProfile(createName(_histogramBaseName,"nPartsProtonBAvgVsImpact"),    _impact_nBins,  _impact_min,  _impact_max,"b (fm)","<N_{part,B}^{p}>");
    h_nPartsNeutronAvgVsImpact   = createProfile(createName(_histogramBaseName,"nPartsNeutronAvgVsImpact"),    _impact_nBins,  _impact_min,  _impact_max,"b (fm)","<N_{part}>^{n}");
    h_nPartsNeutronAAvgVsImpact  = createProfile(createName(_histogramBaseName,"nPartsNeutronAAvgVsImpact"),   _impact_nBins,  _impact_min,  _impact_max,"b (fm)","<N_{part,A}>^{n}");
    h_nPartsNeutronBAvgVsImpact  = createProfile(createName(_histogramBaseName,"nPartsNeutronBAvgVsImpact"),   _impact_nBins,  _impact_min,  _impact_max,"b (fm)","<N_{part,B}^{n}>");

    h_nSpect         = createHistogram(createName(_histogramBaseName,"nSpect"), _nSpect_nBins,  _nSpect_min,  _nSpect_max,"N_{spec}","N",2);
    h_nSpectA        = createHistogram(createName(_histogramBaseName,"nSpectA"), _nSpect_nBins,  _nSpect_min,  _nSpect_max,"N_{spec,A}","N",2);
    h_nSpectB        = createHistogram(createName(_histogramBaseName,"nSpectB"), _nSpect_nBins,  _nSpect_min,  _nSpect_max,"N_{spec,B}","N",2);
    h_nSpectProton   = createHistogram(createName(_histogramBaseName,"nSpectProton"), _nSpect_nBins,  _nSpect_min,  _nSpect_max,"N_{spec}^{p}","N",2);
    h_nSpectProtonA  = createHistogram(createName(_histogramBaseName,"nSpectProtonA"), _nSpect_nBins,  _nSpect_min,  _nSpect_max,"N_{spec,A}^{p}","N",2);
    h_nSpectProtonB  = createHistogram(createName(_histogramBaseName,"nSpectProtonB"), _nSpect_nBins,  _nSpect_min,  _nSpect_max,"N_{spec,B}^{p}","N",2);
    h_nSpectNeutron  = createHistogram(createName(_histogramBaseName,"nSpectNeutron"), _nSpect_nBins,  _nSpect_min,  _nSpect_max,"N_{spec}^{n}","N",2);
    h_nSpectNeutronA = createHistogram(createName(_histogramBaseName,"nSpectNeutronA"), _nSpect_nBins,  _nSpect_min,  _nSpect_max,"N_{spec,A}^{n}","N",2);
    h_nSpectNeutronB = createHistogram(createName(_histogramBaseName,"nSpectNeutronB"), _nSpect_nBins,  _nSpect_min,  _nSpect_max,"N_{spec,B}^{n}","N",2);

    h_nSpectAvgVsImpact             = createProfile(createName(_histogramBaseName,"nSpectAvgVsImpact"),          _impact_nBins,  _impact_min,  _impact_max,"b (fm)","<N_{spect}>");
    h_nSpectAAvgVsImpact            = createProfile(createName(_histogramBaseName,"nSpectAAvgVsImpact"),         _impact_nBins,  _impact_min,  _impact_max,"b (fm)","<N_{spect,A}>");
    h_nSpectBAvgVsImpact            = createProfile(createName(_histogramBaseName,"nSpectBAvgVsImpact"),         _impact_nBins,  _impact_min,  _impact_max,"b (fm)","<N_{spect,B}>");
    h_nSpectProtonAvgVsImpact       = createProfile(createName(_histogramBaseName,"nSpectProtonAvgVsImpact"),    _impact_nBins,  _impact_min,  _impact_max,"b (fm)","<N_{spect}^{p}>");
    h_nSpectProtonAAvgVsImpact      = createProfile(createName(_histogramBaseName,"nSpectProtonAAvgVsImpact"),   _impact_nBins,  _impact_min,  _impact_max,"b (fm)","<N_{spect,A}^{p}>");
    h_nSpectProtonBAvgVsImpact      = createProfile(createName(_histogramBaseName,"nSpectProtonBAvgVsImpact"),   _impact_nBins,  _impact_min,  _impact_max,"b (fm)","<N_{spect,B}^{p}>");
    h_nSpectNeutronAvgVsImpact      = createProfile(createName(_histogramBaseName,"nSpectNeutronAvgVsImpact"),   _impact_nBins,  _impact_min,  _impact_max,"b (fm)","<N_{spect}^{n}>");
    h_nSpectNeutronAAvgVsImpact     = createProfile(createName(_histogramBaseName,"nSpectNeutronAAvgVsImpact"),  _impact_nBins,  _impact_min,  _impact_max,"b (fm)","<N_{spect,A}^{n}>");
    h_nSpectNeutronBAvgVsImpact     = createProfile(createName(_histogramBaseName,"nSpectNeutronBAvgVsImpact"),  _impact_nBins,  _impact_min,  _impact_max,"b (fm)","<N_{spect,B}^{n}>");


    h_nPP   = createHistogram(createName(_histogramBaseName,"nPP"),   _nBinaries_nBins,  _nBinaries_min,  _nBinaries_max,"N_{pp}","N",2);
    h_nPN   = createHistogram(createName(_histogramBaseName,"nPN"),   _nBinaries_nBins,  _nBinaries_min,  _nBinaries_max,"N_{pn}","N",2);
    h_nNP   = createHistogram(createName(_histogramBaseName,"nNP"),   _nBinaries_nBins,  _nBinaries_min,  _nBinaries_max,"N_{np}","N",2);
    h_nNN   = createHistogram(createName(_histogramBaseName,"nNN"),   _nBinaries_nBins,  _nBinaries_min,  _nBinaries_max,"N_{nn}","N",2);
    h_nHard = createHistogram(createName(_histogramBaseName,"nHard"), _nBinaries_nBins,  _nBinaries_min,  _nBinaries_max,"N_{hard}","N",2);

    h_nPPAvgVsImpact = createProfile(createName(_histogramBaseName,"nPPAvgVsImpact"), _impact_nBins,  _impact_min,  _impact_max,"b (fm)","<N_{pp}>");
    h_nPNAvgVsImpact = createProfile(createName(_histogramBaseName,"nPNAvgVsImpact"), _impact_nBins,  _impact_min,  _impact_max,"b (fm)","<N_{pn}>");
    h_nNPAvgVsImpact = createProfile(createName(_histogramBaseName,"nNPAvgVsImpact"), _impact_nBins,  _impact_min,  _impact_max,"b (fm)","<N_{np}>");
    h_nNNAvgVsImpact = createProfile(createName(_histogramBaseName,"nNNAvgVsImpact"), _impact_nBins,  _impact_min,  _impact_max,"b (fm)","<N_{nn}>");
    h_nHardAvgVsImpact = createProfile(createName(_histogramBaseName,"nHardAvgVsImpact"), _impact_nBins,  _impact_min,  _impact_max,"b (fm)","<N_{hard}>");

    }

  if (_fillMoments)
    {

    // system==participants as a whole
    h_sysMeanX = createHistogram(createName(_histogramBaseName,"sysMeanX"), _impact_nBins,_impact_min,_impact_max,"<x> (fm)","N");
    h_sysMeanY = createHistogram(createName(_histogramBaseName,"sysMeanY"), _impact_nBins,_impact_min,_impact_max,"<y> (fm)","N");
    h_sysVarX  = createHistogram(createName(_histogramBaseName,"sysVarX"),  _impact_nBins,_impact_min,_impact_max,"<x^{2}> (fm^{2})","N");
    h_sysVarY  = createHistogram(createName(_histogramBaseName,"sysVarY"),  _impact_nBins,_impact_min,_impact_max,"<y^{2}> (fm^{2})","N");
    h_sysVarXY = createHistogram(createName(_histogramBaseName,"sysVarXY"), _impact_nBins,_impact_min,_impact_max,"<xy> (fm^{2})","N");
    h_sysRmsX  = createHistogram(createName(_histogramBaseName,"sysRmsX"),  _impact_nBins,_impact_min,_impact_max,"<#Delta x^{2}>^{1/2} (fm)","N");
    h_sysRmsY  = createHistogram(createName(_histogramBaseName,"sysRmsY"),  _impact_nBins,_impact_min,_impact_max,"<#Delta y^{2}>^{1/2} (fm)","N");
    h_sysEccX  = createHistogram(createName(_histogramBaseName,"sysEccX"),  _ecc_nBins,_ecc_min,_ecc_max,"#varepsilon_{x}","N");
    h_sysEccY  = createHistogram(createName(_histogramBaseName,"sysEccY"),  _ecc_nBins,_ecc_min,_ecc_max,"#varepsilon_{y}","N");
    h_sysEcc   = createHistogram(createName(_histogramBaseName,"sysEcc"),   _ecc_nBins,_ecc_min,_ecc_max,"#varepsilon","N");
    h_sysPsi   = createHistogram(createName(_histogramBaseName,"sysPsi"),   _psi_nBins,_psi_min,_psi_max,"#psi","N");

    h_sysMeanXVsImpact  = createProfile(createName(_histogramBaseName,"sysMeanXVsImpact"),  _impact_nBins,_impact_min,_impact_max,"b (fm)","<x> (fm)");
    h_sysMeanYVsImpact  = createProfile(createName(_histogramBaseName,"sysMeanYVsImpact"),  _impact_nBins,_impact_min,_impact_max,"b (fm)","<y> (fm)");
    h_sysVarXVsImpact   = createProfile(createName(_histogramBaseName,"sysVarXVsImpact"),   _impact_nBins,_impact_min,_impact_max,"b (fm)","<#Delta x^{2}> (fm^{2})");
    h_sysVarYVsImpact   = createProfile(createName(_histogramBaseName,"sysVarYVsImpact"),   _impact_nBins,_impact_min,_impact_max,"b (fm)","<#Delta y^{2}> (fm^{2})");
    h_sysVarXYVsImpact  = createProfile(createName(_histogramBaseName,"sysVarXYVsImpact"),  _impact_nBins,_impact_min,_impact_max,"b (fm)","<#Delta x#Delta y> (fm^{2})");
    h_sysRmsXVsImpact   = createProfile(createName(_histogramBaseName,"sysRmsXVsImpact"),   _impact_nBins,_impact_min,_impact_max,"b (fm)","<#Delta x^{2}>^{1/2} (fm)");
    h_sysRmsYVsImpact   = createProfile(createName(_histogramBaseName,"sysRmsYVsImpact"),   _impact_nBins,_impact_min,_impact_max,"b (fm)","<#Delta y^{2}>^{1/2} (fm)");
    h_sysEccXVsImpact   = createProfile(createName(_histogramBaseName,"sysEccXVsImpact"),   _impact_nBins,_impact_min,_impact_max,"b (fm)","#varepsilon_{x}");
    h_sysEccYVsImpact   = createProfile(createName(_histogramBaseName,"sysEccYVsImpact"),   _impact_nBins,_impact_min,_impact_max,"b (fm)","#varepsilon_{y}");
    h_sysEccVsImpact    = createProfile(createName(_histogramBaseName,"sysEccVsImpact"),    _impact_nBins,_impact_min,_impact_max,"b (fm)","#varepsilon");
    h_sysPsiVsImpact    = createProfile(createName(_histogramBaseName,"sysPsiVsImpact"),    _impact_nBins,_impact_min,_impact_max,"b (fm)","#psi");
    for (int iOrder=0; iOrder<_nOrders; ++iOrder)
      {
      String s = "";
      s += iOrder+1;
      h_sysCosPhiN.push_back( createHistogram(createName(_histogramBaseName,s,"sysCosPhiN"), _cs_nBins,  _cs_min,  _cs_max,  "#cos N#phi","N") );
      h_sysSinPhiN.push_back( createHistogram(createName(_histogramBaseName,s,"sysSinPhiN"), _cs_nBins,  _cs_min,  _cs_max,  "#sin N#phi","N") );
      h_sysRnN.push_back(     createHistogram(createName(_histogramBaseName,s,"sysRn"),      _rN_nBins,  _rN_min,  _rN_max,  "r^{n}","N") );
      h_sysPsiN.push_back(    createHistogram(createName(_histogramBaseName,s,"sysPsiN"),    _psiN_nBins,_psiN_min,_psiN_max,"#psi_{n}","N") );
      h_sysEccN.push_back(    createHistogram(createName(_histogramBaseName,s,"sysEccN"),    _eccN_nBins,_eccN_min,_eccN_max,"#epsilon_{n}","N") );

      h_sysCosPhiNVsImpact.push_back( createProfile(createName(_histogramBaseName,s,"sysCosPhiNVsImpact"), _impact_nBins,_impact_min,_impact_max,"b (fm)","#cos N#phi") );
      h_sysSinPhiNVsImpact.push_back( createProfile(createName(_histogramBaseName,s,"sysSinPhiVsImpact"),  _impact_nBins,_impact_min,_impact_max,"b (fm)","#sin N#phi") );
      h_sysRnNVsImpact.push_back(     createProfile(createName(_histogramBaseName,s,"sysRnVsImpact"),      _impact_nBins,_impact_min,_impact_max,"b (fm)","r^{n}"    ) );
      h_sysPsiNVsImpact.push_back(    createProfile(createName(_histogramBaseName,s, "sysPsiNVsImpact"),   _impact_nBins,_impact_min,_impact_max,"b (fm)","#psi_{n}") );
      h_sysEccNVsImpact.push_back(    createProfile(createName(_histogramBaseName,s, "sysEccNVsImpact"),   _impact_nBins,_impact_min,_impact_max,"b (fm)","#epsilon_{n}") );
      }
    }

  if (_fillArea)
    {
    h_areaA         = createHistogram(createName(_histogramBaseName,"AreaA"),       _area_nBins,_area_min,_area_max,_area_nBins,_area_min,_area_max,"x(fm)", "y(fm)","P(Area)");
    h_areaB         = createHistogram(createName(_histogramBaseName,"AreaB"),       _area_nBins,_area_min,_area_max,_area_nBins,_area_min,_area_max,"x(fm)", "y(fm)","P(Area)");
    h_areaOverlap   = createHistogram(createName(_histogramBaseName,"AreaOverlap"), _area_nBins,_area_min,_area_max, "Area_{O} (fm^{2})","P(Area)");
    h_areaTotal     = createHistogram(createName(_histogramBaseName,"areaTotal"),   _area_nBins,_area_min,_area_max, "Area_{T} (fm^{2})","P(Area)");
    h_areaAVsImpact       = createProfile(createName(_histogramBaseName,"areaAVsImpact"),       _impact_nBins,_impact_min,_impact_max,"b (fm)","Area_{A} (fm^{2})");
    h_areaBVsImpact       = createProfile(createName(_histogramBaseName,"areaBVsImpact"),       _impact_nBins,_impact_min,_impact_max,"b (fm)","Area_{B} (fm^{2})");
    h_areaOverlapVsImpact = createProfile(createName(_histogramBaseName,"areaOverlapVsImpact"), _impact_nBins,_impact_min,_impact_max,"b (fm)","Area_{O} (fm^{2})");
    h_areaTotalVsImpact   = createProfile(createName(_histogramBaseName,"areaTotalVsImpact"),   _impact_nBins,_impact_min,_impact_max,"b (fm)","Area_{T} (fm^{2})");
    }



  }

  void GlauberHistos::drawEvent(GlauberEvent & event, const String & eventName)
  {
  float xHalfWidth = 14.0;
  float yHalfWidth = 14.0;
  TH2 * h = createNewHistogram(eventName,1, -xHalfWidth, xHalfWidth, 1, -yHalfWidth, yHalfWidth, "x(fm)", "y(fm)","");
  TCanvas * canvas = new TCanvas(eventName,eventName,5,5,800, 800);
  double protonRadius = 0.5*sqrt(event.eventXSection()/(pi()*10));
  h->Draw();
  TEllipse e;
  e.SetFillColor(0);
  e.SetFillStyle(0);
  e.SetLineColor(1);
  e.SetLineStyle(2);
  e.SetLineWidth(1);

  double halfImpact = 0.5*event.impactParameter();
  GlauberNucleus & nucleusA = event.nucleusA();
  GlauberNucleus & nucleusB = event.nucleusB();
  std::vector<GlauberNucleon> & nucleonsA = nucleusA.allNucleons();
  std::vector<GlauberNucleon> & nucleonsB = nucleusB.allNucleons();
  double radiusA =  nucleusA.type().radiusProton();
  double radiusB =  nucleusB.type().radiusProton();
  e.DrawEllipse(halfImpact, 0.0,radiusA,radiusA,0,360,0);
  e.DrawEllipse(-halfImpact,0.0,radiusB,radiusB,0,360,0);

  e.SetLineColor(2);
  e.SetFillColor(2);
  double r = protonRadius;
  for (auto nucleon : nucleonsA)
    {
    double x = nucleon.position().x();
    double y = nucleon.position().y();
    if (nucleon.isWounded())
      e.SetFillStyle(1001);
    else
      e.SetFillStyle(0);
     e.DrawEllipse(x,y,r,r,0.0,360.0,0);
    }
  e.SetLineColor(4);
  e.SetFillColor(4);
  for (auto nucleon : nucleonsB)
    {
    double x = nucleon.position().x();
    double y = nucleon.position().y();
    if (nucleon.isWounded())
      e.SetFillStyle(1001);
    else
      e.SetFillStyle(0);
    e.DrawEllipse(x,y,r,r,0.0,360.0,0);
    }

  std::vector<GlauberPoint> & intPoints = event.interactionPositions();
  r = 0.25;
  e.SetFillStyle(1001);
  e.SetLineColor(1);
  e.SetFillColor(1);
  for (auto point : intPoints)
    {
    double x = point.x();
    double y = point.y();
    e.DrawEllipse(x,y,r,r,0.0,360.0,0);
    }

  canvas->Print(eventName+".pdf");
  delete h;
  }


  void GlauberHistos::loadFrom(TFile & inputFile)
  {
  if (reportStart(__FUNCTION__)) { /* noops*/ };
  h_impactPar      = importH1(inputFile, createName(_histogramBaseName,"impact"));
  h_nParts         = importH1(inputFile, createName(_histogramBaseName,"nParts"));
  h_nPartsA        = importH1(inputFile, createName(_histogramBaseName,"nPartsA"));
  h_nPartsB        = importH1(inputFile, createName(_histogramBaseName,"nPartsB"));
  h_nPartsProton   = importH1(inputFile, createName(_histogramBaseName,"nPartsProton"));
  h_nPartsProtonA  = importH1(inputFile, createName(_histogramBaseName,"nPartsProtonA"));
  h_nPartsProtonB  = importH1(inputFile, createName(_histogramBaseName,"nPartsProtonB"));
  h_nPartsNeutron  = importH1(inputFile, createName(_histogramBaseName,"nPartsNeutron"));
  h_nPartsNeutronA = importH1(inputFile, createName(_histogramBaseName,"nPartsNeutronA"));
  h_nPartsNeutronB = importH1(inputFile, createName(_histogramBaseName,"nPartsNeutronB"));

  h_nBinaries         = importH1(inputFile, createName(_histogramBaseName,"nBinaries"));
  h_nBinariesA        = importH1(inputFile, createName(_histogramBaseName,"nBinariesA"));
  h_nBinariesB        = importH1(inputFile, createName(_histogramBaseName,"nBinariesB"));

  h_nPartsVsImpact     = importH2(inputFile, createName(_histogramBaseName,"nPartsVsImpact"));
  h_nBinariesVsImpact  = importH2(inputFile, createName(_histogramBaseName,"nBinariesVsImpact"));

  h_nPartsAvgVsImpact     = importProfile(inputFile, createName(_histogramBaseName,"nPartsAvgVsImpact"));
  h_nPartsAAvgVsImpact    = importProfile(inputFile, createName(_histogramBaseName,"nPartsAAvgVsImpact"));
  h_nPartsBAvgVsImpact    = importProfile(inputFile, createName(_histogramBaseName,"nPartsBAvgVsImpact"));
  h_nPartsProtonAvgVsImpact    = importProfile(inputFile, createName(_histogramBaseName,"nPartsProtonAvgVsImpact"));
  h_nPartsProtonAAvgVsImpact   = importProfile(inputFile, createName(_histogramBaseName,"nPartsProtonAAvgVsImpact"));
  h_nPartsProtonBAvgVsImpact   = importProfile(inputFile, createName(_histogramBaseName,"nPartsProtonBAvgVsImpact"));
  h_nPartsNeutronAvgVsImpact   = importProfile(inputFile, createName(_histogramBaseName,"nPartsNeutronAvgVsImpact"));
  h_nPartsNeutronAAvgVsImpact  = importProfile(inputFile, createName(_histogramBaseName,"nPartsNeutronAAvgVsImpact"));
  h_nPartsNeutronBAvgVsImpact  = importProfile(inputFile, createName(_histogramBaseName,"nPartsNeutronBAvgVsImpact"));

  h_nBinariesAvgVsImpact         = importProfile(inputFile, createName(_histogramBaseName,"nBinariesAvgVsImpact"));
  h_nBinariesAAvgVsImpact        = importProfile(inputFile, createName(_histogramBaseName,"nBinariesAAvgVsImpact"));
  h_nBinariesBAvgVsImpact        = importProfile(inputFile, createName(_histogramBaseName,"nBinariesBAvgVsImpact"));

  h_nSpectAvgVsImpact             = importProfile(inputFile, createName(_histogramBaseName,"nSpectAvgVsImpact"));
  h_nSpectAAvgVsImpact            = importProfile(inputFile, createName(_histogramBaseName,"nSpectAAvgVsImpact"));
  h_nSpectBAvgVsImpact            = importProfile(inputFile, createName(_histogramBaseName,"nSpectBAvgVsImpact"));
  h_nSpectProtonAvgVsImpact       = importProfile(inputFile, createName(_histogramBaseName,"nSpectProtonAvgVsImpact"));
  h_nSpectProtonAAvgVsImpact      = importProfile(inputFile, createName(_histogramBaseName,"nSpectProtonAAvgVsImpact"));
  h_nSpectProtonBAvgVsImpact      = importProfile(inputFile, createName(_histogramBaseName,"nSpectProtonBAvgVsImpact"));
  h_nSpectNeutronAvgVsImpact      = importProfile(inputFile, createName(_histogramBaseName,"nSpectNeutronAvgVsImpact"));
  h_nSpectNeutronAAvgVsImpact     = importProfile(inputFile, createName(_histogramBaseName,"nSpectNeutronAAvgVsImpact"));
  h_nSpectNeutronBAvgVsImpact     = importProfile(inputFile, createName(_histogramBaseName,"nSpectNeutronBAvgVsImpact"));

  h_nHardAvgVsImpact             = importProfile(inputFile, createName(_histogramBaseName,"h_nHardAvgVsImpact"));
  }

  //!
  //! Fiil  single particle histograms of this class with the particles contained in the given list.
  //!
  void GlauberHistos::fill(GlauberEvent & event, double weight)
  {
  double impact            =  event.impactParameter();
  double nBinaries         =  event.nCollisions();
  double nBinariesA        =  event.nCollisionsA();
  double nBinariesB        =  event.nCollisionsB();

  double nParts           =  event.nParticipants();
  double nPartsProton     =  event.nProtonParticipants();
  double nPartsNeutron    =  event.nNeutronParticipants();
  double nPartsA          =  event.nParticipantsA();
  double nPartsProtonA    =  event.nProtonParticipantsA();
  double nPartsNeutronA   =  event.nNeutronParticipantsA();
  double nPartsB          =  event.nParticipantsB();
  double nPartsProtonB    =  event.nProtonParticipantsB();
  double nPartsNeutronB   =  event.nNeutronParticipantsB();

  double nSpect          =  event.nSpectators();
  double nSpectA         =  event.nSpectatorsA();
  double nSpectB         =  event.nSpectatorsB();
  double nSpectNeutron   =  event.nNeutronSpectators();
  double nSpectNeutronA  =  event.nNeutronSpectatorsA();
  double nSpectNeutronB  =  event.nNeutronSpectatorsB();
  double nSpectProton    =  event.nProtonSpectators();
  double nSpectProtonA   =  event.nProtonSpectatorsA();
  double nSpectProtonB   =  event.nProtonSpectatorsB();

  double nPP   =  event.nPPCollisions();
  double nPN   =  event.nPNCollisions();
  double nNP   =  event.nNPCollisions();
  double nNN   =  event.nNNCollisions();
  double nHard =  event.nHardCollisions();

  if (_fillBasics)
    {
    h_impactPar->Fill(impact,weight);
    h_xSection->Fill(impact,impact);

    h_nParts->Fill(nParts,weight);
    h_nPartsA->Fill(nPartsA,weight);
    h_nPartsB->Fill(nPartsB,weight);
    h_nPartsProton->Fill(nPartsProton,weight);
    h_nPartsProtonA->Fill(nPartsProtonA,weight);
    h_nPartsProtonB->Fill(nPartsProtonB,weight);
    h_nPartsNeutron->Fill(nPartsNeutron,weight);
    h_nPartsNeutronA->Fill(nPartsNeutronA,weight);
    h_nPartsNeutronB->Fill(nPartsNeutronB,weight);

    h_nPartsVsImpact->Fill(impact,nParts,weight);
    h_nPartsAvgVsImpact->Fill(impact,nParts,weight);
    h_nPartsAAvgVsImpact->Fill(impact,nPartsA,weight);
    h_nPartsBAvgVsImpact->Fill(impact,nPartsB,weight);
    h_nPartsProtonAvgVsImpact->Fill(impact,nPartsProton,weight);
    h_nPartsProtonAAvgVsImpact->Fill(impact,nPartsProtonA,weight);
    h_nPartsProtonBAvgVsImpact->Fill(impact,nPartsProtonB,weight);
    h_nPartsNeutronAvgVsImpact->Fill(impact,nPartsNeutron,weight);
    h_nPartsNeutronAAvgVsImpact->Fill(impact,nPartsNeutronA,weight);
    h_nPartsNeutronBAvgVsImpact->Fill(impact,nPartsNeutronB,weight);

    h_nBinaries->Fill(nBinaries,weight);
    h_nBinariesA->Fill(nBinariesA,weight);
    h_nBinariesB->Fill(nBinariesB,weight);
    h_nBinariesVsImpact->Fill(impact,nBinaries,weight);
    h_nBinariesAvgVsImpact->Fill(impact,nBinaries,weight);
    h_nBinariesAAvgVsImpact->Fill(impact,nBinariesA,weight);
    h_nBinariesBAvgVsImpact->Fill(impact,nBinariesB,weight);

    h_nPP->Fill(nPP,weight);
    h_nPN->Fill(nPN,weight);
    h_nNP->Fill(nNP,weight);
    h_nNN->Fill(nNN,weight);
    h_nHard->Fill(nHard,weight);

    h_nPPAvgVsImpact->Fill(impact,nPP,weight);
    h_nPNAvgVsImpact->Fill(impact,nPN,weight);
    h_nNPAvgVsImpact->Fill(impact,nNP,weight);
    h_nNNAvgVsImpact->Fill(impact,nNN,weight);
    h_nHardAvgVsImpact->Fill(impact,nHard,weight);

    h_nSpect->Fill(nSpect,weight);
    h_nSpectA->Fill(nSpectA,weight);
    h_nSpectB->Fill(nSpectB,weight);
    h_nSpectProton->Fill(nSpectProton,weight);
    h_nSpectProtonA->Fill(nSpectProtonA,weight);
    h_nSpectProtonB->Fill(nSpectProtonB,weight);
    h_nSpectNeutron->Fill(nSpectNeutron,weight);
    h_nSpectNeutronA->Fill(nSpectNeutronA,weight);
    h_nSpectNeutronB->Fill(nSpectNeutronB,weight);

    h_nSpectAvgVsImpact->Fill(impact,nSpect,weight);
    h_nSpectAAvgVsImpact->Fill(impact,nSpectA,weight);
    h_nSpectBAvgVsImpact->Fill(impact,nSpectB,weight);
    h_nSpectProtonAvgVsImpact->Fill(impact,nSpectProton,weight);
    h_nSpectProtonAAvgVsImpact->Fill(impact,nSpectProtonA,weight);
    h_nSpectProtonBAvgVsImpact->Fill(impact,nSpectProtonB,weight);
    h_nSpectNeutronAvgVsImpact->Fill(impact,nSpectNeutron,weight);
    h_nSpectNeutronAAvgVsImpact->Fill(impact,nSpectNeutronA,weight);
    h_nSpectNeutronBAvgVsImpact->Fill(impact,nSpectNeutronB,weight);

    }
  if (_fillMoments)
    {
    const GlauberEventMoments & momentsSystem   = event.momentsSystem();

    //momentsSystem
    h_sysMeanX   ->Fill(momentsSystem._meanX,weight);
    h_sysMeanY   ->Fill(momentsSystem._meanY,weight);
    h_sysVarX    ->Fill(momentsSystem._varX,weight);
    h_sysVarY    ->Fill(momentsSystem._varY,weight);
    h_sysVarXY   ->Fill(momentsSystem._varXY,weight);
    h_sysRmsX    ->Fill(momentsSystem._rmsX,weight);
    h_sysRmsY    ->Fill(momentsSystem._rmsY,weight);
    h_sysEccX    ->Fill(momentsSystem._eccX,weight);
    h_sysEccY    ->Fill(momentsSystem._eccY,weight);
    h_sysEcc     ->Fill(momentsSystem._ecc,weight);
    h_sysPsi     ->Fill(momentsSystem._psi,weight);
    h_sysMeanXVsImpact   ->Fill(impact,momentsSystem._meanX);
    h_sysMeanYVsImpact   ->Fill(impact,momentsSystem._meanY);
    h_sysVarXVsImpact    ->Fill(impact,momentsSystem._varX);
    h_sysVarYVsImpact    ->Fill(impact,momentsSystem._varY);
    h_sysVarXYVsImpact   ->Fill(impact,momentsSystem._varXY);
    h_sysRmsXVsImpact    ->Fill(impact,momentsSystem._rmsX);
    h_sysRmsYVsImpact    ->Fill(impact,momentsSystem._rmsY);
    h_sysEccXVsImpact    ->Fill(impact,momentsSystem._eccX);
    h_sysEccYVsImpact    ->Fill(impact,momentsSystem._eccY);
    h_sysEccVsImpact     ->Fill(impact,momentsSystem._ecc);
    h_sysPsiVsImpact     ->Fill(impact,momentsSystem._psi);
//    printValue("Check",4);
//    printValue("_nOrders",_nOrders);
//    for (int iOrder=0; iOrder<_nOrders; ++iOrder)
//      {
//      printValue("iOrder",iOrder);
//      h_sysCosPhiN[iOrder]->Fill(momentsSystem._cosPhiN[iOrder],weight);
//      printValue("Check","4a");
//      h_sysSinPhiN[iOrder]->Fill(momentsSystem._sinPhiN[iOrder],weight);
//      printValue("Check","4b");
//      h_sysRnN[iOrder]    ->Fill(momentsSystem._rN[iOrder],weight);
//      printValue("Check","4c");
//      h_sysPsiN[iOrder]   ->Fill(momentsSystem._psiN[iOrder],weight);
//      printValue("Check","4d");
//      h_sysEccN[iOrder]   ->Fill(momentsSystem._eccN[iOrder],weight);
//      printValue("Check","4e");
//      h_sysCosPhiNVsImpact[iOrder]  ->Fill(impact,momentsSystem._cosPhiN[iOrder]);
//      printValue("Check","4f");
//      h_sysSinPhiNVsImpact[iOrder]  ->Fill(impact,momentsSystem._sinPhiN[iOrder]);
//      printValue("Check","4g");
//      h_sysRnNVsImpact[iOrder]      ->Fill(impact,momentsSystem._rN[iOrder]);
//      printValue("Check","4h");
//      h_sysPsiNVsImpact[iOrder]     ->Fill(impact,momentsSystem._psiN[iOrder]);
//      printValue("Check","4i");
//      h_sysEccNVsImpact[iOrder]     ->Fill(impact,momentsSystem._eccN[iOrder]);
//      }
    }
  if (_fillArea) calculateArea(event);
  if (_drawEvents && _drawEventsIndex<_drawEventsN)
    {
    printValue("_drawEventsIndex",_drawEventsIndex);
    String eventName = createName(_histogramBaseName,"Event_");
    eventName += _drawEventsIndex;
    drawEvent(event,eventName);
    _drawEventsIndex++;
    }
  }

  double GlauberHistos::calculateDensityAt(const GlauberNucleus & nucleus,
                                           double x __attribute__((unused)),
                                           double y __attribute__((unused)),
                                           double rMaxSq __attribute__((unused)))
  {
  double density = 0.0;
  for (auto & nucleon : nucleus.allNucleons())
    {
    density += 1;
    if (nucleon.isWounded() && nucleon.square2DDistanceTo(x,y)<=rMaxSq)
      density += 0.0; //dist.evaluate(sqrt(rSq)); fix me!!!
    }
  return density;
  }

  double GlauberHistos::calculateDensity(NuclearDistribution & dist __attribute__((unused)),
                                         double x __attribute__((unused)),
                                         double y __attribute__((unused)))
  {
//  GlauberEvent & ev = event();
//  GlauberNucleus & nucleusA = ev.nucleusA();
//  GlauberNucleus & nucleusB = ev.nucleusB();
//  GlauberNucleusType & typeA = nucleusA.type();
//  GlauberNucleusType & typeB = nucleusB.type();
//  double aMax = typeA.maxRadius();
//  double bMax = typeB.maxRadius();
//  double rMax = aMax+bMax;
//  double rMaxSq = rMax*rMax;
//  double rSq;
   double density = 0;
//  density += calculateDensityAt(nucleusA,x,y,rMaxSq);
//  density += calculateDensityAt(nucleusB,x,y,rMaxSq);
  return density;
  }

  void GlauberHistos::calculateAreaAt(const GlauberNucleus & nucleus, TH2* area, double mx, double my, double rMaxSq)
  {
 // process nucleus A
  for (auto & nucleon : nucleus.allNucleons())
    {
    if (!nucleon.isWounded()) continue;
    const VectorLorentz & position = nucleon.position();
    double x = position.x()-mx;
    double y = position.y()-my;
    TAxis * xAxis = area->GetXaxis();
    TAxis * yAxis = area->GetYaxis();
    for (int iX=1; iX<=_area_nBins; ++iX)
      {
      for (int iY=1; iY<=_area_nBins; ++iY)
        {
        double val=area->GetBinContent(iX,iY);
        if (val>0) continue;
        double dx = x - xAxis->GetBinCenter(iX);
        double dy = y - yAxis->GetBinCenter(iY);
        if (dx*dx+dy*dy<rMaxSq) area->SetBinContent(iX,iY,1.0);
        }
      }
    }
  }

  void GlauberHistos::calculateArea(GlauberEvent & event)
  {
  double mx, my;
  const GlauberNucleus & nucleusA = event.nucleusA();
  const GlauberNucleus & nucleusB = event.nucleusB();
//  const GlauberNucleusType & typeA = nucleusA.type();
//  const GlauberNucleusType & typeB = nucleusB.type();
  double impact = event.impactParameter();
  h_areaA->Reset();
  h_areaB->Reset();

  const double tempo = 2.0*_area_max/double(_area_nBins);
  const double dArea = tempo*tempo;
  const double d2 = (double) event.eventXSection()/(pi()*10); // in fm^2
  const double rMaxSq = d2/4.0;

  mx = event.momentsNucleusA()._meanX;
  my = event.momentsNucleusA()._meanY;
  calculateAreaAt(nucleusA, h_areaA, mx, my, rMaxSq);
  mx = event.momentsNucleusB()._meanX;
  my = event.momentsNucleusB()._meanY;
  calculateAreaAt(nucleusB, h_areaB, mx, my, rMaxSq);
  double overlapA=0;
  double overlapB=0;
  double overlapAnd=0;
  double overlapOr=0;
  for (int iX=1; iX<=_area_nBins; ++iX)
    {
    for (int iY=1; iY<=_area_nBins; ++iY)
      {
      double vA = h_areaA->GetBinContent(iX,iY);
      double vB = h_areaB->GetBinContent(iX,iY);
      if (vA>0) ++overlapA;
      if (vB>0) ++overlapB;
      if (vA>0 && vB>0) ++overlapAnd;
      if (vA>0 || vB>0) ++overlapOr;
      }
    }

  double areaA       = overlapA*dArea;
  double areaB       = overlapB*dArea;
  double areaOverlap = overlapAnd*dArea;
  double areaTotal   = overlapOr*dArea;
  h_areaOverlap->Fill(areaOverlap);
  h_areaTotal->Fill(areaTotal);
  h_areaAVsImpact->Fill(impact,areaA);
  h_areaBVsImpact->Fill(impact,areaB);
  h_areaOverlapVsImpact->Fill(impact,areaOverlap);
  h_areaTotalVsImpact->Fill(impact,areaTotal);

  event.setAreaOverlap(areaOverlap);
  event.setAreaAll(areaTotal);

  }

  double  GlauberHistos::calculateLength()
  {
  //  const double krhs = sqrt(_xSectionEvent/40./pi());
  //  const double ksg  = krhs/sqrt(5.0);
  //  const double kDL  = 0.1;
  //  TF1 radiusProf("rad","2.0*pi/[0]/[0]*TMath::Exp(-x*x/(2.*[0]*[0]))",0.0,5*ksg);
  //  radiusProf.SetParameter(0,ksg);
  //  const double minval = radiusProf.Eval(5.0*ksg);
  //  double phi0         = gRandom->Uniform(0,twoPi());
  //  double kcphi0  = cos(phi0);
  //  double ksphi0  = sin(phi0);
  //  double x       = fEv.X0;
  //  double y       = fEv.Y0;
  //  double i0a     = 0;
  //  double i1a     = 0;
  //  double l       = 0;
  //  double val     = calcDensity(radiusProf,x,y);
  //  while (val>minval)
  //    {
  //    x     += kDL * kcphi0;
  //    y     += kDL * ksphi0;
  //    i0a   += val;
  //    i1a   += l*val;
  //    l     +=kDL;
  //    val   = calcDensity(radiusProf,x,y);
  //    }
  //  length = 2*i1a/i0a;
  return 0; // length;
  }



} // namespace CAP
