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
#ifndef CAP__ParticleDbXmlWriter
#define CAP__ParticleDbXmlWriter
#include "ParticleDb.hpp"
#include "StreamHelpers.hpp"

namespace CAP
{

class ParticleDbXmlWriter
{
public:

  ParticleDbXmlWriter() {};
  virtual ~ParticleDbXmlWriter() {}
  virtual void writeDb(ParticleDb & _ParticleDb, std::ostream & outputFile=std::cout, bool verbose=false);
  virtual void writeParticleType(ParticleType & _particleType, std::ostream & outputFile=std::cout, bool verbose=false);
  virtual void writeParameter(const String & parameterName, const String & value, std::ostream & outputFile=std::cout, bool verbose=false);
  virtual void writeParameter(const String & parameterName, int value, std::ostream & outputFile=std::cout, bool verbose=false);
  virtual void writeParameter(const String & parameterName, double value, std::ostream & outputFile=std::cout, bool verbose=false);
  virtual void writeParameter(const String & parameterName, const vector<String> & values, std::ostream & outputFile=std::cout, bool verbose=false);
  virtual void writeParameter(const String & parameterName, const vector<int> & values, std::ostream & outputFile=std::cout, bool verbose=false);
  virtual void writeParameter(const String & parameterName, const vector<double> & values, std::ostream & outputFile=std::cout, bool verbose=false);
  virtual void writeParameter(const String & parameterName, const vector<String> & args, const vector<String> & argValues, std::ostream & outputFile=std::cout, bool verbose=false);
  virtual void writeParameter(const String & parameterName, const vector<String> & args, const vector<int> & argValues, std::ostream & outputFile=std::cout, bool verbose=false);
  virtual void writeParameter(const String & parameterName, const vector<String> & args, const vector<double> & argValues, std::ostream & outputFile=std::cout, bool verbose=false);
  virtual void writeBeginTag(const String & parameterName, std::ostream & outputFile=std::cout, bool verbose=false);
  virtual void writeBeginTag(const String & parameterName, const vector<String> & args, const vector<String> & argValues, std::ostream & outputFile=std::cout, bool verbose=false);
  virtual void writeBeginTag(const String & parameterName, const vector<String> & args, const vector<int> & argValues, std::ostream & outputFile=std::cout, bool verbose=false);
  virtual void writeBeginTag(const String & parameterName, const vector<String> & args, const vector<double> & argValues, std::ostream & outputFile=std::cout, bool verbose=false);
  virtual void writeEndTag(const String & parameterName, std::ostream & outputFile=std::cout, bool verbose=false);
  ClassDef(ParticleDbXmlWriter,0)
};

} // namespace CAP

#endif /* ParticleDbParser_hpp */
