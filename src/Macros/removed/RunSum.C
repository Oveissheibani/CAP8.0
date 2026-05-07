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
void loadParticles(const TString & includeSrcPath);
void loadPythia(const TString & includeSrcPath);
void loadPerformance(const TString & includeSrcPath);
void loadAmpt(const TString & includeSrcPath);
void loadEpos(const TString & includeSrcPath);
void loadHijing(const TString & includeSrcPath);
void loadHerwig(const TString & includeSrcPath);
void loadUrqmd(const TString & includeSrcPath);
void loadGlobal(const TString & includeSrcPath);
void loadSpherocity(const TString & includeSrcPath);
void loadSingle(const TString & includeSrcPath);
void loadPair(const TString & includeSrcPath);
void loadNuDyn(const TString & includeSrcPath);
void loadSubSample(const TString & includeSrcPath);
void loadExec(const TString & includeSrcPath);

std::vector<TString> listDirsInFolder(const TString & pathname,
                                      bool verbose=true)
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


int RunSumForFilesIn(TString histogramImportPathName,
                  TString histogramImportFileName,
                  TString histogramExportPathName,
                  TString histogramExportFileName,
                  bool verbose=true)
{
  std::cout << "==================================================================================" << std::endl;
  std::cout << "Executing RunSumForFile" << endl;
  std::cout << "==================================================================================" << std::endl;
  std::vector<TString> directories = listDirsInFolder(histogramImportPathName,true);
  int nJobs = directories.size();
  std::cout << "Number of folders found:" << nJobs << endl;
  if (nJobs<1)
    {
    std::cout << "No folders founds" << endl;
    return 1;
    }
  for (auto directory : directories)
    {
    TString path = histogramImportPathName; path += "/"; path += directory;
    CAP::RunSubSampleSum * task = new CAP::RunSubSampleSum();
    task->run(histogramImportPathName,histogramImportFileName,histogramExportPathName,histogramExportFileName,verbose);
    delete task;
    }

  return 0;
}




int RunSumForFolders(TString histogramImportPathName,
                  TString histogramImportFileName,
                  TString histogramExportPathName,
                  TString histogramExportFileName,
                  bool verbose=true)
{
  std::cout << "==================================================================================" << std::endl;
  std::cout << "Executing RunSumForFolders" << endl;
  std::cout << "==================================================================================" << std::endl;
  std::vector<TString> directories = listDirsInFolder(histogramImportPathName,true);
  int nJobs = directories.size();
  std::cout << "Number of folders found:" << nJobs << endl;
  if (nJobs<1)
    {
    std::cout << "No folders founds" << endl;
    return 1;
    }
  for (auto directory : directories)
    {
    TString path = histogramImportPathName; path += "/"; path += directory;
    RunSumForFilesIn(path,histogramImportFileName,path,histogramExportFileName,verbose);
    }
  return 0;
}

//int RunSumForFile(TString histogramImportPathName="/Volumes/ClaudeDisc4/OutputFiles/pythiaTest/",
//                  TString histogramImportFileName="Single.root",
//                  TString histogramExportPathName="/Volumes/ClaudeDisc4/OutputFiles/pythiaTest/",
//                  TString histogramExportFileName="SingleSum.root",
//                  bool verbose=true)
//{
//    std::vector<TString> directories = listDirsInFolder(histogramImportPathName,true);
//    int nJobs = directories.size();
//    CAP::printValue("Number of folders found",nJobs);
//    if (nJobs<1)
//    {
//        CAP::printString("No folders founds");
//        return 1;
//    }
//    for (auto directory : directories)
//    {
//        TString path = histogramImportPathName; path += "/"; path += directory;
//        CAP::RunSubSampleSum * task = new CAP::RunSubSampleSum();
//        task->run(histogramImportPathName,histogramImportFileName,histogramExportPathName,histogramExportFileName,verbose);
//        delete task;
//    }
//}

int RunSum(TString histogramImportPathName="/Volumes/ClaudeDisc4/OutputFiles/pythiaTest/",
           TString histogramExportPathName="/Volumes/ClaudeDisc4/OutputFiles/pythiaTest/",
           bool verbose=true)
{
  TString includeSrcPath = getenv("CAP_SRC_PATH");
  cout << "includeSrcPath: " << includeSrcPath << endl;
  loadBase(includeSrcPath);
  loadParticles(includeSrcPath);
  loadPythia(includeSrcPath);
  //loadPerformance(includeSrcPath);
  //loadAmpt(includeSrcPath);
  //loadEpos(includeSrcPath);
  //loadHijing(includeSrcPath);
  //loadHerwig(includeSrcPath);
  //loadUrqmd(includeSrcPath);
  loadGlobal(includeSrcPath);
  loadSpherocity(includeSrcPath);
  loadSingle(includeSrcPath);
  loadPair(includeSrcPath);
  loadNuDyn(includeSrcPath);
  loadSubSample(includeSrcPath);
  loadExec(includeSrcPath);
  try
  {
  RunSumForFolders(histogramImportPathName,"GlobalGen.root",histogramExportPathName,"GlobalGenSum.root",verbose);
  RunSumForFolders(histogramImportPathName,"SingleGen.root",histogramExportPathName,"SingleGenSum.root",verbose);
  RunSumForFolders(histogramImportPathName,"PairGen.root",histogramExportPathName,"PairGenSum.root",verbose);
  RunSumForFolders(histogramImportPathName,"NuDynGen.root",histogramExportPathName,"NuDynGenSum.root",verbose);
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
  gSystem->Load(includePath+"MeasurementPerformanceSimulator.hpp");
  gSystem->Load(includePath+"ParticlePerformanceSimulator.hpp");
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

