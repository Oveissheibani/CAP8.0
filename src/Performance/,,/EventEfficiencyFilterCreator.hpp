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
#ifndef CAP__EventEfficiencyFilterCreator
#define CAP__EventEfficiencyFilterCreator
#include "FilterCreator.hpp"
#include "EventEfficiencyFilter.hpp"

namespace CAP
{
class EventEfficiencyFilterCreator
:
public FilterCreator<EventEfficiencyFilter>
{
public:

  EventEfficiencyFilterCreator();
  EventEfficiencyFilterCreator(const EventEfficiencyFilterCreator & task);
  EventEfficiencyFilterCreator & operator=(const EventEfficiencyFilterCreator & task);
  virtual ~EventEfficiencyFilterCreator() {}

  virtual void configure();
  virtual void configureEventFilters(const String  & taskReferenceName);
  virtual void initialize();

  virtual EventEfficiencyFilter * createFilter(const String & name,
                                               const String & title,
                                               const vector<String>  & conditionTypes,
                                               const vector<String>  & conditionSubtypes,
                                               const vector<double>  & conditionMinima,
                                               const vector<double>  & conditionMaxima,
                                               const vector<double>  & conditionMinima2,
                                               const vector<double>  & conditionMaxima2,
                                               bool verbose=1);
  
  ClassDef(EventEfficiencyFilterCreator,0)
};

}

#endif /* CAP_FilterCreator */


