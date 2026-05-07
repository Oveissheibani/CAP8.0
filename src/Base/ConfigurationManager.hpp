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
#ifndef CAP__ConfigurationManager
#define CAP__ConfigurationManager
#include "Object.hpp"
#include "Configuration.hpp"

namespace CAP
{

class ConfigurationManager
{

public:
  ConfigurationManager();
  ConfigurationManager(const ConfigurationManager & source);
  virtual ~ConfigurationManager();
  ConfigurationManager operator=(const ConfigurationManager & rhs);

  //!
  //! Returns a reference to a constant Configuration object controlling the behavior of this task instance.
  //! @return reference to the Configuration instance controlling this task instance.
  //!
  const Configuration & configuration() const
  {
  return _configuration;
  }

  //!
  //! Returns a reference to a Configuration object controlling the behavior of this task instance.
  //! @return reference to the Configuration instance controlling this task instance.
  //!
  Configuration & configuration()
  {
  return _configuration;
  }

  virtual void setDefaultConfiguration()
  {
  // no ops
  }

  //!
  //! Add all the parameters in the given requested configuration to the configuration of this manager.
  //!
  virtual void setConfiguration(const Configuration & reqConfiguration)
  {
  _configuration.addProperties(reqConfiguration);
  }

  //!
  //! Add all the parameters in the given requested configuration to the configuration of this manager.
  //!
  virtual void configure(const Configuration & reqConfiguration)
  {
  _configuration.addProperties(reqConfiguration);
  }

  virtual void exportDictionary(const String & path, const String & name, bool verbose=true);

  //!
  //! Get the bool value of the given parameter named key.
  //!
  bool valueBool(const char *  key)   const
  {
  return _configuration.valueBool(_configurationPath,key);
  }

  //!
  //! Get the int value of the given parameter named key.
  //!
  int valueInt(const char *  key)    const
  {
  return _configuration.valueInt(_configurationPath,key);
  }

  //!
  //! Get the long value of the given parameter named key.
  //!
  long valueLong(const char *  key)    const
  {
  return _configuration.valueLong(_configurationPath,key);
  }

  //!
  //! Get the double value of the given parameter named key.
  //!
  double valueDouble(const char *  key) const
  {
  return _configuration.valueDouble(_configurationPath,key);
  }

  //!
  //! Get the String value of the given parameter named key.
  //!
  String valueString(const char *  key) const
  {
  return _configuration.valueString(_configurationPath,key);
  }

  //!
  //! Get the bool value of the given parameter named key.
  //!
  bool valueBool(const char * path, const char *  key)   const
  {
  return _configuration.valueBool(path,key);
  }

  //!
  //! Get the int value of the given parameter named key at the given path.
  //!
  int valueInt(const char * path, const char *  key)    const
  {
  return _configuration.valueInt(path,key);
  }

  //!
  //! Get the long value of the given parameter named key at the given path.
  //!
  long valueLong(const char * path, const char *  key)    const
  {
  return _configuration.valueLong(path,key);
  }

  //!
  //! Get the double value of the given parameter named key at the given path.
  //!
  double valueDouble(const char * path, const char *  key) const
  {
  return _configuration.valueDouble(path,key);
  }

  //!
  //! Get the String value of the given parameter named key at the given path.
  //!
  String valueString(const char * path, const char *  key) const
  {
  return _configuration.valueString(path,key);
  }

  //!
  //! Add a property with the given name and value
  //!
  void addProperty(const char *  name, bool value)
  {
  _configuration.addProperty(_configurationPath,name,value);
  }

  //!
  //! Add a property with the given name and value
  //!
  void addProperty(const char *  name, int value)
  {
  _configuration.addProperty(_configurationPath,name,value);
  }

  //!
  //! Add a property with the given name and value
  //!
  void addProperty(const char *  name, long value)
  {
  _configuration.addProperty(_configurationPath,name,value);
  }

  //!
  //! Add a property with the given name and value
  //!
  void addProperty(const char *  name, double value)
  {
  _configuration.addProperty(_configurationPath,name,value);
  }

  //!
  //! Add a property with the given name and value
  //!
  void addProperty(const char *  name, const char *  value)
  {
  _configuration.addProperty(_configurationPath,name,value);
  }

  //!
  //! Add a property with the given name and value
  //!
  void addProperty(const char * path, const char * name, bool value)
  {
  _configuration.addProperty(path,name,value);
  }

  //!
  //! Add a property with the given name and value
  //!
  void addProperty(const char * path, const char * name, int value)
  {
  _configuration.addProperty(path,name,value);
  }

  //!
  //! Add a property with the given name and value
  //!
  void addProperty(const char * path, const char * name, long value)
  {
  _configuration.addProperty(path,name,value);
  }

  //!
  //! Add a property with the given name and value
  //!
  void addProperty(const char * path, const char * name, double value)
  {
  _configuration.addProperty(path,name,value);
  }

  //!
  //! Add a property with the given name and value
  //!
  void addProperty(const char * path, const char * name, const char * value)
  {
  _configuration.addProperty(path,name,value);
  }

  //!
  //! print the properties held by this configuration manager.
  //!
  virtual void print() const;

  void printProperty(const char * keyword) const;

  //!
  //!Generates and stores in the configuration of this task a list of key,value properties based on the given properties.value
  //!The keys generated have the form keyBaseName## where keyBaseName is the base name of the keys and ## is an integer k
  //!from 0 to nKeysToGenerate-1 (inclusively)
  //!
  //!@param keyBaseName base name of the keys
  //!@param defaultValue default value given for all key,value pairs
  //!@param nKeysToGenerate number of key,value pairs to generate and add to this task configuration.
  //!
  void generateKeyValuePairs(const char *  keyBaseName, const char *  defaultValue, int nKeysToGenerate);

  void generateKeyValuePairs(const char *  path, const char *  keyBaseName, const char *  defaultValue, int nKeysToGenerate);

  //!
  //!Parse the configuration associated with task to find the key,value pairs that DO NOT feature the defaultValue
  //!
  //!@param keyBaseName base name of the keys
  //!@param defaultValue default value that is selected against
  //!
  std::vector<String>  selectedValues(const char *  keyBaseName, const char *  defaultValue) const
  {  return _configuration.selectedValues(_configurationPath,keyBaseName,defaultValue); }

  std::vector<String>  selectedValues(const char *  path, const char *  keyBaseName, const char *  defaultValue) const
  { return _configuration.selectedValues(path,keyBaseName,defaultValue);}

  //!
  //!Get  the number of key,value pairs with the given key base name  that DO NOT feature the defaultValue
  //!
  //!@param keyBaseName base name of the keys to be tested against.
  //!@param defaultValue default value that is selected against
  //!
  int nSelectedValues(const char *  keyBaseName, const char *  defaultValue)  const {return _configuration.nSelectedValues(_configurationPath,keyBaseName,defaultValue); }
  int nSelectedValues(const char *  path, const char *  keyBaseName, const char *  defaultValue) const {  return _configuration.nSelectedValues(path,keyBaseName,defaultValue); }
  void setConfigurationPath(const String & configurationPath) {  _configurationPath = configurationPath; }
  const String & configurationPath() const  {  return _configurationPath; }

protected:

  Configuration _configuration;
  String _configurationPath;
  ClassDef(ConfigurationManager,0)
};

} // namespace CAP

#endif /* ConfigurationManager_hpp */
