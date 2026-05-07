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
#ifndef CAP__XmlAttribute
#define CAP__XmlAttribute
#include <list>
#include "Parser.hpp"
#include "Aliases.hpp"

namespace CAP
{

class XmlAttribute
{
public:

  XmlAttribute();
  virtual ~XmlAttribute() {}
  void print();

  String name;
  String value;

  ClassDef(XmlAttribute,0)
};

} // namespace CAP

#endif // !CAP__XmlAttribute
