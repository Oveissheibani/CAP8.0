#include "ModelBWA.hpp"

ClassImp(CAP::ModelBWA);

using namespace std;
using CAP::MATH::twoPi;
using CAP::MATH::twoPiCube;

namespace CAP
{

  ModelBWA::ModelBWA()
  :
  Model()
  {    }

  double ModelBWA::calculateIntegrand(ThermalHadron & hadron)
  {
  // Type of _statistics Bose-Einstein or Fermi-Dirac
  ParticleType & type = hadron.type();
  double sf    = type.spinFactor();
  double stat  = type.statistics();
  double mass  = type.mass();
  double mass2 = mass*mass;
  double chemPotential = chemicalPotential(type);

  double rho       = _rhoMax * gRandom->Rndm();
  double sPhi      = twoPi() * gRandom->Rndm();
  double sRapidity = _sRapidityRange * (gRandom->Rndm() - 0.5);;
  double tau       = _tauI + _amp*rho;

  double zeta      = gRandom->Rndm();
  double zetac     = (zeta>0.9999999) ? 0.0000001 : 1.00-zeta;
  double pT        = zeta/zetac;
  double dPt       = 1.0/(zetac*zetac);
  double pPhi      = twoPi() * gRandom->Rndm();
  double pRapidity = _pRapidityRange * (gRandom->Rndm() - 0.5);
  double mT        = sqrt(mass2+pT*pT);
  //double e         = mT*cosh(pRapidity);

  double vT;
  switch (_modelSubtype)
    {
      default:
      case 2:
      case 3:
      vT = _tVelocity; break;
      case 4:
      case 5:
      case 6:
      vT = (rho/_rhoMax)/(_tVelocity+rho/_rhoMax); break;
    }
  // Invariants
  double gammaT     = gammaAtBeta(vT);
  double mTcoshDiff = mT*(cosh(sRapidity-pRapidity));
  double cosDphi    = cos(sPhi-pPhi);

  // Return values
  double Xt = tau * cosh(sRapidity);
  double Xx = rho * cos(sPhi);
  double Xy = rho * sin(sPhi);
  double Xz = tau * sinh(sRapidity);

  double Pe = mT  * cosh(pRapidity);
  double Px = pT  * cos(pPhi);
  double Py = pT  * sin(pPhi);
  double Pz = mT  * sinh(pRapidity);

  switch (_modelSubtype)
    {
      default: break;
      case 3:
      case 5:
      Xt += -_delay * log(gRandom->Rndm()); break;
      case 6:
      double energy = sqrt(mT*mT + Pz*Pz);
      Xt += -_delay * log(gRandom->Rndm());
      Xx += Xt * Px / energy;
      Xy += Xt * Py / energy;
      Xz += Xt * Pz / energy;
      break;
    }
  _position.setTXYZ(Xt,Xx,Xy,Xz);
  _momentum.setTXYZ(Pe,Px,Py,Pz);
  double pDotU   = gammaT*(mTcoshDiff - vT*pT*cosDphi);
  double dSigmaP = tau*rho*(mTcoshDiff - _amp*pT*cosDphi);
  double denom   = stat + exp( (pDotU-chemPotential)/_temperature.value() );
  double integrand = sf*pT*dPt*dSigmaP/(denom*twoPiCube());
  hadron.setIntegrand(integrand);
  return integrand;
  }

} // namespace CAP
