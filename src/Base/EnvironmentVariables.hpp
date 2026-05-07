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
#ifndef CAP__EnvironmentVariables
#define CAP__EnvironmentVariables
#include "Aliases.hpp"
#include "Properties.hpp"

namespace CAP
{

//!
//!A singleton class to provide access to OS environment variables
//!
//!The constructor is protected to implement this class as singleton.
//!
class EnvironmentVariables : public Properties
{
public:

  virtual ~EnvironmentVariables() {}
  String variable(const char* aKeyword) const;
  void  addVariable(const char* aKeyword, const char* aValue);
  void print() const;

  static EnvironmentVariables * environmentVariables();

protected:

  EnvironmentVariables();
  static EnvironmentVariables * _envVariables;

  void initialize();

  ClassDef(EnvironmentVariables,0)
};

} // namespace CAP

#endif /* CAP__Task */
