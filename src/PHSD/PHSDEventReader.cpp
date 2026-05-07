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
#include "PHSDEventReader.hpp"
#include "StreamHelpers.hpp"
#include "Exceptions.hpp"
#include "NameHelpers.hpp"
#include "RootHistogramHelpers.hpp"

ClassImp(CAP::PHSDEventReader);

namespace CAP
{

PHSDEventReader::PHSDEventReader()
:
EventProcessor()
{
  appendClassName("PHSDEventReader");
  setMinimumReportLevel(Object::kInfo);
  setName("PHSDEventReader");
  setTitle("PHSDEventReader");
}

PHSDEventReader::PHSDEventReader(const PHSDEventReader & source)
:
EventProcessor(source)
{ }

void PHSDEventReader::execute()
{
  if (!fileOpened)
    {
    if (currentFileIndex >= inputFileNames.size())
      throw EndOfDataException("No more file","EOD",__FUNCTION__);
    inputFileName = inputFileNames[currentFileIndex];
    inputFile = & openInputAsciiFile("",inputFileName,"");
    if (reportDebug(__FUNCTION__))
      {
      printString("Parsing File");
      printValue("inputPath",inputPath);
      printValue("inputFileName",inputFileName);
      }
    inputFile->seekg(0, std::ios::beg);
    fileOpened = true;
    }

  ParticleFilter & particleFilter = * particleFilters[0];
  
  event.reset();
  Particle::factory().reset();

  ParticleType * particleType;
  // read event header
  *inputFile >> nParticles >> runIndex >> eventIndex >> impact >> impactSamplingMethod >> pdgCodeTarget >> pdgCodeProjectile >> sqrtS;
  *inputFile >> nPart >> psi2 >> epsilon2 >> psi3 >> epsilon3 >> psi4 >> epsilon4 >> psi5 >> epsilon5;
  // read/load particles
  for (int iParticle=0; iParticle<nParticles; iParticle++)
    {
    *inputFile >> pdgCode >> charge >> px >> py >> pz >> e >> phsdCode >> parentCode;
//    printString("-8-");
//    printValue("pdgCode",pdgCode);
//    printValue("charge",charge);
//    printValue("phsdCode",phsdCode);
//    printValue("parentCode",parentCode);
//    printValue("px",px);
//    printValue("py",py);
//    printValue("pz",pz);
//    printValue("e",e);

    particleType = particleTypeList->findPdgCode(pdgCode);
    if (particleType==nullptr)
      {
      if (reportWarning(__FUNCTION__)) cout << "Encountered unknown pdgCode: " << pdgCode << " Particle not added to event." << endl;
      continue;
      }
    Particle & particle = *Particle::factory().nextObject();
    particle.setType(particleType);
    particle.setLive(1);
    particle.setPxPyPzE(px,py,pz,e);
    particle.setXYZT(x,y,z,t);
    if (!particleFilter.accept(particle)) continue;
    event.addParticle(&particle);
    }
  eventAccepted.increment();
  _taskExecuted.increment();;


  if (inputFile->eof())
    {
    inputFile->close();
    fileOpened = false;
    currentFileIndex++;
    }


}

void PHSDEventReader::initialize()
{
  EventProcessor::initialize();
  currentFileIndex = 0;
  inputPath = "/Volumes/ClaudeDisc5/PHSD/";
//  inputFileNames.push_back(String("out0.dat"));
//  inputFileNames.push_back(String("out0.dat"));
  inputFileName = "";

  inputFileNames =  listFilesInDir(inputPath,".dat","",1,1,0);
//                 const String & ext,
//                 bool prependPath,
//                 bool verbose,
//                 int  maximumDepth,
//                 int  currentDepth)
  unsigned int  n = inputFileNames.size();
  printValue("inputFileNames.size()",n);
  for (unsigned int k = 0; k<n; k++)
    {
    String s = "file "; s += k;
    printValue(s,inputFileNames[k]);
    }

  x = 0.0;
  y = 0.0;
  z = 0.0;
  t = 0.0;
}

} // namespace CAP

