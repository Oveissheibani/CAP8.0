#ifndef CAP__LatexSection
#define CAP__LatexSection
#include "LatexElement.hpp"

namespace CAP
{
  class LatexSection : public LatexElement
  {
  public:

  LatexSection();
  LatexSection(const LatexSection & src);
  LatexSection  & operator=(const LatexSection & rhs);
  virtual ~LatexSection() { }
  virtual void writeHeader(std::ofstream & out);

  ClassDef(LatexSection,0)
  };

} // namespace CAP

#endif // !CAP__LatexSection
