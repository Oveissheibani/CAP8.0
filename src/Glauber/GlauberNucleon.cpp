#include "GlauberNucleon.hpp"
#include "PrintHelpers.hpp"
#include "TRandom.h"

ClassImp(CAP::GlauberNucleon)

namespace CAP
{
  using CAP::MATH::pi;
  using CAP::MATH::twoPi;
  
  GlauberNucleon::GlauberNucleon()
  :
  _nBinary(0),
  _nHard(0),
  _type(0),
  _position(),
  _xSect(0)
  { }
  
  GlauberNucleon::GlauberNucleon(const GlauberNucleon & src)
  :
  _nBinary(src._nBinary),
  _nHard(src._nHard),
  _type(src._type),
  _position(src._position),
  _xSect(src._xSect)
  {   }
  
  GlauberNucleon & GlauberNucleon::operator=(const GlauberNucleon & rhs)
  {
  if (this!=&rhs)
    {
    _nBinary  = rhs._nBinary;
    _nHard    = rhs._nHard;
    _type     = rhs._type;
    _position = rhs._position;
    _xSect    = rhs._xSect;
    }
  return *this;
  }
  
  void  GlauberNucleon::set(const VectorLorentz & newPosition, int type)
  {
  _position = newPosition;
  _type     = type;
  }
  
  void  GlauberNucleon::setPosition(const VectorLorentz & newPosition)
  {
  _position = newPosition;
  }
  
    //!
    //!rotate this nucleon's position about x-axis (beam axis)
    //!
  void GlauberNucleon::rotateX(double phi)
  {
  _position.rotateX(phi);
  }
  
    //!
    //!rotate this nucleon's position about y-axis (beam axis)
    //!
  void GlauberNucleon::rotateY(double phi)
  {
  _position.rotateY(phi);
  }
  
    //!
    //!rotate this nucleon's position about z-axis (beam axis)
    //!
  void GlauberNucleon::rotateZ(double phi)
  {
  _position.rotateZ(phi);
  }
  
  void GlauberNucleon::rotateXYZ(double psiX, double psiY, double psiZ)
  {
  _position.rotateX(psiX);
  _position.rotateY(psiY);
  _position.rotateZ(psiZ);
  }
  
  
    //!
    //!shift this nucleon's position by the given vector shift
    //!
  void GlauberNucleon::shiftPosition(const VectorLorentz & shift)
  {
  _position += shift;
  }
  
    //!
    //!Reset this nucleon, but do not change its type
    //!
  void GlauberNucleon::reset()
  {
  double zero = 0.0;
  _position.setTXYZ(zero,zero,zero,zero);
  _nBinary = 0;
  _nHard   = 0;
  }
  
    //!
    //!Clear this nucleon including its type
    //!
  void GlauberNucleon::clear()
  {
  double zero = 0.0;
  _position.setTXYZ(zero,zero,zero,zero);
  _nBinary = 0;
  _nHard   = 0;
  _type       = 0;
  }
  
    //!
    //!Determine the  distance from  the given point (x,y,z)
    //!
  double GlauberNucleon::distanceTo(double x, double y, double z) const
  {
  double dx = _position.x() - x;
  double dy = _position.y() - y;
  double dz = _position.z() - z;
  return std::sqrt(dx*dx+dy*dy+dz*dz);
  }
  
    //!
    //!Determine the  distance from  the given nucleon
    //!
  double GlauberNucleon::distanceTo(const GlauberNucleon & nucleon) const
  {
  const VectorLorentz & other = nucleon.position();
  double dx = _position.x() - other.x();
  double dy = _position.y() - other.y();
  double dz = _position.z() - other.z();
  return std::sqrt(dx*dx+dy*dy+dz*dz);
  }
  
    //!
    //!Determine the  distance from  the given point (x,y) in the transverse plane
    //!
  double GlauberNucleon::distanceTo2D(double x, double y) const
  {
  double dx = _position.x() - x;
  double dy = _position.y() - y;
  return std::sqrt(dx*dx+dy*dy);
  }
  
    //!
    //!Determine the  distance from  the given nucleon in the transverse plane
    //!
  double GlauberNucleon::distanceTo2D(const GlauberNucleon & nucleon) const
  {
  const VectorLorentz & other = nucleon.position();
  double dx = _position.x() - other.x();
  double dy = _position.y() - other.y();
  return std::sqrt(dx*dx+dy*dy);
  }
  
    //!
    //!Determine the  distance from  the given point (x,y,z)
    //!
  double GlauberNucleon::squareDistanceTo(double x, double y, double z) const
  {
  double dx = _position.x() - x;
  double dy = _position.y() - y;
  double dz = _position.z() - z;
  return dx*dx+dy*dy+dz*dz;
  }
  
    //!
    //!Determine the  distance from  the given nucleon
    //!
  double GlauberNucleon::squareDistanceTo(const GlauberNucleon & nucleon) const
  {
  const VectorLorentz & other = nucleon.position();
  double dx = _position.x() - other.x();
  double dy = _position.y() - other.y();
  double dz = _position.z() - other.z();
  return dx*dx+dy*dy+dz*dz;
  }
  
    //!
    //!Determine the  distance from  the given point (x,y) in the transverse plane
    //!
  double GlauberNucleon::square2DDistanceTo(double x, double y) const
  {
  double dx = _position.x() - x;
  double dy = _position.y() - y;
  return dx*dx+dy*dy;
  }
  
    //!
    //!Determine the  distance from  the given nucleon in the transverse plane
    //!
  double GlauberNucleon::square2DDistanceTo(const GlauberNucleon & nucleon) const
  {
  const VectorLorentz & other = nucleon.position();
  double dx = _position.x() - other.x();
  double dy = _position.y() - other.y();
  return dx*dx+dy*dy;
  }
  
    //!
    //!Determine whether this nucleon is at distance from (x,y,z) smaller than the given maximum
    //!Always return false if the maxDistance<0
    //!
  bool   GlauberNucleon::isCloseTo(double x, double y, double z, double maxDistanceSq) const
  {
  if (maxDistanceSq<=0.0) return false;
  double dx = _position.x() - x;
  double dy = _position.y() - y;
  double dz = _position.z() - z;
  return (dx*dx+dy*dy+dz*dz) <= maxDistanceSq;
  }
  
  bool   GlauberNucleon::isCloseTo(const VectorLorentz & reference, double maxDistanceSq) const
  {
  if (maxDistanceSq<=0.0) return false;
  double dx = _position.x() - reference.x();
  double dy = _position.y() - reference.y();
  double dz = _position.z() - reference.z();
  return (dx*dx+dy*dy+dz*dz) <= maxDistanceSq;
  }
  
  
    //!
    //!Determine whether this nucleon is at distance from the given nucleon smaller than the given maximum
    //!Always return false if the maxDistance<0
    //!
  bool   GlauberNucleon::isCloseTo(const GlauberNucleon & nucleon, double maxDistanceSq) const
  {
  if (maxDistanceSq<=0.0) return false;
  const VectorLorentz & other = nucleon.position();
  double dx = _position.x() - other.x();
  double dy = _position.y() - other.y();
  double dz = _position.z() - other.z();
  return (dx*dx+dy*dy+dz*dz) <= maxDistanceSq;
  }
  
    //!
    //!Determine whether this nucleon is at distance from (x,y) smaller than the given maximum in the transverse plane
    //!Always return false if the maxDistance<0
    //!
  bool   GlauberNucleon::isCloseTo2D(double x, double y, double maxDistanceSq) const
  {
  if (maxDistanceSq<=0.0) return false;
  double dx = _position.x() - x;
  double dy = _position.y() - y;
  return (dx*dx+dy*dy) <= maxDistanceSq;
  }
  
  bool   GlauberNucleon::isCloseTo2D(const VectorLorentz & reference, double maxDistanceSq) const
  {
  if (maxDistanceSq<=0.0) return false;
  double dx = _position.x() - reference.x();
  double dy = _position.y() - reference.y();
  return (dx*dx+dy*dy) <= maxDistanceSq;
  }
  
    //!
    //!Determine whether this nucleon is at distance from the given nucleon  smaller than the given maximum in the transverse plane
    //!Always return false if the maxDistance<0
    //!
  bool   GlauberNucleon::isCloseTo2D(const GlauberNucleon & nucleon, double maxDistanceSq) const
  {
  if (maxDistanceSq<=0.0) return false;
  const VectorLorentz & other = nucleon.position();
  double dx = _position.x() - other.x();
  double dy = _position.y() - other.y();
  return (dx*dx+dy*dy) <= maxDistanceSq;
  }
  
  void GlauberNucleon::setRndmPositionInCube(double halfSize)
  {
  double x = halfSize*(2.0*gRandom->Rndm()-1);
  double y = halfSize*(2.0*gRandom->Rndm()-1);
  double z = halfSize*(2.0*gRandom->Rndm()-1);
  _position.setTXYZ(0.0,x,y,z);
  }
  
  void GlauberNucleon::setRndmPositionInSphere(double maxRadiusSq)
  {
  double phi    = twoPi()*gRandom->Rndm();
  double cosTh  = 2.0*gRandom->Rndm()-1.0;
  double sinTh  = std::sqrt(1.0-cosTh*cosTh);
  double r      = std::sqrt(maxRadiusSq)*gRandom->Rndm();
  double rSinTh = r*sinTh;
  double x      = rSinTh*std::cos(phi);
  double y      = rSinTh*std::sin(phi);
  double z      = r*cosTh;
//    printValue("phi",phi);
//    printValue("cosTh",cosTh);
//    printValue("r",r);
//    printValue("x",x);
//    printValue("y",y);
//    printValue("z",z);
  _position.setTXYZ(0.0,x,y,z);
  }
  
  void GlauberNucleon::setRndmPositionInSphere(NuclearDistribution & d)
  {
  double phi    = twoPi()*gRandom->Rndm();
  double cosTh  = 2.0*gRandom->Rndm()-1.0;
  double sinTh  = std::sqrt(1.0-cosTh*cosTh);
  double r      = d.random();
  double rSinTh = r*sinTh;
  double x      = rSinTh*std::cos(phi);
  double y      = rSinTh*std::sin(phi);
  double z      = r*cosTh;
//  printValue("phi",phi);
//  printValue("cosTh",cosTh);
//  printValue("r",r);
//  printValue("x",x);
//  printValue("y",y);
//  printValue("z",z);
  _position.setTXYZ(0.0,x,y,z);
  }
  
  
  void GlauberNucleon::print() const
  {
  if (isProton())
    printString("PROTON");
  else
    printString("NEUTRON");
  printValue("isWounded", isWounded());
  printValue("isSpectator", isSpectator());
  printValue("nCollisions", _nBinary);
  printValue("nHardCollisionsCollisions", _nHard);
  }
  
} // namespace CAP


