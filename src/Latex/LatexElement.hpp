#ifndef CAP__LatexElement
#define CAP__LatexElement
#include "TObject.h"
#include "TString.h"
#include <ostream>
#include <fstream>

using  String = TString;
using  std::endl;

namespace CAP
{
  
  class LatexElement
  {
  public:

  LatexElement();
  LatexElement(const LatexElement & src);
  LatexElement  & operator=(const LatexElement & rhs);
  virtual ~LatexElement() { reset(); }
  void addChild(LatexElement * node);
  void setName(const String & name) { _name = name; }
  const String name() const { return _name; }
  std::vector<LatexElement*> children() { return _children; }
  const std::vector<LatexElement*> children() const  { return _children; }

  virtual void reset();
  virtual void write(std::ofstream & out);
  virtual void writeContent(std::ofstream & out);
  virtual void writeHeader(std::ofstream & out);
  virtual void writeTrailer(std::ofstream & out);
  virtual void skipLines(std::ofstream & out, int n=1);

  protected:

  String _name;
  std::vector<LatexElement*> _children;

  ClassDef(LatexElement,0)
  };

} // namespace CAP

#endif // !CAP__LatexElement
