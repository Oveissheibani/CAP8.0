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
#ifndef CAP__CommandLineParser
#define CAP__CommandLineParser
#include "Parser.hpp"

namespace CAP
{

class CommandLineParser : public Parser
{
public:

  CommandLineParser();
  CommandLineParser(const String & _name,
                const String & _title,
                const String & _version,
                const String & _preHash);
  CommandLineParser(const CommandLineParser & parser);
  virtual ~CommandLineParser(){}
  CommandLineParser & operator=(const CommandLineParser & parser);

  ClassDef(CommandLineParser,0)
};

}



#endif /* CAP__CommandLineParser */
