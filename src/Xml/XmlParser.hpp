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
#ifndef CAP__XmlParser
#define CAP__XmlParser
#include <list>
#include "Parser.hpp"
#include "Aliases.hpp"
#include "XmlAttribute.hpp"
#include "XmlTag.hpp"

namespace CAP
{
class XmlDocument;

class XmlParser : public Parser
{
public:

  XmlParser();
  XmlParser(const XmlParser & parser);
  virtual ~XmlParser() {}
  XmlParser & operator=(const XmlParser & parser);

  virtual void read(XmlDocument & _xmlDocument,
                    const String & _inputPath,
                    const String & _inputFileName)  ;
  XmlTag* createTag(const String& aBuff);

  ClassDef(XmlParser,0)
};

} // namespace CAP

#endif // !CAP__XmlParser
