#include "ModelHydro2D.hpp"
#include "XmlParser.hpp"
#include "XmlScalarField.hpp"

ClassImp(CAP::ModelHydro2D);
using CAP::MATH::twoPiCube;
using CAP::MATH::twoPi;
using CAP::Physics::hBarC;

namespace CAP
{
  ModelHydro2D::ModelHydro2D()
  :
  Model()
  {    }

  double ModelHydro2D::calculateIntegrand(ThermalHadron & hadron)
  {
  // Type of _statistics Bose-Einstein or Fermi-Dirac
  ParticleType & type = hadron.type();
  double sf    = type.spinFactor();
  double stat  = type.statistics();
  double mass  = type.mass();
  double mass2 = mass*mass;
  double chemPotential = chemicalPotential(type);

  double zeta      = _distanceField.randomX(gRandom->Rndm());
  double sPhi      = _distanceField.randomY(gRandom->Rndm());
  double sRapidity = _sRapidityRange * (gRandom->Rndm() - 0.5); // * _sRapidityRange;
  double dhs       = _distanceField.interpolate(zeta,sPhi,0.0);
  double dDdPhi    = _distanceFieldDPhi.interpolate(zeta,sPhi, 0.0);
  double dDdZeta   = _distanceFieldDZeta.interpolate(zeta,sPhi, 0.0);
  double vT        = _vtFluidField.interpolate(zeta,sPhi,0.0);
  double gammaT    = gammaAtBeta(vT);
  double phiF      = _phiFluidField.interpolate(zeta,sPhi,0.0);
  double tau       = _tauI + dhs*sin(zeta);
  double rho       = dhs*cos(zeta);
  _position.setTXYZ(tau*cosh(sRapidity),rho*cos(sPhi),rho*sin(sPhi),tau*sinh(sRapidity));

  zeta      = gRandom->Rndm();
  double zetac     = (zeta>0.9999999) ? 0.00000001 : 1.00-zeta;
  double pT        = zeta/zetac;
  double dPt       = 1.0/(zetac*zetac);
  double pPhi      = twoPi() * gRandom->Rndm();
  double pRapidity = _pRapidityRange * (gRandom->Rndm() - 0.5);
  double mT        = sqrt(mass2+pT*pT);
  double e         = mT * cosh(pRapidity);
  _momentum.setTXYZ(e,pT*cos(pPhi),pT*sin(pPhi),mT*sinh(pRapidity));

  double pDotU   = gammaT * (mT*cosh(pRapidity-sRapidity) - vT*pT*cos(sPhi-pPhi+phiF));
  double dSigmaP =  dhs*(_tauI+dhs*sin(zeta))
  * (
     dhs*cos(zeta)*(mT*sin(zeta)*cosh(pRapidity-sRapidity)+pT*cos(zeta)*cos(sPhi-pPhi))
     + dDdZeta*cos(zeta)*(-mT*cos(zeta)*cosh(pRapidity-sRapidity) + pT*sin(zeta)*cos(sPhi-pPhi))
     + dDdPhi*pT*sin(sPhi-pPhi)
     );
  if (_backFlow)
    {
    // only particle emission directed back to the hydro-region
    if(dSigmaP >= 0.0)
      dSigmaP = 0.0;
    else
      dSigmaP = -dSigmaP;
    }
  else
    {
    // disable particle emission back to the hydro-region
    if(dSigmaP < 0.0) dSigmaP = 0.0;
    }
  double denom = stat + exp( (pDotU-chemPotential)/_temperature.value() );
  double integrand = sf*pT*dPt*dSigmaP/(denom*twoPiCube());
  hadron.setIntegrand(integrand);
  return integrand;
  }

  void   ModelHydro2D::readFromXmlFile(const char * inputPath,
                                        const char * inputFileName)
  {
  double temperature, muB, muI, muS, muC;
  String systemName;
  String deviceName;
  double systemEnergy;
  double centralityMin;
  double centralityMax;
  double impactParameter;
  //double _centerT;
  printCR();
  printValue("Reading from xml file named",inputFileName);
  printValue("from path named",inputPath);
  XmlParser parser;
  XmlScalarField xmlDoc;
  parser.read(xmlDoc,inputPath,inputFileName);
  try
    {
    xmlDoc.getXmlTag("PARAMETER","name","_tau _i");
    _tauI            = xmlDoc.getXmlContent().Atof()/hBarC();       // [GeV^-1]
    xmlDoc.getXmlTag("PARAMETER","name","Temperature");
    temperature     = xmlDoc.getXmlContent().Atof() * 0.001;  // [GeV]
    xmlDoc.getXmlTag("PARAMETER","name","Mu_B");
    muB             = xmlDoc.getXmlContent().Atof() * 0.001;
    xmlDoc.getXmlTag("PARAMETER","name","Mu_I");
    muI             = xmlDoc.getXmlContent().Atof() * 0.001;
    xmlDoc.getXmlTag("PARAMETER","name","Mu_S");
    muS             = xmlDoc.getXmlContent().Atof() * 0.001;
    xmlDoc.getXmlTag("PARAMETER","name","Mu_C");
    muC             = xmlDoc.getXmlContent().Atof() * 0.001;        // [GeV]
    xmlDoc.getXmlTag("PARAMETER","name","colliding_energy");
    systemEnergy = xmlDoc.getXmlContent().Atof();    // [GeV]
    xmlDoc.getXmlTag("PARAMETER","name","centrality_min");
    centralityMin   = xmlDoc.getXmlContent().Atof();      // [%]
    xmlDoc.getXmlTag("PARAMETER","name","centrality_max");
    centralityMax   = xmlDoc.getXmlContent().Atof();     // [%]
    xmlDoc.getXmlTag("PARAMETER","name","impact_parameter");
    impactParameter = xmlDoc.getXmlContent().Atof();    // [fm]
    xmlDoc.getXmlTag("PARAMETER","name","temperature_at_center");
    //_centerT = xmlDoc.getXmlContent().Atof();  // [MeV]
    xmlDoc.getXmlTag("PARAMETER","name","device");
    deviceName      = xmlDoc.getXmlContent().Data();
    xmlDoc.getXmlTag("PARAMETER","name","colliding_system");
    systemName = xmlDoc.getXmlContent().Data();
    xmlDoc.getXmlTag("VECTOR3D", "name","Distance");
    _distanceField       = xmlDoc.getXmlScalarField();      // [GeV^-1]
    xmlDoc.getXmlTag("VECTOR3D", "name","FluidVt" );
    _vtFluidField        = xmlDoc.getXmlScalarField();
    xmlDoc.getXmlTag("VECTOR3D", "name","FluidPhi");
    _phiFluidField       = xmlDoc.getXmlScalarField();      // [c]

    }
  catch (int exceptionIndex)
    {
    throw Exception("I/O Error",__FUNCTION__);
    }
  setTemperature(temperature);
  setMuB(muB);
  setMuI(muI);
  setMuS(muS);
  setMuC(muC);
  setSystemName(systemName);
  setSystemEnergy(systemEnergy);
  setCentralityRange(centralityMin,centralityMax);
  setImpactParameter(impactParameter);
  // _distanceField derivatives
  try
    {
    xmlDoc.getXmlTag("VECTOR3D","name","DistanceDZeta");
    _distanceFieldDZeta = xmlDoc.getXmlScalarField();      // [GeV^-1/rad]
    }
  catch (int exceptionIndex)
    {
    _distanceFieldDZeta = _distanceField.derivativeX("DistanceDZeta");
    }
  try
    {
    xmlDoc.getXmlTag("VECTOR3D","name","DistanceDPhi");
    _distanceFieldDPhi = xmlDoc.getXmlScalarField();      // [GeV^-1/rad]
    }
  catch (int exceptionIndex)
    {
    _distanceFieldDPhi = _distanceField.derivativeY("DistanceDPhi");
    }
  }

} // namespace CAP
