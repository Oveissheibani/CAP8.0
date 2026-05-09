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
#include "Factory.hpp"
#include "Particle.hpp"
#include "PrintHelpers.hpp"

ClassImp(CAP::Particle);
ClassImp(CAP::Factory<CAP::Particle>);


namespace CAP
{

Particle::Particle()
:
_momentum (),
_position (),
_type     (nullptr),
_parent   (nullptr),
_children (),
_truth    (nullptr),
_live     (false),
_pid      (-1),
_sourceIndex(-1),
_ixEtaPhi (0),
_ixYPhi   (0)
{  }

Particle::Particle(const Particle& other)
:
_momentum (other._momentum),
_position (other._position),
_type     (other._type),
_parent   (other._parent),
_children (other._children),
_truth    (other._truth),
_live     (other._live),
_pid      (other._pid),
_sourceIndex(other._sourceIndex),
_ixEtaPhi(other._ixEtaPhi),
_ixYPhi(other._ixYPhi)
{  }

Particle & Particle::operator=(const Particle & other)
{
  if (&other != this)
    {
    _ixEtaPhi    = other._ixEtaPhi;
    _ixYPhi      = other._ixYPhi;
    _momentum    = other._momentum;
    _position    = other._position;
    _type        = other._type;
    _parent      = other._parent;
    _children    = other._children;
    _truth       = other._truth;
    _live        = other._live;
    _pid         = other._pid;
    _sourceIndex = other._sourceIndex;
    }
  return *this;
}

void Particle::clear()
{
  _type       = nullptr;
  _live       = false;
  _pid        = -1;
  _ixEtaPhi   = -1;
  _ixYPhi     = -1;
  _momentum.setTXYZ (0.0,0.0,0.0,0.0);
  _position.setTXYZ(0.0,0.0,0.0,0.0);
  _parent = nullptr;
  _children.clear();
  _truth = nullptr;
}

void Particle::reset()
{
  _live       = false;
  _pid        = -1;
  _ixEtaPhi   = -1;
  _ixYPhi     = -1;
  _momentum.setTXYZ (0.0,0.0,0.0,0.0);
  _position.setTXYZ(0.0,0.0,0.0,0.0);
  _parent = nullptr;
  _children.clear();
  _truth = nullptr;
}

void Particle::set(ParticleType * type,
                   double p_x, double p_y, double p_z, double p_e,
                   double x,  double y,  double z,  double t,
                   bool live)
{
  //clear();
  _type = type;
  _momentum.setTXYZ(p_e,p_x,p_y,p_z);
  _position.setTXYZ(t,x,y,z);
  _live       = live;
  _parent     = nullptr;
  _children.clear();
  _truth = nullptr;
}

void Particle::set(ParticleType * type,
                   VectorLorentz & momentum,
                   VectorLorentz & position,
                   bool live)
{
  clear();
  _type       = type;
  _momentum   = momentum;
  _position   = position;
  _live       = live;
}

void Particle::set(ParticleType * type,
                   VectorLorentz & momentum,
                   Particle* parent,
                   bool live)
{
  clear();
  _type       = type;
  _momentum   = momentum;
  _live       = live;
  _parent     = parent;
}

//!
//! Print the properties of this Particle at the given output
//!
void Particle::print(unsigned int style)  const
{
  switch (style)
    {
      case 0:
      {
      printLine();
      printValue("PDG",_type->pdgCode());
      printValue("Private",_type->privateCode());
      printValue("Name",_type->name());
      printValue("title",_type->title());
      printValue("charge",_type->charge());
      printValue("px",_momentum.x());
      printValue("py",_momentum.y());
      printValue("pt",_momentum.perp());
      printValue("pz",_momentum.z());
      printValue("e",_momentum.t());
      printValue("Y",_momentum.rapidity());
      printValue("eta",_momentum.pseudorapidity());
      printValue("phi",_momentum.phi());
      printValue("x",_position.x());
      printValue("y",_position.y());
      printValue("z",_position.z());
      printValue("t",_position.t());
      printValue("live",_live);
      printValue("pid",_pid);
      printValue("ixEtaPhi",_ixEtaPhi);
      printValue("ixYPhi",_ixYPhi);
      break;
      }
      case 1:
      {
      printValue("PDG",_type->pdgCode());
      printValue("px",_momentum.x());
      printValue("py",_momentum.y());
      printValue("pt",_momentum.perp());
      printValue("pz",_momentum.z());
      printValue("e",_momentum.t());
      break;
      }
      case 2:
      {
      printValue("PDG",_type->pdgCode());
      printValue("pt",_momentum.perp());
      printValue("Y",_momentum.rapidity());
      printValue("phi",_momentum.phi());
      break;
      }
    }
}



void Particle::setTRThetaPhi(double t, double r, double theta, double phi)
{
  double rsinTh = r*std::sin(theta);
  double x = rsinTh*std::cos(phi);
  double y = rsinTh*std::sin(phi);
  double z = r*std::cos(theta);
  _position.setTXYZ(t,x,y,z);
}

void Particle::setTRCosThetaPhi(double t, double r, double cosTheta, double phi)
{
  double rsinTh = r*std::sqrt(1.0-cosTheta*cosTheta);
  double x = rsinTh*std::cos(phi);
  double y = rsinTh*std::sin(phi);
  double z = r*cosTheta;
  _position.setTXYZ (t,x,y,z);
}

void Particle::boost(double ax, double ay, double az)
{
  _momentum.boost(ax,ay,az);
  for (auto & child : _children)   child->boost(ax,ay,az);
}

void Particle::boostRapidity(double boost)
{
  double rapidity = _momentum.rapidity();
  rapidity += boost;
  double px = _momentum.x();
  double py = _momentum.y();
  double pz = _momentum.z();
  double e  = _momentum.t();
  double mt = _momentum.transverseMass();
  pz = mt * std::sinh(rapidity);
  e  = mt * std::cosh(rapidity);
  _momentum.setTXYZ (e,px,py,pz);
  for (auto & child : _children) child->boostRapidity(boost);
}

double Particle::distanceXYZSq(Particle & otherParticle)  const
{
  return _position.distance3DSq(otherParticle._position);
}

double Particle::distanceXYZ(Particle & otherParticle)  const
{
  return _position.distance3D(otherParticle._position);
}

double Particle::distanceXYSq(Particle & otherParticle)  const
{
  double dx = _position.x() - otherParticle._position.x();
  double dy = _position.y() - otherParticle._position.y();
  return dx*dx + dy*dy;
}

double Particle::distanceXY(Particle & otherParticle)  const
{
  return std::sqrt(distanceXYSq(otherParticle));
}

bool Particle::_defaultOwner = true;
long Particle::_defaultSize  = 5000;
long Particle::_defaultSizeIncrement = 1000;
Factory<Particle> * Particle::_factory = nullptr;

void Particle::setFactoryDefaultOwner(bool defaultOwner)
{
  _defaultOwner = defaultOwner;
}

void Particle::setFactoryDefaultCapacity(long defaultSize)
{
  _defaultSize = defaultSize;
}

void Particle::setFactoryDefaultIncrement(long defaultSizeIncrement)
{
  _defaultSizeIncrement = defaultSizeIncrement;
}

Factory<Particle> & Particle::factory()
{
  if (!_factory)
    {
    _factory = new Factory<Particle>(_defaultSize,_defaultSizeIncrement,_defaultOwner);
    _factory->initialize();
    }
  return *_factory;
}


// Particle Interaction 1->1
// Considered a decay vertex
void Particle::setParent(Particle * parent)
{
  _parent = parent;
}

void Particle::addChild(Particle* child)
{
  _children.push_back(child); child->setPosition(_position);
}

void Particle::addChildren(Particle* child1, Particle* child2)
{
  _children.push_back(child1); child1->setPosition(_position);
  _children.push_back(child2); child2->setPosition(_position);
}

void Particle::addChildren(Particle* child1, Particle* child2, Particle* child3)
{
  _children.push_back(child1); child1->setPosition(_position);
  _children.push_back(child2); child2->setPosition(_position);
  _children.push_back(child3); child3->setPosition(_position);
}

void Particle::addChildren(const vector<Particle*> & newChildren)
{
  for (auto & newChild : newChildren)
    {
    newChild->setPosition(_position);
    _children.push_back(newChild);
    }
}


Particle * Particle::proton()
{
  Particle * particle = new Particle();
  particle->setType(ParticleType::protonType);
  return particle;
}

Particle * Particle::neutron()
{
  Particle * particle = new Particle();
  particle->setType(ParticleType::neutronType);
  return particle;
}

bool Particle::isPrimary() const
{
  bool result = true;
  if (_parent) result = false;
  return result;
}

bool Particle::isSecondary() const
{
  bool result = false;
  if (_parent) result = true;
  return result;
}

  int Particle::pdgCode() const 
  {
  if (_type==nullptr)
    throw Exception("Particle has no type",__FUNCTION__);
  return _type->pdgCode();
  }

  double Particle::mass() const
  {
  if (_type==nullptr)
    throw Exception("Particle has no type",__FUNCTION__);
  return _type->mass();
  }

  double Particle::lifeTime() const
  {
  if (_type==nullptr)
    throw Exception("Particle has no type",__FUNCTION__);
  return _type->lifeTime();
  }

  const String & Particle::name() const
  {
  if (_type==nullptr)
    throw Exception("Particle has no type",__FUNCTION__);
  return _type->name();
  }

  const String & Particle::title() const
  {
  if (_type==nullptr)
    throw Exception("Particle has no type",__FUNCTION__);
  return _type->title();
  }


} // namespace CAP
