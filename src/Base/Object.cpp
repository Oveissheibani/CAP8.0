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
#include "Object.hpp"
#include "PrintHelpers.hpp"
#include <iomanip>

using std::cout;
using std::endl;
using std::left;
using std::setw;
using std::setfill;

ClassImp(CAP::Object);

namespace CAP
{

  String Object::kUnknownName   = "UNKNOWN";
  String Object::kTraceName     = "TRACE";;
  String Object::kEndName       = "ENDF";
  String Object::kStartName     = "STARTF";
  String Object::kDebug0Name    = "DEBUG0";
  String Object::kDebug1Name    = "DEBUG1";
  String Object::kDebug2Name    = "DEBUG2";
  String Object::kInfoName      = "INFO";
  String Object::kWarningName   = "WARNING";
  String Object::kErrorName     = "ERROR";
  String Object::kFatalName     = "FATAL";

  Object::Object()
  :
  _name("Object"),
  _title("Object"),
  _className("Object"),
  _minimumReportLevel(kInfo),
  _holdMinimumReportLevel(kInfo),
  _loggerStyle(0)
  { /* no ops */}


  Object::Object(const String & name, const String & title, const String & className)
  :
  _name(name),
  _title(title),
  _className(className)
  {}

  Object::Object(const Object & source)
  :
  _name(source._name),
  _title(source._title),
  _className(source._className),
  _minimumReportLevel(source._minimumReportLevel),
  _holdMinimumReportLevel(source._holdMinimumReportLevel),
  _loggerStyle(source._loggerStyle)
  { /* no ops */}

  Object Object::operator=(const Object & rhs)
  {
  if (this!=&rhs)
    {
    _name = rhs._name;
    _title = rhs._title;
    _className = rhs._className;
    _minimumReportLevel = rhs._minimumReportLevel;
    _holdMinimumReportLevel = rhs._holdMinimumReportLevel;
    _loggerStyle = rhs._loggerStyle;
    }
  return *this;
  }

  Object::~Object()
  {
  clear();
  }

  bool Object::nameHasEndColon() const
  {
  int last = _name.Last(':');
  int size = _name.Length() - 1;
  return (last>0  &&  last == size);
  }

  void Object::clear()
  {
  _name  = "";
  _title = "";
  _className ="";
  _minimumReportLevel  = kInfo;
  _holdMinimumReportLevel = kInfo;
  _loggerStyle = 0;
  }

  void Object::print()  const
  {
  printValue("Name",_name);
  printValue("Title",_title);
  printValue("Class",_className);
  }

  bool Object::report(Severity severity, const String & fctName) const
  {
  if (severity<_minimumReportLevel) return false;
  cout << "<" << severityName(severity) << "> " << _className << "[" << name() << "]::" << fctName << "() ";
  return true;
  }

  void Object::setSeverityName(Severity level, const String name)
  {
  switch (level)
    {
      case kUnknown:
      case kTrace:   kTraceName   = name; break;
      case kEnd:     kEndName     = name; break;
      case kStart:   kStartName   = name; break;
      case kDebug0:  kDebug0Name  = name; break;
      case kDebug1:  kDebug1Name  = name; break;
      case kDebug2:  kDebug2Name  = name; break;
      case kInfo:    kInfoName    = name; break;
      case kWarning: kWarningName = name; break;
      case kError:   kErrorName   = name; break;
      case kFatal:   kFatalName   = name; break;
    }
  }

  void Object::setMinimumReportLevel(const String & severityName)
  {
  if (severityName.EqualTo(kTraceName))
    setMinimumReportLevel(kTrace);
  else if (severityName.EqualTo(kEndName))
    setMinimumReportLevel(kEnd);
  else if (severityName.EqualTo(kStartName))
    setMinimumReportLevel(kStart);
  else if (severityName.EqualTo(kEndName))
    setMinimumReportLevel(kEnd);
  else if (severityName.EqualTo(kDebug0Name))
    setMinimumReportLevel(kDebug0);
  else if (severityName.EqualTo(kDebug1Name))
    setMinimumReportLevel(kDebug1);
  else if (severityName.EqualTo(kDebug2Name))
    setMinimumReportLevel(kDebug2);
  else if (severityName.EqualTo(kInfoName))
    setMinimumReportLevel(kInfo);
  else if (severityName.EqualTo(kWarningName))
    setMinimumReportLevel(kWarning);
  else if (severityName.EqualTo(kErrorName))
    setMinimumReportLevel(kError);
  else if (severityName.EqualTo(kFatalName))
    setMinimumReportLevel(kFatal);
  }



} // namespace CAP
