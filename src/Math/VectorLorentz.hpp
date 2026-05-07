#ifndef CAP_VectorLorentz
#define CAP_VectorLorentz
#include <algorithm>
#include <limits>
#include <cmath>
#include "Aliases.hpp"
#include "Exceptions.hpp"
#include "MathConstants.hpp"
#include "Vector3D.hpp"

namespace CAP
{
namespace MATH
{


//!
//! Class encapsulates Lorentz vectors
//!
//!  Note 1: Uses the convention that c==1 (speed of light equals unity) so that t,x,y,z  have  units of distance (t is ct) and E, px, py, pz have units of energy (MeV or GeV in most cases)
//!  Note 2: The (+,-,-,-) metric is used throughout.
//!  Note 3: double or e are considered the 0th component as in most modern texts.
//!
//! Storage  for lorentz vectors
//! x0=t, x1= x,x2= y, x3=z (i.e., cartesian coordinates)


class VectorLorentz
{
protected:
  double x0;
  double x1;
  double x2;
  double x3;

public:

  VectorLorentz();
  VectorLorentz(const double & _x0, const double & _x1, const double & _x2, const double & _x3);
  VectorLorentz(const float _values[4]);
  VectorLorentz(const double _values[4]);
  VectorLorentz(std::vector<float> _values);
  VectorLorentz(std::vector<double> _values);
  VectorLorentz(const VectorLorentz & source);
  virtual ~VectorLorentz() {};
  double operator[] (unsigned int index) const;
  inline double t()  const
  {
  return x0;
  }

  inline double x()  const
  {
  return x1;
  }

  inline double y()  const
  {
  return x2;
  }

  inline double z()  const
  {
  return x3;
  }

  void setTXYZ(const double & _x0, const double & _x1, const double & _x2, const double & _x3);
  void setTXYZ(const float _values[4]);
  void setTXYZ(const double _values[4]);
  void setTXYZ(const std::vector<float> _values);
  void setTXYZ(const std::vector<double> _values);
  void setTXYZ(const VectorLorentz & source);
  void setTPhiThetaR(const double & t, const double & phi, const double & theta, const double & r);
  void setTPhiRhoZ(const double & t, const double & phi, const double & rho, const double & z);
  void setMXYZ(const double & m, const double & x, const double & y, const double & z);
  void setMPhiRhoZ(const double & m, const double & phi, const double & rho, const double & z);
  void setMPhiPtY(const double & m, const double & phi, const double & pt, const double & y);
  inline double  phi() const
  {
  return std::atan2(x2,x1);
  }

  inline double  cosPhi() const
  {
  return x1/std::sqrt(x1*x1+x2*x2);
  }

  inline double  sinPhi() const
  {
  return x2/std::sqrt(x1*x1+x2*x2);
  }

  double  deltaPhi(const VectorLorentz & other) const;

  inline double theta() const
  {
  return std::atan2(std::sqrt(x1*x1+x2*x2),x3);
  }

  inline double  cosTheta() const
  {
  return x3/std::sqrt(x1*x1+x2*x2+x3*x3);
  }

  inline double sinTheta() const
  {
  double ptSq = x1*x1+x2*x2;
  return std::sqrt(ptSq)/std::sqrt(ptSq+x3*x3);
  }

  inline double modulus3() const
  {
  return std::sqrt(x1*x1+x2*x2+x3*x3);
  }

  inline double  modulus3Square() const
  {
  return x1*x1+x2*x2+x3*x3;
  }

  double modulus() const;

  inline double  modulusSquare() const
  {
  return x0*x0 - (x1*x1+x2*x2+x3*x3);
  }

  inline double  perp() const
  {
  return std::sqrt(x1*x1+x2*x2);
  }

  inline double perpSquare() const
  {
  return x1*x1+x2*x2;
  }

  Vector3D xyz() const;

  inline double massSquare() const
  {
  return modulusSquare();
  }

  double mass() const;
  double betaX() const;
  double betaY() const;
  double betaZ() const;
  double beta() const;
  double betaSquare() const;
  Vector3D betaVector() const;
  double gammaSquare() const;
  double gamma() const;
  double pseudorapidity() const;
  double transverseMassSq() const;
  double transverseMass() const;
  VectorLorentz & operator= (const VectorLorentz & rhs);
  bool  operator== (const VectorLorentz & rhs) const;
  bool  operator!= (const VectorLorentz & rhs) const;
  VectorLorentz & operator+= (const VectorLorentz & rhs);
  VectorLorentz & operator-= (const VectorLorentz & otherVector);
  VectorLorentz operator- () const;
  inline double  scalarProduct3D(const VectorLorentz & rhs) const
  {
  return x1*rhs.x1 + x2*rhs.x2 + x3*rhs.x3;
  }

  inline double  scalarProduct(const VectorLorentz & rhs) const
  {
  return x0*rhs.x0 - x1*rhs.x1 - x2*rhs.x2 - x3*rhs.x3;
  }

  double distanceT(const VectorLorentz & other) const;
  double distanceX(const VectorLorentz & other) const;
  double distanceY(const VectorLorentz & other) const;
  double distanceZ(const VectorLorentz & other) const;
  double distance2DSq(const VectorLorentz & other) const;
  double distance2D(const VectorLorentz & other) const;
  double distance3DSq(const VectorLorentz & other) const;
  double distance3D(const VectorLorentz & other) const;
  double distanceSq(const VectorLorentz & other) const;
  double distance(const VectorLorentz & other) const;


  const Vector3D unit()  const;
  void rotateX(const double & angle);
  void rotateY(const double & angle);
  void rotateZ(const double & angle);
  void boostX(const double & beta);
  void boostY(const double & beta);
  void boostZ(const double & beta);
  void boost(const double & betaX,const double & betaY,const double & betaZ);
  void boost(const Vector3D & v);
  double rapidity() const;
  double  deltaRapidity(const VectorLorentz & other) const;
  double  deltaPseudoRapidity(const VectorLorentz & other) const;
  virtual void print() const;

  friend VectorLorentz operator+(const VectorLorentz & left, const VectorLorentz & right);
  friend VectorLorentz operator-(const VectorLorentz & left, const VectorLorentz & right);
  friend double operator* (const VectorLorentz & left, const VectorLorentz & right);
  friend double operator*(const VectorLorentz & left,const VectorLorentz & right);
  friend VectorLorentz operator*(const VectorLorentz & v, double a);
  friend VectorLorentz operator*(double a, const VectorLorentz & v);
  friend std::ostream& operator<<(std::ostream& out, const VectorLorentz & v);

  ClassDef(VectorLorentz,1)

};


//!
//! Add the two vectors and return a new vector
//!
VectorLorentz operator+ (const VectorLorentz & left, const VectorLorentz & right);
VectorLorentz operator- (const VectorLorentz & left, const VectorLorentz & right);
double operator* (const VectorLorentz & left, const VectorLorentz & right);
double  operator*(const VectorLorentz & left,const VectorLorentz & right);
VectorLorentz operator* (const VectorLorentz & v, double a);
VectorLorentz operator* (double a, const VectorLorentz & v);
std::ostream& operator<<(std::ostream& out, const VectorLorentz & v);

} // namespace MATH
} // namespace CAP

#endif
