#include "LatexText.hpp"
ClassImp(CAP::LatexText);

namespace CAP
{
  LatexText::LatexText()
  :
  LatexElement()
  { }

  LatexText::LatexText(const LatexText & src)
  :
  LatexElement(src)
  { }

  LatexText  & LatexText::operator=(const LatexText & rhs)
  {
//  if (rhs!=*this)
//    {
    LatexElement::operator=(rhs);
//    }
  return *this;
  }

  void LatexText::writeContent(std::ofstream & out)
  {
  out << text() << endl;
  }
}

