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

TH2 * cloneLoadH2(TFile & inputFile, TString histoName)
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

void plotPionSingles(CAP::Plotter * plotter, vector<TFile*> & inputFiles)
{
  int linear = 0;
  int logY   = 1;
  double fontSize = 0.05;
  vector<TH1*> histos;
  vector<TH2*> histos2;

  histos.push_back( cloneLoadH1(*inputFiles[0],"Pair3D_ALL_PiP_n1_pt") );
  histos.push_back( cloneLoadH1(*inputFiles[0],"Pair3D_ALL_PiM_n1_pt"));
  histos[0]->GetYaxis()->SetTitle("#rho_{1}(pT)");
  histos[0]->SetTitle("#rho_{1}^{#pi^{+}}");
  histos[1]->SetTitle("#rho_{1}^{#pi^{-}}");
  plotter->plot("Pair3D_ALL_Pi_rho1_pT",histos,
                TString("p_{T}"), 0.0, 10.0,
                TString("#rho_{1}(pT)"),1.0E-7, 1.99,
                true, 0.5, 0.7, 0.6, 0.8, fontSize,logY);
  histos.clear();

  histos.push_back( cumulativeSum(*inputFiles[0],"Pair3D_ALL_PiP_n1_pt"));
  histos.push_back( cumulativeSum(*inputFiles[0],"Pair3D_ALL_PiM_n1_pt"));
  histos[0]->GetYaxis()->SetTitle("#int #rho_{1}^{#pi^{+}}");
  histos[0]->SetTitle("#rho_{1}^{#pi^{+}}");
  histos[1]->SetTitle("#rho_{1}^{#pi^{-}}");
  plotter->plot("Pair3D_ALL_Pi_int_rho1_pT",histos,
                TString("p_{T}"), 0.0, 10.0,
                TString("#rho_{1}(pT)"),-0.05, 29.99,
                true, 0.5, 0.7, 0.2, 0.45, fontSize);
  histos.clear();

  histos2.push_back( cloneLoadH2(*inputFiles[0],"Pair3D_ALL_PiP_n1_ptY"));
  histos2[0]->GetYaxis()->SetTitle("#rho_{1}^{#pi^{+}}");
  histos2[0]->SetTitle("#rho_{1}^{#pi^{+}}");
  plotter->plot("Pair3D_ALL_Pi_int_rho1_ptY",histos2,
                TString("y"), -10.0, 10.0,
                TString("p_{T}"), 0.0, 8.0,
                TString("#rho_{1}(pT)"),0.00001, 1.0,
                true, 0.5, 0.7, 0.2, 0.45, fontSize,1);
  histos2.clear();

  histos2.push_back( cloneLoadH2(*inputFiles[1],"Pair3D_ALL_PiP_PiM_n1_1_ptY_mc"));
  histos2[0]->GetYaxis()->SetTitle("#rho_{1}^{#pi^{+}}");
  histos2[0]->SetTitle("#rho_{1}^{#pi^{+}} MC");
  plotter->plot("Pair3D_ALL_Pi_int_rho1_ptY_MC",histos2,
                TString("y"), -10.0, 10.0,
                TString("p_{T}"), 0.0, 8.0,
                TString("#rho_{1}(pT)"),0.00001, 1.0,
                true, 0.5, 0.7, 0.2, 0.45, fontSize,1);
  histos2.clear();

  histos2.push_back( cloneLoadH2(*inputFiles[1],"Pair3D_ALL_PiP_PiM_n1r_1_ptY_mc"));
  histos2[0]->GetYaxis()->SetTitle("#rho_{1}^{#pi^{+}}");
  histos2[0]->SetTitle("#rho_{1}^{#pi^{+}} RATIO");
  plotter->plot("Pair3D_ALL_Pi_int_rho1_ptY_RATIO",histos2,
                TString("y"), -10.0, 10.0,
                TString("p_{T}"), 0.0, 2.0,
                TString("#rho_{1}(pT)"),0.5, 1.5,
                true, 0.5, 0.7, 0.2, 0.45, fontSize,2);
  histos2.clear();

}

void plotKaonSingles(CAP::Plotter * plotter, vector<TFile*> & inputFiles)
{
  int linear = 0;
  int logY   = 1;
  int logZ   = 2;
  double fontSize = 0.05;
  vector<TH1*> histos;
  vector<TH2*> histos2;

  histos.push_back( cloneLoadH1(*inputFiles[0],"Pair3D_ALL_KP_n1_pt") );
  histos.push_back( cloneLoadH1(*inputFiles[0],"Pair3D_ALL_KM_n1_pt"));
  histos[0]->GetYaxis()->SetTitle("#rho_{1}(pT)");
  histos[0]->SetTitle("#rho_{1}^{K^{+}}");
  histos[1]->SetTitle("#rho_{1}^{K^{-}}");
  plotter->plot("Pair3D_ALL_K_rho1_pT",histos,
                TString("p_{T}"), 0.0, 10.0,
                TString("#rho_{1}(pT)"),1.0E-7, 1.99,
                true, 0.5, 0.7, 0.6, 0.8, fontSize,logY);
  histos.clear();

  histos.push_back( cumulativeSum(*inputFiles[0],"Pair3D_ALL_KP_n1_pt"));
  histos.push_back( cumulativeSum(*inputFiles[0],"Pair3D_ALL_KM_n1_pt"));
  histos[0]->GetYaxis()->SetTitle("#int #rho_{1}^{K^{+}}");
  histos[0]->SetTitle("#rho_{1}^{Ki^{+}}");
  histos[1]->SetTitle("#rho_{1}^{K^{-}}");
  plotter->plot("Pair3D_ALL_K_int_rho1_pT",histos,
                TString("p_{T}"), 0.0, 10.0,
                TString("#rho_{1}(pT)"),-0.05, 29.99,
                true, 0.5, 0.7, 0.2, 0.45, fontSize);
  histos.clear();

  histos2.push_back( cloneLoadH2(*inputFiles[0],"Pair3D_ALL_KP_n1_ptY"));
  histos2[0]->GetYaxis()->SetTitle("#int #rho_{1}^{K^{+}}");
  histos2[0]->SetTitle("#rho_{1}^{K^{+}}");
  plotter->plot("Pair3D_ALL_K_int_rho1_ptY",histos2,
                TString("y"), -10.0, 10.0,
                TString("p_{T}"), 0.0, 8.0,
                TString("#rho_{1}(pT)"),0.00001, 1.0,
                true, 0.5, 0.7, 0.2, 0.45, fontSize,2);
  histos2.clear();

  histos2.push_back( cloneLoadH2(*inputFiles[1],"Pair3D_ALL_KP_KM_n1_1_ptY_mc"));
  histos2[0]->GetYaxis()->SetTitle("#int #rho_{1}^{K^{+}}");
  histos2[0]->SetTitle("#rho_{1}^{K^{+}} MC");
  plotter->plot("Pair3D_ALL_K_int_rho1_ptY_MC",histos2,
                TString("y"), -10.0, 10.0,
                TString("p_{T}"), 0.0, 8.0,
                TString("#rho_{1}(pT)"),0.00001, 1.0,
                true, 0.5, 0.7, 0.2, 0.45, fontSize,2);
  histos2.clear();

  histos2.push_back( cloneLoadH2(*inputFiles[1],"Pair3D_ALL_KP_KM_n1r_1_ptY_mc"));
  histos2[0]->GetYaxis()->SetTitle("#int #rho_{1}^{K^{+}}");
  histos2[0]->SetTitle("#rho_{1}^{K^{+}} RATIO");
  plotter->plot("Pair3D_ALL_K_int_rho1_ptY_RATIO",histos2,
                TString("y"), -10.0, 10.0,
                TString("p_{T}"), 0.0, 8.0,
                TString("#rho_{1}(pT)"),0.5, 1.5,
                true, 0.5, 0.7, 0.2, 0.45, fontSize,2);
  histos2.clear();

}


void plotPionsQinv(CAP::Plotter * plotter, vector<TFile*> & inputFiles)
{
  int linear = 0;
  int logY   = 1;
  int logZ   = 2;
  double fontSize = 0.03;
  vector<TH1*> histos;
  vector<TH2*> histos2;
  double Qmax = 2.0;
  double yieldMin = -0.1;
  double yieldMax = 6.499;

  histos.push_back( cloneLoadH1(*inputFiles[0],"Pair3D_ALL_PiP_PiP_n2_Qinv"));
  histos.push_back( cloneLoadH1(*inputFiles[0],"Pair3D_ALL_PiP_PiM_n2_Qinv"));
  histos.push_back( cloneLoadH1(*inputFiles[0],"Pair3D_ALL_PiM_PiP_n2_Qinv"));
  histos.push_back( cloneLoadH1(*inputFiles[0],"Pair3D_ALL_PiM_PiM_n2_Qinv"));
  histos[0]->GetYaxis()->SetTitle("#rho_{2}(Q_{inv})");
  histos[0]->SetTitle("#rho_{2}^{#pi^{+}#pi^{+}}");
  histos[1]->SetTitle("#rho_{2}^{#pi^{+}#pi^{-}}");
  histos[2]->SetTitle("#rho_{2}^{#pi^{-}#pi^{+}}");
  histos[3]->SetTitle("#rho_{2}^{#pi^{-}#pi^{-}}");
  plotter->plot("Pair3D_ALL_pi_pi_rho2VsQinv",histos,
                TString("Q_{inv}"), 0.0, Qmax,
                TString("#rho_{2}(Q_{inv})"),0.5, 19.99,
                true, 0.5, 0.7, 0.2, 0.45, fontSize,1);
  histos.clear();

  histos.push_back( cloneLoadH1(*inputFiles[1],"Pair3D_ALL_PiP_PiP_c2_Qinv"));
  histos.push_back( cloneLoadH1(*inputFiles[1],"Pair3D_ALL_PiP_PiM_c2_Qinv"));
  histos.push_back( cloneLoadH1(*inputFiles[1],"Pair3D_ALL_PiM_PiP_c2_Qinv"));
  histos.push_back( cloneLoadH1(*inputFiles[1],"Pair3D_ALL_PiM_PiM_c2_Qinv"));
  histos[0]->GetYaxis()->SetTitle("C_{2}(Q_{inv})");
  histos[0]->SetTitle("C_{2}^{#pi^{+}#pi^{+}}");
  histos[1]->SetTitle("C_{2}^{#pi^{+}#pi^{-}}");
  histos[2]->SetTitle("C_{2}^{#pi^{-}#pi^{+}}");
  histos[3]->SetTitle("C_{2}^{#pi^{-}#pi^{-}}");
  plotter->plot("Pair3D_ALL_Pi_Pi_C2VsQinv",histos,
                TString("Q_{inv}"), 0.0, Qmax,
                TString("C_{2}(Q_{inv})"),yieldMin, yieldMax,
                true, 0.5, 0.7, 0.2, 0.55, fontSize,1);
  histos.clear();

  histos.push_back( cloneLoadH1(*inputFiles[1],"Pair3D_ALL_PiP_PiP_A12_Qinv"));
  histos.push_back( cloneLoadH1(*inputFiles[1],"Pair3D_ALL_PiP_PiM_A12_Qinv"));
  histos.push_back( cloneLoadH1(*inputFiles[1],"Pair3D_ALL_PiM_PiP_A12_Qinv"));
  histos.push_back( cloneLoadH1(*inputFiles[1],"Pair3D_ALL_PiM_PiM_A12_Qinv"));
  histos[0]->GetYaxis()->SetTitle("A_{12}(Q_{inv})");
  histos[0]->SetTitle("A_{12}^{#pi^{+}#pi^{+}}");
  histos[1]->SetTitle("A_{12}^{#pi^{+}#pi^{-}}");
  histos[2]->SetTitle("A_{12}^{#pi^{-}#pi^{+}}");
  histos[3]->SetTitle("A_{12}^{#pi^{-}#pi^{-}}");
  plotter->plot("Pair3D_ALL_Pi_Pi_A12VsQinv",histos,
                TString("Q_{inv}"), 0.0, Qmax,
                TString("A_{2}(Q_{inv})"),yieldMin, 1.0,
                true, 0.5, 0.7, 0.2, 0.65, fontSize,0);
  histos.clear();


  TH1 * pippim_n2_qinv_diff = cloneLoadH1(*inputFiles[0],"Pair3D_ALL_PiP_PiM_n2_Qinv");
  TH1 * pimpip_n2_qinv_diff = cloneLoadH1(*inputFiles[0],"Pair3D_ALL_PiM_PiP_n2_Qinv");
  TH1 * pippip_n2_qinv = cloneLoadH1(*inputFiles[0],"Pair3D_ALL_PiP_PiP_n2_Qinv");
  TH1 * pimpim_n2_qinv = cloneLoadH1(*inputFiles[0],"Pair3D_ALL_PiM_PiM_n2_Qinv");

  pippim_n2_qinv_diff->Add(pimpim_n2_qinv,-1.0);
  pimpip_n2_qinv_diff->Add(pippip_n2_qinv,-1.0);

  histos.push_back( pippim_n2_qinv_diff);
  histos.push_back( pimpip_n2_qinv_diff);
  histos[0]->GetYaxis()->SetTitle("#Delta #rho_{2}(Q_{inv})");
  histos[0]->SetTitle("#rho_{2}^{#pi^{-}#pi^{+}} - #rho_{2}^{#pi^{+}#pi^{+}}");
  histos[1]->SetTitle("#rho_{2}^{#pi^{+}#pi^{-}} - #rho_{2}^{#pi^{-}#pi^{-}}");
  plotter->plot("Pair3D_ALL_Pi_Pi_rho2DiffVsQinv",histos,
                TString("Q_{inv}"), 0.0, Qmax,
                TString("#rho_{2}(Q_{inv})"),yieldMin, 5*yieldMax,
                true, 0.6, 0.75, 0.5, 0.85, 0.01,0);
  histos.clear();

  histos.push_back( cloneLoadH1(*inputFiles[0],"Pair3D_ALL_PiP_PiP_n2_Qinv"));
  histos.push_back( cloneLoadH1(*inputFiles[0],"Pair3D_ALL_PiP_PiM_n2_Qinv"));
  histos.push_back( cloneLoadH1(*inputFiles[1],"Pair3D_ALL_PiP_PiP_n1n1_Qinv"));
  histos.push_back( cloneLoadH1(*inputFiles[1],"Pair3D_ALL_PiP_PiM_n1n1_Qinv"));
  histos[0]->GetYaxis()->SetTitle("n_{2}^{#pi^{+}#pi^{+}}(Q_{inv})");
  histos[0]->SetTitle("n_{2}^{#pi^{+}#pi^{+}}");
  histos[1]->SetTitle("n_{2}^{#pi^{+}#pi^{-}}");
  histos[2]->SetTitle("n_{1}n_{1}^{#pi^{+}#pi^{+}}");
  histos[3]->SetTitle("n_{1}n_{1}^{#pi^{+}#pi^{-}}");
  plotter->plot("Pair3D_ALL_PiP_PiP_n2_vs_n1n1",histos,
                TString("Q_{inv}"), 0.0, Qmax,
                TString("n_{2}(Q_{inv})"),yieldMin, 499.99,
                true, 0.3, 0.5, 0.2, 0.5, 0.01,0);
  histos.clear();

  histos.push_back( cloneLoadH1(*inputFiles[1],"Pair3D_ALL_PiM_PiM_n1r_1_pt_mc"));
  histos.push_back( cloneLoadH1(*inputFiles[1],"Pair3D_ALL_PiM_PiM_n1r_2_pt_mc"));
  histos[0]->GetYaxis()->SetTitle("n_{1}^{MC}/n_{1}");
  histos[0]->SetTitle("n_{1}^{MC}/n_{1}");
  histos[1]->SetTitle("n_{1}^{MC}/n_{1}");
  plotter->plot("Pair3D_ALL_PiP_PiP_n1r_1_pt_mc",histos,
                TString("p_{T}"), 0.0, -1.0,
                TString("n_{1}^{MC}/n_{1}"),yieldMin, 49.99,
                true, 0.3, 0.5, 0.2, 0.5, 0.01,1);
  histos.clear();


//  histos.push_back( cloneLoadH1(*inputFiles[0],"Pair3D_ALL_PiP_PiM_n2_Qinv"));
//  histos.push_back( cloneLoadH1(*inputFiles[1],"Pair3D_ALL_PiP_PiM_n1n1_Qinv"));
//  histos[0]->GetYaxis()->SetTitle("n_{2}^{#pi^{-}#pi^{+}}(Q_{inv})");
//  histos[0]->SetTitle("n_{2}^{#pi^{+}#pi^{-}}");
//  histos[1]->SetTitle("n_{1}n_{1}^{#pi^{+}#pi^{-}}");
//  plotter->plot("Pair3D_ALL_PiP_PiM_n2_vs_n1n1",histos,
//                TString("Q_{inv}"), 0.0, Qmax,
//                TString("n_{2}(Q_{inv})"),-0.1, 49.99,
//                true, 0.5, 0.7, 0.2, 0.65, 0.05,1);
//  histos.clear();


}

void plotKaonsQinv(CAP::Plotter * plotter, vector<TFile*> & inputFiles)
{
  int linear = 0;
  int logY   = 1;
  int logZ   = 2;
  double fontSize = 0.04;
  vector<TH1*> histos;
  vector<TH2*> histos2;

  histos.push_back( cloneLoadH1(*inputFiles[0],"Pair3D_ALL_KP_KP_n2_Qinv"));
  histos.push_back( cloneLoadH1(*inputFiles[0],"Pair3D_ALL_KP_KM_n2_Qinv"));
  histos.push_back( cloneLoadH1(*inputFiles[0],"Pair3D_ALL_KM_KP_n2_Qinv"));
  histos.push_back( cloneLoadH1(*inputFiles[0],"Pair3D_ALL_KM_KM_n2_Qinv"));
  histos[0]->GetYaxis()->SetTitle("#rho_{2}(Q_{inv})");
  histos[0]->SetTitle("#rho_{2}^{K^{+}K^{+}}");
  histos[1]->SetTitle("#rho_{2}^{K^{+}K^{-}}");
  histos[2]->SetTitle("#rho_{2}^{K^{-}K^{+}}");
  histos[3]->SetTitle("#rho_{2}^{K^{-}K^{-}}");
  plotter->plot("Pair3D_ALL_K_K_rho2VsQinv",histos,
                TString("Q_{inv}"), 0.0, 500.0,
                TString("#rho_{2}(Q_{inv})"),0.0001, 0.549,
                true, 0.5, 0.7, 0.2, 0.55, fontSize,1);
  histos.clear();

  histos.push_back( cloneLoadH1(*inputFiles[1],"Pair3D_ALL_KP_KP_c2_Qinv"));
  histos.push_back( cloneLoadH1(*inputFiles[1],"Pair3D_ALL_KP_KM_c2_Qinv"));
  histos.push_back( cloneLoadH1(*inputFiles[1],"Pair3D_ALL_KM_KP_c2_Qinv"));
  histos.push_back( cloneLoadH1(*inputFiles[1],"Pair3D_ALL_KM_KM_c2_Qinv"));
  histos[0]->GetYaxis()->SetTitle("C_{2}(Q_{inv})");
  histos[0]->SetTitle("C_{2}^{K^{+}K^{+}}");
  histos[1]->SetTitle("C_{2}^{K^{+}K^{-}}");
  histos[2]->SetTitle("C_{2}^{K^{-}K^{+}}");
  histos[3]->SetTitle("C_{2}^{K^{-}K^{-}}");
  plotter->plot("Pair3D_ALL_K_K_C2VsQinv",histos,
                TString("Q_{inv}"), 0.0, 500.0,
                TString("C_{2}(Q_{inv})"),0.5, 1.5,
                true, 0.5, 0.7, 0.2, 0.55, fontSize,0);
  histos.clear();

  histos.push_back( cloneLoadH1(*inputFiles[1],"Pair3D_ALL_KP_KP_A12_Qinv"));
  histos.push_back( cloneLoadH1(*inputFiles[1],"Pair3D_ALL_KP_KM_A12_Qinv"));
  histos.push_back( cloneLoadH1(*inputFiles[1],"Pair3D_ALL_KM_KP_A12_Qinv"));
  histos.push_back( cloneLoadH1(*inputFiles[1],"Pair3D_ALL_KM_KM_A12_Qinv"));
  histos[0]->GetYaxis()->SetTitle("A_{12}(Q_{inv})");
  histos[0]->SetTitle("A_{12}^{K^{+}K^{+}}");
  histos[1]->SetTitle("A_{12}^{K^{+}K^{-}}");
  histos[2]->SetTitle("A_{12}^{K^{-}K^{+}}");
  histos[3]->SetTitle("A_{12}^{K^{-}K^{-}}");
  plotter->plot("Pair3D_ALL_K_K_A12VsQinv",histos,
                TString("Q_{inv}"), 0.0, 500.0,
                TString("A_{2}(Q_{inv})"),-0.5, 0.5,
                true, 0.5, 0.7, 0.2, 0.55, fontSize,0);
  histos.clear();


  TH1 * kpkm_n2_qinv_diff = cloneLoadH1(*inputFiles[0],"Pair3D_ALL_KP_KM_n2_Qinv");
  TH1 * kmkp_n2_qinv_diff = cloneLoadH1(*inputFiles[0],"Pair3D_ALL_KM_KP_n2_Qinv");
  TH1 * kpkp_n2_qinv = cloneLoadH1(*inputFiles[0],"Pair3D_ALL_KP_KP_n2_Qinv");
  TH1 * kmkm_n2_qinv = cloneLoadH1(*inputFiles[0],"Pair3D_ALL_KM_KM_n2_Qinv");

  kpkm_n2_qinv_diff->Add(kmkm_n2_qinv,-1.0);
  kmkp_n2_qinv_diff->Add(kpkp_n2_qinv,-1.0);

  histos.push_back( kpkm_n2_qinv_diff);
  histos.push_back( kmkp_n2_qinv_diff);
  histos[0]->GetYaxis()->SetTitle("#Delta #rho_{2}(Q_{inv})");
  histos[0]->SetTitle("#rho_{2}^{K^{-}K^{+}} - #rho_{2}^{K^{+}K^{+}}");
  histos[1]->SetTitle("#rho_{2}^{K^{+}K^{-}} - #rho_{2}^{K^{-}K^{-}}");
  plotter->plot("Pair3D_ALL_K_K_rho2DiffVsQinv",histos,
                TString("Q_{inv}"), 0.0, 500.0,
                TString("#rho_{2}(Q_{inv})"),-0.005, 0.1,
                true, 0.5, 0.7, 0.5, 0.75, fontSize,0);
  histos.clear();


}




//int PlotBF(const TString & path="/Volumes/ClaudeDisc4/OutputFiles/pythiaTest3Dex/",
int PlotBF(const TString & path="/Volumes/ClaudeDisc4/OutputFiles/GaussTest/",
           const TString & fileName0="Pair3DGen.root",
           const TString & fileName1="Pair3DGenDerived.root",
           const TString & fileName2="Pair3DGenBF.root")
{
  //gROOT->SetWebDisplay();

  //bool doPrint = true;
  bool doPrint = false;

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
  plotter->addCanvasConfiguartion(CanvasConfiguration::LandscapeWide,CanvasConfiguration::LogZ);
  plotter->addGraphConfiguration(10,1);
  int linear = 0;
  int logY   = 1;
  try
  {

  TString inputFileName0 = path+fileName0;
  TString inputFileName1 = path+fileName1;
  TString inputFileName2 = path+fileName2;
  vector<TFile*> inputFiles;
  inputFiles.push_back( new TFile(inputFileName0,"OLD") );
  inputFiles.push_back( new TFile(inputFileName1,"OLD") );
  inputFiles.push_back( new TFile(inputFileName2,"OLD") );

  ROOT::RBrowser * b = new ROOT::RBrowser();
  vector<TH1*>    histos;
  double legendSize = 0.3;

  plotPionSingles(plotter,inputFiles);
//  plotKaonSingles(plotter,inputFiles);
  plotPionsQinv(plotter,inputFiles);
//  plotKaonsQinv(plotter,inputFiles);
  if (doPrint) plotter->printAllCanvas(path);
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

