#include "LatexElement.hpp"
ClassImp(CAP::LatexElement);

namespace CAP
{

  void LatexElement::skipLines(std::ofstream & out, int n)
  {
  for (int k=0; k<n; k++) out << endl;
  }

  LatexElement::LatexElement()
  :
  _name("NoName"),
  _children()
  { }

  LatexElement::LatexElement(const LatexElement & src)
  :
  _name(src._name),
  _children(src._children)
  { }

  //Accountant & Accountant::operator=(const Accountant & rhs)
  LatexElement  & LatexElement::operator=(const LatexElement & rhs)
  {
  //if (rhs != *this)
    //{
    _name      = rhs._name;
    _children  = rhs._children;
   // }
  return *this;
  }

  void LatexElement::reset()
  {
  _name = "NoName";
  _children.clear();
  }

  void LatexElement::addChild(LatexElement * node)
  {
  _children.push_back(node);
  }

  void LatexElement::write(std::ofstream & out)
  {
  writeHeader(out);
  writeContent(out);
  writeTrailer(out);
  }

  void LatexElement::writeContent(std::ofstream & out)
  {
  for (auto child : _children) child->write(out);
  }

  void LatexElement::writeHeader(std::ofstream & out)
  {
  skipLines(out,1);
  }

  void LatexElement::writeTrailer(std::ofstream & out)
  {
  skipLines(out,1);
  }


} // namespace CAP
