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
#ifndef CAP__Accountant
#define CAP__Accountant
#include "Aliases.hpp"
#include "Object.hpp"

namespace CAP
{

//!
//!Class used to keep an account of the number of times a task is completed or a certain condition is fulfilled. The counter can be a "Scalar" to keep track of one task or condition but it can also be used as a  "Vector" to keep track of several disjoined conditions (e.g., event fileters)
//!
class Accountant
:
public Object
{
public:

  Accountant(const String & name="Executed", unsigned int size=1);
  Accountant(const Accountant & accountant);
  Accountant & operator=(const Accountant & accountant);
  virtual ~Accountant() { }
  virtual void reset();
  virtual void clear();
  virtual void initialize(const String & name, unsigned );
  virtual void increment(unsigned int index=0);
  virtual void increment(const Accountant & other);
  virtual long valueAt(unsigned int index=0) const;
  virtual void setValueAt(unsigned int index, long value);
  virtual void print() const;
  
  std::vector<long> & values()
  {
  return _counts;
  }
  const std::vector<long> & values() const
  {
  return _counts;
  }

  unsigned int size()
  {
  return _counts.size();
  }

protected:
  
  std::vector<long> _counts;

  ClassDef(Accountant,0)
};

} // namespace CAP

#endif /* CAP__Accountant */
