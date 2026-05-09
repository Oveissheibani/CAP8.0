#ifndef CAP__MODEL
#define CAP__MODEL
#include "Aliases.hpp"
#include "Event.hpp"
#include "EventFilter.hpp"
#include "Hypersurface.hpp"
#include "ModelSettings.hpp"
#include "ParticleFilter.hpp"
#include "ScalarField.hpp"
#include "ThermalHadronGas.hpp"
#include "Thermodynamics.hpp"
#include "VectorLorentz.hpp"
//#include <iostream>

using CAP::MATH::ScalarField;
using CAP::MATH::VectorLorentz;


namespace CAP
{

  class Model
  :
  public ModelSettings,
  public Thermodynamics,
  public Hypersurface
  {
  protected:
  ScalarField _distanceField;
  ScalarField _distanceFieldDPhi;
  ScalarField _distanceFieldDZeta;
  VectorLorentz _position;
  VectorLorentz _momentum;
  ThermalHadronGas _thermalHadronGas;

  public:
  Model();
  virtual ~Model() {};
  virtual void configure(const String & taskName,
                         const String & objectType,
                         const Configuration & configuration);

  virtual void loadFrom(TFile & file __attribute__((unused))) {}
  virtual void loadFromAscii(std::ifstream & file __attribute__((unused))) {}
  virtual void loadFromAscii2(std::ifstream & file1 __attribute__((unused)),
                              std::ifstream & file2 __attribute__((unused))) {}
  virtual void saveTo(TFile & file __attribute__((unused))) {}
  virtual void saveToAscii(std::ofstream & file __attribute__((unused))){}
  virtual void saveToAscii2(std::ofstream & file1 __attribute__((unused)),
                            std::ofstream & file2 __attribute__((unused))) {}

  virtual void print() const;

  virtual void calculateMultiplicities();
  virtual double calculateIntegrand(ThermalHadron& hadron);
  virtual void generateEvent(Event & event,const ParticleFilter & particleFilter);

  inline double gammaAtBeta(double beta)
  {
  return 1.0/std::sqrt(1-beta*beta);
  }

  inline double gammaAtBetaSq(double betaSq)
  {
  return 1.0/std::sqrt(1-betaSq);
  }

  inline double gammaAtV(double v)
  {
  return gammaAtBeta(v/lightVelocitySq_SI());
  }

  inline double gammaAtVSq(double vSq)
  {
  return gammaAtBetaSq(vSq/lightVelocitySq_SI());
  }

  ClassDef(Model,0)
  };

} // namespace CAP

#endif // !CAP__ModelH_
