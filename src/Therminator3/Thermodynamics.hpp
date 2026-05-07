#ifndef CAP__THERMODYNAMICS_H_
#define CAP__THERMODYNAMICS_H_
#include "ScalarField.hpp"
#include "Chemistry.hpp"
#include "Viscosity.hpp"

namespace CAP
{
  class Thermodynamics
  :
  public Chemistry
  {
  public:
  Thermodynamics();
  virtual ~Thermodynamics() {}

  double temperature() { return _temperature.value(); }
  double temperatureAt(double ax, double ay, double az) { return _temperature.valueAt(ax, ay, az); }
  MATH::ScalarField & temperatureField() { return _temperature; }
  const MATH::ScalarField & temperatureField() const { return _temperature; }
  void setTemperature(double value) { _temperature.setValue(value); }
  void setTemperature(const MATH::ScalarField & value){ _temperature.setValue(value); }

  double soundVelocity() { return _soundVelocity.value(); }
  double soundVelocityAt(double ax, double ay, double az) { return _soundVelocity.valueAt(ax, ay, az); }
  MATH::ScalarField & soundVelocityField(){ return _soundVelocity; }
  const MATH::ScalarField & soundVelocityField() const{ return _soundVelocity; }
  void setSoundVelocity(double value) { _soundVelocity.setValue(value); }
  void setSoundVelocity(const MATH::ScalarField & value) { _soundVelocity.setValue(value); }

  double viscosity() { return _viscosity.value(); }
  double viscosityAt(double ax, double ay, double az) { return _viscosity.valueAt(ax, ay, az); }
  MATH::ScalarField & viscosityField(){ return _viscosity; }
  const MATH::ScalarField & viscosityField() const{ return _viscosity; }
  void setViscosity(double value) { _viscosity.setValue(value); }
  void setViscosity(const MATH::ScalarField & value) { _viscosity.setValue(value);}

  double pressure() { return _pressure.value();}
  double pressureAt(double ax, double ay, double az){ return _pressure.valueAt(ax, ay, az); }
  MATH::ScalarField & pressureField(){  return _pressure; }
  const MATH::ScalarField & pressureField() const {  return _pressure; }
  void setPressure(double value) { _pressure.setValue(value); }
  void setPressure(const MATH::ScalarField & value) { _pressure.setValue(value);}

  double entropy(){ return _entropy.value();}
  double entropyAt(double ax, double ay, double az){  return _entropy.valueAt(ax, ay, az); }
  MATH::ScalarField & entropyField() { return _entropy; }
  const MATH::ScalarField & entropyField() const { return _entropy; }
  void setEntropy(double value) { _entropy.setValue(value); }
  void setEntropy(const MATH::ScalarField & value) { _entropy.setValue(value); }

  double energy() { return _energy.value();}
  double energyAt(double ax, double ay, double az) { return _energy.valueAt(ax, ay, az);}
  MATH::ScalarField & energField() { return _energy;}
  const MATH::ScalarField & energField() const { return _energy;}

  double invFugacityFactor(ParticleType & particleType) const;
  double invFugacityFactorAt(ParticleType & particleType, double aX, double aY, double aZ) const;
  double viscosityCorrectionAt(double aX, double aY, double aZ, double aPe, double aPx, double aPy, double aPz, double aPdotU) const;

  protected:

  MATH::ScalarField _energy;
  MATH::ScalarField _entropy;
  MATH::ScalarField _pressure;
  MATH::ScalarField _soundVelocity;
  MATH::ScalarField _temperature;
  Viscosity   _viscosity;

  ClassDef(Thermodynamics,0)
  };


} //namespace CAP


#endif

/*! @file Thermodynamics.h
 * @brief Definition of Thermodynamics class. Contains Temperature, Chemistry classes.
 */
/*! @class Thermodynamics
 * @brief Class inherits from Temperature and Chemistry classes.
 *
 * @fn Thermodynamics::Thermodynamics()
 * @brief Default constructor.
 *
 * @fn Thermodynamics::~Thermodynamics()
 * @brief Destructor.
 *
 * @fn double Thermodynamics::invFugacityFactor(ParticleType* particleType) const
 * @brief Returns the inverse _fugacity factor @f$ \Upsilon^{-1} @f$ for a given ParticleType that depends on the chemistryType().
 *
 * In case of local chemical equilibrium @f$ \Upsilon^{-1} = \exp \left[ - \frac{\mu_B B + \mu_{I_3} I_3 + \mu_S S + \mu_C C}{T}\right] @f$.
 * Otherwise the returned value is just simply @f$ \Upsilon^{-1} = \frac{1}{\Upsilon} @f$, see _fugacity().
 * @param [in] particleType _type of particle
 *
 * @fn double Thermodynamics::invFugacityFactor(ParticleType* particleType, double aX, double aY, double aZ) const
 * @brief Returns the inverse _fugacity factor @f$ \Upsilon^{-1} @f$ for a given ParticleType that depends on the chemistryType().
 *
 * In case of local chemical equilibrium @f$ \Upsilon^{-1} = \exp \left[ - \frac{\mu_B B + \mu_{I_3} I_3 + \mu_S S + \mu_C C}{T}\right] @f$.
 * Otherwise the returned value is just simply @f$ \Upsilon^{-1} = \frac{1}{\Upsilon} @f$, see _fugacity().
 * @param [in] particleType _type of particle
 * @param [in] aX x-coordinate
 * @param [in] aY y-coordinate
 * @param [in] aZ z-coordinate
 *
 * @fn double Thermodynamics::viscosityCorrection(double aX, double aY, double aZ, double aPe, double aPx, double aPy, double aPz, double aPdotU) const
 * @brief Returns the _viscosity correction @f$ \Delta_{vis} @f$ to the distribution function. Type of _viscosity correction terms depend on the viscosityType().
 *
 * Value of _viscosity correction:
 * @f{equation}{
 *   \Delta_{vis} = \Delta_{bulk} + \Delta_{shear} \nonumber
 * @f}
 * where the bulk _viscosity correction term equals
 * @f{equation}{
 *   \Delta_{bulk} = \left( c_s^2 \,\, p_\mu u^\mu + \frac{m^2 - (p_\mu u^\mu)^2}{3 \, p_\mu u^\mu}\right) \frac{\_tau _{bulk}}{T} \frac{\Pi}{\_zeta} \nonumber
 * @f}
 * and the shear _viscosity correction term is equal to
 * @f{equation}{
 *   \Delta_{shear} = \frac{1}{2 T^2}\, p_\mu p_\nu \frac{\pi^{\mu\nu}}{s T} \nonumber
 * @f}
 * The distribution function @f$ f_0 = \left[ \exp \left( \frac{p_\mu u^\mu - \mu_{ch}}{T}\right) \pm 1 \right]^{-1} @f$ with the _viscosity correction is equal to
 * @f{equation}{
 *   f = f_0 \left[ 1 + \left(1 \pm f_0\right) \, \Delta_{vis} \right] \nonumber
 * @f}
 * where the (+) _sign is for the Fermi-Dirac distribution and (-) _sign is for the Bose-Einstein distribution.
 */
