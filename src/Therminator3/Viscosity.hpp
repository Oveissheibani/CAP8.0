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

#ifndef CAP__VISCOSITY_H_
#define CAP__VISCOSITY_H_
#include "ScalarField.hpp"

namespace CAP
{
  enum eViscPi {i00, i0X, i0Y, i0Z,
    iXX, iXY, iXZ,
    iYY, iYZ,
    iZZ};

  class Viscosity : public MATH::ScalarField
  {
  public:
  Viscosity();
  virtual ~Viscosity() {}

  int    viscosityType() const
  {
  return _type;
  }

  double bulkTau() const
  {
  return value();
  }

  double bulkPiAt(double aX, double aY, double aZ) const
  {
  return interpolate(aX,aY,aZ);
  }

  double shearPPdotPiAt(double aX, double aY, double aZ, double aPe, double aPx, double aPy, double aPz) const;

  void   setBulkTau (double atau );
  void   setBulkPi(ScalarField& aBulk);
  void   setShearPi(eViscPi aIndex, ScalarField & aShear);

  protected:
  MATH::ScalarField _shear[10];

  ClassDef(Viscosity,0)
  };

} // namespace CAP
#endif // !CAP__VISCOSITY_H_

/*! @file Viscosity.h
 * @brief Definition of Viscosity class. Contains the bulk @f$ \Pi @f$ and shear _viscosity tensor @f$ \pi^{/mu/nu} @f$.
 * @warning Not finished.
 */
/*! @enum eViscPi
 * @brief Shear _viscosity tensor @f$ \pi^{\mu\nu} @f$ indexes.
 */
/*! @class Viscosity
 * @brief Contains the bulk @f$ \Pi @f$ and shear _viscosity tensor @f$ \pi^{/mu/nu} @f$.
 *
 * @fn Viscosity::Viscosity()
 * @brief Default constructor.
 *
 * @fn Viscosity::~Viscosity()
 * @brief Destructor
 *
 * @fn int Viscosity::viscosityType() const
 * @brief get the _type of _viscosity stored.
 * @retval 1 bulk
 * @retval 2 shear
 * @retval 3 bulk and shear
 * @retval 0 unknown
 *
 * @fn double Viscosity::bulkTau () const
 * @brief Returns the bulk relaxation time @f$ \_tau _{bulk} @f$.
 *
 * @fn double Viscosity::bulkPi(double aX, double aY, double aZ) const
 * @brief Returns the bulk _viscosity ... @f$ \frac{\Pi}{\_zeta} @f$ at the hypersurface.
 * @param [in] aX x-coordinate
 * @param [in] aY y-coordinate
 * @param [in] aZ z-coordinate
 *
 * @fn double Viscosity::shearPPdotPi(double aX, double aY, double aZ, double aPe, double aPx, double aPy, double aPz) const
 * @brief Returns the tensor product @f$ p_\mu p_\nu \frac{\pi^{\mu\nu}}{s T} @f$ at the hypersurface.
 * @param [in] aX x-coordinate
 * @param [in] aY y-coordinate
 * @param [in] aZ z-coordinate
 * @param [in] aPe particle _energy
 * @param [in] aPx particle _momentum, x-coordinate
 * @param [in] aPy particle _momentum, y-coordinate
 * @param [in] aPz particle _momentum, z-coordinate
 *
 * @fn void Viscosity::setBulkTau (double atau )
 * @brief sets the bulk relaxation time @f$ \_tau _{bulk} @f$.
 * @param [in] atau  bulk relaxation time
 *
 * @fn void Viscosity::setBulkPi(ScalarField* aBulk)
 * @brief sets the bulk _viscosity ... @f$ \frac{\Pi}{\_zeta} @f$.
 * @param [in] aBulk
 *
 * @fn void Viscosity::setShearPi(eViscPi aIndex, ScalarField* aShear)
 * @brief sets shear _viscosity tensor @f$ \frac{\pi^{\mu\nu}}{s T} @f$ element given by the index.
 * @param [in] aIndex shear tensor index
 * @param [in] aShear shear tensor element
 *
 */
