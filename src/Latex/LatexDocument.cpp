#include "Exceptions.hpp"
#include "LatexDocument.hpp"
#include "PrintHelpers.hpp"
#include "RootFileHelpers.hpp"
#include "StreamHelpers.hpp"
#include <iostream>

ClassImp(CAP::LatexDocument);
using std::cout;
using std::endl;

namespace CAP
{
  LatexDocument::LatexDocument()
  :
  LatexElement(),
  _documentClassName("beamer"),
  _themeName("Warsaw"),
  _title("Lorem ipsum"),
  _subtitle("Latex Galore"),
  _date("05/05/2026"),
  _useToday(false),
  _outputPath("./"),
  _outFileName("LoremIpsum.tex"),
  _out(nullptr),
  _packages(),
  _commands(),
  _authors(),
  _abstract("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Aenean facilisis leo vitae enim congue, non faucibus dolor aliquam. Proin cursus imperdiet sem. Donec volutpat purus fringilla ex iaculis accumsan."),
  _currentScope(this)
  { }

  LatexDocument::LatexDocument(const LatexDocument & src)
  :
  LatexElement(src),
  _documentClassName(src._documentClassName),
  _themeName(src._themeName),
  _title(src._title),
  _subtitle(src._subtitle),
  _date(src._date),
  _useToday(src._useToday),
  _outputPath(src._outputPath),
  _outFileName(src._outFileName),
  _out(src._out),
  _packages(src._packages),
  _commands(src._commands),
  _authors(src._authors),
  _abstract(src._abstract),
  _currentScope(src._currentScope)
  { }

  LatexDocument &  LatexDocument::operator=(const LatexDocument & rhs)
  {
//  if (rhs!=*this)
//    {
   LatexElement::operator=(rhs);
  _documentClassName = rhs._documentClassName;
  _themeName         = rhs._themeName;
  _title             = rhs._title;
  _subtitle          = rhs._subtitle;
  _date              = rhs._date;
  _useToday          = rhs._useToday;
  _outputPath        = rhs._outputPath;
  _outFileName       = rhs._outFileName;
  _out               = rhs._out;
  _packages          = rhs._packages;
  _commands          = rhs._commands;
  _authors           = rhs._authors;
  _abstract          = rhs._abstract;
  _currentScope      = rhs._currentScope;
  return *this;
  }

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
  for (auto author : _authors) author->write(out);
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

  void LatexDocument::writePackages(std::ofstream & out)
  {
  for (auto package : _packages) package->write(out);
  }

  void LatexDocument::writeCommands(std::ofstream & out)
  {
  for(auto command : _commands) command->write(out);
  }

  void LatexDocument::writeAuthors(std::ofstream & out)
  {
  for(auto author : _authors) author->write(out);
  }

  void LatexDocument::writeAbstract(std::ofstream & out)
  {
  out << "\\begin{abstract}" << endl;
  out << _abstract << endl;
  out << "\\end{abstract}" << endl;
  }

  LatexPackage & LatexDocument::addPackage(const String & name, const String & option)
  {
  LatexPackage * package = new LatexPackage();
  package->setName(name);
  package->setOption(option);
  _packages.push_back(package);
  return *package;
  }


  LatexCommand & LatexDocument::addCommand(const String & keyword, const String & command)
  {
  LatexCommand * comm = new LatexCommand();
  comm->setKeyword(keyword);
  comm->setCommand(command);
  _commands.push_back(comm);
  return *comm;
  }


  LatexAuthor &  LatexDocument::addAuthor(const String & name, const String & email, const String & affiliation)
  {
  LatexAuthor * author = new LatexAuthor();
  author->setName(name);
  author->setEmail(email);
  author->setAffiliation(affiliation);
  _authors.push_back(author);
  return *author;
  }



  void LatexDocument::setScopeToParent()
  {
  LatexElement * p = _currentScope->parent();
  printValue("setScopeToParent() Current Scope",_currentScope->name());
  if (p)
    {
    printValue("setScopeToParent() Parent name",p->name());
    _currentScope = p;
    }
  printValue("setScopeToParent() Current Scope",_currentScope->name());
  }

  LatexFrame & LatexDocument::addFrame(const String & title)
  {
  LatexFrame * f = new LatexFrame();
  f->setTitle(title);
  printValue("addFrame(0) title",currentScope()->name());
  currentScope()->addChild(f);
  setCurrentScope(f);
  printValue("addFrame(1) title",currentScope()->name());
  return *f;
  }

  LatexSection & LatexDocument::addSection(const String & title, const String & label)
  {
  LatexSection * s =  new LatexSection();
  s->setTitle(title);
  s->setLabel(label);
  printValue("addSection() title",title);
  printValue("addSection   currentScope() name",currentScope()->name());
  currentScope()->addChild(s);
  setCurrentScope(s);
  printValue("addSection   currentScope() name",currentScope()->name());
  return *s;
  }

  LatexSubsection & LatexDocument::addSubection(const String & title, const String & label)
  {
  LatexSubsection * s = new LatexSubsection();
  s->setTitle(title);
  s->setLabel(label);
  printValue("addSubection() title",title);
  printValue("addSubection   currentScope() name",currentScope()->name());
  currentScope()->addChild(s);
  setCurrentScope(s);
  printValue("addSubection   currentScope() name",currentScope()->name());
  return *s;
  }

  LatexSubsubsection &  LatexDocument::addSubsubsection(const String & title, const String & label)
  {
  LatexSubsubsection * s = new LatexSubsubsection();
  s->setTitle(title);
  s->setLabel(label);
  printValue("addSubsubsection() title",title);
  printValue("addSubsubsection   currentScope() name",currentScope()->name());
  currentScope()->addChild(s);
  setCurrentScope(s);
  printValue("addSubsubsection   currentScope() name",currentScope()->name());

  return *s;
  }

  LatexParagraph & LatexDocument::addParagraph(const String & title, const String & label)
  {
  LatexParagraph * s = new LatexParagraph();
  s->setTitle(title);
  s->setLabel(label);
  currentScope()->addChild(s);
  setCurrentScope(s);
  return *s;
  }

  LatexEquation & LatexDocument::addEquation(const String & text, const String & label)
  {
  LatexEquation * equation = new LatexEquation();
  equation->setText(text);
  equation->setLabel(label);
  currentScope()->addChild(equation);
  return *equation;
  }


  LatexFigure &  LatexDocument::addFigure(const String & name, const String & label, const String & caption)
  {
  LatexFigure * figure = new LatexFigure();
  figure->setName(name);
  figure->setLabel(label);
  figure->setCaption(caption);
  currentScope()->addChild(figure);
  return *figure;
  }

  LatexText &  LatexDocument::addText(const String & text)
  {
  LatexText * t = new LatexText();
  t->setText(text);
  currentScope()->addChild(t);
  return *t;
  }

  void LatexDocument::endSection()
  {
  setScopeToParent();
  }

  void LatexDocument::addPackage(LatexPackage * package)
  {
  _packages.push_back(package);
  }

  void LatexDocument::addCommand(LatexCommand * command)
  {
  _commands.push_back(command);
  }

  void LatexDocument::addAuthor(LatexAuthor * author)
  {
  _authors.push_back(author);
  }

  void LatexDocument::addPackages(const std::vector<LatexPackage*> & packages)
  {
  for(auto package : packages) _packages.push_back(package);
  }

  void LatexDocument::addCommands(const std::vector<LatexCommand*> & commands)
  {
  for(auto command : commands) _commands.push_back(command);
  }

  String & LatexDocument::addAbstract(const String & text)
  {
  _abstract = text;
  return _abstract;
  }

  void LatexDocument::help()
  {
  cout << "======================================" << endl;
  cout << "LatexDocument::help()" << endl;
  cout << "======================================" << endl;
  cout << "Known DocumentClasses/Styles" << endl;
  cout << "-- beamer    : creation of presentations (frame)"<< endl;
  cout << "  -- Warsaw  : theme Warsaw"<< endl;
  cout << endl;
  cout << "-- aps    : aps style papers"<< endl;
  cout << "-- prc    : prc style papers"<< endl;
  cout << "-- prl    : prlc style papers"<< endl;
  }

  void LatexDocument::addFigures(const String & path, const std::vector<String> & fileList)
  {
  for (auto file : fileList)
    {
    addFrame(file);
    addFigure(path+file,file,file);
    endSection();
    }
  }


  void LatexDocument::addFiguresFrom(const String & path, const String & ext, int depth)
  {
  bool verbose = true;
  // top directory first
  std::vector<String> fileList = listFilesInDir(path,ext,false,verbose,depth,0);
  if (verbose)
    {
    printCR();
    printValue("fileList.size()",fileList.size());
    }
  addFigures(path,fileList);

  // add subdirectories (if any) as subsections.
  std::vector<String> directories = listDirsIn(path,verbose);
  int nDirectories = directories.size();
  if (verbose)
    {
    printCR();
    printValue("directories.size()",nDirectories);
    }
  for (auto dir : directories)
    {
    printValue("dir", dir);
    String subPath = path;
    subPath += "/";
    subPath += dir;
    subPath += "/";
    addSubection(dir,dir);
    addFiguresFrom(subPath,"pdf",0);
    endSection();
    }
  }

} // namespace CAP

