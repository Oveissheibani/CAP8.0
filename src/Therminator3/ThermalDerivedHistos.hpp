#ifndef CAP_ThermalDerivedHistos
#define CAP_ThermalDerivedHistos
#include "ThermalHistos.hpp"

namespace CAP
{

  class ThermalDerivedHistos : public ThermalHistos
  {
  public:

  ThermalDerivedHistos();
  ThermalDerivedHistos(const ThermalDerivedHistos & source);
  ThermalDerivedHistos operator=(const ThermalDerivedHistos & rhs);
  virtual ~ThermalDerivedHistos() {}
  virtual void configure(const String & taskName,
                         const String & objectType,
                         const Configuration & configuration,
                         unsigned int index);
  virtual void create();
  virtual void loadFrom(TFile & inputFile);

  virtual void calculateDerivedHistograms(const ThermalHistos & h);
  
  protected:

  virtual void cloneD(const ThermalDerivedHistos & source);

  ClassDef(ThermalDerivedHistos,0)

  };

} // namespace CAP

#endif /* CAP_thermalHadronGasVsTempHistograms  */



