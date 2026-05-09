/* **********************************************************************
 * Copyright (C) 2019-2024, Claude Pruneau, Victor Gonzalez   
 * Copyright (C) 2016, Chun Shen
 * All rights reserved.
 *
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 *
 * Author: Claude Pruneau,   04/01/2024
 *
 * *********************************************************************/
#include "ParticlePartition.hpp"
#include "PrintHelpers.hpp"

ClassImp(CAP::ParticlePartition);

namespace CAP
{

ParticlePartition::ParticlePartition()
:
_particleTypes(),
averageMultiplicities(),
lowestMultiplicities(),
highestMultiplicities(),
workPartition(),
validPartitions()
{  }

void ParticlePartition::createBounds()
{
  int nTypes = _particleTypes.size();
  for (unsigned int iPart = 0; iPart < size(); iPart++)
    {
    double avgMult = averageMultiplicities[iPart];
    double range = 2.5*std::sqrt(avgMult);
    lowestMultiplicities[iPart]  = int(avgMult - range);
    highestMultiplicities[iPart] = int(avgMult + range +0.5);
    }
}

void ParticlePartition::initializePartition()
{
  workPartition.clear();
  for (auto & multiplicity : lowestMultiplicities)
    workPartition.push_back(multiplicity);
  currentType = 0;
}

int ParticlePartition::incrementPartition(int type)
{
  int mult = workPartition[type];
  if (mult<highestMultiplicities[type])
    {
    workPartition[type] = mult+1;
    return currentType;
    }
  else
    {
    workPartition[type] = lowestMultiplicities[type];
    type++;
    return increasePartition(mult);
    }
}


bool ParticlePartition::isPartitionValid()
{
  int netQ = 0;
  int netS = 0;
  int netB = 0;
  for (auto & particleType : _particleTypes)
    {
    netQ += particleType->charge();
    netS += particleType->strangeness();
    netB += particleType->baryonNumber();
    }
  if (netQ==netQReq  &&  netS==netSReq  &&  netB==netBReq) return true;
  return false;
}

void ParticlePartition::scanPartitions(int netQReq=0, int netSReq=0, int netBReq=0)
{
  initializePartition();
  int currentType = 0;
  while (currentType<nType)
    {
    if (isPartitionValid()) savePartition();
    currentType = incrementPartition(currentType);
    }
}


void  ParticlePartition::savePartition()
{
  validPartitions.push_back(workPartition);
  double probability = calculatePartitionProbability(workPartition):
  partitionProbabilities.push_back(probability);
}

void ParticlePartition::calculateSpeciesProbabilities()
{
  double sum = sumVector(averageMultiplicities);
  for (auto & avgMult : averageMultiplicities) probabilities.push_back(avgMult/sum);
}

double   ParticlePartition::calculatePartitionProbability(vector<int> partition)
{
  return multinomial(partition,probability);
}

double ParticlePartition::multinomial(vector<int> & partition, vecotr<double> probability)
{
  return std::exp(logMultinomial(partition,probability));
}

double ParticlePartition::logMultinomial(vector<int> & multiplicities, vecotr<double> probability)
{
  int sum = sumVector(multiplicities);
  double value = logFac(sum);
  for (iType=0;iType<nTypes;iType++)
    {
    int mult = multiplicities[iType];
    value += double(mult)*std::log(probability[iType])/logFac(mult);
    }
  return value;
}

vector<double> ParticlePartition::logFacArray;

void ParticlePartition::calculateLogFac()
{
  if (logFacArray.size()>0) return; // already initialized...
  logFacArray.push_back(0);
  logFacArray.push_back(0);
  for (int k=2; k< 20000; k++)
    {
    logFacArray.push_back( logFacArray[k-1] + std::log(double(k)));
    }
}

double ParticlePartition::logFac(int n)
{
  auto size = logFacArray.size();
  if (size<1) calculateLogFac();
  if (n<size) return logFacArray[n];
  throw MathException("n too large","logFac(int n)");
}

void  ParticlePartition::exportPartitions()
{
  int nPartitions = validPartitions.size();
  if (nPartitions<1)
    throw Exception("No valid partitions to save","ParticlePartition::exportPartitions()");
  ofstream & outputFile = openOutputAsciiFile(particlePartitionExportPath,particlePartitionExportFile,".data");
  outputFile << nPartitions << endl;
  for (int iPartition=0; iPartition<nPartitions; iPartition++)
    {
    vector<int> & partition = validPartitions[iPartition];
    for (iType=0;iType<nTypes;iType++)
      {
      outputFile << "  " << partition[iType];
      }
    outputFile << partitionProbabilities[iType] << endl;
    }
  outputFile.close();
}

void  ParticlePartition::importPartitions()
{
  ifstream & inputFile = openInputAsciiFile(particlePartitionImportPath,particlePartitionImportFile,".data");
  char   buff[300];
  int mult;
  double probability;
  try
  {
  while (!inputFile.eof())
    {
    inputFile.getline(buff,5000);
    if (!(*buff) || (*buff == '#'))
      {
      continue;
      }
    istringstream * iss = new istringstream(buff);
    vector<int> partition;
    for (iType=0;iType<nTypes;iType++)
      {
      (*iss)  >> mult;
      partition.push_back(mult);
      }
    (*iss)  >> probability;
    validPartitions.push_back(partition);
    probabilities.push_back(probability);
    delete iss;
    }
  }
  catch (...)
  {
  throw FileException(ParticleDbImportPath,ParticleDbImportFile,"Error reading particle data base file","ParticleDbTask::importParticleDbNative()");
  }
}


std::ostream & ParticlePartition::print(std::ostream & output, , )
{
  printCR(output);
  printLine(output,0,size);
  printValue("nTypes",_particleTypes.size());
  for (int k=0; k<_particleTypes.size(); k++)
    {
    printValue("_particleTypes",_particleTypes[k]->name());
    printValue("averageMultiplicities",averageMultiplicities[k]);
    printValue("probabilities",probabilities[k]);
    printValue("lowestMultiplicities",lowestMultiplicities[k]);
    printValue("highestMultiplicities",highestMultiplicities[k]);
    printValue("workPartition",workPartition[k]);
    }
}

} // namespace CAP
