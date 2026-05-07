/* **********************************************************************
 * Copyright (C) 2019-2024, Claude Pruneau, Akash Raj
 * All rights reserved.
 *
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 *
 * Author: Claude Pruneau, Akash Raj,  Nov 2024
 *
 * *********************************************************************/
#ifndef CAP__JetCalculator
#define CAP__JetCalculator
#include "Calculator.hpp"
#include "JetFilter.hpp"

namespace CAP
{

class JetCalculator
:
public Calculator,
public ManagedList<JetFilter>
{
public:

  JetCalculator();
  JetCalculator(const JetCalculator & task);
  JetCalculator & operator=(const JetCalculator & task);
  virtual ~JetCalculator() {}
  virtual void setDefaultConfiguration();
  virtual void execute();
  virtual void create(const Configuration & configuration);
  virtual void loadFrom(TFile & inputFile,const Configuration & configuration);
  virtual void scaleHistograms();

protected:

  std::vector<JetFilter*> jetFilters;
  unsigned int nJetFilters();

  ClassDef(JetCalculator,0)
};

} // namespace CAP

#endif /* CAP__JetCalculator */
