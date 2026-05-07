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
#include "Condition.hpp"
#include "PrintHelpers.hpp"

namespace CAP
{

  Condition::Condition(int type, int subtype)
  :
  _type(type),
  _subtype(subtype)
  {}

  Condition::Condition(const Condition & source)
  :
  _type(source._type),
  _subtype(source._subtype)
  {}

  const Condition & Condition::operator=(const Condition & rhs)
  {
  if (this!=&rhs)
    {
    _type = rhs._type;
    _subtype = rhs._subtype;
    }
  return *this;
  }


  bool Condition::accept(bool value __attribute__(( unused)) )
  {
  throw Exception("Bad Condition",__FUNCTION__);
  }

  bool Condition::accept(int  value __attribute__(( unused)) )
  {
  throw Exception("Improper call to Condition::accept(int)",__FUNCTION__);
  }

  bool Condition::accept(double  value __attribute__(( unused)) )
  {
  throw Exception("Improper call to Condition::accept(double)",__FUNCTION__);
  }

  void Condition::print() const
  {
  printCR();
  printValue("Type",_type);
  printValue("TypeName",typeName());
  printValue("Subtype",_subtype);
  }

  ConditionBool::ConditionBool(int subtype,
                               bool requestedValue)
  :
  Condition(kConditionBool,subtype),
  _requested(requestedValue)
  {}

  ConditionBool::ConditionBool(const ConditionBool & source)
  :
  Condition(source),
  _requested(source._requested)
  {}

  const ConditionBool & ConditionBool::operator=(const ConditionBool & rhs)
  {
  if (this!=&rhs)
    {
    Condition::operator=(rhs);
    _requested = rhs._requested;
    }
  return *this;
  }

  void ConditionBool::print() const
  {
  Condition::print();
  printValue("Request Value",_requested);
  }

  ConditionInteger::ConditionInteger(int subtype,
                                     int requestedValue)
  :
  Condition(kConditionInteger,subtype),
  _requested(requestedValue)
  {}

  ConditionInteger::ConditionInteger(const ConditionInteger & source)
  :
  Condition(source),
  _requested(source._requested)
  {}

  const ConditionInteger & ConditionInteger::operator=(const ConditionInteger & rhs)
  {
  if (this!=&rhs)
    {
    Condition::operator=(rhs);
    _requested = rhs._requested;
    }
  return *this;
  }

  void ConditionInteger::print() const
  {
  Condition::print();
  printValue("Request Value",_requested);
  }

  ConditionIntegerRange::ConditionIntegerRange(int subtype,
                                               int minimum,
                                               int maximum)
  :
  Condition(kConditionIntegerRange,subtype),
  _minimum(minimum),
  _maximum(maximum)
  {}

  ConditionIntegerRange::ConditionIntegerRange(const ConditionIntegerRange & source)
  :
  Condition(source),
  _minimum(source._minimum),
  _maximum(source._maximum)
  {}

  const ConditionIntegerRange & ConditionIntegerRange::operator=(const ConditionIntegerRange & rhs)
  {
  if (this!=&rhs)
    {
    Condition::operator=(rhs);
    _minimum = rhs._minimum;
    _maximum = rhs._maximum;
    }
  return *this;
  }

  void ConditionIntegerRange::print() const
  {
  Condition::print();
  printValue("Minimum Value",_minimum);
  printValue("Maximum Value",_maximum);
  }

  ConditionDoubleRange::ConditionDoubleRange(int subtype,
                                             double minimum,
                                             double maximum)
  :
  Condition(kConditionDoubleRange,subtype),
  _minimum(minimum),
  _maximum(maximum)
  {}

  ConditionDoubleRange::ConditionDoubleRange(const ConditionDoubleRange & source)
  :
  Condition(source),
  _minimum(source._minimum),
  _maximum(source._maximum)
  {}

  const ConditionDoubleRange & ConditionDoubleRange::operator=(const ConditionDoubleRange & rhs)
  {
  if (this!=&rhs)
    {
    Condition::operator=(rhs);
    _minimum = rhs._minimum;
    _maximum = rhs._maximum;
    }
  return *this;
  }

  void ConditionDoubleRange::print() const
  {
  Condition::print();
  printValue("Minimum Value",_minimum);
  printValue("Maximum Value",_maximum);
  }

  ConditionDoubleDualRange::ConditionDoubleDualRange(int subtype,
                                                     double minimum,
                                                     double maximum,
                                                     double minimum2,
                                                     double maximum2)
  :
  ConditionDoubleRange(subtype,minimum,maximum),
  _minimum2(minimum2),
  _maximum2(maximum2)
  {}

  ConditionDoubleDualRange::ConditionDoubleDualRange(const ConditionDoubleDualRange & source)
  :
  ConditionDoubleRange(source),
  _minimum2(source._minimum2),
  _maximum2(source._maximum2)
  {}

  const ConditionDoubleDualRange & ConditionDoubleDualRange::operator=(const ConditionDoubleDualRange & rhs)
  {
  if (this!=&rhs)
    {
    ConditionDoubleRange::operator=(rhs);
    _minimum2 = rhs._minimum2;
    _maximum2 = rhs._maximum2;
    }
  return *this;
  }

  void ConditionDoubleDualRange::print() const
  {
  Condition::print();
  printValue("Minimum Value 1",_minimum);
  printValue("Maximum Value 1",_maximum);
  printValue("Minimum Value 2",_minimum2);
  printValue("Maximum Value 2",_maximum2);
  }

} // namespace CAP

