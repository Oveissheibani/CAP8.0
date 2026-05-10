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
#ifndef CAP__ManagedObjects
#define CAP__ManagedObjects
#include "Aliases.hpp"
#include "Exceptions.hpp"
#include "Configuration.hpp"
#include "PrintHelpers.hpp"
#include "NameHelpers.hpp"
#include "StreamHelpers.hpp"
#include "RootFileHelpers.hpp"
#include "Object.hpp"
#include "EnvironmentVariables.hpp"
#include "Object.hpp"
#include <cstdio>      // exportObject diagnostic tracing
#include <exception>
#include "ManagedObject.hpp"
#include "TFile.h"
#include <iostream>

namespace CAP
{
  
  template <class MANAGED>
  class ManagedObjects
  :
  public Object
  {
  protected:
  
  bool   _owner;
  bool   _use;
  bool   _import;
  bool   _create;
  bool   _export;
  bool   _scale;
  bool   _forceRewrite;
  String _importBaseName;
  String _importPath;
  String _importFileName;
  String _exportBaseName;
  String _exportPath;
  String _exportFileName;
  String _taskName;
  String _ownerName;
  String _typeName;
  std::vector<MANAGED*> _objects;
  TFile * _inputRootFile;
  TFile * _outputRootFile;
  
  static std::vector<MANAGED*> * objectStore;
  
  public:
  
  ManagedObjects()
  :
  _owner(false),
  _use(false),
  _import(false),
  _create(false),
  _export(false),
  _scale(false),
  _forceRewrite(true),
  _importBaseName("BASE"),
  _importPath("DEFAULT"),
  _importFileName("NONE"),
  _exportBaseName("BASE"),
  _exportPath("DEFAULT"),
  _exportFileName("NONE"),
  _taskName("UNKNOWN"),
  _ownerName("UNKNOWN"),
  _typeName("UNKNOWN"),
  _objects(),
  _inputRootFile(nullptr),
  _outputRootFile(nullptr)
  {
  setClassName("ManagedObjects<MANAGED>");
  setName("ManagedObjects<MANAGED>");
  setMinimumReportLevel(Object::kInfo);
  }
  
  //  ManagedObjects(const ManagedObjects & manager)
  //  ManagedObjects & operator=(const ManagedObjects & manager);
  virtual ~ManagedObjects()
  {
  clear();
  }
  
  void setDefaultObjectConfiguration(const String & taskName,
                                     const String & typeName,
                                     Configuration & configuration)
  {
  if (reportStart(__FUNCTION__)) { printCR(); }
  _taskName = taskName;
  _typeName = typeName;
  configuration.addProperty(createKey(_taskName,_typeName,"USE"),    false);
  configuration.addProperty(createKey(_taskName,_typeName,"N"),      0);
  configuration.addProperty(createKey(_taskName,_typeName,"IMPORT"), false);
  configuration.addProperty(createKey(_taskName,_typeName,"CREATE"), false);
  configuration.addProperty(createKey(_taskName,_typeName,"EXPORT"), false);
  configuration.addProperty(createKey(_taskName,_typeName,"IMPORT:BASE_NAME"),"NONE");
  configuration.addProperty(createKey(_taskName,_typeName,"IMPORT:PATH"),"DEFAULT");
  configuration.addProperty(createKey(_taskName,_typeName,"IMPORT:FILE_NAME"),"NONE");
  configuration.addProperty(createKey(_taskName,_typeName,"EXPORT:BASE_NAME"),"NONE");
  configuration.addProperty(createKey(_taskName,_typeName,"EXPORT:PATH"),"DEFAULT");
  configuration.addProperty(createKey(_taskName,_typeName,"EXPORT:FILE_NAME"),"NONE");
  configuration.addProperty(createKey(_taskName,_typeName,"EXPORT:FORCE_REWRITE"),true);
  }
  
  
  virtual void setOwner(bool v)  { _owner = v; }
  virtual void setImport(bool option) { _import = option; }
  virtual void setCreate(bool option) { _create = option; }
  virtual void setExport(bool option) { _export = option; }
  virtual void setForceRewrite(bool option) { _forceRewrite = option; }
  virtual bool isOwner() const   { return _owner; }
  virtual bool imports() const { return _import; }
  virtual bool create() const { return _create; }
  virtual bool exports() const { return _export; }
  virtual bool forceRewrite() const { return _forceRewrite; }
  virtual void setImportBaseName(const String & option) { _importBaseName = option;}
  virtual void setImportPath(const String & option)     { _importPath = option;}
  virtual void setImportFileName(const String & option) { _importFileName = option;}
  virtual void setExportBaseName(const String & option) { _exportBaseName = option;}
  virtual void setExportPath(const String & option)     { _exportPath = option;}
  virtual void setExportFileName(const String & option) { _exportFileName = option;}
  virtual void setParentTaskName(const String & option) { _taskName = option;}
  virtual void setOwnerName(const String & option)      { _ownerName = option;}
  virtual void setTypeName(const String & option)       { _typeName  = option;}
  
  
  
  virtual const String & importBaseName() const { return _importBaseName;}
  virtual const String & importPath() const     { return _importPath;}
  virtual const String & importFileName() const { return _importFileName;}
  virtual const String & getExportBaseName() const { return _exportBaseName;}
  virtual const String & getExportPath() const     { return _exportPath;}
  virtual const String & getExportFileName() const { return _exportFileName;}
  virtual const String & getOwnerName() const      { return _ownerName; }
  virtual const String & taskName() const       { return _taskName; }
  virtual const String & typeName() const       { return _typeName; }
  
  virtual MANAGED* instantiateObject(const String & objectName)
  {
  if (storeContainsObjectNamed(objectName))
    throw InvalidNameException(objectName,"Name already found in object store",__FUNCTION__);
  MANAGED * object = new MANAGED();
  object->setName(objectName);
  appendToStore(object);
  append(object);
  return object;
  }
  
  virtual MANAGED* useObject(const  String & objectName)
  {
  if (!storeContainsObjectNamed(objectName))
    throw InvalidNameException(objectName,"Name NOT found in object store",__FUNCTION__);
  MANAGED * object = getStoreObjectNamed(objectName);
  append(object);
  return object;
  }
  
  virtual std::vector<MANAGED*> & getObjects()
  {
  return _objects;
  }
  
  virtual const std::vector<MANAGED*> & getObjects() const
  {
  return _objects;
  }
  
  virtual void append(MANAGED * object)
  {
  _objects.push_back(object);
  //  if (!storeContainsObjectNamed(object->name()))
  //    appendToStore(object);
  }
  
  virtual bool containsObjectNamed(const String & name)
  {
  for (auto object : _objects) if (object->isNamed(name)) return true;
  return false;
  }
  
  virtual MANAGED * objectAt(unsigned int index)
  {
  if (index>=_objects.size()) throw IndexOutOfBoundException(index,_objects.size(),"Index out of bound",__FUNCTION__);
  return _objects[index];
  }
  
  virtual const MANAGED * objectAt(unsigned int index) const
  {
  if (index>=_objects.size()) throw IndexOutOfBoundException(index,_objects.size(),"Index out of bound",__FUNCTION__);
  return _objects[index];
  }
  
  virtual MANAGED * getObjectNamed(const String & name)
  {
  for (auto object : _objects) if (object->isNamed(name)) return object;
  throw InvalidNameException(name,"Name not found in _objects",__FUNCTION__);
  }
  
  virtual unsigned int size() const
  {
  return _objects.size();
  }
  
  virtual unsigned int size()
  {
  return _objects.size();
  }
  
  

  
  //!
  //!Returns a pointer to a store of type std::vector<MANAGED*>
  //!
  static std::vector<MANAGED*> & getObjectStore()
  {
  if (!objectStore)
    {
    objectStore = new std::vector<MANAGED*>();
    }
  return *objectStore;
  }
  
  virtual void appendToStore(MANAGED * object)
  {
  getObjectStore().push_back(object);
  }
  
  virtual bool storeContainsObjectNamed(const String & name)
  {
  for (auto object : getObjectStore()) if (object->isNamed(name)) return true;
  return false;
  }
  
  static MANAGED * getStoreObjectAt(unsigned int index)
  {
  return getObjectStore().objectAt(index);
  }
  
  static MANAGED * getStoreObjectNamed(const String & name)
  {
  for (auto & object : getObjectStore()) if (object->isNamed(name)) return object;
  throw InvalidNameException(name,"Name not found in _objects",__FUNCTION__);
  }
  
  static unsigned int objectStoreSize()
  {
  return getObjectStore().size();
  }
  
  static void clearStore()
  {
  for (auto object : getObjectStore()) delete object;
  delete objectStore;
  }
  
  
  virtual void configureObjects(const Configuration & configuration)
  {
  //if (reportDebug(__FUNCTION__)) { printCR(); }
  String name;
  String title;
  bool   owner;
  String importPath;
  String exportPath;
  unsigned int nObjects;
  _use            = configuration.valueBool(  createKey(_taskName,_typeName,"USE"));
  _create         = configuration.valueBool(  createKey(_taskName,_typeName,"CREATE"));
  _import         = configuration.valueBool(  createKey(_taskName,_typeName,"IMPORT"));
  _importBaseName = configuration.valueString(createKey(_taskName,_typeName,"IMPORT:BASE_NAME"));
  importPath      = configuration.valueString(createKey(_taskName,_typeName,"IMPORT:PATH"));
  _importFileName = configuration.valueString(createKey(_taskName,_typeName,"IMPORT:FILE_NAME"));
  _export         = configuration.valueBool(  createKey(_taskName,_typeName,"EXPORT"));
  _exportBaseName = configuration.valueString(createKey(_taskName,_typeName,"EXPORT:BASE_NAME"));
  exportPath      = configuration.valueString(createKey(_taskName,_typeName,"EXPORT:PATH"));
  _exportFileName = configuration.valueString(createKey(_taskName,_typeName,"EXPORT:FILE_NAME"));
  _forceRewrite   = configuration.valueBool(  createKey(_taskName,_typeName,"EXPORT:FORCE_REWRITE"));
  nObjects        = configuration.valueInt(   createKey(_taskName,_typeName,"N") );

  // ----------------------------------------------------------------------
  //  Backward-compatibility shim for legacy .ini files
  // ----------------------------------------------------------------------
  //  Older .ini files (the canonical projects/*.ini and everything emitted
  //  by analyses/builder/build-ini-gui) use the form
  //      <task>:nParticleDbs            instead of  <task>:PARTICLE_DB:N
  //      <task>:ParticleDbName<k>       instead of  <task>:PARTICLE_DB:<k>:NAME
  //      <task>:ParticleDbOwner<k>      instead of  <task>:PARTICLE_DB:<k>:OWNER
  //      <task>:nEventFilters           instead of  <task>:EVENT_FILTER:N
  //      <task>:EventFilterName<k>      instead of  <task>:EVENT_FILTER:<k>:NAME
  //      ...
  //  Map _typeName to the corresponding legacy tokens. We need two of them:
  //   * legacyN     — full word used in the count key  ("n<word>s")
  //   * legacyEntry — root used in per-object keys     "<root>Name<k>" etc.
  //  Most types use the same word for both, but EVENT is asymmetric:
  //  the count is `nEventsStreams` while the per-object key is `StreamName<k>`.
  TString legacyN;       // count token, used as "n<token>s"
  TString legacyEntry;   // per-object token, used as "<token>Name<k>" / "<token>Owner<k>"
  if      (_typeName == "PARTICLE_DB")
        { legacyN = "ParticleDb";          legacyEntry = "ParticleDb"; }
  else if (_typeName == "EVENT_FILTER")
        { legacyN = "EventFilter";         legacyEntry = "EventFilter"; }
  else if (_typeName == "PARTICLE_FILTER")
        { legacyN = "ParticleFilter";      legacyEntry = "ParticleFilter"; }
  else if (_typeName == "JET_FILTER")
        { legacyN = "JetFilter";           legacyEntry = "JetFilter"; }
  else if (_typeName == "EVENT")
        { legacyN = "EventsStream";        legacyEntry = "Stream"; }   // ← asymmetric
  else if (_typeName == "EVENT_STREAM")
        { legacyN = "EventsStream";        legacyEntry = "Stream"; }
  else if (_typeName == "EVENT_EFFICIENCY")
        { legacyN = "EventEfficiency";     legacyEntry = "EventEfficiency"; }
  else if (_typeName == "PARTICLE_EFFICIENCY")
        { legacyN = "ParticleEfficiency";  legacyEntry = "ParticleEfficiency"; }
  else if (_typeName == "EVENT_SMEARER")
        { legacyN = "EventSmearer";        legacyEntry = "EventSmearer"; }
  else if (_typeName == "PARTICLE_SMEARER")
        { legacyN = "ParticleSmearer";     legacyEntry = "ParticleSmearer"; }

  if (nObjects == 0 && legacyN.Length() > 0)
    {
    String key = _taskName;
    key += ":n";
    key += legacyN;
    key += "s";
    nObjects = configuration.valueInt(key);
    }

  EnvironmentVariables * envVariables = EnvironmentVariables::environmentVariables();
  String basePath;
  
  if (_typeName.BeginsWith("HISTOGRAM"))
    {
    if (importPath.EqualTo("DEFAULT"))
      {
      _importPath = envVariables->variable("CAP_HISTOS_IMPORT_PATH");
      }
    else if (importPath.BeginsWith("/"))
      {
      _importPath = importPath;
      }
    else
      {
      _importPath = envVariables->variable("CAP_HISTOS_IMPORT_PATH");
      _importPath += "/";
      _importPath += importPath;
      }
    if (exportPath.EqualTo("DEFAULT"))
      {
      _exportPath = envVariables->variable("CAP_HISTOS_EXPORT_PATH");
      }
    else if (exportPath.BeginsWith("/"))
      {
      _exportPath = exportPath;
      }
    else
      {
      _exportPath = envVariables->variable("CAP_HISTOS_IMPORT_PATH");
      _exportPath += "/";
      _exportPath += exportPath;
      }
    }
  if (_typeName.BeginsWith("PARTICLE_DB"))
    {
    if (importPath.BeginsWith("/"))
      _importPath = importPath;
    else
      {
      EnvironmentVariables * envVariables = EnvironmentVariables::environmentVariables();
      _importPath= envVariables->variable("CAP_DATABASE_PATH");
      _importPath += "/";
      _importPath += importPath;
      }
    }
  bool verbose = true;
  if (reportInfo(__FUNCTION__))
    {
    verbose = true;
    printCR();
    printValue(createKey(_taskName,_typeName,"USE"),   _use);
    printValue(createKey(_taskName,_typeName,"CREATE"),_create);
    printValue(createKey(_taskName,_typeName,"IMPORT"),_import);
    printValue(createKey(_taskName,_typeName,"IMPORT:PATH"),_importPath);
    printValue(createKey(_taskName,_typeName,"IMPORT:FILE_NAME"),_importFileName);
    printValue(createKey(_taskName,_typeName,"EXPORT"),_export);
    printValue(createKey(_taskName,_typeName,"EXPORT:PATH"),_exportPath);
    printValue(createKey(_taskName,_typeName,"EXPORT:FILE_NAME"),_exportFileName);
    printValue(createKey(_taskName,_typeName,"EXPORT:FORCE_REWRITE"),_forceRewrite);
    printValue(createKey(_taskName,_typeName,"N"),nObjects);
    }
  MANAGED * object;
  
  for (unsigned int k=0; k<nObjects; k++)
    {
    name  = configuration.valueString(createKey(_taskName,_typeName,k,"NAME"));
    title = configuration.valueString(createKey(_taskName,_typeName,k,"TITLE"));
    owner = configuration.valueBool(  createKey(_taskName,_typeName,k,"OWNER"));

    // Legacy fallback: <task>:<Foo>Name<k> / <Foo>Title<k> / <Foo>Owner<k>.
    if (legacyEntry.Length() > 0)
      {
      if (name.IsNull() || name.Length() == 0 || name == "NONE")
        {
        String key = _taskName; key += ":"; key += legacyEntry; key += "Name"; key += k;
        name = configuration.valueString(key);
        }
      if (title.IsNull() || title.Length() == 0 || title == "NONE")
        {
        String key = _taskName; key += ":"; key += legacyEntry; key += "Title"; key += k;
        title = configuration.valueString(key);
        }
      if (!owner)   // bool default is false; if the new key wasn't there
        {
        String key = _taskName; key += ":"; key += legacyEntry; key += "Owner"; key += k;
        owner = configuration.valueBool(key, /*useDefault=*/true, /*defaultValue=*/false);
        }
      }
    if (verbose)
      {
      printValue(createKey(_taskName,_typeName,k,"NAME"),name);
      printValue(createKey(_taskName,_typeName,k,"TITLE"),title);
      printValue(createKey(_taskName,_typeName,k,"OWNER"),owner);
      }
    if (owner)
      {
      object = instantiateObject(name);
      object->configure(_taskName,_typeName,configuration,k);
      }
    else
      object = useObject(name);
    }
  }
  
  virtual void initialize()
  {
  if (reportInfo(__FUNCTION__)) { printCR(); }
  if (_import) load();
  if (_create) create();
  }
  
  virtual void finalize()
  {
  if (reportDebug(__FUNCTION__)) { printCR(); }
  if (_export) exportObject();
  clear();
  }
  
  virtual void create()
  {
  if (reportDebug(__FUNCTION__)) { printCR(); }
  for (auto & object : _objects) object->create();
  }
  
  virtual void load()
  {
  if (reportDebug(__FUNCTION__)) { printCR(); }
  load(_importPath,_importFileName);
  }
  
  virtual void load(const String & importPath,
                            const String & importFileName)
  {
  if (reportDebug(__FUNCTION__))
    {
    printCR();
    printValue("importPath",importPath);
    printValue("importFileName",importFileName);
    };
  if (importFileName.EndsWith(".root"))
    {
    _inputRootFile = openOldRootFile(importPath,importFileName);
    loadFrom(*_inputRootFile);
    }
  else
    {
    if (_typeName.EqualTo("PARTICLE_DB"))
      {
      String fileName = importPath;
      fileName += "/";
      fileName += importFileName;
      std::ifstream & inputFile1 = openInputAsciiFile(fileName);
      fileName = importPath;
      fileName += "/decays.data";
      std::ifstream & inputFile2 = openInputAsciiFile(fileName);
      loadFromAscii2(inputFile1,inputFile2);
      }
    else
      {
      throw Exception("BAD TYPE",__FUNCTION__);
      }
    
    }
  }
  
  virtual void loadFrom(TFile & file)
  {
  if (reportDebug(__FUNCTION__)) { printCR(); }
  for (auto & object : _objects) object->loadFrom(file);
  }
  
  virtual void loadFromAscii1(std::ifstream & file)
  {
  if (reportDebug(__FUNCTION__)) { printCR(); }
  for (auto & object : _objects) object->loadFromAscii(file);
  }
  
  virtual void loadFromAscii2(std::ifstream & file1,std::ifstream & file2)
  {
  if (reportDebug(__FUNCTION__)) { printCR(); }
  for (auto & object : _objects) object->loadFromAscii2(file1,file2);
  }
  
  virtual void exportObject()
  {
  if (reportInfo(__FUNCTION__))
    {
    printCR();
    printValue("EXPORT:PATH",_exportPath);
    printValue("EXPORT:FILE_NAME",_exportFileName);
    };
  std::fprintf(stderr,
    "[ManagedObj.exportObject] BEGIN typeName=%s file=%s nObjects=%zu\n",
    _typeName.Data(), _exportFileName.Data(), _objects.size());
  std::fflush(stderr);
  if (_exportFileName.EndsWith(".root"))
    {
    // Defensive: openRecreateRootFile may fail if the directory doesn't
    // exist or isn't writable.  Original code dereferenced an unchecked
    // pointer → SEGV in saveTo on a wild reference.
    TFile * outputFilePtr = nullptr;
    try { outputFilePtr = openRecreateRootFile(_exportPath,_exportFileName); }
    catch (std::exception & ex) {
      std::fprintf(stderr,
        "[ManagedObj.exportObject] openRecreateRootFile threw: %s\n", ex.what());
      std::fflush(stderr);
      return;
    }
    catch (...) { return; }
    if (!outputFilePtr || outputFilePtr->IsZombie())
      {
      printCR();
      printString("ManagedObjects::exportObject: cannot open output file");
      printValue("EXPORT:PATH",_exportPath);
      printValue("EXPORT:FILE_NAME",_exportFileName);
      if (outputFilePtr) { outputFilePtr->Close(); delete outputFilePtr; }
      return;
      }
    TFile & outputFile = *outputFilePtr;
    size_t i = 0;
    for (auto & object : _objects)
      {
      if (!object) { ++i; continue; }
      std::fprintf(stderr,
        "[ManagedObj.exportObject]   saving #%zu of %zu (%s)\n",
        i, _objects.size(), object ? "non-null" : "null");
      std::fflush(stderr);
      try { object->saveTo(outputFile); }
      catch (std::exception & ex)
        {
        std::fprintf(stderr,
          "[ManagedObj.exportObject]   saveTo threw at #%zu: %s\n", i, ex.what());
        std::fflush(stderr);
        }
      catch (...)
        {
        std::fprintf(stderr,
          "[ManagedObj.exportObject]   saveTo threw unknown at #%zu\n", i);
        std::fflush(stderr);
        }
      ++i;
      }
    outputFile.Close();
    }
  else
    {
    String fileName = _exportPath;
    fileName += "/";
    fileName += _exportFileName;
    std::ofstream & outputFile = openOutputAsciiFile(fileName);
    for (auto & object : _objects) object->saveToAscii(outputFile);
    outputFile.close();
    }
  }
  
  virtual void reset()
  {
  for (auto & object : _objects) object->reset();
  }

  virtual void clear()
  {
  for (auto & object : _objects) object->clear();
  _objects.clear();
  if (_owner) clearStore();
  }

  virtual  void print() const
  {
  for (auto & object : _objects) object->print();
  }
  
  virtual  void scaleObject(const std::vector<double> & scaleFactors)
  {
  unsigned int k=0;
  for (auto & object : _objects) object->scaleObject(scaleFactors[k++]);
  }
  
  //  ClassDef(ManagedObjects<MANAGED>,0)
  };
  
  template <class MANAGED>
  std::vector<MANAGED*> * ManagedObjects<MANAGED>::objectStore = nullptr;
  
} // namespace CAP

#endif /* CAP_Manager */
