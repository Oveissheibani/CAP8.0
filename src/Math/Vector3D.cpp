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
x1(0),
x2(0),
x3(0)
{ }

Vector3D::Vector3D(const double & _x1, const double & _x2, const double & _x3)
:
x1(_x1),
x2(_x2),
x3(_x3)
{  }

Vector3D::Vector3D(const float _values[3])
:
x1(_values[0]),
x2(_values[1]),
x3(_values[2])
{  }

Vector3D::Vector3D(const double _values[3])
:
x1(_values[0]),
x2(_values[1]),
x3(_values[2])
{  }

Vector3D::Vector3D(std::vector<float> _values)
:
x1(_values[0]),
x2(_values[1]),
x3(_values[2])
{  }

Vector3D::Vector3D(std::vector<double> _values)
:
x1(_values[0]),
x2(_values[1]),
x3(_values[2])
{  }

Vector3D::Vector3D(const Vector3D & source)
:
x1(source.x1),
x2(source.x2),
x3(source.x3)
{  }


//!
//! Returns the value stored at the give index.
//!
double Vector3D::operator[] (unsigned int index) const
{
  switch (index)
    {
      case 1: return x1;
      case 2: return x2;
      case 3: return x3;
      default: throw MathException("index>3","Vector3D::operator()");
    }
}

void Vector3D::setXYZ(const double & _x1, const double & _x2, const double & _x3)
{
  x1 = _x1;
  x2 = _x2;
  x3 = _x3;
}

void Vector3D::setXYZ(const float _values[3])
{
  x1 = _values[0];
  x2 = _values[1];
  x3 = _values[2];
}

void Vector3D::setXYZ(const double _values[3])
{
  x1 = _values[0];
  x2 = _values[1];
  x3 = _values[2];
}

void Vector3D::setXYZ(const std::vector<float> _values)
{
  x1 = _values[0];
  x2 = _values[1];
  x3 = _values[2];
}

void Vector3D::setXYZ(const std::vector<double> _values)
{
  x1 = _values[0];
  x2 = _values[1];
  x3 = _values[2];
}

void Vector3D::setXYZ(const Vector3D & source)
{
  x1 = source.x1;
  x2 = source.x2;
  x3 = source.x3;
}

void Vector3D::setPhiThetaR(const double & phi, const double & theta, const double & r)
{
  double rSinTheta = r*std::sin(theta);
  x1 = rSinTheta*std::cos(phi);
  x2 = rSinTheta*std::sin(phi);
  x3 = r*std::cos(theta);
}


void Vector3D::setPhiRhoZ(const double & phi, const double & rho, const double & z)
{
  x1 = rho*std::cos(phi);
  x2 = rho*std::sin(phi);;
  x3 = z;
}


//!
//! Return the azimuthal angle between this and the other vector
//!  TXYZ=0, TPhiThetaR,  TPhiRhoZ, MXYZ, MPhiRhoY, MPhiRhoEta};
double  Vector3D::deltaPhi(const Vector3D & other) const
{
  double dPhi = std::atan2(x2,x1) - std::atan2(other.x2,other.x1);
  while (dPhi>twoPi()) dPhi -= twoPi();
  while (dPhi<0) dPhi += twoPi();
  return dPhi;
}

Vector3D & Vector3D::operator= (const Vector3D & rhs)
{
  if (this!=&rhs)
    {
    x1 = rhs.x1;
    x2 = rhs.x2;
    x3 = rhs.x3;
    }
  return *this;
}

bool  Vector3D::operator== (const Vector3D & rhs) const
{
  return (x1==rhs.x1)  && 
  (x2==rhs.x2)  && 
  (x3==rhs.x3);
}

bool  Vector3D::operator!= (const Vector3D & rhs) const
{
  return (x1!=rhs.x1) ||
  (x2!=rhs.x2) ||
  (x3!=rhs.x3);
}

Vector3D & Vector3D::operator+= (const Vector3D & rhs)
{
  x1 += rhs.x1;
  x2 += rhs.x2;
  x3 += rhs.x3;
  return *this;
}

Vector3D & Vector3D::operator-= (const Vector3D & rhs)
{
  x1 -= rhs.x1;
  x2 -= rhs.x2;
  x3 -= rhs.x3;
  return *this;
}

//! Unary minus.
Vector3D Vector3D::operator- () const
{
  return Vector3D(-x1,-x2,-x3);
}

double Vector3D::distanceX(const Vector3D & other) const
{
  return x1 - other.x1;
}

double Vector3D::distanceY(const Vector3D & other) const
{
  return x2 - other.x2;
}

double Vector3D::distanceZ(const Vector3D & other) const
{
  return x3 - other.x3;
}

double Vector3D::distanceSq(const Vector3D & other) const
{
  double dx1 = x1 - other.x1;
  double dx2 = x2 - other.x2;
  double dx3 = x3 - other.x3;
  return dx1*dx1+dx2*dx2+dx3*dx3;
}

double Vector3D::distance(const Vector3D & other) const
{
  double dx1 = x1 - other.x1;
  double dx2 = x2 - other.x2;
  double dx3 = x3 - other.x3;
  return std::sqrt(dx1*dx1+dx2*dx2+dx3*dx3);
}

//! produce a 3D unit vector in the direction of this vector
const Vector3D Vector3D::unit()
{
  double r = std::sqrt(x1*x1+x2*x2+x3*x3);
  if (r==0) throw MathException("Cannot produced unit vector from null vector",__FUNCTION__);
  return Vector3D(x1/r,x2/r,x3/r);
}

//! Active rotation of this Vector3D by the given angle relative to the x-axis
void Vector3D::rotateX(const double & angle)
{
  double s = std::sin(angle);
  double c = std::cos(angle);
  double y = x2;
  double z = x3;
  x2 = c*y - s*z;
  x3 = s*y + c*z;
}

//! Active rotation of this Vector3D by the given angle relative to the y-axis
void Vector3D::rotateY(const double & angle)
{
  double s = std::sin(angle);
  double c = std::cos(angle);
  double x = x1;
  double z = x3;
  x1 = c*x + s*z;
  x3 = c*z - s*x;
}

//! Active rotation of this Vector3D by the given angle relative to the z-axis
void Vector3D::rotateZ(const double & angle)
{
  double s = std::sin(angle);
  double c = std::cos(angle);
  double x = x1;
  double y = x2;
  x1 = c*x - s*y;
  x2 = c*y + s*x;
}

void Vector3D::print() const
{
  std::cout << "(" << x1 << "," << x2 << "," << x3 << ") " << std::endl;
}

//!
//! Add the two vectors and return a new vector
//!
Vector3D operator+ (const Vector3D & left, const Vector3D & right)
{
  return Vector3D(left.x1+right.x1, left.x2+right.x2, left.x3+right.x3);
}

Vector3D operator- (const Vector3D & left, const Vector3D & right)
{
  return Vector3D(left.x1-right.x1, left.x2-right.x2, left.x3-right.x3);
}

//!
//! SR (3D) scalar product of two left and right vectors
//!
double operator* (const Vector3D & left, const Vector3D & right)
  {
  return left.x1*right.x1 + left.x2*right.x2 + left.x3*right.x3;
  }


//!
//! Scaling of the left vector by a scalar value from the right
//!
Vector3D operator* (const Vector3D & v, double a)
  {
  return Vector3D(a*v.x1, a*v.x2, a*v.x3);
  }

//!
//! Scaling of the right vector by a scalar value from the left
//!
Vector3D operator* (double a, const Vector3D & v)
  {
  return Vector3D(a*v.x1, a*v.x2, a*v.x3);
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


