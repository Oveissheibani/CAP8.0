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
#include "Configuration.hpp"
#include "PrintHelpers.hpp"
#include "Task.hpp"
#include "Exceptions.hpp"
#include "MathExceptions.hpp"
//#include "Histogram.hpp"
#include "Timer.hpp"
#include "TH1D.h"

void usage()
{
  std::cout << "CAP" << std::endl;
  std::cout << "Version 1.0" << std::endl << std::endl << std::endl;
  std::cout << "Usage:" << std::endl;
  std::cout << "CAP  configurationFile" << std::endl;
  std::cout << "CAP  configurationPath configurationFile" << std::endl;
  std::cout << "CAP  configurationPath configurationFile histogramPath" << std::endl<< std::endl << std::endl;
}

void parse(int argc,
           char* argv[],
           TString &  taskName,
           TString &  configurationPath,
           TString &  configurationFile,
           TString &  histogramPath)
{
  TString configurationPathDefault  = "Test/";
  TString configurationFileDefault  = "test.ini";
  TString histogramPathDefault      = "test/";

  TString hPath;
  TString cPath;
  switch (argc)
    {
      default:
      case 1: usage(); exit(0);

      case 2:
      taskName          = argv[1];
      cPath             = configurationPathDefault;
      configurationFile = configurationFileDefault;
      hPath             = histogramPathDefault;
      break;

      case 3:
      taskName          = argv[1];
      cPath             = "";
      configurationFile = argv[2];
      hPath             = histogramPathDefault;
      break;

      case 4:
      taskName          = argv[1];
      cPath             = argv[2];
      configurationFile = argv[3];
      hPath             = histogramPathDefault;
      break;

      case 5:
      taskName          = argv[1];
      cPath             = argv[2];
      configurationFile = argv[3];
      hPath             = argv[4];
      break;
    }

  if (cPath.BeginsWith("/"))
    {
    configurationPath = cPath;
    }
  else
    {
    TString base = getenv("CAP_PROJECTS_PATH");
    configurationPath = base;
    configurationPath += "/";
    configurationPath += cPath;
    configurationPath += "/";
    }

  if (hPath.BeginsWith("/"))
    {
    histogramPath = hPath;
    }
  else
    {
    histogramPath = getenv("CAP_HISTOS_IMPORT_PATH");
    histogramPath += "/";
    histogramPath += hPath;
    histogramPath += "/";
    }
  setenv("CAP_HISTOS_IMPORT_PATH",histogramPath,1);
  setenv("CAP_HISTOS_EXPORT_PATH",histogramPath,1);
  CAP::printValue("Configuration path",configurationPath);
  CAP::printValue("Configuration file",configurationFile);
  CAP::printValue("CAP_HISTOS_IMPORT_PATH:",histogramPath);
  CAP::printValue("CAP_HISTOS_EXPORT_PATH:",histogramPath);

}

int main(int argc, char* argv[])
{
  TString mainTaskName;
  TString configurationPath;
  TString configurationFile;
  TString histogramPath;
  parse(argc,argv,mainTaskName,configurationPath,configurationFile,histogramPath);
  long seed=112133;
  if (seed!=0)  gRandom->SetSeed(seed);
 try
  {
  CAP::Configuration configuration;
  configuration.loadFrom(configurationPath,configurationFile);
  CAP::Task * creatorTask = new CAP::Task();
  cout << "check 1" << endl;
  CAP::Task * mainTask = creatorTask->createTask(nullptr,mainTaskName,configuration);
  cout << "check 2" << endl;
  mainTask->initialize();
  mainTask->execute();
  mainTask->finalize();
  delete mainTask;
  delete creatorTask;
  CAP::printLine();
  CAP::printLine();

//  CAP::Timer timer;
//
//  timer.start();
//  CAP::Histogram * h = new CAP::Histogram(TString("test"),TString("test"),100,0.0, 100.0);
//  long n = 400000000;
//  CAP::printLine();
//  CAP::printLine();
//  CAP::printLine();
//  CAP::printLine();
//  cout << n << endl;
//  for (long k=0; k<n; k++)
//    {
//   // if (k%1000) cout << k << endl;
//    h->fill(50.0,1.0);
//    }
//  timer.stop();
//  timer.print();
//  CAP::printString("Part I  completed successfully");
//  timer.start();
//  TH1 * hh = new TH1D(TString("test"),TString("test"),100,0.0, 100.0);
//  for (long k=0; k<n; k++)
//    {
//    //if (k%1000) cout << k << endl;
//    hh->Fill(50.0,1.0);
//    }
//  timer.stop();
//  timer.print();



  CAP::printString("CAP completed successfully");
  CAP::printLine();
  CAP::printLine();
  

  return 0;
  }

  catch (CAP::ConfigurationException & exception)
  {
  exception.print();
  }
  catch (CAP::MATH::MathException & exception)
  {
  exception.print();
  }
  catch (CAP::FileException & exception)
  {
  exception.print();
  }
  catch (CAP::MemoryException & exception)
  {
  exception.print();
  }
  catch (CAP::HistogramException & exception)
  {
  exception.print();
  }
  catch (CAP::FactoryException & exception)
  {
  exception.print();
  }
  catch (CAP::Exception & exception)
  {
  exception.print();
  }
  CAP::printLine();
  CAP::printString("CAP FAILED");
  CAP::printLine();
  return 1;
}

