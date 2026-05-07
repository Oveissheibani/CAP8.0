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
#ifndef CAP__Array
#define CAP__Array
#include "Object.hpp"

//!
//! Array
//!
//! Generic base class for Array services.
//! A Array can be used to generate and own a large collection of objects of the same type "T". Objects are allocated on the heap and pointers "T*" to these
//! objects kept in a large array T ** objects. The array is nominally of fixed size but is enlarged whenever more objects are needed. The Array model is useful
//! when objects need to be repeatedly used and discarded (e.g., in data analysis of particles) because no malloc or destroy is required. The memory remains allocated
//! and so there is no time wasted creating and destroying the objects. The reset method is to be called on a Array to indicate that a new "event" is being considered.
//! It must evidently be possible to initialize the objects with "set" methods specific to the class "T". Use the  T * nextObject() method to obtain an used object.
//!

namespace CAP
{

template < class T >
class Array
{
protected:

  unsigned int defaultCapacity;
  unsigned int defaultSizeIncrement;
  T[] objects;

  public:

  Array()
  :
  defaultCapacity(10),
  defaultSizeIncrement(10),
  objects()
  {
    initialize();
  }

  virtual ~Array()
  {
  clear();
  }
  
  void setDefaultCapacity(long _defaultCapacity)
  {
    defaultCapacity = _defaultCapacity;
  }

  void setDefaultSizeIncrement(long _defaultSizeIncrement)
  {
  defaultSizeIncrement = _defaultSizeIncrement;
  }

  void initialize()
  {
  allocate(defaultCapacity);
  }

  void allocate(long size, T initialValue)
  {
    objects = new T[size];
    for (long k=0;k<size;k++) objects[k] = initialValue;
  }

  void reset()
  {
  for (auto & object : objects) object->reset();
  last = 0;
  }

  void clear()
  {
  for (auto & object : objects)
    {
    object->reset();
    delete object;
    }
  objects.clear();
  }


  ClassDef(Array<T>,0)
};

} // namespace CAP

#endif /* Array_hpp */
