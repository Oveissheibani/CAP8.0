#ifndef CAP__GlauberNucleon
#define CAP__GlauberNucleon
#include "GlauberExceptions.hpp"
#include "MathConstants.hpp"
#include "Aliases.hpp"
//#include "TF1.h"
//#include "TF2.h"
#include "VectorLorentz.hpp"
#include "NuclearDistribution.hpp"

using CAP::MATH::VectorLorentz;

namespace CAP
{

//!
//! Class defines  a nucleon for the purpose of MC Glauber simulations
//!
class GlauberNucleon
{
protected:
  int _nBinary;  //! Number of binary collisions
  int _nHard;    //! Number of hard  collisions
  int _type;     //! 0= proton, 1=neutron
  VectorLorentz _position;
  double _xSect; // used when nucleon size fluctuate

public:

  GlauberNucleon();
  GlauberNucleon(const GlauberNucleon & nucleon);
  GlauberNucleon & operator=(const GlauberNucleon & nucleon);
  virtual ~GlauberNucleon() {}

  void  set(const VectorLorentz & newPosition, int type);
  void  setPosition(const VectorLorentz & newPosition);
  void  setNucleonType(int type)       { _type = type;}
  void  setTypeToProton()              { _type = 0; }
  void  setTypeToNeutron()             { _type = 1; }
  void  setXSect(double v)             { _xSect= v; }

  void  collide()                      { ++_nBinary;}
  void  collideHard()                  { ++_nHard;}
  double get2CWeight(double x) const { return 2.*(0.5*(1-x)+0.5*x*_nBinary);}

  VectorLorentz & position()              { return _position; }
  const VectorLorentz & position() const  { return _position; }
  int   type() const                      { return _type;}
  int   nCollisions() const               { return _nBinary;}
  int   nHardCollisions() const           { return _nHard;}
  bool  isProton() const                  { return _type==0; }
  bool  isNeutron() const                 { return _type==1; }
  bool  isSpectator() const               { return _nBinary==0;}
  bool  isWounded() const                 { return _nBinary>0;}
  bool  isWoundedHard() const             { return _nHard>0;}
  double xSect() const                    { return _xSect;  }

  void  rotateX(double phi);
  void  rotateY(double phi);
  void  rotateZ(double phi);
  void  rotateXYZ(double psiX, double psiY, double psiZ);
  void  shiftPosition(const VectorLorentz & shift);
  void  reset();
  void  clear();
  double distanceTo(double x, double y, double z) const;
  double distanceTo(const GlauberNucleon & nucleon) const;
  double distanceTo2D(double x, double y) const;
  double distanceTo2D(const GlauberNucleon & nucleon) const;
  double squareDistanceTo(double x, double y, double z) const;
  double squareDistanceTo(const GlauberNucleon & nucleon) const;
  double square2DDistanceTo(double x, double y) const;
  double square2DDistanceTo(const GlauberNucleon & nucleon) const;
  bool   isCloseTo(double x, double y, double z, double maxDistanceSq) const;
  bool   isCloseTo(const VectorLorentz & _position, double maxDistanceSq) const;
  bool   isCloseTo(const GlauberNucleon & nucleon, double maxDistanceSq) const;
  bool   isCloseTo2D(double x, double y, double maxDistanceSq) const;
  bool   isCloseTo2D(const VectorLorentz & _position, double maxDistanceSq) const;
  bool   isCloseTo2D(const GlauberNucleon & nucleon, double maxDistanceSq) const;

  void   setRndmPositionInCube(double halfSize=20.0);
  void   setRndmPositionInSphere(double radius=400.0);
  void   setRndmPositionInSphere(NuclearDistribution & d);
  void   print() const;

  ClassDef(GlauberNucleon,1) // GlauberNucleon class
};

}  // namespace CAP


#endif



