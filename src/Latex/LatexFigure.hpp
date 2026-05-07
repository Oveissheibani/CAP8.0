#ifndef CAP__LatexFigure
#define CAP__LatexFigure
#include "LatexElement.hpp"

namespace CAP
{
  class LatexFigure : public LatexElement
  {
  public:

  LatexFigure();
  LatexFigure(const LatexFigure & src);
  LatexFigure  & operator=(const LatexFigure & rhs);
  virtual ~LatexFigure() {  }
  virtual void writeHeader(std::ofstream & out);
  virtual void writeContent(std::ofstream & out);
  virtual void writeTrailer(std::ofstream & out);

  void setFileName(const String & fileName) { _fileName = fileName; }
  void setCaption(const String & caption)   { _caption = caption; }
  void setLabel(const String & label)       { _label = label; }
  void setScale(float v)  { _scale = v; }
  void setHeight(float v) { _height = v; }

  const String & fileName() const { return _fileName; }
  const String & caption() const  { return _caption;  }
  const String & label() const  { return _label;  }

  protected:

  String _fileName;
  String _caption;
  String _label;
  float  _scale;
  float  _height;

  ClassDef(LatexFigure,0)
  };

} // namespace CAP

#endif // !CAP__LatexFigure
