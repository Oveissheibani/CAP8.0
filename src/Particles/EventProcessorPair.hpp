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
#ifndef CAP__EventProcessorPair
#define CAP__EventProcessorPair
#include "EventProcessor.hpp"

namespace CAP
{

template <class HISTOGRAM_1,class HISTOGRAM_2,class HISTOGRAM_3,class HISTOGRAM_4>
class EventProcessorPair
:
public EventProcessor
{
  protected :
  ManagedObjects<HISTOGRAM_1>  _managedHistogram_1s;
  ManagedObjects<HISTOGRAM_2>  _managedHistogram_2s;
  ManagedObjects<HISTOGRAM_3>  _managedHistogram_3s;
  ManagedObjects<HISTOGRAM_4>  _managedHistogram_4s;

public:

  EventProcessorPair()
  :
  EventProcessor(),
  _managedHistogram_1s(),
  _managedHistogram_2s(),
  _managedHistogram_3s(),
  _managedHistogram_4s()
  {
  EventProcessor::appendClassName("EventProcessorPair");
  EventProcessor::setMinimumReportLevel(Object::kInfo);
  EventProcessor::setName("EventProcessorPair");
  EventProcessor::setTitle("EventProcessorPair");
  }

  EventProcessorPair(const EventProcessorPair & source)
  :
  EventProcessor(source),
  _managedHistogram_1s(source._managedHistogram_1s),
  _managedHistogram_2s(source._managedHistogram_2s),
  _managedHistogram_3s(source._managedHistogram_3s),
  _managedHistogram_4s(source._managedHistogram_4s)
  {  }

  EventProcessorPair & operator=(const EventProcessorPair & rhs)
  {
  if (this!=&rhs)
    {
    EventProcessor::operator=(rhs);
    _managedHistogram_1s = rhs._managedHistogram_1s;
    _managedHistogram_2s = rhs._managedHistogram_2s;
    _managedHistogram_3s = rhs._managedHistogram_3s;
    _managedHistogram_4s = rhs._managedHistogram_4s;
    }
  return *this;
  }

  virtual ~EventProcessorPair() {}

  virtual void setDefaultConfiguration()
  {
  EventProcessor::setDefaultConfiguration();
  const String & taskName = EventProcessor::name();
  Configuration & config = EventProcessor::configuration();
  _managedHistogram_1s.setDefaultObjectConfiguration(taskName,"HISTOGRAM_1",config);
  _managedHistogram_2s.setDefaultObjectConfiguration(taskName,"HISTOGRAM_2",config);
  _managedHistogram_3s.setDefaultObjectConfiguration(taskName,"HISTOGRAM_3",config);
  _managedHistogram_4s.setDefaultObjectConfiguration(taskName,"HISTOGRAM_4",config);
  }

  unsigned int nhistograms_1() const { return _managedHistogram_1s.size();  }
  unsigned int nhistograms_2() const { return _managedHistogram_2s.size();  }
  unsigned int nhistograms_3() const { return _managedHistogram_3s.size();  }
  unsigned int nhistograms_4() const { return _managedHistogram_4s.size();  }
  const std::vector<HISTOGRAM_1*> & histograms_1() const { return _managedHistogram_1s.getObjects(); }
  const std::vector<HISTOGRAM_2*> & histograms_2() const { return _managedHistogram_2s.getObjects(); }
  const std::vector<HISTOGRAM_3*> & histograms_3() const { return _managedHistogram_3s.getObjects(); }
  const std::vector<HISTOGRAM_4*> & histograms_4() const { return _managedHistogram_4s.getObjects(); }
  std::vector<HISTOGRAM_1*>      & histograms_1()       { return _managedHistogram_1s.getObjects(); }
  std::vector<HISTOGRAM_2*>      & histograms_2()       { return _managedHistogram_2s.getObjects(); }
  std::vector<HISTOGRAM_3*>      & histograms_3()       { return _managedHistogram_3s.getObjects(); }
  std::vector<HISTOGRAM_4*>      & histograms_4()       { return _managedHistogram_4s.getObjects(); }


  virtual void configure(const Configuration & configuration)
  {
  if (EventProcessor::reportDebug(__FUNCTION__)) { printCR();  }
  EventProcessor::configure(configuration);
  _managedHistogram_1s.configureObjects(configuration);
  _managedHistogram_2s.configureObjects(configuration);
  _managedHistogram_3s.configureObjects(configuration);
  _managedHistogram_4s.configureObjects(configuration);
  }

  virtual void initialize()
  {
  if (EventProcessor::reportDebug(__FUNCTION__)) { printCR();}
  EventProcessor::initialize();
  _managedHistogram_1s.initialize();
  _managedHistogram_2s.initialize();
  _managedHistogram_3s.initialize();
  _managedHistogram_4s.initialize();
  }


  virtual void finalize()
  {
  if (EventProcessor::reportDebug(__FUNCTION__)) { printCR();}
  EventProcessor::finalize();
  _managedHistogram_1s.finalize();
  _managedHistogram_2s.finalize();
  _managedHistogram_3s.finalize();
  _managedHistogram_4s.finalize();
  }

  virtual void reset()
  {
  if (EventProcessor::reportDebug(__FUNCTION__)) { printCR();}
  EventProcessor::reset();
  _managedHistogram_1s.reset();
  _managedHistogram_2s.reset();
  _managedHistogram_3s.reset();
  _managedHistogram_4s.reset();
  }

  virtual void clear()
  {
  if (EventProcessor::reportDebug(__FUNCTION__)) { printCR();}
  EventProcessor::clear();
  _managedHistogram_1s.clear();
  _managedHistogram_2s.clear();
  _managedHistogram_3s.clear();
  _managedHistogram_4s.clear();
  }

  virtual void print() const
  {
  EventProcessor::print();
  _managedHistogram_1s.print();
  _managedHistogram_2s.print();
  _managedHistogram_3s.print();
  _managedHistogram_4s.print();
  }

  virtual void calculateDerived()
  {
  if (EventProcessor::reportDebug(__FUNCTION__))  { printCR(); };
  std::vector<HISTOGRAM_1*> & basePairHistos      = histograms_1();
  std::vector<HISTOGRAM_2*> & derivedPairHistos   = histograms_2();
  std::vector<HISTOGRAM_4*> & derivedSingleHistos = histograms_4();
  unsigned int nEF = nEventFilters();
  unsigned int nPF = nParticleFilters();
  unsigned int baseSingle;
  unsigned int basePairs;
  unsigned int indexSingle1;
  unsigned int indexSingle2;
  unsigned int indexPairs;

  printValue("basePairHistos.size()",basePairHistos.size());
  printValue("derivedPairHistos.size()",derivedPairHistos.size());
  printValue("derivedSingleHistos.size()",derivedSingleHistos.size());
  printValue("nEventFilters",nEF);
  printValue("nParticleFilters",nPF);

  for (unsigned int iEventFilter=0; iEventFilter<nEF; iEventFilter++)
    {
    baseSingle = iEventFilter*nPF;
    basePairs  = iEventFilter*nPF*nPF;
    printValue("baseSingle",baseSingle);
    printValue("basePairs",basePairs);

    for (unsigned int iParticleFilter1=0; iParticleFilter1<nPF; iParticleFilter1++)
      {
      indexSingle1 = baseSingle + iParticleFilter1;
      printValue("indexSingle1",indexSingle1);
      for (unsigned int iParticleFilter2=0; iParticleFilter2<nPF; iParticleFilter2++)
        {
        indexSingle2 = baseSingle + iParticleFilter2;
        indexPairs   = basePairs  + iParticleFilter1*nPF + iParticleFilter2;
        printValue("indexSingle2",indexSingle2);
        printValue("indexPairs",indexPairs);
       derivedPairHistos[indexPairs]->calculateDerivedHistograms(*(derivedSingleHistos[indexSingle1]),
                                                                  *(derivedSingleHistos[indexSingle2]),
                                                                  *(basePairHistos[indexPairs]));
        }
      }
    }
  }

  void scaleHistograms(const std::vector<double> & factors)
  {
  if (reportDebug(__FUNCTION__))
    {
    printCR();
    printValue("factors.size()", factors.size());
    printValue("nEventFilters()", _managedEventFilters.size());
    printValue("nParticleFilters()", _managedParticleFilters.size());
    }
  std::vector<HISTOGRAM_1*> histos_1 = histograms_1();
  std::vector<HISTOGRAM_2*> histos_2 = histograms_2();
  unsigned int baseSingleIndex = 0;
  for (unsigned int iEventFilter=0; iEventFilter<_managedEventFilters.size(); iEventFilter++)
    {
    baseSingleIndex = iEventFilter*nParticleFilters();
    for (unsigned int iParticleFilter=0; iParticleFilter<nParticleFilters(); iParticleFilter++)
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



  //ClassDef(EventProcessorPair,0)
};

} // namespace CAP


#endif /* CAP__EventProcessorPair */
