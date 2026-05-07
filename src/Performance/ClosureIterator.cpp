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
//#include "HistogramCollection.hpp"
#include "ClosureIterator.hpp"
#include "ClosureCalculator.hpp"
#include "PrintHelpers.hpp"
#include "NameHelpers.hpp"
#include "RootHistogramHelpers.hpp"
#include "RootFileHelpers.hpp"
#include "Exceptions.hpp"

ClassImp(CAP::ClosureIterator);

namespace CAP
{

ClosureIterator::ClosureIterator()
:
Task()
{
  appendClassName("ClosureIterator");
  setMinimumReportLevel(Object::kInfo);
  setName("ClosureIterator");
  setTitle("ClosureIterator");
}

void ClosureIterator::setDefaultConfiguration()
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
  generateKeyValuePairs("IncludedPattern","NONE",20);
  generateKeyValuePairs("ExcludedPattern","NONE",20);
}

void ClosureIterator::execute()
{
  if (reportStart(__FUNCTION__)) { /* no ops*/ };
  const String & taskName = name();
  String objectType = "HISTOGRAM";
  String none  = "none";
  int    method             = _configuration.valueInt(createKey(taskName,objectType,"METHOD"),1,0);
  String importPath         = _configuration.valueString(createKey(taskName,objectType,"IMPORT:PATH"));
  String importGenFileName  = _configuration.valueString(createKey(taskName,objectType,"IMPORT:GEN_FILE_NAME"));
  String importRecoFileName = _configuration.valueString(createKey(taskName,objectType,"IMPORT:REC_FILE_NAME"));
  String exportPath         = _configuration.valueString(createKey(taskName,objectType,"EXPORT:PATH"));
  String exportFileName     = _configuration.valueString(createKey(taskName,objectType,"EXPORT:CLOSURE_FILE_NAME"));

  for (auto & task : _subTasks)
    {
    Task & subTask     = *task;
    String subTaskName = subTask.name();
    std::vector<String>  includedPatterns = selectedValues(createKey(taskName,objectType,"IncludedPattern"),none);
    std::vector<String>  excludedPatterns = selectedValues(createKey(taskName,objectType,"ExcludedPattern"),none);
    excludedPatterns.push_back("Reco");
    std::vector<String>  allFilesToProcess = listFilesInDir(importPath,includedPatterns,excludedPatterns);
    int nFilesToProcess = allFilesToProcess.size();
    if (nFilesToProcess<1) throw Exception("No selected files",__FUNCTION__);
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
      for (auto & name : allFilesToProcess)
      {
      String histoGeneratorFileName = removeRootExtension(name);
      String histoDetectorFileName  = substitute(histoGeneratorFileName, "_Gen", "_Reco");
      String histoClosureFileName   = substitute(histoGeneratorFileName, "_Gen", "_Closure");

      if (reportDebug(__FUNCTION__))
        {
        printCR();
        printLine();
        printValue("Generator File Name",histoGeneratorFileName);
        printValue("Detector File Name",histoDetectorFileName);
        printValue("Closure File Name",histoClosureFileName);
        }
      Configuration closureConfig;
      closureConfig.addProperty(createKey(taskName,objectType,"METHOD"),method);
      closureConfig.addProperty(createKey(taskName,objectType,"IMPORT:PATH"),importPath);
      closureConfig.addProperty(createKey(taskName,objectType,"IMPORT:GEN_FILE_NAME"),histoGeneratorFileName);
      closureConfig.addProperty(createKey(taskName,objectType,"IMPORT:REC_FILE_NAME"),histoDetectorFileName);
      closureConfig.addProperty(createKey(taskName,objectType,"EXPORT:PATH"),exportPath);
      closureConfig.addProperty(createKey(taskName,objectType,"EXPORT:CLOSURE_FILE_NAME"),histoClosureFileName);
      ClosureCalculator calculator;
      calculator.setName("Closure");
      // BUG calculator.setConfiguration(&closureConfig);
      } // file loop
    } // task loop
  if (reportEnd(__FUNCTION__)) { /* no ops*/ };
}

} // namespace CAP

