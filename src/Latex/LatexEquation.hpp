#ifndef CAP__LatexEquation
#define CAP__LatexEquation
#include "LatexElement.hpp"

namespace CAP
{
  class LatexEquation : public LatexElement
  {
  public:

  LatexEquation();
  LatexEquation(const LatexEquation & src);
  LatexEquation  & operator=(const LatexEquation & rhs);
  virtual ~LatexEquation() { }

  void setText(const String & text) { _name = text; }
  void setLabel(const String & label) { _label = label; }
  const String & text() const { return _name; }
  const String & label() const { return _label; }

  virtual void writeHeader(std::ofstream & out);
  virtual void writeContent(std::ofstream & out);
  virtual void writeTrailer(std::ofstream & out);

  protected:
  String _label;

  ClassDef(LatexEquation,0)
  };

} // namespace CAP

#endif // !CAP__LatexEquation
