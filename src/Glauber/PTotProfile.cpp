#include "GlauberExceptions.hpp"
#include "PTotProfile.hpp"

namespace CAP
{
  PTotProfile::PTotProfile()
  :
  _nnXSect(0),
  _nnXSectOmega(0),
  _nnXSectLambda(0)
  { }

  PTotProfile::PTotProfile(const PTotProfile & src)
  :
  _nnXSect(src._nnXSect),
  _nnXSectOmega(src._nnXSectOmega),
  _nnXSectLambda(src._nnXSectLambda)
  { }

  PTotProfile & PTotProfile::operator=(const PTotProfile & rhs)
  {
  if (this!=&rhs)
    {
    _nnXSect       = rhs._nnXSect;
    _nnXSectOmega  = rhs._nnXSectOmega;
    _nnXSectLambda = rhs._nnXSectLambda;
    }
  return *this;
  }

  void PTotProfile::setParameters(double nnXSect,
                                  double nnXSectOmega,
                                  double nnXSectLambda)
  {
  _nnXSect       = nnXSect;
  _nnXSectOmega  = nnXSectOmega;
  _nnXSectLambda = nnXSectLambda;
  }

  //! From Loizides
  //! f == ((x/[2])/(x/[2]+[0]))*exp(-(((x/[2])/[0]-1 )**2)/([1]*[1]))/[2]
  //!
  //!  ((x/sigma)/(x/sigma+[0]))*exp(-(((x/sigma)/[0]-1 )**2)/([1]*[1]))/[2]
  //!
  //!  let arg = x/sigma
  //!  (arg/(arg+[0]))*exp(-((arg/[0]-1)**2)/([1]*[1]))/[2]
  //! [0] _nnXSect,
  //! [1] _nnXSectOmega,
  //! [2] _nnXSectLambda
  //!
  double PTotProfile::evaluate(double x)
  {

  double xr = x/_nnXSectLambda;
  double ss = _nnXSectOmega*_nnXSectOmega;
  double aa = xr/_nnXSect-1.0;
  return (xr/(xr+_nnXSect))*std::exp(-aa*aa/ss)/_nnXSectLambda;
  }

  double PTotProfile::random()
  {
  return 0; // fix me....
  }

  void PTotProfile::reset()
  {
  _nnXSect = 0;
  _nnXSectOmega = 0;
  _nnXSectLambda = 0;
  }
  
}  // namespace CAP



