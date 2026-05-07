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
#include "TSystem.h"
#include "TSystemDirectory.h"
#include "TSystemFile.h"
#include "EnvironmentVariables.hpp"
#include "PrintHelpers.hpp"

ClassImp(CAP::EnvironmentVariables);

namespace CAP
{

EnvironmentVariables::EnvironmentVariables()
:
Properties()
{
  initialize();
}


void EnvironmentVariables::initialize()
{
  // fetch required env variables from the system
  // store them for easy later retrieval
  addVariable("CAP_DATABASE_PATH",      getenv("CAP_DATABASE_PATH"));
  addVariable("CAP_PROJECTS_PATH",      getenv("CAP_PROJECTS_PATH"));
  addVariable("CAP_DATA_IMPORT_PATH",   getenv("CAP_DATA_IMPORT_PATH"));
  addVariable("CAP_DATA_EXPORT_PATH",   getenv("CAP_DATA_EXPORT_PATH"));
  addVariable("CAP_HISTOS_IMPORT_PATH", getenv("CAP_HISTOS_IMPORT_PATH"));
  addVariable("CAP_HISTOS_EXPORT_PATH", getenv("CAP_HISTOS_EXPORT_PATH"));
  addVariable("CAP_CALIB_IMPORT_PATH",  getenv("CAP_CALIB_IMPORT_PATH"));
  addVariable("CAP_CALIB_EXPORT_PATH",  getenv("CAP_CALIB_EXPORT_PATH"));
}

void EnvironmentVariables::print() const
{
  printValue("CAP_DATABASE_PATH",      valueString("CAP_DATABASE_PATH"));
  printValue("CAP_PROJECTS_PATH",      valueString("CAP_PROJECTS_PATH"));
  printValue("CAP_DATA_IMPORT_PATH",   valueString("CAP_DATA_IMPORT_PATH"));
  printValue("CAP_DATA_EXPORT_PATH",   valueString("CAP_DATA_EXPORT_PATH"));
  printValue("CAP_HISTOS_IMPORT_PATH", valueString("CAP_HISTOS_IMPORT_PATH"));
  printValue("CAP_HISTOS_EXPORT_PATH", valueString("CAP_HISTOS_EXPORT_PATH"));
  printValue("CAP_CALIB_IMPORT_PATH",  valueString("CAP_CALIB_IMPORT_PATH"));
  printValue("CAP_CALIB_EXPORT_PATH",  valueString("CAP_CALIB_EXPORT_PATH"));
}

void  EnvironmentVariables::addVariable(const char* aKeyword, const char* aValue)
{
  addProperty(aKeyword, aValue);
}


String EnvironmentVariables::variable(const char* aKeyword) const
{
  return valueString(aKeyword);
}

EnvironmentVariables * EnvironmentVariables::_envVariables = nullptr;

EnvironmentVariables * EnvironmentVariables::environmentVariables()
{
  if (_envVariables==nullptr)
    {
    _envVariables = new EnvironmentVariables();
    }
  return _envVariables;
}

} // namespace CAP

