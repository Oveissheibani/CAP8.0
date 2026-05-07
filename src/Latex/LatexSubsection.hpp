#ifndef CAP__LatexSubsection
#define CAP__LatexSubsection
#include "LatexElement.hpp"

namespace CAP
{
  class LatexSubsection  : public LatexElement
  {
  public:

  LatexSubsection();
  LatexSubsection(const LatexSubsection & src);
  LatexSubsection  & operator=(const LatexSubsection & rhs);
  virtual ~LatexSubsection() { }
  virtual void writeHeader(std::ofstream & out);

  ClassDef(LatexSubsection,0)
  };

} // namespace CAP

#endif // !CAP__LatexSubsection
