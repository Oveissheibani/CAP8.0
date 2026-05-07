#ifndef CAP_ThermalModel
#define CAP_ThermalModel
#include "TObject.h"
#include "EventProcessor.hpp"
#include "ThermalHistos.hpp"
#include "ThermalHadronGasFD.hpp"

namespace CAP
{
  class ThermalModel
  :
  public EventProcessor
  {
  public:

  ThermalModel();
  ThermalModel(const ThermalModel & task);
  ThermalModel operator=(const ThermalModel & task);
  virtual ~ThermalModel(){};
  virtual void setDefaultConfiguration();
  virtual void configure(const Configuration & configuration);
  virtual void initialize();
  virtual void execute();
  virtual void finalize();

  protected:

  bool   useHistos;
  bool   createHistos;
  bool   importHistos;
  String importBaseName;
  String importPath;
  String importFileName;
  bool   exportHistos;
  String exportBaseName;
  String exportPath;
  String exportFileName;
  bool   forceRewrite;

  ThermalHadronGasFD _thermalHadronGas;
  ThermalHistos _histograms;

  ClassDef(ThermalModel,0)

  };

} // namespace CAP;

#endif // !CAP_ThermalModel
