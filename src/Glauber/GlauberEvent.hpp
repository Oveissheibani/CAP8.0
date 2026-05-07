#ifndef CAP__GlauberEvent
#define CAP__GlauberEvent
#include <TObject.h>
#include "GlauberPoint.hpp"
#include "GlauberNucleus.hpp"
#include "GlauberEventMoments.hpp"
#include "ManagedObject.hpp"
#include "Configuration.hpp"

namespace CAP
{

  class GlauberEvent
  :
  public ManagedObject
  {
  protected:

  double   _impactParameter;
  double   _eventXSection;
  double   _eventPlaneAngle;
  double   _abDistance;
  double   _areaOverlap;
  double   _areaAll;
  double   _length;
  int      _nCollisions;
  int      _nPPCollisions;
  int      _nPNCollisions;
  int      _nNPCollisions;
  int      _nNNCollisions;
  int      _nHard;
  int      _nPPHardCollisions;
  int      _nPNHardCollisions;
  int      _nNPHardCollisions;
  int      _nNNHardCollisions;

  GlauberNucleus _nucleusA;
  GlauberNucleus _nucleusB;
  GlauberEventMoments _momentsNucleusA;
  GlauberEventMoments _momentsNucleusB;
  GlauberEventMoments _momentsSystem;
  std::vector<GlauberPoint> _interactionPositions;

  public:

  GlauberEvent();
  GlauberEvent(const GlauberEvent & source);
  GlauberEvent operator=(const GlauberEvent & source);
  virtual ~GlauberEvent();
  virtual void reset();
  virtual void clear();
  virtual void print() const;

  virtual void incrementColls(GlauberNucleon & nucleonA, GlauberNucleon & nucleonB, bool isHard=0);

  const GlauberNucleus & nucleusA() const { return _nucleusA; }
  const GlauberNucleus & nucleusB() const { return _nucleusB; }
  const std::vector<GlauberNucleon> & allNucleonsA() const { return _nucleusA.allNucleons(); }
  const std::vector<GlauberNucleon> & allNucleonsB() const { return _nucleusB.allNucleons(); }
  const std::vector<GlauberPoint> & interactionPositions() const { return _interactionPositions; }
  const GlauberEventMoments & momentsNucleusA() const { return _momentsNucleusA; }
  const GlauberEventMoments & momentsNucleusB() const { return _momentsNucleusB; }
  const GlauberEventMoments & momentsSystem() const   { return _momentsSystem;   }

  GlauberNucleus & nucleusA() { return _nucleusA; }
  GlauberNucleus & nucleusB() { return _nucleusB; }
  std::vector<GlauberNucleon> & allNucleonsA()   { return _nucleusA.allNucleons(); }
  std::vector<GlauberNucleon> & allNucleonsB()   { return _nucleusB.allNucleons(); }
  std::vector<GlauberPoint> & interactionPositions()  { return _interactionPositions; }
  GlauberEventMoments & momentsNucleusA()         { return _momentsNucleusA; }
  GlauberEventMoments & momentsNucleusB()         { return _momentsNucleusB; }
  GlauberEventMoments & momentsSystem()           { return _momentsSystem;   }

  void setImpactParameter(double v) { _impactParameter = v; }
  void setEventXSection(double v)   { _eventXSection = v; }
  void setAreaOverlap(double v)     { _areaOverlap = v; }
  void setAreaAll(double v)         { _areaAll = v; }

  double   impactParameter() const {  return _impactParameter; };
  double   eventXSection() const   {  return _eventXSection; };
  double   areaOverlap() const     {  return _areaOverlap; }
  double   areaAll() const         {  return _areaAll; }

  int nParticipants() const
  {
  int na = _nucleusA.nWoundedNucleons();
  int nb = _nucleusB.nWoundedNucleons();
  return na+nb;
  }

  int nProtonParticipants() const
  {
  int na = _nucleusA.nWoundedProtons();
  int nb = _nucleusB.nWoundedProtons();
  return na+nb;
  }

  int nNeutronParticipants() const
  {
  int na = _nucleusA.nWoundedNeutrons();
  int nb = _nucleusB.nWoundedNeutrons();
  return na+nb;
  }


  int nParticipantsA() const  {  return _nucleusA.nWoundedNucleons();  }
  int nParticipantsB() const  {  return _nucleusB.nWoundedNucleons();  }
  int nProtonParticipantsA() const   { return _nucleusA.nWoundedProtons();  }
  int nProtonParticipantsB() const   { return _nucleusB.nWoundedProtons();  }
  int nNeutronParticipantsA() const  { return  _nucleusA.nWoundedNeutrons();}
  int nNeutronParticipantsB() const  { return _nucleusB.nWoundedNeutrons(); }

  int nCollisions() const     {  return _nCollisions;  }
  int nPPCollisions() const   {  return _nPPCollisions;  }
  int nPNCollisions() const   {  return _nPNCollisions;  }
  int nNPCollisions() const   {  return _nNPCollisions;  }
  int nNNCollisions() const   {  return _nNNCollisions;  }
  int nCollisionsA() const    {  return _nucleusA.nCollisions();  }
  int nCollisionsB() const    {  return _nucleusB.nCollisions();  }

  int nHardCollisions() const    {  return _nHard; }
  int nPPHardCollisions() const  {  return _nPPHardCollisions; }
  int nPNHardCollisions() const  {  return _nPNHardCollisions; }
  int nNPHardCollisions() const  {  return _nNPHardCollisions; }
  int nNNHardCollisions() const  {  return _nNNHardCollisions; }
  int nHardCollisionsA() const   {  return _nucleusA.nHardCollisions(); }
  int nHardCollisionsB() const   {  return _nucleusB.nHardCollisions(); }

  int nSpectatorsA() const  {  return _nucleusA.nSpectatorNucleons(); }
  int nSpectatorsB() const  {  return _nucleusB.nSpectatorNucleons(); }
  int nProtonSpectatorsA() const  {  return _nucleusA.nSpectatorProtons();  }
  int nProtonSpectatorsB() const  {  return _nucleusB.nSpectatorProtons();  }
  int nNeutronSpectatorsA() const  {  return _nucleusA.nSpectatorNeutrons();  }
  int nNeutronSpectatorsB() const  {  return _nucleusB.nSpectatorNeutrons();  }

  int nSpectators() const
  {
  int na = _nucleusA.nSpectatorNucleons();
  int nb = _nucleusB.nSpectatorNucleons();
  return na+nb;
  }

  int nProtonSpectators() const
  {
  int na = _nucleusA.nSpectatorProtons();
  int nb = _nucleusB.nSpectatorProtons();
  return na+nb;
  }

  int nNeutronSpectators() const
  {
  int na = _nucleusA.nSpectatorNeutrons();
  int nb = _nucleusB.nSpectatorNeutrons();
  return na+nb;
  }



  //  double   calcDensity(TF1 & prof, double xval, double yval) const;
  //  double   calcArea();
  //  double   calc_length();
  //  double   calcEvent(double bgen);
  //  double   calcResult(double bgen);

  ClassDef(GlauberEvent, 1)
  };

} // namespace CAP

#endif



