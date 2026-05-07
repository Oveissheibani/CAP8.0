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
#ifndef CAP__NuDynAnalyzer
#define CAP__NuDynAnalyzer
#include "EventProcessorSingle.hpp"
#include "NuDynHistos.hpp"
#include "NuDynDerivedHistos.hpp"

namespace CAP
{

//!
//!Task used for the determination of multiplicity moments of second, third, and fourth order. These moments
//!can then be used, in a "derived" stage, to compute cumulants of 2nd, 3rd, and 4th order, as well as other
//!variables such as the nu-dynamic variable.
//!As for other task classes of this package, the computation can be carried out for one or  several event class types (selected by the
//!user supplied event filters) and for one or several particle filters (selected by the user as particle filters). It is then possible to calculate
//!moments, cumulants, and various combinations of these for specific kinematic ranges or specific particle types or both.
//!The moments are computed as integrated averages and as function of some global event characteristic or property. The specific
//!property used as independent variable to compute (and eventually plot) the moments, cumulants, etc, is selected based on the  inputType
//!parameter selected at construction of an instance of this class.
//!
class NuDynAnalyzer
:
public EventProcessorSingle<NuDynHistos,NuDynDerivedHistos>
{
public:
  
  NuDynAnalyzer();
  NuDynAnalyzer(const NuDynAnalyzer & task);
  virtual ~NuDynAnalyzer() {}
  NuDynAnalyzer & operator=(const NuDynAnalyzer & task);
  virtual void setDefaultConfiguration();
  virtual void execute();
 
  ClassDef(NuDynAnalyzer,0)
};

} // namespace CAP

#endif /* CAP__NuDynAnalyzer */
