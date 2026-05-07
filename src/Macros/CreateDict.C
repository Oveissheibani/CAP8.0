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

void loadLibraries(const TString & includeBasePath);


int CreateDict(bool verbose=false)
{
  TString includeBasePath = getenv("CAP_SRC_PATH");
  loadLibraries(includeBasePath);

  TString dictionaryPath = getenv("CAP_DATABASE_PATH");
  dictionaryPath += "Dictionaries/";

  TString configFile="Pythia/pp_13.7TeV/RunAna3Dex.ini";
  TString histogramPath="pythiaTest3Dex/";


  try {

    CAP::Configuration configuration;
    CAP::printLine();
    CAP::printString("Create Dictionaries");
    CAP::TaskCreator * taskCreator = new CAP::TaskCreator();

    vector<TString> taskNames;
    taskNames.push_back("RunAnalysis");
    taskNames.push_back("EventIterator");
    taskNames.push_back("FileIterator");
    taskNames.push_back("ParticleDecayerTask");
    taskNames.push_back("ParticleTypeListTask");
    taskNames.push_back("GlobalAnalyzer");
    taskNames.push_back("SpherocityAnalyzer");
    taskNames.push_back("ParticleSingleAnalyzer");
    taskNames.push_back("ParticleSingleCalculator");
    taskNames.push_back("ParticlePairAnalyzer");
    taskNames.push_back("ParticlePair3DAnalyzer");
    taskNames.push_back("JetAnalyzer");
    taskNames.push_back("NuDynAnalyzer");
    taskNames.push_back("PtPtAnalyzer");
    taskNames.push_back("ClosureCalculator");
    taskNames.push_back("ClosureIterator");
    taskNames.push_back("EventPlaneRandomizerTask");
    taskNames.push_back("EventVertexRandomizerTask");
    taskNames.push_back("MeasurementSimulator");
    taskNames.push_back("PerformanceAnalyzer");
    taskNames.push_back("SubSampleStatCalculator");
    taskNames.push_back("PythiaEventGenerator");
    taskNames.push_back("PythiaEventReader");
    taskNames.push_back("TherminatorGenerator");

    for (TString & taskName : taskNames)
      {
      TString name = "CAP::";
      name += taskName;
      CAP::printValue("Creating dictionary for task",name);
      CAP::Task * task = taskCreator->createTask(name);
      task->exportDictionary(dictionaryPath,taskName,verbose);
      delete task;
      }
    CAP::printLine();
    CAP::printString("Done");
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
  catch (CAP::UnknownTaskException & exception)
  {
  exception.print();
  }

  CAP::printLine();
  CAP::printString("CreateDict");
  CAP::printLine();
  return 0;
}

void loadBase(const TString & includeBasePath)
{
  TString includePath = includeBasePath + "/Base/";
  gSystem->Load(includePath+"Configuration.hpp");
  gSystem->Load(includePath+"Timer.hpp");
  gSystem->Load(includePath+"PrintHelpers.hpp");
  gSystem->Load(includePath+"RootHistogramHelpers.hpp");
  gSystem->Load(includePath+"StreamHelpers.hpp");
  gSystem->Load(includePath+"MessageLogger.hpp");
  gSystem->Load(includePath+"Task.hpp");
  gSystem->Load(includePath+"Exceptions.hpp");
  gSystem->Load(includePath+"HistogramGroup.hpp");
  gSystem->Load("libBase.dylib");
}

void loadMath(const TString & includeBasePath)
{
  TString includePath = includeBasePath + "/Math/";
  gSystem->Load(includePath+"MathExceptions.hpp");
  gSystem->Load(includePath+"MathConstants.hpp");
  gSystem->Load("libMath.dylib");
}

void loadParticleTypeList(const TString & includeBasePath)
{
  TString includePath = includeBasePath + "/ParticleTypeList/";
  gSystem->Load(includePath+"ParticleTypeList.hpp");
  gSystem->Load(includePath+"ParticleTypeExceptions.hpp");
  gSystem->Load(includePath+"ParticleTypeListParser.hpp");
  gSystem->Load(includePath+"ParticleTypeListParser_CS.hpp");
  gSystem->Load(includePath+"ParticleTypeListTask.hpp");
  gSystem->Load(includePath+"ParticleDecayMode.hpp");
  gSystem->Load(includePath+"ParticlePartition.hpp");
  gSystem->Load(includePath+"ParticleType.hpp");
  gSystem->Load("libParticleTypeList.dylib");
}

void loadParticles(const TString & includeBasePath)
{
  TString includePath = includeBasePath + "/Particles/";
  gSystem->Load(includePath+"Event.hpp");
  gSystem->Load(includePath+"Particle.hpp");
  gSystem->Load(includePath+"Task.hpp");
  gSystem->Load(includePath+"EventIterator.hpp");
  gSystem->Load("libParticles.dylib");
}



void loadPythia(const TString & includeBasePath)
{
  TString includePath = includeBasePath + "/CAPPythia/";
  gSystem->Load(includePath+"PythiaEventGenerator.hpp");
  gSystem->Load(includePath+"PythiaEventReader.hpp");
  gSystem->Load("libCAPPythia.dylib");
}

void loadPerformance(const TString & includeBasePath)
{
  TString includePath = includeBasePath + "/Performance/";
  gSystem->Load(includePath+"ClosureCalculator.hpp");
  gSystem->Load(includePath+"ClosureIterator.hpp");
  gSystem->Load(includePath+"MeasurementSimulator.hpp");
  gSystem->Load(includePath+"PerformanceAnalyzer.hpp");
  gSystem->Load(includePath+"ParticlePerformanceHistos.hpp");
  gSystem->Load("libPerformance.dylib");
}

//void loadAmpt(const TString & includeBasePath)
//{
//  TString includePath = includeBasePath + "/Ampt/";
//  gSystem->Load(includePath+"AmptEventReader.hpp");
//  gSystem->Load("libAmpt.dylib");
//}
//
//void loadEpos(const TString & includeBasePath)
//{
//  TString includePath = includeBasePath + "/Epos/";
//  gSystem->Load(includePath+"EposEventReader.hpp");
//  gSystem->Load("libEpos.dylib");
//}
//
//void loadHijing(const TString & includeBasePath)
//{
//  TString includePath = includeBasePath + "/Hijing/";
//  gSystem->Load(includePath+"HijingEventReader.hpp");
//  gSystem->Load("libHijing.dylib");
//}

//void loadHerwig(const TString & includeBasePath)
//{
//  TString includePath = includeBasePath + "/Herwig/";
//  gSystem->Load(includePath+"HerwigEventReader.hpp");
//  gSystem->Load("libHerwig.dylib");
//}
//
//void loadUrqmd(const TString & includeBasePath)
//{
//  TString includePath = includeBasePath + "/Urqmd/";
//  gSystem->Load(includePath+"UrqmdEventReader.hpp");
//  gSystem->Load("libUrqmd.dylib");
//}
//
//
//void loadBasicGen(const TString & includeBasePath)
//{
//  TString includePath = includeBasePath + "/BasicGen/";
//  gSystem->Load(includePath+"GaussianGeneratorTask.hpp");
//  gSystem->Load(includePath+"RadialBoostHistos.hpp");
//  gSystem->Load(includePath+"RadialBoostTask.hpp");
//  gSystem->Load(includePath+"RapidityGenerator.hpp");
//  gSystem->Load("libBasicGen.dylib");
//}

void loadGlobal(const TString & includeBasePath)
{
  TString includePath = includeBasePath + "/Global/";
  gSystem->Load(includePath+"GlobalAnalyzer.hpp");
  gSystem->Load(includePath+"GlobalCalculator.hpp");
  gSystem->Load(includePath+"GlobalHistos.hpp");
  gSystem->Load("libGlobal.dylib");
}

void loadSpherocity(const TString & includeBasePath)
{
  TString includePath = includeBasePath + "/Spherocity/";
  gSystem->Load(includePath+"SpherocityAnalyzer.hpp");
  gSystem->Load(includePath+"SpherocityCalculator.hpp");
  gSystem->Load(includePath+"SpherocityHistos.hpp");
  gSystem->Load("libSpherocity.dylib");
}


void loadSingle(const TString & includeBasePath)
{
  TString includePath = includeBasePath + "/ParticleSingle/";
  gSystem->Load(includePath+"ParticleSingleHistos.hpp");
  gSystem->Load(includePath+"ParticleSingleDerivedHistos.hpp");
  gSystem->Load(includePath+"ParticleSingleAnalyzer.hpp");
  gSystem->Load(includePath+"ParticleSingleCalculator.hpp");
  gSystem->Load("libParticleSingle.dylib");
}

void loadPair(const TString & includeBasePath)
{
  TString includePath = includeBasePath + "/ParticlePair/";
  gSystem->Load(includePath+"ParticlePairAnalyzer.hpp");
  gSystem->Load(includePath+"ParticlePairCalculator.hpp");
  gSystem->Load(includePath+"ParticlePairHistos.hpp");
  gSystem->Load(includePath+"ParticlePairDerivedHistos.hpp");
  gSystem->Load(includePath+"BalanceFunctionCalculator.hpp");
  gSystem->Load("libParticlePair.dylib");
}

void loadNuDyn(const TString & includeBasePath)
{
  TString includePath = includeBasePath + "/NuDyn/";
  gSystem->Load(includePath+"NuDynAnalyzer.hpp");
  gSystem->Load(includePath+"NuDynCalculator.hpp");
  gSystem->Load(includePath+"NuDynDerivedHistos.hpp");
  gSystem->Load(includePath+"NuDynHistos.hpp");
  gSystem->Load("libNuDyn.dylib");
}

void loadSubSample(const TString & includeBasePath)
{
  TString includePath = includeBasePath + "/SubSample/";
  gSystem->Load(includePath+"SubSampleStatCalculator.hpp");
  gSystem->Load("libSubSample.dylib");
}

void loadExec(const TString & includeBasePath)
{
  TString includePath = includeBasePath + "/Exec/";
  gSystem->Load(includePath+"RunAnalysis.hpp");
  gSystem->Load("libExec.dylib");
}

void loadTherminator(const TString & includeBasePath)
{
  TString includePath = includeBasePath + "/Therminator/";
  gSystem->Load(includePath+"Model.hpp");
  gSystem->Load("libTherminator.dylib");
}

void loadJets(const TString & includeBasePath)
{
  TString includePath = includeBasePath + "/Jets/";
  gSystem->Load(includePath+"JetAnalyzer.hpp");
  gSystem->Load(includePath+"JetCalculator.hpp");
  gSystem->Load("libJets.dylib");
}


void loadLibraries(const TString & includeBasePath)
{
  loadBase(includeBasePath);
  loadMath(includeBasePath);
  loadParticleTypeList(includeBasePath);
  loadParticles(includeBasePath);
  loadGlobal(includeBasePath);
  loadSpherocity(includeBasePath);
  loadSingle(includeBasePath);
  loadPair(includeBasePath);
  loadNuDyn(includeBasePath);
  loadSubSample(includeBasePath);
  loadPythia(includeBasePath);
  loadTherminator(includeBasePath);
  loadJets(includeBasePath);
//  loadAmpt(includeBasePath)
//  loadEpos(includeBasePath)
//  loadHijing(includeBasePath)
//  loadHerwig(includeBasePath)
//  loadUrqmd(includeBasePath)
  loadExec(includeBasePath);
  loadPerformance(includeBasePath);
}

