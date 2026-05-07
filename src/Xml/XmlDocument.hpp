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
#ifndef CAP__XmlDocument
#define CAP__XmlDocument
#include "XmlParser.hpp"

namespace CAP
{

class XmlDocument
{
public:

  friend XmlParser;

  XmlDocument();
  XmlDocument(const String & _inputPath,
              const String & _inputFileName);
  XmlDocument(const String & _inputFile);
  virtual ~XmlDocument();

  virtual void  read(const String & _inputPath,
                    const String & _inputFileName) ;
  virtual void  read() ;

  XmlDocument* getXmlTag(const char * aTagName, const char * aAttName, const char * aAttVal);
  String   getXmlAttribute(const XmlTag & tag, const char* aAttName) const;
  String   getXmlAttribute(const char* aAttName) const;
  String   getXmlContent(const XmlTag & tag) const;
  String   getXmlContent() const;
  String   getXmlTagDescription();
  //void      getXmlVector3D();
  //ScalarField* getXmlVector3D();
  bool checkAttributeMatch(const char* aAttName, const char* aAttVal) const;

protected:

  String inputPath;
  String inputFileName;
  XmlTag * topTag;
  XmlTag * currentTag;

  ClassDef(XmlDocument,0)

};



} // namespace CAP

#endif /* XmlDocument */
