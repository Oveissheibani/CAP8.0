#include "PrintHelpers.hpp"
#include "Thermodynamics.hpp"
#include "TMath.h"

ClassImp(CAP::Thermodynamics);

namespace CAP
{

  Thermodynamics::Thermodynamics()
  :
  Chemistry(),
  _energy(),
  _entropy(),
  _pressure(),
  _soundVelocity(),
  _temperature(),
  _viscosity()
  {  }

  double Thermodynamics::invFugacityFactor(ParticleType & particleType) const
  {
  switch (chemistryType())
    {
      case 0:  return std::exp( - chemicalPotential(particleType) / _temperature.value() );
      case 2:  return 1.0/fugacity(particleType);
      default: return 1.0;
    }
  }

  double Thermodynamics::invFugacityFactorAt(ParticleType & particleType, double aX, double aY, double aZ) const
  {
  switch (chemistryType()) {
    case 1:  return std::exp( - chemicalPotentialAt(particleType, aX, aY, aZ) / _temperature.valueAt(aX, aY, aZ) );
    case 3:  return 1.0 / fugacityAt(particleType, aX, aY, aZ);
    default: return 1.0;
  }
  }

  // fix me
  double Thermodynamics::viscosityCorrectionAt(double aX __attribute__((unused)),
                                               double aY __attribute__((unused)),
                                               double aZ __attribute__((unused)),
                                               double aPe __attribute__((unused)),
                                               double aPx __attribute__((unused)),
                                               double aPy __attribute__((unused)),
                                               double aPz __attribute__((unused)),
                                               double aPdotU __attribute__((unused))) const
  {
  //double tmass2; // fix me
  //double tTemp;
//  double tCs2;
//  double tBulkPart  = 0.0;
//  double tShearPart = 0.0;
//  double dummy;
  //tmass2 = aPe*aPe - aPx*aPx - aPy*aPy - aPz*aPz;

//  switch (_temperature.type())
//    {
//      case 0:
//      tTemp = _temperature.value();
//      break;
//      case 1:
//      tTemp = _temperature.valueAt(aX, aY, aZ);
//      break;
//    }
//  switch (_soundVelocity.type())
//    {
//      case 0:
//      dummy = _soundVelocity.value();
//      tCs2 = dummy*dummy;
//      break;
//      case 1:
//      dummy = _soundVelocity.valueAt(aX, aY, aZ);
//      tCs2= dummy*dummy;
//      break;
//    }

//  switch (_viscosity.type()) {
//    case 0:
//      tShearPart  = 1.0 / (2.0*tTemp*tTemp) * _viscosity.shearPPdotPi(aX,aY,aZ,aPe,aPx,aPy,aPz);
//      break;
//    case 1:
//      tBulkPart = ( tCs2*aPdotU + (tmass2 - aPdotU*aPdotU) / (3.0*aPdotU) ) * _viscosity.bulkTau () * _viscosity.bulkPi(aX,aY,aZ) / tTemp;
//      break;
//    case 2:
//      tShearPart  = 1.0 / (2.0*tTemp*tTemp) * _viscosity.shearPPdotPi(aX,aY,aZ,aPe,aPx,aPy,aPz);
//      tBulkPart = ( tCs2*aPdotU + (tmass2 - aPdotU*aPdotU) / (3.0*aPdotU) ) * _viscosity.bulkTau () * _viscosity.bulkPi(aX,aY,aZ) / tTemp;
//      break;
//  }
  return 0; //(tBulkPart + tShearPart); // fix me
  }
}  //namespace CAP

