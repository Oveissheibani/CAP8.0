#include "ThermalHadronGas.hpp"
#include "ParticleDecayMode.hpp"
#include "PrintHelpers.hpp"
#include "NameHelpers.hpp"
#include <iostream>
#include <fstream>

ClassImp(CAP::ThermalHadronGas);

//using namespace std;

namespace CAP
{

  ThermalHadronGas::ThermalHadronGas()
  :
  _temperature(0),
  _mu(0),
  _muQ(0),
  _muS(0),
  _muC(0),
  _muB(0),
  _muT(0),
  _muBaryon(0),
  _nDensity(0),
  _eDensity(0),
  _sDensity(0),
  _pressure(0),
  _netBaryonDensity(0),
  _netChargeDensity(0),
  _netStrangeDensity(0),
  _charmDensity(0),
  _bottomDensity(0),
  _topDensity(0),
  _hadrons()
  {  }

  ThermalHadronGas::ThermalHadronGas(const ThermalHadronGas & source)
  :
  _temperature(source._temperature),
  _mu(source._mu),
  _muQ(source._muQ),
  _muS(source._muS),
  _muC(source._muC),
  _muB(source._muB),
  _muT(source._muT),
  _muBaryon(source._muBaryon),
  _nDensity(source._nDensity),
  _eDensity(source._eDensity),
  _sDensity(source._sDensity),
  _pressure(source._pressure),
  _netBaryonDensity(source._netBaryonDensity),
  _netChargeDensity(source._netChargeDensity),
  _netStrangeDensity(source._netStrangeDensity),
  _charmDensity(source._charmDensity),
  _bottomDensity(source._bottomDensity),
  _topDensity(source._topDensity),
  _hadrons(source._hadrons)
  {  }

  ThermalHadronGas ThermalHadronGas::operator=(const ThermalHadronGas & rhs)
  {
  if (this!=&rhs)
    {
    _temperature    = rhs._temperature;
    _mu  = rhs._mu;
    _muQ = rhs._muQ;
    _muS = rhs._muS;
    _muC = rhs._muC;
    _muB = rhs._muB;
    _muT = rhs._muT;
    _muBaryon          = rhs._muBaryon;
    _nDensity          = rhs._nDensity;
    _eDensity          = rhs._eDensity;
    _sDensity          = rhs._sDensity;
    _pressure          = rhs._pressure;
    _netBaryonDensity  = rhs._netBaryonDensity;
    _netChargeDensity  = rhs._netChargeDensity;
    _netStrangeDensity = rhs._netStrangeDensity;
    _charmDensity      = rhs._charmDensity;
    _bottomDensity     = rhs._bottomDensity;
    _topDensity        = rhs._topDensity;
    _hadrons           = rhs._hadrons;
    }
  return *this;
  }

  void ThermalHadronGas::reset()
  {
  _temperature = 0.0;
  _mu  = 0;
  _muQ = 0;
  _muS = 0;
  _muC = 0;
  _muB = 0;
  _muT = 0;
  _muBaryon        = 0;
  _temperature     = 0;
  _nDensity        = 0;
  _eDensity        = 0;
  _sDensity        = 0;
  _pressure        = 0;
  _netBaryonDensity  = 0;
  _netChargeDensity  = 0;
  _netStrangeDensity = 0;
  _charmDensity   = 0;
  _bottomDensity  = 0;
  _topDensity     = 0;
  for (auto & hadron : _hadrons) hadron.reset();
  }

  void ThermalHadronGas::clear()
  {
  reset();
  for (auto & hadron : _hadrons) hadron.clear();
  _hadrons.clear();
  }

  ThermalHadron & ThermalHadronGas::findHadronFor(int code)
  {
  for (auto & hadron : _hadrons)
    {
    if (hadron.type().pdgCode() == code) return hadron;
    }
  throw Exception("Unknown PDG Code",__FUNCTION__);
  }

  const ThermalHadron & ThermalHadronGas::findHadronFor(int code) const
  {
  for (auto & hadron : _hadrons)
    {
    if (hadron.type().pdgCode() == code) return hadron;
    }
  throw Exception("Unknown PDG Code",__FUNCTION__);
  }

  int ThermalHadronGas::findHadronIndexFor(int code)
  {
  int index = 0;
  for (auto & hadron : _hadrons)
    {
    if (hadron.type().pdgCode() == code) return index;
    index++;
    }
  return -1; //not found
  }

  int ThermalHadronGas::findHadronIndexFor(int code) const
  {
  int index = 0;
  for (auto & hadron : _hadrons)
    {
    if (hadron.type().pdgCode() == code) return index;
    index++;
    }
  return -1; //not found
  }

  //!
  //!Calculate all thermal properties of this hadron gas
  //!Assumes values for _temperature and chemical potentials have already been set.
  //!
  void ThermalHadronGas::calculateAllProperties()
  {
  double zero = 0.0;
  _nDensity     = zero;
  _eDensity     = zero;
  _sDensity    = zero;
  _pressure          = zero;
  _netBaryonDensity  = zero;
  _netChargeDensity  = zero;
  _netStrangeDensity = zero;
  _charmDensity   = zero;
  _bottomDensity  = zero;
  _topDensity     = zero;
  for (auto & hadron : _hadrons)
    {
    if (hadron.type().mass()<=0.0) continue;
    hadron.calculateAllProperties(_temperature, _muBaryon, _muS, _muC, _muB, _muT);
    _nDensity          += hadron.numberDensity();
    _eDensity          += hadron.energyDensity();
    _sDensity          += hadron.entropyDensity();
    _pressure          += hadron.partialPressure();
    _netBaryonDensity  += hadron.baryonDensity();
    _netChargeDensity  += hadron.chargeDensity();
    _netStrangeDensity += hadron.strangeDensity();
    _charmDensity      += hadron.charmDensity();
    _bottomDensity     += hadron.bottomDensity();
    _topDensity        += hadron.topDensity();
    }
  }

  void ThermalHadronGas::print() const
  {
  printLine();
  printLine();
  printString("Hadron Gas");
  printValue("_temperature",_temperature);
  printValue("mu",_mu);
  printValue("muQ",_muQ);
  printValue("muS",_muS);
  printValue("muC",_muS);
  printValue("muB",_muS);
  printValue("muT",_muS);
  printValue("muS",_muS);
  printValue("muBaryon",_muBaryon);
  printValue("nDensity",_nDensity);
  printValue("eDensity",_eDensity);
  printValue("sDensity",_sDensity);
  printValue("pressure",_pressure);
  printValue("netChargeDensity",_netChargeDensity);
  printValue("netBaryonDensity",_netBaryonDensity);
  printValue("netStrangeDensity",_netStrangeDensity);
  printValue("charmDensity",_charmDensity);
  printValue("bottomDensity",_bottomDensity);
  printValue("topDensity",_topDensity);
  for (auto & hadron : _hadrons) hadron.print();
  }

  //!
  //! Configures the Thermal Hadron Gas for standalone operation
  //!
  void ThermalHadronGas::configure(const String & taskName,
                                   const String & objectType,
                                   const Configuration & configuration)
  {
  _temperature = configuration.valueDouble(createKey(taskName,objectType,"_temperature"));
  _muBaryon = configuration.valueDouble(   createKey(taskName,objectType,"_muBaryon"));
  _muQ      = configuration.valueDouble(   createKey(taskName,objectType,"_muQ"));
  _muS      = configuration.valueDouble(   createKey(taskName,objectType,"_muS"));
  _muC      = configuration.valueDouble(   createKey(taskName,objectType,"_muC"));
  _muB      = configuration.valueDouble(   createKey(taskName,objectType,"_muB"));
  _muT      = configuration.valueDouble(   createKey(taskName,objectType,"_muT"));

  if (0)
    {
    printCR();
    printLine();
    printValue(createKey(taskName,objectType,"temperature"),_temperature);
    printValue(createKey(taskName,objectType,"muBaryon"),_muBaryon  );
    printValue(createKey(taskName,objectType,"muQ"),_muQ );
    printValue(createKey(taskName,objectType,"muS"),_muS );
    printValue(createKey(taskName,objectType,"muC"),_muC);
    printValue(createKey(taskName,objectType,"muB"),_muB);
    printValue(createKey(taskName,objectType,"muT"),_muT);
    printLine();
    printCR();
    }
  }

  void ThermalHadronGas::loadHadrons(const ParticleDb & particleDb)
  {
  for (auto & type : particleDb.particleTypes())
    {
    //int mcCode = type->pdgCode();
    double mass = type->mass();
    //printValue("Name",_type->name());
    if (type->isQuark()     ||
        type->isAntiQuark() ||
        type->isLepton()    ||
        type->isWeakBoson() ||
        type->isHiggsBoson()||
        mass > 2.5 )  continue;
    ThermalHadron hadron;
    hadron.clear();
    hadron.setType(type);
    _hadrons.push_back(hadron);
    }
  }

  void ThermalHadronGas::loadFromAscii(std::ifstream & inputFile, const ParticleDb & db)
  {
  clear();

  int nHadrons;
  inputFile >> nHadrons;
  if (nHadrons<1)
    {
    if (0)
      {
      printCR();
      printValue("nHadrons",nHadrons);
      }
    throw Exception("nHadrons<1",__FUNCTION__);
    }
  for (int iHadron=0; iHadron<nHadrons; iHadron++)
    {
    try
      {
      int    pdgCode;
      String name;
      double nDensity = 0;
      double eDensity = 0;
      double sDensity = 0;
      double pressure = 0;
      double feedDownNumberDensity = 0;
      double integrand = 0;
      double averageMultiplicity = 0;
      inputFile >> pdgCode >> integrand >> averageMultiplicity;

      ParticleType * type = db.findPdgCode(pdgCode);
      ThermalHadron hadron;
      hadron.setType(type);
      hadron.setNumberDensity(nDensity);
      hadron.setEnergyDensity(eDensity);
      hadron.setEntropyDensity(sDensity);
      hadron.setPartialPressure(pressure);
      hadron.setFeedDownNumberDensity(feedDownNumberDensity);
      hadron.setIntegrand(integrand);
      hadron.setAvgMultiplicity(averageMultiplicity);
      hadron.setMultiplicity(0);
      appendHadron(hadron);
      }
    catch (...)
      {
      throw Exception("Error reading _mult file.",__FUNCTION__);
      }
    }
  }

  void ThermalHadronGas::saveToAscii(std::ofstream & outputFile)
  {
  for (auto & hadron : _hadrons)
    {
    const ParticleType & type = hadron.type();
    outputFile << type.pdgCode() << endl;
    outputFile << type.name() << endl;
    outputFile << hadron.numberDensity() << endl;
    outputFile << hadron.energyDensity() << endl;
    outputFile << hadron.entropyDensity() << endl;
    outputFile << hadron.partialPressure() << endl;
    outputFile << hadron.numberDensityWFD() << endl;
    outputFile << hadron.integrand() << endl;
    outputFile << hadron.avgMultiplicity() << endl;
    }
  }
}   // namespace CAP
