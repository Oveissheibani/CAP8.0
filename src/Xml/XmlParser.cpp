/* **********************************************************************
 * Copyright (C) 2019-2024, Claude Pruneau, Victor Gonzalez   
 * All rights reserved.
 *
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 *
 * Author: Claude Pruneau,   04/01/2024
 *
 * *********************************************************************/
#include "XmlParser.hpp"
#include "XmlDocument.hpp"
#include "StreamHelpers.hpp"

ClassImp(CAP::XmlParser);

namespace CAP
{

XmlParser::XmlParser()
:
Parser()
{ }

XmlParser::XmlParser(const XmlParser & parser)
:
Parser(parser)
{ }

XmlParser & XmlParser::operator=(const XmlParser & parser)
{
  if (this!=&parser)
    {
    Parser::operator=(parser);
    }
  return *this;
}


void XmlParser::read(XmlDocument & _xmlDocument,
                     const String & _inputPath,
                     const String & _inputFileName)
{
  long int filePosition;
  XmlTag* newTag;
  String textBuffer;
  char    chr;
  int     eofFlag;

  std::ifstream & inputFile = openInputAsciiFile(_inputPath,_inputFileName,".xml");
  if (reportDebug(__FUNCTION__))
    std::cout << "Parsing file "<< _inputPath + _inputFileName << std::endl;
  inputFile.seekg(0, std::ios::beg);
  while(!inputFile.eof())
    {
    textBuffer = "";
    eofFlag = 0;
    // find a TAG
    do {
      inputFile.get(chr);
      if(inputFile.eof())
        {
        eofFlag = -1; break;
        }
    } while (chr != '<');
    // no tag
    if(eofFlag==-1) break;

    // copy a TAG
    //  inputFile.unget();
    do {
      textBuffer += chr;
      if(chr=='[')
        { // DTD list
          do
            {
            inputFile.get(chr);
            textBuffer += chr;
            } while (chr!=']');
        }
      if(textBuffer.BeginsWith("<!--")) { // comment
        do {
          inputFile.get(chr);
          textBuffer += chr;
        } while ( !( textBuffer.EndsWith("--")  &&  (inputFile.peek()=='>') ) );
      }
      inputFile.get(chr);
    } while ( (chr!='>') );

    textBuffer += chr;
    textBuffer.ReplaceAll("\n"," ");
    filePosition = inputFile.tellg();
    // analyze TAG
    if(textBuffer.BeginsWith("<?")  &&  textBuffer.EndsWith("?>"))
      {
      if (reportDebug(__FUNCTION__))
        std::cout << "Xml declaration : "<<textBuffer<<"  skipping" << std::endl;
      }
    else if(textBuffer.BeginsWith("<!--")  &&  textBuffer.EndsWith("-->"))
      {
      if (reportDebug(__FUNCTION__))
        std::cout << "Xml commentary  : "<<textBuffer<<"  skipping" << std::endl;
      }
    else if(textBuffer.BeginsWith("<!"))
      {
      if (reportDebug(__FUNCTION__))
        std::cout << "Xml DTD         : "<<textBuffer<<"  skipping" << std::endl;
      }
    else if(textBuffer.BeginsWith("</"))
      {
      // CLOSE TAG
      textBuffer.ReplaceAll("</","");
      textBuffer.ReplaceAll(">","");
      if(_xmlDocument.currentTag->end !=-1)
        _xmlDocument.currentTag = _xmlDocument.currentTag->parent;
      // TAG content ends in file
      _xmlDocument.currentTag->end = filePosition - (textBuffer.Length() + 3);
      // Check if TAG name matches
      if(textBuffer.CompareTo(_xmlDocument.currentTag->name))
        {
        if (reportWarning(__FUNCTION__))
          {
          std::cout << std::endl;
          std::cout << "Closing tag name mismatch \""<< _xmlDocument.currentTag->name<<"\" != \""<<textBuffer<<"\"" << std::endl;
          std::cout << "Xml end tag     : </"<<_xmlDocument.currentTag->name<<"> @ "<<_xmlDocument.currentTag->end << std::endl;
          }
        }
      // TAG closed - go to parent
      }
    else if(textBuffer.EndsWith("/>"))
      {
      // NEW EMPTY TAG
      textBuffer.ReplaceAll("<","");
      textBuffer.ReplaceAll("/>","");
      newTag = createTag(textBuffer);
      newTag->begin = filePosition;
      newTag->end   = newTag->begin;
      // TAG structure BEGIN & END
      if(_xmlDocument.currentTag->end == -1) { // end TAG not found - go level down
        _xmlDocument.currentTag->child  = newTag;
        newTag->parent    = _xmlDocument.currentTag;
      }
      else
        { // add new sibling
          _xmlDocument.currentTag->next  = newTag;
          newTag->parent          = _xmlDocument.currentTag->parent;
          newTag->prev            = _xmlDocument.currentTag;
        }
      // TAG closed - go to parent
      _xmlDocument.currentTag = newTag;
      if (reportWarning(__FUNCTION__))
        {
        std::cout << std::endl;
        std::cout << "Xml empty tag...: <"<<_xmlDocument.currentTag->name<<" /> @ "<<_xmlDocument.currentTag->begin <<  std::endl;
        }
      if (reportDebug(__FUNCTION__))
        {
        std::cout << std::endl;
        for(std::list<XmlAttribute>::iterator iter = _xmlDocument.currentTag->attributes.begin(); iter != _xmlDocument.currentTag->attributes.end(); iter++)
          std::cout << "  attribute.....:  "<<iter->name<<"=\""<<iter->value<<"\"" << std::endl;
        }
      }
    else
      {
      // NEW TAG
      textBuffer.ReplaceAll("<","");
      textBuffer.ReplaceAll(">","");
      newTag = createTag(textBuffer);
      // TAG content begins in file
      newTag->begin = filePosition;
      // TAG structure BEGIN
      if(_xmlDocument.topTag == NULL)
        { // create main tag
          _xmlDocument.topTag = newTag;
        }
      else
        {
        if(_xmlDocument.currentTag->end == -1)
          { // end TAG not found - go level down
            _xmlDocument.currentTag->child  = newTag;
            newTag->parent           = _xmlDocument.currentTag;
          }
        else
          { // add new sibling
            _xmlDocument.currentTag->next  = newTag;
            newTag->parent    = _xmlDocument.currentTag->parent;
            newTag->prev      = _xmlDocument.currentTag;
          }
        }
      _xmlDocument.currentTag = newTag;
      if (reportDebug(__FUNCTION__))
        {
        std::cout << std::endl;
        std::cout << "Xml begin tag.....: <"<<_xmlDocument.currentTag->name<<"> @ "<<_xmlDocument.currentTag->begin << std::endl;
        for(std::list<XmlAttribute>::iterator iter = _xmlDocument.currentTag->attributes.begin(); iter != _xmlDocument.currentTag->attributes.end(); iter++)
          std::cout << "  attribute.....:  "<<iter->name<<"=\""<<iter->value<<"\"" << std::endl;
        }
      }
    }
}

CAP::XmlTag* XmlParser::createTag(const String & aBuff)
{
  XmlTag *  newTag;
  XmlAttribute  newAttribute;
  int    i;

  newTag = new XmlTag;
  newTag->name = "";
  newTag->attributes.clear();
  newTag->begin  = -1;
  newTag->end    = -1;
  newTag->parent = NULL;
  newTag->child  = NULL;
  newTag->prev   = NULL;
  newTag->next   = NULL;

  // TAG name
  for(i=0; i<aBuff.Length(); i++)
    {
    if(aBuff[i] == ' ') break;
    newTag->name += aBuff[i];
    }
  // TAG attributes
  while(i<aBuff.Length())
    {
    if((aBuff[i] != ' ')  &&  (aBuff[i] != '\t')  &&  (aBuff[i] != '/'))
      {
      newAttribute.name  = "";
      newAttribute.value = "";
      while((aBuff[i] != '='))
        {
        newAttribute.name += aBuff[i];
        i++;
        }
      i += 2;
      while((aBuff[i] != '\"')  &&  (aBuff[i] != '\''))
        {
        newAttribute.value += aBuff[i];
        i++;
        }
      newTag->attributes.push_back(newAttribute);
    }
    i++;
    }
  return newTag;
}

} // namespace CAP
