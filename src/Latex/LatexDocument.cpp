#include "LatexDocument.hpp"
#include "StreamHelpers.hpp"

ClassImp(CAP::LatexDocument);


namespace CAP
{
  LatexDocument::LatexDocument()
  :
  LatexElement()
  { }

  LatexDocument::LatexDocument(const LatexDocument & src)
  :
  LatexElement(src)
  { }

//  LatexDocument &  LatexDocument::operator=(const LatexDocument & rhs)
//  {
//  if (rhs!=*this)
//    {
//    LatexElement::operator=(rhs);
//    }
//  return *this;
//  }

  void LatexDocument::create()
  {
  std::ofstream & p = openOutputAsciiFile(_outputPath,_outFileName,".tex",true);
  write(p);
  }

  void LatexDocument::writeHeader(std::ofstream & out)
  {
  if (_documentClassName.Length()<1)
    out << "\\documentclass{beamer}" << endl;
  else
    out << "\\documentclass{" << _documentClassName << "}" << endl;
  if (_themeName.Length()<1)
    out << "\\usetheme{Warsaw}" << endl;
  else
    out << "\\usetheme{" << _themeName << "}" << endl;
  skipLines(out,2);
  out << "\\title{" << _title << "}" << endl;
  if (_subtitle.Length()>0)  out << "\\subtitle{" << _subtitle << "}" << endl;
  out << "\\author{" << _author << "}" << endl;
  out << "\\institute{" << _institution << "}" << endl;
  if (_useToday)
    out << "\\date{\\today}" << endl;
  else
    out << "\\date{" << _date << "}" << endl;
  skipLines(out,2);
  out << "\\begin{document}" << endl;
  skipLines(out,2);
  out << "\\begin{frame}" << endl;
  out << "\\titlepage" << endl;
  out << "\\end{frame}" << endl;
  skipLines(out,2);
  }

  void LatexDocument::writeTrailer(std::ofstream & out)
  {
  skipLines(out,2);
  out << "\\end{document}" << endl;
  }

} // namespace CAP

