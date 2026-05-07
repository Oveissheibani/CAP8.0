#include "ThermalHadron.hpp"
#include "ParticleDecayMode.hpp"
#include "PrintHelpers.hpp"
#include "MathConstants.hpp"
#include <cmath>
#include <TMath.h>
#include "Math/SpecFunc.h"
ClassImp(CAP::ThermalHadron);

namespace CAP
{

  ThermalHadron::ThermalHadron()
  :
  _type(nullptr),
  _nDensity(0),
  _eDensity(0),
  _sDensity(0),
  _pressure(0),
  _densityWFD(0),
  _dPoverTdmu(0),
  _dEoverTdmu(0),
  _dndmu(0),
  _dsdmu(0),
  _deltaN_bulk_term1(0),
  _deltaN_bulk_term2(0),
  _deltaN_qmu_term1(0),
  _deltaN_qmu_term2(0),
  _integrand(0),
  _avgMult(0),
  _mult(0),
  _decayProb(),
  _decayProbPair()
  { }

  ThermalHadron::ThermalHadron(const ThermalHadron & source)
  :
  _type(source._type),
  _nDensity(source._nDensity),
  _eDensity(source._eDensity),
  _sDensity(source._sDensity),
  _pressure(source._pressure),
  _densityWFD(source._densityWFD),
  _dPoverTdmu(source._dPoverTdmu),
  _dEoverTdmu(source._dEoverTdmu),
  _dndmu(source._dndmu),
  _dsdmu(source._dsdmu),
  _deltaN_bulk_term1(source._deltaN_bulk_term1),
  _deltaN_bulk_term2(source._deltaN_bulk_term2),
  _deltaN_qmu_term1(source._deltaN_qmu_term1),
  _deltaN_qmu_term2(source._deltaN_qmu_term2),
  _integrand(source._integrand),
  _avgMult(source._avgMult),
  _mult(source._mult),
  _decayProb(source._decayProb)
  { }

  ThermalHadron ThermalHadron::operator=(const ThermalHadron & rhs)
  {
  if (this!=&rhs)
    {
    _type           = rhs._type;
    _nDensity       = rhs._nDensity;
    _eDensity       = rhs._eDensity;
    _sDensity       = rhs._sDensity;
    _pressure       = rhs._pressure;
    _densityWFD     = rhs._densityWFD;
    _dPoverTdmu     = rhs._dPoverTdmu;
    _dEoverTdmu     = rhs._dEoverTdmu;
    _dndmu          = rhs._dndmu;
    _dsdmu          = rhs._dsdmu;
    _deltaN_bulk_term1 = rhs._deltaN_bulk_term1;
    _deltaN_bulk_term2 = rhs._deltaN_bulk_term2;
    _deltaN_qmu_term1  = rhs._deltaN_qmu_term1;
    _deltaN_qmu_term2  = rhs._deltaN_qmu_term2;
    _integrand      = rhs._integrand;
    _avgMult        = rhs._avgMult;
    _mult           = rhs._mult;
    _decayProb      = rhs._decayProb;
    }
  return *this;
  }

  void ThermalHadron::loadType(ParticleType * type)
  {
  if (!_type) throw Exception("ParticleType * _type=nullptr",__FUNCTION__);
  clear();
  _type  = type;

  }

  void ThermalHadron::reset()
  {
  _nDensity    = 0.0;
  _eDensity    = 0.0;
  _sDensity    = 0.0;
  _pressure    = 0.0;
  _densityWFD  = 0.0;
  _dPoverTdmu  = 0.0;
  _dEoverTdmu  = 0.0;
  _dndmu       = 0.0;
  _dsdmu       = 0.0;
  _deltaN_bulk_term1 = 0.0;
  _deltaN_bulk_term2 = 0.0;
  _deltaN_qmu_term1  = 0.0;
  _deltaN_qmu_term2  = 0.0;
  _integrand   = 0.0;
  _avgMult     = 0.0;
  _mult        = 0.0;
  }

  void ThermalHadron::clear()
  {
  _type  = nullptr;
  _decayProb.clear();
  _decayProbPair.clear();
  reset();
  }


  void ThermalHadron::print()  const
  {
  printValue("Name",_type->name());
  printValue("Title",_type->title());
  printValue("Mass",_type->mass());
  printValue("SpinFactor",_type->spinFactor());
  printValue("Sign",_type->isBaryon());
  printValue("NumberDensity",_nDensity);
  printValue("EnergyDensity",_eDensity);
  printValue("EntropyDensity",_sDensity);
  printValue("PartialPressure",_pressure);
  printValue("BaryonDensity",baryonDensity());
  printValue("ChargeDensity",chargeDensity());
  printValue("StrangeDensity",strangeDensity());
  printValue("CharmDensity",charmDensity());
  printValue("BottomDensity",bottomDensity());
  printValue("TopDensity",topDensity());
  printValue("DensityWFeedDown",_densityWFD);
  printValue("Integrand",_integrand);
  printValue("AvgMultiplicity",_avgMult);
  printValue("Multiplicity",_mult);
  }

  void ThermalHadron::createDecayProbabilityArray(unsigned int n)
  {
  _decayProb.assign(n,0.0);
  _decayProbPair.assign(n*n,0.0);
  }

  void ThermalHadron::setDecayProbability(unsigned int iHadron, double probability)
  {
  unsigned int nHadrons = _decayProb.size();
  //  printValue("setDecayProbability() iHadron",iHadron);
  //  printValue("setDecayProbability() nHadrons",nHadrons);
  if (iHadron>=nHadrons) throw Exception("iHadron>=nHadrons",__FUNCTION__);
  _decayProb[iHadron] = probability;
  }

  void ThermalHadron::setDecayProbabilityPair(unsigned int iHadron1, unsigned int iHadron2, double probability)
  {
  unsigned int nHadrons = _decayProb.size();
  if (iHadron1>=nHadrons || iHadron2>=nHadrons) throw Exception("iHadron1>=nHadrons || iHadron2>=nHadrons",__FUNCTION__);
  _decayProbPair[iHadron1*nHadrons+iHadron2] = probability;
  }

   void ThermalHadron::calculateAllProperties(double temperature,
                                             double muBaryon,
                                             double muS,
                                             double muC,
                                             double muB,
                                             double muT)
  {
  double mass  = _type->mass();
  double mass2 = mass*mass;
  double mass4 = mass2*mass2;
  double gSpin = _type->spinFactor();
  //double sign  = _type->isBaryon() ? -1.0 : 1.0;
  double prefactor = 0.5*gSpin/MATH::piSquare()/hBarC3();
  double mu = muBaryon*_type->baryonNumber() + muS*_type->strangeness() + muC*_type->charm() + muB*_type->bottomNumber() + muT*_type->topNumber();
  double beta      = 1.0/temperature;
  double beta2     = beta*beta;
  double betaMass  = beta*mass;
  //double betaMass2 = betaMass*betaMass;
  double betaMu    = beta*mu;
  double lambda    = exp(betaMu);
  double fugacity = 1.0;
  double theta    = -1.0;

  _nDensity = 0.0;   //! equilibrium contribution
  _eDensity = 0;
  _pressure = 0.0;
  _dndmu    = 0.0;
  _dPoverTdmu  = 0.0;
  _dEoverTdmu  = 0.0;

  double arg, argSq;
  double nn;
  double K_1, K_2;
  for (unsigned int n = 1; n<_truncateOrder; n++)
    {
    nn = n;
    arg   = nn*betaMass;
    argSq = arg*arg;
    K_1   = TMath::BesselK1(arg);
    K_2   = TMath::BesselK(2, arg);
    theta *= -1.0;   //pow(-sign, n-1);
    fugacity *= lambda; // pow(lambda, n);
    _nDensity += theta*fugacity*K_2/nn;
    _eDensity += theta*fugacity*(3.0*K_2/argSq + K_1/arg);
    _pressure += theta*fugacity*K_2/nn/nn;
    _dndmu += theta*fugacity*K_2;
    _dPoverTdmu += theta*fugacity*K_2/nn;
    _dEoverTdmu += theta*fugacity*(3.0*K_2/argSq+ K_1/arg)*nn;
    }

  _nDensity   *= prefactor*mass2/beta;
  _eDensity   *= prefactor*mass4; // unit: GeV/fm^3
  _pressure   *= prefactor*mass2/beta2;
  _dndmu      *= prefactor*mass;
  _dPoverTdmu *= prefactor*mass2;
  _dEoverTdmu *= prefactor*mass4;
  _dsdmu       = _dPoverTdmu - _nDensity - mu*_dndmu + _dEoverTdmu; //!ds/dmu [1/(GeV fm^3)]
  _sDensity    = beta*(_eDensity + _pressure - mu*_nDensity); // unit : 1/fm^3
  }

  double ThermalHadron::decayProbability(unsigned int iStable)
  {
  if (iStable>=_decayProb.size()) throw Exception("iStable>=_decayProb.size()",__FUNCTION__);
  return _decayProb[iStable];
  }

  double ThermalHadron::decayProbabilityPair(unsigned int iStable1,unsigned int iStable2)
  {
  unsigned int nHadrons = _decayProb.size();
  if (iStable1>=nHadrons || iStable2>=nHadrons) throw Exception("iHadron1>=nHadrons || Hadron2>=nHadrons",__FUNCTION__);
  return _decayProbPair[iStable1*nHadrons+iStable2];
  }

  unsigned int ThermalHadron::_truncateOrder = 6;
  unsigned int ThermalHadron::truncateOrder() { return _truncateOrder;}
  void ThermalHadron::setTruncateOrder(unsigned int n) { _truncateOrder = n;}

} // namespace CAP

