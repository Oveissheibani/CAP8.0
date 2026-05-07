#include "MathExceptions.hpp"
#include "PrintHelpers.hpp"


namespace CAP
{
  namespace MATH
  {

  MathException::MathException(const String & message, const String & source)
  :
  Exception(message,source, __FUNCTION__)
  {   }

  MathDomainException::MathDomainException(double min, double max, double x, const String & message, const String & source)
  :
  Exception(message,source,__FUNCTION__),
  _min(min),
  _max(max),
  _x(x)
  { }

  void MathDomainException::print() const
  {
  printCR();
  printLine();
  printValue("Exception Type",_exception);
  printValue("Source",_source);
  printValue("Message",_message);
  printValue("Domain Minimum",_min);
  printValue("Domain Maximum",_max);
  printValue("Value ",_x);
  printLine();
  }

  DivByZeroException::DivByZeroException(const String varName, const String & message, const String & source)
  :
  Exception(message,source,__FUNCTION__),
  _varName(varName)
  { }

  void DivByZeroException::print() const
  {
  printCR();
  printLine();
  printValue("Exception Type",_exception);
  printValue("Source",_source);
  printValue("Message",_message);
  printValue("Variable Name",_varName);
  printLine();
  }

  InvalidBoundsException::InvalidBoundsException(double min, double max, const String & message, const String & source)
  :
  Exception(message,source,__FUNCTION__),
  _min(min),
  _max(max)
  { }

  void InvalidBoundsException::print() const
  {
  printCR();
  printLine();
  printValue("Exception Type",_exception);
  printValue("Source",_source);
  printValue("Message",_message);
  printValue("Minimum",_min);
  printValue("Maximum",_max);
  printLine();
  }


  } // namespace MATH
} // namespace CAP

