#include "GlauberExceptions.hpp"
#include "PrintHelpers.hpp"

namespace CAP
{


GlauberException::GlauberException(const String & message,
                                   const String & source,
                                   const String & ec)
:
Exception(message,source,ec)
{   }

InvalidValueGlauberException::InvalidValueGlauberException(double value,
                                                           const String & message,
                                                           const String & source,
                                                           const String & ec)
:
GlauberException(message,source,ec),
_value(value)
{   }

void InvalidValueGlauberException::print() const
{
  printCR();
  printLine();
  printValue("Exception Type",_exception);
  printLine();
  printValue("Source",_source);
  printValue("Message",_message);
  printValue("Value",_value);
  printLine();
}

NucleusNotFoundGlauberException::NucleusNotFoundGlauberException(const String name,
                                                                 const String & message,
                                                                 const String & source,
                                                                 const String & ec)
:
GlauberException(message,source,ec),
_name(name)
{   }

void NucleusNotFoundGlauberException::print() const
{
  printCR();
  printLine();
  printValue("Exception Type",_exception);
  printLine();
  printValue("Source",_source);
  printValue("Message",_message);
  printValue("Name",_name);
  printLine();
}

RadialTypeGlauberException::RadialTypeGlauberException(int  type,
                                                       const String & message,
                                                       const String & source,
                                                       const String & ec)
:
GlauberException(message,source,ec),
_type(type)
{   }

void RadialTypeGlauberException::print() const
{
  printCR();
  printLine();
  printValue("Exception Type",_exception);
  printLine();
  printValue("Source",_source);
  printValue("Message",_message);
  printValue("Type",_type);
  printLine();
}


IterationsGlauberException::IterationsGlauberException(int maxIterations,
                                                       int iterations,
                                                       const String & message,
                                                       const String & source,
                                                       const String & ec)
:
GlauberException(message,source,ec),
_maxIterations(maxIterations),
_iterations(iterations)
{   }

void IterationsGlauberException::print() const 
{
  printCR();
  printLine();
  printValue("Exception Type",_exception);
  printLine();
  printValue("Source",_source);
  printValue("Message",_message);
  printValue("MaxIterations",_maxIterations);
  printValue("Iterations",_iterations);
  printLine();
}

} // namespace CAP

