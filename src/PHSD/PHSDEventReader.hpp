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
#ifndef CAP__PHSDEventReader
#define CAP__PHSDEventReader
#include "EventProcessor.hpp"

namespace CAP
{

class PHSDEventReader
:
public EventProcessor
{
public:
  PHSDEventReader();
  PHSDEventReader(const PHSDEventReader & task);
  virtual ~PHSDEventReader() {}
  PHSDEventReader & operator=(const PHSDEventReader & task);
  virtual void setDefaultConfiguration();
  virtual void initialize();
  virtual void execute();

protected:

  int    nParticles;
  int    runIndex;
  int    eventIndex;
  double impact;
  int    impactSamplingMethod;
  int    pdgCodeTarget;
  int    pdgCodeProjectile;
  double sqrtS;
  int    nPart;
  double psi2, epsilon2, psi3, epsilon3, psi4, epsilon4, psi5, epsilon5;
  
  double x, y, z, t;
  double cosPhi;
  double sinPhi;
  double eventPhi;

  int    pdgCode;
  int    charge;
  double px;
  double py;
  double pz;
  double e;
  int    phsdCode;
  int    parentCode;

  bool  fileOpened;
  std::ifstream  *inputFile;
  unsigned int    currentFileIndex;
  String inputPath;
  std::vector<String> inputFileNames;
  String inputFileName;


  ClassDef(PHSDEventReader,0)
};

}


#endif /* CAP__PHSDEventReader */
