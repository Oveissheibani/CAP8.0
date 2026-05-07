#ifndef CAP__SmearingProfile
#define CAP__SmearingProfile
#include "MathConstants.hpp"

namespace CAP
{
  class SmearingProfile
  {
  public:

  SmearingProfile();
  SmearingProfile(const SmearingProfile & src);
  SmearingProfile & operator=(const SmearingProfile & rhs);
  virtual ~SmearingProfile(){}
  virtual double evaluate(double x);

  void setNucleonSigma(double value);
  double nucleonSigma() { return _nucleonSigma; }
  void reset();

  protected:

  double _nucleonSigma;
  double _twoNucleonSigmaSq;
  };

} // namespace CAP

#endif // !CAP__SmearingProfile



