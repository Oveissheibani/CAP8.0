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
#include "JetFilterCreator.hpp"
#include "JetFilter.hpp"
#include "PrintHelpers.hpp"

ClassImp(CAP::FilterCreator<JetFilter>);
ClassImp(CAP::JetFilterCreator);

namespace CAP
{

JetFilterCreator::JetFilterCreator()
:
FilterCreator<JetFilter>()
{
 appendClassName("JetFilterCreator");
 setName("JetFilterCreator");
  setName("JetFilterCreator");
  setTitle("JetFilterCreator");
}


JetFilterCreator::JetFilterCreator(const JetFilterCreator & source)
:
FilterCreator<JetFilter>(source)
{   }

JetFilterCreator & JetFilterCreator::operator=(const JetFilterCreator & rhs)
{
  if (this!=&rhs)
    {
    FilterCreator<JetFilter>::operator=(rhs);
    }
  return *this;
}

void JetFilterCreator::createFilter(const String & name,
                                    const String & title,
                                    const vector<String> & conditionTypes,
                                    const vector<String> & conditionSubtypes,
                                    const vector<double> & conditionMinima,
                                    const vector<double> & conditionMaxima,
                                    const vector<double> & conditionMinima2,
                                    const vector<double> & conditionMaxima2,
                                    bool verbose)
{
  if (verbose)
    {
    printCR();
    printLine();
    printValue("JetFilterCreator::createFilter() name",name);
    printValue("JetFilterCreator::createFilter() title",title);
    for (unsigned int k=0; k<conditionTypes.size(); k++)
      {
      printValue("JetFilterCreator::createFilter() condition index",  k);
      printValue("JetFilterCreator::createFilter() conditionTypes",   conditionTypes[k]);
      printValue("JetFilterCreator::createFilter() conditionSubtypes",conditionSubtypes[k]);
      printValue("JetFilterCreator::createFilter() conditionMinima",  conditionMinima[k]);
      printValue("JetFilterCreator::createFilter() conditionMaxima",  conditionMaxima[k]);
      printValue("JetFilterCreator::createFilter() conditionMinima2", conditionMinima2[k]);
      printValue("JetFilterCreator::createFilter() conditionMaxima2", conditionMaxima2[k]);
      }
    printLine();
    }
  JetFilter * filter = ManagedList<JetFilter>::getStoreObjectNamed(name);
  filter->setTitle(title);
  filter->setName(name);

  for (unsigned int k=0; k<conditionTypes.size();k++)
    {
    String conditionType    = conditionTypes[k];
    String conditionSubtype = conditionSubtypes[k];
    //int type;
    int subtype;

    // kNone, kP, kPt, kEta, kMult

    if (conditionType.EqualTo("DoubleRange"))
      {
      if (conditionSubtype.EqualTo("P"))
        subtype = JetFilter::kP;
      else if (conditionSubtype.EqualTo("PT"))
        subtype = JetFilter::kPt;
      else if (conditionSubtype.EqualTo("ETA"))
        subtype = JetFilter::kEta;
      else if (conditionSubtype.EqualTo("MULT"))
        subtype = JetFilter::kMult;
      else
        throw FilterException(conditionSubtype,__FUNCTION__);
      filter->addCondition(subtype,conditionMinima[k],conditionMaxima[k]);
      }
    else if (conditionType.EqualTo("DoubleDualRange"))
      {
      if (conditionSubtype.EqualTo("P"))
        subtype = JetFilter::kP;
      else if (conditionSubtype.EqualTo("PT"))
        subtype = JetFilter::kPt;
      else if (conditionSubtype.EqualTo("ETA"))
        subtype = JetFilter::kEta;
      else if (conditionSubtype.EqualTo("MMULT"))
        subtype = JetFilter::kMult;
      else
        throw FilterException(conditionSubtype,__FUNCTION__);
      filter->addCondition(subtype,
                           conditionMinima[k],conditionMaxima[k],
                           conditionMinima2[k],conditionMaxima2[k]);
      }
    else
      throw FilterException(conditionType,__FUNCTION__);
    }
  return filter;
}



} // namespace CAP
