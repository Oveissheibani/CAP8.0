#include "Hypersurface.hpp"
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
  Hypersurface::Hypersurface()
  :
  _sRapidityRange(0.0),
  _pRapidityRange(0.0),
  _rhoMax(0.0),
  _tauI(0.0),
  _tauC(0.0),
  _lambda(0.0),
  _amp(0.0),
  _delay(0.0),
  _sVolume(0.0),
  _pVolume(0.0),
  _volume(0.0),
  _tVelocity(0.0),
  _position(),
  _momentum()
  {    }

  Hypersurface::Hypersurface(const Hypersurface & source)
  :
  _sRapidityRange(source._sRapidityRange),
  _pRapidityRange(source._pRapidityRange),
  _rhoMax(source._rhoMax),
  _tauI(source._tauI),
  _tauC(source._tauC),
  _lambda(source._lambda),
  _amp(source._amp),
  _delay(source._delay),
  _sVolume(source._sVolume),
  _pVolume(source._pVolume),
  _volume(source._volume),
  _tVelocity(source._tVelocity),
  _tGamma(source._tGamma),
  _position(source._position),
  _momentum(source._momentum)
  {    }

  Hypersurface Hypersurface::operator=(const Hypersurface & rhs)
  {
  if (this!=&rhs)
    {
    _sRapidityRange =  rhs._sRapidityRange;
    _pRapidityRange =  rhs._pRapidityRange;
    _rhoMax   =  rhs._rhoMax;
    _tauI     =  rhs._tauI;
    _tauC     =  rhs._tauC;
    _lambda   =  rhs._lambda;
    _amp      =  rhs._amp;
    _delay    =  rhs._delay;
    _sVolume  =  rhs._sVolume;
    _pVolume  =  rhs._pVolume;
    _volume   =  rhs._volume;
    _tGamma   =  rhs._tGamma;
    _position =  rhs._position;
    _momentum =  rhs._momentum;
    }
  return *this;
  }


} // namespace CAP
