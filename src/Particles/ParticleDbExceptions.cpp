#include "ParticleDbExceptions.hpp"
#include "PrintHelpers.hpp"


namespace CAP
{

ParticleDecayException::ParticleDecayException(const String & message, const String & source)
:
Exception(message,source,__FUNCTION__),
_count(0)
{   }

ParticleDecayException::ParticleDecayException(int count, const String & message, const String & source)
:
Exception(message,source,__FUNCTION__),
_count(count)
{   }

void ParticleDecayException::print() const
{
  printCR();
  printLine();
  printValue("Exception Type",_exception);
  printLine();
  printValue("Source",_source);
  printValue("Message",_message);
  printValue("nChildren",_count);
  printLine();
}

InvalidIndexParticleTypeException::InvalidIndexParticleTypeException(int index, const String & message, const String & source)
:
Exception(message,source,__FUNCTION__),
_index(index)
{   }

void InvalidIndexParticleTypeException::print() const
{
  printCR();
  printLine();
  printValue("Exception Type",_exception);
  printLine();
  printValue("Source",_source);
  printValue("Message",_message);
  printValue("Index",_index);
  printLine();
}


UndefinedParticleTypeException::UndefinedParticleTypeException(const String & name, const String & source)
:
Exception("Name Not Found",source,__FUNCTION__),
_name(name)
{   }

void UndefinedParticleTypeException::print() const
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


NoSelectedParticleTypeException::NoSelectedParticleTypeException(const String & source)
:
Exception("ParticleDb==0 or defaultParticleDb==0",source,__FUNCTION__)
{   }

NonExistingParticleTypeException::NonExistingParticleTypeException(const String & name, const String & source)
:
Exception("Db not defined",source,__FUNCTION__),
_name(name)
{   }

void NonExistingParticleTypeException::print() const
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


ExistingParticleTypeException::ExistingParticleTypeException(const String & name, const String & source)
:
Exception("Db already defined",source,__FUNCTION__),
_name(name)
{   }

void ExistingParticleTypeException::print() const
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

NullPtrParticleTypeException::NullPtrParticleTypeException(const String & source)
:
Exception("Null pointer",source,__FUNCTION__)
{   }

UnknownParticleTypeException::UnknownParticleTypeException(const String & name, const String & source)
:
Exception("Unknown particle",source,__FUNCTION__),
_name(name)
{   }

void UnknownParticleTypeException::print() const
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

NotFoundFileParticleTypeException::NotFoundFileParticleTypeException(const String & pathName,
                                                                 const String & fileName,
                                                                 const String & source)
:
Exception("Db File Not Found",source,__FUNCTION__),
_pathName(pathName),
_fileName(fileName)
{   }

void NotFoundFileParticleTypeException::print() const
{
  printCR();
  printLine();
  printValue("Exception Type",_exception);
  printLine();
  printValue("Source",_source);
  printValue("Message",_message);
  printValue("pathName",_pathName);
  printValue("fileName",_fileName);
  printLine();
}


EmptyDbParticleDbException::EmptyDbParticleDbException(const String & dbName,
                                                       const String & source)
:
Exception("Empty ParticleDb",source,__FUNCTION__),
_dbName(dbName)
{   }

void EmptyDbParticleDbException::print() const
{
  printCR();
  printLine();
  printValue("Exception Type",_exception);
  printLine();
  printValue("Source",_source);
  printValue("Message",_message);
  printValue("dbName",_dbName);
  printLine();
}

InvalidFileParticleDbException::InvalidFileParticleDbException(const String & message,
                                                               const String & source)
:
Exception(message,source,__FUNCTION__)
{   }

void InvalidFileParticleDbException::print() const
{
  printCR();
  printLine();
  printValue("Exception Type",_exception);
  printLine();
  printValue("Source",_source);
  printValue("Message",_message);
  printLine();
}


} // namespace CAP

