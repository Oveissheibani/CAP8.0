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
#ifndef CAP__EventEfficiency
#define CAP__EventEfficiency
#include "HistogramGroup.hpp"
#include "Event.hpp"

namespace CAP
{

class EventEfficiency
:
public HistogramGroup
{
public:

  EventEfficiency();
  EventEfficiency(const EventEfficiency & otherFilter);
  EventEfficiency & operator=(const EventEfficiency & otherFilter);
  virtual ~EventEfficiency() {}
  virtual void configure(const String & ownerName,
                               const String & typeName,
                               const Configuration & configuration,
                               unsigned int index);
  virtual void create();
  virtual void loadFrom(TFile & inputFile);
  virtual double efficiencyAt(const Event & event);
  virtual double weightAt(const Event & event);
  virtual bool   accept(const Event & event);
  virtual void   calculateWeights();
protected:

  virtual void cloneB(const EventEfficiency & source);

  TH1 * h_efficiency_mult;
  TH1 * h_weight_mult;

  int nBins_mult;
  double min_mult;
  double max_mult;


  ClassDef(EventEfficiency,0)
};

}

#endif /* CAP__EventEfficiency */

