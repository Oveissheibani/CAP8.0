#include "GlauberExceptions.hpp"
#include "MathBasicFunctions.hpp"
#include "MathConstants.hpp"
#include "NNXSectVsSqrtS.hpp"
using namespace std;
using CAP::MATH::pi;
using CAP::MATH::twoPi;
using CAP::MATH::absolute;
using CAP::MATH::power;

namespace CAP
{
  //!
  //! nn_xSection vs sqrt{s}
  //! _sqrtS [GeV]
  //! nnXset [mb]
  //!
  //! From fits published in
  //! Improved Monte Carlo Glauber predictions at present and future nuclear colliders
  //! Constantin Loizides (ORNL), Jason Kamin (UiC), David d'Enterria (CERN)
  //! Phys.Rev. C97 (2018) 054910; Erratum: Phys.Rev. C99 (2019) 019901
  //! Parameters are from Table 1.
  //!
  //! s_nn = A + B log^n(s)
  //!
  //! Type    |    A                  |          B                 |     n                  |   chi^2/ndf
  //! ------------------------------------------------
  //! s_nn = A + B log^n(s)
  //! ln(s)     | -3.33 +- 1/58  |  4.195 +- 0.103    |     1  (fixed)      |   1/.52
  //! ln^2(s) | 25.0 +- 0.9      |  0.146 +- 0.004    |     2  (fixed)      | 0.97
  //! ln^n(s) | 29.8 +- 4.7      |  0.038 +- 0.060     |    2.43 +- 0.50 | 0.98
  //!
  //!  s_nn  = A+B log^2(s) + C s^{-eta}
  //!  A = 24.4\pm 1.4
  //!  B = 0.01008 +- 0.1537
  //!  C = 0.1.454 +- 1.768
  //!  eta = 0.131 +- 0.0180
  //!  chi^2/ndf  1.09
  //!
  //!  s_nn  A+B log^2(s)+D log(s)
  //!  A = 39.7 +- 1.4
  //!  B = 0.2212 +-0.0708
  //!  D = -2.154 +- 2.035
  //!  chi^2/ndf = 0.96
  //!
  NNXSectVsSqrtS::NNXSectVsSqrtS()
  :
  _option(0),
  _a(0),
  _b(0),
  _c(0),
  _d(0),
  _n(0),
  _eta(0)
  {}

  NNXSectVsSqrtS::NNXSectVsSqrtS(const NNXSectVsSqrtS & src)
  :
  _option(src._option),
  _a(src._a),
  _b(src._b),
  _c(src._c),
  _d(src._d),
  _n(src._n),
  _eta(src._eta)
  {}

  NNXSectVsSqrtS & NNXSectVsSqrtS::operator=(const NNXSectVsSqrtS & rhs)
  {
  if (this!=&rhs)
    {
    _option = rhs._option;
    _a = rhs._a;
    _b = rhs._b;
    _c = rhs._c;
    _d = rhs._d;
    _n = rhs._n;
    _eta = rhs._eta;
    }
  return *this;
  }

  void NNXSectVsSqrtS::reset()
  {
  _option = 0;
  _a = 0.0;
  _b = 0.0;
  _c = 0.0;
  _d = 0.0;
  _n = 0.0;
  _eta = 0.0;
  }


  void NNXSectVsSqrtS::setOption(unsigned int option)
  {
  reset();
  _option = option;
  switch (_option)
    {
      case 1: // exponent n == 1
      {
      _a = -3.33;
      _b = 4.195;
      break;
      }

      case 2: // exponent n == 2
      {
      _a = 25.0;
      _b = 0.146;
      break;
      }

      case 3: // exponent n == 2.43
      {
      _a = 29.8;
      _b = 0.038;
      _n = 2.43;
      break;
      }

      case 4:
      {
      //!  s_nn  = A+B log^2(s) + C s^{-eta}
      //!  A = 24.4\pm 1.4
      //!  B = 0.01008 +- 0.1537
      //!  C = 0.1.454 +- 1.768
      //!  eta = 0.131 +- 0.0180
      //!  chi^2/ndf  1.09
      _a = 24.4;
      _b = 0.01008;
      _c = 1.454;
      _eta = 0.131;
      break;
      }

      case 5:
      {
      //!  s_nn  = A+B log^2(s)+D log(s)
      //!  A = 39.7 +- 1.4
      //!  B = 0.2212 +-0.0708
      //!  D = -2.154 +- 2.035
      _a = 39.7;
      _b = 0.2212;
      _d = -2.154;
      break;
      }

      default:
      throw InvalidValueGlauberException(option,"Invalid Option for calculation of xSect vs s",__FUNCTION__);
    }
  }

  //!
  //!Calculate the NN cross section at the given value of x=sqrt(s)
  //!
  double NNXSectVsSqrtS::evaluate(double x)
  {
  double result = 0;
  switch (_option)
  {
    case 1: // exponent n == 1
    {
    result = _a + _b*log(x*x);
    break;
    }

    case 2: // exponent n == 2
    {
    double z = log(x*x);
    result = _a + _b*z*z;
    break;
    }

    case 3: // exponent n == 2.43
    {
    result = _a + _b*power(log(x*x),_n);
    break;
    }

    case 4: // Compete collaboration
    {
    //!  s_nn  = A+B log^2(s) + C s^{-eta}
    //!  A = 24.4\pm 1.4
    //!  B = 0.01008 +- 0.1537
    //!  C = 0.1.454 +- 1.768
    //!  eta = 0.131 +- 0.0180
    //!  chi^2/ndf  1.09
    double xx = x*x;
    double z = log(xx);
    result = _a + _b*z*z + _c*power(xx,-_eta);
    break;
    }

    case 5: // Compete collaboation
    {
    //!  s_nn  = A+B log^2(s)+D log(s)
    //!  A = 39.7 +- 1.4
    //!  B = 0.2212 +-0.0708
    //!  D = -2.154 +- 2.035
    double xx = x*x;
    double z = log(xx);
    result = _a + _b*z*z + _d*z;
    break;
    }

    default:
    throw InvalidValueGlauberException(_option,"Invalid Option for calculation of xSect vs s",__FUNCTION__);
  }
  return result;
  }


}  // namespace CAP



