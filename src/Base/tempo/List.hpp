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
#ifndef CAP__List
#define CAP__List
#include <iostream>
#include <vector>
#include "Aliases.hpp"
#include "Object.hpp"

//!
//! ObjectList
//!
//! A named list (std::vector) of objects
//! The objects can be of any types "T"
//! The list can be given a name and a title (or description) via the Object class
//!
//!
namespace CAP
{

//!
//!Named List of objects of type T
//!The objects are owned by the list
//!
template < class T >
class List
:
public Object
{
protected:

  std::vector<T> objects;

public:

  List(const String & name, const String & title, const String & className="List")
  :
  Object(name,title,className),
  objects()
  {  }

  List()
  :
  Object(),
  objects()
  {  }

  List(const List<T> & source)
  :
  Object(source),
  objects()
  {
  copy(source.getObjects());
  }

  virtual ~List()
  {  }

  List<T> & operator=(const List<T> & rhs)
  {
  if (this==&rhs) return *this;
  Object::operator=(rhs);
  copy(rhs.getObjects());
  return *this;
  }

  std::vector<T> & getObjects()
  {
  return objects;
  }

  const std::vector<T> & getObjects() const
  {
  return objects;
  }

  T &  getObjectAt(unsigned int index)
  {
  if (index>=objects.size()) throw IndexIndexOutOfBoundException(index,"Invalid index",__FUNCTION__);
  return objects[index];
  }

  T &  getObjectAt(unsigned int index) const
  {
  if (index>=objects.size()) throw IndexIndexOutOfBoundException(index,"Invalid index",__FUNCTION__);
  return objects[index];
  }

  T & push_back(T & object)
  {
  objects.push_back(object);
  return object;
  }

  const T & push_back(const T & object)
  {
  objects.push_back(object);
  return object;
  }

  unsigned size() const
  {
  return objects.size();
  }

  bool sameSizeAs(const List<T> & other) const
  {
  return objects.size() == other.size();
  }

  //!
  //!Object contained in this class must has a copy constructor
  //!
  void copy(const std::vector<T> & source)
  {
  for (auto & object : source) objects.push_back( * new T(object) );
  }

  //!
  //!Clear objects
  //!
  void clear()
  {
  objects.clear();
  }

  ClassDef(List<T>,1)
};



} // namespace CAP

#endif /* CAP__List */
