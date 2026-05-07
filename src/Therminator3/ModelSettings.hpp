#ifndef CAP__ModelSettings
#define CAP__ModelSettings
#include "Aliases.hpp"
#include "ParticleType.hpp"
#include <iostream>

namespace CAP
{
  class ModelSettings
  {
  public:

  enum ModelTypes {kHG, kBW, kBWA, kH2D, kH3D, kSFO};
  enum CentralityTypes { kFixedImpact, kImpactRange, kFixedCentrality, kCentralityRange };
  enum FeedDownOption { kDisableAll, kDisable2, kDisable3, kDisableGE4};

  ModelSettings();
  ModelSettings(const ModelSettings & source);
  virtual ~ModelSettings() {}
  ModelSettings operator=(const ModelSettings & rhs);
  double type() const    { return _modelType; }
  double subtype() const { return _modelSubtype; }
  const String & systemName() const { return _systemName; }
  const String & systemEnergy() const { return _systemEnergy; }
  double centrality() const { return _centralityMin; }
  double centralityMin() const { return _centralityMin; }
  double centralityMax() const { return _centralityMax; }
  double impactParameter() const { return _centralityMin; }
  double impactParameterMin() const { return _centralityMin; }
  double impactParameterMax() const { return _centralityMax; }

  void setSystemName(const String value)  { _systemName = value; }
  void setSystemEnergy(double value)  { _systemEnergy = value; }
  void setCentralityRange(double min, double max)
  {
  _centralityType = kCentralityRange;
  _centralityMin  = min;
  _centralityMax  = max;
  }

  void setCentrality(double value)
  {
  _centralityType = kFixedCentrality;
  _centralityMin  = value;
  _centralityMax  = value;
  }
  void setImpactParameterRange(double min, double max)
  {
  _centralityType = kImpactRange;
  _centralityMin  = min;
  _centralityMax  = max;
  }
  void setImpactParameter(double value)
  {
  _centralityType = kFixedImpact;
  _centralityMin  = value;
  _centralityMax  = value;
  }

  void setMultiplicityFraction(double min, double max, unsigned int fluctuationType=0)
  {
  _multFractionMin = min;
  _multFractionMax = max;
  _multFractionRange = _multFractionMax - _multFractionMin;
  _fluctType = fluctuationType;
  }


  void setDisableFeedDown(unsigned int value)
  {
  _disableFD = value;
  }

  void setBackFlow(unsigned int value)
  {
  _backFlow = value;
  }

  protected:

  unsigned int    _modelType;
  unsigned int    _modelSubtype;
  String          _systemName;      //! collision system name
  String          _systemEnergy;    //! collision _energy (GeV)
  CentralityTypes _centralityType;  //! _modelType of impact parameter selection
                                   //! kFixedImpact, kImpactRange, kFixedCentrality, kCentralityRange
  double          _centralityMin;   //! collision centrality (%) or impact parameter min (fm)
  double          _centralityMax;   //! collision centrality (%) or impact parameter max (fm)
  unsigned int    _nSamples;
  unsigned int    _fluctType;
  double          _multFractionMin;
  double          _multFractionMax;
  double          _multFractionRange;
  unsigned int    _disableFD;
  unsigned int    _backFlow;
  };



} // namespace CAP

#endif // !CAP__ModelH_
