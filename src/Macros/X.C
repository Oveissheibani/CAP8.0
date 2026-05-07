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

TCanvas * createCanvas(const TString & canvasName,int inc)
{
  TCanvas * canvas = new TCanvas(canvasName,canvasName,30,30,600,600);
  canvas->SetLogx(        0 );
  canvas->SetLogy(        1 );
//  canvas->SetLogz(        1 );
  canvas->SetRightMargin( 0.1 );
  canvas->SetLeftMargin(  0.2 );
  canvas->SetBottomMargin(0.15);
  canvas->SetTopMargin(   0.05 );
  canvas->SetTicky(       1 );
  canvas->SetTickx(       1 );
  canvas->SetGridx(       0 );
  canvas->SetGridy(       0 );
//  canvas->SetFillColor(   configuration.getValueInt("windowFillColor") );
//  canvas->SetFillStyle(   configuration.getValueInt("windowFillStyle") );
//  canvas->SetBorderSize(  configuration.getValueInt("windowBorderSize") );
//  canvas->SetBorderMode(  configuration.getValueInt("windowBorderMode") );
  return canvas;
}

int X(const TString & path="./")
{
  TString includeBasePath = getenv("CAP_SRC");
  loadBase(includeBasePath);
//  loadPlotting(includeBasePath);

  ROOT::RBrowser * b = new ROOT::RBrowser();
  TF1 * xAt10GeV = new TF1("xAt10GeV","([0]/[1])*TMath::Exp(x)",-5.0,5.0);
  xAt10GeV->SetParameters(10.0,200.0);
  TF1 * xAt50GeV = new TF1("xAt50GeV","([0]/[1])*TMath::Exp(x)",-5.0,5.0);
  xAt50GeV->SetParameters(50.0,200.0);
  TF1 * xAt100GeV = new TF1("xAt100GeV","([0]/[1])*TMath::Exp(x)",-5.0,5.0);
  xAt100GeV->SetParameters(100.0,200.0);

  TCanvas * c = createCanvas("xVsY",20);
  xAt10GeV->SetMinimum(1E-5);
  xAt10GeV->SetMaximum(1.0);
  xAt10GeV->Draw();
  xAt50GeV->Draw("SAME");
  xAt100GeV->Draw("SAME");
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

