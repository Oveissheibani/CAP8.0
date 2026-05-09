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
#include "BasicEventGen.hpp"
#include "MathConstants.hpp"
#include "VectorLorentz.hpp"

ClassImp(CAP::BasicEventGen);

namespace CAP
{

BasicEventGen::BasicEventGen()
:
EventProcessor(),
pdgSelected(113),
multMin(2.0),
multMax(20.0),
rapidityMin(-2.0),
rapidityMax(2.0),
ptSlope(.5),
mass(0),
massSq(0)
{
  appendClassName("BasicEventGen");
  setMinimumReportLevel(Object::kInfo);
  setName("BasicEventGen");
  setTitle("BasicEventGen");
}

BasicEventGen::BasicEventGen(const BasicEventGen & source)
:
EventProcessor(source),
pdgSelected(source.pdgSelected),
multMin(source.multMin),
multMax(source.multMax),
rapidityMin(source.rapidityMin),
rapidityMax(source.rapidityMax),
ptSlope(source.ptSlope),
mass(source.mass),
massSq(source.massSq)
{ }

BasicEventGen & BasicEventGen::operator=(const BasicEventGen & rhs)
{
  if (this!=&rhs)
    {
    EventProcessor::operator=(rhs);
    pdgSelected = rhs.pdgSelected;
    multMin     = rhs.multMin;
    multMax     = rhs.multMax;
    rapidityMin = rhs.rapidityMin;
    rapidityMax = rhs.rapidityMax;
    ptSlope     = rhs.ptSlope;
    mass        = rhs.mass;
    massSq      = rhs.massSq;
    }
  return *this;
}

void BasicEventGen::setDefaultConfiguration()
{
  EventProcessor::setDefaultConfiguration();
  String taskName = name();
  _configuration.addProperty(createKey(taskName,"MultMin"),        multMin);
  _configuration.addProperty(createKey(taskName,"MultMax"),        multMax);
  _configuration.addProperty(createKey(taskName,"PdgSelected"),    pdgSelected);
  _configuration.addProperty(createKey(taskName,"RapidityMin"),    rapidityMin);
  _configuration.addProperty(createKey(taskName,"RapidityMax"),    rapidityMax);
  _configuration.addProperty(createKey(taskName,"PtSlope"),        ptSlope);
  _configuration.addProperty(createKey(taskName,"Random:SetSeed"), true);
  _configuration.addProperty(createKey(taskName,"Random:Seed"),    14316431);
}

void BasicEventGen::initialize()
{
  if (reportStart(__FUNCTION__)) { printCR(); }
  EventProcessor::initialize();
  ParticleDb & particleDb = EventProcessor::db();
  String taskName = name();
  setSeed     = _configuration.valueBool(  createKey(taskName,"Random:SetSeed"));
  seedValue   = _configuration.valueLong(  createKey(taskName,"Random:Seed"));
  multMin     = _configuration.valueDouble(createKey(taskName,"MultMin"));
  multMax     = _configuration.valueDouble(createKey(taskName,"MultMax"));
  rapidityMin = _configuration.valueDouble(createKey(taskName,"RapidityMin"));
  rapidityMax = _configuration.valueDouble(createKey(taskName,"RapidityMax"));
  ptSlope     = _configuration.valueDouble(createKey(taskName,"PtSlope"));
  if (reportDebug(__FUNCTION__))
    {
    printCR();
    printValue("multMin",multMin);
    printValue("multMax",multMax);
    printValue("rapidityMin",rapidityMin);
    printValue("rapidityMax",rapidityMax);
    printValue("ptSlope",ptSlope);
    printValue("particleDb.size()",particleDb.size());
    printValue("Random:SetSeed",setSeed);
    printValue("Random:Seed",seedValue);
    }

  //pdgSelected = _configuration.valueInt("PdgSelected");
  mass.clear();
  massSq.clear();
  types.clear();
//  types.push_back(particleDb.findPdgCode(pdgSelected)); //113
  types.push_back(particleDb.findPdgCode(211));
  types.push_back(particleDb.findPdgCode(-211));
  types.push_back(particleDb.findPdgCode(113));
  //types.push_back(particleDb.findPdgCode(323));
  //types.push_back(particleDb.findPdgCode(-323));
  types.push_back(particleDb.findPdgCode(321));
  types.push_back(particleDb.findPdgCode(-321));
  for (ParticleType * type : types)
    {
    double m = type->mass();
    mass.push_back(m);
    massSq.push_back(m*m);
    }
}

void BasicEventGen::execute()
{
  Event & theEvent = event();
  //event.reset();
  double phi;
  double rapidity;
  double pt;
  double mt;
  double r;
  VectorLorentz momentum;
  VectorLorentz position;
  position.setTXYZ(0.0, 0.0, 0.0, 0.0);
  for (unsigned int iType = 0; iType<types.size(); iType++)
    {
    r = gRandom->Rndm();
    int nParticles = r*100.0;
    for (int i = 0; i < nParticles; i++)
      {
      phi      = MATH::twoPi() * gRandom->Rndm();
      r        = gRandom->Rndm();
      rapidity = rapidityMin + r*(rapidityMax-rapidityMin);
      pt       = gRandom->Exp(ptSlope);
      mt       = std::sqrt(massSq[iType] + pt*pt);
      momentum.setTXYZ(mt*std::cosh(rapidity),pt*std::cos(phi),pt*std::sin(phi),mt*std::sinh(rapidity));
      Particle & particle = Particle::factory().nextObject();
      particle.reset();
      particle.setType(types[iType]);
      particle.setLive(1);
      particle.setMomentum(momentum);
      particle.setPosition(position);
      for (auto & particleFilter : particleFilters())
        {
        if (particleFilter->accept(particle)) theEvent.addParticle(&particle);
        }
      }
    }
  _eventAccepted.increment();
  _taskExecuted.increment();
}

} // namespace CAP
