/* **********************************************************************
 * Copyright (C) 2019-2024, Claude Pruneau
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 *
 * Author: Claude Pruneau,   05/16/2024
 * This version uses the format of the Therminator package.
 * *********************************************************************/
#ifndef CAP__ParticleDbParser
#define CAP__ParticleDbParser
#include "PropertiesParser.hpp"
#include "ParticleDb.hpp"


namespace CAP
{

class ParticleDbParser : public PropertiesParser
{
public:

  ParticleDbParser();
  ParticleDbParser(Object::Severity _severity);
  virtual ~ParticleDbParser() {}
  virtual void readDb(ParticleDb & _particleDb,
                      ifstream & inputFileParticles,
                      ifstream & inputFileDecays,
                      bool verbose=false);
  double clebschGordan(double aJot,  double aEm,
                       double aJot1, double aEm1,
                       double aJot2, double aEm2);
  double deltaJ(double aJot1, double aJot2, double aJot);

  ClassDef(ParticleDbParser,0)
};

} // namespace CAP

#endif /* ParticleDbParser_hpp */
