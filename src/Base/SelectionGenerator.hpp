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
#ifndef CAP__SelectionGenerator
#define CAP__SelectionGenerator
#include <vector>
#include "TRandom.h"

namespace CAP
{
//!
//! SelectionGenerator
//!
//! Produces a random integer based
//! on a probability distribution.
//!
class SelectionGenerator
{
public:
  SelectionGenerator();
  SelectionGenerator(std::vector<double> & probabilities);
  SelectionGenerator & operator=(const SelectionGenerator & source);

  virtual ~SelectionGenerator(){}
  virtual void initializeWith(std::vector<double> & probabilities);
  virtual int generate();
  int nPartitions() const
  {
    return cumulativeProbability.size();
  }

protected:

  std::vector<double> cumulativeProbability;
  ClassDef(SelectionGenerator,0)
};

}

#endif /* SelectionGenerator_hpp */
