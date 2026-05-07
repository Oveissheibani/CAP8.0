#ifndef CAP_CHEMISTRY_H_
#define CAP_CHEMISTRY_H_
#include "ScalarField.hpp"
#include "ParticleType.hpp"

using CAP::MATH::ScalarField;

namespace CAP
{

  class Chemistry
  {
  public:
  Chemistry();
  Chemistry(const Chemistry & source);
  Chemistry operator=(const Chemistry & rhs);
  virtual ~Chemistry() {}

  int    chemistryType() const { return _type; }
  double muB() const  { return _muBField.value(); }
  double muI() const  { return _muIField.value(); }
  double muS() const  { return _muSField.value(); }
  double muC() const  { return _muCField.value(); }
  double lambdaQ() const { return _lambdaQField.value(); }
  double lambdaI() const { return _lambdaIField.value(); }
  double lambdaS() const { return _lambdaSField.value(); }
  double lambdaC() const { return _lambdaCField.value(); }
  double gammaQ() const  { return _gammaQField.value(); }
  double gammaS() const  { return _gammaSField.value(); }
  double gammaC() const  { return _gammaCField.value(); }

  double chemicalPotential(ParticleType & type) const;
  double chemicalPotentialAt(ParticleType & type, double aX, double aY, double aZ) const;
  double fugacity(ParticleType & aPartType) const;
  double fugacityAt(ParticleType & aPartType, double aX, double aY, double aZ) const;

  double invFugacityFactor(ParticleType & t, double temperature) const;
  double invFugacityFactorAt(ParticleType & t,
                             double aX,
                             double aY,
                             double aZ,
                             double temperature) const;

  // set uniform field values

  void setMuB(double value) { _muBField.setValue(value); }
  void setMuI(double value) { _muIField.setValue(value); }
  void setMuS(double value) { _muSField.setValue(value); }
  void setMuC(double value) { _muCField.setValue(value); }

  void setGammaQ(double value) { _gammaQField.setValue(value); }
  void setGammaS(double value) { _gammaSField.setValue(value); }
  void setGammaC(double value) { _gammaCField.setValue(value); }

  void setLambdaI(double value) { _lambdaIField.setValue(value); }
  void setLambdaQ(double value) { _lambdaQField.setValue(value); }
  void setLambdaS(double value) { _lambdaSField.setValue(value); }
  void setLambdaC(double value) { _lambdaCField.setValue(value); }

  // set field
  void setMuBField(const ScalarField & value) { _muBField.setValue(value); }
  void setMuIField(const ScalarField &  value) { _muIField.setValue(value); }
  void setMuSField(const ScalarField &  value) { _muSField.setValue(value); }
  void setMuCField(const ScalarField &  value) { _muCField.setValue(value); }

  void setGammaQField(const ScalarField &  value) { _gammaQField.setValue(value); }
  void setGammaSField(const ScalarField &  value) { _gammaSField.setValue(value); }
  void setGammaCField(const ScalarField &  value) { _gammaCField.setValue(value); }

  void setLambdaIField(const ScalarField &  value) { _lambdaIField.setValue(value); }
  void setLambdaQField(const ScalarField &  value) { _lambdaQField.setValue(value); }
  void setLambdaSField(const ScalarField &  value) { _lambdaSField.setValue(value); }
  void setLambdaCField(const ScalarField &  value) { _lambdaCField.setValue(value); }

  void setChemistry(double aMuB,
                    double aMuI,
                    double aMuS,
                    double aMuC);
  void setChemistry(const ScalarField & _muB,
                    const ScalarField & _muI,
                    const ScalarField & _muS,
                    const ScalarField & _muC);
  void setChemistry(double aLambdaQ,
                    double aLambdaI,
                    double aLambdaS,
                    double aLambdaC,
                    double aGammaQ,
                    double aGammaS,
                    double aGammaC);

  void setChemistry(const ScalarField & lambdaQ,
                    const ScalarField & lambdaI,
                    const ScalarField & lambdaS,
                    const ScalarField & lambdaC,
                    const ScalarField & gammaQ,
                    const ScalarField & gammaS,
                    const ScalarField & gammaC);

  protected:

  int      _type;
  ScalarField  _muBField;
  ScalarField  _muIField;
  ScalarField  _muSField;
  ScalarField  _muCField;
  ScalarField  _gammaQField;
  ScalarField   _gammaSField;
  ScalarField   _gammaCField;
  ScalarField   _lambdaIField;
  ScalarField   _lambdaQField;
  ScalarField   _lambdaSField;
  ScalarField   _lambdaCField;


  ClassDef(Chemistry,0)
  };

}  //namespace CAP

#endif // !CAP_CHEMISTRY_H_
