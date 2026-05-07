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
#include "Exceptions.hpp"
#include "Accountant.hpp"
#include "PrintHelpers.hpp"

ClassImp(CAP::Accountant);

namespace CAP
{

Accountant::Accountant(const String & _name, unsigned int size)
:
Object(),
_counts(size,0)
{
  setName(_name);
}

Accountant::Accountant(const Accountant & source)
:
Object(source),
_counts(source._counts)
{  /* no ops */ }

Accountant & Accountant::operator=(const Accountant & rhs)
{
  if (this!=&rhs)
    {
    Object::operator=(rhs);
    _counts = rhs._counts;
    }
  return *this;
}

void Accountant::reset()
{
  _counts.assign(_counts.size(),0);
}

void Accountant::clear()
{
  _counts.clear();
}

void Accountant::initialize(const String & name, unsigned int size)
{
  setName(name);
  _counts.assign(size,0);
}

void Accountant::increment(unsigned int index)
{
  if (index>=_counts.size()) throw IndexOutOfBoundException(index,_counts.size(),"invalid index",__FUNCTION__);
  _counts[index]++;
}

void Accountant::increment(const Accountant & other)
{
  unsigned int n = _counts.size();
  if (n != other._counts.size())
    throw Exception("Trying to add two incompatible account objects",__FUNCTION__);
  for (unsigned int k=0; k<n; k++)
    _counts[k] += other._counts[k];
}

long Accountant::valueAt(unsigned int index) const
{
  if (index>=_counts.size()) throw IndexOutOfBoundException(index,_counts.size(),"invalid index",__FUNCTION__);
  return _counts[index];
}

void Accountant::setValueAt(unsigned int index, long value)
{
  if (index>=_counts.size()) throw IndexOutOfBoundException(index,_counts.size(),"invalid index",__FUNCTION__);
  _counts[index] = value;
}

void Accountant::print() const
{
  printCR();
  printLine();
  unsigned int size = _counts.size();
  String className  = "Accountant";
  String objectName = name();
  String outputName;
  outputName = className;
  outputName = ":";
  outputName = name();
  outputName = ":N";
  printValue(outputName,size);
  for (unsigned int k=0; k<_counts.size(); k++)
    {
    outputName = className;
    outputName = ":";
    outputName = name();
    outputName = "[";
    outputName += k;
    outputName = "]";
    printValue(outputName,_counts[k]);
    }
}

} // namespace CAP



