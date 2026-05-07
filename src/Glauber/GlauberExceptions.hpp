#ifndef CAP_GlauberExceptions
#define CAP_GlauberExceptions
#include "Exceptions.hpp"

namespace CAP
{
  
  class GlauberException : public Exception
  {
  public:
  GlauberException(const String & message,
                   const String & source,
                   const String & ec="GlauberException");
  virtual ~GlauberException() {}
  };
  
  class InvalidValueGlauberException  : public GlauberException
  {
  public:
  InvalidValueGlauberException(double value,
                               const String & message,
                               const String & source,
                               const String & ec="InvalidValueGlauberException");
  virtual ~InvalidValueGlauberException() {}
  virtual void print() const;
  protected:
  double _value;
  };
  
  class NucleusNotFoundGlauberException  : public GlauberException
  {
  public:
  NucleusNotFoundGlauberException(const String name,
                                  const String & message,
                                  const String & source,
                                  const String & ec="NucleusNotFoundGlauberException");
  virtual ~NucleusNotFoundGlauberException() {}
  virtual void print() const;
  protected:
  String _name;
  };
  
  class RadialTypeGlauberException  : public GlauberException
  {
  public:
  RadialTypeGlauberException(int type,
                             const String & message,
                             const String & source,
                             const String & ec="RadialTypeGlauberException");
  virtual ~RadialTypeGlauberException() {}
  virtual void print() const;
  protected:
  int _type;
  };
  
  class IterationsGlauberException  : public GlauberException
  {
  public:
  IterationsGlauberException(int maxIterations,
                             int iterations,
                             const String & message,
                             const String & source,
                             const String & ec="OverlapGlauberException");
  virtual ~IterationsGlauberException() {}
  virtual void print() const;
  
  protected:
  int _maxIterations;
  int _iterations;
  };
  
} // namespace CAP

#endif
