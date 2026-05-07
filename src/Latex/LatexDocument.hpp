#ifndef CAP__LatexDocument
#define CAP__LatexDocument
#include "LatexElement.hpp"
//#include "LatexFigure.hpp"
//#include "LatexFigureGroup.hpp"
//#include "LatexFrame.hpp"
//#include "LatexList.hpp"
//#include "LatexSection.hpp"
//#include "LatexSubsection.hpp"
//#include "LatexSubsubsection.hpp"
//#include "LatexText.hpp"

namespace CAP
{
  class LatexDocument : public LatexElement
  {
  public:

  LatexDocument();
  LatexDocument(const LatexDocument & src);
  LatexDocument  & operator=(const LatexDocument & rhs)
  {
//  if (rhs!=*this)
//    {
    LatexElement::operator=(rhs);
//    }
  return *this;
  }

  virtual ~LatexDocument() { }
  virtual void create();
  virtual void writeHeader(std::ofstream & out);
  virtual void writeTrailer(std::ofstream & out);

  const String & documentClassName() const { return  _documentClassName;}
  const String & themeName()         const { return  _themeName;}
  const String & author()            const { return  _author;}
  const String & institution()       const { return  _institution;}
  const String & title()             const { return  _title;}
  const String & subtitle()          const { return  _subtitle;}
  const String & date()              const { return  _date;}
  bool  useToday()                   const { return  _useToday;}
  const String & outputPath()        const { return  _outputPath;}
  const String & outFileName()       const { return  _outFileName;}
  std::ofstream & out()              const { return  *_out;}

  void setDocumentClassName(const String & v)  { _documentClassName = v;}
  void setThemeName(const String & v)          { _themeName = v;}
  void setAuthor(const String & v)             { _author = v;}
  void setInstitution(const String & v)        { _institution = v;}
  void setTitle(const String & v)              { _title = v;}
  void setSubtitle(const String & v)           { _subtitle = v;}
  void setDate(const String & v)               { _date = v;}
  void setUseToday(bool v)                     { _useToday = v;}
  void setOutputPath(const String & v)         { _outputPath = v;}
  void setOutFileName(const String & v)       { _outFileName = v;}

  protected:

  String _documentClassName;
  String _themeName;
  String _author;
  String _institution;
  String _title;
  String _subtitle;
  String _date;
  bool   _useToday;
  String _outputPath;
  String _outFileName;
  std::ofstream * _out;

  ClassDef(LatexDocument,0)

  };

} // namespace CAP

#endif // !CAP__LatexDocument
