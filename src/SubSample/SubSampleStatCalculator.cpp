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
#include "HistogramGroup.hpp"
#include "SubSampleStatCalculator.hpp"
#include "PrintHelpers.hpp"
#include "RootFileHelpers.hpp"
#include "RootHistogramHelpers.hpp"
#include "RootFileHelpers.hpp"
#include "NameHelpers.hpp"
#include "Exceptions.hpp"

ClassImp(CAP::SubSampleStatCalculator);

namespace CAP
{

  SubSampleStatCalculator::SubSampleStatCalculator()
  :
  Task(),
  _totalTaskExecuted(),
  _eventAccepted(),
  _totalEventAccepted(),
  _importPath(),
  _importFile(),
  _exportPath(),
  _exportFile()
  {
  appendClassName("SubSampleStatCalculator");
  setMinimumReportLevel(Object::kInfo);
  setName("SubSampleStatCalculator");
  }

  SubSampleStatCalculator::SubSampleStatCalculator(const SubSampleStatCalculator & src)
  :
  Task(src),
  _totalTaskExecuted(src._totalTaskExecuted),
  _eventAccepted(src._eventAccepted),
  _totalEventAccepted(src._totalEventAccepted),
  _importPath(src._importPath),
  _importFile(src._importFile),
  _exportPath(src._exportPath),
  _exportFile(src._exportFile)
  { }

  SubSampleStatCalculator & SubSampleStatCalculator::operator=(const SubSampleStatCalculator & rhs)
  {
  if (this!=&rhs)
    {
    Task::operator=(rhs),
    _totalTaskExecuted  = rhs._totalTaskExecuted;
    _eventAccepted      = rhs._eventAccepted;
    _totalEventAccepted = rhs._totalEventAccepted;
    _importPath = rhs._importPath;
    _importFile = rhs._importFile;
    _exportPath = rhs._exportPath;
    _exportFile = rhs._exportFile;
    }
  return *this;
  }

  void SubSampleStatCalculator::setDefaultConfiguration()
  {
  Task::setDefaultConfiguration();
  const String & taskName = name();
  String objectType = "HISTOGRAM";
  _importPath  = _configuration.valueString(createKey(taskName,objectType,"IMPORT:PATH"));
  _importFile  = _configuration.valueString(createKey(taskName,objectType,"IMPORT:FILE_NAME"));
  _exportPath  = _configuration.valueString(createKey(taskName,objectType,"EXPORT:PATH"));
  _exportFile  = _configuration.valueString(createKey(taskName,objectType,"EXPORT:FILE_NAME"));
  }

  void SubSampleStatCalculator::execute()
  {
  const String & taskName = name();
  String objectType = "HISTOGRAM";
  _importPath  = _configuration.valueString(createKey(taskName,objectType,"IMPORT:PATH"));
  _importFile  = _configuration.valueString(createKey(taskName,objectType,"IMPORT:FILE_NAME"));
  _exportPath  = _configuration.valueString(createKey(taskName,objectType,"EXPORT:PATH"));
  _exportFile  = _configuration.valueString(createKey(taskName,objectType,"EXPORT:FILE_NAME"));
  if (reportDebug(__FUNCTION__))
    {
    printCR();
    printValue(createKey(taskName,objectType,"IMPORT:PATH"),_importPath);
    printValue(createKey(taskName,objectType,"IMPORT:FILE_NAME"),_importFile);
    printValue(createKey(taskName,objectType,"EXPORT:PATH"),_exportPath);
    printValue(createKey(taskName,objectType,"EXPORT:FILE_NAME"),_exportFile);
    }
  bool verbose = false;
  std::vector<String> directories = listDirsIn(_importPath,verbose);
  int nDirectories = directories.size();
  if (directories.size()<2) throw Exception("directories.size()<2",__FUNCTION__);
  if (reportDebug(__FUNCTION__))
    {
    printCR();
    printValue("directories.size()",nDirectories);
    }
  TFile * firstFile;
  TFile * otherFile;
  HistogramGroup * groupAverage  = new HistogramGroup();
  HistogramGroup * group;
  groupAverage->setName("Average");
  int iFile = 0;
  _taskExecuted.initialize("Executed",1);
  _totalTaskExecuted.initialize("Executed",1);
  for (auto & name : directories)
    {
    String path = _importPath; path += "/"; path += name;

    if (reportDebug(__FUNCTION__))
      {
      printCR();
      printValue(createKey(taskName,objectType,"IMPORT:PATH"),_importPath);
      printValue(createKey(taskName,objectType,"IMPORT:FILE_NAME"),_importFile);
      printValue("File index",iFile);
      }
    if (iFile==0)
      {
      firstFile = openOldRootFile(path, _importFile);
      groupAverage->loadList(*firstFile);
      importAccountant(_taskExecuted,*firstFile);
      _totalTaskExecuted.increment(_taskExecuted);
      importAccountant(_eventAccepted,*firstFile);
      _totalEventAccepted.increment(_eventAccepted);
      }
    else
      {
      otherFile = openOldRootFile(path, _importFile);
      group = new HistogramGroup();
      group->loadList(*otherFile);
      importAccountant(_taskExecuted,*otherFile);
      _totalTaskExecuted.increment(_taskExecuted);
      importAccountant(_eventAccepted,*otherFile);
      _totalEventAccepted.increment(_eventAccepted);
      }
    if (iFile>0)
      { // needs to be fixed....
        // groupAverage->squareDifferenceLists(*group, double(nExecutedTotal), double(nExecuted), (iFile==(nDirectories-1)) ? nDirectories-1 : -iFile);
        otherFile->Close();
        delete group;
      }
    iFile++;
    }

  TFile & rootOutputFile = *openRecreateRootFile(_exportPath,_exportFile);
  exportAccountant(_totalTaskExecuted,rootOutputFile);
  exportAccountant(_totalEventAccepted,rootOutputFile);
  //groupAverage->exportObject(rootOutputFile);
  firstFile->Close();
  delete groupAverage;
  rootOutputFile.Close();
  }

} // namespace CAP

