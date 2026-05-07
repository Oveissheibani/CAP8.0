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
#include <iostream>
#include <fstream>
#include <TStyle.h>
#include <TROOT.h>
#include <TRandom.h>

void loadLibraries(const TString & includeBasePath);

//!
//! Run generic data analysis based on the configuration listed in 'configFile'
//!
//! configFile     :  configuration file (.ini) describing the task(s) to be done
//! outputPath   :  output path used for all files created
//! seed            :  provided by slurm (grid job engine) or directly by user
//! isGrid          :   must be true for jobs running on grid
//! nEventsPerSubbunch : number of events to run per bunch  (actual on grid or simulated on a single node)
//! nSubbunchesPerBunch : number of sub-bunches  (must be 1 on grid)
//! nBunches :  number of bunches  (must be 1 on grid)
//!

int RunAnaPerformance(TString configFile="Pythia/pp_13.7TeV/RunAnaPerformance.ini",
           TString histogramPath="pythiaTest3D/",
           long seed=1121331)
{
  TString includeBasePath = getenv("CAP_SRC_PATH");
  loadLibraries(includeBasePath);

  if (seed!=0)  gRandom->SetSeed(seed);
  try
  {
  CAP::Configuration configuration;
  TString configurationPath = getenv("CAP_PROJECTS_PATH");
  TString configurationFile = configFile;
  CAP::printCR();
  CAP::printLine();
  CAP::printString("RunAna()");
  CAP::printLine();
  CAP::printValue("Configuration path",configurationPath);
  CAP::printValue("Configuration file",configurationFile);
  configuration.importProperties(configurationPath,configurationFile);

  String histoBasePath;
  if (histogramPath.BeginsWith("/"))
    {
    histoBasePath = histogramPath;
    }
  else
    {
    histoBasePath = getenv("CAP_HISTOS_IMPORT_PATH");
    histoBasePath += "/";
    histoBasePath += histogramPath;
    histoBasePath += "/";
    }
  cout << "Setting environment variable CAP_HISTOS_IMPORT_PATH to :" << histoBasePath << endl;
  cout << "Setting environment variable CAP_HISTOS_EXPORT_PATH to :" << histoBasePath << endl;
  setenv("CAP_HISTOS_IMPORT_PATH",histoBasePath,1);
  setenv("CAP_HISTOS_EXPORT_PATH",histoBasePath,1);

  CAP::printLine();
  CAP::printString("Instantiate TaskCreator");
  CAP::TaskCreator * creatorTask = new CAP::TaskCreator();
  creatorTask->setTargetTaskName("RunAnalysis");
  creatorTask->setRequestedConfiguration(configuration);
  creatorTask->configure();
  creatorTask->execute();
  CAP::Task * ana = creatorTask->getCreatedTask();
  ana->initialize();
  ana->execute();
  ana->finalize();
  delete ana;
  delete creatorTask;
  CAP::printLine();
  CAP::printString("RunAna completed successfully");
  CAP::printLine();
  return 0;
  }
  catch (CAP::TaskException & exception)
  {
  exception.print();
  }
  catch (CAP::FileException  & exception)
  {
  exception.print();
  }
  catch (CAP::IncompatibleHistogramException   & exception)
  {
  exception.print();
  }

  CAP::printLine();
  CAP::printString("RunAna FAILED");
  CAP::printLine();
  return 0;
}

void loadBase(const TString & includeBasePath)
{
  TString includePath = includeBasePath + "/Base/";
  gSystem->Load("libBase.dylib");
}


void loadExec(const TString & includeBasePath)
{
  TString includePath = includeBasePath + "/Performance/";
  gSystem->Load("libExec.dylib");
}




void loadLibraries(const TString & includeBasePath)
{
  loadExec(includeBasePath);
}

