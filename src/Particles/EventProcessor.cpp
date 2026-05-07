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
#include "EventProcessor.hpp"

ClassImp(CAP::EventProcessor);

namespace CAP
{
  EventProcessor::EventProcessor()
  :
  Task(),
  _managedEvents(),
  _managedEventFilters(),
  _managedEventEfficiencies(),
  _managedEventSmearers(),
  _managedParticleDbs(),
  _managedParticleFilters(),
  _managedParticleEfficiencies(),
  _managedParticleSmearers(),
  _acceptedEventFilters(),
  _acceptedParticleFilters(),
  _particleMomentsFilter0(),
  _particleMomentsFilter1(),
  _particleMomentsFilter2(),
  _particleMomentsFilter3(),
  _particleMomentsFilter4(),
  _particleMomentsFilter5(),
  _particleMomentsFilter6(),
  _particleMomentsFilter7(),
  _allAcceptedParticlesFilter0(),
  _allAcceptedParticlesFilter1(),
  _allAcceptedParticlesFilter2(),
  _allAcceptedParticlesFilter3(),
  _allAcceptedParticlesFilter4(),
  _allAcceptedParticlesFilter5(),
  _allAcceptedParticlesFilter6(),
  _allAcceptedParticlesFilter7(),
  _ptMomentOrder(2),
  _eventAccepted(),
  _evtType()
  {
  appendClassName("EventProcessor");
  setMinimumReportLevel(Object::kInfo);
  setName("EventProcessor");
  setTitle("EventProcessor");
  }

  EventProcessor::EventProcessor(const EventProcessor & source)
  :
  Task(source),
  _managedEvents(source._managedEvents),
  _managedEventFilters(source._managedEventFilters),
  _managedEventEfficiencies(source._managedEventEfficiencies),
  _managedEventSmearers(source._managedEventSmearers),
  _managedParticleDbs(source._managedParticleDbs),
  _managedParticleFilters(source._managedParticleFilters),
  _managedParticleEfficiencies(source._managedParticleEfficiencies),
  _managedParticleSmearers(source._managedParticleSmearers),
  _acceptedEventFilters(source._acceptedEventFilters),
  _acceptedParticleFilters(source._acceptedParticleFilters),
  _particleMomentsFilter0(source._particleMomentsFilter0),
  _particleMomentsFilter1(source._particleMomentsFilter1),
  _particleMomentsFilter2(source._particleMomentsFilter2),
  _particleMomentsFilter3(source._particleMomentsFilter3),
  _particleMomentsFilter4(source._particleMomentsFilter4),
  _particleMomentsFilter5(source._particleMomentsFilter5),
  _particleMomentsFilter6(source._particleMomentsFilter6),
  _particleMomentsFilter7(source._particleMomentsFilter7),
  _allAcceptedParticlesFilter0(source._allAcceptedParticlesFilter0),
  _allAcceptedParticlesFilter1(source._allAcceptedParticlesFilter1),
  _allAcceptedParticlesFilter2(source._allAcceptedParticlesFilter2),
  _allAcceptedParticlesFilter3(source._allAcceptedParticlesFilter3),
  _allAcceptedParticlesFilter4(source._allAcceptedParticlesFilter4),
  _allAcceptedParticlesFilter5(source._allAcceptedParticlesFilter5),
  _allAcceptedParticlesFilter6(source._allAcceptedParticlesFilter6),
  _allAcceptedParticlesFilter7(source._allAcceptedParticlesFilter7),
  _ptMomentOrder(source._ptMomentOrder),
  _eventAccepted(source._eventAccepted),
  _evtType(source._evtType)
  {   }

  EventProcessor & EventProcessor::operator=(const EventProcessor & rhs)
  {
  if (this!=&rhs)
    {
    Task::operator=(rhs);
    _managedEvents               = rhs._managedEvents;
    _managedEventFilters         = rhs._managedEventFilters;
    _managedEventEfficiencies    = rhs._managedEventEfficiencies;
    _managedEventSmearers        = rhs._managedEventSmearers;
    _managedParticleDbs          = rhs._managedParticleDbs;
    _managedParticleFilters      = rhs._managedParticleFilters;
    _managedParticleEfficiencies = rhs._managedParticleEfficiencies;
    _managedParticleSmearers     = rhs._managedParticleSmearers;
    _acceptedEventFilters        = rhs._acceptedEventFilters;
    _acceptedParticleFilters= rhs._acceptedParticleFilters;
    _particleMomentsFilter0 = rhs._particleMomentsFilter0;
    _particleMomentsFilter1 = rhs._particleMomentsFilter1;
    _particleMomentsFilter2 = rhs._particleMomentsFilter2;
    _particleMomentsFilter3 = rhs._particleMomentsFilter3;
    _particleMomentsFilter4 = rhs._particleMomentsFilter4;
    _particleMomentsFilter5 = rhs._particleMomentsFilter5;
    _particleMomentsFilter6 = rhs._particleMomentsFilter6;
    _particleMomentsFilter7 = rhs._particleMomentsFilter7;
    _allAcceptedParticlesFilter0 = rhs._allAcceptedParticlesFilter0;
    _allAcceptedParticlesFilter1 = rhs._allAcceptedParticlesFilter1;
    _allAcceptedParticlesFilter2 = rhs._allAcceptedParticlesFilter2;
    _allAcceptedParticlesFilter3 = rhs._allAcceptedParticlesFilter3;
    _allAcceptedParticlesFilter4 = rhs._allAcceptedParticlesFilter4;
    _allAcceptedParticlesFilter5 = rhs._allAcceptedParticlesFilter5;
    _allAcceptedParticlesFilter6 = rhs._allAcceptedParticlesFilter6;
    _allAcceptedParticlesFilter7 = rhs._allAcceptedParticlesFilter7;
    _ptMomentOrder = rhs._ptMomentOrder;
    _eventAccepted = rhs._eventAccepted;
    _evtType = rhs._evtType;
    }
  return *this;
  }

  void EventProcessor::setDefaultConfiguration()
  {
  Task::setDefaultConfiguration();
  const String & taskName = Task::name();
  _managedEvents.setDefaultObjectConfiguration(taskName,"EVENT", _configuration);
  _managedEventFilters.setDefaultObjectConfiguration(taskName,"EVENT_FILTER", _configuration);
  _managedEventEfficiencies.setDefaultObjectConfiguration(taskName,"EVENT_EFFICIENCY", _configuration);
  _managedEventSmearers.setDefaultObjectConfiguration(taskName,"EVENT_SMEARER", _configuration);
  _managedParticleDbs.setDefaultObjectConfiguration(taskName,"PARTICLE_DB", _configuration);
  _managedParticleFilters.setDefaultObjectConfiguration(taskName,"PARTICLE_FILTER", _configuration);
  _managedParticleEfficiencies.setDefaultObjectConfiguration(taskName,"PARTICLE_EFFICIENCY", _configuration);
  _managedParticleSmearers.setDefaultObjectConfiguration(taskName,"PARTICLE_SMEARER", _configuration);
  }

  void EventProcessor::configure(const Configuration & configuration)
  {
  if (reportDebug(__FUNCTION__)) { printCR();  }
  Task::configure(configuration);
  _managedEvents.configureObjects(configuration);
  _managedEventFilters.configureObjects(configuration);
  _managedEventEfficiencies.configureObjects(configuration);
  _managedEventSmearers.configureObjects(configuration);
  _managedParticleDbs.configureObjects(configuration);
  _managedParticleFilters.configureObjects(configuration);
  _managedParticleEfficiencies.configureObjects(configuration);
  _managedParticleSmearers.configureObjects(configuration);
  }

  void EventProcessor::initialize()
  {
  if (reportDebug(__FUNCTION__)) { printCR(); }
  _timer.start();
  _taskExecuted.initialize("taskExecuted",1);

  _managedEvents.initialize();
  _managedEventFilters.initialize();
  _managedEventEfficiencies.initialize();
  _managedEventSmearers.initialize();
  _managedParticleDbs.initialize();
  _managedParticleFilters.initialize();
  _managedParticleEfficiencies.initialize();
  _managedParticleSmearers.initialize();

  _eventAccepted.initialize("EventAccepted",nEventFilters());
  _acceptedEventFilters.assign(nEventFilters(),false);
  _acceptedParticleFilters.assign(nParticleFilters(),false);
  _particleMomentsFilter0.assign(_ptMomentOrder,0.0);
  _particleMomentsFilter1.assign(_ptMomentOrder,0.0);
  _particleMomentsFilter2.assign(_ptMomentOrder,0.0);
  _particleMomentsFilter3.assign(_ptMomentOrder,0.0);
  _particleMomentsFilter4.assign(_ptMomentOrder,0.0);
  _particleMomentsFilter5.assign(_ptMomentOrder,0.0);
  _particleMomentsFilter6.assign(_ptMomentOrder,0.0);
  _particleMomentsFilter7.assign(_ptMomentOrder,0.0);

  _allAcceptedParticlesFilter0.reserve(4000);
  _allAcceptedParticlesFilter1.reserve(4000);
  _allAcceptedParticlesFilter2.reserve(4000);
  _allAcceptedParticlesFilter3.reserve(4000);
  _allAcceptedParticlesFilter4.reserve(4000);
  _allAcceptedParticlesFilter5.reserve(4000);
  _allAcceptedParticlesFilter6.reserve(4000);
  _allAcceptedParticlesFilter7.reserve(4000);
  if (hasSubTasks()) initializeSubTasks();
  }

  void EventProcessor::finalize()
  {
  if (reportDebug(__FUNCTION__)) { printCR();}
  if (hasSubTasks()) finalizeSubTasks();
 _managedEventFilters.finalize();
  _managedEventEfficiencies.finalize();
  _managedEventSmearers.finalize();
  _managedParticleDbs.finalize();
  _managedParticleFilters.finalize();
  _managedParticleEfficiencies.finalize();
  _managedParticleSmearers.finalize();
  _taskExecuted.clear();
  _eventAccepted.clear();
  }


  void EventProcessor::reset()
  {
  if (reportDebug(__FUNCTION__)) { printCR();}
  _managedEventFilters.reset();
  _managedEventEfficiencies.reset();
  _managedEventSmearers.reset();
  _managedParticleFilters.reset();
  _managedParticleEfficiencies.reset();
  _managedParticleSmearers.reset();
  }

  void EventProcessor::clear()
  {
  if (reportDebug(__FUNCTION__)) { printCR();}
  _managedEventFilters.clear();
  _managedEventEfficiencies.clear();
  _managedEventSmearers.clear();
  _managedParticleDbs.clear();
  _managedParticleFilters.clear();
  _managedParticleEfficiencies.clear();
  _managedParticleSmearers.clear();
  }

  void EventProcessor::print() const
  {
  if (reportDebug(__FUNCTION__)) { printCR();}
  _managedEventFilters.print();
  _managedEventEfficiencies.print();
  _managedEventSmearers.print();
  _managedParticleDbs.print();
  _managedParticleFilters.print();
  _managedParticleEfficiencies.print();
  _managedParticleSmearers.print();
  }

  const std::vector<Particle*> & EventProcessor::acceptedParticles(unsigned int index) const
  {
  switch (index)
    {
      case 0:  return _allAcceptedParticlesFilter0;
      case 1:  return _allAcceptedParticlesFilter1;
      case 2:  return _allAcceptedParticlesFilter2;
      case 3:  return _allAcceptedParticlesFilter3;
      case 4:  return _allAcceptedParticlesFilter4;
      case 5:  return _allAcceptedParticlesFilter5;
      case 6:  return _allAcceptedParticlesFilter6;
      case 7:  return _allAcceptedParticlesFilter7;
      default:
      throw MemoryException("index>=_allAcceptedParticlesFilter.size()",__FUNCTION__);
    }
  }

  std::vector<Particle*> & EventProcessor::acceptedParticles(unsigned int index)
  {
  switch (index)
    {
      case 0:  return _allAcceptedParticlesFilter0;
      case 1:  return _allAcceptedParticlesFilter1;
      case 2:  return _allAcceptedParticlesFilter2;
      case 3:  return _allAcceptedParticlesFilter3;
      case 4:  return _allAcceptedParticlesFilter4;
      case 5:  return _allAcceptedParticlesFilter5;
      case 6:  return _allAcceptedParticlesFilter6;
      case 7:  return _allAcceptedParticlesFilter7;
      default:
      throw MemoryException("index>=_allAcceptedParticlesFilter.size()",__FUNCTION__);
    }
  }


  std::vector<double> & EventProcessor::moments(unsigned int index)
  {
  switch (index)
    {
      case 0:  return _particleMomentsFilter0;
      case 1:  return _particleMomentsFilter1;
      case 2:  return _particleMomentsFilter2;
      case 3:  return _particleMomentsFilter3;
      case 4:  return _particleMomentsFilter4;
      case 5:  return _particleMomentsFilter5;
      case 6:  return _particleMomentsFilter6;
      case 7:  return _particleMomentsFilter7;
      default:
      throw MemoryException("index>=_particleMomentsFilter.size()",__FUNCTION__);
    }
  }

  double EventProcessor::getEvtClassValue() const
  {
  const Event & ev = event();
  switch (_evtType)
    {
      case EventFilter::kAll:    return ev.mult(0); break;
      case EventFilter::kMult0:  return ev.mult(0); break;
      case EventFilter::kMult1:  return ev.mult(1); break;
      case EventFilter::kMult2:  return ev.mult(2); break;
      case EventFilter::kMult3:  return ev.mult(3); break;
      case EventFilter::kMult4:  return ev.mult(4); break;
      case EventFilter::kMult5:  return ev.mult(5); break;
      case EventFilter::kEnergy0:  return ev.energy(0); break;
      case EventFilter::kEnergy1:  return ev.energy(1); break;
      case EventFilter::kEnergy2:  return ev.energy(2); break;
      case EventFilter::kEnergy3:  return ev.energy(3); break;
      case EventFilter::kEnergy4:  return ev.energy(4); break;
      case EventFilter::kEnergy5:  return ev.energy(5); break;
      case EventFilter::kSphero0:  return ev.spherocity(0); break;
      case EventFilter::kSphero1:  return ev.spherocity(1); break;
      case EventFilter::kSphero2:  return ev.spherocity(2); break;
      case EventFilter::kSphero3:  return ev.spherocity(3); break;
      case EventFilter::kSphero4:  return ev.spherocity(4); break;
      case EventFilter::kSphero5:  return ev.spherocity(5); break;
      default: throw Exception("Unknown evtClassType",__FUNCTION__);
    }
  return 0;
  }

  void EventProcessor::scaleHistograms()
  {
  if (reportDebug(__FUNCTION__)) { printCR();}
  std::vector<double> factors;
  factors.clear();
  double f;
  unsigned int nef = nEventFilters();
  for (unsigned int iEventFilter=0; iEventFilter<nef; iEventFilter++)
    {
    long count = _eventAccepted.valueAt(iEventFilter);
    if (count>0)
      f = 1.0/double(count);
    else
      f = 1.0;
    factors.push_back(f);
    if (reportDebug(__FUNCTION__))
      {
      printCR();
      printValue("iEventFilter",iEventFilter);
      printValue("counts",count);
      printValue("scaling factor",f);
      }
    }
  scaleHistograms(factors);
  }

  void EventProcessor::scaleHistograms(double factor __attribute__((unused)))
  {
  throw NoImplementationException("EventProcessor",__FUNCTION__);
  }

  void EventProcessor::scaleHistograms(const std::vector<double> & factors __attribute__((unused)))
  {
  throw NoImplementationException("EventProcessor",__FUNCTION__);
  }

  void EventProcessor::execute()
  {
  throw NoImplementationException("EventProcessor",__FUNCTION__);
  }

  void EventProcessor::postProcess()
  {
  if (reportDebug(__FUNCTION__))  { printCR(); };
  calculateDerived();
  }

  void EventProcessor::calculateDerived()
  {
  throw NoImplementationException("EventProcessor",__FUNCTION__);
  }

  void EventProcessor::calculateBf()
  {
  throw NoImplementationException("EventProcessor",__FUNCTION__);
  }

  bool EventProcessor::filterEvent(Event & event)
  {
  bool analyzeThisEvent = false;
  std::vector<EventFilter*> & ef = eventFilters();
  for (unsigned int iEventFilter=0; iEventFilter<ef.size();iEventFilter++)
    {
    if (ef[iEventFilter]->accept(event))
      {
      if (iEventFilter>=_acceptedEventFilters.size())
        throw MemoryException("iEventFilter>=_acceptedEventFilters.size()",__FUNCTION__);
      _eventAccepted.increment(iEventFilter);
      _acceptedEventFilters[iEventFilter] = true;
      analyzeThisEvent = true;
      }
    else
      {
      _acceptedEventFilters[iEventFilter] = false;
      }
    }
  return analyzeThisEvent;
  }


  bool EventProcessor::filterParticles(std::vector<Particle*> & particles)
  {
  bool analyzeThisEvent = false;
  bool useThisFilter = false;
  std::vector<ParticleFilter*> & pf = particleFilters();
  for (unsigned int iParticleFilter = 0;iParticleFilter<pf.size();iParticleFilter++)
    {
    useThisFilter = false;
    std::vector<Particle*> & ap =  acceptedParticles(iParticleFilter);
    ap.clear();
    std::vector<double> & mom = moments(iParticleFilter);
    for (auto & v : mom) v=0;
    for (auto  particle : particles)
      {
      if (pf[iParticleFilter]->accept(*particle))
        {
        useThisFilter = true; // at least one particle accepted
        ap.push_back(particle);
        VectorLorentz p = particle->momentum();
        double pt = p.perp();
        double ptFac = 1.0;
        for (unsigned int k=0; k<=_ptMomentOrder; k++)
          {
          mom[k] += ptFac;
          ptFac *= pt;
          }
        }
      }
    _acceptedParticleFilters[iParticleFilter] = useThisFilter;
    if (useThisFilter) analyzeThisEvent = true;
    }
  ///printValue("Analyzer<HISTOGRAM_2,RANK>::filterParticles()",12);
  return analyzeThisEvent;
  }

} // namespace CAP

