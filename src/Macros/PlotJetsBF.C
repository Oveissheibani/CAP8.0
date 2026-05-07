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
void loadBase(const TString & includeBasePath);
void loadPlotting(const TString & includeBasePath);

TH1 * cloneLoadH1(TFile & inputFile, TString histoName)
{
  TH1 * h = (TH1*) inputFile.Get(histoName);
  TH1 * clone = (TH1*) h->Clone();
  return clone;
}

TH1 * cloneLoadH2(TFile & inputFile, TString histoName)
{
  TH2 * h = (TH2*) inputFile.Get(histoName);
  TH2 * clone = (TH2*) h->Clone();
  return clone;
}

TH1 * cumulativeSum(TFile & inputFile, TString histoName, bool noWidth=true)
{
  TH1 * h = (TH1*) inputFile.Get(histoName);
  TH1 * clone = (TH1*) h->Clone();
  int n = clone->GetNbinsX();
  double v, ev, w, sum, eSum;
  v  = clone->GetBinContent(1);
  ev = clone->GetBinError(1);
  w  = clone->GetBinWidth(1);
  if (noWidth)
    {
    sum  = v;
    eSum = ev;
    }
  else
    {
    sum  = w*v;
    eSum = w*ev;
    }
  for (int k=2;k<=n; k++)
    {
    v  = clone->GetBinContent(k);
    ev = clone->GetBinError(k);
    w  = clone->GetBinWidth(k);
    if (noWidth)
      {
      sum  += v;;
      eSum += sqrt(eSum*eSum + ev*ev);
      }
    else
      {
      sum  = w*v;
      eSum = w*ev;
      }
    clone->SetBinContent(k,sum);
    clone->SetBinError(k,0.0);
    }
  return clone;
}

TH1 * computeHistoSum(const vector<TH1*> & histos, const TString & name)
{
  TH1 * histoSum = (TH1*) histos[0]->Clone();
  for (int k=1; k<histos.size(); k++)
    {
    histoSum->Add(histos[k]);
    }
  histoSum->SetName(name);
  return histoSum;
}

int PlotJetsBF(const TString & path="/Volumes/ClaudeDisc4/OutputFiles/pythiaTestJets/",
               const TString & fileName0="JetsGenR2.root",
               const TString & fileName1="JetsGenR4.root",
               const TString & fileName2="JetsGenR6.root",
               const TString & fileName3="JetsGenR8.root",
               const TString & fileName4="JetsGenR10.root")
{
  //gROOT->SetWebDisplay();

  TString includeBasePath = getenv("CAP_SRC");
  loadBase(includeBasePath);
  loadPlotting(includeBasePath);

  using CAP::Configuration;
  using CAP::CanvasConfiguration;
  using CAP::GraphConfiguration;
  using CAP::Plotter;

  Configuration taskConfig;
  Plotter * plotter = new Plotter();
  plotter->setDefaultOptions(1);

  //enum CanvasFormat   { PortraitTight, Portrait, PortraitWide, SquareTight, Square, SquareWide, LandscapeTight, Landscape, LandscapeWide, LandscapeXtop };
  //enum CanvasAxes     { Linear, LogX, LogY, LogZ, LogXY, LogXZ, LogYZ, LogXYZ  };
  plotter->addCanvasConfiguartion(CanvasConfiguration::LandscapeWide,CanvasConfiguration::Linear);
  plotter->addCanvasConfiguartion(CanvasConfiguration::LandscapeWide,CanvasConfiguration::LogY);
  plotter->addGraphConfiguration(10,1);
  int linear = 0;
  int logY   = 1;
  try
  {

  TString inputFileName0 = path+fileName0;
  TString inputFileName1 = path+fileName1;
  TString inputFileName2 = path+fileName2;
  TString inputFileName3 = path+fileName3;
  TString inputFileName4 = path+fileName4;
  TFile * inputFile0 =  new TFile(inputFileName0,"OLD");
  TFile * inputFile1 =  new TFile(inputFileName1,"OLD");
  TFile * inputFile2 =  new TFile(inputFileName2,"OLD");
  TFile * inputFile3 =  new TFile(inputFileName3,"OLD");
  TFile * inputFile4 =  new TFile(inputFileName4,"OLD");

  ROOT::RBrowser * b = new ROOT::RBrowser();
  vector<TH1*>    histos;
  double legendSize = 0.3;

  histos.push_back( cloneLoadH1(*inputFile1,"JetR4_ALL_E10_jet_n") );
  histos.push_back( cloneLoadH1(*inputFile1,"JetR4_ALL_E20_jet_n"));
  histos.push_back( cloneLoadH1(*inputFile1,"JetR4_ALL_E30_jet_n"));
  histos.push_back( cloneLoadH1(*inputFile1,"JetR4_ALL_E40_jet_n"));
  histos.push_back( cloneLoadH1(*inputFile1,"JetR4_ALL_E50_jet_n"));
  histos[0]->GetYaxis()->SetTitle("Counts");
  histos[0]->SetTitle("10<p_{Jet,T}<20 GeV/c");
  histos[1]->SetTitle("20<p_{Jet,T}<30 GeV/c");
  histos[2]->SetTitle("30<p_{Jet,T}<40 GeV/c");
  histos[3]->SetTitle("40<p_{Jet,T}<50 GeV/c");
  histos[4]->SetTitle("50<p_{Jet,T}<60 GeV/c");
  plotter->plot("Jet_ALL_E10_E50_n",histos,
                TString("n"), 0.0, 100.0,
                TString("Counts"),1.0, 3.00E4,
                true, 0.5, 0.7, 0.5, 0.7, 0.05);//,logY);
  histos.clear();

  histos.push_back( cloneLoadH1(*inputFile1,"JetR4_ALL_E10_jet_n1_pt") );
  histos.push_back( cloneLoadH1(*inputFile1,"JetR4_ALL_E20_jet_n1_pt"));
  histos.push_back( cloneLoadH1(*inputFile1,"JetR4_ALL_E30_jet_n1_pt"));
  histos.push_back( cloneLoadH1(*inputFile1,"JetR4_ALL_E40_jet_n1_pt"));
  histos.push_back( cloneLoadH1(*inputFile1,"JetR4_ALL_E50_jet_n1_pt"));
  histos[0]->GetYaxis()->SetTitle("Counts");
  histos[0]->SetTitle("10<p_{Jet,T}<20 GeV/c");
  histos[1]->SetTitle("20<p_{Jet,T}<30 GeV/c");
  histos[2]->SetTitle("30<p_{Jet,T}<40 GeV/c");
  histos[3]->SetTitle("40<p_{Jet,T}<50 GeV/c");
  histos[4]->SetTitle("50<p_{Jet,T}<60 GeV/c");
  plotter->plot("Jet_ALL_E10_E50_n1_pt",histos,
                TString("p_{T}"), 0.0, 100.0,
                TString("Counts"),1.0, 3.00E5,
                true, 0.5, 0.7, 0.5, 0.7, 0.05,logY);
  histos.clear();


  histos.push_back( cloneLoadH1(*inputFile1,"JetR4_ALL_E10_jet_netQ_p") );
  histos.push_back( cloneLoadH1(*inputFile1,"JetR4_ALL_E20_jet_netQ_p"));
  histos.push_back( cloneLoadH1(*inputFile1,"JetR4_ALL_E30_jet_netQ_p"));
  histos.push_back( cloneLoadH1(*inputFile1,"JetR4_ALL_E40_jet_netQ_p"));
  histos.push_back( cloneLoadH1(*inputFile1,"JetR4_ALL_E50_jet_netQ_p"));
  histos[0]->GetYaxis()->SetTitle("Counts");
  histos[0]->SetTitle("10<p_{Jet,T}<20 GeV/c");
  histos[1]->SetTitle("20<p_{Jet,T}<30 GeV/c");
  histos[2]->SetTitle("30<p_{Jet,T}<40 GeV/c");
  histos[3]->SetTitle("40<p_{Jet,T}<50 GeV/c");
  histos[4]->SetTitle("50<p_{Jet,T}<60 GeV/c");
  plotter->plot("Jet_ALL_E10_E50_netQ",histos,
                TString("p_{T}"), 0.0, 100.0,
                TString("<Q>"),-1.0, 3.0,
                true, 0.5, 0.7, 0.5, 0.7, 0.05);
  histos.clear();



  histos.push_back( cloneLoadH1(*inputFile0,"JetR2_ALL_E10_jet_n") );
  histos.push_back( cloneLoadH1(*inputFile1,"JetR4_ALL_E10_jet_n"));
  histos.push_back( cloneLoadH1(*inputFile2,"JetR6_ALL_E10_jet_n"));
  histos.push_back( cloneLoadH1(*inputFile3,"JetR8_ALL_E10_jet_n"));
  histos.push_back( cloneLoadH1(*inputFile4,"JetR10_ALL_E10_jet_n"));
  histos[0]->GetYaxis()->SetTitle("Counts");
  histos[0]->SetTitle("R = 2");
  histos[1]->SetTitle("R = 4");
  histos[2]->SetTitle("R = 6");
  histos[3]->SetTitle("R = 8");
  histos[4]->SetTitle("R = 1.0");
  plotter->plot("Jet_ALL_E10_n_vs_R",histos,
                TString("n"), 0.0, 100.0,
                TString("Counts"),1.0, 3.00E4,
                true, 0.5, 0.7, 0.5, 0.7, 0.05);//,logY);
  histos.clear();
//
//  histos.push_back( cumulativeSum(*inputFile0,"Pair3D_ALL_PiP_n1_pt"));
//  histos.push_back( cumulativeSum(*inputFile0,"Pair3D_ALL_PiM_n1_pt"));
//  histos[0]->GetYaxis()->SetTitle("#int #rho_{1}^{#pi^{+}}");
//  histos[0]->SetTitle("#rho_{1}^{#pi^{+}}");
//  histos[1]->SetTitle("#rho_{1}^{#pi^{-}}");
//  plotter->plot("Pair3D_ALL_Pi_int_rho1_pT",histos,
//                TString("p_{T}"), 0.0, 8.0,
//                TString("#rho_{1}(pT)"),-0.05, 29.99,
//                true, 0.5, 0.7, 0.2, 0.45, 0.05);
//  histos.clear();
//

//
//  histos.push_back( cloneLoadH1(*inputFile2,"Pair3D_ALL_PP_PP_B21_Qinv"));
//  histos.push_back( cloneLoadH1(*inputFile2,"Pair3D_ALL_PP_PP_B12_Qinv"));
//  histos.push_back( cloneLoadH1(*inputFile2,"Pair3D_ALL_PP_PP_Bs_Qinv"));
//  histos[0]->GetYaxis()->SetTitle("B(Q_{INV})");
//  histos[0]->SetTitle(" B^{21}(P,P)");
//  histos[1]->SetTitle(" B^{12}(P,P)");
//  histos[2]->SetTitle(" B^{s}(P,P)");
//  plotter->plot("Pair3D_ALL_PP_PP_B_Qinv",histos,TString("Q_{inv}"), 0.0, 5.0,
//                TString("B"),-0.002, 0.0199,
//                true, 0.5, 0.7, 0.4, 0.7, 0.5);
//  histos.clear();
//
//
//  histos.push_back( cloneLoadH1(*inputFile2,"Pair3D_ALL_KP_PiP_B21_Qinv"));
//  histos.push_back( cloneLoadH1(*inputFile2,"Pair3D_ALL_KP_PiP_B12_Qinv"));
//  histos.push_back( cloneLoadH1(*inputFile2,"Pair3D_ALL_KP_PiP_Bs_Qinv"));
//  histos[0]->GetYaxis()->SetTitle("B(Q_{INV})");
//  histos[0]->SetTitle(" B^{21}(K,#pi)");
//  histos[1]->SetTitle(" B^{12}(K,#pi)");
//  histos[2]->SetTitle(" B^{s}(K,#pi)");
//  plotter->plot("Pair3D_ALL_KP_PiP_B_Qinv",histos,TString("Q_{inv}"), 0.0, 5.0,
//                TString("B"),-0.002, 0.0899,
//                true, 0.5, 0.7, 0.4, 0.7, 0.5);
//  histos.clear();
//


  plotter->printAllCanvas(path);

  }
  catch (CAP::PropertyException & e)
  {
  e.print();
  }
  return 0;
}

void loadBase(const TString & includeBasePath)
{
  TString includePath = includeBasePath + "/Base/";
//  gSystem->Load(includePath+"Exceptions.hpp");
//  gSystem->Load(includePath+"Timer.hpp");
//  gSystem->Load(includePath+"MessageLogger.hpp");
//  gSystem->Load(includePath+"Task.hpp");
//  gSystem->Load(includePath+"Collection.hpp");
//  gSystem->Load(includePath+"HistogramCollection.hpp");
//  gSystem->Load(includePath+"HistogramGroup.hpp");
  gSystem->Load("libBase.dylib");
}

void loadPlotting(const TString & includeBasePath)
{
  TString includePath = includeBasePath + "/Plotting/";
//  gSystem->Load(includePath+"CanvasCollection.hpp");
//  gSystem->Load(includePath+"GraphConfiguration.hpp");
//  gSystem->Load(includePath+"CanvasConfiguration.hpp");
//  gSystem->Load(includePath+"Plotter.hpp");
//  gSystem->Load(includePath+"BalFctPlotter.hpp");
  gSystem->Load("libPlotting.dylib");
}

