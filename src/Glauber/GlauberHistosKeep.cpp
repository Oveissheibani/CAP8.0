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
#include "RootHelpers.hpp"
#include "PrintHelpers.hpp"

ClassImp(CAP::GlauberHistos);

namespace CAP
{

GlauberHistos::GlauberHistos()
:
HistogramGroup(),

{
  appendClassName("GlauberHistos");
  setInstanceName("Glauber");
}

GlauberHistos::GlauberHistos(const GlauberHistos & group)
:
HistogramGroup(group)
{
  cloneAll(group);
}

GlauberHistos & GlauberHistos::operator=(const GlauberHistos & group)
{
  if (this!=&group)
    {
    HistogramGroup::operator=(group);
    cloneAll(group);
    }
  return *this;
}

void GlauberHistos::cloneAll(const GlauberHistos & group)
{
  // ony clone those that exist...
//  if (group.h_n1)         h_n1         = (TH1*) group.h_n1->Clone();
}

// for now use the same boundaries for eta and y histogram
void GlauberHistos::createHistograms()
{
  if (reportStart(__FUNCTION__)) { /* noops*/ };
  const String & bn  = name();
  const String & ptn = parentName();
  const String & ppn = parentPathName();
//  printString("GlauberHistos::createHistograms() -- 1 -- ");
//  printValue("bn",bn);
//  printValue("ptn",ptn);
//  printValue("ppn",ppn);
//  configuration.print();

//  nBins_n1 = _configuration.valueInt(ptn,"nBins_n1");
//  min_n1   = _configuration.valueDouble(ptn,"Min_n1");
//  max_n1   = _configuration.valueDouble(ptn,"Max_n1");

  fillBasics         = _configuration.valueBool(ptn,"FillBasics");
  fillMoments        = _configuration.valueBool(ptn,"FillMoments");
  fillSmearedMoments = _configuration.valueBool(ptn,"FillSmearedMoments");

  int impactPar_nbins  = _configuration.valueInt(ptn,"impactPar_nbins");
  double impactPar_min = _configuration.valueDouble(ptn,"impactPar_min");
  double impactPar_max = _configuration.valueDouble(ptn,"impactPar_max");
  int nParts_nbins     = _configuration.valueInt(ptn,"nParts_nbins");
  double nParts_min    = _configuration.valueDouble(ptn,"nParts_min");
  double nParts_max    = _configuration.valueDouble(ptn,"nParts_max");
  int  nBinaries_nbins = _configuration.valueInt(ptn,"nBinaries_nbins");
  double nBinaries_min = _configuration.valueDouble(ptn,"nBinaries_min");
  double nBinaries_max = _configuration.valueDouble(ptn,"nBinaries_max");



  if (reportDebug(__FUNCTION__))
    {
    printCR();
    printLine();
    printValue("Glauber:Parent Task Name",  ptn);
    printValue("Glauber:Parent Path Name",  ppn);
    printValue("Glauber:Histo Base Name",   bn);
    printValue("Glauber:FillBasics",        fillBasics);
    printValue("Glauber:FillMoments",       fillMoments  );
    printValue("Glauber:FillSmearedMoments",fillSmearedMoments );

    printValue("impactPar_nbins",impactPar_nbins);
    printValue("impactPar_min",impactPar_min);
    printValue("impactPar_max",impactPar_max);
    printValue("nParts_nbins",nParts_nbins);
    printValue("nParts_min",nParts_min);
    printValue("nParts_max",nParts_max);
    printValue("nBinaries_nbins",nBinaries_nbins);
    printValue("nBinaries_min",nBinaries_min);
    printValue("nBinaries_max",nBinaries_max);
    printLine();
    printCR();
    }

  h_impactPar   = createHistogram(createName(_histogramBaseName,"impactPar"),     impactPar_nbins,  impactPar_min,  impactPar_max,  "b (fm)","N",2);

  h_nParts         = createHistogram(createName(_histogramBaseName,"nParts"),        nParts_nbins,     nParts_min,     nParts_max,     "N_{parts}","N",2);
  h_nPartsA        = createHistogram(createName(_histogramBaseName,"nPartsA"),       nParts_nbins,     nParts_min,     nParts_max,     "N_{parts,A}","N",2);
  h_nPartsB        = createHistogram(createName(_histogramBaseName,"nPartsB"),       nParts_nbins,     nParts_min,     nParts_max,     "N_{parts,B}","N",2);
  h_nPartsProton   = createHistogram(createName(_histogramBaseName,"nPartsProton"),  nParts_nbins,     nParts_min,     nParts_max,     "N_{parts}^{p}","N",2);
  h_nPartsProtonA  = createHistogram(createName(_histogramBaseName,"nPartsProtonA"), nParts_nbins,     nParts_min,     nParts_max,     "N_{parts,A}^{p}","N",2);
  h_nPartsProtonB  = createHistogram(createName(_histogramBaseName,"nPartsProtonB"), nParts_nbins,     nParts_min,     nParts_max,     "N_{parts,B}^{p}","N",2);
  h_nPartsNeutron  = createHistogram(createName(_histogramBaseName,"nPartsNeutron"), nParts_nbins,     nParts_min,     nParts_max,     "N_{parts}^{n}","N",2);
  h_nPartsNeutronA = createHistogram(createName(_histogramBaseName,"nPartsNeutronA"),nParts_nbins,     nParts_min,     nParts_max,     "N_{parts,A}^{n}","N",2);
  h_nPartsNeutronB = createHistogram(createName(_histogramBaseName,"nPartsNeutronB"),nParts_nbins,     nParts_min,     nParts_max,     "N_{parts,B}^{n}","N",2);

  h_nBinaries         = createHistogram(createName(_histogramBaseName,"nBinaries"),         nBinaries_nbins,  nBinaries_min,  nBinaries_max,  "N_{bins}","N",2);
  h_nBinariesA        = createHistogram(createName(_histogramBaseName,"nBinariesA"),        nBinaries_nbins,  nBinaries_min,  nBinaries_max,  "N_{bins,A}","N",2);
  h_nBinariesB        = createHistogram(createName(_histogramBaseName,"nBinariesB"),        nBinaries_nbins,  nBinaries_min,  nBinaries_max,  "N_{bins,B}","N",2);
  h_nBinariesProton   = createHistogram(createName(_histogramBaseName,"nBinariesProton"),   nBinaries_nbins,  nBinaries_min,  nBinaries_max,  "N_{bins}^{p}","N",2);
  h_nBinariesProtonA  = createHistogram(createName(_histogramBaseName,"nBinariesProtonA"),  nBinaries_nbins,  nBinaries_min,  nBinaries_max,  "N_{bins,A}^{p}","N",2);
  h_nBinariesProtonB  = createHistogram(createName(_histogramBaseName,"nBinariesProtonB"),  nBinaries_nbins,  nBinaries_min,  nBinaries_max,  "N_{bins,B}^{p}","N",2);
  h_nBinariesNeutron  = createHistogram(createName(_histogramBaseName,"nBinariesNeutron"),  nBinaries_nbins,  nBinaries_min,  nBinaries_max,  "N_{bins}^{n}","N",2);
  h_nBinariesNeutronA = createHistogram(createName(_histogramBaseName,"nBinariesNeutronA"), nBinaries_nbins,  nBinaries_min,  nBinaries_max,  "N_{bins,A}^{n}","N",2);
  h_nBinariesNeutronB = createHistogram(createName(_histogramBaseName,"nBinariesNeutronB"), nBinaries_nbins,  nBinaries_min,  nBinaries_max,  "N_{bins,B}^{n}","N",2);

  h_nPartsVsImpact     = createHistogram(createName(_histogramBaseName,"nPartsVsImpact"),        impactPar_nbins,  impactPar_min,  impactPar_max, nParts_nbins,    nParts_min,   nParts_max,   "b (fm)","N_{parts}","N",2);
  h_nBinariesVsImpact  = createHistogram(createName(_histogramBaseName,"nBinariesVsImpact"),     impactPar_nbins,  impactPar_min,  impactPar_max, nBinaries_nbins, nBinaries_min,nBinaries_max,"b (fm)","N_{bins}","N",2);

  h_nPartsAvgVsImpact     = createProfile(createName(_histogramBaseName,"nPartsAvgVsImpact"),    impactPar_nbins,  impactPar_min,  impactPar_max, "b (fm)", "<N_{parts}>",2);
  h_nPartsAAvgVsImpact    = createProfile(createName(_histogramBaseName,"nPartsAAvgVsImpact"),   impactPar_nbins,  impactPar_min,  impactPar_max, "b (fm)", "<N_{parts,A}>",2);
  h_nPartsBAvgVsImpact    = createProfile(createName(_histogramBaseName,"nPartsBAvgVsImpact"),   impactPar_nbins,  impactPar_min,  impactPar_max, "b (fm)", "<N_{parts,B}>",2);
  h_nPartsProtonAvgVsImpact   = createProfile(createName(_histogramBaseName,"nPartsProtonAvgVsImpact"),    impactPar_nbins,  impactPar_min,  impactPar_max, "b (fm)", "<N_{parts}^{p}>",2);
  h_nPartsProtonAAvgVsImpact  = createProfile(createName(_histogramBaseName,"nPartsProtonAAvgVsImpact"),   impactPar_nbins,  impactPar_min,  impactPar_max, "b (fm)", "<N_{parts,A}^{p}>",2);
  h_nPartsProtonBAvgVsImpact  = createProfile(createName(_histogramBaseName,"nPartsProtonBAvgVsImpact"),   impactPar_nbins,  impactPar_min,  impactPar_max, "b (fm)", "<N_{parts,B}^{p}>",2);
  h_nPartsNeutronAvgVsImpact  = createProfile(createName(_histogramBaseName,"nPartsNeutronAvgVsImpact"),    impactPar_nbins,  impactPar_min,  impactPar_max, "b (fm)", "<N_{parts}>^{n}",2);
  h_nPartsNeutronAAvgVsImpact = createProfile(createName(_histogramBaseName,"nPartsNeutronAAvgVsImpact"),   impactPar_nbins,  impactPar_min,  impactPar_max, "b (fm)", "<N_{parts,A}>^{n}",2);
  h_nPartsNeutronBAvgVsImpact = createProfile(createName(_histogramBaseName,"nPartsNeutronBAvgVsImpact"),   impactPar_nbins,  impactPar_min,  impactPar_max, "b (fm)", "<N_{parts,B}^{n}>",2);

  h_nBinariesAvgVsImpact  = createProfile(createName(_histogramBaseName,"nBinariesAvgVsImpact"), impactPar_nbins,  impactPar_min,  impactPar_max, "b (fm)", "<N_{bins}>",2);
  h_nBinariesAAvgVsImpact    = createProfile(createName(_histogramBaseName,"nBinariesAAvgVsImpact"),   impactPar_nbins,  impactPar_min,  impactPar_max, "b (fm)", "<N_{bins,A}>",2);
  h_nBinariesBAvgVsImpact    = createProfile(createName(_histogramBaseName,"nBinariesBAvgVsImpact"),   impactPar_nbins,  impactPar_min,  impactPar_max, "b (fm)", "<N_{bins,B}>",2);
  h_nBinariesProtonAvgVsImpact   = createProfile(createName(_histogramBaseName,"nBinariesProtonAvgVsImpact"),    impactPar_nbins,  impactPar_min,  impactPar_max, "b (fm)", "<N_{bins}^{p}>",2);
  h_nBinariesProtonAAvgVsImpact  = createProfile(createName(_histogramBaseName,"nBinariesProtonAAvgVsImpact"),   impactPar_nbins,  impactPar_min,  impactPar_max, "b (fm)", "<N_{bins,A}^{p}>",2);
  h_nBinariesProtonBAvgVsImpact  = createProfile(createName(_histogramBaseName,"nBinariesProtonBAvgVsImpact"),   impactPar_nbins,  impactPar_min,  impactPar_max, "b (fm)", "<N_{bins,B}^{p}>",2);
  h_nBinariesNeutronAvgVsImpact  = createProfile(createName(_histogramBaseName,"nBinariesNeutronAvgVsImpact"),    impactPar_nbins,  impactPar_min,  impactPar_max, "b (fm)", "<N_{bins}>^{n}",2);
  h_nBinariesNeutronAAvgVsImpact = createProfile(createName(_histogramBaseName,"nBinariesNeutronAAvgVsImpact"),   impactPar_nbins,  impactPar_min,  impactPar_max, "b (fm)", "<N_{bins,A}>^{n}",2);
  h_nBinariesNeutronBAvgVsImpact = createProfile(createName(_histogramBaseName,"nBinariesNeutronBAvgVsImpact"),   impactPar_nbins,  impactPar_min,  impactPar_max, "b (fm)", "<N_{bins,B}^{n}>",2);

  h_nSpecAvgVsImpact  = createProfile(createName(_histogramBaseName,"nSpecAvgVsImpact"), impactPar_nbins,  impactPar_min,  impactPar_max, "b (fm)", "<N_{spect}>",2);
  h_nSpecAAvgVsImpact    = createProfile(createName(_histogramBaseName,"nSpecAAvgVsImpact"),   impactPar_nbins,  impactPar_min,  impactPar_max, "b (fm)", "<N_{spect,A}>",2);
  h_nSpecBAvgVsImpact    = createProfile(createName(_histogramBaseName,"nSpecBAvgVsImpact"),   impactPar_nbins,  impactPar_min,  impactPar_max, "b (fm)", "<N_{spect,B}>",2);
  h_nSpecProtonAvgVsImpact   = createProfile(createName(_histogramBaseName,"nSpecProtonAvgVsImpact"),    impactPar_nbins,  impactPar_min,  impactPar_max, "b (fm)", "<N_{spect}^{p}>",2);
  h_nSpecProtonAAvgVsImpact  = createProfile(createName(_histogramBaseName,"nSpecProtonAAvgVsImpact"),   impactPar_nbins,  impactPar_min,  impactPar_max, "b (fm)", "<N_{spect,A}^{p}>",2);
  h_nSpecProtonBAvgVsImpact  = createProfile(createName(_histogramBaseName,"nSpecProtonBAvgVsImpact"),   impactPar_nbins,  impactPar_min,  impactPar_max, "b (fm)", "<N_{spect,B}^{p}>",2);
  h_nSpecNeutronAvgVsImpact  = createProfile(createName(_histogramBaseName,"nSpecNeutronAvgVsImpact"),    impactPar_nbins,  impactPar_min,  impactPar_max, "b (fm)", "<N_spect}>^{n}",2);
  h_nSpecNeutronAAvgVsImpact = createProfile(createName(_histogramBaseName,"nSpecNeutronAAvgVsImpact"),   impactPar_nbins,  impactPar_min,  impactPar_max, "b (fm)", "<N_spect,A}>^{n}",2);
  h_nSpecNeutronBAvgVsImpact = createProfile(createName(_histogramBaseName,"nSpecNeutronBAvgVsImpact"),   impactPar_nbins,  impactPar_min,  impactPar_max, "b (fm)", "<N_{bins,B}^{n}>",2);


}

void GlauberHistos::importHistograms(TFile & inputFile)
{
  if (reportStart(__FUNCTION__)) { /* noops*/ };
  const String & bn  = name();

  printValue("GlauberHistos::importHistograms() - bn",bn);

  const String & ptn = parentName();
  const String & ppn = parentPathName();
  setSeverityLevel(Object::Debug);
  if (reportDebug(__FUNCTION__))
    {
    cout << endl;
    printValue("Glauber:Parent Task Name",   ptn);
    printValue("Glauber:Parent Path Name",   ppn);
    printValue("Glauber:Histo Base Name.",   bn);
    printValue("Glauber:FillEta",            fillEta);
    printValue("Glauber:FillY",              fillY  );
    printValue("Glauber:FillP2",             fillP2 );
    printValue("Glauber:FillPid",            fillPid );
    }

  fillBasics         = _configuration.valueBool(ptn,"FillBasics");
  fillMoments        = _configuration.valueBool(ptn,"FillMoments");
  fillSmearedMoments = _configuration.valueBool(ptn,"FillSmearedMoments");

  h_impactPar   = importH1(inputFile, createName(_histogramBaseName,"impactPar"));
  h_nParts      = importH1(inputFile, createName(_histogramBaseName,"nParts"));
  h_nBinaries   = importH1(inputFile, createName(_histogramBaseName,"nBinaries"));
  h_nPartsVsImpact     = importH2(inputFile, createName(_histogramBaseName,"nPartsVsImpact"));
  h_nBinariesVsImpact  = importH2(inputFile, createName(_histogramBaseName,"nBinariesVsImpact"));
  h_nPartsAvgVsImpact     = importProfile(inputFile, createName(_histogramBaseName,"nPartsAvgVsImpact"));
  h_nBinariesAvgVsImpact  = importProfile(inputFile, createName(_histogramBaseName,"nBinariesAvgVsImpact"));

}

//!
//! Fiil  single particle histograms of this class with the particles contained in the given list.
//!
void GlauberHistos::fill(const GlauberEvent & event, double weight)
{

  double impactPar         =  event.impactParameter();
  double nBinaries         =  event.nCollisions();
  double nBinariesProton   =  event.nProtonCollisions();
  double nBinariesNeutron  =  event.nNeutronCollisions();
  double nBinariesA        =  event.nCollisionsA();
  double nBinariesProtonA  =  event.nProtonCollisionsA();
  double nBinariesNeutronA =  event.nNeutronCollisionsA();
  double nBinariesB        =  event.nCollisionsB();
  double nBinariesProtonB  =  event.nProtonCollisionsB();
  double nBinariesNeutronB =  event.nNeutronCollisionsB();

  double nParts         =  event.nParticipants();
  double nPartsProton   =  event.nProtonParticipants();
  double nPartsNeutron  =  event.nNeutronParticipants();
  double nPartsA        =  event.nParticipantsA();
  double nPartsProtonA  =  event.nProtonParticipantsA();
  double nPartsNeutronA =  event.nNeutronParticipantsA();
  double nPartsB        =  event.nParticipantsB();
  double nPartsProtonB  =  event.nProtonParticipantsB();
  double nPartsNeutronB =  event.nNeutronParticipantsB();

  double nNSpect   =  event.nSpectators();
  double nNSpectA  =  event.nSpectatorsA();
  double nNSpectB  =  event.nSpectatorsB();
  double nNSpectNeutron   =  event.nNeutronSpectators();
  double nNSpectNeutronA  =  event.nNeutronSpectatorsA();
  double nNSpectNeutronB  =  event.nNeutronSpectatorsB();
  double nNSpectProton    =  event.nProtonSpectators();
  double nNSpectProtonA   =  event.nProtonSpectatorsA();
  double nNSpectProtonB   =  event.nProtonSpectatorsB();

  double nPP   =  event.nPPCollisions();
  double nPN   =  event.nPNCollisions();
  double nNP   =  event.nNPCollisions();
  double nNN   =  event.nNNCollisions();
  double nHard =  event.nHardCollisions();



  h_impactPar->Fill(impactPar,weight);

  h_nParts->Fill(nParts,weight);
  h_nPartsA->Fill(nPartsA,weight);
  h_nPartsB->Fill(nPartsB,weight);
  h_nPartsProton->Fill(nPartsProton,weight);
  h_nPartsProtonA->Fill(nPartsProtonA,weight);
  h_nPartsProtonB->Fill(nPartsProtonB,weight);
  h_nPartsNeutron->Fill(nPartsNeutron,weight);
  h_nPartsNeutronA->Fill(nPartsNeutronA,weight);
  h_nPartsNeutronB->Fill(nPartsNeutronB,weight);

  h_nBinaries->Fill(nBinaries,weight);
  h_nBinariesA->Fill(nBinariesA,weight);
  h_nBinariesB->Fill(nBinariesB,weight);
  h_nBinariesProton->Fill(nBinaries,weight);
  h_nBinariesProtonA->Fill(nBinariesProtonA,weight);
  h_nBinariesProtonB->Fill(nBinariesProtonB,weight);
  h_nBinariesNeutron->Fill(nBinariesNeutron,weight);
  h_nBinariesNeutronA->Fill(nBinariesNeutronA,weight);
  h_nBinariesNeutronB->Fill(nBinariesNeutronB,weight);

  h_nSpect->Fill(nSpect,weight);
  h_nSpectA->Fill(nSpectA,weight);
  h_nSpectB->Fill(nSpectB,weight);
  h_nSpectProton->Fill(nSpectProton,weight);
  h_nSpectProtonA->Fill(nSpectProtonA,weight);
  h_nSpectProtonB->Fill(nSpectProtonB,weight);
  h_nSpectNeutron->Fill(nSpectNeutron,weight);
  h_nSpectNeutronA->Fill(nSpectNeutronA,weight);
  h_nSpectNeutronB->Fill(nSpectNeutronB,weight);

  h_nPP->Fill(nPP,weight);
  h_nPN->Fill(nPN,weight);
  h_nNP->Fill(nNP,weight);
  h_nNN->Fill(nNN,weight);
  h_nHard->Fill(nHard,weight);

  h_nPartsVsImpact->Fill(impactPar,nNParts,weight);
  h_nBinariesVsImpact->Fill(impactPar,nBinaries,weight);

  h_nPartsAvgVsImpact->Fill(impactPar,nParts,weight);
  h_nPartsAAvgVsImpact->Fill(impactPar,nPartsA,weight);
  h_nPartsBAvgVsImpact->Fill(impactPar,nPartsB,weight);
  h_nPartsProtonAvgVsImpact->Fill(impactPar,nPartsProton,weight);
  h_nPartsProtonAAvgVsImpact->Fill(impactPar,nPartsProtonA,weight);
  h_nPartsProtonBAvgVsImpact->Fill(impactPar,nPartsProtonB,weight);
  h_nPartsNeutronAvgVsImpact->Fill(impactPar,nPartsNeutron,weight);
  h_nPartsNeutronAAvgVsImpact->Fill(impactPar,nPartsNeutronA,weight);
  h_nPartsNeutronBAvgVsImpact->Fill(impactPar,nPartsNeutronB,weight);

  h_nBinariesAvgVsImpact->Fill(impactPar,nBinaries,weight);
  h_nBinariesAAvgVsImpact->Fill(impactPar,nBinariesA,weight);
  h_nBinariesBAvgVsImpact->Fill(impactPar,nBinariesB,weight);
  h_nBinariesProtonAvgVsImpact->Fill(impactPar,nBinariesProton,weight);
  h_nBinariesProtonAAvgVsImpact->Fill(impactPar,nBinariesProtonA,weight);
  h_nBinariesProtonBAvgVsImpact->Fill(impactPar,nBinariesProtonB,weight);
  h_nBinariesNeutronAvgVsImpact->Fill(impactPar,nBinariesNeutron,weight);
  h_nBinariesNeutronAAvgVsImpact->Fill(impactPar,nBinariesNeutronA,weight);
  h_nBinariesNeutronBAvgVsImpact->Fill(impactPar,nBinariesNeutronB,weight);

  h_nSpectAvgVsImpact->Fill(impactPar,nSpect,weight);
  h_nSpectAAvgVsImpact->Fill(impactPar,nSpectA,weight);
  h_nSpectBAvgVsImpact->Fill(impactPar,nSpectB,weight);
  h_nSpectProtonAvgVsImpact->Fill(impactPar,nSpectProton,weight);
  h_nSpectProtonAAvgVsImpact->Fill(impactPar,nSpectProtonA,weight);
  h_nSpectProtonBAvgVsImpact->Fill(impactPar,nSpectProtonB,weight);
  h_nSpectNeutronAvgVsImpact->Fill(impactPar,nSpectNeutron,weight);
  h_nSpectNeutronAAvgVsImpact->Fill(impactPar,nSpectNeutronA,weight);
  h_nSpectNeutronBAvgVsImpact->Fill(impactPar,nSpectNeutronB,weight);

  h_nPPAvgVsImpact->Fill(impactPar,nPP,weight);
  h_nPNAvgVsImpact->Fill(impactPar,nPN,weight);
  h_nNPAvgVsImpact->Fill(impactPar,nNP,weight);
  h_nNNAvgVsImpact->Fill(impactPar,nNN,weight);
  h_nHardAvgVsImpact->Fill(impactPar,nHard,weight);

}



} // namespace CAP
