#include "GlauberExceptions.hpp"
#include "MathConstants.hpp"
#include "NNProfile.hpp"
#include "PrintHelpers.hpp"
#include "TMath.h"
using namespace std;
using CAP::MATH::pi;
using CAP::MATH::twoPi;
using CAP::MATH::sqrtTwoPi;

namespace CAP
{

  NNProfile::NNProfile()
  :
  _xSection(0),
  _omega(0),
  _g(0),
  _r2(0.0),
  _omegaInv(0.0),
  _gOverOmegaR2(0.0)
  {
  setParameters(67.6,0.4,1.0);
  }

  NNProfile::NNProfile(const NNProfile & src)
  :
  _xSection(src._xSection),
  _omega(src._omega),
  _g(src._g),
  _r(src._r),
  _r2(src._r2),
  _omegaInv(src._omegaInv),
  _gOverOmegaR2(src._gOverOmegaR2)
  {}

  NNProfile & NNProfile::operator=(const NNProfile & rhs)
  {
  if (this!=&rhs)
    {
    _xSection = rhs._xSection;
    _omega    = rhs._omega;
    _g        = rhs._g;
    _r        = rhs._r;
    _r2       = rhs._r2;
    _omegaInv = rhs._omegaInv;
    _gOverOmegaR2 = rhs._gOverOmegaR2;
    }
  return *this;
  }


  double NNProfile::evaluate(double x)
  {
//  printValue("NNProfile::evaluate(double x) at",x);
//  printValue("NNProfile::evaluate(double x) _omega",_omega);
//  printValue("NNProfile::evaluate(double x) _r2",_r2);
//  printValue("NNProfile::evaluate(double x) _g",_g);
//  printValue("NNProfile::evaluate(double x) _omegaInv",_omegaInv);
//  printValue("NNProfile::evaluate(double x) _gOverOmegaR2",_gOverOmegaR2);
  if (x<0) throw InvalidValueGlauberException(x,"Invalid value : x<0",__FUNCTION__);
  if (_omega==0) // in this limit _r is the radius of the hard sphere
    return (x<=_r) ? 1.0 : 0.0;
  else if (_omega==1) // in this limit a Gaussian dist is used _r2 = r*r
    {
    //printValue("x",x);
    double arg = x/_r;
    double p = std::exp(-0.5*arg*arg)/CAP::MATH::sqrtTwoPi()/_r;
//    printValue("x",x);
//    printValue("p",p);
    return p;
    }
  else // otherwise use the Gamma formula from RybczyRybczynski et al.
    return _g*(1-TMath::Gamma(_omegaInv,_gOverOmegaR2*x*x));
  }

  //!
  //! Reset this instance to the default parameter values
  //!
  void NNProfile::reset()
  {
  setParameters(67.6,0.4,1.0);
  }

  //! xSection : nucleon nucleon inel cross section (mb)
  //! omega: shape of the NN profile
  //! g: normalization
  void NNProfile::setParameters(double xSection,
                                double omega,
                                double g)
  {
  _xSection = xSection;
  _r2 = _xSection/(10.0*pi());
  _r  = std::sqrt(_r2);
  if ((omega<0) || (omega>1))
    throw InvalidValueGlauberException(omega,"Must have 0.0<omega<=1.0",__FUNCTION__);
  _omega = omega;
  _g = g;
  if (_omega>0)
    {
    _omegaInv = 1.0/_omega;
    _gOverOmegaR2 = _g*_omegaInv/_r2;
    }
  else  // limit omega==0
    {
    _omegaInv = 0.0; // do not use
    _gOverOmegaR2 = 0.0; // do not use
    }
  }

}  // namespace CAP



