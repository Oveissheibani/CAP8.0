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
#ifndef CAP__ObjectList
#define CAP__ObjectList
#include "Exceptions.hpp"
#include "List.hpp"
#include "Object.hpp"

namespace CAP
{

//!
//!List of objects
//!Objects are assume to have name, title, etc
//!
//!
template < class T >
class ObjectList
{
public:

  ObjectList();
  ObjectList(const String & listName, const String & className);
  ObjectList(const ObjectList & ObjectList);
  ObjectList & operator=(const ObjectList & ObjectList);
  virtual ~ObjectList();
  void initialize(const String & listName, const String & objectClassName);

  String  getClassName();
  const String & getClassName() const;
  TClass * getObjectClass() const;
  void createClassReference(const String & name="NOCLASS");

  virtual void addObject(const Object & object);
  virtual bool containsObjectNamed(const String & name);
  virtual T * getObjectAt(unsigned int index);
  virtual T * getObjectNamed(const String & name);
  virtual const Object & getObjectAt(unsigned int index) const;
  virtual const Object & getObjectNamed(const String & name) const;

  virtual void reset();
  virtual void clear();
  std::vector<Object> & getObjects();
  const std::vector<Object*> & getObjects() const;

 

protected:

  TClass * objectClass;



  ClassDef(ObjectList,0)
};

} //namespace CAP

#endif /* CAP_ObjectList */


