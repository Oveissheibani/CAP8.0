#ifndef CAP_Exceptions
#define CAP_Exceptions
#include <iostream>
#include "Aliases.hpp"

namespace CAP
{

class Exception
{
public:
  Exception(const String & message,
            const String & source,
            const String & exceptionClass="Exception");
  virtual ~Exception() {}
  virtual void print() const;

protected:
  const String  _exception;
  const String  _source;
  const String  _message;
};

class NullPointerException : public Exception
{
public:
  NullPointerException(const String & source);
  NullPointerException(const String & message, const String & source);
  virtual ~NullPointerException() {}
};

class IndexOutOfBoundException : public Exception
{
public:
  IndexOutOfBoundException(int index, int bound, const String & message, const String & source);
  virtual ~IndexOutOfBoundException() {}
  virtual void print() const;

protected:
  int _index;
  int _bound;
};

class InvalidNameException : public Exception
{
public:
  InvalidNameException(const String & name, const String & message, const String & source);
  virtual ~InvalidNameException() {}
  virtual void print() const;

protected:
  String _name;
};


class MemoryException : public Exception
{
public:
  MemoryException(const String & message, const String & source);
  virtual ~MemoryException() {}
};

class FactoryException : public Exception
{
public:
  FactoryException(long currentSize,
                   long requestedSize,
                   const String & message,
                   const String & source);
  virtual ~FactoryException() {}
  virtual void print() const;
protected:
  long _currentSize;
  long _requestedSize;
};



class FileException : public Exception
{
public:
  FileException(const String & fileName, const String & message, const String & source);
  FileException(const String & pathName, const String & fileName, const String & message, const String & source);
  virtual ~FileException() {}
  virtual void print() const;
protected:
  const String  _pathName;
  const String  _fileName;
};


class NoImplementationException : public Exception
{
public:
  NoImplementationException(const String & className, const String & source);
  virtual ~NoImplementationException() {}
  virtual void print() const;
protected:
  const String _className;
};

class EndOfFileException : public FileException
{
public:
  EndOfFileException(const String & fileName, const String & message, const String & source);
  EndOfFileException(const String & pathName, const String & fileName, const String & message, const String & source);
  virtual ~EndOfFileException() {}
};

class EndOfDataException : public FileException
{
public:
  EndOfDataException(const String & fileName, const String & message, const String & source);
  EndOfDataException(const String & pathName, const String & fileName, const String & message, const String & source);
  virtual ~EndOfDataException() {}
};

class ConfigurationException : public Exception
{
public:
  ConfigurationException(const String & name, const String & message, const String & source);
  virtual ~ConfigurationException() {}
  virtual void print() const;
protected:
  const String  _keyword;
};

class HistogramException : public Exception
{
public:
  HistogramException(const String & histogramName, const String & message, const String & source);
  virtual ~HistogramException() {}
  virtual void print() const;
protected:
  const String  _name;
};


class IncompatibleHistogramException : public Exception
{
public:
  IncompatibleHistogramException(const String & source);
  virtual ~IncompatibleHistogramException() {}
};

class IncompatibleHistogramGroupException : public Exception
{
public:
  IncompatibleHistogramGroupException(const String & message, const String & source);
  virtual ~IncompatibleHistogramGroupException() {}
};

  class UnknownClassException : public Exception
  {
  public:
  UnknownClassException(const String & name, const String & source);
  virtual ~UnknownClassException() {}
  virtual void print() const;
  protected:
  const String  _name;
  };

  class ResourceNotFoundException : public Exception
  {
  public:
  ResourceNotFoundException(const String & name, const String & source);
  virtual ~ResourceNotFoundException() {}
  virtual void print() const;
  protected:
  const String  _name;
  };

} // namespace CAP

#endif
