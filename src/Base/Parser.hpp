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
#ifndef CAP__Parser
#define CAP__Parser
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <fstream>
#include <cmath>
#include <iomanip>
#include "Object.hpp"

namespace CAP
{

  class Parser
  :
  public Object
  {
  public:

  Parser();
  Parser(const Parser & parser);
  Parser & operator=(const Parser & parser);
  virtual ~Parser(){}

  protected:

  ClassDef(Parser,0)
  };

} // namespace CAP

#endif /* Parser_hpp */
