#include "ModelKrakowSFO.hpp"

ClassImp(CAP::ModelKrakowSFO);

using CAP::MATH::twoPi;
using CAP::MATH::twoPiCube;

namespace CAP
{
  ModelKrakowSFO::ModelKrakowSFO()
  :
  Model()
  {    }

  double ModelKrakowSFO::calculateIntegrand(ThermalHadron & hadron)
  {
  // Type of _statistics Bose-Einstein or Fermi-Dirac
  ParticleType & type = hadron.type();
  double sf    = type.spinFactor();
  double stat  = type.statistics();
  double mass  = type.mass();
  double mass2 = mass*mass;
  double chemPotential = chemicalPotential(type);

  double rho        = _rhoMax * gRandom->Rndm();
  double sPhi       = MATH::twoPi() * gRandom->Rndm();
  double sRapidity  = _sRapidityRange * (gRandom->Rndm() - 0.5);
  double tau        = _tauC; // that is the KrakowSFO Model _tau  that equals $\_tau _{KrakowSFO}^2 = t^2 - x^2 - y^2 - z^2 = \_tau ^2 - \_rho^2 - z^2$
  double tauTrue    =  sqrt(tau*tau  + rho*rho);  // that's the true $\_tau ^2 = t^2 - z^2$
  _position.setTXYZ(tauTrue*cosh(sRapidity),rho*cos(sPhi),rho*sin(sPhi),tauTrue*sinh(sRapidity));

  double zeta      = gRandom->Rndm();
  double zetac     = (zeta>0.9999999) ? 0.00000001 : 1.00-zeta;
  double pT        = zeta/zetac;
  double dPt       = 1.0/(zetac*zetac);
  double pPhi      = twoPi() * gRandom->Rndm();
  double pRapidity = _pRapidityRange * (gRandom->Rndm() - 0.5);
  double mT        = sqrt(mass2+pT*pT);
  double e         = mT * cosh(pRapidity);
  _momentum.setTXYZ(e,pT*cos(pPhi),pT*sin(pPhi),mT*sinh(pRapidity));

  double pDotU   = (mT*tauTrue*cosh(sRapidity-pRapidity)-pT*rho*cos(sPhi-pPhi))/tau;
  double dSigmaP = tau*rho*pDotU;
  double denom   = stat + exp( (pDotU-chemPotential)/_temperature.value() );
  double integrand = sf*pT*dPt*dSigmaP/(denom*twoPiCube());
  hadron.setIntegrand(integrand);
  return integrand;  }

} // namespace CAP
