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
#ifndef CAP__Event
#define CAP__Event
#include "Aliases.hpp"
#include "Exceptions.hpp"
#include "Particle.hpp"
#include "ManagedObject.hpp"
#include "Configuration.hpp"

namespace CAP
{

class Event;

//template <class Event> class Manager;

//!
//! Class encupsalting all components of events (real data or monte carlo)
//!
class Event
:
public ManagedObject
{
  public:
  Event();
  virtual ~Event();
  virtual void clear();
  virtual void reset();
  void addParticle(Particle * particle);
  std::vector<Particle*> & particles() {  return _particles;}
  const std::vector<Particle*> & particles() const {  return _particles;}

  //!
  //! Return to the particle at position "index" in this event. A null pointer is returned is index exceeds the
  //!  number of particles in this event.
  //!
  Particle * particleAt(unsigned long index);

  void printParticles() const;
  void print() const;


  //!
  //! Return the index of this event. The event index might correspond to the position of the event
  //! in the production or input stream.
  //!
  unsigned long eventIndex() const      { return _eventIndex;  }

   //!
  //! Return the event number of this event. The event number might correspond to a tag or index given
  //! to this event externally by some other  production or input stream.
  //!
  unsigned long eventNumber() const     { return _eventNumber; }

  //!
  //! Return the number of particles stored in this event.
  //!
  unsigned long  nParticles() const { return _particles.size();}
  unsigned long  size() const { return _particles.size();}

  Particle & projectileA() { return _projectileA; }
  Particle & projectileB() { return _projectileB; }


  //!
  //! Increment the index of this event.
  //!
  void incrementEventIndex()      { _eventIndex++;  }

  //!
  //! Set the index of this event.
  //!
  void setEventIndex(unsigned long index)  { _eventIndex  = index;}

  void initializeMult(unsigned int n=5)    {  _multiplicity.assign(n,0.0); }
  void initializeEnergy(unsigned int n=5)  {  _energy.assign(n,0.0); }
  void initializeCharge(unsigned int n=5)  {  _charge.assign(n,0.0); }
  void initializeStrangeness(unsigned int n=5)  {  _strangeness.assign(n,0.0); }
  void initializeBaryon(unsigned int n=5)       {  _baryon.assign(n,0.0); }
  void initializePtSum(unsigned int n=5)        {  _ptSum.assign(n,0.0); }
  void initializeSpherocity(unsigned int n=5)   {  _spherocity.assign(n,0.0); }

  double mult(unsigned int index)   const { return _multiplicity[index]; }
  double energy(unsigned int index) const { return _energy[index]; }
  double charge(unsigned int index) const { return _charge[index]; }
  double strangeness(unsigned int index) const { return _strangeness[index]; }
  double baryon(unsigned int index)      const { return _baryon[index]; }
  double ptSum(unsigned int index)       const { return _ptSum[index]; }
  double spherocity(unsigned int index)  const { return _spherocity[index]; }

  void setMult(unsigned int index, double value)        { _multiplicity[index] = value; }
  void setEnergy(unsigned int index, double value)      { _energy[index] = value; }
  void setCharge(unsigned int index, double value)      { _charge[index] = value; }
  void setStrangeness(unsigned int index, double value) { _strangeness[index] = value; }
  void setBaryon(unsigned int index, double value)      { _baryon[index] = value; }
  void setPtSum(unsigned int index, double value)       { _ptSum[index] = value; }
  void setSpherocity(unsigned int index, double value)  { _spherocity[index] = value; }

  void resetMult()         {  for (auto& v :_multiplicity) v=0.0; }
  void resetEnergy()       {  for (auto& v :_energy) v=0.0; }
  void resetCharge()       {  for (auto& v :_charge) v=0.0; }
  void resetStrangeness()  {  for (auto& v :_strangeness) v=0.0; }
  void resetBaryon()       {  for (auto& v :_baryon) v=0.0; }
  void resetPtSum()        {  for (auto& v :_ptSum)  v=0.0; }
  void resetpherocity()    {  for (auto& v :_spherocity) v=0.0; }

protected:

  unsigned long _eventIndex;
  unsigned long _eventNumber;
  Particle _projectileA;
  Particle _projectileB;
  std::vector<Particle*> _particles;

  std::vector<double> _multiplicity;
  std::vector<double> _energy;
  std::vector<double> _charge;
  std::vector<double> _strangeness;
  std::vector<double> _baryon;
  std::vector<double> _ptSum;
  std::vector<double> _spherocity;

  ClassDef(Event,0)
};

} //namespace CAP

#endif /* Event */
