/* **********************************************************************
 * Copyright (C) 2019-2024, Claude Pruneau
 * All rights reserved.
 *
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 *
 * Author: Claude Pruneau, 05/16/2024
 *
 * *********************************************************************/
#include "ParticleDbParser.hpp"
#include "PrintHelpers.hpp"
#include <iostream>
#include "MathBasicFunctions.hpp"

using CAP::MATH::absolute;
using CAP::MATH::power;

ClassImp(CAP::ParticleDbParser);


namespace CAP
{


const double factorials[7] = {1.0, 1.0, 2.0, 6.0, 24.0, 120.0, 720.0 };



ParticleDbParser::ParticleDbParser()
:
PropertiesParser()
{ }

//!
//!Therminator Particle Db Parser
//!
//! @param ParticleDb Database to be loaded by this parse
//! @param inputFile Input stream for particles
//! @param inputFileDecays Input stream for decays
//!
//!
void  ParticleDbParser::readDb(ParticleDb  & particleDb,
                               ifstream & inputFile,
                               ifstream & inputFileDecays,
                               bool verbose)
{
  char   buff[300];
  char   name[30];
  char   title[30];
  int    pdgCode, quarks, antiquarks, leptons, index;
  double charge, mass, massUncertainty, width, widthUncertainty, spin, isospin, isospin3;

  index = 0;
  ParticleType * particleType;

  //
  // read particles
  //
  while (!inputFile.eof())
    {
    inputFile.getline(buff,400);
    if (!(*buff) || (*buff == '#')) continue;
    istringstream * iss = new istringstream(buff);
    (*iss) >> name >> title >> pdgCode >> charge >> mass >> massUncertainty >> width >> widthUncertainty >> spin >> isospin >> isospin3 >> quarks >> antiquarks >> leptons;
    particleType = new ParticleType();
    particleType->setIndex(index);
    particleType->setName(name);
    particleType->setTitle(title);
    particleType->setCharge(charge);
    particleType->setMass(mass);
    particleType->setWidth(width);
    particleType->setSpin(spin);
    particleType->setIsospin(isospin);
    particleType->setIsospin3(isospin3);
    particleType->setQuarkNumbers(quarks);
    particleType->setAntiQuarkNumbers(antiquarks);
    particleType->setLeptonNumbers(leptons);
    particleType->setPdgCode(pdgCode);
    particleType->disableDecay();
    particleType->enable();
    particleDb.append(particleType);

//    printValue("name",name);
//    printValue("title",title);
//    printValue("pdgCode",pdgCode);
//    printValue("charge",charge);
//    printValue("mass",mass);
//    printValue("massUncertainty",massUncertainty);
//    printValue("width",width);
//    printValue("widthUncertainty",widthUncertainty);
//    printValue("spin",spin);
//    printValue("isospin",isospin);
//    printValue("isospin3",isospin3);
//    printValue("quarks",quarks);
//    printValue("antiquarks",antiquarks);
//    printValue("leptons",leptons);
    delete iss;
    index++;
    }

  //
  // read decays
  //
  double tBRatio, tRatio;
  int    CGcoeff; // complete branching ratio by Clebsch-Gordan coefficient: 0-no 1-yes
  ParticleType * parentType = nullptr;
  ParticleType * childType1 = nullptr;
  ParticleType * childType2 = nullptr;
  ParticleType * childType3 = nullptr;
  ParticleType * childType4 = nullptr;
  double mass0 = 0;
  double mass1 = 0;
  double mass2 = 0;
  double mass3 = 0;
  double mass4 = 0;

  while (!inputFileDecays.eof())
    {
    inputFileDecays.getline(buff,300);
    if (!(*buff) || (*buff == '#')) continue;
    char parentName[30];
    char childName1[30];
    char childName2[30];
    char childName3[30];
    char childName4[30];
    istringstream * isss = new istringstream(buff);
    *isss >> parentName >> childName1 >> childName2 >> childName3 >> childName4 >> tBRatio >> CGcoeff;;
    //cout << parentName << "  " << childName1 << "  " << childName2 << "  " << childName3 << "  " <<  childName4 <<"  " << tBRatio << "  " <<CGcoeff <<  endl;;
    delete isss;

//    printValue("parentName",parentName);
//    printValue("childName1",childName1);
//    printValue("childName2",childName2);

    parentType = nullptr;
    childType1 = nullptr;
    childType2 = nullptr;
    childType3 = nullptr;
    childType4 = nullptr;
    mass0 = 0;
    mass1 = 0;
    mass2 = 0;
    mass3 = 0;
    mass4 = 0;

    parentType = particleDb.particleType(parentName);
    mass0 = parentType->mass();
    childType1 = particleDb.particleType(childName1);
    mass1 = childType1->mass();
    childType2 = particleDb.particleType(childName2);
    mass2 = childType2->mass();
    String name3(childName3);
    if (!name3.EqualTo("none"))
      {
      childType3 = particleDb.particleType(childName3);
      mass3 = childType3->mass();
      }
    String name4(childName4);
    if (!name4.EqualTo("none"))
      {
      childType4 = particleDb.particleType(childName4);
      mass4 = childType4->mass();
      }
    double massDiff = mass1+mass2+mass3+mass4-mass0;
    if (massDiff>0  &&  verbose)
      {
      printCR();
      printValue("Mass exception: parentName",parentName);
      printValue("Mass exception: childName1",childName1);
      printValue("Mass exception: childName2",childName2);
      printValue("Mass exception: massDiff",massDiff);
      }

    if (CGcoeff)
      {
      // complete branching ratio by Clebsch-Gordan coefficient
        double j1, m1, j2, m2, J, M, CB;
        J  = parentType->isospin();
        M  = parentType->isospin3();
        j1 = childType1->isospin();
        m1 = childType1->isospin3();
        j2 = childType2->isospin();
        m2 = childType2->isospin3();
        CB = clebschGordan(J, M, j1, m1, j2, m2);
        tRatio = CB*CB * tBRatio;
        // Multiply the Clebsh by two?
        // The same spin, mass, strangeness (charm?)
        // and different I3?
        if (  (absolute(childType1->spin()  - childType2->spin()) < 0.01)
            &&  (absolute(childType1->mass() - childType2->mass()) < 0.01)
            &&  (absolute(childType1->isospin3()-childType2->isospin3())   > 0.01)
            &&  (childType1->strangeness()-childType2->strangeness() == 0)
            &&  (childType1->charm()- childType2->charm() == 0)        )
          {
          tRatio *= 2.0;
          }
      }
    else
      {
      tRatio = tBRatio;
      }
    ParticleDecayMode decayMode;
    decayMode.setBranchingRatio(tRatio);
    decayMode.addChild(childType1);
    decayMode.addChild(childType2);
    if (childType3) decayMode.addChild(childType3);
    if (childType4) decayMode.addChild(childType3);
    parentType->addDecayMode(decayMode);
   // parentType->enableDecay();
    }

  if (verbose)
    {
    printCR();
    printValue("Total index of particles read",particleDb.size());
    }
  inputFileDecays.close();
  particleDb.mapAntiParticleIndices();
  particleDb.setupDecayGenerator();
}

double ParticleDbParser::clebschGordan(double aJot,  double aEm,
                                       double aJot1, double aEm1,
                                       double aJot2, double aEm2)
{
  int mint, maxt;
  double cgc = 0.0;
  int tIter;
  double coef;

  maxt = lrint(aJot1 + aJot2 - aJot);
  mint = 0;
  if (lrint(aJot1 - aEm1)  < maxt) maxt = lrint(aJot1 - aEm1);
  if (lrint(aJot2 + aEm2)  < maxt) maxt = lrint(aJot2 + aEm2);
  if (lrint(-(aJot-aJot2+aEm1))  > mint) mint = lrint(-(aJot-aJot2+aEm1));
  if (lrint(-(aJot-aJot1-aEm2))  > mint) mint = lrint(-(aJot-aJot1-aEm2));

  //  // PRINT_DEBUG_3("\t\t\tmint " << mint << " j1 " <<  aJot1 << " m1 " << aEm1);
  //  // PRINT_DEBUG_3("\t\t\tampxt " << maxt << " j2 " <<  aJot2 << " m2 " << aEm2);

  for (tIter = mint; tIter<=maxt; tIter ++)
    {
    coef = power(-1, tIter);
    // PRINT_DEBUG_3("\t\t\tcoef1 " << coef);
    coef *= std::sqrt( (2*aJot+1)
                 * factorials[lrint(aJot1+aEm1)]
                 * factorials[lrint(aJot1-aEm1)]
                 * factorials[lrint(aJot2+aEm2)]
                 * factorials[lrint(aJot2-aEm2)]
                 * factorials[lrint(aJot+aEm)]
                 * factorials[lrint(aJot-aEm)]
                 );
    // PRINT_DEBUG_3("\t\t\tcoef2 " << coef);
    coef /= factorials[tIter]
    * factorials[lrint(aJot1+aJot2-aJot-tIter)]
    * factorials[lrint(aJot1-aEm1-tIter)]
    * factorials[lrint(aJot2+aEm2-tIter)]
    * factorials[lrint(aJot-aJot2+aEm1+tIter)]
    * factorials[lrint(aJot-aJot1-aEm2+tIter)];
    // PRINT_DEBUG_3("\t\t\tcoef3 " << coef);
    cgc += coef;
    }
  cgc *= deltaJ(aJot1, aJot2, aJot);

  return cgc;
}

double ParticleDbParser::deltaJ(double aJot1, double aJot2, double aJot)
{
  double res = std::sqrt( 1.0
                    * factorials[lrint( aJot1 + aJot2 - aJot)]
                    * factorials[lrint( aJot1 - aJot2 + aJot)]
                    * factorials[lrint(-aJot1 + aJot2 + aJot)]
                    / factorials[lrint( aJot1 + aJot2 + aJot + 1)]
                    );
  return res;
}





}
