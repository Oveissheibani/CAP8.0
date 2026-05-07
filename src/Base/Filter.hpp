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
#ifndef CAP__Filter
#define CAP__Filter
#include "Exceptions.hpp"
#include "Object.hpp"
#include "Condition.hpp"
#include "PrintHelpers.hpp"

namespace CAP
{

class FilterException : public Exception
{
public:
  FilterException(const String & _message,
                  const String & _source);
  virtual ~FilterException() {}
};

template < class T >
class Filter
{
protected:

  std::vector<Condition*> _conditions;

public:

  Filter()
  :
  _conditions()
  { /* no ops */ }

  virtual ~Filter()
  {
  for (auto & condition : _conditions) delete condition;
  _conditions.clear();
  }

  unsigned int nConditions() const {  return _conditions.size(); }
  std::vector<Condition*> & conditions() { return _conditions; }
  const std::vector<Condition*> & conditions() const { return _conditions; }

  //!
  //! Test whether the given object is acceptable based on this filter's criteria.
  //!  The base class accepts all objects submitted.
  //!
  bool accept(const T & object __attribute__((unused))) const
  {
  throw FilterException("Improper call to accept of base class","Filter::accept(const T & object __attribute__((unused)))");
  }

  //! Test whether the given objects are acceptable based on this filter's criteria.
  //!  The base class accepts all objects submitted.
  bool accept(const T & object1 __attribute__((unused)), const T & object2 __attribute__((unused))) const
  {
  throw FilterException("Improper call to accept of base class","Filter::accept(const T & object1 __attribute__((unused)), const T & object2 __attribute__((unused)))");
  }

//  virtual void print(  ) const
//  {
//  for (auto & condition : _conditions) condition->print();
//  }

  void addCondition(int _subtype,
                    bool _requested)
  {
  _conditions.push_back(new ConditionBool(_subtype,_requested));
  }

  void addCondition(int _subtype,
                    int _requested)
  {
  _conditions.push_back(new ConditionInteger(_subtype,_requested));
  }

  void addCondition(int _subtype,
                    int _minimum,
                    int _maximum)
  {
  _conditions.push_back(new ConditionIntegerRange(_subtype,_minimum,_maximum));
  }

  void addCondition(int _subtype,
                    double _minimum,
                    double _maximum)
  {
  _conditions.push_back(new ConditionDoubleRange(_subtype,_minimum,_maximum));
  }

  void addCondition(int _subtype,
                    double _minimum,
                    double _maximum,
                    double _minimum2,
                    double _maximum2)
  {
  _conditions.push_back(new ConditionDoubleDualRange(_subtype,_minimum,_maximum,_minimum2,_maximum2));
  }


}; // Filter<T>

}// namespace CAP


#endif /* CAP__Filter  */

