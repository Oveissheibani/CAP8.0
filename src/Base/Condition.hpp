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
#ifndef CAP__Condition
#define CAP__Condition
#include "Aliases.hpp"
#include "Exceptions.hpp"

using CAP::String;

namespace CAP
{

class Condition
{
protected:

  int  _type;
  int  _subtype;

public:

  enum ConditionType   { kCondition, kConditionBool, kConditionInteger, kConditionIntegerRange, kConditionDoubleRange, kConditionDoubleDualRange};

  Condition(int type, int subtype);
  Condition(const Condition & source);
  virtual ~Condition() {}
  const Condition & operator=(const Condition & source);
  virtual int type() const { return _type; }
  virtual int subtype() const  { return _subtype; }
  virtual String typeName() const  {  return String("ConditionDoubleDualRange");  }
  //virtual String subtypeName() const;
  virtual bool accept(bool value);
  virtual bool accept(int  value);
  virtual bool accept(double  value);
  virtual void print() const;
};

class ConditionBool : public Condition
{
protected:

  bool _requested;

public:

  ConditionBool(int subtype,
                bool requestedValue);
  ConditionBool(const ConditionBool & source);
  virtual ~ConditionBool() {};
  const ConditionBool & operator=(const ConditionBool & source);
  virtual String typeName() const { return String("ConditionBool");}
  virtual bool accept(bool value) { return (value == _requested);  }
  virtual void print() const;
};

class ConditionInteger : public Condition
{
protected:

  int _requested;

public:

  ConditionInteger(int subtype,
                   int requestedValue);
  ConditionInteger(const ConditionInteger & source);
  virtual ~ConditionInteger() {}
  const ConditionInteger & operator=(const ConditionInteger & source);
  virtual String typeName() const { return String("ConditionInteger");  }
  virtual bool accept(int value) {  return (_requested == value); }
  virtual void print() const;
};

class ConditionIntegerRange : public Condition
{
public:

  int _minimum;
  int _maximum;

  ConditionIntegerRange(int subtype,
                        int minimum,
                        int maximum);
  ConditionIntegerRange(const ConditionIntegerRange & source);
  virtual ~ConditionIntegerRange() {}
  const ConditionIntegerRange & operator=(const ConditionIntegerRange & source);
  virtual String conditionTypeName() const {  return String("ConditionIntegerRange");  }
  virtual bool accept(int value) {  return (value>=_minimum  &&  value<=_maximum);  }
  virtual void print() const;
};

class ConditionDoubleRange : public Condition
{
protected:

  double _minimum;
  double _maximum;

public:

  ConditionDoubleRange(int subtype,
                       double minimum,
                       double maximum);
  ConditionDoubleRange(const ConditionDoubleRange & source);
  virtual ~ConditionDoubleRange() {}
  const ConditionDoubleRange & operator=(const ConditionDoubleRange & source);
  virtual String conditionTypeName() const  { return String("ConditionDoubleRange");  }
  virtual bool accept(double value)  {  return (value>=_minimum  &&  value<_maximum);  }
  virtual void print() const;
};

class ConditionDoubleDualRange : public ConditionDoubleRange
{
public:

  double _minimum2;
  double _maximum2;

  ConditionDoubleDualRange(int subtype,
                           double minimum,
                           double maximum,
                           double minimum2,
                           double maximum2);
  ConditionDoubleDualRange(const ConditionDoubleDualRange & source);
  virtual ~ConditionDoubleDualRange() {}
  const ConditionDoubleDualRange & operator=(const ConditionDoubleDualRange & source);
  virtual String conditionTypeName() const { return String("ConditionDoubleDualRange"); }
  virtual bool accept(double value) { return (value>=_minimum  &&  value<_maximum) || (value>=_minimum2  &&  value<_maximum2); }
  virtual void print() const;
};

#endif /* CAP__Filter  */


} // namespace CAP


