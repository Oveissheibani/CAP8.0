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

double polynomial(double x, const vector<double> & coeffs)
{
  int n = coeffs.size();
  double v = coeffs[0];
  double xx = x;
  for (int k=1;k<n;k++)
    {
    v += coeffs[k]*xx;
    xx *= x;
    }
  return v;
}

TH1 * createHistVsMult(const TString & histoName,
                        int    nBins_mult,
                        double min_emult,
                        double max_mult)
{
  TH1 * h = new TH1D(histoName,histoName,nBins_mult,min_emult,max_mult);
  h->SetXTitle("mult");
  h->SetYTitle("#epsilon(mult})");
  double mult;
  for (int iMult=1; iMult<=nBins_mult; iMult++)
    {
    mult = h->GetXaxis()->GetBinCenter(iMult);
    h->SetBinContent(iMult,1.0);
    h->SetBinError(iMult,0.0);
    }
  return h;
}


TH2 * createHistVsEtaPt(const TString & histoName,
                        int    eta_nbins,
                        double eta_min,
                        double eta_max,
                        int    pt_nbins,
                        double pt_min,
                        double pt_max,
                        const vector<double> & etaCoeff,
                        const vector<double> & ptCoeff)
{
  TH2 * h = new TH2D(histoName,histoName,eta_nbins,eta_min,eta_max,pt_nbins,pt_min,pt_max);
  h->SetXTitle("#eta");
  h->SetYTitle("p_{T}");
  h->SetZTitle("#epsilon(#eta,p_{T})");

  double pt, eta, eff;

  for (int iEta=1; iEta<=eta_nbins; iEta++)
    {
    eta = h->GetXaxis()->GetBinCenter(iEta);
    for (int iPt=1; iPt<=pt_nbins; iPt++)
      {
      pt = h->GetYaxis()->GetBinCenter(iPt);
      double v1 = polynomial(eta,etaCoeff);
      double v2 = polynomial(pt,ptCoeff);
      h->SetBinContent(iEta,iPt, v1*v2);
      h->SetBinError(iEta,iPt,0.0);
      }
    }
  return h;
}

int CreateEfficiencyHistograms(const TString & fileName="/Volumes/ClaudeDisc4/OutputFiles/pythiaTest/Perform.root")
{
  TFile * file = new TFile(fileName,"RECREATE");

  vector<double> etaCoeff;
  vector<double> ptCoeff;
  vector<TH2*> histos;

  //ROOT::RBrowser * b = new ROOT::RBrowser();

  TH1 * h1 = createHistVsMult("EVENT_EFF_ALL_MULT",100,1.0,10000.);
  h1->Write();

  int eta_nbins = 40;
  double eta_min = -4.0;
  double eta_max =  4.0;
  int pt_nbins = 50;
  double pt_min = 0.0;
  double pt_max = 10.0;

  etaCoeff.push_back(0.9);
  etaCoeff.push_back(0.0);
  etaCoeff.push_back(-0.002);
  etaCoeff.push_back(0.0);
  etaCoeff.push_back(-0.002);
  ptCoeff.push_back(0.95);
  ptCoeff.push_back(-0.001);
  ptCoeff.push_back(-0.0005);
  histos.push_back(createHistVsEtaPt("PART_EFF_ALL_ALL_etaPt",eta_nbins,eta_min,eta_max,pt_nbins,pt_min,pt_max,etaCoeff,ptCoeff));

  etaCoeff.clear();
  ptCoeff.clear();
  etaCoeff.push_back(1.0);
  etaCoeff.push_back(0.0);
  etaCoeff.push_back(0.02);
  ptCoeff.push_back(0.01);
  ptCoeff.push_back(0.001);
  ptCoeff.push_back(-0.0005);
  histos.push_back(createHistVsEtaPt("PART_RMS_PT_ALL_ALL_etaPt",eta_nbins,eta_min,eta_max,pt_nbins,pt_min,pt_max,etaCoeff,ptCoeff));

  etaCoeff.clear();
  ptCoeff.clear();
  etaCoeff.push_back(1.0);
  etaCoeff.push_back(0.0);
  etaCoeff.push_back(0.01);
  ptCoeff.push_back(0.01);
  ptCoeff.push_back(0.001);
  ptCoeff.push_back(0.0005);
  histos.push_back(createHistVsEtaPt("PART_BIAS_PT_ALL_ALL_etaPt",eta_nbins,eta_min,eta_max,pt_nbins,pt_min,pt_max,etaCoeff,ptCoeff));

  etaCoeff.clear();
  ptCoeff.clear();
  etaCoeff.push_back(1.0);
  etaCoeff.push_back(0.0);
  etaCoeff.push_back(0.01);
  ptCoeff.push_back(0.01);
  ptCoeff.push_back(0.001);
  ptCoeff.push_back(0.0005);
  histos.push_back(createHistVsEtaPt("PART_RMS_ETA_ALL_ALL_etaPt",eta_nbins,eta_min,eta_max,pt_nbins,pt_min,pt_max,etaCoeff,ptCoeff));

  etaCoeff.clear();
  ptCoeff.clear();
  etaCoeff.push_back(1.0);
  etaCoeff.push_back(0.0);
  etaCoeff.push_back(0.01);
  ptCoeff.push_back(0.01);
  ptCoeff.push_back(0.001);
  ptCoeff.push_back(0.0005);
  histos.push_back(createHistVsEtaPt("PART_BIAS_ETA_ALL_ALL_etaPt",eta_nbins,eta_min,eta_max,pt_nbins,pt_min,pt_max,etaCoeff,ptCoeff));

  etaCoeff.clear();
  ptCoeff.clear();
  etaCoeff.push_back(1.0);
  etaCoeff.push_back(0.0);
  etaCoeff.push_back(0.01);
  ptCoeff.push_back(0.01);
  ptCoeff.push_back(0.001);
  ptCoeff.push_back(0.0005);
  histos.push_back(createHistVsEtaPt("PART_RMS_PHI_ALL_ALL_etaPt",eta_nbins,eta_min,eta_max,pt_nbins,pt_min,pt_max,etaCoeff,ptCoeff));

  etaCoeff.clear();
  ptCoeff.clear();
  etaCoeff.push_back(1.0);
  etaCoeff.push_back(0.0);
  etaCoeff.push_back(0.01);
  ptCoeff.push_back(0.01);
  ptCoeff.push_back(0.001);
  ptCoeff.push_back(0.0005);
  histos.push_back(createHistVsEtaPt("PART_BIAS_PHI_ALL_ALL_etaPt",eta_nbins,eta_min,eta_max,pt_nbins,pt_min,pt_max,etaCoeff,ptCoeff));

  for (auto & h : histos) h->Write();
  file->Close();
  return 0;
}


