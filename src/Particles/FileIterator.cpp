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
#include "FileIterator.hpp"
#include "PrintHelpers.hpp"
#include "EnvironmentVariables.hpp"
#include "Exceptions.hpp"

ClassImp(CAP::FileIterator);

namespace CAP
{

  FileIterator::FileIterator()
  :
  EventProcessor(),
  _nLevels(0)
  {
  appendClassName("FileIterator");
  setMinimumReportLevel(Object::kInfo);
  setName("FileIterator");
  setTitle("FileIterator");
  }

  FileIterator::FileIterator(const FileIterator & source)
  :
  EventProcessor(source),
  _nLevels(source._nLevels)
  {    }

  FileIterator FileIterator::operator=(const FileIterator & rhs)
  {
  if (this!=&rhs)
    {
    EventProcessor::operator=(rhs);
    _nLevels = rhs._nLevels;
    }
  return *this;
  }

  void FileIterator::setDefaultConfiguration()
  {
  if (reportDebug(__FUNCTION__))  { printCR(); };
  EventProcessor::setDefaultConfiguration();
  const String & taskName = Task::name();
  _configuration.addProperty( createKey(taskName,"nLevels"),0);
  }

  void FileIterator::configure(const Configuration & configuration)
  {
  EventProcessor::configure(configuration);
  String taskName = name();
  _nLevels = _configuration.valueInt(createKey(taskName,"nLevels"));
  if (reportInfo(__FUNCTION__))
    {
    printCR();
    printValue("TaskName",taskName);
    printValue("nLevels",_nLevels);
    printLine();
    };
  //configureSubtasks(this,taskName,configuration);
  }

  void FileIterator::execute()
  {
  if (reportDebug(__FUNCTION__))  { printCR(); };
  _timer.start();
  _taskExecuted.increment();
  std::vector<String> importPathNames;
  std::vector<String> exportPathNames;
  EnvironmentVariables * env = EnvironmentVariables::environmentVariables();
  String histogramsImportPath = env->variable("CAP_HISTOS_IMPORT_PATH");
  String histogramsExportPath = env->variable("CAP_HISTOS_EXPORT_PATH");
  String taskName = name();

  if (reportDebug(__FUNCTION__))
    {
    printCR();
    printValue("TaskName",taskName);
    printValue("nLevels",_nLevels);
    printValue("HistogramsImportPath",histogramsImportPath);
    printValue("HistogramsExportPath",histogramsExportPath);
    }
  switch (_nLevels)
    {
      case 0:
      {
      String name = histogramsImportPath; name += "/";
      importPathNames.push_back(name);
      name = histogramsExportPath;  name += "/";
      exportPathNames.push_back(name);
      break;
      }

      case 1: // one folder with many subfolders
      {
      // this breaks things...
      //      importPathNames =//listDirsIn(histogramsImportPath,true);
      exportPathNames = importPathNames;
      break;
      }
    }
  if (importPathNames.size()<1 )
    throw Exception("Attempting to execute file iterator with no selected files",__FUNCTION__);
  unsigned int nPaths = importPathNames.size();
  if (reportDebug(__FUNCTION__))
    {
    printCR();
    printValue("nPaths", nPaths);
    }
  for (auto & subtask : _subTasks)
    {
    
    String subtaskName = subtask->name();
    for (unsigned int iPath=0; iPath<nPaths; iPath++)
      {
      if (reportDebug(__FUNCTION__))
        {
        printCR();
        printValue("SubtaskName",subtaskName);
        }
      Configuration & conf = subtask->configuration();
      conf.addProperty(createKey(taskName,"HistogramsImportPath"),importPathNames[iPath]);
      conf.addProperty(createKey(taskName,"HistogramsExportPath"),exportPathNames[iPath]);
      if (reportDebug(__FUNCTION__))
        {
        printCR();
        printValue(createKey(taskName,"HistogramsImportPath"), conf.valueString(createKey(taskName,"HistogramsImportPath")));
        printValue(createKey(taskName,"HistogramsExportPath"), conf.valueString(createKey(taskName,"HistogramsExportPath")));
        }
      //subtask->initialize();
      subtask->postProcess();
      //subtask->finalize();
      //subtask->reset();
      }
    }
  _timer.stop();
  printCR();
  printLine();
  _timer.print();
  printLine();
  }

} // namespace CAP
