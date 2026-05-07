#ifndef CAP__LatexText
#define CAP__LatexText
#include "LatexElement.hpp"

namespace CAP
{
  class LatexText : public LatexElement
  {
  public:

  LatexText();
  LatexText(const LatexText & src);
  LatexText  & operator=(const LatexText & rhs);
  virtual ~LatexText() { reset(); }
  virtual void writeContent(std::ofstream & out);

  void setText(const String & text) { _name = text; }
  const String & text() const { return _name; }

  ClassDef(LatexText,0)
  };

} // namespace CAP

#endif // !CAP__LatexText
