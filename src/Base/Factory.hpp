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
#ifndef CAPFactory
#define CAPFactory
#include "Exceptions.hpp"

//!
//! Factory
//!
//! Generic base class for factory services.
//! A factory can be used to generate and own a large collection of objects of the same type "T". Objects are allocated on the heap and pointers "T*" to these
//! objects kept in a large array T ** objects. The array is nominally of fixed size but is enlarged whenever more objects are needed. The factory model is useful
//! when objects need to be repeatedly used and discarded (e.g., in data analysis of particles) because no malloc or destroy is required. The memory remains allocated
//! and so there is no time wasted creating and destroying the objects. The reset method is to be called on a factory to indicate that a new "event" is being considered.
//! It must evidently be possible to initialize the objects with "set" methods specific to the class "T". Use the  T * nextObject() method to obtain an used object.
//!

namespace CAP
{

template < class T >
class Factory
{
protected:

  long _currentSize;
  long _defaultSize;
  long _defaultSizeIncrement;
  long _nextServed;
  bool _owner;
  T ** _objects;

  public:

  Factory(long defaultSize=5000, long defaultSizeIncrement=1000, bool owner=true)
  :
  _currentSize(0),
  _defaultSize(defaultSize),
  _defaultSizeIncrement(defaultSizeIncrement),
  _nextServed(0),
  _owner(owner),
  _objects(nullptr)
  {   }

  virtual ~Factory()
  {
  clear();
  }

  void setDefaultSize(long defaultSize)
  {
  _defaultSize = defaultSize;
  }

  long defaultSize() const
  {
  return _defaultSize;
  }

  void setDefaultSizeIncrement(long defaultSizeIncrement)
  {
  _defaultSizeIncrement = defaultSizeIncrement;
  }

  long defaultSizeIncrement() const
  {
  return _defaultSizeIncrement;
  }

  void setOwner(bool owner)
  {
  _owner = owner;
  }

  long size()
  {
    return _currentSize;
  }

  void initialize()
  {
  if (_objects) clear();
  _objects = new T*[_defaultSize];
  for (unsigned int k=0; k<_defaultSize; k++ ) _objects[k] = new T();
  _currentSize = _defaultSize;
  _nextServed = 0;
  }

  void increaseSize(long sizeIncrement)
  {
  T** hold = _objects;
  long newSize = _currentSize + sizeIncrement;
  _objects = new T*[newSize];
  for (long k=0; k<_currentSize; k++) _objects[k] = hold[k];
  for (long k=_currentSize; k<newSize; k++) _objects[k] = new T();
  delete[] hold;
  }

  void reset()
  {
  _nextServed = 0;
  }

  void clear()
  {
  if (_owner)
    {
    for (long k=0; k<_currentSize; k++) delete _objects[k];
    delete[] _objects;
    }
  else
    throw MemoryException("Non owner cannot delete array",__FUNCTION__);
  }

  T & objectAt(unsigned int index)
  {
  return *(_objects[index]);
  }

  T & nextObject()
  {
  if ((_nextServed+1)>=size()) increaseSize(_defaultSizeIncrement);
  return *(_objects[_nextServed++]);
  }

  ClassDef(Factory<T>,0)
};

} // namespace CAP

#endif /* Factoryhpp */
