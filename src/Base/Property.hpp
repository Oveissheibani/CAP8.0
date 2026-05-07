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
#ifndef CAP__Property
#define CAP__Property
#include "Object.hpp"

namespace CAP
{

class PropertyException : public Exception
{
public:
  PropertyException(const String & name, const String & message, const String & source);
  virtual ~PropertyException()  {}
  virtual void print() const;
protected:
  const String  _keyword;
};


//!
//! A property features three elements
//! name: a name or key to refer to the object
//! title: a readable name or title for the object
//! value: the value of the property
//!
//! The value is stored as String but can be converted on input and output into other types of data
//!
class Property : public Object
{
public:
  Property();
  Property(const char * name, const char * value);
  Property(const char * name, const String & value);
  Property(const char * name, bool value);
  Property(const char * name, int value);
  Property(const char * name, long value);
  Property(const char * name, double value);
  
  Property(const char * name, const char * title, const char * value);
  Property(const char * name, const char * title, const String & value);
  Property(const char * name, const char * title, bool value);
  Property(const char * name, const char * title, int value);
  Property(const char * name, const char * title, long value);
  Property(const char * name, const char * title, double value);
  
  Property(const Property & property);
  Property operator=(const Property & property);
  virtual ~Property(){}
  
  virtual void setValue(const String & title);
  virtual void setValue(const char * value);
  const String & value() const;
  
  virtual bool valueContains(const char * aKeyword) const;
  virtual bool isEqualTo(const Property & rhs, int option=1) const;

  virtual void set(const String & name,const String & title,const String & value="");
  virtual void set(const String & name,const String & title,bool value);
  virtual void set(const String & name,const String & title,int value);
  virtual void set(const String & name,const String & title,long value);
  virtual void set(const String & name,const String & title,double value);

  virtual void set(const char * name, const char * value);
  virtual void set(const char * name, const String & value);
  virtual void set(const char * name, bool value);
  virtual void set(const char * name, int value);
  virtual void set(const char * name, long value);
  virtual void set(const char * name, double value);

  virtual void setValue(bool value);
  virtual void setValue(int value);
  virtual void setValue(long value);
  virtual void setValue(double value);


  bool isBool() const;
  bool isInt() const;
  bool isLong() const;
  bool isDouble() const;
  bool isString() const;
  bool getBool() const;
  int  integer() const;
  long longInteger() const;
  double doubleValue() const;
  const String string() const;
  const String upperString() const;
  const String lowerString() const;
  bool nameHasEndColon() const;
  void print() const;
  virtual void saveTo(std::ostream & os) const;

  virtual void reset();
  virtual void clear();
  virtual void erase();

protected:
  String _value;

  ClassDef(Property,0)

};

} // namespace CAP

#endif /* CAP_PropertyGroup */


