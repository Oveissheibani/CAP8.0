/* **********************************************************************
 * Copyright (C) 2019-2024, Claude Pruneau, Victor Gonzalez   
 * All rights reserved.
 *
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 *
 * Author: Claude Pruneau,   04/01/2024
 *
 * *********************************************************************/
#include <ostream>
#include "ParticleDigit.hpp"
#include "Factory.hpp"
#include "PrintHelpers.hpp"

ClassImp(CAP::Factory<ParticleDigit>);
ClassImp(CAP::ParticleDigit);

namespace CAP
{
  
  ParticleDigit::ParticleDigit()
  :
  _iY(0),
  _iEta(0),
  _iPhi(0),
  _iPt(0),
  _pt(0),
  _phi(0),
  _eta(0),
  _y(0),
  _e(0)
  {  }
  
  ParticleDigit::ParticleDigit(unsigned int iY,
                               unsigned int iEta,
                               unsigned int iPhi,
                               unsigned int iPt,
                               float       pt,
                               float       phi,
                               float       eta,
                               float       y,
                               float       e)
  :
  _iY(iY),
  _iEta(iEta),
  _iPhi(iPhi),
  _iPt(iPt),
  _pt(pt),
  _phi(phi),
  _eta(eta),
  _y(y),
  _e(e)
  {  }
  
  ParticleDigit::ParticleDigit(const ParticleDigit& other)
  :
  _iY(other._iY),
  _iEta(other._iEta),
  _iPhi(other._iPhi),
  _iPt(other._iPt),
  _pt(other._pt),
  _phi(other._phi),
  _eta(other._eta),
  _y(other._y),
  _e(other._e)
  {  }
  
  ParticleDigit & ParticleDigit::operator=(const ParticleDigit & other)
  {
  if (&other != this)
    {
    _iY   = other._iY;
    _iEta = other._iEta;
    _iPhi = other._iPhi;
    _iPt  = other._iPt;
    _pt   = other._pt;
    _phi  = other._phi;
    _eta  = other._eta;
    _y    = other._y;
    _e    = other._e;
    }
  return *this;
  }
  
  
  
  void ParticleDigit::reset()
  {
  _iY   = 0;
  _iEta = 0;
  _iPhi = 0;
  _iPt  = 0;
  _pt   = 0.0;
  _phi  = 0.0;
  _eta  = 0.0;
  _y    = 0.0;
  _e    = 0.0;
  }
  
  
  
  //!
  //! Print the properties of this ParticleDigit at the given output
  //!
  void ParticleDigit::print(ostream & output)
  {
  output << "-------------------------------------" << endl;
  output << "              iY: " << _iY << endl;
  output << "            iEta: " << _iEta << endl;
  output << "            iPhi: " << _iPhi << endl;
  output << "             iPt: " << _iPt << endl;
  output << "              pt: " << _pt << endl;
  output << "             phi: " << _phi << endl;
  output << "             eta: " << _eta << endl;
  output << "               y: " << _y << endl;
  output << "               e: " << _e  << endl;
  }
  
  
  int ParticleDigit::_factorySize = 5000;
  Factory<ParticleDigit> * ParticleDigit::_factory = 0;
  Factory<ParticleDigit> * ParticleDigit::factory()
  {
  if (!_factory)
    {
    _factory = new Factory<ParticleDigit>();
    _factory->initialize(factorySize);
    }
  return _factory;
  }
} // namespace cap
