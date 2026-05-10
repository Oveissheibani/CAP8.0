#include "LatexSubsubsection.hpp"
ClassImp(CAP::LatexSubsubsection);

namespace CAP
{
  LatexSubsubsection::LatexSubsubsection()
  :
  LatexElement()
  { }

  LatexSubsubsection::LatexSubsubsection(const LatexSubsubsection & src)
  :
  LatexElement(src)
  { }

  LatexSubsubsection  & LatexSubsubsection::operator=(const LatexSubsubsection & rhs)
  {
//  if (rhs!=*this)
//    {
    LatexElement::operator=(rhs);
//    }
  return *this;
  }

  void LatexSubsubsection::writeHeader(std::ofstream & out)
  {
  skipLines(out,2);
  out << "\\subsubsection{" << name() << "}" << endl;
  skipLines(out,1);
  }

} // namespace CAP
