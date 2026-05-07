/* **********************************************************************
 * Copyright (C) 2019-2024, Claude Pruneau
 * All rights reserved.
 *
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 *
 * Author: Claude Pruneau,   04/01/2024
 *
 * *********************************************************************/
#include "MeasurementSimulator.hpp"
#include "NameHelpers.hpp"
#include "PrintHelpers.hpp"
#include "RootHistogramHelpers.hpp"
#include "TRandom.h"

ClassImp(CAP::MeasurementSimulator);

namespace CAP
{

MeasurementSimulator::MeasurementSimulator()
:
EventProcessor()
{
  appendClassName("MeasurementSimulator");
  setName("MeasurementSimulator");
  setTitle("MeasurementSimulator");
}

MeasurementSimulator::MeasurementSimulator(const MeasurementSimulator & source)
:
EventProcessor(source)
{  }

MeasurementSimulator & MeasurementSimulator::operator=(const MeasurementSimulator & rhs)
{
  if (this!=&rhs)
    {
    EventProcessor::operator=(rhs);
    }
  return *this;
}

void MeasurementSimulator::setDefaultConfiguration()
{
  EventProcessor::setDefaultConfiguration();
}

void MeasurementSimulator::execute()
{
  std::vector<Event*> & theEvents = events();
  Event & genEvent  = *(theEvents[0]);
  Event & recoEvent = *(theEvents[1]);
  recoEvent.reset();
  if (!filterEvent(genEvent)) return;
  if (!filterParticles(genEvent.particles()))  return;
  unsigned int nef = nEventFilters();
  unsigned int npf = nParticleFilters();
  unsigned int baseSingle;
  unsigned int index;
  std::vector<ParticleSmearer*> & smearers = particleSmearers();
  for (unsigned int iEventFilter=0; iEventFilter<nef;iEventFilter++)
    {
    if (!_acceptedEventFilters[iEventFilter]) continue;
    baseSingle = iEventFilter*npf;
    for (unsigned int iParticleFilter=0; iParticleFilter<npf;iParticleFilter++)
      {
      if (!_acceptedParticleFilters[iParticleFilter]) continue;
      index = baseSingle + iParticleFilter;
      ParticleSmearer * particleSmearer = smearers[index];
      std::vector<Particle*> & particlesSelected = acceptedParticles(iParticleFilter);
      for (auto & genParticle : particlesSelected)
        {
        Particle & recoParticle = Particle::factory().nextObject();
        particleSmearer->smear(*genParticle,recoParticle);
        recoEvent.addParticle(&recoParticle);
        }
      }
    }
}

} // namespace CAP
