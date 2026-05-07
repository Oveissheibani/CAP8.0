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
#include "Property.hpp"
#include "PrintHelpers.hpp"

ClassImp(CAP::Property);

namespace CAP
{

PropertyException::PropertyException(const String & keyword, const String & message, const String & source)
:
Exception(message,source,__FUNCTION__),
_keyword(keyword)
{   }

void PropertyException::print() const
{
  printCR();
  printLine();
  printValue("Exception Type",_exception);
  printLine();
  printValue("Source",_source);
  printValue("Message",_message);
  printValue("Keyword",_keyword);
  printLine();
}

Property::Property()
:
Object(),
_value()
{ /* no ops */}

Property::Property(const char * name, const char * value)
:
Object(name,"","Property"),
_value(value)
{}

Property::Property(const char * name, const String & value)
:
Object(name,"","Property"),
_value(value)
{}

Property::Property(const char * name, bool value)
:
Object(name,"","Property"),
_value()
{
  setValue(value);
}

Property::Property(const char * name, int value)
:
Object(name,"","Property"),
_value()
{
  setValue(value);
}

Property::Property(const char * name, long value)
:
Object(name,"","Property"),
_value()
{
  setValue(value);
}

Property::Property(const char * name, double value)
:
Object(name,"","Property"),
_value()
{
  setValue(value);
}

Property::Property(const char * name, const char * title, const char * value)
:
Object(name,title,"Property"),
_value(value)
{
  setValue(value);
}

Property::Property(const char * name, const char * title, const String & value)
:
Object(name,title,"Property"),
_value(value)
{ }

Property::Property(const char * name, const char * title, bool value)
:
Object(name,title,"Property"),
_value()
{
  setValue(value);
}

Property::Property(const char * name, const char * title, int value)
:
Object(name,title,"Property"),
_value()
{
  setValue(value);
}

Property::Property(const char * name, const char * title, long value)
:
Object(name,title,"Property"),
_value()
{
  setValue(value);
}

Property::Property(const char * name, const char * title, double value)
:
Object(name,title,"Property"),
_value()
{
  setValue(value);
}

Property::Property(const Property & property)
:
Object(property),
_value(property._value)
{ }

Property Property::operator=(const Property & property)
{
  if (this!=&property)
    {
    Object::operator=(property);
    _value = property._value;
    }
  return *this;
}

void Property::setValue(const String & value)
{
  _value = value;
}

void Property::setValue(const char *  value)
{
  _value = value;
}

const String & Property::value() const
{
  return _value;
}

bool Property::valueContains(const char * keyword) const
{
  return _value.Contains(keyword);
}

//!
//!Determines whethe this and the given object are equal
//!
//!option determine how stuct the equality is.
//!option=0 : same name
//!option=1 : same name and same title
//!
bool Property::isEqualTo(const Property & rhs, int option) const
{
  switch (option)
    {
      case 0: return _name.EqualTo(rhs._name);
      case 1: return _name.EqualTo(rhs._name)  &&  _value.EqualTo(rhs._value);
      case 2: return _name.EqualTo(rhs._name)  &&  _title.EqualTo(rhs._title)  &&  _value.EqualTo(rhs._value);
    }
  return false; // should never get here...
}

void Property::set(const char * name, const char * value)
{
  _name = name;
  _title = name;
  _value  = value;
}

void Property::set(const char * name, const String & value)
{
  _name = name;
  _title = name;
  _value  = value;
}

void Property::set(const char * name, bool value)
{
  _name = name;
  _title = name;
  setValue(value);
}

void Property::set(const char * name, int value)
{
  _name = name;
  _title = name;
  setValue(value);
}

void Property::set(const char * name, long value)
{
  _name = name;
  _title = name;
  setValue(value);
}

void Property::set(const char * name, double value)
{
  _name = name;
  _title = name;
  setValue(value);
}

void Property::set(const String & name, const String & title, const String & value)
{
  _name = name;
  _title = title;
  _value  = value;
}

void Property::set(const String & name, const String & title, bool value)
{
  _name = name;
  _title = title;
  setValue(value);
}

void Property::set(const String & name, const String & title, int  value)
{
  _name = name;
  _title = title;
  setValue(value);
}

void Property::set(const String & name, const String & title, long value)
{
  _name = name;
  _title = title;
  setValue(value);
}

void Property::set(const String & name, const String & title, double value)
{
  _name = name;
  _title = title;
  setValue(value);
}

void Property::reset()
{
  _name  = "";
  _title = "";
  _value = "";
}

void Property::clear()
{
  _name  = "";
  _title = "";
  _value = "";
}

void Property::erase()
{
  _name  = "";
  _title = "";
  _value = "";
}

void Property::setValue(bool value)
{
  _value = "";
  _value += value;
}

void Property::setValue(int value)
{
  _value  = "";
  _value  += value;
}

void Property::setValue(long value)
{
  _value  = "";
  _value  += value;
}

void Property::setValue(double value)
{
  _value  = "";
  _value  += value;
}

// name & value

//void Property::set(const char * name, const char * value)
//{
//  name    = name;
//  title   = name;
//  value   = "";
//  value   += value;
//}
//
//void Property::set(const char * name, const String & value)
//{
//  name    = name;
//  title   = name;
//  value   = "";
//  value   += value;
//}
//
//void Property::set(const char * name, bool value)
//{
//  name    = name;
//  title   = name;
//  value   = "";
//  value   += value;
//}
//
//void Property::set(const char * name, int value)
//{
//  name    = name;
//  title   = name;
//  value   = "";
//  value   += value;
//}
//
//void Property::set(const char * name, long value)
//{
//  name    = name;
//  title   = name;
//  value   = "";
//  value   += value;
//}
//
//void Property::set(const char * name, double value)
//{
//  name    = name;
//  title   = name;
//  value   = "";
//  value   += value;
//}

// extract bool, int, long, double
// v.ToUpper();

bool  Property::isBool() const
{
  String v = _value;
  if (v.EqualTo("0") || v.EqualTo("FALSE") || v.EqualTo("YES") ) return true;
  else if (v.EqualTo("1") || v.EqualTo("TRUE")  || v.EqualTo("NO")  ) return true;
  return false;
}

// not quite right... should return false if integer is too large
bool Property::isInt() const
{
  if (!_value.IsDec()) return false;
  return true;
}

// not quite right...
bool Property::isLong() const
{
  if (!_value.IsDec()) return false;
  return true;
}

bool Property::isDouble() const
{
  if (_value.IsFloat()) return true;
  return false;
}

// always can be considered a string... not good?
bool Property::isString() const
{
  return true;
}

bool  Property::getBool() const
{
  String v = _value;
  v.ToUpper();
  if (v.EqualTo("0") || v.EqualTo("FALSE") || v.EqualTo("YES") ) return false;
  else if (v.EqualTo("1") || v.EqualTo("TRUE")  || v.EqualTo("NO")  ) return true;
  else if (v.IsDec())
    {
    int vv = v.Atoi();
    return vv>0;
    }
  return false;
}

int Property::integer() const
{
  String v = _value;
  if (v.IsDec()) return v.Atoi();
  return -9999;
  //throw Exception("Property value not an integer","Property::integer()");
}

long Property::longInteger() const
{
  String v = _value;
  if (!v.IsDec())
    {
    return -99999;
    }
  return v.Atoll();
}

double Property::doubleValue() const
{
  String v = _value;
  if (!v.IsFloat())
    {
    return -1.0E100;
    }
  return v.Atof();
}

const String Property::string() const
{
  return _value;
}

const String Property::upperString() const
{
  String v = _value; v.ToUpper();
  return v;
}

const String Property::lowerString() const
{
  String v = _value; v.ToLower();
  return v;
}

bool Property::nameHasEndColon() const
{
  int last = _name.Last(':');
  int size = _name.Length() - 1;
  return (last>0  &&  last == size);
}

void Property::print()  const
{
  printValue(_name,_value);
}

void Property::saveTo(std::ostream & output)  const
{
  output << _name << " : ";
  if (isBool())
    output << "BOOL";
  else if (isInt())
    output << "INTEGER";
  else if (isLong())
    output << "LONG";
  else if (isDouble())
    output << "DOUBLE";
  else if (isString())
    output << "STRING";
  else
    output << "UNKNOWN";
  output << " : " << _value << std::endl;
}

} // namespace CAP
