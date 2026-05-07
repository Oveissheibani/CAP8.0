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
#ifndef CAP__PtPtAnalyzer
#define CAP__PtPtAnalyzer
#include "EventProcessorSingle.hpp"
#include "PtPtHistos.hpp"
#include "PtPtDerivedHistos.hpp"

namespace CAP
{
//!
//!Task used for the determination of multiplicity moments of second, third, and fourth order. These moments
//!can then be used, in a "derived" stage, to compute cumulants of 2nd, 3rd, and 4th order, as well as other
//!variables such as the nu-dynamic variable.
//!As for other task classes of this package, the computation can be carried out for one or  several event class types (selected by the
//!user supplied event filters) and for one or several particle filters (selected by the user as particle filters). It is then possible to calculate
//!moments, cumulants, and various combinations of these for specific kinematic ranges or specific particle types or both.
//!
class PtPtAnalyzer
:
public EventProcessorSingle<PtPtHistos,PtPtDerivedHistos>
{
public:
  
  PtPtAnalyzer();
  PtPtAnalyzer(const PtPtAnalyzer & task);
  virtual ~PtPtAnalyzer() {}
  PtPtAnalyzer & operator=(const PtPtAnalyzer & task);
  virtual void setDefaultConfiguration();
  virtual void execute();

  ClassDef(PtPtAnalyzer,0)
};

} // namespace CAP

#endif /* CAP__PtPtAnalyzer */
