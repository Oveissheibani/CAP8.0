#include "LatexAuthor.hpp"
ClassImp(CAP::LatexAuthor);

namespace CAP
{
  LatexAuthor::LatexAuthor()
  :
  LatexElement(),
  _email(),
  _affiliation()
  { }

  LatexAuthor::LatexAuthor(const LatexAuthor & src)
  :
  LatexElement(src),
  _email(src._email),
  _affiliation(src._affiliation)
  { }

  LatexAuthor  & LatexAuthor::operator=(const LatexAuthor & rhs)
  {
//  if (rhs!=*this)
//    {
    LatexElement::operator=(rhs);
  _email       = rhs._email;
  _affiliation = rhs._affiliation;
//    }
  return *this;
  }

  void LatexAuthor::write(std::ofstream & out)
  {
  skipLines(out,1);
  out << "\\author{" << name() << "}" << endl;
//  if (email().Length()>0)
//    out << "\\email{" << email() << "}" << endl;
  if (affiliation().Length()>0)
  out << "\\institute{" << affiliation() << "}" << endl;
  }
  
}

