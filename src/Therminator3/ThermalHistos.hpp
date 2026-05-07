// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/
#ifndef CAP_ThermalHistos
#define CAP_ThermalHistos
#include "HistogramGroup.hpp"
#include "ThermalHadronGasFD.hpp"

namespace CAP
{
  
  
  //!
  //!Histograming class designed to ampke plots of the  properties of the  hadron gas (HG)  particle species partial quantities as
  //!a function of species index or particle _mass.
  //!Quantities of interest for each of the (therampl) particle species included in the HG calculation include
  //!- number density
  //!- _entropy density
  //!- partial _pressure
  //!Quantities of interest for each of the  stable particle species included in the HG calculation include
  //!- number density
  //!
  //!
  class ThermalHistos
  :
  public HistogramGroup
  {
  public:
  
  ThermalHistos();
  ThermalHistos(const ThermalHistos & source);
  ThermalHistos operator=(const ThermalHistos & rhe);
  virtual ~ThermalHistos() {}
  virtual void configure(const String & taskName,
                         const String & objectType,
                         const Configuration & configuration,
                         unsigned int index);
  virtual void create();
  virtual void loadFrom(TFile & inputFile);
  virtual void fill(const ThermalHadronGasFD & hadronGas);
  //virtual void fillRho1VsP(ThermalHadronGas & hadronGas, double _volume);
  
  void setLabels(vector<ThermalHadron> & hadrons,
                 vector<ThermalHadron*> & stableHadrons);
  
  void drawMassSpectrum(const ThermalHadronGas & thermalGas);
  
  protected:
  
  virtual void cloneB(const ThermalHistos & source);
  
  TH1 * h_massVsIndex;
  TH1 * h_numberDensityVsIndex;
  TH1 * h_energyDensityVsIndex;
  TH1 * h_entropyDensityVsIndex;
  TH1 * h_pressureVsIndex;
  TH1 * h_netBaryonDensityVsIndex;
  TH1 * h_netChargeDensityVsIndex;
  TH1 * h_netStrangeDensityVsIndex;
  TH1 * h_charmDensityVsIndex;
  TH1 * h_bottomDensityVsIndex;
  TH1 * h_topDensityVsIndex;
  TH1 * h_densityWFDVsIndex;
  
  // stable particles only
  TH1 * diag;
  TH1 * h_singleDensityVsIndex;
  TH1 * h_singleDensityWFDVsIndex;
  TH1 * h_singleDensityRatioVsIndex;
  vector<TH1*> h_pairDensityVsIndex;
  vector<TH1*> h_normPairDensityVsIndex;
  
  
  TH1 * h_relNumberDensityVsIndex;
  //  TH1 * h_relEnergyDensityVsIndex;
  //  TH1 * h_relEntropyDensityVsIndex;
  //  TH1 * h_relPressureVsIndex;
  
  // stable only
  TH1 * h_relDensityWFDVsIndex;
  //  TH1 * h_ratioDensityWtoWOFDVsIndex;
  //  TH1 * h_ratioDensityWtoWOFDVsMass;
  
  TProfile * h_numberDensityVsMass;
  TProfile * h_energyDensityVsMass;
  TProfile * h_entropyDensityVsMass;
  TProfile * h_pressureVsMass;
  TProfile * h_netBaryonDensityVsMass;
  TProfile * h_netChargeDensityVsMass;
  TProfile * h_netStrangeDensityVsMass;
  TProfile * h_charmDensityVsMass;
  TProfile * h_bottomDensityVsMass;
  TProfile * h_topDensityVsMass;
  TProfile * h_densityWFDVsMass;
  
  TProfile * h_relNumberDensityVsMass;
  TProfile * h_relEnergyDensityVsMass;
  TProfile * h_relEntropyDensityVsMass;
  TProfile * h_relPressureVsMass;
  TProfile * h_relDensityWFDVsMass;
  TProfile * h_relNetBaryonDensityVsMass;
  TProfile * h_relNetChargeDensityVsMass;
  TProfile * h_relNetStrangeDensityVsMass;
  TProfile * h_relcharmDensityVsMass;
  TProfile * h_relbottomDensityVsMass;
  TProfile * h_reltopDensityVsMass;
  
  
  bool    index_fill;
  int     index_nbins;
  double  index_min;
  double  index_max;
  bool    mass_fill;
  int     mass_nbins;
  double  mass_min;
  double  mass_max;
  
  ClassDef(ThermalHistos,0)
  
  };
  
} // namespace CAP

#endif // !CAP_ThermalHistos



