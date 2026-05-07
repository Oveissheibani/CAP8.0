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
#ifndef CAP__XmlTag
#define CAP__XmlTag
#include <list>
#include "Parser.hpp"
#include "Aliases.hpp"
#include "XmlAttribute.hpp"

namespace CAP
{

class XmlTag
{
public:

  XmlTag();
  virtual ~XmlTag() {}
  void print();

  String  name;
  std::list<XmlAttribute> attributes;
  long int begin;
  long int end;
  XmlTag*  parent;
  XmlTag*  child;
  XmlTag*  prev;
  XmlTag*  next;

  ClassDef(XmlTag,0)
};

} // namespace CAP

#endif // CAP__XmlTag
