#ifndef CAP_MathConstants
#define CAP_MathConstants
#include <cmath>

//!
//! Compendium of basic mathematical constants
//!
//!
namespace CAP
{

  namespace MATH
  {

  //!
  //! Basic  constants
  //!
  //! \f$ \pi\f$
  constexpr double pi()
  {
  return 3.14159265358979323846;
  }

  //!
  //! \f$ 2\pi\f$
  constexpr double twoPi()
  {
  return 2.0 * pi();
  }

  //!
  //! \f$ 4\pi\f$
  constexpr double fourPi()
  {
  return 4.0 * pi();
  }


  //!
  //! \f$ \frac{\pi}{2} \f$
  constexpr double piOver2()
  {
  return pi() / 2.0;
  }

  //!
  //! \f$ \frac{\pi}{4} \f$
  constexpr double piOver4()
  {
  return pi() / 4.0;
  }

  //!
  //! \f$ \frac{1.}{\pi}\f$
  constexpr double invPi()
  {
  return 1.0 / pi();
  }

  //!
  //! \f$ \pi^2\f$
  constexpr double piSquare()
  {
  return pi() * pi();
  }

  //!
  //! \f$ \pi^3\f$
  constexpr double piCube()
  {
  return piSquare() * pi();
  }

  //!
  //! \f$ (2\pi)^2\f$
  constexpr double twoPiSquare()
  {
  return twoPi()*twoPi();
  }

  //!
  //! \f$ (2\pi)^3\f$
  constexpr double twoPiCube()
  {
  return twoPiSquare()*twoPi();
  }

  //!
  //! \f$ \ln(2\pi)\f$
  constexpr double logTwoPi()
  {
  return 1.8378770664093454835606594728111235279723;
  }



  //!
  //! \f$ \std::sqrt{2\pi}\f$
  constexpr double sqrtTwoPi()
  {
  //  return std::sqrt(twoPi());
  return 2.506628274631000241612355239340104162693;
  }

  //!
  //! \f$1/ \std::sqrt{2\pi}\f$
  constexpr double oneOverSqrtTwoPi()
  {
  //  return 1.0/std::sqrt(twoPi());  this is a test
  return 0.3989422804014327028632180827116826549172;
  }


  //!
  //! Conversion from radian to degree: \f$ \frac{180}{\pi} \f$
  constexpr double radToDeg()
  {
  return 180.0 / pi();
  }

  //!
  //! Conversion from degree to radian: \f$ \frac{\pi}{180} \f$
  constexpr double degToRad()
  {
  return pi() / 180.0;
  }

  //!
  //! \f$ \std::sqrt{2} \f$
  constexpr double sqrt2()
  {
  return 1.4142135623730950488016887242097;
  }

  //!
  //! \f$ \cuberoot{2} \f$
  constexpr double cubeRoot2()
  {
  return 1.25992104989487316476721060728;
  }

  //!
  //! \f$ 1/\std::sqrt{2} \f$
  constexpr double sqrtHalf()
  {
  return 0.707106781186547524401;
  }



  //!
  //! Base of natural std::log: \f$ e \f$
  constexpr double e()
  {
  return 2.71828182845904523536;
  }


  //!
  //! Natural std::log of 10 (to convert std::log to ln)
  constexpr double ln2()
  {
  return 0.69314718055994528623;
  }

  //!
  //! Natural std::log of 10 (to convert std::log to ln)
  constexpr double ln10()
  {
  return 2.30258509299404568402;
  }

  //!
  //! Base-10 std::log of e  (to convert ln to std::log)
  constexpr double logE()
  {
  return 0.43429448190325182765;
  }



  } // Math namespace
} // CAP namespace

#endif
