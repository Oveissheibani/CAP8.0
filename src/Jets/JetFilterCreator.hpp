
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
#ifndef CAP__JetFilterCreator
#define CAP__JetFilterCreator
#include "FilterCreator.hpp"
#include "PseudoJet.hpp"

namespace CAP
{
class JetFilterCreator
:
public FilterCreator<JetFilter>
{

public:
  
  JetFilterCreator();
  JetFilterCreator(const JetFilterCreator & task);
  JetFilterCreator & operator=(const JetFilterCreator & task);
  virtual ~JetFilterCreator() {}

  void createFilter(const String & name,
                    const String & title,
                    const vector<String> & conditionTypes,
                    const vector<String> & conditionSubypes,
                    const vector<double> & conditionMinima,
                    const vector<double> & conditionMaxima,
                    const vector<double> & conditionMinima2,
                    const vector<double> & conditionMaxima2,
                    bool verbose=true);

  ClassDef(JetFilterCreator,0)
};

}

#endif /* CAP_FilterCreator */


