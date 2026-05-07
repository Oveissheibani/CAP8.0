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
#include "XmlTag.hpp"
#include "PrintHelpers.hpp"

ClassImp(CAP::XmlTag);

namespace CAP
{
  XmlTag::XmlTag()
  { }

  void XmlTag::print()
  {
  printString("XmlTag");
  printValue("Name",name);
  printValue("begin",begin);
  printValue("end",end);
  for (auto & attribute : attributes)
    attribute.print();
  }

} // namespace CAP
