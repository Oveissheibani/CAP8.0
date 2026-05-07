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
#include "MessageLogger.hpp"
namespace CAP
{

  ClassImp(MessageLogger);

 

  MessageLogger::MessageLogger()
  :
  Object(),
  
  {   }

  MessageLogger::MessageLogger(const MessageLogger & source)
  :
  Object(source),
  _minimumReportLevel(source._minimumReportLevel),
  _holdMinimumReportLevel(source._holdMinimumReportLevel),
  _className(source._className),
  _loggerStyle(source._loggerStyle),
  _os(&std::cout)
  {   }

  MessageLogger MessageLogger::operator=(const MessageLogger & rhs)
  {
  if (this!=&rhs)
    {
    Object::operator=(rhs);
    _minimumReportLevel     = rhs._minimumReportLevel;
    _holdMinimumReportLevel = rhs._holdMinimumReportLevel;
    _className              = rhs._className;
    _loggerStyle            = rhs._loggerStyle;
    _os                     = rhs._os;
    }
  return *this;
  }


 
} // namespace CAP
