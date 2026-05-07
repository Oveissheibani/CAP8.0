#ifndef CAP__NucleonProfile
#define CAP__NucleonProfile
//#include <ostream>
#include "MathConstants.hpp"


namespace CAP
{
  class NucleonProfile
  {
  public:

  NucleonProfile();
  NucleonProfile(const NucleonProfile & src);
  NucleonProfile & operator=(const NucleonProfile & rhs);
  virtual ~NucleonProfile(){}
  virtual double evaluate(double x);
  void setNucleonRadius(double value);
  double nucleonRadius() { return _nucleonRadius; }
  virtual void reset() { _nucleonRadius = 0.0; }

  protected:

  double _nucleonRadius;
  };

} // namespace CAP

#endif // !CAP__NucleonProfile



