#ifndef CAP__LatexDocument
#define CAP__LatexDocument
#include "LatexAuthor.hpp"
#include "LatexCommand.hpp"
#include "LatexElement.hpp"
#include "LatexEquation.hpp"
#include "LatexFigure.hpp"
#include "LatexFrame.hpp"
#include "LatexPackage.hpp"
#include "LatexParagraph.hpp"
#include "LatexSection.hpp"
#include "LatexSubsection.hpp"
#include "LatexSubsubsection.hpp"
#include "LatexText.hpp"

namespace CAP
{
  class LatexDocument : public LatexElement
  {
  public:

  LatexDocument();
  LatexDocument(const LatexDocument & src);
  LatexDocument  & operator=(const LatexDocument & rhs);
  virtual ~LatexDocument() { }
  virtual void create();
  virtual void writeHeader(std::ofstream & out);
  virtual void writeTrailer(std::ofstream & out);
  virtual void writePackages(std::ofstream & out);
  virtual void writeCommands(std::ofstream & out);
  virtual void writeAuthors(std::ofstream & out);
  virtual void writeAbstract(std::ofstream & out);
  virtual void help();

  const String & documentClassName() const { return  _documentClassName;}
  const String & themeName()         const { return  _themeName;}
  const String & title()             const { return  _title;}
  const String & subtitle()          const { return  _subtitle;}
  const String & date()              const { return  _date;}
  bool  useToday()                   const { return  _useToday;}
  const String & outputPath()        const { return  _outputPath;}
  const String & outFileName()       const { return  _outFileName;}
  std::ofstream & out()              const { return  *_out;}
  const String & abstract()          const { return  _abstract;}
  LatexElement * currentScope()            { return  _currentScope; }

  const std::vector<LatexPackage*> & packages() const { return _packages;}
  const std::vector<LatexCommand*> & commands() const { return _commands;}
  const std::vector<LatexAuthor*> &  authors()  const { return _authors;}

  void setDocumentClassName(const String & v)  { _documentClassName = v;}
  void setThemeName(const String & v)          { _themeName = v;}
  void setTitle(const String & v)              { _title = v;}
  void setSubtitle(const String & v)           { _subtitle = v;}
  void setDate(const String & v)               { _date = v;}
  void setUseToday(bool v)                     { _useToday = v;}
  void setOutputPath(const String & v)         { _outputPath = v;}
  void setOutFileName(const String & v)        { _outFileName = v;}
  void setCurrentScope(LatexElement * newScope){ _currentScope = newScope; }
  void setScopeToParent();

  LatexPackage &       addPackage(const String & name, const String & option);
  LatexCommand &       addCommand(const String & keyword, const String & command);
  LatexAuthor &        addAuthor(const String & name, const String & email, const String & affiliation);
  String &             addAbstract(const String & text);
  LatexFrame &         addFrame(const String & title);
  LatexSection &       addSection(const String & title, const String & label);
  LatexSubsection &    addSubection(const String & title, const String & label);
  LatexSubsubsection & addSubsubsection(const String & title, const String & label);
  LatexParagraph &     addParagraph(const String & title, const String & label);
  LatexEquation &      addEquation(const String & text, const String & label);
  LatexFigure &        addFigure(const String & name, const String & label, const String & caption);
  void                 addFigures(const String & path, const std::vector<String> & fileNames);
  void                 addFiguresFrom(const String & path, const String & ext=".pdf", int depth=1);
  LatexText &          addText(const String & text);
  void endSection();


  void addPackage(LatexPackage * package);
  void addCommand(LatexCommand * command);
  void addAuthor(LatexAuthor * author);
  void addPackages(const std::vector<LatexPackage*> & package);
  void addCommands(const std::vector<LatexCommand*> & commands);
  void addAuthors(const std::vector<LatexAuthor*> & authors);

  protected:

  String _documentClassName;
  String _themeName;
  String _title;
  String _subtitle;
  String _date;
  bool   _useToday;
  String _outputPath;
  String _outFileName;
  std::ofstream * _out;
  std::vector<LatexPackage*> _packages;
  std::vector<LatexCommand*> _commands;
  std::vector<LatexAuthor*>  _authors;
  String _abstract;
  LatexElement * _currentScope;
 
  ClassDef(LatexDocument,0)

  };

} // namespace CAP

#endif // !CAP__LatexDocument
