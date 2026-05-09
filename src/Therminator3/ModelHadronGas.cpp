#include "ModelHadronGas.hpp"
#include "MathConstants.hpp"

ClassImp(CAP::ModelHadronGas);
using CAP::MATH::twoPiCube;
using CAP::MATH::twoPi;

namespace CAP
{

  ModelHadronGas::ModelHadronGas()
  :
  Model()
  {   }

  double ModelHadronGas::calculateIntegrand(ThermalHadron & hadron)
  {
  // Type of _statistics Bose-Einstein or Fermi-Dirac
  ParticleType & type = hadron.type();
  double sf    = type.spinFactor();
  double stat  = type.statistics();
  double mass  = type.mass();
  double mass2 = mass*mass;
  double chemPotential = chemicalPotential(type);

  double rho        = _rhoMax * gRandom->Rndm();
  double sPhi       = twoPi() * gRandom->Rndm();
  double sRapidity  = _sRapidityRange * (gRandom->Rndm() - 0.5);
  double tau        = _tauI;
  _position.setTXYZ(tau*std::cosh(sRapidity),rho*std::cos(sPhi),rho*std::sin(sPhi),tau*std::sinh(sRapidity));

  double zeta      = gRandom->Rndm();
  double zetac     = (zeta>0.9999999) ? 0.00000001 : 1.00-zeta;
  double pT        = zeta/zetac;
  double dPt       = 1.0/(zetac*zetac);
  double pPhi      = twoPi() * gRandom->Rndm();
  double pRapidity = _pRapidityRange * (gRandom->Rndm() - 0.5);
  double mT        = std::sqrt(mass2+pT*pT);
  double e         = mT * std::cosh(pRapidity);
  _momentum.setTXYZ(e,pT*std::cos(pPhi),pT*std::sin(pPhi),mT*std::sinh(pRapidity));

  double pDotU   = 1.0;
  double dSigmaP = 1.0;
  double denom = stat + std::exp( (pDotU-chemPotential)/_temperature.value() );
  double integrand = sf*pT*dPt*dSigmaP/(denom*twoPiCube());
  hadron.setIntegrand(integrand);
  return integrand;
  }

} // namespace CAP
