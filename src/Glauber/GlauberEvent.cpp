#include "GlauberEvent.hpp"
#include "NameHelpers.hpp"
#include "PrintHelpers.hpp"
ClassImp(CAP::Manager<CAP::GlauberEvent>);
ClassImp(CAP::GlauberEvent)

namespace CAP
{

  using CAP::MATH::pi;
  using CAP::MATH::twoPi;


  GlauberEvent::GlauberEvent()
  :
  ManagedObject(),
  _impactParameter(0),
  _eventPlaneAngle(0),
  _abDistance(0),
  _areaAll(0),
  _length(0),
  _nCollisions(0),
  _nPPCollisions(0),
  _nPNCollisions(0),
  _nNPCollisions(0),
  _nNNCollisions(0),

  _nHard(0),
  _nPPHardCollisions(0),
  _nPNHardCollisions(0),
  _nNPHardCollisions(0),
  _nNNHardCollisions(0),

  _nucleusA(),
  _nucleusB(),
  _momentsNucleusA(),
  _momentsNucleusB(),
  _momentsSystem(),
  _interactionPositions()
  {    }

  GlauberEvent::GlauberEvent(const GlauberEvent & source)
  :
  ManagedObject(source),
  _impactParameter(source._impactParameter),
  _eventPlaneAngle(source._eventPlaneAngle),
  _abDistance(source._abDistance),
  _areaAll(source._areaAll),
  _length(source._length),
  _nCollisions(source._nCollisions),
  _nPPCollisions(source._nPPCollisions),
  _nPNCollisions(source._nPNCollisions),
  _nNPCollisions(source._nNPCollisions),
  _nNNCollisions(source._nNNCollisions),

  _nHard(source._nHard),
  _nPPHardCollisions(source._nPPHardCollisions),
  _nPNHardCollisions(source._nPNHardCollisions),
  _nNPHardCollisions(source._nNPHardCollisions),
  _nNNHardCollisions(source._nNNHardCollisions),

  _nucleusA(source._nucleusA),
  _nucleusB(source._nucleusB),
  _momentsNucleusA(source._momentsNucleusA),
  _momentsNucleusB(source._momentsNucleusB),
  _momentsSystem(source._momentsSystem),
  _interactionPositions(source._interactionPositions)
  {    }

  GlauberEvent GlauberEvent::operator=(const GlauberEvent & rhs)
  {
  if (this!=&rhs)
    {
    ManagedObject::operator=(rhs),
    _impactParameter  = rhs._impactParameter;
    _eventPlaneAngle  = rhs._eventPlaneAngle;
    _abDistance       = rhs._abDistance;
    _areaAll          = rhs._areaAll;
    _length           = rhs._length;
    _nCollisions      = rhs._nCollisions;
    _nPPCollisions    = rhs._nPPCollisions;
    _nPNCollisions    = rhs._nPNCollisions;
    _nNPCollisions    = rhs._nNPCollisions;
    _nNNCollisions    = rhs._nNNCollisions;

    _nHard            = rhs._nHard;
    _nPPHardCollisions= rhs._nPPHardCollisions;
    _nPNHardCollisions= rhs._nPNHardCollisions;
    _nNPHardCollisions= rhs._nNPHardCollisions;
    _nNNHardCollisions= rhs._nNNHardCollisions;
    _nucleusA         = rhs._nucleusA;
    _nucleusB         = rhs._nucleusB;
    _momentsNucleusA  = rhs._momentsNucleusA;
    _momentsNucleusB  = rhs._momentsNucleusB;
    _momentsSystem    = rhs._momentsSystem;
    _interactionPositions = rhs._interactionPositions;
    }
  return *this;
  }


  GlauberEvent::~GlauberEvent()
  {
  clear();
  }

  void GlauberEvent::reset()
  {
  _impactParameter  = 0;
  _eventPlaneAngle  = 0;
  _abDistance       = 0;
  _areaAll          = 0;
  _length           = 0;
  _nCollisions      = 0;
  _nPPCollisions    = 0;
  _nPNCollisions    = 0;
  _nNPCollisions    = 0;
  _nNNCollisions    = 0;

  _nHard            = 0;
  _nPPHardCollisions= 0;
  _nPNHardCollisions= 0;
  _nNPHardCollisions= 0;
  _nNNHardCollisions= 0;

 // resetNucleons()
  _nucleusA.reset();
  _nucleusB.reset();
  _momentsNucleusA.reset();
  _momentsNucleusB.reset();
  _momentsSystem.reset();
  _interactionPositions.clear();
  }

  void GlauberEvent::clear()
  {
  _nucleusA.clear();
  _nucleusB.clear();
  _momentsNucleusA.clear();
  _momentsNucleusB.clear();
  _momentsSystem.clear();
  _interactionPositions.clear();
  }

  void GlauberEvent::incrementColls(GlauberNucleon & nucleonA, GlauberNucleon & nucleonB, bool isHard)
  {
  nucleonA.collide();
  nucleonB.collide();
  if (isHard)
    {
    nucleonA.collideHard();
    nucleonB.collideHard();
    }
    _nCollisions++;
  if (isHard) _nHard++;

  if (nucleonA.isProton())
    {
    if (nucleonB.isProton())
      {
      _nPPCollisions++;
      if (isHard) _nPPHardCollisions++;
      }
    else
      {
      _nPNCollisions++;
      if (isHard) _nPNHardCollisions++;
      }
    }
  else
    {
    if (nucleonB.isProton())
      {
      _nNPCollisions++;
      if (isHard) _nNPHardCollisions++;
      }
    else
      {
      _nNNCollisions++;
      if (isHard) _nNNHardCollisions++;
      }
    }
  }

 

  void GlauberEvent::print() const
  {
  printCR();
  printLine();
  printLine();
  _nucleusA.print();
  _nucleusB.print();
  printLine();
  printString("Global Conditions");
  printValue("impactParameter",   impactParameter());
  printValue("NParticipants",     nParticipants());
  printValue("NParticipantsA",    nParticipantsA());
  printValue("NParticipantsB",    nParticipantsB());
  printValue("nCollisions", nCollisions());
  printValue("nCollisionsA",nCollisionsA());
  printValue("nCollisionsB",nCollisionsA());
  printValue("NSpectators",       nSpectators());
  printValue("NSpectatorsA",      nSpectatorsA());
  printValue("NSpectatorsB",      nSpectatorsB());
  printValue("nPPCollisions",     nPPCollisions());
  printValue("nPNCollisions",     nPNCollisions());
  printValue("nNPCollisions",     nNPCollisions());
  printValue("nNNCollisions",     nNPCollisions());
  printValue("nHardCollisions",   nHardCollisions());
  printLine();
  printString("Moments Nucleus A");
  _momentsNucleusA.print();
  printLine();
  printString("Moments Nucleus B");
  _momentsNucleusB.print();
  printLine();
  printString("Moments System");
  _momentsSystem.print();
  printLine();
  }
} // namespace CAP
