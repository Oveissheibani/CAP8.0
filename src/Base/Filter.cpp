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
#include "Filter.hpp"

namespace CAP
{

FilterException::FilterException(const String & _message,
                                 const String & _source)
:
Exception(_message,_source,__FUNCTION__)
{   }

} // namespace CAP

