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
#ifndef CAP__MessageLogger
#define CAP__MessageLogger
#include <iostream>
#include "Aliases.hpp"
#include "Object.hpp"
#include "Exceptions.hpp"

namespace CAP
{
  
  //!
  //! Message logger class used  by several  classes and subclasses of this package.
  //!
  class MessageLogger
  :
  public Object
  {
  public:
  
  enum Severity
  {
  kUnknown,
  kTrace,
  kEnd,
  kStart,
  kDebug0,
  kDebug1,
  kDebug2,
  kInfo,
  kWarning,
  kError,
  kFatal
  };
  
  protected:
  
  Severity  _minimumReportLevel;      //!< Minimal error level reported by object instances inheriting from this class.
  Severity  _holdMinimumReportLevel;  //!< Internal variable used internally to set/reset the nominal report level selected by the user.
  String    _className;          //!< name of the class of this object instance
  unsigned int _loggerStyle;
  std::ostream * _os;

  public:
  
  static String kUnknownName;
  static String kTraceName;
  static String kStartName;
  static String kEndName;
  static String kDebug0Name;
  static String kDebug1Name;
  static String kDebug2Name;
  static String kInfoName;
  static String kWarningName;
  static String kErrorName;
  static String kFatalName;
  static void setSeverityName(Severity level, const String name);
  
  public:
  
  MessageLogger();
  MessageLogger(const MessageLogger & logger);
  MessageLogger operator=(const MessageLogger & logger);
  virtual ~MessageLogger() { }
  
  //!
  //! Set the desired reported level used by instances (objects) of this class or subclasses.
  //!
  inline void setMinimumReportLevel(Severity minimumReportLevel)
  {
  _minimumReportLevel = minimumReportLevel;
  }
  
  void setMinimumReportLevel(const String & severityName);
  
  //!
  //! Get the desired reported level used by instances (objects) of this class or subclasses.
  //!
  inline Severity getMinimumReportLevel() const
  {
  return _minimumReportLevel;
  }
  
  inline String  getMinimumReportName() const
  {
  return severityName(_minimumReportLevel);
  }
  
  inline String  severityName(Severity level) const
  {
  String s;
  switch (level)
    {
      case kFatal:    s = kFatalName;   break;
      case kError:    s = kErrorName;   break;
      case kWarning:  s = kWarningName; break;
      case kInfo:     s = kInfoName;    break;
      case kDebug2:   s = kDebug2Name;  break;
      case kDebug1:   s = kDebug1Name;  break;
      case kDebug0:   s = kDebug0Name;  break;
      case kStart:    s = kStartName;   break;
      case kEnd:      s = kEndName;     break;
      case kTrace:    s = kTraceName;   break;
      default:
      case kUnknown:  s = kUnknownName; break;
    }
  return s;
  }
  
  //!
  //! Set the hold level .
  //!
  inline void holdSeverityLevel(Severity requiredLevel)
  {
  _holdMinimumReportLevel = _minimumReportLevel;
  _minimumReportLevel     = requiredLevel;
  }
  
  //!
  //!Restore the nominal report level requested by the user.
  //!
  inline void restoreSeverityLevel()
  {
  _minimumReportLevel = _holdMinimumReportLevel;
  }
  
  inline void setStyle(unsigned int s)
  {
  _loggerStyle = s;
  }
  
  inline unsigned int  getStyle()
  {
  return _loggerStyle;
  }
  
  //!
  //!Append the given string to the class name sequence of this object instance.
  //!
  inline void appendClassName(const String & className)
  {
  _className += "/";
  _className += className;
  }

  //!
  //!Issue a debug message if the severity level is not below Trace.
  //!
  bool reportTrace(const String & fctName) const
  {
  return report(kTrace,fctName);
  }
  
  bool reportEnd(const String & fctName) const
  {
  return report(kEnd,fctName);
  }
  
  bool reportStart(const String & fctName) const
  {
  return report(kStart,fctName);
  }
  
  inline bool reportDebug0(const String & fctName) const
  {
  return report(kDebug0,fctName);
  }
  
  inline bool reportDebug1(const String & fctName) const
  {
  return report(kDebug1,fctName);
  }
  
  inline bool reportDebug2(const String & fctName) const
  {
  return report(kDebug2,fctName);
  }
  
  inline bool reportDebug(const String & fctName) const
  {
  return report(kDebug0,fctName);
  }
  
  inline bool reportInfo(const String & fctName) const
  {
  return report(kInfo,fctName);
  }
  
  bool reportWarning(const String & fctName) const
  {
  return report(kWarning,fctName);
  }
  
  bool reportError(const String & fctName) const
  {
  return report(kError,fctName);
  }
  
  bool reportFatal(const String & fctName) const
  {
  return report(kFatal,fctName);
  }
  
  bool report(Severity severity, const String & fctName) const;
  
  ClassDef(MessageLogger,0)
  };
  
} // namespace CAP

#endif /* CAP__MessageLogger */
