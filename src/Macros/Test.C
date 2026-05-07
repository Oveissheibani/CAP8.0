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

TH1 * setupEffVsPt(int pt_nbins,
                   double pt_min,
                   double pt_max,
                   const vector<double> & thres_pt,
                   const vector<double> & value_pt)
{
  TH1 * h = new TH1D("test","test",pt_nbins,pt_min,pt_max);
  double width_pt = (pt_max-pt_min)/double(pt_nbins);
  int nSteps = thres_pt.size();
  int iStep  = 0;
  double lastEff = 0.0;
  double eff = 0.0;
  int ipt;
  double pt = pt_min+width_pt/2.0;
  while (pt<pt_max)
    {
    ipt = h->GetXaxis()->FindBin(pt);
    if (pt<thres_pt[0])
      {
      eff = 0.0;
      lastEff = eff;
      h->SetBinContent(ipt,eff);
      pt += width_pt;
      }
    else if (pt<=thres_pt[iStep])
      {
      double a = (value_pt[iStep] - value_pt[iStep-1])/(thres_pt[iStep] - thres_pt[iStep-1]);
      eff = value_pt[iStep-1] + a * (pt-thres_pt[iStep-1]);
      if (pt<1) cout << "iStep:" << iStep << "  pt:" << pt << "   a:" << a << "  eff:" << eff << "  last eff:" << lastEff << endl;
      if (eff>1)
        eff = 1.0;
      else if (eff<0)
        eff = 0.0;
      lastEff = eff;
      h->SetBinContent(ipt,eff);
      pt += width_pt;
      }
    else
      iStep++;
    }
  return h;
}

TH2 * setupEffVsEtaPt(int eta_nbins,
                      double eta_min,
                      double eta_max,
                      int pt_nbins,
                      double pt_min,
                      double pt_max,
                      const vector<double> & thres_eta,
                      const vector<double> & value_eta,
                      const vector<double> & thres_pt,
                      const vector<double> & value_pt)
{
  TH2 * h = new TH2D("test2","test2",eta_nbins,eta_min,eta_max,pt_nbins,pt_min,pt_max);
  double width_pt  = (pt_max-pt_min)/double(pt_nbins);
  double width_eta = (eta_max-eta_min)/double(eta_nbins);
  int nSteps_pt = thres_pt.size();
  int iStep_pt  = 0;
  int nSteps_eta = thres_eta.size();
  int iStep_eta  = 0;
  double lastEff_eta = 0.0;
  double eff_eta = 0.0;
  double lastEff_pt = 0.0;
  double eff_pt = 0.0;
  int ipt;
  int ieta;
  double pt  = pt_min+width_pt/2.0;
  double eta = eta_min+width_eta/2.0;
  double a_eta;
  double a_pt;
  while (eta<eta_max)
    {
    ieta = h->GetXaxis()->FindBin(eta);
    pt  = pt_min+width_pt/2.0;
    if (eta<thres_eta[0])
      {
      eff_eta = 0.0;
      lastEff_eta = eff_eta;
      for (ipt=0; ipt<pt_nbins; ipt++) h->SetBinContent(ieta,ipt,eff_eta);
      eta += width_eta;
      }
    else  if (eta<thres_eta[iStep_eta])
      {
      double a_eta = (value_eta[iStep_eta] - value_eta[iStep_eta-1])/(thres_eta[iStep_eta] - thres_eta[iStep_eta-1]);
      eff_eta = value_eta[iStep_eta-1] + a_eta * (eta-thres_eta[iStep_eta-1]);
      if (eff_eta>1)
        eff_eta = 1.0;
      else if (eff_eta<0)
        eff_eta = 0.0;
      lastEff_eta = eff_eta;
      pt  = pt_min+width_pt/2.0;
      while (pt<pt_max)
      {
      ipt  = h->GetYaxis()->FindBin(pt);
      if (pt<thres_pt[0])
        {
        eff_pt = 0.0;
        lastEff_pt = eff_pt;
        h->SetBinContent(ieta,ipt,eff_pt);
        pt += width_pt;
        }
      else if (pt<=thres_pt[iStep_pt])
        {
        double a_pt = (value_pt[iStep_pt] - value_pt[iStep_pt-1])/(thres_pt[iStep_pt] - thres_pt[iStep_pt-1]);
        eff_pt = value_pt[iStep_pt-1] + a_pt * (pt-thres_pt[iStep_pt-1]);
        if (eff_pt>1)
          eff_pt = 1.0;
        else if (eff_pt<0)
          eff_pt = 0.0;
        lastEff_pt = eff_pt;
        h->SetBinContent(ieta,ipt,eff_eta*eff_pt);
        pt += width_pt;
        }
      else
        iStep_pt++;
      }
      eta += width_eta;
      }
    else
      iStep_eta++;
    }
  return h;
}

int Test(const TString & path="/Volumes/ClaudeDisc4/OutputFiles/pythiaTest3D/",
           const TString & fileName0="Test.root")
{
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
  plotter->addCanvasConfiguartion(CanvasConfiguration::LandscapeWide,CanvasConfiguration::Linear);
  plotter->addCanvasConfiguartion(CanvasConfiguration::LandscapeWide,CanvasConfiguration::LogY);
  plotter->addGraphConfiguration(10,1);
  int linear = 0;
  int logY   = 1;
  try
  {

  TString inputFileName0 = path+fileName0;
  vector<TFile*> inputFiles;
  inputFiles.push_back( new TFile(inputFileName0,"OLD") );
  ROOT::RBrowser * b = new ROOT::RBrowser();
  vector<TH1*>    histos;
  vector<TH2*>    histos2;
  double legendSize = 0.3;

  int pt_nbins = 500;
  double pt_min = 0.0;
  double pt_max = 5.0;
  vector<double> thres_pt;
  vector<double> value_pt;
  thres_pt.push_back(0.2); value_pt.push_back(0.01);
  thres_pt.push_back(0.3); value_pt.push_back(0.05);
  thres_pt.push_back(0.4); value_pt.push_back(0.20);
  thres_pt.push_back(0.5); value_pt.push_back(0.50);
  thres_pt.push_back(0.6); value_pt.push_back(0.80);
  thres_pt.push_back(0.7); value_pt.push_back(0.87);
  thres_pt.push_back(0.8); value_pt.push_back(0.90);
  thres_pt.push_back(0.9); value_pt.push_back(0.94);
  thres_pt.push_back(1.0); value_pt.push_back(0.95);
  thres_pt.push_back(2.0); value_pt.push_back(0.955);
  thres_pt.push_back(4.0); value_pt.push_back(0.945);
  thres_pt.push_back(pt_max); value_pt.push_back(0.94);

  int eta_nbins = 240;
  double eta_min = -1.2;
  double eta_max =  1.2;
  vector<double> thres_eta;
  vector<double> value_eta;
  thres_eta.push_back(-1.00); value_eta.push_back(0.01);
  thres_eta.push_back(-0.90); value_eta.push_back(0.4);
  thres_eta.push_back(-0.85); value_eta.push_back(0.91);
  thres_eta.push_back(-0.80); value_eta.push_back(1.00);
  thres_eta.push_back( 0.80); value_eta.push_back(1.00);
  thres_eta.push_back( 0.85); value_eta.push_back(0.91);
  thres_eta.push_back( 0.90); value_eta.push_back(0.4);
  thres_eta.push_back( 1.00); value_eta.push_back(0.01);

  //TH1 * h = setupEffVsPt(pt_nbins,pt_min,pt_max,thres_pt,value_pt);
  TH2 * h = setupEffVsEtaPt(eta_nbins,eta_min,eta_max, pt_nbins,pt_min,pt_max,thres_eta, value_eta, thres_pt,value_pt);

  histos2.push_back(h);
  histos2[0]->SetTitle("Eff");
  plotter->plot("Eff_vsEtaPt",histos2,
                TString("#eta"), eta_min, eta_max,
                TString("p_{T}"), pt_min, pt_max,
                TString("#varepsilon"),-0.1, 1.1,
                true, 0.5, 0.7, 0.6, 0.8, 0.05);
  histos2.clear();
 // plotter->printAllCanvas(path);

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

