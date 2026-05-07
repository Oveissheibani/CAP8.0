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
#include "ParticleDbXmlWriter.hpp"
#include "PrintHelpers.hpp"
#include "ParticleDbExceptions.hpp"

using CAP::ParticleDbXmlWriter;

ClassImp(CAP::ParticleDbXmlWriter);


namespace CAP
{

void ParticleDbXmlWriter::writeDb(ParticleDb & ParticleDb, std::ostream & outputFile, bool verbose)
{
  std::vector<ParticleType*> & _particleTypes = ParticleDb._particleTypes();
  unsigned int nParticles = _particleTypes.size();
  if (nParticles<1)
    throw EmptyDbParticleDbException(ParticleDb.name(),__FUNCTION__);
  writeBeginTag("ParticleDb",outputFile,verbose);
  for (auto particleType : _particleTypes) writeParticleType(*particleType,outputFile,verbose);
  writeEndTag("ParticleDb",outputFile,verbose);
}

void ParticleDbXmlWriter::writeParticleType(ParticleType & particleType, std::ostream & outputFile, bool verbose)
{
  vector<String> args;
  vector<int>    intValues;
  vector<double> doubleValues;
  writeParameter("index",        particleType.index,        outputFile,verbose);
  writeParameter("name",         particleType.name,         outputFile,verbose);
  writeParameter("title",        particleType.title,        outputFile,verbose);
  writeParameter("userCode",     particleType.privateCode,  outputFile,verbose);
  writeParameter("pdgCode",      particleType.pdgCode,      outputFile,verbose);
  doubleValues.push_back(particleType.mass);
  doubleValues.push_back(particleType.massUncertainty);
  writeParameter("mass",         doubleValues,              outputFile,verbose);
  writeParameter("charge",       particleType.charge,       outputFile,verbose);
  doubleValues.clear();
  doubleValues.push_back(particleType.width);
  doubleValues.push_back(particleType.widthUncertainty);
  writeParameter("width",        doubleValues,              outputFile,verbose);
  writeParameter("spin",         particleType.spin,         outputFile,verbose);
  writeParameter("isospin",      particleType.isospin,      outputFile,verbose);
  writeParameter("isospin3",     particleType.isospin3,     outputFile,verbose);
  writeParameter("strangeness",  particleType.strangeness,  outputFile,verbose);
  writeParameter("charm",        particleType.charm,        outputFile,verbose);
  writeParameter("baryon",       particleType.baryon,       outputFile,verbose);
  writeParameter("enabled",      particleType.enabled,      outputFile,verbose);
  writeParameter("decayEnabled", particleType.decayEnabled, outputFile,verbose);
  args.push_back("nq");  intValues.push_back(particleType.nq);
  args.push_back("naq"); intValues.push_back(particleType.naq);
  args.push_back("ns");  intValues.push_back(particleType.ns);
  args.push_back("nas"); intValues.push_back(particleType.nas);
  args.push_back("nc");  intValues.push_back(particleType.nc);
  args.push_back("nac"); intValues.push_back(particleType.nac);
  args.push_back("nb");  intValues.push_back(particleType.nb);
  args.push_back("nab"); intValues.push_back(particleType.nab);
  args.push_back("nt");  intValues.push_back(particleType.nt);
  args.push_back("nat"); intValues.push_back(particleType.nat);
  writeParameter("quark",args,intValues,outputFile,verbose);
  args.clear();
  intValues.clear();
  args.push_back("electron"); intValues.push_back(particleType.leptonElectron);
  args.push_back("muon");     intValues.push_back(particleType.leptonMuon);
  args.push_back("tau");      intValues.push_back(particleType.leptonTau);
  writeParameter("lepton",args,intValues,outputFile,verbose);
}

void ParticleDbXmlWriter::writeParameter(const String & parameterName, const String & value, std::ostream & outputFile, bool verbose)
{
  writeBeginTag(parameterName,outputFile,verbose);
  if (verbose) cout << value << std::endl;
  outputFile << value << std::endl;
  writeEndTag(parameterName,outputFile,verbose);
}

void ParticleDbXmlWriter::writeParameter(const String & parameterName, int value, std::ostream & outputFile, bool verbose)
{
  writeBeginTag(parameterName,outputFile,verbose);
  if (verbose) cout << value << std::endl;
  outputFile << value << std::endl;
  writeEndTag(parameterName,outputFile,verbose);
}

void ParticleDbXmlWriter::writeParameter(const String & parameterName, double value, std::ostream & outputFile, bool verbose)
{
  writeBeginTag(parameterName,outputFile,verbose);
  if (verbose) cout << value << std::endl;
  outputFile << value << std::endl;
  writeEndTag(parameterName,outputFile,verbose);
}

void ParticleDbXmlWriter::writeParameter(const String & parameterName, const vector<String> & values, std::ostream & outputFile, bool verbose)
{
  writeBeginTag(parameterName,outputFile,verbose);
  unsigned int n = values.size();
  if (verbose) for (unsigned int k=0; k<n; k++) std::cout << values[k] << " ";
  for (unsigned int k=0; k<n; k++) outputFile << values[k] << " ";
  writeEndTag(parameterName,outputFile,verbose);
}

void ParticleDbXmlWriter::writeParameter(const String & parameterName, const vector<int> & values, std::ostream & outputFile, bool verbose)
{
  writeBeginTag(parameterName,outputFile,verbose);
  unsigned int n = values.size();
  if (verbose) for (unsigned int k=0; k<n; k++) std::cout << values[k] << " ";
  for (unsigned int k=0; k<n; k++) outputFile << values[k] << " ";
  writeEndTag(parameterName,outputFile,verbose);
}


void ParticleDbXmlWriter::writeParameter(const String & parameterName, const vector<double> & values, std::ostream & outputFile, bool verbose)
{
  writeBeginTag(parameterName,outputFile,verbose);
  unsigned int n = values.size();
  if (verbose) for (unsigned int k=0; k<n; k++) std::cout << values[k] << " ";
  for (unsigned int k=0; k<n; k++) outputFile << values[k] << " ";
  writeEndTag(parameterName,outputFile,verbose);
}

void ParticleDbXmlWriter::writeParameter(const String & parameterName, const vector<String> & args, const vector<String> & argValues, std::ostream & outputFile, bool verbose)
{
  writeBeginTag(parameterName,args,argValues,outputFile,verbose);
  writeEndTag(parameterName,outputFile,verbose);
}


void ParticleDbXmlWriter::writeParameter(const String & parameterName, const vector<String> & args, const vector<int> & argValues, std::ostream & outputFile, bool verbose)
{
  writeBeginTag(parameterName,args,argValues,outputFile,verbose);
  writeEndTag(parameterName,outputFile,verbose);
}

void ParticleDbXmlWriter::writeParameter(const String & parameterName, const vector<String> & args, const vector<double> & argValues, std::ostream & outputFile, bool verbose)
{
  writeBeginTag(parameterName,args,argValues,outputFile,verbose);
  writeEndTag(parameterName,outputFile,verbose);
}

void ParticleDbXmlWriter::writeBeginTag(const String & parameterName, std::ostream & outputFile, bool verbose)
{
  if (verbose) std::cout << "<" << parameterName << ">" << std::endl;
  outputFile << "<" << parameterName << ">" << std::endl;
}

void ParticleDbXmlWriter::writeBeginTag(const String & parameterName, const vector<String> & args, const vector<String> & argValues, std::ostream & outputFile, bool verbose)
{
  if (verbose)
    {
    std::cout << "<" << parameterName;
    unsigned int n = args.size();
    for (unsigned int k=0; k<n; k++)
      {
      std::cout << " " << args[k] << "=\"" << argValues[k] << "\"";
      }
    std::cout << " >" << std::endl;
    }
  outputFile << "<" << parameterName;
  unsigned int n = args.size();
  for (unsigned int k=0; k<n; k++)
    {
    outputFile << " " << args[k] << "=\"" << argValues[k] << "\"";
    }
  outputFile << " >" << std::endl;
}

void ParticleDbXmlWriter::writeBeginTag(const String & parameterName, const vector<String> & args, const vector<int> & argValues, std::ostream & outputFile, bool verbose)
{
  if (verbose)
    {
    std::cout << "<" << parameterName;
    unsigned int n = args.size();
    for (unsigned int k=0; k<n; k++)
      {
      std::cout << " " << args[k] << "=\"" << argValues[k] << "\"";
      }
    std::cout << " >" << std::endl;
    }
  outputFile << "<" << parameterName;
  unsigned int n = args.size();
  for (unsigned int k=0; k<n; k++)
    {
    outputFile << " " << args[k] << "=\"" << argValues[k] << "\"";
    }
  outputFile << " >" << std::endl;
}

void ParticleDbXmlWriter::writeBeginTag(const String & parameterName, const vector<String> & args, const vector<double> & argValues, std::ostream & outputFile, bool verbose)
{
  if (verbose)
    {
    std::cout << "<" << parameterName;
    unsigned int n = args.size();
    for (unsigned int k=0; k<n; k++)
      {
      std::cout << " " << args[k] << "=\"" << argValues[k] << "\"";
      }
    std::cout << " >" << std::endl;
    }
  outputFile << "<" << parameterName;
  unsigned int n = args.size();
  for (unsigned int k=0; k<n; k++)
    {
    outputFile << " " << args[k] << "=\"" << argValues[k] << "\"";
    }
  outputFile << " >" << std::endl;
}


void ParticleDbXmlWriter::writeEndTag(const String & parameterName, std::ostream & outputFile, bool verbose)
{
  if (verbose) std::cout << "</" << parameterName << ">" << std::endl;
  outputFile << "</" << parameterName << ">" << std::endl;
}


}
