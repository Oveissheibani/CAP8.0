#ifndef CAP__GlauberNucleus
#define CAP__GlauberNucleus
#include "GlauberExceptions.hpp"
#include "GlauberNucleon.hpp"
#include "GlauberNucleusType.hpp"
#include "NuclearDistribution.hpp"

namespace CAP
{
  class GlauberNucleus
  {
  public:
  
  GlauberNucleus();
  GlauberNucleus(const GlauberNucleus & nucleus);
  GlauberNucleus operator=(const GlauberNucleus & nucleus);
  virtual ~GlauberNucleus();
  
  //!
  //!Returns the type of this nucleus
  //!
  const GlauberNucleusType & type() const
  {
  return _nucleusType;
  }
  
  //!
  //!Returns the type of this nucleus
  //!
  GlauberNucleusType & type()
  {
  return _nucleusType;
  }
  
  //!
  //!Returns a constant vector of all the nucleons of this nucleus
  //!
  const vector<GlauberNucleon> & allNucleons() const
  {
  return _nucleons;
  }
  
  //!
  //!Returns a mutable vector of all the nucleons of this nucleus
  //!
  vector<GlauberNucleon> & allNucleons()
  {
  return _nucleons;
  }
  
  String name() const      { return _nucleusType.name(); }
  int nProtons() const     { return _nucleusType.nProtons(); }
  int nNeutrons() const    { return _nucleusType.nNeutrons(); }
  int nNucleons() const    { return _nucleusType.nNucleons(); }
  int nWoundedNucleons() const;
  int nWoundedProtons() const;
  int nWoundedNeutrons() const;
  int nWoundedHardNucleons() const;
  int nWoundedHardProtons() const;
  int nWoundedHardNeutrons() const;

  int nCollisions() const;
  int nPCollisions() const;
  int nNCollisions() const;
  int nHardCollisions() const;
  int nPHardCollisions() const;
  int nNHardCollisions() const;

  int nSpectatorNucleons() const;
  int nSpectatorProtons() const;
  int nSpectatorNeutrons() const;

  bool   allowOverlap() const    { return _allowOverlap;}
  bool   useLattice() const      { return _useLattice;}
  int    latticeType() const     { return _latticeType;}
  int    nTrialsMax() const      { return _nTrialsMax;}
  double nnDistanceMin() const   { return _nnDistanceMin;}
  double nnDistanceMinSq() const { return _nnDistanceMinSq;}
  double nodeDistance() const    { return _nodeDistance;}
  int    recenteringMode() const { return _recenteringMode;}
  int    overlapMode() const     { return _overlapMode;}
  bool   userSigmaMode() const   { return _userSigmaMode;}

  virtual void generateNucleons();
  virtual void positionNucleons();
  virtual void recenterNucleus();
  virtual void findNucleusCenter(VectorLorentz & center);
  virtual void shiftNucleus(VectorLorentz & shift);
  virtual void rotateX(double phi);
  virtual void rotateY(double phi);
  virtual void rotateZ(double phi);
  virtual void rotateXYZ(double psiX, double psiY, double psiZ);
  virtual void resetNucleons();
  virtual void clearNucleons();
  virtual void reset();
  virtual void clear();
  virtual void print() const;

  bool hasNucleonCloseTo(double x, double y, double z, double dSqMax) const;
  bool hasNucleonCloseTo(const VectorLorentz & reference, double dSqMax) const;
  bool hasNucleonCloseTo(const GlauberNucleon & reference, double dSqMax) const;

  void setNucleus();
  void setAllowOverlap(bool v)    { _allowOverlap = v;}
  void setUseLattice(bool v)      { _useLattice = v;}
  void setMaxNumberTrials(int v)  { _nTrialsMax = v;}
  void setLatticeType(int v)      { _latticeType = v;}
  void setMinInterNucleonDistance(double v)
  {
  _nnDistanceMin = v;
  _nnDistanceMinSq = v*v;
  }
  void setNodeDistance(double v)     { _nodeDistance = v;}
  void setRecenteringMode(double v)  { _recenteringMode = v; }
  void setUserSigmaMode(bool v)      { _userSigmaMode = v;}

  protected:
  
  GlauberNucleusType _nucleusType;
  vector<GlauberNucleon> _nucleons;
  
  bool   _allowOverlap;
  bool   _useLattice;
  int    _latticeType;
  int    _nTrialsMax;
  double _nnDistanceMin;
  double _nnDistanceMinSq;
  double _nodeDistance;
  int    _recenteringMode;
  int    _overlapMode;
  bool   _userSigmaMode;

  NuclearDistribution _protonDistribution;
  NuclearDistribution _neutronDistribution;

  ClassDef(GlauberNucleus,1) // GlauberNucleus class
  };
  
} // namespace CAP

#endif



