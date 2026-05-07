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
#ifndef CAP__PropertiesParser
#define CAP__PropertiesParser
#include "Parser.hpp"
#include "Properties.hpp"

namespace CAP
{
class PropertiesParser
  :
  public Parser
{
public:

  PropertiesParser();
  virtual ~PropertiesParser() {}

  virtual void  loadFrom(Properties & _properties,
                             const String & _inputPath,
                             const String & _inputFileName,
                             const String & _extension = ".ini") ;
  virtual void  saveTo(Properties & _properties,
                             const String & _path,
                             const String & _fileName,
                             const String & _extension) ;

  ClassDef(PropertiesParser,0)

};

} // namespace CAP

#endif /* PropertiesParser_hpp */
