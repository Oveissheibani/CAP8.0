#include "Exceptions.hpp"
#include "PrintHelpers.hpp"
#include "NameHelpers.hpp"

namespace CAP
{


  Exception::Exception(const String & message,
                       const String & source,
                       const String & exceptionClass)
  :
  _exception(exceptionClass),
  _source(source),
  _message(message)
  {   }

  void Exception::print() const
  {
  printCR();
  printLine();
  printValue("Exception Type",_exception);
  printValue("Source",_source);
  printValue("Message",_message);
  printLine();
  }

  NullPointerException::NullPointerException(const String & source)
  :
  Exception("Null Pointer",source,__FUNCTION__)
  {   }

  NullPointerException::NullPointerException(const String & message, const String & source)
  :
  Exception(message,source,__FUNCTION__)
  {   }


  IndexOutOfBoundException::IndexOutOfBoundException(int index, int bound, const String & message, const String & source)
  :
  Exception(message,source,__FUNCTION__),
  _index(index),
  _bound(bound)
  {   }

  void IndexOutOfBoundException::print() const
  {
  printCR();
  printLine();
  printValue("Exception Type",_exception);
  printValue("Source",_source);
  printValue("Message",_message);
  printValue("Index requested",_index);
  printValue("Max Bound",_bound);
  printLine();
  }

  InvalidNameException::InvalidNameException(const String & name, const String & message, const String & source)
  :
  Exception(message,source,__FUNCTION__),
  _name(name)
  {   }

  void InvalidNameException::print() const
  {
  printCR();
  printLine();
  printValue("Exception Type",_exception);
  printValue("Source",_source);
  printValue("Message",_message);
  printValue("Name",_name);
  printLine();
  }


  FactoryException::FactoryException(long currentSize,
                                     long requestedSize,
                                     const String & message,
                                     const String & source)
  :
  Exception(message,source,__FUNCTION__),
  _currentSize(currentSize),
  _requestedSize(requestedSize)
  {   }

  void FactoryException::print() const
  {
  printCR();
  printLine();
  printValue("Exception Type",_exception);
  printValue("Source",_source);
  printValue("Message",_message);
  printValue("Current Size",_currentSize);
  printValue("Requested Size",_requestedSize);
  printLine();
  }


  MemoryException::MemoryException(const String & message, const String & source)
  :
  Exception(message,source,__FUNCTION__)
  {   }

  FileException::FileException(const String & fileName, const String & message, const String & source)
  :
  Exception(message,source,__FUNCTION__),
  _pathName("nil"),
  _fileName(fileName)
  {   }

  FileException::FileException(const String & pathName, const String & fileName, const String & message, const String & source)
  :
  Exception(message,source),
  _pathName(pathName),
  _fileName(fileName)
  {   }

  void FileException::print() const
  {
  printCR();
  printLine();
  printValue("Exception Type",_exception);
  printValue("Source",_source);
  printValue("Message",_message);
  printValue("Path name",_pathName);
  printValue("File name",_fileName);
  printLine();
  }

  NoImplementationException::NoImplementationException(const String & className, const String & source)
  :
  Exception("No Implementation Exception",source),
  _className(className)
  {   }

  void NoImplementationException::print() const
  {
  printCR();
  printLine();
  printValue("Exception Type",_exception);
  printValue("Source",_source);
  printValue("Message",_message);
  printValue("Class Name",_className);
  printLine();
  }


  EndOfFileException::EndOfFileException(const String & fileName, const String & message, const String & source)
  :
  FileException(fileName,message,source)
  { }

  EndOfFileException::EndOfFileException(const String & pathName, const String & fileName, const String & message, const String & source)
  :
  FileException(pathName,fileName,message,source)
  { }

  EndOfDataException::EndOfDataException(const String & fileName, const String & message, const String & source)
  :
  FileException(fileName,message,source)
  { }

  EndOfDataException::EndOfDataException(const String & pathName, const String & fileName, const String & message, const String & source)
  :
  FileException(pathName,fileName,message,source)
  { }

  ConfigurationException::ConfigurationException(const String & keyword, const String & message, const String & source)
  :
  Exception(message,source,__FUNCTION__),
  _keyword(keyword)
  {   }

  void ConfigurationException::print() const
  {
  printCR();
  printLine();
  printValue("Exception Type",_exception);
  printValue("Source",_source);
  printValue("Message",_message);
  printValue("Keyword",_keyword);
  printLine();
  }

  HistogramException::HistogramException(const String & name, const String & message, const String & source)
  :
  Exception(message,source,__FUNCTION__),
  _name(name)
  {   }

  void HistogramException::print() const
  {
  printCR();
  printLine();
  printValue("Exception Type",_exception);
  printValue("Source",_source);
  printValue("Message",_message);
  printValue("Name",_name);
  printLine();
  }


  IncompatibleHistogramException::IncompatibleHistogramException(const String & source)
  :
  Exception("Histograms have incompatible dimensions",source,__FUNCTION__)
  {   }

  IncompatibleHistogramGroupException::IncompatibleHistogramGroupException(const String & message, const String & source)
  :
  Exception(message,source,__FUNCTION__)
  {   }

  UnknownClassException::UnknownClassException(const String & name, const String & source)
  :
  Exception("Unknown Class",source,__FUNCTION__),
  _name(name)
  {   }

  void UnknownClassException::print() const
  {
  printCR();
  printLine();
  printValue("Exception Type",_exception);
  printLine();
  printValue("Source", _source);
  printValue("Message",_message);
  printValue("Name",   _name);
  printLine();
  }

  ResourceNotFoundException::ResourceNotFoundException(const String & name, const String & source)
  :
  Exception("Resource Not Found",source,__FUNCTION__),
  _name(name)
  {   }


  void ResourceNotFoundException::print() const
  {
  printCR();
  printLine();
  printValue("Exception Type",_exception);
  printLine();
  printValue("Source",_source);
  printValue("Message",_message);
  printValue("Resource",_name);
  printLine();
  }

} // namespace CAP

