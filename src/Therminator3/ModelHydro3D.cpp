#include "ModelHydro3D.hpp"
#include "XmlParser.hpp"
#include "XmlScalarField.hpp"

ClassImp(CAP::ModelHydro3D);

namespace CAP
{

  ModelHydro3D::ModelHydro3D()
  :
  Model()
  {   }

  double ModelHydro3D::calculateIntegrand(ThermalHadron & hadron)
  {
  // Type of _statistics Bose-Einstein or Fermi-Dirac
  ParticleType & type = hadron.type();
  double sf    = type.spinFactor();
  double stat  = type.statistics();
  double mass  = type.mass();
  double mass2 = mass*mass;
  double chemPotential = chemicalPotential(type);
  double zeta    = _distanceField.randomX( gRandom->Rndm() );
  double sPhi    = _distanceField.randomY( gRandom->Rndm() );
  double theta   = _distanceField.randomZ( gRandom->Rndm() );;
  double dhs     = _distanceField.interpolate(zeta,sPhi,theta);
  double dDdZeta = _distanceFieldDZeta.interpolate(zeta,sPhi,theta);
  double dDdPhi  = _distanceFieldDPhi.interpolate(zeta,sPhi,theta);
  double dDdTheta= _distanceFieldDTheta.interpolate(zeta,sPhi,theta);
  double uxFluidVelocity       = uxFluidVelocityField.interpolate(zeta,sPhi,theta);
  double uyFluidVelocity       = uyFluidVelocityField.interpolate(zeta,sPhi,theta);
  double rapidityFluidVelocity = rapidityFluidField.interpolate(zeta,sPhi,theta);
  double tau       = _tauI + dhs*std::sin(theta)*std::sin(zeta);
  double rho       = dhs * std::sin(theta)*std::cos(zeta);
  double sRapidity = dhs * std::cos(theta)/_lambda;
  _position.setTXYZ(tau *std::cosh(sRapidity),rho*std::cos(sPhi),rho*std::sin(sPhi),tau *std::sinh(sRapidity));

  zeta             = gRandom->Rndm();
  double zetac     = (zeta>0.9999999) ? 0.00000001 : 1.00-zeta;
  double pT        = zeta/zetac;
  double dPt       = 1.0/(zetac*zetac);
  double pPhi      = twoPi() * gRandom->Rndm();
  double pRapidity = _pRapidityRange * (gRandom->Rndm() - 0.5);
  double mT        = std::sqrt(mass2+pT*pT);
  double e         = mT*std::cosh(pRapidity);
  _momentum.setTXYZ(e,pT*std::cos(pPhi),pT*std::sin(pPhi),mT*std::sinh(pRapidity));

  double pDotU = std::sqrt(1.0 + uxFluidVelocity * uxFluidVelocity + uyFluidVelocity*uyFluidVelocity)*mT*std::cosh(rapidityFluidVelocity - pRapidity) - pT*(uxFluidVelocity*std::cos(pPhi) + uyFluidVelocity*std::sin(pPhi));
  double dSigmaP =  dhs*dhs*std::sin(theta) *
  ( tau  / _lambda * (
                       dDdZeta * std::cos(zeta)* (-mT * std::cos(zeta) * std::cosh(pRapidity-sRapidity) + pT*std::sin(zeta)*std::cos(sPhi-pPhi))
                       + (dhs*std::sin(theta) - dDdTheta*std::cos(theta))*std::cos(zeta)*std::sin(theta)*(mT*std::sin(zeta)*std::cosh(pRapidity-sRapidity) + pT*std::cos(zeta)*std::cos(sPhi-pPhi))
                       + dDdPhi*pT*std::sin(sPhi-pPhi)
                       )
   +   (dhs * std::cos(theta) + dDdTheta*std::sin(theta))*std::sin(theta)*mT*std::cos(zeta)*std::sinh(pRapidity-sRapidity)
   );
  if (_backFlow)
    {
    // only particle emission directed back to the hydro-region
    dSigmaP = (dSigmaP >= 0.0) ? 0.0 : -dSigmaP;
    }
  else
    {
    // disable particle emission back to the hydro-region
    if (dSigmaP < 0.0) dSigmaP = 0.0;
    }
  double denom = stat + std::exp( (pDotU-chemPotential)/_temperature.value() );
  double integrand = sf*pT*dPt*dSigmaP/(denom*twoPiCube());
  hadron.setIntegrand(integrand);
  return integrand; }

  void ModelHydro3D::readFromXmlFile(const char * _inputPath,
                                      const char * _inputFileName)
  {
  double temperature, muB, muI, muS, muC;
  String systemName;
  double systemEnergy;
  double centralityMin;
  double centralityMax;
  double impactParameter;
  //double _centerT;

  printCR();
  printValue("Reading from xml file named",_inputFileName);
  printValue("from path named",_inputPath);
  XmlParser parser;
  XmlScalarField xmlDoc;
  parser.read(xmlDoc,_inputPath,_inputFileName);
  try
    {
    xmlDoc.getXmlTag("PARAMETER","name","Tau_i");
    _tauI            = xmlDoc.getXmlContent().Atof() / CAP::Physics::hBarC();       // [GeV^-1]
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
    //deviceName      = xmlDoc.getXmlContent().Data();
    xmlDoc.getXmlTag("PARAMETER","name","colliding_system");
    systemName = xmlDoc.getXmlContent().Data();
    xmlDoc.getXmlTag("VECTOR3D", "name","Distance");
    _distanceField       = xmlDoc.getXmlScalarField();      // [GeV^-1]
    xmlDoc.getXmlTag("VECTOR3D", "name","FluiduxFluidVelocity");
    uxFluidVelocityField        = xmlDoc.getXmlScalarField();      // [1]
    xmlDoc.getXmlTag("VECTOR3D", "name","FluiduyFluidVelocity");
    uyFluidVelocityField        = xmlDoc.getXmlScalarField();      // [1]
    xmlDoc.getXmlTag("VECTOR3D", "name","FluidRap");
    rapidityFluidField  = xmlDoc.getXmlScalarField();      // [1]
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
    _distanceFieldDZeta = xmlDoc.getXmlScalarField();    // [GeV^-1/rad]
    }
  catch (int exceptionIndex)
    {
    throw Exception("I/O Error DistanceDZeta",__FUNCTION__);
    }

  try
    {
    xmlDoc.getXmlTag("VECTOR3D","name","DistanceDPhi");
    _distanceFieldDPhi = xmlDoc.getXmlScalarField();    // [GeV^-1/rad]
    }
  catch (int exceptionIndex)
    {
    throw Exception("I/O Error DistanceDPhi",__FUNCTION__);
    }

  try
    {
    xmlDoc.getXmlTag("VECTOR3D","name","DistanceDTheta");
    _distanceFieldDTheta = xmlDoc.getXmlScalarField();    // [GeV^-1/rad]
    }
  catch (int exceptionIndex)
    {
    throw Exception("I/O Error DistanceDTheta",__FUNCTION__);
    }
  }

} // namespace CAP

