/********************************************************************************
 *                                                                              *
 *             THERMINATOR 2: THERampl heavy-IoN generATOR 2                     *
 *                                                                              *
 * Version:                                                                     *
 *      Release, 2.0.3, 1 February 2011                                         *
 *                                                                              *
 * Authors:                                                                     *
 *      Mikolaj Chojnacki   (Mikolaj.Chojnacki@ifj.edu.pl)                      *
 *      Adam Kisiel         (kisiel@if.pw.edu.pl)                               *
 *      Wojciech Broniowski (Wojciech.Broniowski@ifj.edu.pl)                    *
 *      Wojciech Florkowski (Wojciech.Florkowski@ifj.edu.pl)                    *
 *                                                                              *
 * Project homepage:                                                            *
 *      http://therminator2.ifj.edu.pl/                                         *
 *                                                                              *
 * For the detailed description of the program and further references           *
 * to the description of the model please refer to                              *
 * http://arxiv.org/abs/1102.0273                                               *
 *                                                                              *
 * This code can be freely used and redistributed. However if you decide to     *
 * ampke modifications to the code, please, inform the authors.                  *
 * Any publication of results obtained using this code must include the         *
 * reference to arXiv:1102.0273 and the published version of it, when           *
 * available.                                                                   *
 *                                                                              *
 ********************************************************************************/
#include "Viscosity.hpp"

ClassImp(CAP::Viscosity);

namespace CAP
{

  Viscosity::Viscosity()
  :
  MATH::ScalarField()
  {  }

  double Viscosity::shearPPdotPiAt(double aX, double aY, double aZ, double aPe, double aPx, double aPy, double aPz) const
  {
  return (
          aPe*aPe * _shear[i00].interpolate(aX,aY,aZ)
          + aPx*aPx * _shear[iXX].interpolate(aX,aY,aZ)
          + aPy*aPy * _shear[iYY].interpolate(aX,aY,aZ)
          + aPz*aPz * _shear[iZZ].interpolate(aX,aY,aZ)
          + 2.0 * (
                   aPx*aPy * _shear[iXY].interpolate(aX,aY,aZ)
                   + aPx*aPz * _shear[iXZ].interpolate(aX,aY,aZ)
                   + aPy*aPz * _shear[iYZ].interpolate(aX,aY,aZ)
                   ) - 2.0 * aPe * (
                                    aPx * _shear[i0X].interpolate(aX,aY,aZ)
                                    + aPy * _shear[i0Y].interpolate(aX,aY,aZ)
                                    + aPz * _shear[i0Z].interpolate(aX,aY,aZ)
                                    )
          );
  }

  void Viscosity::setBulkTau(double value)
  {
  _type = 0 | 0x01;
  setValue(value);
  }

  void Viscosity::setBulkPi(MATH::ScalarField  & value)
  {
  _type = _type | 0x01; // this makes no sense
  //constValue = 1.0E-100;
  setValue(value);
  }

  void Viscosity::setShearPi(eViscPi aIndex, MATH::ScalarField  & aShear)
  {
  _type = _type | 0x02;
  _shear[aIndex] = aShear;
  }
  
} // namespace CAP
