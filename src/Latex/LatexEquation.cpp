#include "LatexEquation.hpp"
ClassImp(CAP::LatexEquation);

namespace CAP
{
  LatexEquation::LatexEquation()
  :
  LatexElement()
  { }

  LatexEquation::LatexEquation(const LatexEquation & src)
  :
  LatexElement(src)
  { }

  LatexEquation  & LatexEquation::operator=(const LatexEquation & rhs)
  {
//  if (rhs!=*this)
//    {
    LatexElement::operator=(rhs);
//    }
  return *this;
  }

  void LatexEquation::writeHeader(std::ofstream & out)
  {
  out << "\\begin{equation}" << endl;
  out << "\\label{" << label() << "}"<< endl;
  }

  void LatexEquation::writeTrailer(std::ofstream & out)
  {
  out << "\\end{equation}" << endl;
  }

  void LatexEquation::writeContent(std::ofstream & out)
  {
  out << text() << endl;
  }
}

