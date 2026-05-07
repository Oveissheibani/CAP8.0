#include "Therminator3.hpp"
#include "PrintHelpers.hpp"
#include "ModelHadronGas.hpp"
#include "ModelBlastWave.hpp"
#include "ModelBWA.hpp"
#include "ModelHydro2D.hpp"
#include "ModelHydro3D.hpp"
#include "ModelKrakowSFO.hpp"

ClassImp(CAP::Therminator3);

namespace CAP
{

  Therminator3::Therminator3()
  :
  EventProcessor(),
  _modelSelected("BW"),
  _useHistos(true),
  _createHistos(true),
  _importHistos(true),
  _importBaseName(""),
  _importPath(""),
  _importFileName(""),
  _exportHistos(true),
  _exportBaseName(""),
  _exportPath(""),
  _exportFileName("Therminator3"),
  _forceRewrite(true),
  _model(nullptr),
  _histograms()
  {
  appendClassName("Therminator3");
  setMinimumReportLevel(Object::kInfo);
  setName("Therminator3");
  setTitle("Therminator3");
  }

  Therminator3::Therminator3(const Therminator3 & source)
  :
  EventProcessor(source),
  _modelSelected(source._modelSelected),
  _useHistos(source._useHistos),
  _createHistos(source._createHistos),
  _importHistos(source._importHistos),
  _importBaseName(source._importBaseName),
  _importPath(source._importPath),
  _importFileName(source._importFileName),
  _exportHistos(source._exportHistos),
  _exportBaseName(source._exportBaseName),
  _exportPath(source._exportPath),
  _exportFileName(source._exportFileName),
  _forceRewrite(source._forceRewrite),
  _model(source._model),
  _histograms(source._histograms)
  {   }

  Therminator3 Therminator3::operator=(const Therminator3 & rhs)
  {
  if (this!=&rhs)
    {
    EventProcessor::operator=(rhs);
    _modelSelected  = rhs._modelSelected;
    _useHistos      = rhs._useHistos;
    _createHistos   = rhs._createHistos;
    _importHistos   = rhs._importHistos;
    _importBaseName = rhs._importBaseName;
    _importPath     = rhs._importPath;
    _importFileName = rhs._importFileName;
    _exportHistos   = rhs._exportHistos;
    _exportBaseName = rhs._exportBaseName;
    _exportPath     = rhs._exportPath;
    _exportFileName = rhs._exportFileName;
    _forceRewrite   = rhs._forceRewrite;
    _model          = rhs._model;
    _histograms     = rhs._histograms;
    }
  return *this;
  }

  void Therminator3::setDefaultConfiguration()
  {
  if (reportDebug(__FUNCTION__)) { printCR(); }
  const String & taskName = EventProcessor::name();
  EventProcessor::setDefaultConfiguration();

  String objectType = "HYDRO_MODEL";
  _configuration.addProperty(createKey(taskName,objectType,"TYPE"), "BW");
  _configuration.addProperty(createKey(taskName,objectType,"MULT:IMPORT"), false);
  _configuration.addProperty(createKey(taskName,objectType,"MULT:CREATE"), true);
  _configuration.addProperty(createKey(taskName,objectType,"MULT:EXPORT"), true);
  _configuration.addProperty(createKey(taskName,objectType,"MULT:IMPORT:BASE_NAME"),"NONE");
  _configuration.addProperty(createKey(taskName,objectType,"MULT:IMPORT:PATH"),"DEFAULT");
  _configuration.addProperty(createKey(taskName,objectType,"MULT:IMPORT:FILE_NAME"),"NONE");
  _configuration.addProperty(createKey(taskName,objectType,"MULT:EXPORT:BASE_NAME"),"NONE");
  _configuration.addProperty(createKey(taskName,objectType,"MULT:EXPORT:PATH"),"DEFAULT");
  _configuration.addProperty(createKey(taskName,objectType,"MULT:EXPORT:FILE_NAME"),"NONE");
  _configuration.addProperty(createKey(taskName,objectType,"MULT:EXPORT:FORCE_REWRITE"),true);

  _configuration.addProperty(createKey(taskName,objectType,"HYPERS:IMPORT"), false);
  _configuration.addProperty(createKey(taskName,objectType,"HYPERS:CREATE"), false);
  _configuration.addProperty(createKey(taskName,objectType,"HYPERS:EXPORT"), false);
  _configuration.addProperty(createKey(taskName,objectType,"HYPERS:IMPORT:BASE_NAME"),"NONE");
  _configuration.addProperty(createKey(taskName,objectType,"HYPERS:IMPORT:PATH"),"DEFAULT");
  _configuration.addProperty(createKey(taskName,objectType,"HYPERS:IMPORT:FILE_NAME"),"NONE");
  _configuration.addProperty(createKey(taskName,objectType,"HYPERS:EXPORT:BASE_NAME"),"NONE");
  _configuration.addProperty(createKey(taskName,objectType,"HYPERS:EXPORT:PATH"),"DEFAULT");
  _configuration.addProperty(createKey(taskName,objectType,"HYPERS:EXPORT:FILE_NAME"),"NONE");
  _configuration.addProperty(createKey(taskName,objectType,"HYPERS:EXPORT:FORCE_REWRITE"),true);

  // parameters for event generation are handled in the base class.

  objectType = "HADRON_GAS";
  _configuration.addProperty(createKey(taskName,objectType,"temperature"), 0.160);
  _configuration.addProperty(createKey(taskName,objectType,"muBaryon"),    0.0);
  _configuration.addProperty(createKey(taskName,objectType,"muQ"),         0.0);
  _configuration.addProperty(createKey(taskName,objectType,"muS"),         0.0);
  _configuration.addProperty(createKey(taskName,objectType,"muC"),         0.0);
  _configuration.addProperty(createKey(taskName,objectType,"muB"),         0.0);
  _configuration.addProperty(createKey(taskName,objectType,"muT"),         0.0);
  _configuration.addProperty(createKey(taskName,objectType,"IMPORT"),          false);
  _configuration.addProperty(createKey(taskName,objectType,"CREATE"),          true);
  _configuration.addProperty(createKey(taskName,objectType,"EXPORT"),          false);
  _configuration.addProperty(createKey(taskName,objectType,"IMPORT:PATH"),     "DEFAULT");
  _configuration.addProperty(createKey(taskName,objectType,"IMPORT:BASE_NAME"),"NONE");
  _configuration.addProperty(createKey(taskName,objectType,"IMPORT:FILE_NAME"),"NONE");
  _configuration.addProperty(createKey(taskName,objectType,"EXPORT:PATH"),     "DEFAULT");
  _configuration.addProperty(createKey(taskName,objectType,"EXPORT:BASE_NAME"),"NONE");
  _configuration.addProperty(createKey(taskName,objectType,"EXPORT:FILE_NAME"),"NONE");
  _configuration.addProperty(createKey(taskName,objectType,"EXPORT:FORCE_REWRITE"),true);

  objectType = "HISTOGRAM";
  if (reportInfo(__FUNCTION__))
    {
    printCR();
    printValue("taskName",taskName);
    printValue("objectType",objectType);
    }
  _configuration.addProperty(createKey(taskName,objectType,"USE"),             false);
  _configuration.addProperty(createKey(taskName,objectType,"N"),               0);
  _configuration.addProperty(createKey(taskName,objectType,"IMPORT"),          false);
  _configuration.addProperty(createKey(taskName,objectType,"CREATE"),          false);
  _configuration.addProperty(createKey(taskName,objectType,"EXPORT"),          false);
  _configuration.addProperty(createKey(taskName,objectType,"IMPORT:BASE_NAME"),"NONE");
  _configuration.addProperty(createKey(taskName,objectType,"IMPORT:PATH"),     "DEFAULT");
  _configuration.addProperty(createKey(taskName,objectType,"IMPORT:FILE_NAME"),"NONE");
  _configuration.addProperty(createKey(taskName,objectType,"EXPORT:BASE_NAME"),"NONE");
  _configuration.addProperty(createKey(taskName,objectType,"EXPORT:PATH"),     "DEFAULT");
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

  void Therminator3::configure(const Configuration & configuration)
  {
  EventProcessor::configure(configuration);
  String taskName = name();
  String typeName = "HYDRO_MODEL";

  _modelSelected = configuration.valueString(createKey(taskName,typeName,"TYPE"));
  if (_modelSelected.EqualTo("HG")) // Hadron Gas W/O flow
    _model = new ModelHadronGas();
  else if (_modelSelected.EqualTo("BW")) // Basic Blastwave
    _model = new ModelBlastWave();
  else if (_modelSelected.EqualTo("BWA")) // Basic Blastwave Alternate
    _model = new ModelBWA();
  else if (_modelSelected.EqualTo("HYDRO2D")) // Hydro2D
    _model = new ModelHydro2D();
  else if (_modelSelected.EqualTo("HYDRO3D")) // Hydro3D
    _model = new ModelHydro3D();
  else if (_modelSelected.EqualTo("SFO")) // Hydro SFO
    _model = new ModelKrakowSFO();
  else
    throw Exception("Unknown model selected",__FUNCTION__);
  _model->configure(taskName,typeName,configuration);
  typeName = "HISTOGRAM";
  String importPathA;
  String exportPathA;
  _useHistos      = configuration.valueBool(  createKey(taskName,typeName,"USE"));
  _createHistos   = configuration.valueBool(  createKey(taskName,typeName,"CREATE"));
  _importHistos   = configuration.valueBool(  createKey(taskName,typeName,"IMPORT"));
  _importBaseName = configuration.valueString(createKey(taskName,typeName,"IMPORT:BASE_NAME"));
  importPathA    = configuration.valueString(createKey(taskName,typeName,"IMPORT:PATH"));
  _exportHistos   = configuration.valueBool(  createKey(taskName,typeName,"EXPORT"));
  _exportBaseName = configuration.valueString(createKey(taskName,typeName,"EXPORT:BASE_NAME"));
  exportPathA    = configuration.valueString(createKey(taskName,typeName,"EXPORT:PATH"));
  _forceRewrite   = configuration.valueBool(  createKey(taskName,typeName,"EXPORT:FORCE_REWRITE"));
  EnvironmentVariables * envVariables = EnvironmentVariables::environmentVariables();
  String basePath;

  if (importPathA.EqualTo("DEFAULT"))
    {
    _importPath = envVariables->variable("CAP_HISTOS_IMPORT_PATH");
    }
  else if (importPathA.BeginsWith("/") || exportPathA.BeginsWith("./"))
    {
    _importPath = importPathA;
    }
  else
    {
    _importPath = envVariables->variable("CAP_HISTOS_IMPORT_PATH");
    _importPath += "/";
    _importPath += importPathA;
    }
  if (exportPathA.EqualTo("DEFAULT"))
    {
    _exportPath = envVariables->variable("CAP_HISTOS_EXPORT_PATH");
    }
  else if (exportPathA.BeginsWith("/") || exportPathA.BeginsWith("./"))
    {
    _exportPath = exportPathA;
    }
  else
    {
    _exportPath = envVariables->variable("CAP_HISTOS_IMPORT_PATH");
    _exportPath += "/";
    _exportPath += exportPathA;
    }
  _histograms.configure(taskName,"HISTOGRAM",configuration,0);
  }


  void Therminator3::initialize()
  {
  if (reportDebug(__FUNCTION__)) { printCR(); }
  EventProcessor::initialize();
  _histograms.create();
  }

  void Therminator3::execute()
  {
  _taskExecuted.increment();

  if (_createEvents) // fix me
    {
    //Event & theEvent = event();
    //const std::vector<ParticleFilter*> & pFs = particleFilters();
    Particle::factory().reset();
    //ParticleDb & particleDb = db();
    //std::vector<ParticleType*> & _particleTypes = particleDb._particleTypes();
    //_model->generateEvent(event,_particleTypes,particleFilters); // each _model has its own event
    }
  }

  void Therminator3::finalize()
  {
  if (reportDebug(__FUNCTION__)) { printCR(); }
  if (_model) delete _model;
  EventProcessor::finalize();

  if (_exportModel)
    {
    int temp = 1000*_model->temperature();
    _exportFileName = _exportBaseName + temp;
    std::ofstream & outputFile = openOutputAsciiFile(_exportPath,_exportFileName,1);
    _model->saveToAscii(outputFile);
    outputFile.close();
    }

  if (_exportHistos)
    {
    int temp = 1000*_model->temperature();
    _exportFileName = _exportBaseName + temp;
    TFile * outputFile;
    if (_forceRewrite)
      outputFile = openRecreateRootFile(_exportPath,_exportFileName,1);
    else
      outputFile = openNewRootFile(_exportPath,_exportFileName,1);
    _histograms.saveTo(*outputFile);
    outputFile->Close();
    }
  }


} // namespace CAP;
