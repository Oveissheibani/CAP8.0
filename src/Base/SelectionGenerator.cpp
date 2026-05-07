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
#include "SelectionGenerator.hpp"

ClassImp(CAP::SelectionGenerator);

CAP::SelectionGenerator::SelectionGenerator()
:
cumulativeProbability()
{    }

CAP::SelectionGenerator::SelectionGenerator(std::vector<double> & probabilities)
:
cumulativeProbability()
{
  initializeWith(probabilities);
}

CAP::SelectionGenerator & CAP::SelectionGenerator::operator=(const CAP::SelectionGenerator & source)
{
  if (this!=&source)
    {
    cumulativeProbability = source.cumulativeProbability;
    }
  return *this;
}

void CAP::SelectionGenerator::initializeWith(std::vector<double> & probabilities)
{
  cumulativeProbability.clear();
  int n = probabilities.size();
  double sum = 0.0;
  for (int k=0; k<n; k++)
    {
    sum += probabilities[k];
    }
  double prob = 0.0;
  for (int kk=0; kk<n; kk++)
    {
    prob += probabilities[kk]/sum;
    cumulativeProbability.push_back(prob);
    }
}


int CAP::SelectionGenerator::generate()
{
  double v = gRandom->Rndm();
  int n = cumulativeProbability.size();
  for (int k=0; k<n; k++)
  {
  if (v <= cumulativeProbability[k]) return k;
  }
  return n-1;
}

