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
#ifndef CAP_ThermalHadronGasFD
#define CAP_ThermalHadronGasFD
#include "ThermalHadronGas.hpp"

namespace CAP
{
  //!
  //! This class embodies a thermal gas of independent hadrons and their basic properties as well as
  //! their decay into specific stable species - which are user specified..
  //! The temperature and chemical potentials are common to all particles of the gas.
  //!
  class ThermalHadronGasFD
  :
  public ThermalHadronGas
  {
  protected:
  vector<ThermalHadron*> _stableHadrons;
  vector<double> _stableHadronPairs;

  public:

  ThermalHadronGasFD();
  ThermalHadronGasFD(const ThermalHadronGasFD & source);
  ThermalHadronGasFD & operator=(const ThermalHadronGasFD & rhs);
  virtual ~ThermalHadronGasFD() {}
//  virtual void configure(const String & taskName,
//                         const String & objectType,
//                         const Configuration & configuration);
  virtual void reset();
  virtual void clear();
  void print() const;
  std::vector<ThermalHadron*> & stableHadrons() { return _stableHadrons;}
  const std::vector<ThermalHadron*> & stableHadrons() const { return _stableHadrons;}

  virtual void calculateAllProperties();

  //!
  //!Calculate the decay probabilities of all "thermal" _hadrons to the selected _stable _hadrons.
  //!
  virtual void calculateParticleDecayProbability();

  //!
  //!Calculate the decay probabilities of all "thermal" _hadrons to the selected _stable _hadrons.
  //!
  virtual void calculateStableDensities();

  ThermalHadron & findStableHadronFor(int pdgCode);
  virtual void findStableHadrons();
  int findStableHadronIndexFor(int pdgCode);

  ParticleType * stableParticleTypeAt(int index);

  const vector<double> & stableHadronPairs() const
  {
  return _stableHadronPairs;
  }

  vector<double> & stableHadronPairs()
  {
  return _stableHadronPairs;
  }

  void setupDecayGenerator();
  ParticleType * generateRandomHadron();
  int generateRandomHadronByIndex();


  ClassDef(ThermalHadronGasFD,0)
  };

} // namespace CAP

#endif  // CAP_ThermalHadronGasFD

