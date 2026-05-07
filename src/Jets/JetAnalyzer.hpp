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
#ifndef CAP__JetAnalyzer
#define CAP__JetAnalyzer
#include "EventProcessor.hpp"
#include "JetFilter.hpp"

namespace CAP
{

class JetAnalyzer
:
public EventProcessorSingle<JetSingleHistos,JetSingleDerivedHistos>,
public ManagedList<JetFilter>
{
public:

  JetAnalyzer();
  JetAnalyzer(const JetAnalyzer & soure);
  JetAnalyzer & operator=(const JetAnalyzer & rhs);
  virtual ~JetAnalyzer() {}
  virtual void setDefaultConfiguration();
  virtual void initialize();
  virtual void execute();

protected:

  std::vector<PseudoJet> pseudoJetsInput;
  std::vector<PseudoJet> clusteredJets = sorted_by_pt( clusterSequence->inclusive_jets(jetPtMin) );

  double jetRadius;
  double jetPtMin;


  ClassDef(JetAnalyzer,0)
};

} // namespace CAP

#endif /* CAP__JetAnalyzer */
