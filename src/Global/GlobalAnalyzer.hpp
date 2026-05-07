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
#ifndef CAP__GlobalAnalyzer
#define CAP__GlobalAnalyzer
#include "EventProcessorSingle.hpp"
#include "GlobalHistos.hpp"
#include "GlobalDerivedHistos.hpp"

namespace CAP
{

//!
//! This class implements a basic analyzer of the multiplicity, energy, net charge, net strangeness, and net baryon number of the particles composing an event.
//! A global analyzer task can be used to determine the multiplcity (or other characteristics) of an event and set the event record with these values. The values
//! then become available to other tasks, and the event filters they operate,  to decide whether the analysis of an event should be carried out. The task can be
//! operated with several event filters and it is then possible to determine the distribution of multiplicity, energy, net charge, etc, based on specific event filters
//! (e.g., multiplicity or collision centrality classes). The task can  be operated with one or more particle filters. It is then possible to determine the multiplicity of
//! specific types of particles or the yield in specific pseudorapidity, transverse momentum ranges (etc).  When several event filters are used, only the first (index zero)
//! is used to set the event property record of an event. Note that the HistogramsCreate parameter must be set to YES (true) to fill histograms of this class.
//! This means that an instance of this class can be used to analyze the multiplicity (energy, etc) of events with or without filling corresponding histograms. It is
//! then possible to set the event property record only (base on event filter index 0), fill histograms only, or both set the record and fill histograms.
//!
class GlobalAnalyzer
:
public EventProcessorSingle<GlobalHistos,GlobalDerivedHistos>
{
public:

  GlobalAnalyzer();
  GlobalAnalyzer(const GlobalAnalyzer & analyzer);
  GlobalAnalyzer & operator=(const GlobalAnalyzer & analyzer);
  virtual ~GlobalAnalyzer() {}
  virtual void setDefaultConfiguration();
  virtual void execute();

  ClassDef(GlobalAnalyzer,0)
};

}

#endif /* CAP__GlobalAnalyzer */
