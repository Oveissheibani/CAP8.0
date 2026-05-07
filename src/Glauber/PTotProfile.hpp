#ifndef CAP__PTotProfile
#define CAP__PTotProfile
#include "MathConstants.hpp"
using namespace std;
using CAP::MATH::pi;
using CAP::MATH::twoPi;

namespace CAP
{

  class PTotProfile
  {
  public:

  PTotProfile();
  PTotProfile(const PTotProfile & src);
  PTotProfile & operator=(const PTotProfile & rhs);
  virtual ~PTotProfile(){}
  virtual double evaluate(double x);
  double random();
  void setParameters(double nnXSect,
                     double nnXSectOmega,
                     double nnXSectLambda);
  double nnXSection() { return _nnXSect; }
  double nnXSectionOmega() { return _nnXSectOmega; }
  double nnXSectionLambda() { return _nnXSectLambda; }
  virtual void reset();

  protected:

  double _nnXSect;
  double _nnXSectOmega;
  double _nnXSectLambda;
  };

} // namespace CAP

#endif



