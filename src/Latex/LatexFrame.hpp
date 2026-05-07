#ifndef CAP__LatexFrame
#define CAP__LatexFrame
#include "LatexElement.hpp"

namespace CAP
{

class LatexFrame : public LatexElement
  {
  public:

  LatexFrame();
  LatexFrame(const LatexFrame & src);
  LatexFrame   & operator=(const LatexFrame & rhs);
  ~LatexFrame() { reset(); }
  virtual void writeHeader(std::ofstream & out);
  virtual void writeTrailer(std::ofstream & out);

  virtual void addText(const String & text);
  virtual void addFigure(const String & figureName,
                         const String & figureCaption,
                         const String & figureLabel,
                         float scale = 0.75,
                         float height = 1.0);
  virtual void addList(const std::vector<String> & items);
  virtual void addEquation(const String & text,const String & equationLabel);


  ClassDef(LatexFrame,0)
  };

} // namespace CAP

#endif // !CAP__LatexFrame
