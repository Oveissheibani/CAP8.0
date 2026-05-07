#include "ThermalModel.hpp"
#include "PrintHelpers.hpp"
#include "NameHelpers.hpp"

ClassImp(CAP::ThermalModel);

namespace CAP
{

  ThermalModel::ThermalModel()
  :
  EventProcessor(),
  _thermalHadronGas(),
  _histograms()
  {
  appendClassName("ThermalModel");
  setMinimumReportLevel(Object::kInfo);
  setName("ThermalModel");
  setTitle("ThermalModel");
  }

  ThermalModel::ThermalModel(const ThermalModel & source)
  :
  EventProcessor(source),
  _thermalHadronGas(source._thermalHadronGas),
  _histograms(source._histograms)
  {   }

  ThermalModel ThermalModel::operator=(const ThermalModel & rhs)
  {
  if (this!=&rhs)
    {
    EventProcessor::operator=(rhs);
    _thermalHadronGas  = rhs._thermalHadronGas;
    _histograms        = rhs._histograms;
    }
  return *this;
  }


  void ThermalModel::setDefaultConfiguration()
  {
  if (reportDebug(__FUNCTION__)) { printCR(); }
  const String & taskName = EventProcessor::name();
  EventProcessor::setDefaultConfiguration();

  String objectType = "ThermalHadronGas";
  _configuration.addProperty(createKey(taskName,objectType,"temperature"),    0.160);
  _configuration.addProperty(createKey(taskName,objectType,"muBaryon"),    0.0);
  _configuration.addProperty(createKey(taskName,objectType,"muQ"),    0.0);
  _configuration.addProperty(createKey(taskName,objectType,"muS"),    0.0);
  _configuration.addProperty(createKey(taskName,objectType,"muC"),    0.0);
  _configuration.addProperty(createKey(taskName,objectType,"muB"),    0.0);
  _configuration.addProperty(createKey(taskName,objectType,"muT"),    0.0);

  printValue("ThermalHadronGas:temperature",_configuration.valueDouble(createKey(taskName,objectType,"temperature")));
  printValue("ThermalHadronGas:muBaryon",_configuration.valueDouble(createKey(taskName,objectType,"muBaryon")));
  printValue("ThermalHadronGas:muQ",_configuration.valueDouble(createKey(taskName,objectType,"muQ")));
  printValue("ThermalHadronGas:muS",_configuration.valueDouble(createKey(taskName,objectType,"muS")));
  printValue("ThermalHadronGas:muC",_configuration.valueDouble(createKey(taskName,objectType,"muC")));
  printValue("ThermalHadronGas:muB",_configuration.valueDouble(createKey(taskName,objectType,"muB")));
  printValue("ThermalHadronGas:muT",_configuration.valueDouble(createKey(taskName,objectType,"muT")));

  objectType = "HISTOGRAM";
  if (reportInfo(__FUNCTION__))
    {
    printCR();
    printValue("taskName",taskName);
    printValue("objectType",objectType);
    }
  _configuration.addProperty(createKey(taskName,objectType,"USE"),    false);
  _configuration.addProperty(createKey(taskName,objectType,"N"),      0);
  _configuration.addProperty(createKey(taskName,objectType,"IMPORT"), false);
  _configuration.addProperty(createKey(taskName,objectType,"CREATE"), false);
  _configuration.addProperty(createKey(taskName,objectType,"EXPORT"), false);
  _configuration.addProperty(createKey(taskName,objectType,"IMPORT:BASE_NAME"),"NONE");
  _configuration.addProperty(createKey(taskName,objectType,"IMPORT:PATH"),"DEFAULT");
  _configuration.addProperty(createKey(taskName,objectType,"IMPORT:FILE_NAME"),"NONE");
  _configuration.addProperty(createKey(taskName,objectType,"EXPORT:BASE_NAME"),"NONE");
  _configuration.addProperty(createKey(taskName,objectType,"EXPORT:PATH"),"DEFAULT");
  _configuration.addProperty(createKey(taskName,objectType,"EXPORT:FILE_NAME"),"NONE");
  _configuration.addProperty(createKey(taskName,objectType,"EXPORT:FORCE_REWRITE"),true);
  _configuration.addProperty(createKey(taskName,objectType,"index_fill"),    true);
  _configuration.addProperty(createKey(taskName,objectType,"index_nbins"),   200);
  _configuration.addProperty(createKey(taskName,objectType,"index_min"),     0.0);
  _configuration.addProperty(createKey(taskName,objectType,"index_max"),     250.0);
  _configuration.addProperty(createKey(taskName,objectType,"mass_fill"),     true);
  _configuration.addProperty(createKey(taskName,objectType,"mass_nbins"),    10);
  _configuration.addProperty(createKey(taskName,objectType,"mass_min"),      0.0);
  _configuration.addProperty(createKey(taskName,objectType,"mass_max"),      4.0);

  printValue("HISTOGRAM:index_fill",_configuration.valueBool(createKey(taskName,objectType,"index_fill")));
  printValue("HISTOGRAM:index_nbins",_configuration.valueInt(createKey(taskName,objectType,"index_nbins")));
  printValue("HISTOGRAM:index_min",_configuration.valueDouble(createKey(taskName,objectType,"index_min")));
  printValue("HISTOGRAM:index_max",_configuration.valueDouble(createKey(taskName,objectType,"index_max")));
  printValue("HISTOGRAM:mass_fill",_configuration.valueBool(createKey(taskName,objectType,"mass_fill")));
  printValue("HISTOGRAM:mass_nbins",_configuration.valueInt(createKey(taskName,objectType,"mass_nbins")));
  printValue("HISTOGRAM:mass_min",_configuration.valueDouble(createKey(taskName,objectType,"mmass_min")));
  printValue("HISTOGRAM:mass_max",_configuration.valueDouble(createKey(taskName,objectType,"mmass_max")));
  }

  void ThermalModel::execute()
  {
  _taskExecuted.increment();
  _thermalHadronGas.calculateAllProperties();
  //_thermalHadronGas.print();
  std::vector<ThermalHadron>  & hadrons = _thermalHadronGas.hadrons();
  std::vector<ThermalHadron*> & stableHadrons = _thermalHadronGas.stableHadrons();
  _histograms.fill(_thermalHadronGas);
  _histograms.setLabels(hadrons,stableHadrons);
 // _histograms.drawMassSpectrum(*_thermalHadronGas);
  }

  //void ThermalModel::configureObjects(const Configuration & configuration)
  void ThermalModel::configure(const Configuration & configuration)
  {
  EventProcessor::configure(configuration);
  String taskName = name();
  String typeName = "HISTOGRAM";
  String importPathA;
  String exportPathA;
  useHistos      = _configuration.valueBool(  createKey(taskName,typeName,"USE"));
  createHistos   = _configuration.valueBool(  createKey(taskName,typeName,"CREATE"));
  importHistos   = _configuration.valueBool(  createKey(taskName,typeName,"IMPORT"));
  importBaseName = _configuration.valueString(createKey(taskName,typeName,"IMPORT:BASE_NAME"));
  importPathA    = _configuration.valueString(createKey(taskName,typeName,"IMPORT:PATH"));
  exportHistos   = _configuration.valueBool(  createKey(taskName,typeName,"EXPORT"));
  exportBaseName = _configuration.valueString(createKey(taskName,typeName,"EXPORT:BASE_NAME"));
  exportPathA    = _configuration.valueString(createKey(taskName,typeName,"EXPORT:PATH"));
  forceRewrite   = _configuration.valueBool(  createKey(taskName,typeName,"EXPORT:FORCE_REWRITE"));

  EnvironmentVariables * envVariables = EnvironmentVariables::environmentVariables();
  String basePath;

  if (importPathA.EqualTo("DEFAULT"))
    {
    importPath = envVariables->variable("CAP_HISTOS_IMPORT_PATH");
    }
  else if (importPathA.BeginsWith("/") || exportPathA.BeginsWith("./"))
    {
    importPath = importPathA;
    }
  else
    {
    importPath = envVariables->variable("CAP_HISTOS_IMPORT_PATH");
    importPath += "/";
    importPath += importPathA;
    }
  if (exportPathA.EqualTo("DEFAULT"))
    {
    exportPath = envVariables->variable("CAP_HISTOS_EXPORT_PATH");
    }
  else if (exportPathA.BeginsWith("/") || exportPathA.BeginsWith("./"))
    {
    exportPath = exportPathA;
    }
  else
    {
    exportPath = envVariables->variable("CAP_HISTOS_IMPORT_PATH");
    exportPath += "/";
    exportPath += exportPathA;
    }

  String _typeName = "ThermalHadronGas";

  _thermalHadronGas.configure(taskName,_typeName,configuration);
  _histograms.configure(taskName,"HISTOGRAM",configuration,0);

  }

  void ThermalModel::initialize()
  {
  if (reportDebug(__FUNCTION__)) { printCR(); }
  EventProcessor::initialize();
  _thermalHadronGas.reset();
  _thermalHadronGas.loadHadrons(db());
  _histograms.create();
   }


  void ThermalModel::finalize()
  {
  if (reportDebug(__FUNCTION__)) { printCR(); }
  EventProcessor::finalize();
  if (exportHistos)
    {
    int temperature = 1000* _thermalHadronGas.temperature();
    exportFileName = exportBaseName + temperature;
    TFile * outputFile;
    if (forceRewrite)
      outputFile = openRecreateRootFile(exportPath,exportFileName,1);
    else
      outputFile = openNewRootFile(exportPath,exportFileName,1);
    _histograms.saveTo(*outputFile);
    outputFile->Close();
    }
  //if (_thermalHadronGas) delete _thermalHadronGas;
  }

} // namespace CAP;
