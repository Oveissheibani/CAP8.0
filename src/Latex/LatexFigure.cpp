#include "LatexFigure.hpp"

ClassImp(CAP::LatexFigure);

namespace CAP
{
  LatexFigure::LatexFigure()
  :
  LatexElement(),
  _fileName(),
  _caption(),
  _label(),
  _scale(0.75),
  _height(2)
  { }

  LatexFigure::LatexFigure(const LatexFigure & src)
  :
  LatexElement(src),
  _fileName(src._fileName),
  _caption(src._caption),
  _label(src._label),
  _scale(src._scale),
  _height(src._height)
  { }

  LatexFigure  & LatexFigure::operator=(const LatexFigure & rhs)
  {
//  if (rhs!=*this)
//    {
    LatexElement::operator=(rhs);
    _fileName  = rhs._fileName;
    _caption   = rhs._caption;
    _label     = rhs._label;
    _scale     = rhs._scale;
    _height    = rhs._height;
//    }
  return *this;
  }

  void LatexFigure::writeHeader(std::ofstream & out)
  {
  out << "\\begin{figure}[htbp]" << endl;
  out << "\\centering" << endl;
  }

  void LatexFigure::writeContent(std::ofstream & out)
  {
  // _scale is used as a fraction of \textwidth.  A width-based size keeps
  // figures readable and centred, and avoids the distortion of pinning an
  // absolute height regardless of the image's aspect ratio.
  String s(""); s += _scale;
  out << "\\includegraphics[width=" << s << "\\textwidth]{"
      << fileName() << "}" << endl;
  if (caption().Length() > 0)
    out << "\\caption{" << caption() << "}" << endl;
  if (label().Length() > 0)
    out << "\\label{" << label() << "}" << endl;
  }

  void LatexFigure::writeTrailer(std::ofstream & out)
  {
  out << "\\end{figure}" << endl;
  }

} //namespace CAP
