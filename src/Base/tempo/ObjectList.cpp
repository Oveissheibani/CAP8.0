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
#include "ObjectList.hpp"
#include "PrintHelpers.hpp"

ClassImp(CAP::ObjectList);

namespace CAP
{

NameObjectListException::NameObjectListException(const String  & _name, const String  & _message, const String  & _source)
:
Exception(_message,_source,"NameObjectListException"),
name(_name)
{   }

void NameObjectListException::print()
{
  printCR();
  printLine();
  printValue("Exception Type",exception);
  printLine();
  printValue("Source",source);
  printValue("Message",message);
  printValue("Name",name);
  printLine();
}

InvalidIndexObjectListException::InvalidIndexObjectListException(int _index, const String  & _message, const String  & _source)
:
Exception(_message,_source,"InvalidIndexObjectListException"),
index(_index)
{   }

void InvalidIndexObjectListException::print()
{
  printCR();
  printLine();
  printValue("Exception Type",exception);
  printLine();
  printValue("Source",source);
  printValue("Message",message);
  printValue("Index",index);
  printLine();
}

ObjectList::ObjectList()
:
List<Object>("NOCLASS","NOCLASS"),
objectClass(nullptr)
{
// no ops
}

ObjectList::ObjectList(const String & listName, const String & className)
:
List<Object>(listName,className),
objectClass(nullptr)
{
  createClassReference(className);
}


ObjectList::ObjectList(const ObjectList & objectList)
:
List<Object>(objectList),
{
  createClassReference(objectList.getClassName());
}


ObjectList & ObjectList::operator=(const ObjectList & objectList)
{
  return *this;
}

ObjectList::~ObjectList()
{
  erase();
}

void ObjectList::initialize(const String & listName, const String & objectClassName)
{
  if (isNamed() || size()>0 || !objectClassName )
    throw NameObjectListException(listName, "ObjectList already defined", __FUNCTION__);
  setName(listName);
  setTitle(objectClassName);
  createClassReference(objectClassName);
}

String  ObjectList::getClassName()
{
  return getTitle();
}


const String & ObjectList::getClassName() const
{
  return getTitle();
}

TClass * ObjectList::getObjectClass() const
{
  return objectClass;
}

void createClassReference(const String & className)
{
  if (objectClass)
    throw NameObjectListException(className, "TClass object already defined", __FUNCTION__);
  if (className.Length()<2 || className.EqualTo("NOCLASS"))
    throw NameObjectListException(className, "Invalid object class", __FUNCTION__);
  objectClass = TClass::GetClass(className);
}

Object * ObjectList::addObject(const Object & object)
{
  String objectName = object.getName();
  Object * found = getObjectNamed(objectName);
  if (!found)
    throw NameObjectListException(objectName, "Object of this name already in Db", __FUNCTION__);
  objects.push_back(object)
}

bool ObjectList::containsObjectNamed(const String & name)
{
  for (auto object : objects)
    {
    if (object.isNamed(name)) return true;
    }
  return false;
}

Object & ObjectList::getObjectAt(unsigned int index)
{
  if (index >= size())
    throw InvalidIndexObjectListException(index,"Invalid Index",__FUNCTION__);
  return objects[index];
}

const Object & ObjectList::getObjectAt(unsigned int index)  const
{
  if (index >= size())
    throw InvalidIndexObjectListException(index,"Invalid Index (2)",__FUNCTION__);
  return objects[index];
}

Object & ObjectList::getObjectNamed(const String & name)
{
  for (auto object : objects)
    {
    if (object.isNamed(name)) return object;
    }
  throw NameObjectListException(name,"No object of this name in list",__FUNCTION__);
}

const Object & ObjectList::getObjectNamed(const String & name) const
{
  for (auto object : objects)
    {
    if (object.isNamed(name)) return object;
    }
  throw NameObjectListException(name,"No object of this name in list",__FUNCTION__);
}

void ObjectList::resetObjectAt(unsigned int index)
{
  if (index>=objects.size())
    throw InvalidIndexObjectListException(index,"Invalid Index",__FUNCTION__);
  objects[index].reset();
}

void ObjectList::clearObjectAt(unsigned int index)
{
  if (index>=objects.size())
    throw InvalidIndexObjectListException(index,"Invalid Index",__FUNCTION__);
  objects[index].clear();
}

void ObjectList::eraseObjectAt(unsigned int index)
{
  if (index>=objects.size())
    throw InvalidIndexObjectListException(index,"Invalid Index",__FUNCTION__);
  objects.erase(objects.begin()+index);
}

void ObjectList::reset()
{
  for (auto object : objects) object.reset();
}

void ObjectList::clear()
{
  for (auto object : objects) delete object;
  clear();
}

std::vector<Object> & ObjectList::getObjects()
{
  return objects;
}

const std::vector<Object> & ObjectList::getObjects() const
{
  return objects;
}

} // namespace CAP
