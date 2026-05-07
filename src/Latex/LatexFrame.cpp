#include "LatexEquation.hpp"
#include "LatexFrame.hpp"
#include "LatexFigure.hpp"
#include "LatexText.hpp"
#include "LatexList.hpp"
ClassImp(CAP::LatexFrame);

namespace CAP
{
  LatexFrame::LatexFrame()
  :
  LatexElement()
  { }

  LatexFrame::LatexFrame(const LatexFrame & src)
  :
  LatexElement(src)
  { }

  LatexFrame & LatexFrame::operator=(const LatexFrame & rhs)
  {
//  if (rhs!=*this)
//    {
    LatexElement::operator=(rhs);
//    }
  return *this;
  }

  void LatexFrame::writeHeader(std::ofstream & out)
  {
  skipLines(out,2);
  out << "\\begin{frame}" << endl;
  out << "\\frametitle{" << name() << "}" << endl;
  skipLines(out,1);
  }

  void LatexFrame::writeTrailer(std::ofstream & out)
  {
  skipLines(out,1);
  out << "\\end{frame}" << endl;
  skipLines(out,1);
  }

  void LatexFrame::addText(const String & text)
  {
  LatexText * t = new LatexText();
  t->setText(text);
  addChild(t);
  }

  void LatexFrame::addFigure(const String & figureName,
                         const String & figureCaption,
                         const String & figureLabel,
                         float scale,
                         float height)
  {
  LatexFigure * f = new LatexFigure();
  f->setName(figureName);
  f->setFileName(figureName);
  f->setCaption(figureCaption);
  f->setLabel(figureLabel);
  f->setScale(scale);
  f->setHeight(height);
  addChild(f);
  }

  void LatexFrame::addList(const std::vector<String> & items)
  {
  LatexList * list = new LatexList();
  list->addItems(items);
  addChild(list);
  }

  void LatexFrame::addEquation(const String & text,const String & equationLabel)
  {
  LatexEquation * eq = new LatexEquation();
  eq->setName(text);
  eq->setLabel(equationLabel);
  addChild(eq);
  }


} // namespace CAP

