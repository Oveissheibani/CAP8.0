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
#include "ConfigurationManager.hpp"
#include "StreamHelpers.hpp"
#include "PrintHelpers.hpp"
#include <iomanip>

ClassImp(CAP::ConfigurationManager);

namespace CAP
{
ConfigurationManager::ConfigurationManager()
:
_configuration(),
_configurationPath("")
{
  setDefaultConfiguration();
}

ConfigurationManager::ConfigurationManager(const ConfigurationManager & source)
:
_configuration(source._configuration),
_configurationPath(source._configurationPath)
{  }

ConfigurationManager::~ConfigurationManager()
{  }

ConfigurationManager ConfigurationManager::operator=(const ConfigurationManager & rhs)
{
  if (this!=&rhs)
    {
    _configuration     = rhs._configuration;
    _configurationPath = rhs._configurationPath;
    }
  return *this;
}


void ConfigurationManager::exportDictionary(const String & path, const String & name, bool verbose)
{
  setDefaultConfiguration();
  std::ofstream & outputFile = openOutputAsciiFile(path,name,".Dict",verbose);
  _configuration.print();
  outputFile.close();
}



void ConfigurationManager::print() const
{
  printCR();
  _configuration.print();
}

void ConfigurationManager::printProperty(const char * keyword) const
{
  const Property & property = _configuration.property(keyword);
  property.print();
}

void ConfigurationManager::generateKeyValuePairs(const char *  keyBaseName, const char *  defaultValue, int nKeysToGenerate)
{
  _configuration.generateKeyValuePairs(_configurationPath,keyBaseName,defaultValue,nKeysToGenerate);
}

void ConfigurationManager::generateKeyValuePairs(const char *  path, const char *  keyBaseName, const char *  defaultValue, int nKeysToGenerate)
{
  _configuration.generateKeyValuePairs(path,keyBaseName,defaultValue,nKeysToGenerate);
}


} // namespace CAP

