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
#ifndef CAP__RunSubSampleSum
#define CAP__RunSubSampleSum
#include "Aliases.hpp"

namespace CAP
{

class RunSubSampleSum 
{
public:

  RunSubSampleSum();
  virtual ~RunSubSampleSum();

  void run(String & histogramImportPathName,
           String & histogramImportFileName,
           String & histogramExportPathName,
           String & histogramExportFileName,
           bool verbose=true);

  ClassDef(RunSubSampleSum,0)
};

}    // namespace CAP

#endif /* CAP_RunSubSampleSum */


