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
#include "Event.hpp"
#include "PrintHelpers.hpp"

ClassImp(CAP::Event);
ClassImp(CAP::Manager<CAP::Event>);

namespace CAP
{

  Event::Event()
  :
  ManagedObject(),
  _eventIndex(0),
  _eventNumber(0),
  _projectileA(),
  _projectileB(),
  _particles(),
  _multiplicity(),
  _energy(),
  _charge(),
  _strangeness(),
  _baryon(),
  _ptSum(),
  _spherocity()
  {
  initializeMult();
  initializeEnergy();
  initializeCharge();
  initializeStrangeness();
  initializeBaryon();
  initializePtSum();
  initializeSpherocity();
  }

  Event::~Event()
  {
  _particles.clear();
  }

  //!
  // Call before (re)starting simulation
  //!
  void Event::clear()
  {
  _eventIndex  = 0;
  _eventNumber = 0;
  _particles.clear();
  }

  //!
  // Call before generating new event
  //!
  void Event::reset()
  {
  _eventIndex++;
  _eventNumber  = 0;
  _particles.clear();
  //  resetMult();
  //  resetEnergy();
  //  resetCharge();
  //  resetStrangeness();
  //  resetBaryon();
  //  resetPtSum();
  //  resetpherocity();
  }

  void Event::addParticle(Particle * particle)
  {
  _particles.push_back(particle);
  }

  Particle * Event::particleAt(unsigned long index)
  {
  if (index>=_particles.size() )
    throw Exception("index out of bounds","Event::particleAt(unsigned long index)");
  return _particles[index];
  }

  void Event::printParticles() const
  {
  unsigned int n = _particles.size();
  for (unsigned int k=0; k<n; k++)
    {
    printValue("k",k);
    Particle::factory().objectAt(k).print();
    //particles[k]->print();
    }

  //  for (auto & particle : particles) particle->print();
  }

  void Event::print() const
  {
  printCR();
  printLine();
  printValue("Event Name",name());
  printValue("Event Index",_eventIndex);
  printValue("Event Number",_eventNumber);
  //  printString("Projectile A");
  //  projectileA.print();
  //  printString("Projectile B");
  //  projectileB.print();
  printParticles();
  printLine();
  printCR();
  }

} // namespace CAP
