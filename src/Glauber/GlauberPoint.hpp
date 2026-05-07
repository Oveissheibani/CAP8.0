#ifndef CAP__GlauberPoint
#define CAP__GlauberPoint
#include "TObject.h"

namespace CAP
{
using namespace std;


class GlauberPoint
{
public:
  GlauberPoint();
  GlauberPoint(float x, float y);
  GlauberPoint(const GlauberPoint & GlauberPoint);
  GlauberPoint operator=(const GlauberPoint & GlauberPoint);
  virtual ~GlauberPoint(){}
  float x() const  { return _x; }
  float y() const  { return _y; }
  float r() const  { return sqrt(_x*_x+_y*_y); }
  float r2() const { return _x*_x+_y*_y; }
  void set(float x, float y)
  {
  _x = x;
  _y = y;
  }
  float  distanceSquare(const GlauberPoint & GlauberPoint)  const;
  float  distance(const GlauberPoint & GlauberPoint)  const;
  bool   equalTo(const GlauberPoint & GlauberPoint)  const;
  bool   closeTo(const GlauberPoint & GlauberPoint, float maxDistance)  const;
  void   rotateXY(float phi);
    //!
    //! determine the azimuthal angle of the point relative to the x axis.
    //!
  float  phi() const { return atan2(_y,_x);}
  
protected:

  float _x;
  float _y;

  ClassDef(GlauberPoint,1) // GlauberPoint class

};

} // namespace CAP

#endif



