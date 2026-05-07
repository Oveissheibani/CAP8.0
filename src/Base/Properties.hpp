/* **********************************************************************
 * Copyright (C) 2019-2024, Claude Pruneau
 * All rights reserved.
 *
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 *
 * Author: Claude Pruneau,   04/01/2024
 *
 * *********************************************************************/
#ifndef CAP__Properties
#define CAP__Properties
#include "Aliases.hpp"
#include "Property.hpp"

namespace CAP
{
  
  class Properties
  {
  public:
  Properties();
  Properties(const Properties & source);
  virtual ~Properties();
  Properties & operator=(const Properties & rhs);
  
  const Property & property(const char* aKeyword) const;
  Property & property(const char* aKeyword);
  
  std::vector<Property> & properties() { return _properties; }
  const std::vector<Property> & properties() const  { return _properties; }
  
  virtual bool isBool(const char * name) const;
  virtual bool isInt(const char * name) const;
  virtual bool isLong(const char * name) const;
  virtual bool isDouble(const char * name) const;
  virtual bool isString(const char * name) const;
  virtual bool isFound(const char * name) const;
  
  
  virtual bool    valueBool(const char* aKeyword, bool useDefault=1, bool defaultValue=false) const;
  virtual int     valueInt(const char* aKeyword,  bool useDefault=1, int defaultValue=0) const;
  virtual long    valueLong(const char* aKeyword,  bool useDefault=1, long defaultValue=0) const;
  virtual double  valueDouble(const char* aKeyword,  bool useDefault=1, double defaultValue=0) const;
  virtual String  valueString(const char* aKeyword,  bool useDefault=1, const String defaultValue="NONE") const;
  
  String  standardize(   const char * path, const char* aKeyword) const;
  virtual bool    valueBool(const char * path, const char* aKeyword) const;
  virtual int     valueInt (const char * path, const char* aKeyword) const;
  virtual long    valueLong(const char * path, const char* aKeyword) const;
  virtual double  valueDouble(const char * path, const char* aKeyword) const;
  virtual String  valueString(const char * path, const char* aKeyword) const;
  
  virtual void addSelectedProperties(const Properties & source,
                                     const String keyword);
  //  virtual void addSelectedProperty(const Property & _property,
  //                                   const String keyword);
  
  
  virtual void addProperties(const Properties & source);
  virtual void addProperty(const Property & _property);
  virtual void addProperty(const char * name, bool value);
  virtual void addProperty(const char * name, int value);
  virtual void addProperty(const char * name, long value);
  virtual void addProperty(const char * name, double value);
  virtual void addProperty(const char * name, const char * value);
  virtual void addProperty(const char * name, const String & value);
  virtual void addProperty(const char * path, const char * name, bool value);
  virtual void addProperty(const char * path, const char * name, int value);
  virtual void addProperty(const char * path, const char * name, long value);
  virtual void addProperty(const char * path, const char * name, double value);
  virtual void addProperty(const char * path, const char * name, const char * value);
  virtual void addProperty(const char * path, const char * name, const String & value);
  
  bool hasEndColon(const char * path) const;
  
  void loadFrom(const char * _path,
                const char * _fileName,
                const char * _extension=".ini");
  void saveTo(const char * _path,
              const char * _fileName,
              const char * _extension="ini");
  void saveTo(std::ostream & os) const;
  void print() const;
  void clear();
  
  
  protected:
  
  std::vector<Property> _properties;
  
  ClassDef(Properties,0)
  
  };
  
} // namespace CAP

#endif /* Properties_hpp */
