#include "ThermalHistos.hpp"
#include "PrintHelpers.hpp"
#include "NameHelpers.hpp"
#include "RootHistogramHelpers.hpp"
#include "TCanvas.h"
#include "TLine.h"
#include "TArrow.h"
#include "TFrame.h"
#include "TStyle.h"
#include "ParticleDb.hpp"
#include "ThermalHadron.hpp"
#include "ThermalHadronGas.hpp"

ClassImp(CAP::ThermalHistos);

namespace CAP
{
  TLine * createLine(float x1, float y1, float x2, float y2, int style, int color, int width, bool doDraw)
  {
  TLine *line = new TLine(x1,y1,x2,y2);
  line->SetLineStyle(style);
  line->SetLineColor(color);
  line->SetLineWidth(width);
  if (doDraw) line->Draw();
  return line;
  }
  
  //!
  //!Create Arrow Line
  //!
  TArrow * createArrow(float x1, float y1, float x2, float y2, float arrowSize, Option_t* option, int style, int color, int width, bool doDraw)
  {
  TArrow *line = new TArrow(x1,y1,x2,y2,arrowSize,option);
  line->SetLineStyle(style);
  line->SetLineColor(color);
  line->SetLineWidth(width);
  line->SetFillColor(color);
  
  if (doDraw) line->Draw();
  return line;
  }
  
  
  
  ThermalHistos::ThermalHistos()
  :
  HistogramGroup()
  {
  appendClassName("ThermalHistos");
  setMinimumReportLevel(Object::kInfo);
  setName("ThermalHistos");
  setTitle("ThermalHistos");
  }
  
  ThermalHistos::ThermalHistos(const ThermalHistos & source __attribute__((unused)))
  {   }
  
  ThermalHistos ThermalHistos::operator=(const ThermalHistos & rhs __attribute__((unused)))
  {
  return *this;
  }
  
  
  
  void ThermalHistos::configure(const String & taskName,
                                const String & objectType,
                                const Configuration & configuration,
                                unsigned int index  __attribute__((unused)))
  {
  _histogramBaseName = configuration.valueString(createKey(taskName,objectType,"BASE_NAME"));
  index_fill  = configuration.valueBool(createKey(taskName,objectType,"index_fill"));
  index_nbins = configuration.valueInt(createKey(taskName,objectType,"index_nbins"));
  index_min   = configuration.valueDouble(createKey(taskName,objectType,"index_min"));
  index_max   = configuration.valueDouble(createKey(taskName,objectType,"index_max"));
  mass_fill   = configuration.valueBool(createKey(taskName,objectType,"mass_fill"));
  mass_nbins  = configuration.valueInt(createKey(taskName,objectType,"mass_nbins"));
  mass_min    = configuration.valueDouble(createKey(taskName,objectType,"mass_min"));
  mass_max    = configuration.valueDouble(createKey(taskName,objectType,"mass_max"));
  }
  
  void ThermalHistos::create()
  {
  if (reportStart(__FUNCTION__)) { printCR(); }
  //stables
  unsigned int n1 = 15;
  double n1Min = -0.001;
  double n1Max = double(n1)-0.001;
  //int n2 = n1*n1;
  //double n2Min = -0.5;
  //double n2Max = double(n2)-0.5;
  String yTitle  = "#rho_{2}^{#alpha|#beta}";
  String yTitle2 = "#rho_{2}^{#alpha|#beta}/#int #rho_{2}^{#alpha|#beta}";
  
  h_massVsIndex = createHistogram(createName(_histogramBaseName,"_mass"),  index_nbins,index_min,index_max,"#alpha","m (GeV/c2)");
  diag          = createHistogram(createName(_histogramBaseName,"mass_spectrum"),  332,0.0,333.0,"","Mass (GeV/c^{2})");
  
  
  if (index_fill)
    {
    h_numberDensityVsIndex    = createHistogram(createName(_histogramBaseName,"_nDensity"),  index_nbins,index_min,index_max,"#alpha","n_{1}");
    h_energyDensityVsIndex    = createHistogram(createName(_histogramBaseName,"_eDensity"),  index_nbins,index_min,index_max,"#alpha","e");
    h_entropyDensityVsIndex   = createHistogram(createName(_histogramBaseName,"_sDensity"),  index_nbins,index_min,index_max,"#alpha","s");
    h_pressureVsIndex         = createHistogram(createName(_histogramBaseName,"_pressure"),       index_nbins,index_min,index_max,"#alpha","p");
    h_densityWFDVsIndex       = createHistogram(createName(_histogramBaseName,"densityWFeed"),   index_nbins,index_min,index_max,"#alpha","nfeed");
    h_netBaryonDensityVsIndex = createHistogram(createName(_histogramBaseName,"netBaryonDensity"), index_nbins,index_min,index_max,"#alpha","Baryon");
    h_netChargeDensityVsIndex = createHistogram(createName(_histogramBaseName,"netChargeDensity"), index_nbins,index_min,index_max,"#alpha","Q");
    h_netStrangeDensityVsIndex= createHistogram(createName(_histogramBaseName,"netStrangeDensity"),index_nbins,index_min,index_max,"#alpha","S");
    h_charmDensityVsIndex  = createHistogram(createName(_histogramBaseName,"charmDensity"),  index_nbins,index_min,index_max,"#alpha","C");
    h_bottomDensityVsIndex = createHistogram(createName(_histogramBaseName,"bottomDensity"), index_nbins,index_min,index_max,"#alpha","B");
    h_topDensityVsIndex    = createHistogram(createName(_histogramBaseName,"topDensity"),    index_nbins,index_min,index_max,"#alpha","T");
    
    for (unsigned int iStable=0; iStable<n1-1; iStable++)
      {
      String name;
      name = "pair_"; name += iStable;
      h_pairDensityVsIndex.push_back(createHistogram(createName(_histogramBaseName,name),  n1+2,n1Min,n1Max+2,"#alpha",yTitle));
      name = "normPair_"; name += iStable;
      h_normPairDensityVsIndex.push_back(createHistogram(createName(_histogramBaseName,name),  n1,n1Min,n1Max,"#alpha",yTitle2));
      }
    
    h_relNumberDensityVsIndex    = createHistogram(createName(_histogramBaseName,"relNumberDensity"),  index_nbins,index_min,index_max,"#alpha","#_rho(#alpha)/#_rho(#pi)");
    //    h_relEnergyDensityVsIndex    = createHistogram(createName(_histogramBaseName,"relEnergyDensity"),  index_nbins,index_min,index_max,"#alpha","e");
    //    h_relEntropyDensityVsIndex   = createHistogram(createName(_histogramBaseName,"relEntropyDensity"), index_nbins,index_min,index_max,"#alpha","s");
    
    //stable
    h_singleDensityVsIndex        = createHistogram(createName(_histogramBaseName,"singleDensityVsIndex"),     n1,n1Min,n1Max,"Stable Index","#rho_{1}");
    h_singleDensityWFDVsIndex     = createHistogram(createName(_histogramBaseName,"singleDensityFDVsIndex"),   n1,n1Min,n1Max,"Stable Index","#rho_{1}");
    h_singleDensityRatioVsIndex   = createHistogram(createName(_histogramBaseName,"singleDensityRatioVsIndex"),n1,n1Min,n1Max,"#alpha","#_rho^{TH+FD}(#alpha)/#_rho^{TH}(#alpha)");
    h_relDensityWFDVsIndex        = createHistogram(createName(_histogramBaseName,"relWtoPiVsIndex"),          n1,n1Min,n1Max,"#alpha","#_rho(#alpha)/#_rho(#pi)");
    }
  
  if (mass_fill)
    {
    h_numberDensityVsMass     = createProfile(createName(_histogramBaseName,"numberDensityVsM"),  mass_nbins,mass_min,mass_max,"M (GeV/c^{2})","n");
    h_energyDensityVsMass     = createProfile(createName(_histogramBaseName,"energyDensityVsM"),  mass_nbins,mass_min,mass_max,"M (GeV/c^{2})","e");
    h_entropyDensityVsMass    = createProfile(createName(_histogramBaseName,"entropyDensityVsM"), mass_nbins,mass_min,mass_max,"M (GeV/c^{2})","s");
    h_pressureVsMass          = createProfile(createName(_histogramBaseName,"pressureVsM"),       mass_nbins,mass_min,mass_max,"M (GeV/c^{2})","p");
    h_densityWFDVsMass        = createProfile(createName(_histogramBaseName,"densityWFeedVsM"),     mass_nbins,mass_min,mass_max,"M (GeV/c^{2})","D");
    h_netBaryonDensityVsMass  = createProfile(createName(_histogramBaseName,"netBaryonDensityVsM"), mass_nbins,mass_min,mass_max,"M (GeV/c^{2})","Baryon");
    h_netChargeDensityVsMass  = createProfile(createName(_histogramBaseName,"netChargeDensityVsM"), mass_nbins,mass_min,mass_max,"M (GeV/c^{2})","Q");
    h_netStrangeDensityVsMass = createProfile(createName(_histogramBaseName,"netStrangeDensityVsM"),mass_nbins,mass_min,mass_max,"M (GeV/c^{2})","S");
    h_charmDensityVsMass   = createProfile(createName(_histogramBaseName,"charmDensityVsM"),  mass_nbins,mass_min,mass_max,"M (GeV/c^{2})","C");
    h_bottomDensityVsMass  = createProfile(createName(_histogramBaseName,"bottomDensityVsM"), mass_nbins,mass_min,mass_max,"M (GeV/c^{2})","B");
    h_topDensityVsMass     = createProfile(createName(_histogramBaseName,"topDensityVsM"),    mass_nbins,mass_min,mass_max,"M (GeV/c^{2})","T");
    
    h_relNumberDensityVsMass  = createProfile(createName(_histogramBaseName,"relNumberDensityVsM"), mass_nbins,mass_min,mass_max,"M (GeV/c^{2})","#_rho(#alpha)/#_rho(#pi)");
    h_relDensityWFDVsMass     = createProfile(createName(_histogramBaseName,"relWtoPiVsM"),         mass_nbins,mass_min,mass_max,"M (GeV/c^{2})","#_rho^{FD}(#alpha)/#_rho(#pi)");
    }
  }
  
  // Fix me...
  void ThermalHistos::loadFrom(TFile & inputFile __attribute__((unused)))
  {
  
  }
  
  void ThermalHistos::cloneB(const ThermalHistos & source)
  {
  h_numberDensityVsIndex     = safeCloneH1(source.h_numberDensityVsIndex);
  h_energyDensityVsIndex     = safeCloneH1(source.h_energyDensityVsIndex);
  h_entropyDensityVsIndex    = safeCloneH1(source.h_entropyDensityVsIndex);
  h_pressureVsIndex          = safeCloneH1(source.h_pressureVsIndex);
  h_densityWFDVsIndex      = safeCloneH1(source.h_densityWFDVsIndex);
  h_netBaryonDensityVsIndex  = safeCloneH1(source.h_netBaryonDensityVsIndex);
  h_netChargeDensityVsIndex  = safeCloneH1(source.h_netChargeDensityVsIndex);
  h_netStrangeDensityVsIndex = safeCloneH1(source.h_netStrangeDensityVsIndex);
  h_charmDensityVsIndex   = safeCloneH1(source.h_charmDensityVsIndex);
  h_bottomDensityVsIndex  = safeCloneH1(source.h_bottomDensityVsIndex);
  h_topDensityVsIndex     = safeCloneH1(source.h_topDensityVsIndex);
  
  h_numberDensityVsMass      = safeCloneProfile(source.h_numberDensityVsMass);
  h_energyDensityVsMass      = safeCloneProfile(source.h_energyDensityVsMass);
  h_entropyDensityVsMass     = safeCloneProfile(source.h_entropyDensityVsMass);
  h_pressureVsMass           = safeCloneProfile(source.h_pressureVsMass);
  h_densityWFDVsMass       = safeCloneProfile(source.h_densityWFDVsMass);
  h_netBaryonDensityVsMass   = safeCloneProfile(source.h_netBaryonDensityVsMass);
  h_netChargeDensityVsMass   = safeCloneProfile(source.h_netChargeDensityVsMass);
  h_netStrangeDensityVsMass  = safeCloneProfile(source.h_netStrangeDensityVsMass);
  h_charmDensityVsMass    = safeCloneProfile(source.h_charmDensityVsMass);
  h_bottomDensityVsMass   = safeCloneProfile(source.h_bottomDensityVsMass);
  h_topDensityVsMass      = safeCloneProfile(source.h_topDensityVsMass);
  }
  
  double NoInf(double num,double denom1,double denom2=1.0)
  {
  if (denom1 == 0.0) return 0.0;
  return num/(denom1*denom2);
  }
  
  void ThermalHistos::fill(const ThermalHadronGasFD & hadronGas)
  {
  if (reportInfo(__FUNCTION__)) { printCR(); }
  //double  zero = 0.0;
  const vector<ThermalHadron> & hadrons        = hadronGas.hadrons();
  const vector<ThermalHadron*> & stableHadrons = hadronGas.stableHadrons();
  const vector<double> & stableHadronPairs     = hadronGas.stableHadronPairs();
  //unsigned int nHadrons           = hadrons.size();
  unsigned int nStableHadrons     = stableHadrons.size();
  //unsigned int nStableHadronPairs = stableHadronPairs.size();
  // thermal particle (undecayed)
  unsigned int iHadron = 1;
  const ThermalHadron & pion = hadrons[1];
  double pionSpinFactor = pion.type().spinFactor();
  double zero = 0.0;
  for (auto & hadron : hadrons)
    {
    double _mass  = hadron.type().mass();
    double width = hadron.type().width();
    double gspin = hadron.type().spinFactor()/pionSpinFactor;
    h_massVsIndex->SetBinContent(iHadron,_mass);
    h_massVsIndex->SetBinError(iHadron,width);
    //double _gSpin = hadron.type().spinFactor();
    if (index_fill)
      {
      h_numberDensityVsIndex    ->SetBinContent(iHadron,hadron.numberDensity());
      h_energyDensityVsIndex    ->SetBinContent(iHadron,hadron.energyDensity());
      h_entropyDensityVsIndex   ->SetBinContent(iHadron,hadron.entropyDensity());
      h_pressureVsIndex         ->SetBinContent(iHadron,hadron.partialPressure());
      h_netBaryonDensityVsIndex ->SetBinContent(iHadron,hadron.baryonDensity());
      h_netChargeDensityVsIndex ->SetBinContent(iHadron,hadron.chargeDensity());
      h_netStrangeDensityVsIndex->SetBinContent(iHadron,hadron.strangeDensity());
      h_charmDensityVsIndex     ->SetBinContent(iHadron,hadron.charmDensity());
      h_bottomDensityVsIndex    ->SetBinContent(iHadron,hadron.bottomDensity());
      h_topDensityVsIndex       ->SetBinContent(iHadron,hadron.topDensity());
      h_densityWFDVsIndex       ->SetBinContent(iHadron,hadron.numberDensityWFD());
      h_relNumberDensityVsIndex ->SetBinContent(iHadron,NoInf(hadron.numberDensity(),pion.numberDensity(),gspin));
      h_relDensityWFDVsIndex    ->SetBinContent(iHadron,NoInf(hadron.numberDensityWFD(),pion.numberDensityWFD(),gspin));
      
      h_numberDensityVsIndex    ->SetBinError(iHadron,zero);
      h_energyDensityVsIndex    ->SetBinError(iHadron,zero);
      h_entropyDensityVsIndex   ->SetBinError(iHadron,zero);
      h_pressureVsIndex         ->SetBinError(iHadron,zero);
      h_netBaryonDensityVsIndex ->SetBinError(iHadron,zero);
      h_netChargeDensityVsIndex ->SetBinError(iHadron,zero);
      h_netStrangeDensityVsIndex->SetBinError(iHadron,zero);
      h_charmDensityVsIndex     ->SetBinError(iHadron,zero);
      h_bottomDensityVsIndex    ->SetBinError(iHadron,zero);
      h_topDensityVsIndex       ->SetBinError(iHadron,zero);
      h_densityWFDVsIndex       ->SetBinError(iHadron,zero);
      h_relNumberDensityVsIndex ->SetBinError(iHadron,zero);
      h_relDensityWFDVsIndex    ->SetBinError(iHadron,zero);
      
      iHadron++;
      }
    
    if (mass_fill)
      {
      h_numberDensityVsMass    ->Fill(_mass,hadron.numberDensity()/gspin);
      h_energyDensityVsMass    ->Fill(_mass,hadron.energyDensity());
      h_entropyDensityVsMass   ->Fill(_mass,hadron.entropyDensity());
      h_pressureVsMass         ->Fill(_mass,hadron.partialPressure());
      h_densityWFDVsMass       ->Fill(_mass,hadron.numberDensityWFD());
      h_netBaryonDensityVsMass ->Fill(_mass,hadron.baryonDensity());
      h_netChargeDensityVsMass ->Fill(_mass,hadron.chargeDensity());
      h_netStrangeDensityVsMass->Fill(_mass,hadron.strangeDensity());
      h_charmDensityVsMass     ->Fill(_mass,hadron.charmDensity());
      h_bottomDensityVsMass    ->Fill(_mass,hadron.bottomDensity());
      h_topDensityVsMass       ->Fill(_mass,hadron.topDensity());
      h_relNumberDensityVsMass ->Fill(_mass,NoInf(hadron.numberDensity(),pion.numberDensity(),gspin));
      h_relDensityWFDVsMass    ->Fill(_mass,NoInf(hadron.numberDensityWFD(),pion.numberDensityWFD(),gspin));
      }
    }
  
  // Stable particles
  for (unsigned int iHadron = 1; iHadron<nStableHadrons; iHadron++)
    {
    unsigned int iHadron1 = iHadron + 1;
    h_singleDensityVsIndex->SetBinContent(iHadron1,stableHadrons[iHadron]->numberDensity());
    h_singleDensityVsIndex->SetBinError(iHadron1,0.0);
    h_singleDensityWFDVsIndex->SetBinContent(iHadron1,stableHadrons[iHadron]->numberDensityWFD());
    h_singleDensityWFDVsIndex->SetBinError(iHadron1,0.0);
    }
  h_singleDensityRatioVsIndex->Divide(h_singleDensityWFDVsIndex,h_singleDensityVsIndex);
  
  for (unsigned int iHadron1 = 0; iHadron1<nStableHadrons; iHadron1++)
    {
    TH1 * hPair = h_pairDensityVsIndex[iHadron1];
    TH1 * hNormPair= h_normPairDensityVsIndex[iHadron1];
    //unsigned int iHadron1P = iHadron1+1;
    double sum = 0;
    for (unsigned int iHadron2 = 1; iHadron2<nStableHadrons; iHadron2++)
      {
      unsigned int iHadron2P = iHadron2+1;
      unsigned int aIndex = iHadron1*nStableHadrons+iHadron2;
      double stableYield = stableHadronPairs[aIndex];
      sum += stableYield;
      hPair->SetBinContent(iHadron2P,stableYield);
      hPair->SetBinError(iHadron2P,0.0);
      hNormPair->SetBinContent(iHadron2P,stableYield);
      hNormPair->SetBinError(iHadron2P,0.0);
      }
    if (sum>0.0)
      {
      hPair->SetBinContent(nStableHadrons+2,sum);
      hNormPair->Scale(1.0/sum);
      }
    //    else
    //      {
    //      printValue("iHadron1",iHadron1);
    //      printValue("sum",sum);
    //      }
    }
  if (reportEnd(__FUNCTION__)) { printCR(); }
  }
  
  void ThermalHistos::setLabels(vector<ThermalHadron> & hadrons,
                                vector<ThermalHadron*> & stableHadrons)
  {
  if (reportInfo(__FUNCTION__)) { printCR(); }
  unsigned int nHadrons = hadrons.size();
  unsigned int nStable  = stableHadrons.size();
  printValue("nHadrons",nHadrons);
  printValue("nStable",nStable);
  
  //  for (unsigned int iSpecies=0; iSpecies<nHadrons; iSpecies++)
  //    {
  //    int bin = iSpecies+1;
  //    String label = hadrons[iSpecies].type().title();
  //    h_numberDensityVsIndex  ->GetXaxis()->SetBinLabel(bin,label);
  //    }
  String base = "#rho_{2}(#alpha|";
  String label;
  String title;
  for (unsigned int iSpecies=0; iSpecies<nStable; iSpecies++)
    {
    label = stableHadrons[iSpecies]->type().title();
    title = base + label + ")";
    h_pairDensityVsIndex[iSpecies]->SetTitle(title);
    h_normPairDensityVsIndex[iSpecies]->SetTitle(title);
    }
  
  
  for (unsigned int iSpecies2=0; iSpecies2<nStable; iSpecies2++)
    {
    int binLabel = iSpecies2+1;
    if (iSpecies2==0)
      label = " ";
    else
      label = stableHadrons[iSpecies2]->type().title();

    h_singleDensityVsIndex->GetXaxis()->SetBinLabel(binLabel,label);
    h_singleDensityWFDVsIndex->GetXaxis()->SetBinLabel(binLabel,label);
    h_singleDensityRatioVsIndex->GetXaxis()->SetBinLabel(binLabel,label);
    for (unsigned int iSpecies1=0; iSpecies1<nStable; iSpecies1++)
      {
      //      std::cout << binLabel <<  " LABEL= " << label <<  std::endl;
      h_pairDensityVsIndex[iSpecies1]->GetXaxis()->SetBinLabel(binLabel,label);
      h_normPairDensityVsIndex[iSpecies1]->GetXaxis()->SetBinLabel(binLabel,label);
      h_pairDensityVsIndex[iSpecies1]->GetXaxis()->SetBinLabel(nStable+2,"#Sigma_{#alpha}");
      }
    }
  }
  
  void ThermalHistos::drawMassSpectrum(const ThermalHadronGas & thermalGas)
  {
  gStyle->SetPalette(1,0);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);
  gStyle->SetOptDate(0);
  gStyle->SetOptTitle(0);
  gStyle->SetPadBorderMode(0);
  Double_t w = 4000;
  Double_t h = 4000;
  auto c = new TCanvas("c", "c", w, h);
  c->SetCanvasSize(4000, 4000);
  //c->SetWindowSize(2000, 500);
  double parentMass;
  double daughterMass;
  diag->SetMinimum(0.0);
  diag->SetMaximum(2.2);
  diag->GetXaxis()->SetTickLength(0); // Removes tick marks
  diag->GetXaxis()->SetLabelOffset(999); //
                                         //diag->GetYaxis()->SetTitle("Mass (GeV/c^{2})"); //
  diag->GetYaxis()->SetTitleOffset(1.3); //
  diag->Draw();
  double _position = 0.5;
  const std::vector<ThermalHadron> & hadrons = thermalGas.hadrons();
  
  unsigned int k = 0;
  for (auto & hadron : hadrons)
    {
    _position = double(6*(k-200)) + 2.5;
    parentMass = hadron.type().mass();
    createLine(0.0, parentMass, 333.0, parentMass, 1, 1, 1, true);
    if (hadron.type().isStable() || parentMass>2.0) continue;
    k++;
    if (k<200 | k>250) continue;
    const std::vector<ParticleDecayMode> & decayModes = hadron.type().decayModes();
    
    for (auto & decayMode : decayModes)
      {
      const vector<int> & childrenCodes = decayMode.childrenPdgCodes();
      for (auto & childCode : childrenCodes)
        {
        if (childCode == 22) continue; // dont care about photons
        const ThermalHadron & childHadron = thermalGas.findHadronFor(childCode);
        daughterMass = childHadron.type().mass();
        double massDif = parentMass - daughterMass;
        if (massDif>0.1  &&  daughterMass>0)
          {
          //createArrow(float x1, float y1, float x2, float y2, float arrowSize, Option_t* option, int style, int color, int width, bool doDraw)
          if (massDif<0.5) createArrow(_position, parentMass, _position, daughterMass, 0.004, "|>", 1, 1, 1,true);
          else if (massDif<1.0) createArrow(_position, parentMass, _position, daughterMass, 0.004, "|>", 1, 2, 1,true);
          else if (massDif<1.5) createArrow(_position, parentMass, _position, daughterMass, 0.004, "|>", 2, 4, 1,true);
          else if (massDif<2.0) createArrow(_position, parentMass, _position, daughterMass, 0.004, "|>", 2, 6, 1,true);
          else  createArrow(_position, parentMass, _position, daughterMass, 0.004, "|>", 1, 7, 1,true);
          }
        _position += 1.0;
        }
      }
    }
  c->Print("./MassDecay.pdf");
  }
  
  
} // namespace CAP
