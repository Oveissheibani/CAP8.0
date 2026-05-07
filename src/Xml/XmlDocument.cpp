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
#include "XmlDocument.hpp"
#include "StreamHelpers.hpp"
#include "Exceptions.hpp"

ClassImp(CAP::XmlDocument);

namespace CAP
{

XmlDocument::XmlDocument()
:
inputPath(),
inputFileName(),
topTag(nullptr),
currentTag(nullptr)
{   }

XmlDocument::XmlDocument(const String & _inputPath,
                         const String & _inputFileName)
:
inputPath(_inputPath),
inputFileName(_inputFileName),
topTag(nullptr),
currentTag(nullptr)
{   }

XmlDocument::XmlDocument(const String & _inputFile)
:
inputPath(),
inputFileName(_inputFile),
topTag(nullptr),
currentTag(nullptr)
{   }

XmlDocument::~XmlDocument()
{
  if (topTag)
    {
    currentTag = topTag;
    while(topTag->child)
      { // main Xml tag only has children, next=0
      if(currentTag->next == 0)
        if(currentTag->child == 0)
          if(currentTag->prev == 0) { //last child
            currentTag = currentTag->parent;
            delete (currentTag->child);
            currentTag->child = 0;
          } else {
            currentTag = currentTag->prev;
            delete (currentTag->next);
            currentTag->next = 0;
          }
          else
            currentTag = currentTag->child;
          else
            currentTag = currentTag->next;
    }
    delete topTag;
  }
}



void  XmlDocument::read(const String & _inputPath,
                       const String & _inputFileName)
{
  inputPath = _inputPath;
  inputFileName = _inputFileName;
  XmlParser parser;
  parser.read(*this,inputPath,inputFileName);
}

void  XmlDocument::read()
{
  XmlParser parser;
  parser.read(*this,inputPath,inputFileName);
}

XmlDocument* XmlDocument::getXmlTag(const char* aTagName,
                                    const char* aAttName,
                                    const char* aAttVal)
{
  if(!topTag) throw Exception("Document is empty",__FUNCTION__);
  XmlTag* tempTag;
  currentTag = topTag;
  if((currentTag->child == 0)  && 
     (currentTag->name != aTagName)  && 
     (!checkAttributeMatch(aAttName,aAttVal)))
    {
    throw Exception("Document with only one entry or unmatched",__FUNCTION__);
    }

  tempTag    = topTag;
  currentTag = topTag->child;
  while(currentTag != topTag)
    {
    if((currentTag->name == aTagName)
       &&  (currentTag->attributes.empty() || checkAttributeMatch(aAttName,aAttVal)))
      {
      return (this);
      // currentTag is set to the requested TAG.
      // getXmlAttribute(), getXmlContent()  calls available.
      }
    else
      if(currentTag == tempTag)  // moving level up
        if(currentTag->next == 0)
          {
          currentTag = currentTag->parent;
          tempTag = currentTag;
          }
        else
          currentTag = currentTag->next;
        else      // moving level down
          if(currentTag->child == 0)
            if(currentTag->next == 0)
              {
              currentTag = currentTag->parent;
              tempTag = currentTag;
              }
            else
              currentTag = currentTag->next;
            else
              {
              tempTag = currentTag;
              currentTag = currentTag->child;
              }
    }
  throw Exception("Requested tag/attribute/value not found",__FUNCTION__);
}

// Get the attribute name attributeName from the given tag
String XmlDocument::getXmlAttribute(const XmlTag & tag, const char* attributeName) const
{
  std::list<XmlAttribute>::const_iterator iter;
  for(iter=tag.attributes.begin(); iter!=tag.attributes.end(); iter++)
    {
    if (iter->name == attributeName) return iter->value;
    }
  throw InvalidNameException(attributeName,"Attribute Not Found",__FUNCTION__);
}

String XmlDocument::getXmlAttribute(const char* attributeName) const
{
  if(!currentTag) throw Exception(String("Attempting to get attribute of a null tag (currentTag)"),__FUNCTION__);
  return getXmlAttribute(*currentTag,attributeName);
}

String XmlDocument::getXmlContent(const XmlTag & tag) const
{
  XmlParser parser;
  std::ifstream & inputFile = openInputAsciiFile(inputPath,inputFileName,".xml");
  inputFile.seekg(tag.begin);
  String buffer = "";
  char   chr;
  while(static_cast<long>(inputFile.tellg()) != tag.end)
    {
    inputFile.get(chr);
    buffer += chr;
    }
  inputFile.close();
  return buffer;
}


String XmlDocument::getXmlContent()  const
{
  if(!currentTag) throw Exception("Attempting to get content of a null tag (currentTag)",__FUNCTION__);
  return getXmlContent(*currentTag);
}


String XmlDocument::getXmlTagDescription()
{
  String tagDescription;
  if(!currentTag)
    {
    tagDescription = "<NULL />";
    return tagDescription;
    }
  tagDescription = "<" + currentTag->name;
  for (std::list<XmlAttribute>::iterator iter = currentTag->attributes.begin(); iter != currentTag->attributes.end(); iter++)
    tagDescription += " " + iter->name + "=\"" + iter->value + "\"";
  tagDescription += ">\n\t@:";
  tagDescription += currentTag->begin;
  tagDescription += " - ";
  tagDescription += currentTag->end;
  if(currentTag->parent != NULL)
    tagDescription += "\n\tF:" + currentTag->parent->name;
  else
    tagDescription += "\n\tF:<NULL />";
  if(currentTag->child != NULL)
    tagDescription += "\n\tC:" + currentTag->child->name;
  else
    tagDescription += "\n\tC:<NULL />";
  if(currentTag->prev != NULL)
    tagDescription += "\n\tP:" + currentTag->prev->name;
  else
    tagDescription += "\n\tP:<NULL />";
  if(currentTag->next != NULL)
    tagDescription += "\n\tN:" + currentTag->next->name;
  else
    tagDescription += "\n\tN:<NULL />";
  return tagDescription;
}

bool XmlDocument::checkAttributeMatch(const char* aAttName, const char* aAttVal) const
{
  std::list<XmlAttribute>::const_iterator iter;
  for (iter = currentTag->attributes.begin(); iter != currentTag->attributes.end(); iter++)
    if (iter->name.Contains(aAttName)  &&  iter->value.Contains(aAttVal))
      return true;
  return false;
}

} // namespace CAP

