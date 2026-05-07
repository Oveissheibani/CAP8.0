#ifndef CAP_MathExceptions
#define CAP_MathExceptions
#include "Exceptions.hpp"
namespace CAP
{
  namespace MATH
  {

  class MathException : public Exception
  {
  public:
  MathException(const String & message, const String & source);
  virtual ~MathException() {}
  };

  class MathDomainException : public Exception
  {
  public:
  MathDomainException(double min, double max, double x, const String & message, const String & source);
  virtual ~MathDomainException() {}
  virtual void print() const;

  protected:
  double _min;
  double _max;
  double _x;
  };

  class DivByZeroException : public Exception
  {
  public:
  DivByZeroException(const String varName, const String & message, const String & source);
  virtual ~DivByZeroException() {}
  virtual void print() const;

  protected:
  const String  _varName;
  };

  class InvalidBoundsException : public Exception
  {
  public:
  InvalidBoundsException(double min, double max, const String & message, const String & source);
  virtual ~InvalidBoundsException() {}
  virtual void print() const;

  protected:
  double  _min;
  double  _max;
  };




  // MathIterationException

  } // namespace MATH
} // namespace CAP

#endif
