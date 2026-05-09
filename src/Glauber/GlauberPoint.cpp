#include "MathConstants.hpp"
#include "GlauberPoint.hpp"

ClassImp(CAP::GlauberPoint)

namespace CAP
{
  
  using CAP::MATH::pi;
  using CAP::MATH::twoPi;
  
  //!
  //!Default CTOR
  //!
  GlauberPoint::GlauberPoint()
  :
  _x(0),
  _y(0)
  { }
  
    //!
    //!Detailed CTOR
    //!
  GlauberPoint::GlauberPoint(float x, float y)
  :
  _x(x),
  _y(y)
  { }
  
    //!
    //!Copy CTOR
    //!
  GlauberPoint::GlauberPoint(const GlauberPoint & src)
  :
  _x(src._x),
  _y(src._y)
  { }
  
    //!
    //!Assignment operator
    //!
  GlauberPoint GlauberPoint::operator=(const GlauberPoint & rhs)
  {
    if (this!=&rhs)
    {
      _x = rhs._x;
      _y = rhs._y;
    }
    return *this;
  }
  
    //!
    //! return the square of the transverse  distance (xy plane) between this and the given point.
    //!
  float GlauberPoint::distanceSquare(const GlauberPoint & rhs) const
  {
    float dx = _x - rhs._x;
    float dy = _y - rhs._y;
    return dx*dx + dy*dy;
  }
  
    //!
    //! return the transverse distance (xy plane) between this and the given point.
    //!
  float GlauberPoint::distance(const GlauberPoint & rhs) const
  {
    float dx = _x - rhs._x;
    float dy = _y - rhs._y;
    return std::sqrt(dx*dx + dy*dy);
  }
  
    //!
    //! determine whether this point is equal to the given point.
    //!
  bool  GlauberPoint::equalTo(const GlauberPoint & rhs) const
  {
    return _x==rhs._x  &&  _y==rhs._y;
  }
  
    //!
    //! determine whether this point is within the given max distance of  the given point.
    //!
  bool  GlauberPoint::closeTo(const GlauberPoint & rhs,float maxDistance) const
  {
    float dx = _x - rhs._x;
    float dy = _y - rhs._y;
    return std::sqrt(dx*dx + dy*dy)<=maxDistance;
  }
  
    //!
    //! perform active rotation of the point in the xy plane.
    //!
  void   GlauberPoint::rotateXY(float phi)
  {
    float cosPhi = std::cos(phi);
    float sinPhi = std::sin(phi);
    float x = _x*cosPhi - _y*sinPhi;
    float y = _x*sinPhi + _y*cosPhi;
    _x = x;
    _y = y;
  }
  
  
}  // namespace


