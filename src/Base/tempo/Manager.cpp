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
#include "Manager.hpp"
#include "PrintHelpers.hpp"

namespace CAP
{

ManagerException::ManagerException(const String  & _name, const String  & _message, const String  & _source)
:
Exception(_message,_source,"ManagerException"),
name(_name),
index(-1)
{   }

ManagerException::ManagerException(int _index, const String  & _message, const String  & _source)
:
Exception(_message,_source,"ManagerException"),
name(""),
index(_index)
{   }


void ManagerException::print()
{
  printCR();
  printLine();
  printValue("Exception Type",exception);
  printLine();
  printValue("Source",source);
  printValue("Message",message);
  printValue("Name",name);
  printValue("Index",index);
  printLine();
}

} // namespace CAP
