#include "Model.hpp"
#include "PhysicsConstants.hpp"
#include "NameHelpers.hpp"
#include "PrintHelpers.hpp"
#include <iostream>
#include <fstream>

using namespace std;
ClassImp(CAP::CollisionSystem);
ClassImp(CAP::Model);

namespace CAP
{

  ModelSettings::ModelSettings()
  :
  _modelType(kHG),
  _modelSubtype(0),
  _systemName("PbPb"),
  _systemEnergy("5000.0"),
  _centralityType(kCentralityRange),
  _centralityMin(0.0),
  _centralityMax(1.0),
  _nSamples(100000),
  _fluctType(0),
  _multFractionMin(0),
  _multFractionMax(1),
  _multFractionRange(1),
  _disableFD(kDisableAll),
  _backFlow(0)
  { }

  ModelSettings::ModelSettings(const ModelSettings & source)
  :
  _modelType(source._modelType),
  _modelSubtype(source._modelSubtype),
  _systemName(source._systemName),
  _systemEnergy(source._systemEnergy),
  _centralityType(source._centralityType),
  _centralityMin(source._centralityMin),
  _centralityMax(source._centralityMax),
  _nSamples(source._nSamples),
  _fluctType(source._fluctType),
  _multFractionMin(source._multFractionMin),
  _multFractionMax(source._multFractionMax),
  _multFractionRange(source._multFractionRange),
  _disableFD(source._disableFD),
  _backFlow(source._backFlow)
  { }

  ModelSettings ModelSettings::operator=(const ModelSettings & rhs)
  {
  if (this!=&rhs)
    {
    _systemName     = rhs._systemName;
    _systemEnergy   = rhs._systemEnergy;
    _centralityType = rhs._centralityType;
    _centralityMin   =  rhs._centralityMin;
    _centralityMax   = rhs._centralityMax;
    _modelType         = rhs._modelType;
    _modelSubtype      = rhs._modelSubtype;
    _nSamples          = rhs._nSamples;
    _fluctType         = rhs._fluctType;
    _multFractionMin   = rhs._multFractionMin;
    _multFractionMax   = rhs._multFractionMax;
    _multFractionRange = rhs._multFractionRange;
    _disableFD         = rhs._disableFD;
    _backFlow          = rhs._backFlow;
    }
  return *this;
  }


 } // namespace CAP
