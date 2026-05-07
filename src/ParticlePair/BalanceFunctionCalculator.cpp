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
#include "BalanceFunctionCalculator.hpp"
#include "HistogramGroup.hpp"
#include "MathConstants.hpp"
#include "PrintHelpers.hpp"
#include "NameHelpers.hpp"

ClassImp(CAP::BalanceFunctionCalculator)

namespace CAP
{

BalanceFunctionCalculator::BalanceFunctionCalculator()
:
Calculator()
sObsNames(),
pObsNames(),
allFilesToAnalyze(),
appendedString("BalFct"),
calculateCI(true),
calculateCD(true),
calculateBF(true),
calculateDiffs(false)
{
  appendClassName("BalanceFunctionCalculator");
  setName("BF");
  setMinimumReportLevel(Object::kInfo);
}

BalanceFunctionCalculator::BalanceFunctionCalculator(const BalanceFunctionCalculator & source)
:
Calculator(source),
sObsNames(rhs.sObsNames),
pObsNames(rhs.pObsNames),
allFilesToAnalyze(rhs.allFilesToAnalyze),
calculateCI(rhs.calculateCI),
calculateCD(rhs.calculateCD),
calculateBF(rhs.calculateBF),
calculateDiffs(rhs.calculateDiffs)
{   }

BalanceFunctionCalculator & BalanceFunctionCalculator::operator=(const BalanceFunctionCalculator & rhs)
{
  if (this!=&rhs)
    {
    Calculator::operator=(rhs);
    sObsNames = rhs.sObsNames;
    pObsNames = rhs.pObsNames;
    allFilesToAnalyze = rhs.allFilesToAnalyze;
    calculateCI = rhs.calculateCI;
    calculateCD = rhs.calculateCD;
    calculateBF = rhs.calculateBF;
    calculateDiffs = rhs.calculateDiffs;
    }
  return *this;
}


void BalanceFunctionCalculator::setDefaultConfiguration()
{
  Task::setDefaultConfiguration();
  String null("null");
  addProperty("load",true);
  addProperty("exportObject",true);
  addProperty("CreateHistograms",true);

  addProperty("HistogramsForceRewrite", true);
  addProperty("AppendedString",         String("BalFct"));
  addProperty("calculateCI",            true);
  addProperty("calculateCD",            true);
  addProperty("calculateBF",            true);
  addProperty("calculateDiffs",         false);
  addProperty("eta_fill",                true);
  addProperty("rapidity_fill",                  false);
  addProperty("p2_fill",                 false);
  addProperty("n2_nbins",     100);
  addProperty("n2_min",       0.0);
  addProperty("n2_max",       100.0);
  addProperty("etot_nbins",   100);
  addProperty("etot_min",     0.0);
  addProperty("etot_max",     100.0);
  addProperty("pt_nbins",     18);
  addProperty("pt_min",       0.20);
  addProperty("pt_max",       2.00);
  addProperty("phi_nbins",    72);
  addProperty("phi_min",      0.0);
  addProperty("phi_max",      CAP::MATH::twoPi());
  addProperty("eta_nbins",    20);
  addProperty("eta_min",      -1.0);
  addProperty("eta_max",      1.0);
  addProperty("rapidity_nbins",      20);
  addProperty("rapidity_min",        -1.0);
  addProperty("rapidity_max",        1.0);
  addProperty("phiEta_nbins",      720);
  addProperty("phiEtaPt_nbins",    7200);
  addProperty("phiY_nbins",        720);
  addProperty("phiYPt_nbins",      7200);
  addProperty("n2_nbins",          100);
  addProperty("n2_min",            0.0);
  addProperty("n2_max",            1000.0);
  addProperty("DeltaPl_nbinsong",  10);
  addProperty("DeltaPl_minong",    -1.0);
  addProperty("DeltaPl_maxong",    1.0);
  addProperty("DeltaPs_nbinside",  10);
  addProperty("DeltaPs_minide",    -1.0);
  addProperty("DeltaPs_maxide",    1.0);
  addProperty("range_DeltaPside",  2.0);
  addProperty("DeltaPo_nbinsut",   10);
  addProperty("DeltaPo_minut",     -1.0);
  addProperty("DeltaPo_maxut",     1.0);
  addProperty("range_DeltaPout",   2.0);
  addProperty("Dphi_nbins",        36);
  addProperty("Dphi_min",          0.0);
  addProperty("Dphi_max",          CAP::MATH::twoPi());
  addProperty("Dphi_width",        CAP::MATH::twoPi());
  addProperty("Dphi_nbins_shft",   36);
  addProperty("Dphi_min_shft",     0.0);
  addProperty("Dphi_max_shft",     0.0);
  addProperty("Deta_nbins",        39);
  addProperty("Deta_min",          -2.0);
  addProperty("Deta_max",          2.0);
  addProperty("Deta_width",        4.0/39.0);
  addProperty("Drapidity_nbins",          39);
  addProperty("Drapidity_min",            -2.0);
  addProperty("Drapidity_max",            2.0);
  addProperty("Drapidity_width",          4.0/39.0);
}


TH2* BalanceFunctionCalculator::calculate_CI(const String & histoBaseName,
                                             const String & en,
                                             const String & pn1,
                                             const String & pn2,
                                             const String & obsName,
                                             TH2* obs_1_2,
                                             TH2* obs_1Bar_2,
                                             TH2* obs_1_2Bar,
                                             TH2* obs_1Bar_2Bar,
                                             HistogramGroup * hGroup)
{
  String name = createName(name(),en,pn1,pn2,obsName, "CI" );
  TH2 * obs;
  TH1 * obs_x;
  TH1 * obs_y;
  obs = (TH2*) obs_1Bar_2->Clone();
  obs->SetName(name);
  obs->SetTitle(name);
  obs->Add(obs_1_2Bar);
  obs->Add(obs_1_2);
  obs->Add(obs_1Bar_2Bar);
  obs->Scale(0.25);
  hGroup->push_back(obs);

  name = createName(histoBaseName,en,pn1,pn2,obsName,"CI_x");
  obs_x = obs->ProjectionX();
  obs_x->SetName(name);
  obs_x->SetTitle(name);
  hGroup->push_back(obs_x);

  name = createName(histoBaseName,en,pn1,pn2,obsName,"CI_y");
  obs_y = obs->ProjectionY();
  obs_y->SetName(name);
  obs_y->SetTitle(name);
  hGroup->push_back(obs_y);

  return obs;
}

TH2* BalanceFunctionCalculator::calculate_CD(const String & histoBaseName,
                                             const String & en,
                                             const String & pn1,
                                             const String & pn2,
                                             const String & obsName,
                                             TH2* obs_1_2,
                                             TH2* obs_1Bar_2,
                                             TH2* obs_1_2Bar,
                                             TH2* obs_1Bar_2Bar,
                                             HistogramGroup * hGroup)
{
  String name = createName(histoBaseName,en,pn1,pn2,obsName, "CD" );
  TH2 * obs;
  TH1 * obs_x;
  TH1 * obs_y;
  obs = (TH2*) obs_1Bar_2->Clone();
  obs->SetName(name);
  obs->SetTitle(name);
  obs->Add(obs_1_2Bar);
  obs->Add(obs_1_2, -1.0);
  obs->Add(obs_1Bar_2Bar, -1.0);
  obs->Scale(0.5);
  hGroup->push_back(obs);

  name = createName(histoBaseName,en,pn1,pn2,obsName,"CD_x");
  obs_x = obs->ProjectionX();
  obs_x->SetName(name);
  obs_x->SetTitle(name);
  hGroup->push_back(obs_x);

  name = createName(histoBaseName,en,pn1,pn2,obsName,"CD_y");
  obs_y = obs->ProjectionY();
  obs_y->SetName(name);
  obs_y->SetTitle(name);
  hGroup->push_back(obs_y);

  return obs;
}

TH2* BalanceFunctionCalculator::calculate_BalFct(const String & histoBaseName,
                                                 const String & en,
                                                 const String & pn1,
                                                 const String & pn2,
                                                 const String & obsName,
                                                 const String & comboName,
                                                 TH1* rho1_2,
                                                 TH2* obs_US,
                                                 TH2* obs_LS,
                                                 HistogramGroup * hGroup)
{
  String name = createName(histoBaseName,en,pn1,pn2,obsName,comboName);
  TH2 * obs;
  TH1 * obs_x;
  TH1 * obs_y;
  obs = (TH2*) obs_US->Clone();
  obs->SetName(name);
  obs->SetTitle(name);
  obs->Add(obs_LS, -1.0);

  double wx = obs->GetXaxis()->GetBinWidth(1);
  //double wy = obs->GetYaxis()->GetBinWidth(1);
  obs->Scale(1.0/wx); // make this a function of delta y
  double rho1Integral = rho1_2->Integral();
  printValue("rho1Integral",rho1Integral);
  //obs->Scale(1.0/rho1Integral);
  hGroup->push_back(obs);

  name = createName(histoBaseName,en,pn1,pn2,obsName,comboName+"_x");
  obs_x = obs->ProjectionX();
  obs_x->SetName(name);
  obs_x->SetTitle(name);
  hGroup->push_back(obs_x);

  name = createName(histoBaseName,en,pn1,pn2,obsName,comboName+"_y");
  obs_y = obs->ProjectionY();
  obs_y->SetName(name);
  obs_y->SetTitle(name);
  hGroup->push_back(obs_y);

  return obs;
}

TH2* BalanceFunctionCalculator::calculate_BalFctSum(const String & histoBaseName,
                                                    const String & en,
                                                    const String & pn1,
                                                    const String & pn2,
                                                    const String & obsName,
                                                    const String & comboName,
                                                    TH2* obs_12Bar,
                                                    TH2* obs_1Bar2,
                                                    HistogramGroup * hGroup)
{
  String name = createName(histoBaseName,en,pn1,pn2,obsName,comboName);
  TH2 * obs;
  TH1 * obs_x;
  TH1 * obs_y;
  obs = (TH2*) obs_12Bar->Clone();
  obs->SetName(name);
  obs->SetTitle(name);
  obs->Add(obs_1Bar2, 1.0);

  //double rho1Integral = rho1_2->Integral();
  //obs->Scale(0.5/wx);
  obs->Scale(0.5);
  hGroup->push_back(obs);

  name = createName(histoBaseName,en,pn1,pn2,obsName,comboName+"_x");
  obs_x = obs->ProjectionX();
  obs_x->SetName(name);
  obs_x->SetTitle(name);
  hGroup->push_back(obs_x);

  name = createName(histoBaseName,en,pn1,pn2,obsName,comboName+"_y");
  obs_y = obs->ProjectionY();
  obs_y->SetName(name);
  obs_y->SetTitle(name);
  hGroup->push_back(obs_y);

  return obs;
}

//TH2* BalanceFunctionCalculator::calculate_BalFct2(const String & histoBaseName,
//                                                  const String & en,
//                                                  const String & pn1,
//                                                  const String & pn2,
//                                                  const String & obsName,
//                                                  const String & comboName,
//                                                  TH1* rho1_1,
//                                                  TH1* rho1_2,
//                                                  TH2* obs_US,
//                                                  TH2* obs_LS,
//                                                  HistogramGroup * hGroup)
//{
//  String name = createName(histoBaseName,en,pn1,pn2,obsName,comboName);
//  TH2 * obs;
//  TH1 * obs_x;
//  TH1 * obs_y;
//  double rho1_1_Integral = rho1_1->Integral();
//  //double rho1_2_Integral = rho1_2->Integral();
//
////  double low  = rho1_1->GetXaxis()->GetXmin();
////  double high = rho1_1->GetXaxis()->GetXmax();
////  double yieldA = yieldA/(high-low)/CAP::MATH::twoPi();
////  double yieldB = yieldB/(high-low)/CAP::MATH::twoPi();
//
//
//  obs = (TH2*) obs_US->Clone();
//  obs->SetName(name);
//  obs->SetTitle(name);
//  obs->Add(obs_LS, -1.0);
//  obs->Scale(rho1_1_Integral);
//  hGroup->push_back(obs);
//
//  name = createName(histoBaseName,en,pn1,pn2,obsName,comboName+"_x");
//  obs_x = obs->ProjectionX();
//  obs_x->SetName(name);
//  obs_x->SetTitle(name);
//  hGroup->push_back(obs_x);
//
//  name = createName(histoBaseName,en,pn1,pn2,obsName,comboName+"_y");
//  obs_y = obs->ProjectionY();
//  obs_y->SetName(name);
//  obs_y->SetTitle(name);
//  hGroup->push_back(obs_y);
//
//  return obs;
//}

//TH2* BalanceFunctionCalculator::calculate_BalFct3(const String & histoBaseName,
//                                                  const String & en,
//                                                  const String & pn1,
//                                                  const String & pn2,
//                                                  const String & obsName,
//                                                  const String & comboName,
//                                                  TH1* rho1_2 __attribute__((unused)),
//                                                  TH2* obs_US,
//                                                  TH2* obs_LS,
//                                                  HistogramGroup * hGroup)
//{
//  String name = createName(histoBaseName,en,pn1,pn2,obsName,comboName);
//  TH2 * obs;
//  TH1 * obs_x;
//  TH1 * obs_y;
//  obs = (TH2*) obs_US->Clone();
//  obs->SetName(name);
//  obs->SetTitle(name);
//  obs->Add(obs_LS, -1.0);
////  double rho1Integral = rho1_2->Integral("Width");
////  obs->Scale(rho1Integral);
//  hGroup->push_back(obs);
//
//  name = createName(histoBaseName,en,pn1,pn2,obsName,comboName+"_x");
//  obs_x = obs->ProjectionX();
//  obs_x->SetName(name);
//  obs_x->SetTitle(name);
//  hGroup->push_back(obs_x);
//
//  name = createName(histoBaseName,en,pn1,pn2,obsName,comboName+"_y");
//  obs_y = obs->ProjectionY();
//  obs_y->SetName(name);
//  obs_y->SetTitle(name);
//  hGroup->push_back(obs_y);
//
//  return obs;
//}



TH2* BalanceFunctionCalculator::calculate_Diff(const String & histoBaseName,
                                                 const String & en,
                                                 const String & pn1,
                                                 const String & pn2,
                                                 const String & obsName,
                                                 const String & comboName,
                                                 TH2* obs_first,
                                               TH2* obs_second,
                                               HistogramGroup * hGroup)
{
  String name = createName(histoBaseName,en,pn1,pn2,obsName,comboName);
  TH2 * obs;
  TH1 * obs_x;
  TH1 * obs_y;
  obs = (TH2*) obs_first->Clone();
  obs->SetName(name);
  obs->SetTitle(name);
  obs->Add(obs_second, -1.0);
  hGroup->push_back(obs);

  name = createName(histoBaseName,en,pn1,pn2,obsName,comboName+"_x");
  obs_x = obs->ProjectionX();
  obs_x->SetName(name);
  obs_x->SetTitle(name);
  hGroup->push_back(obs_x);

  name = createName(histoBaseName,en,pn1,pn2,obsName,comboName+"_y");
  obs_y = obs->ProjectionY();
  obs_y->SetName(name);
  obs_y->SetTitle(name);
  hGroup->push_back(obs_y);

  return obs;
}

void BalanceFunctionCalculator::execute()
{
//  if (reportStart(__FUNCTION__)) {/* no ops */}
//  String histogramsImportPath    = getHistoImportPath();
//  String histogramsImportFile    = getHistoImportFile();
//  String histogramsExportPath    = getHistoExportPath();
//  String histogramsExportFile    = getHistoExportFile();
//  bool   forceRewrite  = histogramsForceRewrite();
//  appendedString      = valueString("AppendedString");
//  calculateCI         = valueBool("calculateCI" );
//  calculateCD         = valueBool("calculateCD" );
//  calculateBF         = valueBool("calculateBF" );
//  calculateDiffs      = valueBool("calculateDiffs" );
//
//
//  if (histogramsImportFile.Contains("DEFAULT") ||
//      histogramsImportFile.Contains("none") ||
//      histogramsImportFile.Contains("null") ||
//      histogramsImportFile.Contains("nil") ||
//      histogramsImportFile.IsNull() )
//    {
//    if (reportDebug(__FUNCTION__))
//      {
//      printCR();
//      printString("Generating a list of files!");
//      }
//    vector<String> includePatterns = selectedValues("IncludedPattern", "none");
//    vector<String> excludePatterns = selectedValues("ExcludedPattern", "none");
//    for (unsigned int k=0;k<includePatterns.size();k++) cout << " k:" << k << "  Include: " << includePatterns[k] << endl;
//    for (unsigned int k=0;k<excludePatterns.size();k++) cout << " k:" << k << "  Exclude: " << excludePatterns[k] << endl;
//    printLine();
//    bool prependPath = true;
//    bool verbose     = true;
//    int  maximumDepth = 2;
//    allFilesToAnalyze = listFilesInDir(histogramsImportPath,
//                                       includePatterns,excludePatterns,
//                                       prependPath, verbose, maximumDepth,0);
//    }
//  else
//    {
//    if (reportDebug(__FUNCTION__))
//      {
//      printLine();
//      printString("Using a fixed file!");
//      printValue("HistogramsImportPath",histogramsImportPath);
//      printValue("HistogramsImportFile",histogramsImportFile);
//      }
//    allFilesToAnalyze.push_back(histogramsImportPath+histogramsImportFile);
//    }
//  int nFilesToAnalyze = allFilesToAnalyze.size();
//  if (reportDebug(__FUNCTION__))
//    {
//    printCR();
//    printValue("AppendedString",        appendedString);
//    printValue("calculateCI",           calculateCI);
//    printValue("calculateCD",           calculateCD);
//    printValue("calculateBF",           calculateBF);
//    printValue("calculateDiffs",        calculateDiffs);
//    printValue("nFilesToAnalyze",       nFilesToAnalyze);
//    printCR();
//    }
//  if (nFilesToAnalyze<1) throw FileException(histogramImportPath,"*","No files found",__FUNCTION__);
//  std::vector<EventFilter*> & eventFilters = ManagedList<EventFilter>::getObjects();
//  std::vector<ParticleFilter*> & particleFilters = ManagedList<ParticleFilter>::getObjects();
//
//  for (auto & histogramImportFile : allFilesToAnalyze)
//    {
//    histogramExportFile  = removeRootExtension(histogramImportFile);
//    histogramExportFile.ReplaceAll(String("Derived"),appendedString);
//    TFile & inputFile = *openRootFile("",histogramImportFile,"OLD");
//    String option = "NEW";
//    if (histogramForceRewrite) option = "RECREATE";
//    TFile & outputFile = *openRootFile("",histogramExportFile,option);
//    if (reportDebug(__FUNCTION__))
//      {
//      printCR();
//      printLine();
//      printValue("From",histogramImportFile);
//      printValue("Saved to",histogramExportFile);
//      }
//    // Use hGroup  as helper to load and calculate histograms, etc.
//    HistogramGroup * hGroup  = new HistogramGroup();
//    //hGroup ->setOwnership(false);
//    hGroup ->setName(name());
//    hGroup ->setParentTask(this);
//    hGroup ->setConfiguration(configuration);
//    importNEexecutedTask(inputFile);
//    EventAccountant::importEventsAccepted(inputFile);
//
//    unsigned int nSpecies = particleFilters.size()/2;
//    vector<String>  sObsNames;
//    vector<String>  pObsNames;
//    int observableSelection = 5;
//    switch (observableSelection)
//      {
//        default:
//        case 0: // eta based observables, full complement
//        sObsNames.push_back("n1_eta");
//        sObsNames.push_back("n1_phi");
//        pObsNames.push_back("R2_ptpt");
//        pObsNames.push_back("R2_phiphi");
//        pObsNames.push_back("R2_etaeta");
//        pObsNames.push_back("R2_DetaDphi_shft");
//        break;
//
//        case 1: // eta based observables, only DeltaEta vs DeltaPhi
//        sObsNames.push_back("n1_eta");
//        sObsNames.push_back("n1_phi");
//        pObsNames.push_back("rho2_DetaDphi_shft");
//        break;
//
//        case 2: // y based observables, full complement
//        sObsNames.push_back("n1_y");
//        sObsNames.push_back("n1_phi");
//        pObsNames.push_back("R2_ptpt");
//        pObsNames.push_back("R2_phiphi");
//        pObsNames.push_back("R2_yY");
//        pObsNames.push_back("R2_DyDphi_shft");
//        break;
//
//        case 3: // y based observables, only DeltaY vs DeltaPhi
//        sObsNames.push_back("n1_y");
//        sObsNames.push_back("n1_phi");
//        pObsNames.push_back("R2_DyDphi_shft");
//        break;
//
//        case 4: // eta based observables, only DeltaEta vs DeltaPhi
//        sObsNames.push_back("n1_eta");
//        sObsNames.push_back("n1_phi");
//        pObsNames.push_back("rho2_DetaDphi_shft");
//        pObsNames.push_back("R2_DetaDphi_shft");
//        //pObsNames.push_back("B2AB_DetaDphi_shft");
//        //pObsNames.push_back("B2BA_DetaDphi_shft");
//        //        pObsNames.push_back("n2_phiphi");
//        break;
//
//        case 5: // y based observables
//        sObsNames.push_back("n1_y");
//        sObsNames.push_back("n1_phi");
//        pObsNames.push_back("A2_DyDphi_shft");
//        pObsNames.push_back("B2_DyDphi_shft");
//        pObsNames.push_back("C2_DyDphi_shft");
//        pObsNames.push_back("D2_DyDphi_shft");
//        pObsNames.push_back("R2_DyDphi_shft");
//        pObsNames.push_back("B2_yY");
//        //pObsNames.push_back("B2_phiphi");
//        break;
//      }
//    if (reportDebug(__FUNCTION__))
//      {
//      cout << endl;
//      printValue("nSpecies",nSpecies);
//      for (unsigned int iPart1=0; iPart1<nSpecies; iPart1++)
//        {
//        cout << "iPart1:" <<  iPart1 << "  named: "<< particleFilters[iPart1]->name() << endl;
//        }
//      printValue("sObsNames.size()",int(sObsNames.size()));
//      for (unsigned int k=0; k<sObsNames.size(); k++)
//        printValue("   ",sObsNames[k]);
//      printValue("pObsNames.size()",int(pObsNames.size()));
//      for (unsigned int k=0; k<pObsNames.size(); k++)
//        printValue("   ",pObsNames[k]);
//      }
//    for (auto & pObsName : pObsNames)
//      {
//      for (auto & particleFilter1 : particleFilters)
//        {
//        for (auto & particleFilter2 : particleFilters)
//          {
//          for (auto & eventFilter : eventFilters)
//            {
//            // load histogram and compute derived files.
//            String en     = eventFilter->name();
//            String pn1    = particleFilter1->name();
//            String pn1Bar = (particleFilter1+nSpecies)->name();
//            String pn2    = particleFilter2->name();
//            String pn2Bar = (particleFilter2+nSpecies)->name();
//            //TH1 * rho1_1         = hGroup ->importH1(inputFile,createName(name(),en,pn1,   sObsNames[0]));
//            //TH1 * rho1_1Bar      = hGroup ->importH1(inputFile,createName(name(),en,pn1Bar,sObsNames[0]));
//            TH1 * rho1_2         = hGroup ->importH1(inputFile,createName(name(),en,pn2,   sObsNames[0]));
//            TH1 * rho1_2Bar      = hGroup ->importH1(inputFile,createName(name(),en,pn2Bar,sObsNames[0]));
//            TH2 * obs_1_2        = hGroup ->importH2(inputFile,createName(name(),en,pn1,    pn2,    pObsName));
//            TH2 * obs_1Bar_2     = hGroup ->importH2(inputFile,createName(name(),en,pn1Bar, pn2,    pObsName));
//            TH2 * obs_1_2Bar     = hGroup ->importH2(inputFile,createName(name(),en,pn1,    pn2Bar, pObsName));
//            TH2 * obs_1Bar_2Bar  = hGroup ->importH2(inputFile,createName(name(),en,pn1Bar, pn2Bar, pObsName));
//
//            if (calculateCI)
//              calculate_CI(name(),en,pn1,pn2, pObsName,obs_1_2,obs_1Bar_2,obs_1_2Bar,obs_1Bar_2Bar,hGroup );
//
//            if (calculateCD)
//              calculate_CD(name(),en,pn1,pn2, pObsName,obs_1_2,obs_1Bar_2,obs_1_2Bar,obs_1Bar_2Bar,hGroup );
//
//            if (calculateBF)
//              {
//              TH2* bfa = calculate_BalFct(name(),en,pn1,pn2, pObsName, "B2_1_2Bar",rho1_2Bar, obs_1_2Bar, obs_1Bar_2Bar,hGroup );
//              TH2* bfb = calculate_BalFct(name(),en,pn1,pn2, pObsName, "B2_1Bar_2",rho1_2,    obs_1Bar_2, obs_1_2,hGroup );
//              calculate_BalFctSum(name(),en,pn1,pn2, pObsName, "B2_12Sum",bfa,bfb,hGroup );
//              }
//            if (calculateDiffs)
//              {
//              calculate_Diff(name(),en,pn1,pn2, pObsName, "Diff_US",   obs_1Bar_2,    obs_1_2Bar,hGroup );
//              calculate_Diff(name(),en,pn1,pn2, pObsName, "Diff_LS",   obs_1Bar_2Bar, obs_1_2,hGroup );
//              }
//            }
//          }
//        }
//      }
//    outputFile.cd();
//    hGroup->exportObject(outputFile);
//    EventAccountant::exportEventsAccepted(outputFile);
//    TaskAccountant::exportNEexecutedTask(outputFile);
//    inputFile.Close();
//    outputFile.Close();
//    hGroup->clear();
//    delete hGroup ;
//    }
//  if (reportEnd(__FUNCTION__)) {/* no ops */}
}

}  // namespace CAP


