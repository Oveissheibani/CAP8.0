#ifndef CAP__LatexSubsubsection
#define CAP__LatexSubsubsection
#include "LatexElement.hpp"

namespace CAP
{

  class LatexSubsubsection : public LatexElement
  {
  public:

  LatexSubsubsection();
  LatexSubsubsection(const LatexSubsubsection & src);
  LatexSubsubsection  & operator=(const LatexSubsubsection & rhs);
  ~LatexSubsubsection(){ }
  virtual void writeHeader(std::ofstream & out);
  void setTitle(const String &title) { setName(title); }
  const String & title() const { return name(); }

  ClassDef(LatexSubsubsection,0)
  };

} // namespace CAP

#endif // !CAP__LatexSubsubsection
