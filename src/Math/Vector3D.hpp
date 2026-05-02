#ifndef CAP_Vector3D
#define CAP_Vector3D
#include <algorithm>
#include <limits>
#include <cmath>
#include <ostream>
#include <vector>
#include "Aliases.hpp"

namespace CAP
{
namespace MATH
{

class Vector3D
{
protected:
  double _x1;
  double _x2;
  double _x3;

public:

  friend class VectorLorentz;
  
  Vector3D();
  Vector3D(const double & _x1, const double & _x2, const double & _x3);
  Vector3D(const float values[3]);
  Vector3D(const double values[3]);
  Vector3D(std::vector<float> values);
  Vector3D(std::vector<double> values);
  Vector3D(const Vector3D & source);
  virtual ~Vector3D() {};
  double operator[] (unsigned int index) const;

  inline double x()  const
  {
  return _x1;
  }

  inline double y()  const
  {
  return _x2;
  }

  inline double z()  const
  {
  return _x3;
  }

  void setXYZ(const double & _x1, const double & _x2, const double & _x3);
  void setXYZ(const float values[3]);
  void setXYZ(const double values[3]);
  void setXYZ(const std::vector<float> values);
  void setXYZ(const std::vector<double> values);
  void setXYZ(const Vector3D & source);
  void setPhiThetaR(const double & phi, const double & theta, const double & r);
  void setPhiRhoZ(const double & phi, const double & rho, const double & z);
  inline double  phi() const
  {
  return std::atan2(_x2,_x1);
  }

  inline double  cosPhi() const
  {
  return _x1/std::sqrt(_x1*_x1+_x2*_x2);
  }

  inline double  sinPhi() const
  {
  return _x2/std::sqrt(_x1*_x1+_x2*_x2);
  }

  double  deltaPhi(const Vector3D & other) const;

  inline double theta() const
  {
  return std::atan2(std::sqrt(_x1*_x1+_x2*_x2),_x3);
  }

  inline double  cosTheta() const
  {
  return _x3/std::sqrt(_x1*_x1+_x2*_x2+_x3*_x3);
  }

  inline double sinTheta() const
  {
  double ptSq = _x1*_x1+_x2*_x2;
  return std::sqrt(ptSq)/std::sqrt(ptSq+_x3*_x3);
  }

  inline double modulus() const
  {
  return std::sqrt(_x1*_x1+_x2*_x2+_x3*_x3);
  }

  inline double  modulusSquare() const
  {
  return _x1*_x1+_x2*_x2+_x3*_x3;
  }

  inline double  perp() const
  {
  return std::sqrt(_x1*_x1+_x2*_x2);
  }

  inline double perpSquare() const
  {
  return _x1*_x1+_x2*_x2;
  }

  Vector3D & operator= (const Vector3D & rhs);
  bool  operator== (const Vector3D & rhs) const;
  bool  operator!= (const Vector3D & rhs) const;
  Vector3D & operator+= (const Vector3D & rhs);
  Vector3D & operator-= (const Vector3D & otherVector);
  Vector3D operator- () const;
  inline double  scalarProduct(const Vector3D & rhs) const
  {
  return _x1*rhs._x1 + _x2*rhs._x2 + _x3*rhs._x3;
  }

  double distanceX(const Vector3D & other) const;
  double distanceY(const Vector3D & other) const;
  double distanceZ(const Vector3D & other) const;
  double distanceSq(const Vector3D & other) const;
  double distance(const Vector3D & other) const;

  const Vector3D unit();
  void rotateX(const double & angle);
  void rotateY(const double & angle);
  void rotateZ(const double & angle);
  virtual void print() const;

  friend Vector3D operator+(const Vector3D & left, const Vector3D & right);
  friend Vector3D operator-(const Vector3D & left, const Vector3D & right);
  friend double operator*(const Vector3D & left, const Vector3D & right);
  friend double operator*(const Vector3D & left,const Vector3D & right);
  friend Vector3D operator* (const Vector3D & v, double a);
  friend Vector3D operator* (double a, const Vector3D & v);
  friend std::ostream& operator<<(std::ostream& out, const Vector3D & v);

  ClassDef(Vector3D,1)

};


//!
//! Add the two vectors and return a new vector
//!
Vector3D operator+(const Vector3D & left, const Vector3D & right);
Vector3D operator-(const Vector3D & left, const Vector3D & right);
double operator*(const Vector3D & left, const Vector3D & right);
double operator*(const Vector3D & left, const Vector3D & right);
Vector3D operator* (const Vector3D & v, double a);
Vector3D operator* (double a, const Vector3D & v);
std::ostream& operator<<(std::ostream& out, const Vector3D & v);

} // namespace MATH
} // namespace CAP

#endif
