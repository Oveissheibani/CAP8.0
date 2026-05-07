#ifndef CAP__LatexList
#define CAP__LatexList
#include "LatexElement.hpp"

namespace CAP
{
  class LatexList : public LatexElement
  {
  public:

  LatexList();
  LatexList(const LatexList & src);
  LatexList  & operator=(const LatexList & rhs);
  virtual ~LatexList()  { }
  void setType(int v) { _type = v; }
  void addItem(const String & text);
  void addItems(const std::vector<String> & items);

  int type() const { return _type; }
  std::vector<String>  items();
  const std::vector<String>  items() const;

  virtual void writeHeader(std::ofstream & out);
  virtual void writeContent(std::ofstream & out);
  virtual void writeTrailer(std::ofstream & out);

  protected:
  
  int _type;

  ClassDef(LatexList,0)
  };
} // namespace CAP

#endif // !CAP__LatexList
