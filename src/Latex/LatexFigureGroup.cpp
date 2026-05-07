#include "LatexFigureGroup.hpp"
ClassImp(CAP::LatexFigureGroup);

namespace CAP
{
  LatexFigureGroup::LatexFigureGroup()
  :
  LatexElement()
  { }

  LatexFigureGroup::LatexFigureGroup(const LatexFigureGroup & src)
  :
  LatexElement(src)
  { }

  LatexFigureGroup & LatexFigureGroup::operator=(const LatexFigureGroup & rhs)
  {
//  if (rhs!=*this)
//    {
    LatexElement::operator=(rhs);
//    }
  return *this;
  }

  void LatexFigureGroup::writeContent(std::ofstream & out)
  {
  for (auto child : children())
    {
    child->write(out);
    }
  }

} // namespace CAP

