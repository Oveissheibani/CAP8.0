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
#include "BaseHistos.hpp"
#include "NameHelpers.hpp"
#include "PrintHelpers.hpp"
#include "Task.hpp"
#include "Configuration.hpp"
#include "NameHelpers.hpp"
#include "RootHistogramHelpers.hpp"

ClassImp(CAP::BaseHistos);

namespace CAP
{

BaseHistos::BaseHistos()
:
HistogramGroup()
{
  appendClassName("BaseHistos");
  setMinimumReportLevel(Object::kInfo);
  setName("BaseHistos");
  setTitle("BaseHistos");
}

BaseHistos::BaseHistos(const BaseHistos & source)
:
HistogramGroup(source)
{
  cloneB(source);
}

BaseHistos & BaseHistos::operator=(const BaseHistos & rhs)
{
  if (reportWarning(__FUNCTION__)) { printCR(); }
  if (this!=&rhs)
    {
    HistogramGroup::operator=(rhs);
    cloneB(rhs);
    }
  return *this;
}

void BaseHistos::cloneB(const BaseHistos & source __attribute__((unused)) )
{
  if (reportWarning(__FUNCTION__)) { printCR(); }
}

} // namespace CAP
