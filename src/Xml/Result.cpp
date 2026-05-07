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
using CAP::Factory;
using CAP::Particle;

ClassImp(Factory<Particle>);

ClassImp(Particle);

Particle::Particle()
:
momentum (),
position (),
type     (nullptr),
parents  (),
children (),
truth    (nullptr),
live     (false),
pid      (-1),
sourceIndex(-1),
ixEtaPhi (0),
ixYPhi   (0)
{  }

Particle::Particle(const Particle& other)
:
momentum (other.momentum),
position (other.position),
type     (other.type),
parents  (other.parents),
children (other.children),
truth    (other.truth),
live     (other.live),
pid      (other.pid),
sourceIndex(other.sourceIndex),
ixEtaPhi(other.ixEtaPhi),
ixYPhi(other.ixYPhi)
{  }

Particle & Particle::operator=(const Particle & other)
{
  if (&other != this)
    {
    ixEtaPhi    = other.ixEtaPhi;
    ixYPhi      = other.ixYPhi;
    momentum    = other.momentum;
    position    = other.position;
    type        = other.type;
    parents     = other.parents;
    children    = other.children;
    truth       = other.truth;
    live        = other.live;
    pid         = other.pid;
    sourceIndex = other.sourceIndex;
    }
  return *this;
}

void Particle::clear()
{
  type       = nullptr;
  live       = false;
  pid        = -1;
  ixEtaPhi   = -1;
  ixYPhi     = -1;
  momentum.SetPxPyPzE (0.0,0.0,0.0,0.0);
  _position.SetXYZT    (0.0,0.0,0.0,0.0);
  parents.clear();
  children.clear();
  truth = nullptr;
}

void Particle::reset()
{
  live       = false;
  pid        = -1;
  ixEtaPhi   = -1;
  ixYPhi     = -1;
  momentum.SetPxPyPzE (0.0,0.0,0.0,0.0);
  _position.SetXYZT    (0.0,0.0,0.0,0.0);
  parents.clear();
  children.clear();
  truth = nullptr;
}

void Particle::set(ParticleType * _type,
                   double p_x, double p_y, double p_z, double p_e,
                   double _x,  double _y,  double _z,  double _t,
                   bool _live)
{
  //clear();
  type = _type;
  momentum.SetPxPyPzE (p_x,p_y,p_z,p_e);
  _position.SetXYZT    (_x,_y,_z,_t);
  live       = _live;
  parents.clear();
  children.clear();
  truth = nullptr;
}

void Particle::set(ParticleType * _type,
                   VectorLorentz & _momentum,
                   VectorLorentz & _position,
                   bool _live)
{
  clear();
  type       = _type;
  momentum   = _momentum;
  position   = _position;
  live       = _live;
}

void Particle::set(ParticleType * _type,
                   VectorLorentz & _momentum,
                   vector<Particle*> _parents,
                   bool _live)
{
  clear();
  type       = _type;
  momentum   = _momentum;
  live       = _live;
  setParents(_parents);
}

// ==================================================
// Print the properties of this Particle at the given output
// ==================================================
void Particle::print(ostream & output)
{
  output << "-------------------------------------" << endl;
  output << "                  PDG: " << type->pdgCode() << endl;
  output << "              Private: " << type->privateCode() << endl;
  output << "                 Name: " << type->name() << endl;
  output << "                Title: " << type->title() << endl;
  output << "               charge: " << type->charge() << endl;
  output << "                   px: " << momentum.x() << endl;
  output << "                   py: " << momentum.y() << endl;
  output << "                   pt: " << momentum.perp() << endl;
  output << "                   pz: " << momentum.z() << endl;
  output << "                    e: " << momentum.t() << endl;
  output << "                    Y: " << momentum.rapidity()  << endl;
  output << "                  eta: " << momentum.pseudorapidity() << endl;
  output << "                  phi: " << momentum.phi() << endl;
  output << "                    x: " << _position.x() << endl;
  output << "                    y: " << _position.y() << endl;
  output << "                    z: " << _position.z() << endl;
  output << "                    t: " << _position.t() << endl;
  output << "                 live: " << live      << endl;
  output << "                  pid: " << pid       << endl;
  output << "             ixEtaPhi: " << ixEtaPhi  << endl;
  output << "               ixYPhi: " << ixYPhi    << endl;
}


void Particle::setPidPxPyPzE(long _pid, double p_x, double p_y, double p_z, double p_e)
{
  pid = _pid;
  momentum.SetPxPyPzE (p_x,p_y,p_z,p_e);
}

void Particle::setRThetaPhiT(double r, double theta, double phi,double t)
{
  double rsinTh = r*sin(theta);
  double x = rsinTh*cos(phi);
  double y = rsinTh*sin(phi);
  double z = r*cos(theta);
  _position.SetXYZT (x,y,z,t);
}

void Particle::setRCosThetaPhiT(double r, double cosTheta, double phi,double t)
{
  double rsinTh = r*sqrt(1.0-cosTheta*cosTheta);
  double x = rsinTh*cos(phi);
  double y = rsinTh*sin(phi);
  double z = r*cosTheta;
  _position.SetXYZT (x,y,z,t);
}

void Particle::boost(double ax, double ay, double az)
{
  momentum.Boost(ax,ay,az);
  unsigned int nChildren = children.size();
  for (unsigned int iChildren=0; iChildren<nChildren; iChildren++)
    {
    children[iChildren]->boost(ax,ay,az);
    }
}

void Particle::boostRapidity(double boost)
{
  double rapidity = momentum.rapidity();
  rapidity += boost;
  double px = momentum.x();
  double py = momentum.y();
  double pz = momentum.z();
  double e  = momentum.t();
  double mt = sqrt(e*e - pz*pz);
  pz = mt * sinh(rapidity);
  e  = mt * cosh(rapidity);
  momentum.SetPxPyPzE (px,py,pz,e);
  unsigned int nChildren = children.size();
  for (unsigned int iChildren=0; iChildren<nChildren; iChildren++)
    {
    children[iChildren]->boostRapidity(boost);
    }
}


double Particle::distanceXYZSq(Particle  * otherParticle)  const
{
  double dx = _position.X() - otherParticle->_position.X();
  double dy = _position.Y() - otherParticle->_position.Y();
  double dz = _position.Z() - otherParticle->_position.Z();
  return dx*dx + dy*dy + dz*dz;
}

double Particle::distanceXYZ(Particle * otherParticle)  const
{
  return sqrt(distanceXYZSq(otherParticle));
}


double Particle::distanceXYSq(Particle * otherParticle)  const
{
  double dx = _position.X() - otherParticle->_position.X();
  double dy = _position.Y() - otherParticle->_position.Y();
  return dx*dx + dy*dy;
}

double Particle::distanceXY(Particle * otherParticle)  const
{
  return sqrt(distanceXYSq(otherParticle));
}

int Particle::factorySize = 5000;
Factory<Particle> * Particle::factory = 0;
Factory<Particle> * Particle::factory()
{
  if (!factory)
    {
    factory = new Factory<Particle>();
    factory->initialize(factorySize);
    }
  return factory;
}

void Particle::resetFactory()
{
  factory->reset();
}


// Particle Interaction 1->1
// Considered a decay vertex
void Particle::setParent(Particle * _parent)
{
  parents.clear();
  parents.push_back(_parent);
  momentum = _parent.momentum();
  position = _parent.position();

}

// Particle Interaction 2->X
void Particle::setParents(Particle * parent1, Particle * parent2)
{
  parents.clear();
  parents.push_back(parent1);
  parents.push_back(parent2);
  momentum = parent1->momentum(); momentum += parent2->momentum();
  position = parent1->position(); position += parent2->position();
  position *= 0.5;

}

// Particle Interaction 3->X
void Particle::setParents(Particle * parent1, Particle * parent2, Particle * parent3)
{
  parents.clear();
  parents.push_back(parent1);
  parents.push_back(parent2);
  parents.push_back(parent3);
  momentum = parent1->momentum(); momentum += parent2->momentum(); momentum += parent3->momentum();
  position = parent1->position(); position += parent2->position(); position += parent3->position();
  position *= 0.3333333333;

}

// Particle Interaction n->X
void Particle::setParents(const vector<Particle*> & newParents)
{
  parents.clear();
  Particle * parent = newParents[0];
  momentum = parent.momentum();
  position = parent.position();
  unsigned int nParents = newParents.size();
  for (unsigned int iParent=1;iParent<nParents;iParent++)
  {
  parent   =  newParents[iParent];
  momentum += parent.momentum();
  position += parent.position();
  parents.push_back(parent);
  }
  position *= 1.0/double(nParents);
}

void Particle::addChild(Particle* child)
{
  children.push_back(child); child->setPosition(position);
}

void Particle::addChildren(Particle* child1, Particle* child2)
{
  children.push_back(child1); child1.setPosition(position);
  children.push_back(child2); child2.setPosition(position);
}

void Particle::addChildren(Particle* child1, Particle* child2, Particle* child3)
{
  children.push_back(child1); child1.setPosition(position);
  children.push_back(child2); child2.setPosition(position);
  children.push_back(child3); child3.setPosition(position);
}

void Particle::addChildren(const vector<Particle*> & newChildren)
{
  for (unsigned int iChild=0;iChild<newChildren.size();iChild++)
    {
    Particle * child =  newChildren[iChild];
    child->setPosition(position);
    children.push_back(child);
    }
}


bool Particle::isNucleonNucleonInteraction() const
{
  bool result = false;
  if (parents.size() == 2  && 
      parents[0]->isNucleon()  && 
      parents[1]->isNucleon() ) result = true;
  return result;
}


bool Particle::isProtonProton() const
{
  bool result = false;
  if (parents.size() == 2  && 
      parents[0]->isProton()  && 
      parents[1]->isProton() ) result = true;
  return result;
}

bool Particle::isNeutronNeutron() const
{
  bool result = false;
  if (parents.size() == 2  && 
      parents[0]->isNeutron()  && 
      parents[1]->isNeutron() ) result = true;
  return result;
}

bool Particle::isProtonNeutron() const
{
  bool result = false;
  if ((parents.size() == 2)  && 
      ((parents[0]->isProton()  &&  parents[1]->isNeutron()) ||
       (parents[1]->isProton()  &&  parents[0]->isNeutron()))) result = true;
  return result;
}


Particle * Particle::proton()
{
  Particle * particle = new Particle();
  particle->setType(ParticleType::protonType());
  return particle;
}

Particle * Particle::neutron()
{
  Particle * particle = new Particle();
  particle->setType(ParticleType::neutronType());
  return particle;
}

bool Particle::isParticle() const
{
  // Returns false for vertices and interactions
  bool result = true;
  if (type->isDecay() ||
      type->isInteraction() ||
      type->isPPInteraction() ||
      type->isPNInteraction() ) result = false;
  return result;
}

bool Particle::isPrimary() const
{
  bool result = true;
  if (!isParticle()) result = false;
  else if (parents.size()==1  &&  parents[0]->isDecay()) result = false;
  return result;
}

bool Particle::isSecondary() const
{
  bool result = true;
  if (!isParticle()) result = false;
  else if (parents.size()<1) result = false;
  else if (!parents[0]->isDecay()) result = false;
  return result;
}
