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
#ifndef CAP__RunClosure
#define CAP__RunClosure
#include "Aliases.hpp"

namespace CAP
{

class RunClosureAnalysis
{
public:

  RunClosureAnalysis();
  virtual ~RunClosureAnalysis();

  void run(String & histogramImportPathName,
           String & histogramImportGenFileName,
           String & histogramImportRecoFileName,
           String & histogramExportPathName,
           String & histogramExportFileName,
           bool verbose=true);

  ClassDef(RunClosureAnalysis,0)
};

}    // namespace CAP

#endif /* CAP_RunClosure */


