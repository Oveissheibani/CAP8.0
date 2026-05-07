#ifndef CAP__NuclearDistribution
#define CAP__NuclearDistribution
#include "MathConstants.hpp"
#include "TH1.h"

namespace CAP
{
  //!
  //!Class implements the notion of nuclear density distribution.
  //!
  //!From Improved Monte Carlo Glauber predictions at present and future nuclear colliders
  //!by Loizides, Kamin, d'Enterria
  //!
  //!The implementation of the MCG calculation is described in  detail in [6, 7].
  //!It consists of two steps: first, constructing the nuclei and, second, colliding the nuclei.
  //!To construct a nucleus, the position of each nucleon is determined according to a
  //!probability density function usually   taken from measurements of the charge density
  //!distribution of   the nucleus [2, 3]. For spherical nuclei, the nucleon positions
  //!can be determined in polar coordinates with a uniform distribution for the azimuthal and polar angles,
  //!coupled with a 2pF   distribution in the radial direction
  //![6] B. Alver, M. Baker, C. Loizides, and P. Steinberg, “The  PHOBOS Glauber Monte Carlo,” arXiv:0805.4411   [nucl-ex].
  //![7] C. Loizides, J. Nagle, and P. Steinberg, “Improved version of the PHOBOS Glauber Monte Carlo,” SoftwareX 1-2 (2015)  13–18, arXiv:1408.2549 [nucl-ex].
  //!
  //!Several implmentation and profiles are used -- based on the original code by Loizides and Steinberg.
  //!
  class NuclearDistribution
  {
  public:

  NuclearDistribution();
  NuclearDistribution(const NuclearDistribution & src);
  NuclearDistribution & operator=(const NuclearDistribution & rhs);
  virtual ~NuclearDistribution(){}
  virtual double evaluate(double x, double theta=0.0);
  virtual double random();
  virtual void reset();
  virtual void createHistogram();

  void setType(int value)      {  _type   = value; }
  void setRadius(double value) {  _radius = value; }
  void setSkin(double value)   {  _skin   = value; }
  void setW(double value)      {  _w = value;     }
  void setA(double value)      {  _a = value;     }
  void setB(double value)      {  _b = value;     }
  void setBeta2(double value)  {  _beta2 = value; }
  void setBeta4(double value)  {  _beta4 = value; }
  void setR0(double value)     {  _r0 = value;    }
  void setR1(double value)     {  _r1 = value;    }
  void setR2(double value)     {  _r2 = value;    }
  void setMaxRadius(double value) { _maxRadius = value; }

  int type() const       { return _type;  }
  double radius() const  { return _radius; }
  double skin() const    { return _skin; }
  double w() const       { return _w;    }
  double a() const       { return _a;    }
  double b() const       { return _b;    }
  double beta2() const   { return _beta2;}
  double beta4() const   { return _beta4;}
  double r0() const      { return _r0;   }
  double r1() const      { return _r1;   }
  double r2() const      { return _r2;   }
  double maxRadius() const { return _maxRadius; }

  

  protected:
  
  int    _type;
  double _radius;
  double _skin;
  double _w;
  double _a;
  double _b;
  double _beta2;
  double _beta4;
  double _r0;
  double _r1;
  double _r2;
  double _maxRadius;
  TH1 *  _radiusGen;

  static int _instances;

  };

} // namespace CAP

#endif // !CAP__NuclearDistribution



