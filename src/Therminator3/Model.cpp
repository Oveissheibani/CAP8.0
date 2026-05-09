#include "Model.hpp"
#include "PhysicsConstants.hpp"
#include "MathBasicFunctions.hpp"
#include "NameHelpers.hpp"
#include "PrintHelpers.hpp"
#include "ParticleTypeFilter.hpp"
#include "ParticleFilter.hpp"
#include <fstream>
#include "TMath.h"

using namespace std;
using CAP::MATH::twoPiCube;
using CAP::MATH::twoPi;

ClassImp(CAP::Model);

namespace CAP
{

  Model::Model()
  :
  ModelSettings(),
  Thermodynamics(),
  Hypersurface()
  {    }

  void Model::configure(const String & taskName,
                        const String & typeName,
                        const Configuration & configuration)
  {
  _modelType      = configuration.valueInt(  createKey(taskName,typeName,"Type"));
  _modelSubtype   = configuration.valueInt(  createKey(taskName,typeName,"Subtype"));
  setTemperature(configuration.valueDouble(  createKey(taskName,typeName,"Thermodynamics:Temperature")));
  setMuB(        configuration.valueDouble(  createKey(taskName,typeName,"Thermodynamics:MuB")));
  setMuI(        configuration.valueDouble(  createKey(taskName,typeName,"Thermodynamics:MuI")));
  setMuS(        configuration.valueDouble(  createKey(taskName,typeName,"Thermodynamics:MuS")));
  setMuC(        configuration.valueDouble(  createKey(taskName,typeName,"Thermodynamics:MuC")));
  _pRapidityRange = configuration.valueDouble(createKey(taskName,typeName,"pRapidityRange"));
  _sRapidityRange = configuration.valueDouble(createKey(taskName,typeName,"sRapidityRange"));
  _rhoMax         = configuration.valueDouble(createKey(taskName,typeName,"rhoMax"));
  _tauI           = configuration.valueDouble(createKey(taskName,typeName,"TauI"));
  _tauC           = configuration.valueDouble(createKey(taskName,typeName,"TauC"));
  _tVelocity      = configuration.valueDouble(createKey(taskName,typeName,"tVelocity"));
  _amp            = configuration.valueDouble(createKey(taskName,typeName,"Amplitude"));
  _delay          = configuration.valueDouble(createKey(taskName,typeName,"Delay"));
  _backFlow       = configuration.valueBool(  createKey(taskName,typeName,"BackFlow"));
  _tGamma         = gammaAtBeta(_tVelocity);

  setSystemName(      configuration.valueString(createKey(taskName,typeName,"SystemName")));
  setSystemEnergy(    configuration.valueDouble(createKey(taskName,typeName,"SystemEnergy")));
  setCentrality(      configuration.valueDouble(createKey(taskName,typeName,"Centrality")));
  double min = configuration.valueDouble(createKey(taskName,typeName,"CentralityMin"));
  double max = configuration.valueDouble(createKey(taskName,typeName,"CentralityMax"));
  setCentralityRange(min,max);
  min = configuration.valueDouble(createKey(taskName,typeName,"ImpactParameter"));
  max = configuration.valueDouble(createKey(taskName,typeName,"ImpactParameterMin"));
  setImpactParameter( configuration.valueDouble(createKey(taskName,typeName,"ImpactParameterMax")));
  setImpactParameterRange(min,max);
   _thermalHadronGas.configure(taskName,typeName,configuration);
  }

  void   Model::print() const
  {
  printCR();
  printLine();
  printString("Therminator 3");
  printString("Release, 1.0.0, 6 February 2026 ");
  printCR();
  printString("Authors");
  printString("    Claude A Pruneau   (aa7526@wayne.edu)");
  printCR();
  printCR();
  printString("Based on Therminator 2");
  printString("Release, 2.0.3, 1 February 2011 ");
  printCR();
  printString("Authors");
  printString("    Mikolaj Chojnacki   (Mikolaj.Chojnacki@ifj.edu.pl)");
  printString("    Adam Kisiel         (kisiel@if.pw.edu.pl)");
  printString("    Wojciech Broniowski (Wojciech.Broniowski@ifj.edu.pl)");
  printString("    Wojciech Florkowski (Wojciech.Florkowski@ifj.edu.pl)");
  printLine();
  printCR();
  }


  void Model::calculateMultiplicities()
  {
  _volume = _sVolume * _pVolume;
//  for (auto & hadron : _thermalHadronGas.hadrons())
//    {
//    //ParticleType & type = hadron.type();
//    //XXXX
//    }
  }

  double Model::calculateIntegrand(ThermalHadron & hadron)
  {
  // Type of _statistics Bose-Einstein or Fermi-Dirac
  ParticleType & type = hadron.type();
  double sf    = type.spinFactor();
  double stat  = type.statistics();
  double mass  = type.mass();
  double mass2 = mass*mass;
  double chemPotential = chemicalPotential(type);

  _position.setTXYZ(0.0,0.0,0.0,0.0);

  double zeta      = gRandom->Rndm();
  double zetac     = (zeta>0.9999999) ? 0.0000001 : 1.00-zeta;
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

//  {
//
//  for (unsigned int iSample = 0; iSample < nSamplesIntegration; iSample++)
//    {
//    _integrand = calculateIntegrand(hadron);
//    if (_integrand>maxIntegrand) maxIntegrand = _integrand;
//    _mult += _integrand;
//    }
//  hadron.setIntegrand(maxIntegrand);
//  hadron.setAvgMultiplicity(_mult*_volume/nSamplesIntegration);
//  return _integrand;
//  }

  void Model::generateEvent(Event & event,const ParticleFilter & particleFilter)
  {
  std::vector<ThermalHadron> & hadrons = _thermalHadronGas.hadrons();

  // choose (scaled down) _mult of this event
  double multFraction = 1.0;
  if (_multFractionRange>0)
    multFraction = _multFractionMin + _multFractionRange*gRandom->Rndm();
  else
    multFraction = _multFractionMin;
  double mean  = 0;
  
  for (auto & hadron : hadrons)
    {
    ParticleType & type = hadron.type();
    mean  = multFraction * hadron.avgMultiplicity();
    switch (_fluctType)
      {
        case 0: // Poisson fluctuations
        hadron.setMultiplicity(gRandom->Poisson(mean));
        break;

        case 1: // Negative Binomial  fluctuations
        hadron.setMultiplicity(0.0); // Fix me!
        break;

        default:
        case 2: // Gaussian fluctuations
        hadron.setMultiplicity(TMath::Max(0, int(gRandom->Gaus(mean,std::sqrt(mean)))));
        break;

        case 3:  // Poisson or Gaussian fluctuations
        if (mean>20)
          hadron.setMultiplicity(TMath::Max(0, int(gRandom->Gaus(mean,std::sqrt(mean)))));
        else
          hadron.setMultiplicity(TMath::Max(0, int(gRandom->Poisson(mean))));
        break;
      }

    // now generate the particles
    double   value;
    double   valueTest;
    double   maxIntegrand = hadron.integrand();
    unsigned int mult = hadron.multiplicity();
    unsigned int iParticle = 0;
    while (iParticle < mult)
      {
      value = calculateIntegrand(hadron);
      valueTest = maxIntegrand * gRandom->Rndm();
      if (valueTest<value)
        {
        Particle & particle = Particle::factory().nextObject();
        particle.setType(&type);
        particle.setLive(true);
        particle.setPosition(_position);
        particle.setMomentum(_momentum);
        iParticle++;
        if (particleFilter.accept(particle)) event.addParticle(&particle);
        }
      }
    }
  }

} // namespace CAP
