/* **********************************************************************
 * Copyright (C) 2019-2022, Claude Pruneau, Victor Gonzalez, Sumit Basu
 * All rights reserved.
 *
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 *
 * Author: Claude Pruneau,   04/01/2022
 * Copyright 2016 Chun Shen
 * Copyright 2022 Claude Pruneau
 *
 * *********************************************************************/
#ifndef CAP_ThermalHadronGas
#define CAP_ThermalHadronGas
#include "ThermalHadron.hpp"
#include "ParticleDb.hpp"

namespace CAP
{

  //!
  //! This class embodies a thermal gas of independent hadrons and their basic properties.
  //! The temperature and chemical potentials are common to all particles of the gas.
  //!
  class ThermalHadronGas
  {
  protected:

  double _temperature;
  double _mu;
  double _muQ;
  double _muS;
  double _muC;
  double _muB;
  double _muT;
  double _muBaryon;
  double _nDensity;
  double _eDensity;
  double _sDensity;
  double _pressure;
  double _netBaryonDensity;
  double _netChargeDensity;
  double _netStrangeDensity;
  double _charmDensity;
  double _bottomDensity;
  double _topDensity;
  vector<ThermalHadron> _hadrons;

  public:

  ThermalHadronGas();
  ThermalHadronGas(const ThermalHadronGas & source);
  ThermalHadronGas operator=(const ThermalHadronGas & rhs);
  virtual ~ThermalHadronGas() {}
  virtual void configure(const String & taskName,
                         const String & objectType,
                         const Configuration & configuration);
  virtual void loadHadrons(const ParticleDb & particleDb);
  virtual void reset();
  virtual void clear();
  void print() const;
  void appendHadron(ThermalHadron & hadron) { _hadrons.push_back(hadron);}
  std::vector<ThermalHadron> & hadrons() { return _hadrons;}
  const std::vector<ThermalHadron> & hadrons() const { return _hadrons;}
  ThermalHadron & findHadronFor(int pdgCode);
  const ThermalHadron & findHadronFor(int pdgCode) const;
  int  findHadronIndexFor(int pdgCode);
  int  findHadronIndexFor(int pdgCode) const;
  virtual void calculateAllProperties();
  ParticleType & particleTypeAt(unsigned int index)
  {
  if (index>=_hadrons.size())
    throw IndexOutOfBoundException(index,_hadrons.size(),"Out of bound index",__FUNCTION__);
  return _hadrons[index].type();
  }

  ThermalHadron & hadronAt(unsigned int index)
  {
  if (index>=_hadrons.size())
    throw IndexOutOfBoundException(index,_hadrons.size(),"Out of bound index",__FUNCTION__);
  return _hadrons[index];
  }

  const ThermalHadron & hadronAt(unsigned int index) const
  {
  if (index>=_hadrons.size())
    throw IndexOutOfBoundException(index,_hadrons.size(),"Out of bound index",__FUNCTION__);
  return _hadrons[index];
  }


  //!
  //!Get  the _temperature of this hadron gas
  //!
  double temperature() const{ return _temperature; }

  //!
  //!Get  the chemical potential of this hadron gas
  //!
  double mu()  const { return _mu; }

  //!
  //!Get  the baryon chemical potential of this hadron gas
  //!
  double muB() const { return _muBaryon; }

  //!
  //!Get  the strangeness chemical potential of this hadron gas
  //!
  double muS() const { return _muS; }

  //!
  //!Get  the charge chemical potential of this hadron gas
  //!
  double muQ() const { return _muQ; }

  //!
  //!Get  the particle number density associated with all the particles in this hadron gas
  //!
  double numberDensity()    const{ return _nDensity; }

  //!
  //!Get  the _energy density associated with all the particles in this hadron gas
  //!
  double energyDensity()    const{ return _eDensity; }

  //!
  //!Get  the _entropy density associated with all the particles in this hadron gas
  //!
  double entropyDensity()   const{ return _sDensity; }

  //!
  //!Get  (total) _pressure associated with all the particles in this hadron gas
  //!
  double pressure()         const{ return _pressure; }

  //!
  //!Get  Net Baryon Density of this hadron gas
  //!
  double netBaryonDensity() const{ return _netBaryonDensity;}

  //!
  //!Get  Net Charge Density of this hadron gas
  //!
  double netChargeDensity() const{ return _netChargeDensity;}

  //!
  //!Get  Net Strangeness Density of this hadron gas
  //!
  double netStrangeDensity() const{ return _netStrangeDensity;}

  //!
  //!Get  Net Strangeness Density of this hadron gas
  //!
  double charmDensity() const{ return _charmDensity;}

  //!
  //!Get  Net Strangeness Density of this hadron gas
  //!
  double bottomDensity() const{ return _bottomDensity;}

  void loadFromAscii(std::ifstream & inputFile, const ParticleDb & db);
  void saveToAscii(std::ofstream & outputFile);
  
  ClassDef(ThermalHadronGas,0)
  };

} // namespace CAP

#endif  // CAP_ThermalHadronGas

