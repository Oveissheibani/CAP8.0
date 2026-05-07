/* **********************************************************************
 * Copyright (C) 2019-2024, Claude Pruneau, Victor Gonzalez
 * Copyright (C) 2016, Chun Shen
 * All rights reserved.
 *
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 *
 * Author: Claude Pruneau,   04/01/2024
 *
 * *********************************************************************/
#ifndef CAP__ParticleDb
#define CAP__ParticleDb
#include "Configuration.hpp"
#include "ManagedObject.hpp"
#include "Object.hpp"
#include "ParticleType.hpp"

namespace CAP
{
  class ParticleType;

  class ParticleDb
  :
  public ManagedObject
  {
  public:

  // const String& _name, bool _ownership, Severity logLevel
  ParticleDb();
  ParticleDb(const ParticleDb & analyzer);
  ParticleDb & operator=(const ParticleDb & analyzer);

  virtual ~ParticleDb() {}
  void sortByMass();
  ParticleDb * extractList(int option);
  int findIndexForType(ParticleType * type) const;
  int findIndexForName(const String & name) const;
  int findIndexForPdgCode(int pdgCode) const;
  int findIndexForPrivateCode(int privateCode) const;
  bool containsTypeNamed(const String & aName) const;

  Size_t size() { return _particleTypes.size();}
  std::vector<ParticleType*> & particleTypes()   { return _particleTypes; }
  const std::vector<ParticleType*> & particleTypes() const   { return _particleTypes; }

  void resolveTypes();
  void mapAntiParticleIndices();
  void setupDecayGenerator();
  ParticleType * findPdgCode(int pdgCode) const;
  ParticleType * findPrivateCode(int privateCode) const;
  ParticleType * findTypeNamed(const String & aName) const;
  //  ParticleType * findPdgCode(int pdgCode);
  //  ParticleType * findPrivateCode(int privateCode);
  //  ParticleType * findTypeNamed(const String & aName);
  std::vector<ParticleType*> & findWeakDecays();

  void append(ParticleType * particleType);
  ParticleType * particleType(const String & name);
  ParticleType * particleType(unsigned int index);
  virtual void clear();
  std::vector<int> listOfPdgCodes();

  virtual void print() const;
  virtual void printDecays() const;

  //  static ParticleDb & list();


  virtual void configure(const String & taskName,
                         const String & objectType,
                         const Configuration & configuration,
                         unsigned int index);


  virtual void loadFromAscii2(std::ifstream & file1,
                              std::ifstream & file2);

  //virtual void dbAnalyzer();
  virtual void enableAllParticles();
  virtual void enableNeutralParticles();
  virtual void enableChargedParticles();
  virtual void enableSelectedParticles(std::vector<ParticleType*> & selection);
  virtual void disableAllParticles();
  virtual void disableNeutralParticles();
  virtual void disableChargedParticles();
  virtual void disableSelectedParticles(std::vector<ParticleType*> & selection);

  virtual void disableAllDecays();
  virtual void disableNeutralDecays();
  virtual void disableChargedDecays();
  virtual void disableWeakDecays();
  virtual void disableDecaysTauGreatThan(double lifeTime);
  virtual void disableDecaysTauShorterThan(double lifeTime);
  virtual void disableSelectedDecays(std::vector<ParticleType*> & selection);

  virtual void enableAllDecays();
  virtual void enableNeutralDecays();
  virtual void enableChargedDecays();
  virtual void enableWeakDecays();
  virtual void enableDecaysTauGreaterThan(double lifeTime);
  virtual void enableDecaysTauShorterThan(double lifeTime);
  virtual void enableSelectedDecays(std::vector<ParticleType*> & selection);
  virtual void listParticleStatus();

  protected:
  void cloneAll(const ParticleDb & db);

  std::vector<ParticleType*> _particleTypes;
  std::vector<ParticleType*> _weakDecays;

  bool   _dbImport;
  String _dbImportPath;
  String _dbImportFile;
  String _dbImportDecaysFile;
  bool   _dbExport;
  String _dbExportPath;
  String _dbExportFile;
  String _dbExportDecaysFile;

  bool _dbDisableNeutralParticles;
  bool _dbDisableChargedParticles;
  bool _dbDisableSelected;

  bool _dbEnableAllParticles;
  bool _dbEnableNeutralParticles;
  bool _dbEnableChargedParticles;
  bool _dbEnableSelected;

  bool _dbDisableNeutralDecays;
  bool _dbDisableChargedDecays;
  bool _dbDisableSelectedDecays;
  bool _dbDisableWeakDecays;
  bool _dbDisableLongLivedDecays;
  bool _dbDisableShortLivedDecays;

  bool _dbEnableAllDecays;
  bool _dbEnableNeutralDecays;
  bool _dbEnableChargedDecays;
  bool _dbEnableSelectedDecays;
  bool _dbEnableWeakDecays;
  bool _dbEnableLongLivedDecays;
  bool _dbEnableShortLivedDecays;
  double _dbMaxLifeTime;

  ClassDef(ParticleDb,0)
  };

} //namespace CAP

#endif  // CAP__ParticleDb

