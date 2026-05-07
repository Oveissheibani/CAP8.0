/* **********************************************************************
 * Copyright (C) 2019-2024, Claude Pruneau, Victor Gonzalez
 * ALL rights reserved.
 *
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 *
 * Author: Claude Pruneau,   04/01/2024
 *
 * *********************************************************************/
#include "EventEfficiencyFilterCreator.hpp"
#include "EventEfficiencyFilter.hpp"
#include "PrintHelpers.hpp"

ClassImp(CAP::FilterCreator<EventEfficiencyFilter>);
ClassImp(CAP::EventEfficiencyFilterCreator);

namespace CAP
{

EventEfficiencyFilterCreator::EventEfficiencyFilterCreator()
:
FilterCreator<EventEfficiencyFilter>()
{
 logger.appendClassName("EventEfficiencyFilterCreator");
 logger.setInstanceName("EventEfficiencyFilterCreator");
  setName("EventEfficiencyFilterCreator");
  setTitle("EventEfficiencyFilterCreator");
}

EventEfficiencyFilterCreator::EventEfficiencyFilterCreator(const EventEfficiencyFilterCreator & source)
:
FilterCreator<EventEfficiencyFilter>(source)
{   }

EventEfficiencyFilterCreator & EventEfficiencyFilterCreator::operator=(const EventEfficiencyFilterCreator & rhs)
{
  if (this!=&rhs)
    {
    FilterCreator<EventEfficiencyFilter>::operator=(rhs);
    }
  return *this;
}

///
void EventEfficiencyFilterCreator::configure()
{
  FilterCreator<EventEfficiencyFilter>::configure();
  configureEventFilters(getName());
}

void EventEfficiencyFilterCreator::configureEventFilters(const String  & taskName)
{
  int nEventEfficiencyFilters = requestedConfiguration->getValueInt(taskName+":EventEfficiencyFilter:N");

  for (int k=0; k<nEventEfficiencyFilters; k++)
    {
    String key = taskName;
    key += ":EventEfficiencyFilter";
    key += k;
    key += ":Name";
    String name = requestedConfiguration->getValueString(key);
    //printCR();
    //printValue("EventEfficiencyFilter Index",k);
    //printValue("EventEfficiencyFilter Name",name);
    createObject(name);
    }
}

void EventEfficiencyFilterCreator::initialize()
{
  FilterCreator<EventEfficiencyFilter>::initialize();
  String taskName = getName();
  String key;
  String name;
  String title;
  String conditionType;
  String conditionSubtype;
  double minimum;
  double maximum;
  //  double minimum2;
  //  double maximum2;
  int    nConditions;
  vector<String> conditionTypes;
  vector<String> conditionSubtypes;
  vector<double> conditionMinima;
  vector<double> conditionMaxima;
  vector<double> conditionMinima2;
  vector<double> conditionMaxima2;

  int nEventEfficiencyFilters = getValueInt("EventEfficiencyFilter:N");
  if (nEventEfficiencyFilters<1) throw TaskException("nEventEfficiencyFilters<1",__FUNCTION__);

  //printCR();
  //printLine();
  //printString("EventEfficiencyFilterCreator::initialize() Building filters");
  //printValue("EventEfficiencyFilterCreator::initialize()  nEventEfficiencyFilters",nEventEfficiencyFilters);
  for (int iEventEfficiencyFilter=0; iEventEfficiencyFilter<nEventEfficiencyFilters; iEventEfficiencyFilter++)
    {
    conditionTypes.clear();
    conditionSubtypes.clear();
    conditionMinima.clear();
    conditionMaxima.clear();
    conditionMinima2.clear();
    conditionMaxima2.clear();
    key =  "EventEfficiencyFilter"; key += iEventEfficiencyFilter; key += ":Name";
    name = getValueString(key);
    key =  "EventEfficiencyFilter"; key += iEventEfficiencyFilter; key += ":Title";
    title = getValueString(key);
    key =  "EventEfficiencyFilter"; key += iEventEfficiencyFilter; key += ":nConditions";
    nConditions = getValueInt(key);
    for (int iCondition=0; iCondition<nConditions; iCondition++)
      {
      key =  "EventEfficiencyFilter"; key += iEventEfficiencyFilter; key += ":Condition"; key += iCondition;key += ":Type";
      conditionType  = getValueString(key);
      conditionTypes.push_back(conditionType);
      key =  "EventEfficiencyFilter"; key += iEventEfficiencyFilter; key += ":Condition"; key += iCondition;key += ":Subtype";
      conditionSubtype  = getValueString(key);
      conditionSubtypes.push_back(conditionSubtype);
      key =  "EventEfficiencyFilter"; key += iEventEfficiencyFilter; key += ":Condition"; key += iCondition;key += ":Minimum";
      minimum  = getValueDouble(key);
      conditionMinima.push_back(minimum);
      key =  "EventEfficiencyFilter"; key += iEventEfficiencyFilter; key += ":Condition"; key += iCondition;key += ":Maximum";
      maximum  = getValueDouble(key);
      conditionMaxima.push_back(maximum);
      conditionMinima2.push_back(0.0);
      conditionMaxima2.push_back(0.0);
      }
    //    printValue("conditionTypes.size()",conditionTypes.size());
    //    printValue("conditionSubtypes.size()",conditionSubtypes.size());
    //    printValue("conditionMinima.size()",conditionMinima.size());
    //    printValue("conditionMaxima.size()",conditionMaxima.size());
    //    printValue("conditionMinima2.size()",conditionMinima2.size());
    //    printValue("conditionMaxima2.size()",conditionMaxima2.size());
    //    printString("EventEfficiencyFilterCreator::initialize() Assembled all pieces");

    createFilter(name,title,
                 conditionTypes, conditionSubtypes,
                 conditionMinima, conditionMaxima,
                 conditionMinima2, conditionMaxima2);
    }
}

///
///


EventEfficiencyFilter *  EventEfficiencyFilterCreator::createFilter(const String & name,
                                                                    const String & title,
                                                                    const vector<String>  & conditionTypes,
                                                                    const vector<String>  & conditionSubtypes,
                                                                    const vector<double>  & conditionMinima,
                                                                    const vector<double>  & conditionMaxima,
                                                                    const vector<double>  & conditionMinima2,
                                                                    const vector<double>  & conditionMaxima2,
                                                                    bool verbose)
{
  if (verbose)
    {
    printCR();
    printLine();
    printValue("EventEfficiencyFilterCreator::createFilter() name",name);
    printValue("EventEfficiencyFilterCreator::createFilter() title",title);
    for (unsigned int k=0; k<conditionTypes.size(); k++)
      {
      printValue("EventEfficiencyFilterCreator() condition index",  k);
      printValue("EventEfficiencyFilterCreator() conditionTypes",   conditionTypes[k]);
      printValue("EventEfficiencyFilterCreator() conditionSubtypes",conditionSubtypes[k]);
      printValue("EventEfficiencyFilterCreator() conditionMinima",  conditionMinima[k]);
      printValue("EventEfficiencyFilterCreator() conditionMaxima",  conditionMaxima[k]);
      printValue("EventEfficiencyFilterCreator() conditionMinima2", conditionMinima2[k]);
      printValue("EventEfficiencyFilterCreator() conditionMaxima2", conditionMaxima2[k]);
      }
    printLine();
    }
  EventEfficiencyFilter * filter = ManagedList< EventEfficiencyFilter >::getStoreObjectNamed(name);
  filter->EventEfficiencyFilter::setTitle(title);
  filter->EventEfficiencyFilter::setName(name);
  for (unsigned int k=0; k<conditionTypes.size();k++)
    {
    String conditionType    = conditionTypes[k];
    String conditionSubtype = conditionSubtypes[k];
    //int type;
    int subtype;
    if (conditionType.EqualTo("DoubleRange"))
      {
      if (conditionSubtype.EqualTo("MULT_0"))
        subtype = EventEfficiencyFilter::kMult0;
      else if (conditionSubtype.EqualTo("MULT_1"))
        subtype = EventEfficiencyFilter::kMult1;
      else if (conditionSubtype.EqualTo("MULT_2"))
        subtype = EventEfficiencyFilter::kMult2;
      else if (conditionSubtype.EqualTo("MULT_3"))
        subtype = EventEfficiencyFilter::kMult3;
      else if (conditionSubtype.EqualTo("MULT_4"))
        subtype = EventEfficiencyFilter::kMult4;
      else if (conditionSubtype.EqualTo("MULT_5"))
        subtype = EventEfficiencyFilter::kMult5;
      else if (conditionSubtype.EqualTo("ENERGY_0"))
        subtype = EventEfficiencyFilter::kEnergy0;
      else if (conditionSubtype.EqualTo("ENERGY_1"))
        subtype = EventEfficiencyFilter::kEnergy1;
      else if (conditionSubtype.EqualTo("ENERGY_2"))
        subtype = EventEfficiencyFilter::kEnergy2;
      else if (conditionSubtype.EqualTo("ENERGY_3"))
        subtype = EventEfficiencyFilter::kEnergy3;
      else if (conditionSubtype.EqualTo("ENERGY_4"))
        subtype = EventEfficiencyFilter::kEnergy4;
      else if (conditionSubtype.EqualTo("ENERGY_5"))
        subtype = EventEfficiencyFilter::kEnergy5;
      else if (conditionSubtype.EqualTo("SPHERO_0"))
        subtype = EventEfficiencyFilter::kSphero0;
      else if (conditionSubtype.EqualTo("SPHERO_1"))
        subtype = EventEfficiencyFilter::kSphero1;
      else if (conditionSubtype.EqualTo("SPHERO_2"))
        subtype = EventEfficiencyFilter::kSphero2;
      else if (conditionSubtype.EqualTo("SPHERO_3"))
        subtype = EventEfficiencyFilter::kSphero3;
      else if (conditionSubtype.EqualTo("SPHERO_4"))
        subtype = EventEfficiencyFilter::kSphero4;
      else if (conditionSubtype.EqualTo("SPHERO_5"))
        subtype = EventEfficiencyFilter::kSphero5;
      else
        throw FilterException(conditionSubtype,__FUNCTION__);
      filter->addCondition(subtype,conditionMinima[k],conditionMaxima[k]);
      }
    else if (conditionType.EqualTo("DoubleDualRange"))
      {
      if (conditionSubtype.EqualTo("MULT_0"))
        subtype = EventEfficiencyFilter::kMult0;
      else if (conditionSubtype.EqualTo("MULT_1"))
        subtype = EventEfficiencyFilter::kMult1;
      else if (conditionSubtype.EqualTo("MULT_2"))
        subtype = EventEfficiencyFilter::kMult2;
      else if (conditionSubtype.EqualTo("MULT_3"))
        subtype = EventEfficiencyFilter::kMult3;
      else if (conditionSubtype.EqualTo("MULT_4"))
        subtype = EventEfficiencyFilter::kMult4;
      else if (conditionSubtype.EqualTo("MULT_5"))
        subtype = EventEfficiencyFilter::kMult5;
      else if (conditionSubtype.EqualTo("ENERGY_0"))
        subtype = EventEfficiencyFilter::kEnergy0;
      else if (conditionSubtype.EqualTo("ENERGY_1"))
        subtype = EventEfficiencyFilter::kEnergy1;
      else if (conditionSubtype.EqualTo("ENERGY_2"))
        subtype = EventEfficiencyFilter::kEnergy2;
      else if (conditionSubtype.EqualTo("ENERGY_3"))
        subtype = EventEfficiencyFilter::kEnergy3;
      else if (conditionSubtype.EqualTo("ENERGY_4"))
        subtype = EventEfficiencyFilter::kEnergy4;
      else if (conditionSubtype.EqualTo("ENERGY_5"))
        subtype = EventEfficiencyFilter::kEnergy5;
      else if (conditionSubtype.EqualTo("SPHERO_0"))
        subtype = EventEfficiencyFilter::kSphero0;
      else if (conditionSubtype.EqualTo("SPHERO_1"))
        subtype = EventEfficiencyFilter::kSphero1;
      else if (conditionSubtype.EqualTo("SPHERO_2"))
        subtype = EventEfficiencyFilter::kSphero2;
      else if (conditionSubtype.EqualTo("SPHERO_3"))
        subtype = EventEfficiencyFilter::kSphero3;
      else if (conditionSubtype.EqualTo("SPHERO_4"))
        subtype = EventEfficiencyFilter::kSphero4;
      else if (conditionSubtype.EqualTo("SPHERO_5"))
        subtype = EventEfficiencyFilter::kSphero5;
      else
        throw FilterException(conditionSubtype,__FUNCTION__);
      filter->addCondition(subtype,
                           conditionMinima[k],conditionMaxima[k],
                           conditionMinima2[k],conditionMaxima2[k]);
      }
    else
      throw FilterException(conditionSubtypes[k],__FUNCTION__);
    }
  return filter;
}

} // namespace CAP
