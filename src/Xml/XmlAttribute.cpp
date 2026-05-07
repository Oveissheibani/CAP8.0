#include "XmlAttribute.hpp"
#include "PrintHelpers.hpp"

ClassImp(CAP::XmlAttribute);

namespace CAP
{
  XmlAttribute::XmlAttribute()
  { }

  void XmlAttribute::print()
  {
  printString("XmlAttribute");
  printValue("name",name);
  printValue("value",value);
  }
} // namespace CAP
