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
#include "EposEventReader.hpp"

ClassImp(CAP::EposEventReader);

namespace CAP
{


EposEventReader::EposEventReader()
:
EventProcessor(),
RootChainManager()
{
  appendClassName("EposEventReader");
  setMinimumReportLevel(Object::kInfo);
  setName("EposEventReader");
  setTitle("EposEventReader");
}

void EposEventReader::setDefaultConfiguration()
{
  EventProcessor::setDefaultConfiguration();
}

void EposEventReader::execute()
{
  Event & theEvent = event();
  event.reset();
  Particle::factory().reset();
  Long64_t ientry = LoadTree(entryIndex);
  if (ientry < 0)
    throw EndOfDataException("EPOS File","EPOS input stream is empty",__FUNCTION__);
  nb = rootInputTreeChain()->GetEntry(entryIndex++);
  nBytes += nb;
  if (nParticles > arraySize)
    throw Exception("nParticles > arraySize",__FUNCTION__);
  double eventPhi;
  double cosPhi;
  double sinPhi;
  if (useRandomizeEventPlane())
    {
    eventPhi = getRandomEventPlaneAngle();
    cosPhi = std::cos(eventPhi);
    sinPhi = std::sin(eventPhi);
    }
  
  VectorLorentz & sourcePosition = parentInteraction->position();
  double r_x = sourcePosition.X();
  double r_y = sourcePosition.Y();
  double r_z = sourcePosition.Z();
  double r_t = sourcePosition.T();
  ParticleType * type;
  
  for (int iParticle=0; iParticle<nParticles; iParticle++)
    {
    int pdgCode = pid[iParticle];
    type = particleTypeList->findPdgCode(pdgCode);
    if (type==nullptr)
      {
      if (reportWarning(__FUNCTION__)) cout << "Encountered unknown pdgCode: " << pdgCode << " Particle not added to event." << endl;
      continue;
      }

    Particle * particle = Particle::factory().nextObject();
    double p_x, p_y, p_z, p_e;
    double mass = type->mass();
    if (useRandomizeEventPlane())
      {
      p_x  = cosPhi*px[iParticle] - sinPhi*py[iParticle];
      p_y  = sinPhi*px[iParticle] + cosPhi*py[iParticle];
      }
    else
      {
      p_x  = px[iParticle];
      p_y  = py[iParticle];
      }
    p_z  = pz[iParticle];
    p_e  = std::sqrt(p_x*p_x + p_y*p_y + p_z*p_z + mass*mass);
    particle->set(type,p_x,p_y,p_z,p_e,r_x,r_y,r_z,r_t,true);
    if (!particleFilters[0].accept(*particle)) continue;
    incrementNacceptedParticles();
    event().add(particle);
    }
  event.setEventNumber(events);
//  EventProperties & eventProperties = * event.eventProperties();
//  eventProperties.zProjectile           = 0;
//  eventProperties.aProjectile           = 0;
//  eventProperties.nPartProjectile       = 0;
//  eventProperties.zTarget               = 0;
//  eventProperties.aTarget               = 0;
//  eventProperties.nPartTarget           = 0;
//  eventProperties.nParticipantsTotal    = 0;
//  eventProperties.nBinaryTotal          = 0;
//  eventProperties.impactParameter       = impact;
//  eventProperties.fractionalXSection    = -99999;
}

void EposEventReader::initialize()
{
  EventProcessorSingle<Event,EventFilter,EventEfficiency,EventSmearer,Particle,ParticleFilter,PARTICLE_EFFICIENCY,
  ParticleEfficiency,BaseHisto,BaseHistoDerived>::::initialize();
  particleTypeList = ManagedList<ParticleTypeList>::getObjects();
  eventFilters = ManagedList<EventFilter>::getObjects();
  particleFilters = ManagedList<ParticleFilter>::getObjects();
  
  TTree * tree = rootInputTreeChain();
  tree->SetMakeClass(1);
  tree->SetBranchAddress("Events", & events,  & b_Events);
  tree->SetBranchAddress("Mult", & nParticles, & b_Mult);
  tree->SetBranchAddress("Impact", & impact,  & b_Impact);
  tree->SetBranchAddress("PID", pid, & b_PID);
  tree->SetBranchAddress("Px", px, & b_Px);
  tree->SetBranchAddress("Py", py, & b_Py);
  tree->SetBranchAddress("Pz", pz, & b_Pz);
  tree->SetBranchAddress("E", e, & b_E);
}


} // namespace CAP

