#include "LatexList.hpp"
#include "LatexText.hpp"
//ClassImp(CAP::LatexList);

namespace CAP
{
  LatexList::LatexList()
  :
  _type(0)
  {   }

  LatexList::LatexList(const LatexList & src)
  :
  _type(src._type)
  {   }

  LatexList & LatexList::operator=(const LatexList & rhs)
  {
//  if (rhs!=*this)
//    {
    _type  = rhs._type;
//    }
  return *this;
  }

  void LatexList::addItem(const String & text)
  {
  LatexText * t = new LatexText();
  t->setName(text);
  _children.push_back(t);
  }

  void LatexList::addItems(const std::vector<String> & items)
  {
  for (auto item : items)
    {
    LatexText * t = new LatexText();
    t->setName(item);
    _children.push_back(t);
    }

  }

  void LatexList::writeHeader(std::ofstream & out)
  {
  skipLines(out,1);
  if (_type==0)
    out << "\\begin{itemize}" << endl;
  else
    out << "\\begin{enumerate}" << endl;
  skipLines(out,1);
  }

  void LatexList::writeContent(std::ofstream & out)
  {
  for (auto child : children())
    {
    out << "\\item" << endl;
    child->writeContent(out);
    }
  }

  void LatexList::writeTrailer(std::ofstream & out)
  {
  skipLines(out,1);
  if (_type==0)
    out << "\\end{itemize}" << endl;
  else
    out << "\\end{enumerate}" << endl;
  skipLines(out,1);
  }

  std::vector<String>  LatexList::items()
  {
  std::vector<String> items;
  for (auto child : children())
    {
    items.push_back(child->name());
    }
  return items;
  }

  const std::vector<String>  LatexList::items() const
  {
  std::vector<String> items;
  for (auto child : children())
    {
    items.push_back(child->name());
    }
  return items;
  }

} // namespace CAP
