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
#ifndef CAP__EventProcessor
#define CAP__EventProcessor
#include "TObject.h"
#include "Task.hpp"
#include "Exceptions.hpp"
#include "ManagedObjects.hpp"
#include "Event.hpp"
#include "EventFilter.hpp"
#include "EventEfficiency.hpp"
#include "EventSmearer.hpp"
#include "ParticleDb.hpp"
#include "Particle.hpp"
#include "ParticleFilter.hpp"
#include "ParticleEfficiency.hpp"
#include "ParticleSmearer.hpp"

namespace CAP
{

  class EventProcessor
  :
  public Task
  {
  protected:

  ManagedObjects<Event>              _managedEvents;
  ManagedObjects<EventFilter>        _managedEventFilters;
  ManagedObjects<EventEfficiency>    _managedEventEfficiencies;
  ManagedObjects<EventSmearer>       _managedEventSmearers;
  ManagedObjects<ParticleDb>         _managedParticleDbs;
  ManagedObjects<ParticleFilter>     _managedParticleFilters;
  ManagedObjects<ParticleEfficiency> _managedParticleEfficiencies;
  ManagedObjects<ParticleSmearer>    _managedParticleSmearers;

  std::vector<bool>   _acceptedEventFilters;
  std::vector<bool>   _acceptedParticleFilters;
  std::vector<double> _particleMomentsFilter0;
  std::vector<double> _particleMomentsFilter1;
  std::vector<double> _particleMomentsFilter2;
  std::vector<double> _particleMomentsFilter3;
  std::vector<double> _particleMomentsFilter4;
  std::vector<double> _particleMomentsFilter5;
  std::vector<double> _particleMomentsFilter6;
  std::vector<double> _particleMomentsFilter7;
  std::vector<Particle*> _allAcceptedParticlesFilter0;
  std::vector<Particle*> _allAcceptedParticlesFilter1;
  std::vector<Particle*> _allAcceptedParticlesFilter2;
  std::vector<Particle*> _allAcceptedParticlesFilter3;
  std::vector<Particle*> _allAcceptedParticlesFilter4;
  std::vector<Particle*> _allAcceptedParticlesFilter5;
  std::vector<Particle*> _allAcceptedParticlesFilter6;
  std::vector<Particle*> _allAcceptedParticlesFilter7;

  unsigned int _ptMomentOrder;
  Accountant _eventAccepted;
  int _evtType;


  public:

  EventProcessor();
  EventProcessor(const EventProcessor & source);
  EventProcessor & operator=(const EventProcessor & rhs);
  virtual ~EventProcessor() {}
  virtual void setDefaultConfiguration();
  virtual void configure(const Configuration & configuration);
  virtual void initialize();
  virtual void execute();
  virtual void finalize();
  virtual void reset();
  virtual void clear();
  virtual void print() const;
  unsigned int nEvents() const  { return _managedEvents.size();  }
  unsigned int nEventFilters() const  { return _managedEventFilters.size();  }
  unsigned int nEventEfficiencies() const  { return _managedEventEfficiencies.size();  }
  unsigned int nEventSmearers() const  { return _managedEventSmearers.size();  }
  unsigned int nParticleDbs() const  { return _managedParticleDbs.size();  }
  unsigned int nParticleFilters() const  { return _managedParticleFilters.size();  }
  unsigned int nParticleEfficiencies() const  { return _managedParticleEfficiencies.size();  }
  unsigned int nParticlesmearers() const  { return _managedParticleSmearers.size();  }
  const ParticleDb & db() const                                         { return *_managedParticleDbs.objectAt(0); }
  const Event & event() const                                           { return *_managedEvents.objectAt(0); }
  const std::vector<Event*>             & events() const               { return _managedEvents.getObjects(); }
  const std::vector<EventFilter*>       & eventFilters() const         { return _managedEventFilters.getObjects(); }
  const std::vector<EventEfficiency*>   & eventEfficiencies() const    { return _managedEventEfficiencies.getObjects(); }
  const std::vector<EventSmearer*>      & eventSmearers() const        { return _managedEventSmearers.getObjects(); }
  const std::vector<ParticleDb*>        & particleDbs() const          { return _managedParticleDbs.getObjects(); }
  const std::vector<ParticleFilter*>    & particleFilters() const      { return _managedParticleFilters.getObjects(); }
  const std::vector<ParticleEfficiency*> & particleEfficiencies() const { return _managedParticleEfficiencies.getObjects(); }
  const std::vector<ParticleSmearer*>   & particleSmearers() const     { return _managedParticleSmearers.getObjects(); }

  ParticleDb & db()                                                     { return *_managedParticleDbs.objectAt(0); }
  Event & event()                                                       { return *_managedEvents.objectAt(0); }
  std::vector<Event*> & events()                                        { return _managedEvents.getObjects(); }
  std::vector<EventFilter*>       & eventFilters()                     { return _managedEventFilters.getObjects(); }
  std::vector<EventEfficiency*>   & eventEfficiencies()                { return _managedEventEfficiencies.getObjects(); }
  std::vector<EventSmearer*>      & eventSmearers()                    { return _managedEventSmearers.getObjects(); }
  std::vector<ParticleDb*>        & particleDbs()                      { return _managedParticleDbs.getObjects(); }
  std::vector<ParticleFilter*>    & particleFilters()                  { return _managedParticleFilters.getObjects(); }
  std::vector<ParticleEfficiency*> & particleEfficiencies()             { return _managedParticleEfficiencies.getObjects(); }
  std::vector<ParticleSmearer*>   & particleSmearers()                 { return _managedParticleSmearers.getObjects(); }

  void setPtOrder(unsigned int order) { _ptMomentOrder = order; }
  unsigned int ptOrder() const { return _ptMomentOrder; }
  Accountant & eventAccepted() { return _eventAccepted; }
  const Accountant & eventAccepted() const { return _eventAccepted; }
  const std::vector<bool> & acceptedEventFilters() const { return _acceptedEventFilters; }
  const std::vector<bool> & acceptedParticleFilters() const { return _acceptedParticleFilters;}
  const std::vector<Particle*> & acceptedParticles(unsigned int index) const;
  std::vector<Particle*> & acceptedParticles(unsigned int index);
  std::vector<double> & moments(unsigned int index);
  double getEvtClassValue() const;
  virtual void scaleHistograms();
  virtual void scaleHistograms(double factor);
  virtual void scaleHistograms(const std::vector<double> & factors);

  virtual void postProcess();
  virtual void calculateDerived();
  virtual void calculateBf();
  virtual bool filterEvent(Event & event);
  virtual bool filterParticles(std::vector<Particle*> & particles);

  ClassDef(EventProcessor,0)
  };

} // namespace CAP

#endif /* CAP__EventProcessor */
