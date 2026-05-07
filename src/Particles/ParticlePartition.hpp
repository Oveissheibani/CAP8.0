/* **********************************************************************
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
#ifndef CAP__ParticlePartition
#define CAP__ParticlePartition
#include <iostream>
#include <vector>
#include "ParticleType.hpp"


namespace CAP
{

class ParticleType;

class ParticlePartition
{
protected:

  int nTypes;
  std::vector<ParticleType*> _particleTypes;
  std::vector<double> averageMultiplicities;
  std::vector<double> probabilities;
  std::vector<int> lowestMultiplicities;
  std::vector<int> highestMultiplicities;
  std::vector<int> workPartition;

  std::vector< std::vector<int> > validPartitions;
  std::vector< double > partitionProbabilities;

  static std::vector<double> logFacArray;
  static void calculateLogFac();


public:

  // const String& _name, bool _ownership, Severity logLevel
  ParticlePartition();
  virtual ~ParticlePartition() {}
  void readFromFile(const String & inputFileName);
  void writeToFile(const String & outputFileName, bool printDecays=true);

  void createBounds();
  bool isValid(int netCharge=0, int baryon=0, int netStrange=0);
  void initializePartition();
  int  incrementPartition(int type);
  bool isPartitionValid();
  void scanPartitions(int netQReq=0, int netSReq=0, int netBReq=0);
  void savePartition();
  void calculateSpeciesProbabilities();
  double calculatePartitionProbability(std::vector<int> partition);
  double multinomial(std::vector<int> & partition, vecotr<double> probability);
  double logMultinomial(std::vector<int> & multiplicities, std::vector<double> probability);
  double logFac(int n);
  void   exportPartitions();
  void importPartitions();
  void exportPartitions();


  templage<typename T>
  T  Partition::sumVector(std::vector<T> & vector)
  {
  int  sum = 0;
  for (int k=0; k<vector.size(); k++) sum += vector[k];
  return sum;
  }

  std::ostream & print(  );

  ClassDef(ParticlePartition,0)
};

} //namespace CAP

#endif  // CAP__ParticlePartition

