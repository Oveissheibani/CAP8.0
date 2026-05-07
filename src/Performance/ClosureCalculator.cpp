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
#include "ClosureCalculator.hpp"
#include "PrintHelpers.hpp"
#include "NameHelpers.hpp"
#include "RootFileHelpers.hpp"
#include "RootHistogramHelpers.hpp"
#include "HistogramGroup.hpp"

ClassImp(CAP::ClosureCalculator);

namespace CAP
{

ClosureCalculator::ClosureCalculator()
:
Task()
{
  appendClassName("ClosureCalculator");
  setMinimumReportLevel(Object::kInfo);
}

void ClosureCalculator::setDefaultConfiguration()
{
  Task::setDefaultConfiguration();
  const String & taskName = name();
  String objectType = "HISTOGRAM";
  _configuration.addProperty(createKey(taskName,objectType,"METHOD"),int(0));
  _configuration.addProperty(createKey(taskName,objectType,"IMPORT:PATH"),"DEFAULT");
  _configuration.addProperty(createKey(taskName,objectType,"IMPORT:GEN_FILE_NAME"),"NONE");
  _configuration.addProperty(createKey(taskName,objectType,"IMPORT:REC_FILE_NAME"),"NONE");
  _configuration.addProperty(createKey(taskName,objectType,"EXPORT:PATH"),"DEFAULT");
  _configuration.addProperty(createKey(taskName,objectType,"EXPORT:CLOSURE_FILE_NAME"),"NONE");
}

void ClosureCalculator::execute()
{
  if (reportInfo(__FUNCTION__)) { printCR(); }
  const String & taskName = name();
  String objectType = "HISTOGRAM";
  int    method = _configuration.valueInt(createKey(taskName,objectType,"METHOD"),1,0);
  String importPath         = _configuration.valueString(createKey(taskName,objectType,"IMPORT:PATH"));
  String importGenFileName  = _configuration.valueString(createKey(taskName,objectType,"IMPORT:GEN_FILE_NAME"));
  String importRecoFileName = _configuration.valueString(createKey(taskName,objectType,"IMPORT:REC_FILE_NAME"));
  String exportPath         = _configuration.valueString(createKey(taskName,objectType,"EXPORT:PATH"));
  String exportFileName     = _configuration.valueString(createKey(taskName,objectType,"EXPORT:CLOSURE_FILE_NAME"));

  if (reportInfo(__FUNCTION__))
    {
    printCR();
    printValue(createKey(taskName,objectType,"METHOD"),method);
    printValue(createKey(taskName,objectType,"IMPORT:PATH"),importPath);
    printValue(createKey(taskName,objectType,"IMPORT:GEN_FILE_NAME"),importGenFileName);
    printValue(createKey(taskName,objectType,"IMPORT:REC_FILE_NAME"),importRecoFileName);
    printValue(createKey(taskName,objectType,"EXPORT:PATH"),exportPath);
    printValue(createKey(taskName,objectType,"EXPORT:CLOSURE_FILE_NAME"),exportFileName);
    }
  TFile & generatorFile = *openOldRootFile(importPath,importGenFileName);
  TFile & detectorFile  = *openOldRootFile(importPath,importRecoFileName);
  TFile * closureFile = nullptr; // fix me
//  if (histogramsForceRewrite())
//    closureFile = openRecreateRootFile(importPath,exportFileName);
//  else
//    closureFile = openNewRootFile(importPath,exportFileName);

  HistogramGroup * generator = new HistogramGroup();
  HistogramGroup * detector  = new HistogramGroup();
  HistogramGroup * closure   = new HistogramGroup();
  generator->setParentTask(this);
  generator->setName("GeneratorLevel");
  generator->loadList(generatorFile);
  detector->setParentTask(this);
  detector->setName("DetectorLevel");
  detector->loadList(detectorFile);
  //detector->setOwnership(false);
  closure->setName("Closure");
  closure->setParentTask(this);
  //closure->setOwnership(false);
   switch (method)
    {
      case 0: printValue("method",0); break;
      case 1: printValue("method",1); break;
//      case 0: closure->differenceGroup(*detector,*generator,true); break;
//      case 1: closure->ratioHistogramGroups(*detector,*generator,true); break;
    }
  closure->saveTo(*closureFile);
  //TaskAccountant::exportNEexecutedTask(*closureFile);
  generatorFile.Close();
  detectorFile.Close();
  closureFile->Close();
  delete generator;
  delete detector;
  delete closure;
  if (reportInfo(__FUNCTION__))
    {
    printCR();
    printString("Closure Test Completed.");
    }
}

} // namespace CAP
