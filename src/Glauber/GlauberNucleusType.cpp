#include "GlauberNucleusType.hpp"
#include "PrintHelpers.hpp"

ClassImp(CAP::GlauberNucleusType)

namespace CAP
{
  
  GlauberNucleusType::GlauberNucleusType()
  :
  _name(),
  _type(0),
  _nProtons(0),
  _nNeutrons(0),
  _useHulthen(false),
  _radiusProton(0),
  _skinProton(0),
  _wProton(0),
  _radiusNeutron(0),
  _skinNeutron(0),
  _wNeutron(0),
  _beta2(0),
  _beta4(0),
  _a(0),
  _b(0),
  _r0(0),
  _r1(0),
  _r2(0),
  _maxRadius(0),
  _radialDistType(0)
  {
    // no ops
  }
  
  GlauberNucleusType::GlauberNucleusType(const GlauberNucleusType & src)
  :
  _name(src._name),
  _type(src._type),
  _nProtons(src._nProtons),
  _nNeutrons(src._nNeutrons),
  _useHulthen(src._useHulthen),
  _radiusProton(src._radiusProton),
  _skinProton(src._skinProton),
  _wProton(src._wProton),
  _radiusNeutron(src._radiusNeutron),
  _skinNeutron(src._skinNeutron),
  _wNeutron(src._wNeutron),
  _beta2(src._beta2),
  _beta4(src._beta4),
  _a(src._a),
  _b(src._b),
  _r0(src._r0),
  _r1(src._r1),
  _r2(src._r2),
  _maxRadius(src._maxRadius),
  _radialDistType(src._radialDistType)
  {
    // no ops
  }
  
  GlauberNucleusType GlauberNucleusType::operator=(const GlauberNucleusType & rhs)
  {
  if (this!=&rhs)
    {
    _name            = rhs._name;
    _type            = rhs._type;
    _nProtons        = rhs._nProtons;
    _nNeutrons       = rhs._nNeutrons;
    _useHulthen      = rhs._useHulthen;
    _radiusProton    = rhs._radiusProton;
    _skinProton      = rhs._skinProton;
    _wProton         = rhs._wProton;
    _radiusNeutron   = rhs._radiusNeutron;
    _skinNeutron     = rhs._skinNeutron;
    _wNeutron        = rhs._wNeutron;
    _beta2            = rhs._beta2;
    _beta4            = rhs._beta4;
    _a                = rhs._a;
    _b                = rhs._b;
    _r0               = rhs._r0;
    _r1               = rhs._r1;
    _r2               = rhs._r2;
    _maxRadius        = rhs._maxRadius;
    _radialDistType   = rhs._radialDistType;
    }
  return *this;
  }
  
  
  void GlauberNucleusType::print()
  {
  printCR();
  printLine();
  printValue("name",          _name);
  printValue("type",          _type);
  printValue("nProtons",      _nProtons);
  printValue("nNeutrons",     _nNeutrons);
  printValue("nNucleons",     _nProtons+_nNeutrons);
  printValue("useHulthen",    _useHulthen);
  printValue("radiusProton",  _radiusProton);
  printValue("skinProton",    _skinProton);
  printValue("wProton",       _wProton);
  printValue("radiusNeutron", _radiusNeutron);
  printValue("skinNeutron",   _skinNeutron);
  printValue("wNeutron",      _wNeutron);
  printValue("beta2", _beta2);
  printValue("beta4", _beta4);
  printValue("a",  _a);
  printValue("b",  _b);
  printValue("r0", _r0);
  printValue("r1", _r1);
  printValue("r2", _r2);
  printValue("maxRadius", _maxRadius);
  printValue("radialDistType", _radialDistType);
  }
  
} // namespace CAP

