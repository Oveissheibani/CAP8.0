#ifndef CAP__Hypersurface
#define CAP__Hypersurface
#include "Aliases.hpp"
#include "VectorLorentz.hpp"

using CAP::MATH::VectorLorentz;

namespace CAP
{
  class Hypersurface
  {
  public:
  Hypersurface();
  Hypersurface(const Hypersurface & source);
  virtual ~Hypersurface() {}
  Hypersurface operator=(const Hypersurface & rhs);

  virtual double pRapidityRange() const  { return _pRapidityRange; }
  virtual double sRapidityRange() const  { return _sRapidityRange; }
  virtual double rhoMax() const {  return _rhoMax; }
  virtual double tauI() const {  return _tauI; }
  virtual double tauC() const { return _tauC; }
  virtual double amplitude() const { return _amp; }
  virtual double delay() const { return _delay; }
  virtual double sVolume() const  { return _sVolume; }
  virtual double pVolume() const { return _pVolume; }
  virtual double volume() const { return _volume; }

  double _sRapidityRange;
  double _pRapidityRange;
  double _rhoMax;
  double _tauI;
  double _tauC;
  double _lambda;
  double _amp;
  double _delay;
  double _sVolume;
  double _pVolume;
  double _volume;
  double _tVelocity;
  double _tGamma;
  MATH::VectorLorentz _position;
  MATH::VectorLorentz _momentum;
  };

} // namespace CAP

#endif // !CAP__ModelH_
