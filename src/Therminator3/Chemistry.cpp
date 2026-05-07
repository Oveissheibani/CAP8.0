#include "Chemistry.hpp"
#include "MathBasicFunctions.hpp"

//using Tpower;

ClassImp(CAP::Chemistry);

using CAP::MATH::power;
using CAP::MATH::ScalarField;

namespace CAP
{

  Chemistry::Chemistry()
  :
  _type (-1),
  _muBField("B",0.0),
  _muIField("I",0.0),
  _muSField("S",0.0),
  _muCField("C",0.0),
  _gammaQField("_gammaQ",0.0),
  _gammaSField("_gammaS",0.0),
  _gammaCField("_gammaC",0.0),
  _lambdaIField("_lambdaI",0.0),
  _lambdaQField("_lambdaQ",0.0),
  _lambdaSField("_lambdaS",0.0),
  _lambdaCField("_lambdaC",0.0)
  { }

  Chemistry::Chemistry(const Chemistry & source)
  :
  _type (source._type ),
  _muBField(source._muBField),
  _muIField(source._muIField),
  _muSField(source._muSField),
  _muCField(source._muCField),
  _gammaQField(source._gammaQField),
  _gammaSField(source._gammaSField),
  _gammaCField(source._gammaCField),
  _lambdaIField(source._lambdaIField),
  _lambdaQField(source._lambdaQField),
  _lambdaSField(source._lambdaSField),
  _lambdaCField(source._lambdaCField)
  {  }

  Chemistry Chemistry::operator=(const Chemistry & rhs)
  {
  if (this!=&rhs)
    {
    _type   = rhs._type ;
    _muBField     = rhs._muBField;
    _muIField     = rhs._muIField;
    _muSField     = rhs._muSField;
    _muCField     = rhs._muCField;
    _gammaQField  = rhs._gammaQField;
    _gammaSField  = rhs._gammaSField;
    _gammaCField  = rhs._gammaCField;
    _lambdaIField = rhs._lambdaIField;
    _lambdaQField = rhs._lambdaQField;
    _lambdaSField = rhs._lambdaSField;
    _lambdaCField = rhs._lambdaCField;
    }
  return *this;
  }

  double Chemistry::chemicalPotential(ParticleType & t) const
  {
  double muB = _muBField.value();
  double muI = _muIField.value();
  double muS = _muSField.value();
  double muC = _muCField.value();
  return (t.baryonNumber()*muB +
          t.isospin3()*muI +
          t.strangeness()*muS +
          t.charm()*muC );
  }

  double Chemistry::chemicalPotentialAt(ParticleType & t, double aX, double aY, double aZ) const
  {
  double muB = _muBField.interpolate(aX, aY, aZ);
  double muI = _muIField.interpolate(aX, aY, aZ);
  double muS = _muSField.interpolate(aX, aY, aZ);
  double muC = _muCField.interpolate(aX, aY, aZ);
  return (t.baryonNumber()*muB +
          t.isospin3()*muI +
          t.strangeness()*muS +
          t.charm()*muC );
  }

  double Chemistry::fugacity(ParticleType & t) const
  {
  double  gammaQ  = _gammaQField.value();
  double  gammaS  = _gammaSField.value();
  double  gammaC  = _gammaCField.value();
  double  lambdaI = _lambdaIField.value();
  double  lambdaQ = _lambdaQField.value();
  double  lambdaS = _lambdaSField.value();
  double  lambdaC = _lambdaCField.value();
  int nq = t.charge();
  int ns = t.strangeness();
  int nc = t.charm();
  return lambdaI
  * power(gammaQ,nq) * power(lambdaQ,nq)
  * power(gammaS,ns) * power(lambdaS,ns)
  * power(gammaC,nc) * power(lambdaC,nc);
  }

  double Chemistry::fugacityAt(ParticleType & t, double aX, double aY, double aZ) const
  {
  double  gammaQ  = _gammaQField.interpolate(aX, aY, aZ);
  double  gammaS  = _gammaSField.interpolate(aX, aY, aZ);
  double  gammaC  = _gammaCField.interpolate(aX, aY, aZ);
  double  lambdaI = _lambdaIField.interpolate(aX, aY, aZ);
  double  lambdaQ = _lambdaQField.interpolate(aX, aY, aZ);
  double  lambdaS = _lambdaSField.interpolate(aX, aY, aZ);
  double  lambdaC = _lambdaCField.interpolate(aX, aY, aZ);
  int nq = t.charge();
  int ns = t.strangeness();
  int nc = t.charm();
  return lambdaI
  * power(gammaQ,nq) * power(lambdaQ,nq)
  * power(gammaS,ns) * power(lambdaS,ns)
  * power(gammaC,nc) * power(lambdaC,nc);
  }

  double Chemistry::invFugacityFactor(ParticleType & t, double temperature) const
  {
  switch (_type)
    {
      case 0:  return exp(-chemicalPotential(t) / temperature);
      case 2:  return 1.0/fugacity(t);
      default: return 1.0;
    }
  }

  double Chemistry::invFugacityFactorAt(ParticleType & t,
                                        double aX,
                                        double aY,
                                        double aZ,
                                        double temperature) const
  {
  switch (_type)
    {
      case 1:  return exp(- chemicalPotentialAt(t,aX,aY,aZ)/temperature);
      case 3:  return 1.0/fugacityAt(t,aX,aY,aZ);
      default: return 1.0;
    }
  }

  void Chemistry::setChemistry(double muB,
                               double muI,
                               double muS,
                               double muC)
  {
  _type   = 0;
  _muBField.setValue(muB);
  _muIField.setValue(muI);
  _muSField.setValue(muS);
  _muCField.setValue(muC);
  }

  void Chemistry::setChemistry(const ScalarField & muBField,
                               const ScalarField & muIField,
                               const ScalarField & muSField,
                               const ScalarField & muCField)
  {
  _type   = 1;
  _muBField = muBField;
  _muIField = muIField;
  _muSField = muSField;
  _muCField = muCField;
  }

  void Chemistry::setChemistry(double lambdaQ,
                               double lambdaI,
                               double lambdaS,
                               double lambdaC,
                               double gammaQ,
                               double gammaS,
                               double gammaC)
  {
  _type   = 2;
  _gammaQField.setValue(gammaQ);
  _gammaSField.setValue(gammaS);
  _gammaCField.setValue(gammaC);
  _lambdaIField.setValue(lambdaI);
  _lambdaQField.setValue(lambdaQ);
  _lambdaSField.setValue(lambdaS);
  _lambdaCField.setValue(lambdaC);
  }

  void Chemistry::setChemistry(const ScalarField  & lambdaQ,
                               const ScalarField  & lambdaI,
                               const ScalarField  & lambdaS,
                               const ScalarField  & lambdaC,
                               const ScalarField  & gammaQ,
                               const ScalarField  & gammaS,
                               const ScalarField  & gammaC)
  {
  _type   = 3;
  _gammaQField  = gammaQ;
  _gammaSField  = gammaS;
  _gammaCField  = gammaC;
  _lambdaIField = lambdaI;
  _lambdaQField = lambdaQ;
  _lambdaSField = lambdaS;
  _lambdaCField = lambdaC;
  }

} // namespace CAP
