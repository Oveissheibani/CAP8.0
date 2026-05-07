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
#ifndef CAP__ParticleDbParser_CS
#define CAP__ParticleDbParser_CS
#include "PropertiesParser.hpp"
#include "ParticleDb.hpp"


namespace CAP
{

class ParticleDbParser_CS : public PropertiesParser
{
public:

  ParticleDbParser_CS();
  ParticleDbParser_CS(Object::Severity _severity);
  virtual ~ParticleDbParser_CS() {}
  virtual void readDb(ParticleDb  & _ParticleDb, ifstream & inputFile);


  ClassDef(ParticleDbParser_CS,0)

};

} // namespace CAP

#endif /* ParticleDbParser_CS_hpp */
