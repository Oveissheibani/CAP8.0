#ifndef CAP_ParticleDbExceptions
#define CAP_ParticleDbExceptions
#include "Aliases.hpp"
#include "Exceptions.hpp"
namespace CAP
{

class ParticleDecayException : public Exception
{
public:
  ParticleDecayException(const String  & _message, const String & _source);
  ParticleDecayException(int count, const String  & _message, const String & _source);
  virtual ~ParticleDecayException() {}
  virtual void print() const;

protected:
  int _count;
};

class InvalidIndexParticleTypeException : public Exception
{
public:
  InvalidIndexParticleTypeException(int index, const String & message, const String & source);
  virtual ~InvalidIndexParticleTypeException() {}
  virtual void print() const;

protected:
  int _index;
};

class UndefinedParticleTypeException : public Exception
  {
  public:
  UndefinedParticleTypeException(const String & name, const String & _source);
  virtual ~UndefinedParticleTypeException() {}
  virtual void print() const;

  protected:
  String _name;
  };


class NoSelectedParticleTypeException : public Exception
{
public:
  NoSelectedParticleTypeException(const String & _source);
  virtual ~NoSelectedParticleTypeException() {}
};

class NonExistingParticleTypeException : public Exception
{
public:
  NonExistingParticleTypeException(const String & name, const String & _source);
  virtual ~NonExistingParticleTypeException() {}
  virtual void print() const;

protected:
  String _name;
};

class ExistingParticleTypeException : public Exception
{
public:
  ExistingParticleTypeException(const String & name, const String & _source);
  virtual ~ExistingParticleTypeException() {}
  virtual void print() const;

protected:
  String _name;
};

class NullPtrParticleTypeException : public Exception
{
public:
  NullPtrParticleTypeException(const String & _source);
  virtual ~NullPtrParticleTypeException() {}
};

class UnknownParticleTypeException : public Exception
{
public:
  UnknownParticleTypeException(const String & _name,const String & _source);
  virtual ~UnknownParticleTypeException() {}
  virtual void print() const;

protected:
  String _name;
};

class NotFoundFileParticleTypeException : public Exception
{
public:
  NotFoundFileParticleTypeException(const String & _pathName,
                                  const String & _fileName,
                                  const String & _source);
  virtual ~NotFoundFileParticleTypeException() {}
  virtual void print() const;

protected:
  String _pathName;
  String _fileName;
};

class EmptyDbParticleDbException : public Exception
{
public:
  EmptyDbParticleDbException(const String & _message,
                             const String & _source);
  virtual ~EmptyDbParticleDbException() {}
  virtual void print() const;

protected:
  String _dbName;

};

class InvalidFileParticleDbException : public Exception
{
public:
  InvalidFileParticleDbException(const String & _message,
                             const String & _source);
  virtual ~InvalidFileParticleDbException() {}
  virtual void print() const;
};


} // namespace CAP

#endif
