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
#include "EnvironmentVariables.hpp"
#include "NameHelpers.hpp"
#include "ParticleDbExceptions.hpp"
#include "ParticleDb.hpp"
#include "ParticleDbParser.hpp"
#include "PrintHelpers.hpp"
#include "StreamHelpers.hpp"
//#include "Task.hpp"

ClassImp(CAP::ParticleDb);

namespace CAP
{

  ParticleDb::ParticleDb()
  :
  ManagedObject(),
  _particleTypes(),
  _dbImport(true),
  _dbImportPath("ParticleData/"),
  _dbImportFile("particles.data"),
  _dbImportDecaysFile("decays.data"),
  _dbExport(false),
  _dbExportPath("ParticleData/"),
  _dbExportFile("newParticles.data"),
  _dbExportDecaysFile("newDecays.data"),
  _dbDisableNeutralParticles(false),
  _dbDisableChargedParticles(false),
  _dbDisableSelected(false),
  _dbEnableAllParticles(true),   // default value
  _dbEnableNeutralParticles(false),
  _dbEnableChargedParticles(false),
  _dbEnableSelected(false),
  //
  _dbDisableNeutralDecays(false),
  _dbDisableChargedDecays(false),
  _dbDisableSelectedDecays(false),
  _dbDisableWeakDecays(true),
  _dbDisableLongLivedDecays(false),
  _dbDisableShortLivedDecays(false),
  //
  _dbEnableAllDecays(true),  // default value
  _dbEnableNeutralDecays(false),
  _dbEnableChargedDecays(false),
  _dbEnableSelectedDecays(false),
  _dbEnableWeakDecays(false),
  _dbEnableLongLivedDecays(false),
  _dbEnableShortLivedDecays(false),
  _dbMaxLifeTime(0.0)
  {
  setClassName("ParticleDb");
  setClassName("ParticleDb");
  setMinimumReportLevel(Object::kTrace);
  }

  ParticleDb::ParticleDb(const ParticleDb & source)
  :
  ManagedObject(source),
  _particleTypes(source._particleTypes),
  _dbImport(source._dbImport),
  _dbImportPath(source._dbImportPath),
  _dbImportFile(source._dbImportFile),
  _dbImportDecaysFile(source._dbImportDecaysFile),
  _dbExport(source._dbExport),
  _dbExportPath(source._dbExportPath),
  _dbExportFile(source._dbExportFile),
  _dbExportDecaysFile(source._dbExportDecaysFile),
  _dbDisableNeutralParticles(source._dbDisableNeutralParticles),
  _dbDisableChargedParticles(source._dbDisableChargedParticles),
  _dbDisableSelected(source._dbDisableSelected),
  _dbEnableAllParticles(source._dbEnableAllParticles),   // default value
  _dbEnableNeutralParticles(source._dbEnableNeutralParticles),
  _dbEnableChargedParticles(source._dbEnableChargedParticles),
  _dbEnableSelected(source._dbEnableSelected),
  //
  _dbDisableNeutralDecays(source._dbDisableNeutralDecays),
  _dbDisableChargedDecays(source._dbDisableChargedDecays),
  _dbDisableSelectedDecays(source._dbDisableSelectedDecays),
  _dbDisableWeakDecays(source._dbDisableWeakDecays),
  _dbDisableLongLivedDecays(source._dbDisableLongLivedDecays),
  _dbDisableShortLivedDecays(source._dbDisableShortLivedDecays),
  //
  _dbEnableAllDecays(source._dbEnableAllDecays),  // default value
  _dbEnableNeutralDecays(source._dbEnableNeutralDecays),
  _dbEnableChargedDecays(source._dbEnableChargedDecays),
  _dbEnableSelectedDecays(source._dbEnableSelectedDecays),
  _dbEnableWeakDecays(source._dbEnableWeakDecays),
  _dbEnableLongLivedDecays(source._dbEnableLongLivedDecays),
  _dbEnableShortLivedDecays(source._dbEnableShortLivedDecays),
  _dbMaxLifeTime(source._dbMaxLifeTime)
  {   }

  ParticleDb & ParticleDb::operator=(const ParticleDb & rhs)
  {
  if (this!=&rhs)
    {
    Object::operator=(rhs);
    cloneAll(rhs);
    _dbImport = rhs._dbImport;
    _dbImportPath = rhs._dbImportPath;
    _dbImportFile = rhs._dbImportFile;
    _dbImportDecaysFile = rhs._dbImportDecaysFile;
    _dbExport = rhs._dbExport;
    _dbExportPath = rhs._dbExportPath;
    _dbExportFile = rhs._dbExportFile;
    _dbExportDecaysFile = rhs._dbExportDecaysFile;
    _dbDisableNeutralParticles = rhs._dbDisableNeutralParticles;
    _dbDisableChargedParticles = rhs._dbDisableChargedParticles;
    _dbDisableSelected = rhs._dbDisableSelected;
    _dbEnableAllParticles = rhs._dbEnableAllParticles;   // default value
    _dbEnableNeutralParticles = rhs._dbEnableNeutralParticles;
    _dbEnableChargedParticles = rhs._dbEnableChargedParticles;
    _dbEnableSelected = rhs._dbEnableSelected;
    //
    _dbDisableNeutralDecays = rhs._dbDisableNeutralDecays;
    _dbDisableChargedDecays = rhs._dbDisableChargedDecays;
    _dbDisableSelectedDecays = rhs._dbDisableSelectedDecays;
    _dbDisableWeakDecays = rhs._dbDisableWeakDecays;
    _dbDisableLongLivedDecays = rhs._dbDisableLongLivedDecays;
    _dbDisableShortLivedDecays = rhs._dbDisableShortLivedDecays;
    //
    _dbEnableAllDecays = rhs._dbEnableAllDecays;  // default value
    _dbEnableNeutralDecays = rhs._dbEnableNeutralDecays;
    _dbEnableChargedDecays = rhs._dbEnableChargedDecays;
    _dbEnableSelectedDecays = rhs._dbEnableSelectedDecays;
    _dbEnableWeakDecays = rhs._dbEnableWeakDecays;
    _dbEnableLongLivedDecays = rhs._dbEnableLongLivedDecays;
    _dbEnableShortLivedDecays = rhs._dbEnableShortLivedDecays;
    _dbMaxLifeTime = rhs._dbMaxLifeTime;
    }
  return *this;
  }

  void ParticleDb::cloneAll(const ParticleDb & list)
  {
  for (auto & particleType : list._particleTypes)
    _particleTypes.push_back( new ParticleType(*particleType) );
  }

  //!
  //!Sort by increasing mass
  //!
  void ParticleDb::sortByMass()
  {
  int n = size();
  for (int i = 1; i<n; i++)
    {
    int k = i;
    int j = i - 1;
    //m1 = objects[k]->mass();
    //m2 = objects[j]->mass();
    while (j >= 0  &&  (_particleTypes[k]->mass() < _particleTypes[j]->mass()) )
      {
      ParticleType* temp = _particleTypes[j];
      _particleTypes[j] = _particleTypes[k];
      _particleTypes[k] = temp;
      k--;
      j--;
      }
    }
  }

  void ParticleDb::resolveTypes()
  {
  //  for (auto & type : _particleTypes)
  //    {
  //    for (auto & mode : type->decayModes())
  //      mode->resolveTypes(*this);
  //    }
  }

  void ParticleDb::setupDecayGenerator()
  {
  for (auto & type : _particleTypes)
    {
    if (type->hasDecayModes()) type->setupDecayGenerator();
    }
  }


  // ================================================================================================
  // Extract a subset of this collection based on the given ctriterion
  // option : types selected
  //      0 : stable particle types
  //      1 :
  //      2 : unstable
  //      3 : mesons
  //      4 : baryons
  //      5 : leptons
  //      6 : fermions
  //      7 : bosons
  // ================================================================================================
  ParticleDb *  ParticleDb::extractList(int option)
  {
  ParticleDb * db = new ParticleDb();
  for (auto & type : _particleTypes)
    {
    bool selected = false;
    switch (option)
      {
        case 0:  if (type->isStable() && !type->isPhoton()) selected = true; break;
        case 1:  if (type->isDecayDisabled() && !type->isPhoton()) selected = true; break;
        case 2:  if (!type->isStable()) selected = true; break;
        case 3:  if (!type->isDecayDisabled()) selected = true; break;
        case 4:  if (type->isMeson()) selected = true; break;
        case 5:  if (type->isBaryon()) selected = true; break;
        case 6:  if (type->isLepton()) selected = true; break;
        case 7:  if (type->isFermion()) selected = true; break;
        case 8:  if (type->isBoson()) selected = true; break;
      }
    if (selected) db->append(type);
    }
  return db;
  }

  int ParticleDb::findIndexForType(ParticleType * type)  const
  {
  int index = 0;
  for (auto & pt : _particleTypes)
    {
    if (type==pt) return index;
    index++;
    }
  return -1;
  }

  int ParticleDb::findIndexForName(const String & aName) const
  {
  int index = 0;
  for (auto & pt : _particleTypes)
    {
    if (pt->name().EqualTo(aName)) return index;
    index++;
    }
  return -1;
  }


  int ParticleDb::findIndexForPdgCode(int code) const
  {
  int index = 0;
  for (auto & pt : _particleTypes)
    {
    if (code == pt->pdgCode()) return index;
    index++;
    }
  return -1;
  }


  int ParticleDb::findIndexForPrivateCode(int code) const
  {
  int index = 0;
  for (auto & pt : _particleTypes)
    {
    if (code == pt->privateCode()) return index;
    index++;
    }
  return -1;
  }

  bool ParticleDb::containsTypeNamed(const String & aName) const
  {
  for (auto & pt : _particleTypes)
    {
    if (pt->name().EqualTo(aName)) return true;
    }
  return false;
  }

  ParticleType * ParticleDb::findTypeNamed(const String & aName) const
  {
  for (auto & pt : _particleTypes)
    {
    if (pt->name().EqualTo(aName)) return pt;
    }
  throw UndefinedParticleTypeException(aName,__FUNCTION__);

  }

  ParticleType * ParticleDb::findPdgCode(int code) const
  {
  for (auto & pt : _particleTypes)
    {
    if (code == pt->pdgCode()) return pt;
    }
  throw InvalidIndexParticleTypeException(code,"Unknown pdg code",__FUNCTION__);
  }

  ParticleType * ParticleDb::findPrivateCode(int code) const
  {
  for (auto & pt : _particleTypes)
    {
    if (code == pt->pdgCode()) return pt;
    }
  throw InvalidIndexParticleTypeException(code,"Unknown private code",__FUNCTION__);
  }

  //  ParticleType * ParticleDb::findTypeNamed(const String & aName)
  //  {
  //  for (auto & pt : _particleTypes)
  //    {
  //    if (pt->name().EqualTo(aName)) return pt;
  //    }
  //  throw UndefinedParticleTypeException(aName,__FUNCTION__);
  //
  //  }
  //
  //  ParticleType * ParticleDb::findPdgCode(int code)
  //  {
  //  for (auto & pt : _particleTypes)
  //    {
  //    if (code == pt->pdgCode()) return pt;
  //    }
  //  throw InvalidIndexParticleTypeException(code,"Unknown pdg code",__FUNCTION__);
  //  }
  //
  //  ParticleType * ParticleDb::findPrivateCode(int code)
  //  {
  //  for (auto & pt : _particleTypes)
  //    {
  //    if (code == pt->pdgCode()) return pt;
  //    }
  //  throw InvalidIndexParticleTypeException(code,"Unknown private code",__FUNCTION__);
  //  }


  void ParticleDb::append(ParticleType * type)
  {
  _particleTypes.push_back(type);
  }

  ParticleType * ParticleDb::particleType(const String & aName)
  {
  for (auto & type : _particleTypes)
    {
    if (type->name().EqualTo(aName)) return type;
    }
  throw UndefinedParticleTypeException(aName,__FUNCTION__);
  }


  ParticleType * ParticleDb::particleType(unsigned int index)
  {
  if (index<_particleTypes.size())
    return _particleTypes[index];
  throw IndexOutOfBoundException(index,_particleTypes.size(),"invalid index", __FUNCTION__);
  }

  void ParticleDb::clear()
  {
  for (auto & type : _particleTypes) delete type;
  _particleTypes.clear();
  }

  //!
  //!Find the antiparticles of all particles in the DB and
  //!store their DB index for fast retrieval in some functions.
  //!(assumes no sorting is done after this operation)
  //!Antiparticles have -pdgCode of their particle and viceversa
  //!Particles that have no antiparticle will store -1 to indicates
  //!they do not have an anti-particle.
  //!
  void ParticleDb::mapAntiParticleIndices()
  {
  int index = 0;
  int indexBar = 0;
  for (auto & type : _particleTypes)
    {
    indexBar = findIndexForPdgCode(-type->pdgCode());
    if (indexBar>=0)
      type->setAntiParticleIndex(indexBar);
    else
      type->setAntiParticleIndex(index); // self (has no antiparticle)
    index++;
    }
  }

  void ParticleDb::print()  const
  {
  std::ostream & os = *gOutputStream;
  printLine();
  printString("ParticleDb");
  os <<  fixed << setw(5)  << "k";
  os <<  fixed << setw(10) << "PDG";
  os <<  fixed << setw(10) << "Name";
  os <<  fixed << setw(12) << "Title";
  os <<  fixed << setw(12) << "Mass";
  os <<  fixed << setw(12) << "Width";
  os <<  fixed << setw(9)  << "Charge";
  os <<  fixed << setw(9)  << "Spin";
  os <<  fixed << setw(9)  << "Isospin";
  os <<  fixed << setw(9)  << "Isospin3";
  os <<  fixed << setw(5)  << "Nq";
  os <<  fixed << setw(5)  << "Naq";
  os <<  fixed << setw(5)  << "Ns";
  os <<  fixed << setw(5)  << "Nas";
  os <<  fixed << setw(5)  << "Nc";
  os <<  fixed << setw(5)  << "Nac";
  os <<  fixed << setw(5)  << "Nb";
  os <<  fixed << setw(5)  << "Nab";
  os <<  fixed << setw(5)  << "Nt";
  os <<  fixed << setw(5)  << "Nat";
  os <<  fixed << setw(5)  << "L(e)";
  os <<  fixed << setw(5)  << "L(m)";
  os <<  fixed << setw(5)  << "L(t)";
  os <<  fixed << setw(5)  << "S";
  os <<  fixed << setw(5)  << "W";
  printLine();
  for (auto & type : _particleTypes) type->print();
  }

  void ParticleDb::printDecays() const
  {
  std::ostream & os = *gOutputStream;
  printLine();  os << std::setw(10) << "Pdg";
  os << setw(20) << "Title";
  os << setw(20) << "Mass(GeV/c2)";
  os << setw(20) << "Width(GeV/c2)" << endl;
  printLine();
  for (auto & type : _particleTypes) type->printDecays();
  }


  std::vector<int> ParticleDb::listOfPdgCodes()
  {
  std::vector<int> codes;
  for (auto & type : _particleTypes)
    codes.push_back(type->pdgCode());
  return codes;
  }

  std::vector<ParticleType*> & ParticleDb::findWeakDecays()
  {
  _weakDecays.push_back( findTypeNamed("Ka0492zer")); // 311);   // neutral kaons
  _weakDecays.push_back( findTypeNamed("Ka0492zrb")); // -311);
                                                      // lambda
  _weakDecays.push_back( findTypeNamed("Lm1115zer")); //  3122); // lambda
  _weakDecays.push_back( findTypeNamed("Lm1115zrb")); // -3122);
                                                      // sigma
  _weakDecays.push_back( findTypeNamed("Sg1189min")); //  3112); // sigmas
  _weakDecays.push_back( findTypeNamed("Sg1189mnb")); // -3112);
  _weakDecays.push_back( findTypeNamed("Sg1192zer")); //  3212);
  _weakDecays.push_back( findTypeNamed("Sg1192zrb")); // -3212);
  _weakDecays.push_back( findTypeNamed("Sg1189plu")); //  3222);
  _weakDecays.push_back( findTypeNamed("Sg1189plb")); // -3222);
                                                      // xi (cascade)
  _weakDecays.push_back( findTypeNamed("Xi1321min")); //  3312);  // cascades
  _weakDecays.push_back( findTypeNamed("Xi1321mnb")); // -3312);
  _weakDecays.push_back( findTypeNamed("Xi1321zer")); //  3322);
  _weakDecays.push_back( findTypeNamed("Xi1321zrb")); // -3322);
                                                      // Omega
  _weakDecays.push_back( findTypeNamed("UM1672min")); //  3334);  // Omegas
  _weakDecays.push_back( findTypeNamed("UM1672mnb")); // -3334);
  return _weakDecays;
  }

  void ParticleDb::configure(const String & taskName,
                             const String & objectType,
                             const Configuration & configuration,
                             unsigned int index)
  {
  if (reportDebug(__FUNCTION__))
    {
    printCR();
    printValue("taskName",  taskName);
    printValue("objectType",objectType);
    }
  //  _dbImport            = configuration.valueBool(  createKey(taskName,objectType,index,"IMPORT"));
  //  _dbImportPath        = configuration.valueString(createKey(taskName,objectType,index,"IMPORT:PATH"));
  //  _dbImportFile        = configuration.valueString(createKey(taskName,objectType,index,"IMPORT:FILE_NAME"));
  //  _dbImportDecaysFile  = configuration.valueString(createKey(taskName,objectType,index,"IMPORT:DECAYS_FILE"));
  //  _dbExport            = configuration.valueBool(  createKey(taskName,objectType,index,"EXPORT"));
  //  _dbExportPath        = configuration.valueString(createKey(taskName,objectType,index,"EXPORT:PATH"));
  //  _dbExportFile        = configuration.valueString(createKey(taskName,objectType,index,"EXPORT:FILE"));
  //  _dbExportDecaysFile  = configuration.valueString(createKey(taskName,objectType,index,"EXPORT:DECAYS_FILE"));

  _dbDisableNeutralParticles = configuration.valueBool(  createKey(taskName,objectType,index,"_dbDisableNeutralParticles"));
  _dbDisableChargedParticles = configuration.valueBool(  createKey(taskName,objectType,index,"_dbDisableChargedParticles"));
  _dbDisableSelected         = configuration.valueBool(  createKey(taskName,objectType,index,"_dbDisableSelected"));

  _dbEnableAllParticles      = configuration.valueBool(  createKey(taskName,objectType,index,"_dbEnableAllParticles"));
  _dbEnableNeutralParticles  = configuration.valueBool(  createKey(taskName,objectType,index,"_dbEnableNeutralParticles"));
  _dbEnableChargedParticles  = configuration.valueBool(  createKey(taskName,objectType,index,"_dbEnableChargedParticles"));
  _dbEnableSelected          = configuration.valueBool(  createKey(taskName,objectType,index,"_dbEnableSelected"));

  _dbDisableNeutralDecays    = configuration.valueBool(  createKey(taskName,objectType,index,"_dbDisableNeutralDecays"));
  _dbDisableChargedDecays    = configuration.valueBool(  createKey(taskName,objectType,index,"_dbDisableChargedDecays"));
  _dbDisableSelectedDecays   = configuration.valueBool(  createKey(taskName,objectType,index,"_dbDisableSelectedDecays"));
  _dbDisableWeakDecays       = configuration.valueBool(  createKey(taskName,objectType,index,"_dbDisableWeakDecays"));
  _dbDisableLongLivedDecays  = configuration.valueBool(  createKey(taskName,objectType,index,"_dbDisableLongLivedDecays"));
  _dbDisableShortLivedDecays = configuration.valueBool(  createKey(taskName,objectType,index,"_dbDisableShortLivedDecays"));
  _dbEnableAllDecays         = configuration.valueBool(  createKey(taskName,objectType,index,"_dbEnableAllDecays"));
  _dbEnableNeutralDecays     = configuration.valueBool(  createKey(taskName,objectType,index,"_dbEnableNeutralDecays"));
  _dbEnableChargedDecays     = configuration.valueBool(  createKey(taskName,objectType,index,"_dbEnableChargedDecays"));
  _dbEnableSelectedDecays    = configuration.valueBool(  createKey(taskName,objectType,index,"_dbEnableSelectedDecays"));
  _dbEnableWeakDecays        = configuration.valueBool(  createKey(taskName,objectType,index,"_dbEnableWeakDecays"));
  _dbEnableLongLivedDecays   = configuration.valueBool(  createKey(taskName,objectType,index,"_dbEnableLongLivedDecays"));
  _dbEnableShortLivedDecays  = configuration.valueBool(  createKey(taskName,objectType,index,"_dbEnableLongLivedDecays"));
  _dbMaxLifeTime             = configuration.valueDouble(createKey(taskName,objectType,index,"MaxLifeTime"));

  if (reportDebug(__FUNCTION__))
    {
    cout << endl;
    printValue(createKey(taskName,objectType,index,"IMPORT")             ,_dbImport);
    printValue(createKey(taskName,objectType,index,"IMPORT:PATH")        ,_dbImportPath);
    printValue(createKey(taskName,objectType,index,"IMPORT:FILE")        ,_dbImportFile);
    printValue(createKey(taskName,objectType,index,"IMPORT:DECAYS_FILE") ,_dbImportDecaysFile);
    printValue(createKey(taskName,objectType,index,"EXPORT")             ,_dbExport);
    printValue(createKey(taskName,objectType,index,"EXPORT:PATH")        ,_dbExportPath);
    printValue(createKey(taskName,objectType,index,"EXPORT:FILE")        ,_dbExportFile);
    printValue(createKey(taskName,objectType,index,"EXPORT:DECAYS_FILE") ,_dbExportDecaysFile);

    printValue(createKey(taskName,objectType,index,"EnableAllParticles")     ,_dbEnableAllParticles);
    printValue(createKey(taskName,objectType,index,"EnableNeutralParticles") ,_dbEnableNeutralParticles);
    printValue(createKey(taskName,objectType,index,"EnableChargedParticles") ,_dbEnableChargedParticles);
    printValue(createKey(taskName,objectType,index,"EnableSelected")         ,_dbEnableSelected);
    printValue(createKey(taskName,objectType,index,"DisableNeutralParticles"),_dbDisableNeutralParticles);
    printValue(createKey(taskName,objectType,index,"DisableChargedParticles"),_dbDisableChargedParticles);
    printValue(createKey(taskName,objectType,index,"DisableSelected")        ,_dbDisableSelected);

    printValue(createKey(taskName,objectType,index,"EnableAllDecays"),        _dbEnableAllDecays);
    printValue(createKey(taskName,objectType,index,"EnableSelectedDecays")   ,_dbEnableSelectedDecays);
    printValue(createKey(taskName,objectType,index,"EnableNeutralDecays")    ,_dbEnableNeutralDecays);
    printValue(createKey(taskName,objectType,index,"EnableChargedDecays")    ,_dbEnableChargedDecays);
    printValue(createKey(taskName,objectType,index,"EnableWeakDecays")       ,_dbEnableWeakDecays);
    printValue(createKey(taskName,objectType,index,"EnableLongLivedDecays")  ,_dbEnableLongLivedDecays);
    printValue(createKey(taskName,objectType,index,"EnableShortLivedDecays") ,_dbEnableShortLivedDecays);
    printValue(createKey(taskName,objectType,index,"MaxLifeTime")            ,_dbMaxLifeTime);
    printValue(createKey(taskName,objectType,index,"DisableNeutralDecays")   ,_dbDisableNeutralDecays);
    printValue(createKey(taskName,objectType,index,"DisableChargedDecays")   ,_dbDisableChargedDecays);
    printValue(createKey(taskName,objectType,index,"DisableSelectedDecays")  ,_dbDisableSelectedDecays);
    printValue(createKey(taskName,objectType,index,"DisableWeakDecays")      ,_dbDisableWeakDecays);
    printValue(createKey(taskName,objectType,index,"DisableLongLivedDecays") ,_dbDisableLongLivedDecays);
    printValue(createKey(taskName,objectType,index,"DisableShortLivedDecays"),_dbDisableShortLivedDecays);
    for (int k=0; k<40; k++)
      {
      String s = "_dbEnable"; s += k;
      cout << k << "   " << s << "  :  " << configuration.valueString( createKey(taskName,objectType,index,s))  << endl;;
      }
    for (int k=0; k<40; k++)
      {
      String s = "_dbDisable"; s += k;
      cout << k << "   " << s << "  :  " << configuration.valueString( createKey(taskName,objectType,index,s)) << endl;;
      }
    for (int k=0; k<40; k++)
      {
      String s = "_dbDisableDecay"; s += k;
      cout << k << "   " << s << "  :  " << configuration.valueString( createKey(taskName,objectType,index,s)) << endl;;
      }
    for (int k=0; k<40; k++)
      {
      String s = "_dbEnableDecay"; s += k;
      cout << k << "   " << s << "  :  " << configuration.valueString( createKey(taskName,objectType,index,s)) << endl;;
      }
    cout << endl;
    }
  }


  void ParticleDb::loadFromAscii2(std::ifstream & inputFile1,
                                  std::ifstream & inputFile2)
  {
  ParticleDbParser parser;
  parser.readDb(*this,inputFile1,inputFile2);
  //  inputFile.close();
  //  inputFileDecays.close();
  mapAntiParticleIndices();
  setupDecayGenerator();
  if (reportDebug(__FUNCTION__))
    {
    printCR();
    printString("Successfully initiated ParticleDb");
    printValue("ParticleDb.size()",_particleTypes.size());
    }

  // finish setup

  if (_dbEnableAllParticles)
    {
    enableAllParticles(); // default
    if (_dbDisableNeutralParticles) disableNeutralParticles();
    if (_dbDisableChargedParticles) disableChargedParticles();
    //if (_dbDisableSelected)         disableSelectedParticles();
    }
  else
    {
    disableAllParticles();
    if (_dbEnableNeutralParticles) enableNeutralParticles();
    if (_dbEnableChargedParticles) enableChargedParticles();
    //if (_dbEnableSelected)         enableSelectedParticles();
    }

  if (_dbEnableAllDecays)
    {
    enableAllDecays(); // default
                       //    if (_dbDisableNeutralDecays)    disableNeutralDecays();
                       //    if (_dbDisableChargedDecays)    disableChargedDecays();
                       //    if (_dbDisableWeakDecays)       disableWeakDecays();
                       //    if (_dbDisableLongLivedDecays)  disableDecaysTauGreatThan(_dbMaxLifeTime);
                       //    if (_dbDisableShortLivedDecays) disableDecaysTauShorterThan(_dbMaxLifeTime);
                       //    if (_dbDisableSelectedDecays)   disableSelectedDecays(); // default
    }
  //  else
  //    {
  //    disableAllDecays();
  //    if (_dbEnableNeutralDecays)    enableNeutralDecays();
  //    if (_dbEnableChargedDecays)    enableChargedDecays();
  //    if (_dbEnableWeakDecays)       enableWeakDecays();
  //    if (_dbEnableLongLivedDecays)  enableDecaysTauGreaterThan(_dbMaxLifeTime);
  //    if (_dbEnableShortLivedDecays) enableDecaysTauShorterThan(_dbMaxLifeTime);
  //    //if (_dbEnableSelectedDecays)   enableSelectedDecays();
  //    }
  if (reportDebug(__FUNCTION__))
    {
    printCR();
    printString("Successfully loaded/initiated ParticleDb");
    }
  sortByMass();
  }

  //  void ParticleDb::dbAnalyzer()
  //  {
  //  unsigned int nTypes = size();
  //  int count = 0;
  //  for (unsigned int iType=0; iType<nTypes; iType++)
  //    {
  //    ParticleType * particleType = particleType(iType);
  //    int     pdg     = particleType->pdgCode();
  //    //    double  charge  = particleType->charge();
  //    //    double  strange = particleType->strangeness();
  //    //    double  baryon  = particleType->baryonNumber();
  //    count = 0;
  //    for (unsigned int kType=0; kType<nTypes; kType++)
  //      {
  //      ParticleType * particleType2 = particleType(kType);
  //      int     pdg2     = particleType2->pdgCode();
  //      //      double  charge2  = particleType2->charge();
  //      //      double  strange2 = particleType2->strangeness();
  //      //      double  baryon2  = particleType2->baryonNumber();
  //      if (pdg == pdg2) count++;
  //      }
  //    if (count>1)
  //      {
  //      cout << "iType: " << iType << " Count: " << count << endl;
  //      cout << " name1: " << particleType->name() << "   PDG:" <<pdg<< endl;
  //      //cout << " name2: " << particleType2->name() << endl;
  //      }
  //    }
  //
  //  for (unsigned int iType=0; iType<nTypes; iType++)
  //    {
  //    ParticleType * type = particleType(iType);
  //    String  name    = type->name();
  //    int     pdg     = type->pdgCode();
  //    double  charge  = type->charge();
  //    double  strange = type->strangeness();
  //    double  baryon  = type->baryonNumber();
  //    bool    stable  = type->isStable();
  //    int     nModes  = type->nDecayModes();
  //    if (stable) continue;
  //    if (nModes<1) throw ParticleDecayException("nModes<1  &&  !isStable()",__FUNCTION__);
  //
  //    cout << "Analysing particle named: " << name << " PDG Index:" << pdg << endl;
  //    cout << "    Charge: " << charge << endl;
  //    cout << "   Strange: " << strange << endl;
  //    cout << "    Baryon: " << baryon << endl;
  //    cout << "    Stable: " << stable << endl;
  //    cout << "    nModes: " << nModes << endl;
  //
  //    std::vector<ParticleDecayMode> decayModes =  type->decayModes();
  //
  //    for (int iMode=0; iMode<nModes; iMode++)
  //      {
  //      ParticleDecayMode & decayMode = decayModes[iMode];
  //      int nChildren = decayMode.nChildren();
  //      //cout << "Decay mode: " << iMode << "  nChildren: " << nChildren << endl;
  //
  //      switch (nChildren)
  //        {
  //          case 1:
  //          {
  //          String s = "Single particle decay for ";
  //          s += name;
  //          throw ParticleDecayException(s,__FUNCTION__);
  //          }
  //
  //          case 2:
  //          {
  //          ParticleType & childType1 = decayMode.childType(0);
  //          ParticleType & childType2 = decayMode.childType(1);
  //          String name1 = childType1.name();
  //          String name2 = childType2.name();
  //          double sumCharge  = childType1.charge()          + childType2.charge();
  //          double sumStrange = childType1.strangeness() + childType2.strangeness();
  //          double sumBaryon  = childType1.baryonNumber()    + childType2.baryonNumber();
  //          if (charge!=sumCharge || strange!=sumStrange || baryon!=sumBaryon)
  //            {
  //            cout << "Check sanity!!!" << endl;
  //            cout << "Analysing particle named: " << name << " PDG Index:" << pdg << endl;
  //            //    cout << "    Charge: " << charge << endl;
  //            //    cout << "   Strange: " << strange << endl;
  //            //    cout << "    Baryon: " << baryon << endl;
  //            //    cout << "    Stable: " << stable << endl;
  //            //    cout << "    nModes: " << nModes << endl;
  //            cout << name << "  -->  " << name1 << "  +  " << name2 << endl;
  //            cout << " Charge: " << charge <<  "    sumCharge: " << sumCharge << endl;
  //            cout << "Strange: " << strange << "   sumStrange: " << sumStrange << endl;
  //            cout << " Baryon: " << baryon <<  "   sumBaryon: " << sumBaryon << endl;
  //            }
  //          break;
  //          }
  //
  //          case 3:
  //          {
  //          ParticleType & childType1 = decayMode.childType(0);
  //          ParticleType & childType2 = decayMode.childType(1);
  //          ParticleType & childType3 = decayMode.childType(2);
  //          String name1 = childType1.name();
  //          String name2 = childType2.name();
  //          String name3 = childType3.name();
  //          double sumCharge  = childType1.charge()          + childType2.charge()+ childType3.charge();
  //          double sumStrange = childType1.strangeness() + childType2.strangeness()+ childType3.strangeness();
  //          double sumBaryon  = childType1.baryonNumber()    + childType2.baryonNumber()+ childType3.baryonNumber();
  //          if (charge!=sumCharge || strange!=sumStrange || baryon!=sumBaryon)
  //            {
  //            cout << "ERROR!!!!!!!!!!!" << endl;
  //            cout << "Analysing particle named: " << name << " PDG Index:" << pdg << endl;
  //            cout << name << "  -->  " << name1 << "  +  " << name2 << "  +  " << name3 << endl;
  //            cout << " Charge: " << charge <<  "    sumCharge: " << sumCharge << endl;
  //            cout << "Strange: " << strange << "   sumStrange: " << sumStrange << endl;
  //            cout << " Baryon: " << baryon <<  "   sumBaryon: " << sumBaryon << endl;
  //            }
  //          break;
  //          }
  //        }
  //
  //      }
  //    }
  //  }

  void ParticleDb::enableAllParticles()
  {
  for (auto & particleType : _particleTypes)
    {
    if (!particleType) particleType->enable();
    }
  }

  void ParticleDb::enableNeutralParticles()
  {
  for (auto & particleType : _particleTypes)
    if (particleType->isNeutral()) particleType->enable();
  }

  void ParticleDb::enableChargedParticles()
  {
  for (auto & particleType : _particleTypes)
    if (particleType->isCharged()) particleType->enable();
  }

  void ParticleDb::enableSelectedParticles(std::vector<ParticleType*> & selection)
  {
  for (auto & particleType : selection) particleType->enable();
  }

  void ParticleDb::disableAllParticles()
  {
  for (auto & particleType : _particleTypes) particleType->disable();
  }

  void ParticleDb::disableNeutralParticles()
  {
  for (auto & particleType : _particleTypes)
    if (particleType->isNeutral()) particleType->disable();
  }

  void ParticleDb::disableChargedParticles()
  {
  for (auto & particleType : _particleTypes)
    if (particleType->isCharged()) particleType->disable();
  }

  void ParticleDb::disableSelectedParticles(std::vector<ParticleType*> & selection)
  {
  for (auto & particleType : selection) particleType->disable();
  }

  void ParticleDb::disableAllDecays()
  {
  for (auto & particleType : _particleTypes) particleType->disableDecay();
  }

  void ParticleDb::disableNeutralDecays()
  {
  for (auto & particleType : _particleTypes)
    {
    if (particleType->isNeutral()) particleType->disableDecay();
    }
  }

  void ParticleDb::disableChargedDecays()
  {
  for (auto & particleType : _particleTypes)
    {
    if (particleType->isCharged()) particleType->disableDecay();
    }
  }


  void ParticleDb::disableWeakDecays()
  {
  _weakDecays.clear();
  findWeakDecays();
  for (auto & type : _weakDecays) type->disableDecay();
  }

  void ParticleDb::disableDecaysTauGreatThan(double lifeTime)
  {
  for (auto & type : _particleTypes)
    {
    if (!type->isStable())
      {
      double tau = type->lifeTime();
      if (tau>lifeTime) type->disableDecay();
      }
    }
  }

  void ParticleDb::disableDecaysTauShorterThan(double time)
  {
  for (auto & type : _particleTypes)
    {
    if (!type->isStable() &&  time>type->lifeTime()) type->disableDecay();
    }
  }


  void ParticleDb::disableSelectedDecays(std::vector<ParticleType*> & selection)
  {
  for (auto & type : selection) type->disableDecay();
  }

  void ParticleDb::enableAllDecays()
  {
  for (auto & type : _particleTypes) type->enableDecay();
  }

  void ParticleDb::enableNeutralDecays()
  {
  for (auto & type : _particleTypes)
    if (type->isNeutral()) type->enableDecay();
  }

  void ParticleDb::enableChargedDecays()
  {
  for (auto & type : _particleTypes)
    if (type->isCharged()) type->enableDecay();
  }

  void ParticleDb::enableWeakDecays()
  {
  _weakDecays.clear();
  findWeakDecays();
  for (auto & type : _weakDecays)  type->enableDecay();
  }

  void ParticleDb::enableDecaysTauGreaterThan(double time)
  {
  for (auto & type : _particleTypes)
    {
    if (!type->isStable() && time<type->lifeTime()) type->enableDecay();
    }
  }

  void ParticleDb::enableDecaysTauShorterThan(double time)
  {
  for (auto & type : _particleTypes)
    {
    if (!type->isStable() && time>=type->lifeTime()) type->enableDecay();
    }
  }


  void ParticleDb::enableSelectedDecays(std::vector<ParticleType*> & selection)
  {
  for (auto & type : selection) type->enableDecay();
  }

  void ParticleDb::listParticleStatus()
  {
  for (auto & type : _particleTypes)
    {
    std::cout << type->name();
    if (type->isEnabled())
      std::cout << " Enabled,";
    else
      std::cout << " Disabled,";

    if (type->isStable())
      std::cout << " Stable.";
    else
      {
      std::cout << " Unstable,";
      if (type->isDecayEnabled())
        std::cout << " DecayEnabled,";
      else
        std::cout << " DecayDisabled, ";
      std::cout << " n decay modes: " << type->nDecayModes();
      }
    std::cout << std::endl;
    }
  }

} // namespace CAP
