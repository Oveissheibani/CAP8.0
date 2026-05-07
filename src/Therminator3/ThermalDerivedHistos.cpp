#include "ThermalDerivedHistos.hpp"
#include "PrintHelpers.hpp"
#include "NameHelpers.hpp"
#include "RootHistogramHelpers.hpp"
#include "RootFileHelpers.hpp"

ClassImp(CAP::ThermalDerivedHistos);

namespace CAP
{

  ThermalDerivedHistos::ThermalDerivedHistos()
  :
  ThermalHistos()
  {
  appendClassName("ThermalDerivedHistos");
  }

  ThermalDerivedHistos::ThermalDerivedHistos(const ThermalDerivedHistos & source __attribute__((unused)))
  { }

  ThermalDerivedHistos ThermalDerivedHistos::operator=(const ThermalDerivedHistos & rhs __attribute__((unused)))
  {
  return *this;
  }

  void ThermalDerivedHistos::configure(const String & taskName   __attribute__((unused)),
                                       const String & objectType   __attribute__((unused)),
                                       const Configuration & configuration   __attribute__((unused)),
                                       unsigned int index  __attribute__((unused)))
  {   }

  void ThermalDerivedHistos::create()
  {   }

  // Fix me...
  void ThermalDerivedHistos::loadFrom(TFile & inputFile   __attribute__((unused)))
  {   }

  void ThermalDerivedHistos::cloneD(const ThermalDerivedHistos & source  __attribute__((unused)))
  {   }

  void ThermalDerivedHistos::calculateDerivedHistograms(const ThermalHistos & h   __attribute__((unused)))
  {   }

}  // namespace CAP
