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
#ifndef CAP__EventProcessorSingle
#define CAP__EventProcessorSingle
#include "EventProcessor.hpp"

namespace CAP
{
  template <class HISTOGRAM_1,class HISTOGRAM_2>
  class EventProcessorSingle
  :
  public EventProcessor
  {
  protected:
  ManagedObjects<HISTOGRAM_1>        _managedHistograms_1s;
  ManagedObjects<HISTOGRAM_2>        _managedHistograms_2s;

  public:

  EventProcessorSingle()
  :
  EventProcessor(),
  _managedHistograms_1s(),
  _managedHistograms_2s()
  {
  appendClassName("EventProcessorSingle");
  setMinimumReportLevel(Object::kInfo);
  setName("EventProcessorSingle");
  setTitle("EventProcessorSingle");
  }

  EventProcessorSingle(const EventProcessorSingle & source)
  :
  EventProcessor(source),
  _managedHistograms_1s(source._managedHistograms_1s),
  _managedHistograms_2s(source._managedHistograms_2s)
  {   }

  EventProcessorSingle & operator=(const EventProcessorSingle & rhs)
  {
  if (this!=&rhs)
    {
    EventProcessor::operator=(rhs);
    _managedHistograms_1s        = rhs._managedHistograms_1s;
    _managedHistograms_2s        = rhs._managedHistograms_2s;
    }
  return *this;
  }

  virtual ~EventProcessorSingle() {}

  virtual void setDefaultConfiguration()
  {
  EventProcessor::setDefaultConfiguration();
  const String & taskName = Task::name();
  _managedHistograms_1s.setDefaultObjectConfiguration(taskName,"HISTOGRAM_1",configuration());
  _managedHistograms_2s.setDefaultObjectConfiguration(taskName,"HISTOGRAM_2",configuration());
  }

  virtual void configure(const Configuration & configuration)
  {
  if (reportDebug(__FUNCTION__)) { printCR();  }
  EventProcessor::configure(configuration);
  _managedHistograms_1s.configureObjects(configuration);
  _managedHistograms_2s.configureObjects(configuration);
  }


  virtual void initialize()
  {
  if (reportDebug(__FUNCTION__)) { printCR();}
  EventProcessor::initialize();
  _managedHistograms_1s.initialize();
  _managedHistograms_2s.initialize();
  }

  virtual void finalize()
  {
  if (reportDebug(__FUNCTION__)) { printCR();}
  EventProcessor::finalize();
  _managedHistograms_1s.finalize();
  _managedHistograms_2s.finalize();
  }

  virtual void reset()
  {
  if (reportDebug(__FUNCTION__)) { printCR();}
  EventProcessor::reset();
  _managedHistograms_1s.reset();
  _managedHistograms_2s.reset();
  }

  virtual void clear()
  {
  if (reportDebug(__FUNCTION__)) { printCR();}
  EventProcessor::clear();
  _managedHistograms_1s.clear();
  _managedHistograms_2s.clear();
  }

  virtual void print() const
  {
  if (reportDebug(__FUNCTION__)) { printCR();}
  EventProcessor::print();
  _managedHistograms_1s.print();
  _managedHistograms_2s.print();
  }

  unsigned int nHistograms_1() const  { return _managedHistograms_1s.size();  }
  unsigned int nHistograms_2() const  { return _managedHistograms_2s.size();  }

  const std::vector<HISTOGRAM_1*> & histograms_1() const         { return _managedHistograms_1s.getObjects(); }
  const std::vector<HISTOGRAM_2*> & histograms_2() const         { return _managedHistograms_2s.getObjects(); }
  std::vector<HISTOGRAM_1*> & histograms_1()                     { return _managedHistograms_1s.getObjects(); }
  std::vector<HISTOGRAM_2*> & histograms_2()                     { return _managedHistograms_2s.getObjects(); }

  void scaleHistograms(const std::vector<double> & factors)
  {
  if (reportDebug(__FUNCTION__))
    {
    printCR();
    printValue("factors.size()", factors.size());
    printValue("nEventFilters()",    _managedEventFilters.size());
    printValue("nParticleFilters()", _managedParticleFilters.size());
    }
  std::vector<HISTOGRAM_1*> histos_1 = histograms_1();
  //std::vector<HISTOGRAM_2*> histos_2 = histograms_2();
  unsigned int baseSingleIndex = 0;
  for (unsigned int iEventFilter=0; iEventFilter<_managedEventFilters.size(); iEventFilter++)
    {
    baseSingleIndex = iEventFilter*nParticleFilters();
    for (unsigned int iParticleFilter=0; iParticleFilter<_managedParticleFilters.size(); iParticleFilter++)
      {
      histos_1[baseSingleIndex + iParticleFilter]->scaleHistograms(factors[iEventFilter]);
      histos_1[baseSingleIndex + iParticleFilter]->scaleHistograms(factors[iEventFilter]);
      }
    }
  if (reportDebug(__FUNCTION__))
    {
    printCR();
    printString("DONE!!!!! ");
    }
  }

  virtual void calculateDerived()
  {
  if (EventProcessor::reportDebug(__FUNCTION__))  { printCR(); };
  std::vector<HISTOGRAM_1*> & baseSingleHistos      = histograms_1();
  std::vector<HISTOGRAM_2*> & derivedSingleHistos   = histograms_2();

  unsigned int nEF = nEventFilters();
  unsigned int nPF = nParticleFilters();
  unsigned int baseSingle;
  unsigned int indexSingle;

  printValue("baseSingleHistos.size()",baseSingleHistos.size());
  printValue("derivedSingleHistos.size()",derivedSingleHistos.size());
  printValue("nEventFilters",nEF);
  printValue("nParticleFilters",nPF);

  for (unsigned int iEventFilter=0; iEventFilter<nEF; iEventFilter++)
    {
    baseSingle = iEventFilter*nPF;
    printValue("baseSingle",baseSingle);

    for (unsigned int iParticleFilter=0; iParticleFilter<nPF; iParticleFilter++)
      {
      indexSingle = baseSingle + iParticleFilter;
      printValue("indexSingle",indexSingle);
      derivedSingleHistos[indexSingle]->calculateDerivedHistograms(*(baseSingleHistos[indexSingle]));
      }
    }
  }

  ClassDef(EventProcessorSingle,0)
  };

} // namespace CAP

#endif /* CAP__EventProcessorSingle */
