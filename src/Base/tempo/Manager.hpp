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
#ifndef CAP__Manager
#define CAP__Manager
#include "Aliases.hpp"
#include "Exceptions.hpp"
#include "List.hpp"
#include "PrintHelpers.hpp"

namespace CAP
{
class ManagerException : public Exception
{
public:
  ManagerException(const String  & _name, const String  & _message, const String  & _source);
  ManagerException(int index, const String  & _message, const String  & _source);
  virtual ~ManagerException() {}
  virtual void print();
};

//!
//!Manager manages a static list of stored object (pointers) and a "local" list
//!
template < class T >
class Manager : public List<T>
{

public:

  Manager();
  Manager(const Manager & manager);
  Manager & operator=(const Manager & manager);
  virtual ~Manager();

  T create(const String & name);
  T use(const  String &  name);
  
  virtual void resetObjectAt(unsigned int index);
  virtual void clearObjectAt(unsigned int index);
  virtual void eraseObjectAt(unsigned int index);
  virtual void reset();
  virtual void clear();
  virtual void erase();
  std::vector<T> & getObjects();
  const std::vector<T> & getObjects() const;
  T getObjectNamed(const String & name);
  T getObjectAt(unsigned int index);

  // static methods

  static void setObjectStoreName(const String & name);
  static bool objectStoreContainsObjectNamed(const String & name);
  static bool isObjectStoreNamed(const String & name);
  static T & createStoreObjectNamed(const String & name);
  static T & getStoreObjectNamed(const String & name);
  static int getObjectStoreSize();

protected:

  ListPtr<T> list;

  static ListPtr<T> objectStore;

  ClassDef(Manager,0)
};

template < class T >
Manager<T>::Manager()
:
List<T>()
{

}

template < class T >
Manager<T>::Manager(const Manager & source)
:
List<T>()
{ }

template < class T >
Manager<T> & Manager<T>::operator=(const Manager<T> & rhs)
{
  if (this!=&rhs)
    {
    List<T>operator=(rhs);
    }
  return *this;
}

template < class T >
Manager<T>::~Manager()
{
  clear();
}

template < class T >
T & Manager<T>::create(const String & name)
{
  if (objectStoreContainsObjectNamed(name))
    {
    T object = getStoreObjectNamed(name);
    push_back(object);
    return object;
    }
  T object = Manager<T>::createStoreObjectNamed(name);
  push_back(object);
  return object;
}

template < class T >
T & Manager<T>::use(const  String &  name)
{
  if (objectStoreContainsObjectNamed(name))
    {
    T object = getStoreObjectNamed(name);
    push_back(object);
    return object;
    }
  throw ManagerException(name,"Object not found in object store",__FUNCTION__);
  return object;
}

template < class T >
void Manager<T>::resetObjectAt(unsigned int index)
{
  if (index>=size()) throw ManagerException(index,"Index out of bound",__FUNCTION__);
  std::vector<T>::at(index).reset();
}

;

template < class T >
void Manager<T>::clearObjectAt(unsigned int index)
{
  if (index>=size()) throw ManagerException(index,"Index out of bound",__FUNCTION__);
  std::vector<T>::at(index).clear();
}

template < class T >
void Manager<T>::eraseObjectAt(unsigned int index)
{
  if (index>=size()) throw ManagerException(index,"Index out of bound",__FUNCTION__);
  T object = std::vector<T>::at(index);
  object.clear();
}

template < class T >
void Manager<T>::reset()
{
  std::vector<T> & objects = getObjects();
  for (auto object :  objects) object.reset();
}

template < class T >
void Manager<T>::clear()
{
  std::vector<T> & objects = getObjects();
  for (auto object :  objects) object.clear();
}

template < class T >
void Manager<T>::erase()
{
}

template < class T >
std::vector<T> & Manager<T>::getObjects()
{
  return (std::vector<T> &)*this;
}

template < class T >
const std::vector<T*> & Manager<T>::getObjects() const
{
  return (const std::vector<T> &)*this;
}

template < class T >
T Manager<T>::getObjectNamed(const String & name)
{
  if (size()<1) throw ManagerException("EMPTY","No objects available",__FUNCTION__);
  const std::vector<T*> & objects = getObjects();
  for (auto & object : objects)
    {
    if (object.isNamed(name)) return object;
    }
  throw ManagerException(name,"Unknown object", __FUNCTION__);
}

template < class T >
T Manager<T>::getObjectAt(unsigned int index)
{
  unsigned int n = size();
  if (n<1) 
    throw ManagerException(index,"objects.size()<1",__FUNCTION__);
  else if (index>=n)
    throw ManagerException(index,"index>=objects.size()",__FUNCTION__);

  const std::vector<T*> & objects = getObjects();
  return objects[index];
}

// static members

template <class T>
void T Manager<T>::setObjectStoreName(const String & name)
{
  objectStore.setName(name);
}

template <class T>
bool T Manager<T>::objectStoreContainsObjectNamed(const String & name)
{
  return objectStore.contains(name);
}

template <class T>
bool T Manager<T>::isObjectStoreNamed(const String & name);

template <class T>
T & T Manager<T>::createStoreObjectNamed(const String & name);

template <class T>
T & T Manager<T>::getStoreObjectNamed(const String & name);

template <class T>
int T Manager<T>::getObjectStoreSize()
{
  return objectStore.size();
}


template <class T>
T Manager<T>::createStoreObjectNamed(const String & newName)
{
  for (auto & name : objectStoreNames)
    {
    if (name.EqualTo(newName))
      throw ManagerException(name,"T already exist",__FUNCTION__);
    }
  T object = new T();
  objectStore.push_back(object);
  return object;
}

template <class T>
T* Manager<T>::getStoreObjectNamed(const String & requestedName)
{
  //printValue("Store Size Name", objectStoreNames.size());
  //printValue("Store Size Obje", objectStore.size());
  unsigned int index=0;
  for (auto & name : objectStoreNames)
    {
    //printValue("Store Name",name);
    if (name.EqualTo(requestedName)) return objectStore[index];
    index++;
    }
  throw ManagerException(requestedName,"T does not exist",__FUNCTION__);
}

template <class T>
int Manager<T>::getNObjectStore()
{
  return objectStore.size();
}

} // namespace CAP

#endif /* CAP_Manager */


