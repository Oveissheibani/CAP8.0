/* **********************************************************************
 * Copyright (C) 2019-2024, Claude Pruneau
 * All rights reserved.
 *
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 *
 * Author: Claude Pruneau,   04/01/2024
 *
 * This version uses the format of Chun Shen
 *
 * *********************************************************************/
#include "ParticleDbParser_CS.hpp"
using CAP::ParticleDbParser_CS;

ClassImp(CAP::ParticleDbParser_CS);


namespace CAP
{

ParticleDbParser_CS::ParticleDbParser_CS()
:
PropertiesParser()
{ }

void  ParticleDbParser_CS::readDb(ParticleDb & ParticleDb,
                                  ifstream & inputFile)
{
  //  ifstream * inputDbFile = openInputAsciiFile(_inputPathName,_inputFileName,_extension);
  //  if (!inputDbFile)
  //    throw CAP::NotFoundFileParticleTypeException(_inputPathName,_inputFileName,__FUNCTION__);
  //  ifstream & inputFile = *inputDbFile;

  string name, title;
  double mass, width, decayBranchingRatio;
  int pdgCode, gSpin, gIsospin, baryonNumber, strangeNumber, charm, bottom, charge;
  int nDecayModes, decayNpart, dummy_int;
  int decayPart[5] = {0, 0, 0, 0, 0};
  ParticleType * particleType     = nullptr;
  ParticleType * antiParticleType = nullptr;
  String theName;
  String theAntiName;
  String theTitle;
  String theAntiTitle;
  while (1)
    {
    inputFile >> pdgCode;
    if (inputFile.eof())
      {
      //      if (reportDebug(__FUNCTION__))
      //        cout << "Reached EOF." << endl;
      break;
      }
    inputFile >> name;
    inputFile >> mass;
    inputFile >> width;
    inputFile >> gSpin;
    inputFile >> baryonNumber;
    inputFile >> strangeNumber;
    inputFile >> charm;
    inputFile >> bottom;
    inputFile >> gIsospin;
    inputFile >> charge;
    inputFile >> nDecayModes;
    inputFile >> title;
    theName  = name;
    theTitle = title;
    particleType = new ParticleType();
    particleType->setPdgCode(pdgCode);
    particleType->setName(theName);
    particleType->setTitle(theTitle);
    particleType->setMass(mass);
    particleType->setWidth(width);
    particleType->setSpinFactor(gSpin);
    particleType->setBaryonNumber(baryonNumber);
    particleType->setStrangeness(strangeNumber);
    particleType->setCharm(charm);
    particleType->setBottomNumber(bottom);
    particleType->setIsoSpinFactor(gIsospin);
    particleType->setCharge(charge);
    ParticleDb.append(particleType);

    if (particleType->isFermion())
      {
      theAntiName  = "Anti-";
      theAntiName  += theName;
      theAntiTitle = "#bar ";
      theAntiTitle += theTitle;
      particleType = new ParticleType();
      particleType->setPdgCode(-pdgCode);
      particleType->setName(theName);
      particleType->setTitle(theTitle);
      particleType->setMass(mass);
      particleType->setWidth(width);
      particleType->setSpinFactor(gSpin);
      particleType->setBaryonNumber(-baryonNumber);
      particleType->setStrangeness(-strangeNumber);
      particleType->setCharm(-charm);
      particleType->setBottomNumber(-bottom);
      particleType->setIsoSpinFactor(gIsospin);
      particleType->setCharge(-charge);
      ParticleDb.append(particleType);
      ParticleDb.append(antiParticleType);
      }
    // read decay information
    for (int j = 0; j < nDecayModes; j++)
      {
      inputFile >> dummy_int;
      inputFile >> decayNpart;
      inputFile >> decayBranchingRatio;
      inputFile >> decayPart[0];
      inputFile >> decayPart[1];
      inputFile >> decayPart[2];
      inputFile >> decayPart[3];
      inputFile >> decayPart[4];
      decayNpart = abs(decayNpart);
      std::vector<int> children;
      std::vector<int> antiChildren;
      for (int k=0; k<decayNpart; k++)
        {
        children.push_back(decayPart[k]);
        }
      if (!particleType->isFermion())
        {
        particleType->addDecayMode(decayBranchingRatio,children);
        }
      else
        {
        for (int k=0; k<decayNpart; k++)
          {
          antiChildren.push_back(ParticleDb.findPdgCode(decayPart[k])->getAntiParticlePdgCode());
          }
        particleType->addDecayMode(decayBranchingRatio,children);
        antiParticleType->addDecayMode(decayBranchingRatio,antiChildren);
        }
      }
    }
  //cout << "Reading in ParticleType resonance decay table.. - 5- " << endl;
  ParticleDb.resolveTypes();
  ParticleDb.sortByMass();
}




}
