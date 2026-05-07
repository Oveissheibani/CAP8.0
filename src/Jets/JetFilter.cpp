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
#include "JetFilter.hpp"
#include "Manager.hpp"
#include "PrintHelpers.hpp"
#include "fastjet/ClusterSequence.hh"

ClassImp(CAP::Filter<fastjet::PseudoJet>);
ClassImp(CAP::JetFilter);

namespace CAP
{

JetFilter::JetFilter()
:
Filter<fastjet::PseudoJet>()
{ /* no ops */}

JetFilter::JetFilter(const JetFilter & source)
:
Filter<fastjet::PseudoJet>(source)
{ /* no ops */}

JetFilter & JetFilter::operator=(const JetFilter & source)
{
  if (this!=&source)
    {
    Filter<fastjet::PseudoJet>::operator=(source);
    }
  return *this;
}


bool JetFilter::accept(const fastjet::PseudoJet & pseudoJet)
{
  if (nConditions()<1) return true;
  bool   accepting = false;

  double jet_px = pseudoJet.px();
  double jet_py = pseudoJet.py();
  double jet_pz = pseudoJet.pz();
  double jet_pt = sqrt(jet_px*jet_px + jet_py*jet_py);
  double jet_p  = sqrt(jet_pt*jet_pt + jet_pz*jet_pz);
  double jet_eta = 0.0;
  double jet_mult = pseudoJet.constituents().size();

  for (auto & condition : conditions)
    {
    int filterSubType = condition->conditionSubtype;
    switch (filterSubType)
      {
        case kP:    accepting = condition->accept(jet_p);    break;
        case kPt:   accepting = condition->accept(jet_pt);   break;
        case kEta:  accepting = condition->accept(jet_eta);  break;
        case kMult: accepting = condition->accept(jet_mult); break;
      }
    if (!accepting)  return false;
    }
  return true;
}

} // namespace CAP
