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
#include "ParticleEfficiencyFilterCreator.hpp"
#include "NameHelpers.hpp"
#include "PrintHelpers.hpp"

ClassImp(CAP::ParticleEfficiencyFilterCreator);

namespace CAP
{

ParticleEfficiencyFilterCreator::ParticleEfficiencyFilterCreator()
:
EventProcessor<Event,EventFilter,EventEfficiency,EventSmearer,Particle,ParticleFilter,ParticleEfficiency,
ParticleStearer,BaseHistos,BaseDerivedHistos>()
{
 logger.appendClassName("ParticleEfficiencyFilterCreator");
 logger.setInstanceName("ParticleEfficiencyFilterCreator");
  setName("ParticleEfficiencyFilterCreator");
  setTitle("ParticleEfficiencyFilterCreator");
}

ParticleEfficiencyFilterCreator::ParticleEfficiencyFilterCreator(const ParticleEfficiencyFilterCreator & source)
:
EventProcessor<Event,EventFilter,EventEfficiency,EventSmearer,Particle,ParticleFilter,ParticleEfficiency,
ParticleStearer,BaseHistos,BaseDerivedHistos>(source)
{   }

ParticleEfficiencyFilterCreator & ParticleEfficiencyFilterCreator::operator=(const ParticleEfficiencyFilterCreator & rhs)
{
  if (this!=&rhs)
    {
    EventProcessor<Event,EventFilter,EventEfficiency,EventSmearer,Particle,ParticleFilter,PARTICLE_EFFICIENCY,
    ParticleEfficiency,BaseHistos,BaseDerivedHistos>::operator=(rhs);
    }
  return *this;
}

void ParticleEfficiencyFilterCreator::initialize()
{
  if (reportInfo(__FUNCTION__)) { printCR(); }
  EventProcessor<Event,EventFilter,EventEfficiency,EventSmearer,Particle,ParticleFilter,PARTICLE_EFFICIENCY,
  ParticleEfficiency,BaseHistos,BaseDerivedHistos>::initialize();
  String key;
  String name;
  String title;
  String conditionType;
  String conditionSubtype;
  double minimum;
  double maximum;
  double minimum2;
  double maximum2;
  int    nConditions;
  vector<String> conditionTypes;
  vector<String> conditionSubtypes;
  vector<double> conditionMinima;
  vector<double> conditionMaxima;
  vector<double> conditionMinima2;
  vector<double> conditionMaxima2;

  int nParticleEfficiencyFilters = getValueInt("ParticleEfficiencyFilter:N");
  if (nParticleEfficiencyFilters<1) throw TaskException("ParticleEfficiencyFilter:N<1",__FUNCTION__);
  //  printCR();
  //  printLine();
  //  printString("Building filters");
  //  printValue("nParticleEfficiencyFilters",nParticleEfficiencyFilters);

  for (int iParticleEfficiencyFilter=0; iParticleEfficiencyFilter<nParticleEfficiencyFilters; iParticleEfficiencyFilter++)
    {
    conditionTypes.clear();
    conditionSubtypes.clear();
    conditionMinima.clear();
    conditionMaxima.clear();
    conditionMinima2.clear();
    conditionMaxima2.clear();

    key = createName("ParticleEfficiencyFilter",iParticleEfficiencyFilter,":Name");
    name = getValueString(key);
    key = createName("ParticleEfficiencyFilter",iParticleEfficiencyFilter,":Title");
    title = getValueString(key);
    key = createName("ParticleEfficiencyFilter",iParticleEfficiencyFilter,":nConditions");
    nConditions = getValueInt(key);
    for (int iCondition=0; iCondition<nConditions; iCondition++)
      {
      key = createName("ParticleEfficiencyFilter",iParticleEfficiencyFilter,":Condition",iCondition,":Type");
      conditionType  = getValueString(key);
      conditionTypes.push_back(conditionType);
      key = createName("ParticleEfficiencyFilter",iParticleEfficiencyFilter,":Condition",iCondition,":Subtype");
      conditionSubtype  = getValueString(key);
      conditionSubtypes.push_back(conditionSubtype);
      key = createName("ParticleEfficiencyFilter",iParticleEfficiencyFilter,":Condition",iCondition,":Minimum");
      minimum  = getValueDouble(key);
      conditionMinima.push_back(minimum);
      key = createName("ParticleEfficiencyFilter",iParticleEfficiencyFilter,":Condition",iCondition,":Maximum");
      maximum  = getValueDouble(key);
      conditionMaxima.push_back(maximum);

      if (conditionType.EqualTo("DoubleDualRange"))
        {
        key = createName("ParticleEfficiencyFilter",iParticleEfficiencyFilter,":Condition",iCondition,":Minimum2");
        minimum2  = getValueDouble(key);
        conditionMinima2.push_back(minimum2);
        key = createName("ParticleEfficiencyFilter",iParticleEfficiencyFilter,":Condition",iCondition,":Maximum2");
        maximum2  = getValueDouble(key);
        conditionMaxima2.push_back(maximum2);
        }
      else
        {
        conditionMinima2.push_back(0.0);
        conditionMaxima2.push_back(0.0);
        }
      //      printValue("iCondition",iCondition);
      //      printValue("conditionType",conditionType);
      //      printValue("conditionSubtype",conditionSubtype);
      //      printValue("minimum",minimum);
      //      printValue("maximum",maximum);
      //      printValue("minimum2",minimum2);
      //      printValue("maximum2",maximum2);

      }
    createFilter(name,title,
                 conditionTypes, conditionSubtypes,
                 conditionMinima, conditionMaxima,
                 conditionMinima2, conditionMaxima2);
    }
}



ParticleEfficiencyFilter * ParticleEfficiencyFilterCreator::createFilter(const String & name,
                                                   const String & title,
                                                   const vector<String>  & conditionTypes,
                                                   const vector<String>  & conditionSubtypes,
                                                   const vector<double>  & conditionMinima,
                                                   const vector<double>  & conditionMaxima,
                                                   const vector<double>  & conditionMinima2,
                                                   const vector<double>  & conditionMaxima2,
                                                   bool  verbose)
{
  if (reportInfo(__FUNCTION__))
    {
    printCR();
    printLine();
    printValue("ParticleEfficiencyFilterCreator::createFilter() name",name);
    printValue("ParticleEfficiencyFilterCreator::createFilter() title",title);
    for (unsigned int k=0; k<conditionTypes.size(); k++)
      {
      printValue("ParticleEfficiencyFilterCreator() condition index",  k);
      printValue("ParticleEfficiencyFilterCreator() conditionTypes",   conditionTypes[k]);
      printValue("ParticleEfficiencyFilterCreator() conditionSubtypes",conditionSubtypes[k]);
      printValue("ParticleEfficiencyFilterCreator() conditionMinima",  conditionMinima[k]);
      printValue("ParticleEfficiencyFilterCreator() conditionMaxima",  conditionMaxima[k]);
      printValue("ParticleEfficiencyFilterCreator() conditionMinima2", conditionMinima2[k]);
      printValue("ParticleEfficiencyFilterCreator() conditionMaxima2", conditionMaxima2[k]);
      }
    printLine();
    }
  ParticleEfficiencyFilter * filter = ManagedList< ParticleEfficiencyFilter >::getStoreObjectNamed(name);
  filter->ParticleEfficiencyFilter::setTitle(title);
  filter->ParticleEfficiencyFilter::setName(name);
  for (unsigned int k=0; k<conditionTypes.size();k++)
    {
    String conditionType    = conditionTypes[k];
    String conditionSubtype = conditionSubtypes[k];
    //int type;
    int subtype;
    if (conditionType.EqualTo("DoubleRange"))
      {
      if (conditionSubtype.EqualTo("PT"))
        subtype = ParticleEfficiencyFilter::kPt;
      else if (conditionSubtype.EqualTo("PX"))
        subtype = ParticleEfficiencyFilter::kPx;
      else if (conditionSubtype.EqualTo("PY"))
        subtype = ParticleEfficiencyFilter::kPy;
      else if (conditionSubtype.EqualTo("PZ"))
        subtype = ParticleEfficiencyFilter::kPz;
      else if (conditionSubtype.EqualTo("ENERGY"))
        subtype = ParticleEfficiencyFilter::kEnergy;
      else if (conditionSubtype.EqualTo("ETA"))
        subtype = ParticleEfficiencyFilter::kEta;
      else if (conditionSubtype.EqualTo("RAPIDITY"))
        subtype = ParticleEfficiencyFilter::kRapidity;
      else if (conditionSubtype.EqualTo("PHI"))
        subtype = ParticleEfficiencyFilter::kPhi;
      else
        throw FilterException(conditionSubtype,__FUNCTION__);
      filter->addCondition(subtype,conditionMinima[k],conditionMaxima[k]);
      }
    else if (conditionType.EqualTo("DoubleDualRange"))
      {
      if (conditionSubtype.EqualTo("PT"))
        subtype = ParticleEfficiencyFilter::kPt;
      else if (conditionSubtype.EqualTo("PX"))
        subtype = ParticleEfficiencyFilter::kPx;
      else if (conditionSubtype.EqualTo("PY"))
        subtype = ParticleEfficiencyFilter::kPy;
      else if (conditionSubtype.EqualTo("PZ"))
        subtype = ParticleEfficiencyFilter::kPz;
      else if (conditionSubtype.EqualTo("ENERGY"))
        subtype = ParticleEfficiencyFilter::kEnergy;
      else if (conditionSubtype.EqualTo("ETA"))
        subtype = ParticleEfficiencyFilter::kEta;
      else if (conditionSubtype.EqualTo("RAPIDITY"))
        subtype = ParticleEfficiencyFilter::kRapidity;
      else if (conditionSubtype.EqualTo("PHI"))
        subtype = ParticleEfficiencyFilter::kPhi;
      else
        throw FilterException(conditionSubtype,__FUNCTION__);
      filter->addCondition(subtype,
                           conditionMinima[k],conditionMaxima[k],
                           conditionMinima2[k],conditionMaxima2[k]);
      }
    else if (conditionType.EqualTo("Integer"))
      {
      if (conditionSubtype.EqualTo("PDG"))
        subtype = ParticleEfficiencyFilter::kPdg;
      else if (conditionSubtype.EqualTo("CHARGE"))
        subtype = ParticleEfficiencyFilter::kCharge;
      else if (conditionSubtype.EqualTo("BARYON"))
        subtype = ParticleEfficiencyFilter::kBaryon;
      else if (conditionSubtype.EqualTo("STRANGE"))
        subtype = ParticleEfficiencyFilter::kStrange;
      else if (conditionSubtype.EqualTo("CHARM"))
        subtype = ParticleEfficiencyFilter::kCharm;
      else
        throw FilterException(conditionSubtype,__FUNCTION__);
      filter->addCondition(subtype,int(conditionMinima[k]));
      }
    else if (conditionType.EqualTo("Bool"))
      {
      if (conditionSubtype.EqualTo("LIVE"))
        subtype = ParticleEfficiencyFilter::kLive;
      else if (conditionSubtype.EqualTo("ENABLED"))
        subtype = ParticleEfficiencyFilter::kEnabled;
      else
        throw FilterException(conditionSubtype,__FUNCTION__);
      filter->addCondition(subtype,bool(conditionMinima[k]));
      }
    else
      throw FilterException(conditionType,__FUNCTION__);
    }
  return filter;
}

} // namespace CAP
