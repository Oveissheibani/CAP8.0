#include "Vector3D.hpp"
#include "MathExceptions.hpp"
#include "MathConstants.hpp"

ClassImp(Vector3D);


namespace CAP
{
namespace MATH
{
Vector3D::Vector3D()
:
_x1(0),
_x2(0),
_x3(0)
{ }

Vector3D::Vector3D(const double & x1, const double & x2, const double & x3)
:
_x1(x1),
_x2(x2),
_x3(x3)
{  }

Vector3D::Vector3D(const float values[3])
:
_x1(values[0]),
_x2(values[1]),
_x3(values[2])
{  }

Vector3D::Vector3D(const double values[3])
:
_x1(values[0]),
_x2(values[1]),
_x3(values[2])
{  }

Vector3D::Vector3D(std::vector<float> values)
:
_x1(values[0]),
_x2(values[1]),
_x3(values[2])
{  }

Vector3D::Vector3D(std::vector<double> values)
:
_x1(values[0]),
_x2(values[1]),
_x3(values[2])
{  }

Vector3D::Vector3D(const Vector3D & source)
:
_x1(source._x1),
_x2(source._x2),
_x3(source._x3)
{  }


//!
//! Returns the value stored at the give index.
//!
double Vector3D::operator[] (unsigned int index) const
{
  switch (index)
    {
      case 1: return _x1;
      case 2: return _x2;
      case 3: return _x3;
      default: throw MathException("index>3","Vector3D::operator()");
    }
}

void Vector3D::setXYZ(const double & x1, const double & x2, const double & x3)
{
  _x1 = x1;
  _x2 = x2;
  _x3 = x3;
}

void Vector3D::setXYZ(const float values[3])
{
  _x1 = values[0];
  _x2 = values[1];
  _x3 = values[2];
}

void Vector3D::setXYZ(const double values[3])
{
  _x1 = values[0];
  _x2 = values[1];
  _x3 = values[2];
}

void Vector3D::setXYZ(const std::vector<float> values)
{
  _x1 = values[0];
  _x2 = values[1];
  _x3 = values[2];
}

void Vector3D::setXYZ(const std::vector<double> values)
{
  _x1 = values[0];
  _x2 = values[1];
  _x3 = values[2];
}

void Vector3D::setXYZ(const Vector3D & source)
{
  _x1 = source._x1;
  _x2 = source._x2;
  _x3 = source._x3;
}

void Vector3D::setPhiThetaR(const double & phi, const double & theta, const double & r)
{
  double rSinTheta = r*std::sin(theta);
  _x1 = rSinTheta*std::cos(phi);
  _x2 = rSinTheta*std::sin(phi);
  _x3 = r*std::cos(theta);
}


void Vector3D::setPhiRhoZ(const double & phi, const double & rho, const double & z)
{
  _x1 = rho*std::cos(phi);
  _x2 = rho*std::sin(phi);;
  _x3 = z;
}


//!
//! Return the azimuthal angle between this and the other vector
//!  TXYZ=0, TPhiThetaR,  TPhiRhoZ, MXYZ, MPhiRhoY, MPhiRhoEta};
double  Vector3D::deltaPhi(const Vector3D & other) const
{
  double dPhi = std::atan2(_x2,_x1) - std::atan2(other._x2,other._x1);
  while (dPhi>twoPi()) dPhi -= twoPi();
  while (dPhi<0) dPhi += twoPi();
  return dPhi;
}

Vector3D & Vector3D::operator= (const Vector3D & rhs)
{
  if (this!=&rhs)
    {
    _x1 = rhs._x1;
    _x2 = rhs._x2;
    _x3 = rhs._x3;
    }
  return *this;
}

bool  Vector3D::operator== (const Vector3D & rhs) const
{
  return (_x1==rhs._x1)  && 
  (_x2==rhs._x2)  && 
  (_x3==rhs._x3);
}

bool  Vector3D::operator!= (const Vector3D & rhs) const
{
  return (_x1!=rhs._x1) ||
  (_x2!=rhs._x2) ||
  (_x3!=rhs._x3);
}

Vector3D & Vector3D::operator+= (const Vector3D & rhs)
{
  _x1 += rhs._x1;
  _x2 += rhs._x2;
  _x3 += rhs._x3;
  return *this;
}

Vector3D & Vector3D::operator-= (const Vector3D & rhs)
{
  _x1 -= rhs._x1;
  _x2 -= rhs._x2;
  _x3 -= rhs._x3;
  return *this;
}

//! Unary minus.
Vector3D Vector3D::operator- () const
{
  return Vector3D(-_x1,-_x2,-_x3);
}

double Vector3D::distanceX(const Vector3D & other) const
{
  return _x1 - other._x1;
}

double Vector3D::distanceY(const Vector3D & other) const
{
  return _x2 - other._x2;
}

double Vector3D::distanceZ(const Vector3D & other) const
{
  return _x3 - other._x3;
}

double Vector3D::distanceSq(const Vector3D & other) const
{
  double d_x1 = _x1 - other._x1;
  double d_x2 = _x2 - other._x2;
  double d_x3 = _x3 - other._x3;
  return d_x1*d_x1+d_x2*d_x2+d_x3*d_x3;
}

double Vector3D::distance(const Vector3D & other) const
{
  double d_x1 = _x1 - other._x1;
  double d_x2 = _x2 - other._x2;
  double d_x3 = _x3 - other._x3;
  return std::sqrt(d_x1*d_x1+d_x2*d_x2+d_x3*d_x3);
}

//! produce a 3D unit vector in the direction of this vector
const Vector3D Vector3D::unit()
{
  double r = std::sqrt(_x1*_x1+_x2*_x2+_x3*_x3);
  if (r==0) throw MathException("Cannot produced unit vector from null vector",__FUNCTION__);
  return Vector3D(_x1/r,_x2/r,_x3/r);
}

//! Active rotation of this Vector3D by the given angle relative to the x-axis
void Vector3D::rotateX(const double & angle)
{
  double s = std::sin(angle);
  double c = std::cos(angle);
  double y = _x2;
  double z = _x3;
  _x2 = c*y - s*z;
  _x3 = s*y + c*z;
}

//! Active rotation of this Vector3D by the given angle relative to the y-axis
void Vector3D::rotateY(const double & angle)
{
  double s = std::sin(angle);
  double c = std::cos(angle);
  double x = _x1;
  double z = _x3;
  _x1 = c*x + s*z;
  _x3 = c*z - s*x;
}

//! Active rotation of this Vector3D by the given angle relative to the z-axis
void Vector3D::rotateZ(const double & angle)
{
  double s = std::sin(angle);
  double c = std::cos(angle);
  double x = _x1;
  double y = _x2;
  _x1 = c*x - s*y;
  _x2 = c*y + s*x;
}

void Vector3D::print() const
{
  std::cout << "(" << _x1 << "," << _x2 << "," << _x3 << ") " << std::endl;
}

//!
//! Add the two vectors and return a new vector
//!
Vector3D operator+ (const Vector3D & left, const Vector3D & right)
{
  return Vector3D(left._x1+right._x1, left._x2+right._x2, left._x3+right._x3);
}

Vector3D operator- (const Vector3D & left, const Vector3D & right)
{
  return Vector3D(left._x1-right._x1, left._x2-right._x2, left._x3-right._x3);
}

//!
//! SR (3D) scalar product of two left and right vectors
//!
double operator* (const Vector3D & left, const Vector3D & right)
  {
  return left._x1*right._x1 + left._x2*right._x2 + left._x3*right._x3;
  }


//!
//! Scaling of the left vector by a scalar value from the right
//!
Vector3D operator* (const Vector3D & v, double a)
  {
  return Vector3D(a*v._x1, a*v._x2, a*v._x3);
  }

//!
//! Scaling of the right vector by a scalar value from the left
//!
Vector3D operator* (double a, const Vector3D & v)
  {
  return Vector3D(a*v._x1, a*v._x2, a*v._x3);
  }

//!
//! Print out the given vector to the given ostream.
//!
std::ostream& operator<<(std::ostream& out, const Vector3D & v)
{
  out << "(" << v.x() << "," << v.y() << "," << v.z() << ")" ;
  return out;
}

} // namespace MATH
} // namespace CAP


