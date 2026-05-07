#ifndef CAP_Therminator3
#define CAP_Therminator3
#include "TObject.h"
#include "Model.hpp"
#include "Thermodynamics.hpp"
#include "EventProcessor.hpp"
#include "ThermalHistos.hpp"
#include "ThermalDerivedHistos.hpp"
#include "ThermalHadronGas.hpp"

namespace CAP
{
  class Therminator3
  :
  public EventProcessor
  {
  public:
  
  Therminator3();
  Therminator3(const Therminator3 & task);
  Therminator3 operator=(const Therminator3 & task);
  virtual ~Therminator3(){};
  virtual void setDefaultConfiguration();
  virtual void configure(const Configuration & configuration);
  virtual void initialize();
  virtual void execute();
  virtual void finalize();

  const Model * model() const { return _model; }
  Model * model() { return _model; }
  const ThermalHistos & histograms() const { return _histograms; }
  ThermalHistos & histograms() { return _histograms; }

  protected:

  bool   _createEvents;
  bool   _exportModel;

  String _modelSelected;
  bool   _useHistos;
  bool   _createHistos;
  bool   _importHistos;
  String _importBaseName;
  String _importPath;
  String _importFileName;
  bool   _exportHistos;
  String _exportBaseName;
  String _exportPath;
  String _exportFileName;
  bool   _forceRewrite;
  Model * _model;
  ThermalHistos _histograms;

  ClassDef(Therminator3,0)

  };

} // namespace CAP;

#endif
