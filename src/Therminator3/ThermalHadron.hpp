#ifndef CAP_ThermalHadron
#define CAP_ThermalHadron
#include <vector>
#include "ParticleType.hpp"
#include "TObject.h"

class ThermalHadronGas;
class Model;

namespace CAP
{

  class ThermalHadron : public TObject
  {
  protected:

  ParticleType * _type;   //! Hadron species
  double _nDensity; //! number density
  double _eDensity; //! _energy density
  double _sDensity; //! _entropy density
  double _pressure;     //! partial _pressure
  double _densityWFD;   //! density with feed down
  double _dPoverTdmu;
  double _dEoverTdmu;
  double _dndmu;
  double _dsdmu;
  double _deltaN_bulk_term1;     // contribution from bulk delta f
  double _deltaN_bulk_term2;     // contribution from bulk delta f
  double _deltaN_qmu_term1;      // contribution from baryon diffusion
  double _deltaN_qmu_term2;      // contribution from baryon diffusion
  double _integrand;
  double _avgMult;
  double _mult;

  std::vector<double> _decayProb;
  std::vector<double> _decayProbPair;

  public:

  friend class ThermalHadronGas;
  friend class Model;

  ThermalHadron();
  ThermalHadron(const ThermalHadron & source);
  ThermalHadron operator=(const ThermalHadron & rhs);
  virtual ~ThermalHadron() {}
  virtual void loadType(ParticleType * _type);
  virtual void reset();
  virtual void clear();
  virtual void createDecayProbabilityArray(unsigned int n);
  virtual void setDecayProbability(unsigned int iHadron, double probability);
  virtual void setDecayProbabilityPair(unsigned int iHadron1, unsigned int iHadron2, double probability);
  virtual void setDecayProbabilities(const std::vector<double> & probabilities)
  {
  _decayProb = probabilities;
  }
  
  virtual void calculateAllProperties(double temperature,
                                      double muBaryon,
                                      double muS,
                                      double muC,
                                      double muB,
                                      double muT);

  const ParticleType & type() const { return *_type; }
  ParticleType & type() { return *_type; }
  int pdgCode() const { return _type->pdgCode();}
  double numberDensity() const{ return _nDensity; }
  double energyDensity() const{ return _eDensity; }
  double entropyDensity() const{ return _sDensity; }
  double partialPressure() const{ return _pressure; }
  double baryonDensity() const{ return _nDensity*_type->baryonNumber(); }
  double chargeDensity() const{ return _nDensity*_type->charge();}
  double strangeDensity() const{ return _nDensity*_type->strangeness();}
  double charmDensity() const{ return _nDensity*_type->charm(); }
  double bottomDensity() const{ return _nDensity*_type->bottom();}
  double topDensity() const{ return _nDensity*_type->top();}
  double numberDensityWFD() const{ return _densityWFD; }
  double integrand() const { return _integrand; }
  double avgMultiplicity() const { return _avgMult;}
  double multiplicity() const { return _mult;}
  const std::vector<double>  decayProbabilities() const { return _decayProb;}
  std::vector<double>  decayProbabilities() { return _decayProb;}
  double decayProbability(unsigned int iStable);
  double decayProbabilityPair(unsigned int iStable1,unsigned int iStable2);

  void setType(ParticleType * value) { _type = value; }
  void setNumberDensity(double value) { _nDensity = value; }
  void setEnergyDensity(double value) { _eDensity = value; }
  void setEntropyDensity(double value) { _sDensity = value; }
  void setPartialPressure(double value) { _pressure = value; }
  void setFeedDownNumberDensity(double value) {  _densityWFD = value; }
  void setIntegrand(double value) { _integrand = value; }
  void setAvgMultiplicity(double value) { _avgMult = value;}
  void setMultiplicity(double value) { _mult = value;}
  void print() const;

  protected:
  static unsigned int _truncateOrder;
  public:
  static unsigned int truncateOrder();
  static void setTruncateOrder(unsigned int n);

  ClassDef(ThermalHadron,0)
  };

} // namespace CAP

#endif  // CAP_thermalHadronGas

