#include "MathBasicGeometry.hpp"


template bool CAP::MATH::isInside<double> (double  xp, double  yp, int  np, double  *x, double  *y);
template double CAP::MATH::modulus<double> (const double  v[3]);
template double CAP::MATH::modulusSq<double> (const double  v[3]);
template double CAP::MATH::normalize<double> (double  v[3]);
template double CAP::MATH::scalarProduct<double>    (const double  v1[3], const double  v2[3]);
template double *CAP::MATH::crossProduct<double>    (const double  v1[3], const double  v2[3], double  out[3]);
template double CAP::MATH::normCrossProduct<double> (const double  v1[3], const double  v2[3], double  out[3]);
template double *CAP::MATH::normalToPlane<double>   (const double  v1[3], const double  v2[3], const double  v3[3], double normal[3]);
