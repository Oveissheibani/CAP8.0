#include "GlauberExceptions.hpp"
#include "NucleonProfile.hpp"
using namespace std;
using CAP::MATH::pi;
using CAP::MATH::twoPi;

namespace CAP
{
  NucleonProfile::NucleonProfile()
  :
  _nucleonRadius(0)
  { }

  NucleonProfile::NucleonProfile(const NucleonProfile & src)
  :
  _nucleonRadius(src._nucleonRadius)
  { }


  NucleonProfile & NucleonProfile::operator=(const NucleonProfile & rhs)
  {
  if (this!=&rhs)
    {
    _nucleonRadius = rhs._nucleonRadius;
    }
  return *this;
  }

  void NucleonProfile::setNucleonRadius(double value)
  {
  if (value<=0.0)
    throw GlauberException("nucleonRadius<=0",__FUNCTION__);
  _nucleonRadius = value;
  }

  double NucleonProfile::evaluate(double x)
  {
  return x*x*exp(-x/_nucleonRadius);
  }

}  // namespace CAP



