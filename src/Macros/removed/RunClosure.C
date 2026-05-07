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
#include <TROOT.h>


void loadBase(const TString & includeSrcPath);
void loadPerformance(const TString & includeSrcPath);
void loadExec(const TString &  includeSrcPath);
void loadLibrairies(const TString & includeSrcPath);

std::vector<TString> listFolders(const TString & pathname,
                                      bool verbose=true);

int RunClosureForFiles(const TString & histogramsImportPathName,
                       const TString & histogramsImportGenFileName,
                       const TString & histogramsImportRecoFileName,
                       const TString & histogramsExportPathName,
                       const TString & histogramsExportFileName,
                       int   selectedMethod,
                       bool verbose=true);

int RunClosureForFolders(const TString & histogramsImportPathName,
                         const TString & histogramsImportGenFileName,
                         const TString & histogramsImportRecoFileName,
                         const TString & histogramsExportPathName,
                         const TString & histogramsExportFileName,
                         int   selectedMethod,
                         bool verbose=true);


int RunClosure(const TString histogramsImportPathName="/Volumes/ClaudeDisc4/OutputFiles/pythiaTest3D/",
               const TString histogramsImportGenFileName="SingleGen.root",
               const TString histogramsImportRecoFileName="SingleReco.root",
               const TString histogramsExportPathName="/Volumes/ClaudeDisc4/OutputFiles/pythiaTest3D/",
               const TString histogramsExportFileName="SingleRatio.root",
               int   nLevels=1,
               int   selectedMethod=1,
               bool  verbose=true)
{
  TString includeSrcPath = getenv("CAP_SRC_PATH");
  loadBase(includeSrcPath);
  loadPerformance(includeSrcPath);
  loadExec(includeSrcPath);
//  loadLibrairies(includeSrcPath);
//  try
//  {
  if (nLevels==1)
    {
    RunClosureForFiles(histogramsImportPathName,
                       histogramsImportGenFileName,
                       histogramsImportRecoFileName,
                       histogramsExportPathName,
                       histogramsExportFileName,
                       selectedMethod,
                       verbose);
    }
  else if (nLevels>1)
    {
    RunClosureForFolders(histogramsImportPathName,
                         histogramsImportGenFileName,
                         histogramsImportRecoFileName,
                         histogramsExportPathName,
                         histogramsExportFileName,
                         selectedMethod,
                         verbose);
    }
//  }
//  catch (CAP::TaskException & exception)
//  {
//  exception.print();
//  }
//  catch (CAP::FileException  & exception)
//  {
//  exception.print();
//  }
//  catch (CAP::IncompatiblehistogramsException   & exception)
//  {
//  exception.print();
//  }
  return 0;
}



void loadBase(const TString & includeSrcPath)
{
  TString includePath = includeSrcPath + "/Base/";
  gSystem->Load(includePath+"PrintHelpers.hpp");
  gSystem->Load(includePath+"RootHistogramHelpers.hpp");
  gSystem->Load(includePath+"Configuration.hpp");
  gSystem->Load(includePath+"Timer.hpp");
  gSystem->Load(includePath+"MessageLogger.hpp");
  gSystem->Load(includePath+"Exceptions.hpp");
  gSystem->Load(includePath+"Task.hpp");
  gSystem->Load("libBase.dylib");
}

void loadParticles(const TString & includeSrcPath)
{
  TString includePath = includeSrcPath + "/Particles/";
  gSystem->Load("libParticles.dylib");
}

void loadSingle(const TString & includeSrcPath)
{
  TString includePath = includeSrcPath + "/ParticleSingle/";
  gSystem->Load(includePath+"ParticleSingleHistos.hpp");
  gSystem->Load(includePath+"ParticleSingleDerivedHistos.hpp");
  gSystem->Load(includePath+"ParticleSingleAnalyzer.hpp");
  gSystem->Load("libParticleSingle.dylib");
}

void loadPair(const TString & includeSrcPath)
{
  TString includePath = includeSrcPath + "/ParticlePair/";
  gSystem->Load(includePath+"ParticlePairAnalyzer.hpp");
  gSystem->Load(includePath+"ParticlePairHistos.hpp");
  gSystem->Load(includePath+"ParticlePairDerivedHistos.hpp");
  gSystem->Load(includePath+"BalanceFunctionCalculator.hpp");
  gSystem->Load("libParticlePair.dylib");
}

void loadPythia(const TString & includeSrcPath)
{
  TString includePath = includeSrcPath + "/CAPPythia/";
  gSystem->Load(includePath+"PythiaEventGenerator.hpp");
  gSystem->Load(includePath+"PythiaEventReader.hpp");
  gSystem->Load("libCAPPythia.dylib");
}

void loadPerformance(const TString & includeSrcPath)
{
  TString includePath = includeSrcPath + "/Performance/";
  gSystem->Load(includePath+"ClosureCalculator.hpp");
  gSystem->Load(includePath+"ClosureIterator.hpp");
  gSystem->Load(includePath+"MeasurementSimulator.hpp");
  gSystem->Load(includePath+"PerformanceAnalyzer.hpp");
  gSystem->Load(includePath+"ParticlePerformanceHistos.hpp");
  gSystem->Load("libPerformance.dylib");
}

void loadAmpt(const TString & includeSrcPath)
{
  TString includePath = includeSrcPath + "/Ampt/";
  gSystem->Load(includePath+"AmptEventReader.hpp");
  gSystem->Load("libAmpt.dylib");
}

void loadEpos(const TString & includeSrcPath)
{
  TString includePath = includeSrcPath + "/Epos/";
  gSystem->Load(includePath+"EposEventReader.hpp");
  gSystem->Load("libEpos.dylib");
}

void loadHijing(const TString & includeSrcPath)
{
  TString includePath = includeSrcPath + "/Hijing/";
  gSystem->Load(includePath+"HijingEventReader.hpp");
  gSystem->Load("libHijing.dylib");
}

void loadHerwig(const TString & includeSrcPath)
{
  TString includePath = includeSrcPath + "/Herwig/";
  gSystem->Load(includePath+"HerwigEventReader.hpp");
  gSystem->Load("libHerwig.dylib");
}

void loadUrqmd(const TString & includeSrcPath)
{
  TString includePath = includeSrcPath + "/Urqmd/";
  gSystem->Load(includePath+"UrqmdEventReader.hpp");
  gSystem->Load("libUrqmd.dylib");
}

void loadGlobal(const TString & includeSrcPath)
{
  TString includePath = includeSrcPath + "/Global/";
  gSystem->Load(includePath+"GlobalAnalyzer.hpp");
  gSystem->Load(includePath+"GlobalHistos.hpp");
  gSystem->Load("libGlobal.dylib");
}

void loadSpherocity(const TString & includeSrcPath)
{
  TString includePath = includeSrcPath + "/Spherocity/";
  gSystem->Load(includePath+"SpherocityAnalyzer.hpp");
  gSystem->Load(includePath+"SpherocityHistos.hpp");
  gSystem->Load("libSpherocity.dylib");
}



void loadNuDyn(const TString & includeSrcPath)
{
  TString includePath = includeSrcPath + "/NuDyn/";
  gSystem->Load(includePath+"NuDynAnalyzer.hpp");
  gSystem->Load(includePath+"NuDynDerivedHistos.hpp");
  gSystem->Load(includePath+"NuDynHistos.hpp");
  gSystem->Load("libNuDyn.dylib");
}

void loadSubSample(const TString & includeSrcPath)
{
  TString includePath = includeSrcPath + "/SubSample/";
  gSystem->Load(includePath+"SubSampleStatCalculator.hpp");
  gSystem->Load("libSubSample.dylib");
}

void loadExec(const TString & includeSrcPath)
{
  TString includePath = includeSrcPath + "/Exec/";
  // gSystem->Load(includePath+"RunSubSampleSum.hpp");
  gSystem->Load("libExec.dylib");
}

void loadLibrairies(const TString & includeSrcPath)
{
  loadBase(includeSrcPath);
  loadPerformance(includeSrcPath);
  loadExec(includeSrcPath);
}


std::vector<TString> listFolders(const TString & pathname,
                                      bool verbose)
{
  TString directoryName = pathname;
  std::vector<TString>  subdirectories;
  if (!directoryName.EndsWith("/")) directoryName += "/";
  if (verbose) cout << "==================== Searching folders ==================== " << endl;
  TSystemDirectory topDirectory(directoryName, directoryName);
  TList *files = topDirectory.GetListOfFiles();
  if (!files)
    {
    cout << " files is a null pointer" << endl;
    return subdirectories;
    }
  TSystemFile *file;
  TString fname;
  TIter next(files);
  while ((file=(TSystemFile*)next()) )
    {
    fname = file->GetName();
    if (file->IsDirectory()   && !fname.BeginsWith(".")  ) subdirectories.push_back(fname);
    }
  return subdirectories;
}

int RunClosureForFiles(const TString & histogramsImportPathName,
                       const TString & histogramsImportGenFileName,
                       const TString & histogramsImportRecoFileName,
                       const TString & histogramsExportPathName,
                       const TString & histogramsExportClosureFileName,
                       int   selectedMethod,
                       bool  verbose)
{
  std::cout << "==================================================================================" << std::endl;
  std::cout << "Executing RunClosureForFilesIn" << endl;
  std::cout << "==================================================================================" << std::endl;
//  CAP::printLine();
//  CAP::printString("Executing RunClosure.run()");
//  CAP::printValue("histogramsImportPathName",histogramsImportPathName);
//  CAP::printValue("histogramsImportGenFileName",histogramsImportGenFileName);
//  CAP::printValue("histogramsImportRecoFileName",histogramsImportRecoFileName);
//  CAP::printValue("histogramsExportPathName",histogramsExportPathName);
  CAP::Configuration configuration;
  try
  {
  if (verbose) configuration.addProperty("ClosureCalculator:Severity","DEBUG");
  else configuration.addProperty("ClosureCalculator:Severity","INFO");
  configuration.addProperty("ClosureCalculator:HistogramsImportPath",     histogramsImportPathName);
  configuration.addProperty("ClosureCalculator:HistogramsImportGenFile",  histogramsImportGenFileName);
  configuration.addProperty("ClosureCalculator:HistogramsImportRecoFile", histogramsImportRecoFileName);
  configuration.addProperty("ClosureCalculator:HistogramsExportPath",     histogramsExportPathName);
  configuration.addProperty("ClosureCalculator:HistogramsExportFile",     histogramsExportClosureFileName);
  configuration.addProperty("ClosureCalculator:SelectedMethod",           selectedMethod);
  CAP::Task * task = new CAP::ClosureCalculator();
  task->setName("ClosureCalculator");
  task->setRequestedConfiguration(configuration);
  task->configure();
  task->execute();
  delete task;
  }
  catch (CAP::PropertyException & pe)
  {
  pe.print();
  }
  catch (CAP::TaskException & te)
  {
  te.print();
  }
  return 0;
}


int RunClosureForFolders(const TString & histogramsImportPathName,
                         const TString & histogramsImportGenFileName,
                         const TString & histogramsImportRecoFileName,
                         const TString & histogramsExportPathName,
                         const TString & histogramsExportFileName,
                         int   selectedMethod,
                         bool  verbose=true)
{
  std::cout << "==================================================================================" << std::endl;
  std::cout << "Executing RunClosureForFolders" << endl;
  std::cout << "==================================================================================" << std::endl;
  std::vector<TString> directories = listFolders(histogramsImportPathName,true);
  int nJobs = directories.size();
  std::cout << "Number of folders found:" << nJobs << endl;
  if (nJobs<1)
    {
    std::cout << "No folders founds" << endl;
    return 1;
    }
  for (auto directory : directories)
    {
    TString path = histogramsImportPathName; path += "/"; path += directory;
    RunClosureForFiles(path,
                       histogramsImportGenFileName,
                       histogramsImportRecoFileName,
                       path,
                       histogramsExportFileName,
                       selectedMethod,
                       verbose);
    }
  return 0;
}

