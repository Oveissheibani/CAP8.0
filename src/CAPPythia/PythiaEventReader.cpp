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
#include "PythiaEventReader.hpp"

ClassImp(CAP::PythiaEventReader);

namespace CAP
{

PythiaEventReader::PythiaEventReader()
:
ProcessorSingle(),
RootChainManager(),
saveFinalOnly(true),
savePhotons(false),
saveNeutrinos(false),
saveQuarks(false),
saveGaugeBosons(false)
{
 appendClassName("PythiaEventReader");
 setName("PythiaEventReader");
 setTitle("PythiaEventReader");
}

PythiaEventReader::PythiaEventReader(const PythiaEventReader & source)
:
EventProcessor(source),
RootChainManager(source),
saveFinalOnly(source.saveFinalOnly),
savePhotons(source.savePhotons),
saveNeutrinos(source.saveNeutrinos),
saveQuarks(source.saveQuarks),
saveGaugeBosons(source.saveGaugeBosons)
{  }

PythiaEventReader & PythiaEventReader::operator=(const PythiaEventReader & rhs)
{
  if (this!=&rhs)
    {
    EventProcessor::operator=(rhs);
    RootChainManager::operator=(rhs);
    saveFinalOnly    = rhs.saveFinalOnly;
    savePhotons      = rhs.savePhotons;
    saveNeutrinos    = rhs.saveNeutrinos;
    saveQuarks       = rhs.saveQuarks;
    saveGaugeBosons  = rhs.saveGaugeBosons;
    }
  return *this;
}


void PythiaEventReader::setDefaultConfiguration()
{
  if (reportDebug(__FUNCTION__)) { printCR(); }
  ProcessorSingle::setDefaultConfiguration();
  const String & taskName = name();
  _configuration.addProperty(createKey(taskName,"SaveFinalOnly"),    true);
  _configuration.addProperty(createKey(taskName,"SavePhotons"),      false);
  _configuration.addProperty(createKey(taskName,"SaveNeutrinos"),    false);
  _configuration.addProperty(createKey(taskName,"SaveQuarks"),       false);
  _configuration.addProperty(createKey(taskName,"SaveGaugeBosons"),  false);
}

//!
//! Read a PYTHIA  event from file
//! Copy the event into Event for convenience..
//!
void PythiaEventReader::execute()
{
  //if (reportDebug(__FUNCTION__)) { printCR(); }
  ParticleTypeList & particleTypeList = ParticleTypeList::list();
  Event & theEvent = event();
  event.reset();
  Particle::factory().reset();
  bool seekingEvent = true;
  while (seekingEvent)
    {
    Long64_t ientry = LoadTree(entryIndex++);
    if (ientry < 0) return;
    nb = GetEntry(entryIndex);   nBytes += nb;
    if (reportDebug(__FUNCTION__)) cout << " nb:" << nb << " nParticles:" <<  nParticles << endl;
    if (nParticles>2) seekingEvent = false;
    }
  ParticleType * type;

  for (int iParticle = 0; iParticle < nParticles; iParticle++)
    {
    int ist = tracks_fStatusCode[iParticle];
    if (saveFinalOnly    &&  ist <= 0) continue;
    int pdg = tracks_fPdgCode[iParticle];
    if (!saveQuarks      &&  abs(pdg)<10) continue; // skip quarks, gluons, etc
    if (!saveNeutrinos   &&  (abs(pdg)==12 || abs(pdg)==14  || abs(pdg)==16 || abs(pdg)==18)) continue;
    if (!savePhotons     &&  pdg==22) continue;
    if (!saveGaugeBosons  &&  (abs(pdg)>22  &&  abs(pdg)<40)) continue;
    type = particleTypeList.findPdgCode(pdg);
    if (type==nullptr) continue;
    double px = tracks_fPx[iParticle];
    double py = tracks_fPy[iParticle];
    double pz = tracks_fPz[iParticle];
    double e  = tracks_fE[iParticle];
    double sourceX = 0.0;
    double sourceY = 0.0;
    double sourceZ = 0.0;
    double sourceT = 0.0;
    Particle & particle = Particle::factory().nextObject();
    particle.set(type,px,py,pz,e,sourceX,sourceY,sourceZ,sourceT,true);
    event.addParticle(&particle);
    }
  _taskExecuted.increment();;
}

void PythiaEventReader::initialize()
{
  if (reportDebug(__FUNCTION__)) { printCR(); }
  ProcessorSingle::initialize();
  const String & taskName = name();
  saveFinalOnly   = _configuration.valueBool(createKey(taskName,"SaveFinalOnly"));
  savePhotons     = _configuration.valueBool(createKey(taskName,"SavePhotons"));
  saveNeutrinos   = _configuration.valueBool(createKey(taskName,"SaveNeutrinos");
  saveQuarks      = _configuration.valueBool(createKey(taskName,"SaveQuarks"));
  saveGaugeBosons = _configuration.valueBool(createKey(taskName,"SaveGaugeBosons");

  TTree * tree = chain();
  tree->SetMakeClass(1);

  tree->SetBranchAddress("tracks", & nParticles, & b_tracks_);
  tree->SetBranchAddress("tracks.fUniqueID", tracks_fUniqueID, & b_tracks_fUniqueID);
  tree->SetBranchAddress("tracks.fBits", tracks_fBits, & b_tracks_fBits);
  tree->SetBranchAddress("tracks.fLineColor", tracks_fLineColor, & b_tracks_fLineColor);
  tree->SetBranchAddress("tracks.fLineStyle", tracks_fLineStyle, & b_tracks_fLineStyle);
  tree->SetBranchAddress("tracks.fLineWidth", tracks_fLineWidth, & b_tracks_fLineWidth);
  tree->SetBranchAddress("tracks.fPdgCode", tracks_fPdgCode, & b_tracks_fPdgCode);
  tree->SetBranchAddress("tracks.fStatusCode", tracks_fStatusCode, & b_tracks_fStatusCode);
  tree->SetBranchAddress("tracks.fMother[2]", tracks_fMother, & b_tracks_fMother);
  tree->SetBranchAddress("tracks.fDaughter[2]", tracks_fDaughter, & b_tracks_fDaughter);
  tree->SetBranchAddress("tracks.fWeight", tracks_fWeight, & b_tracks_fWeight);
  tree->SetBranchAddress("tracks.fCalcMass", tracks_fCalcMass, & b_tracks_fCalcMass);
  tree->SetBranchAddress("tracks.fPx", tracks_fPx, & b_tracks_fPx);
  tree->SetBranchAddress("tracks.fPy", tracks_fPy, & b_tracks_fPy);
  tree->SetBranchAddress("tracks.fPz", tracks_fPz, & b_tracks_fPz);
  tree->SetBranchAddress("tracks.fE", tracks_fE, & b_tracks_fE);
  tree->SetBranchAddress("tracks.fVx", tracks_fVx, & b_tracks_fVx);
  tree->SetBranchAddress("tracks.fVy", tracks_fVy, & b_tracks_fVy);
  tree->SetBranchAddress("tracks.fVz", tracks_fVz, & b_tracks_fVz);
  tree->SetBranchAddress("tracks.fVt", tracks_fVt, & b_tracks_fVt);
  tree->SetBranchAddress("tracks.fPolarTheta", tracks_fPolarTheta, & b_tracks_fPolarTheta);
  tree->SetBranchAddress("tracks.fPolarPhi", tracks_fPolarPhi, & b_tracks_fPolarPhi);
  tree->SetBranchAddress("HP_Inc1", & HP_Inc1, & b_HP_Inc1);
  tree->SetBranchAddress("HP_Inc2", & HP_Inc2, & b_HP_Inc2);
  tree->SetBranchAddress("HP_Inc3", & HP_Inc3, & b_HP_Inc3);
  tree->SetBranchAddress("HP_Inc4", & HP_Inc4, & b_HP_Inc4);
  tree->SetBranchAddress("HP_Out1", & HP_Out1, & b_HP_Out1);
  tree->SetBranchAddress("HP_Out2", & HP_Out2, & b_HP_Out2);
  tree->SetBranchAddress("HP_Out3", & HP_Out3, & b_HP_Out3);
  tree->SetBranchAddress("HP_Out4", & HP_Out4, & b_HP_Out4);
  tree->SetBranchAddress("HP_Inter1", & HP_Inter1, & b_HP_Inter1);
  tree->SetBranchAddress("HP_Inter2", & HP_Inter2, & b_HP_Inter2);
  tree->SetBranchAddress("HP_Inter3", & HP_Inter3, & b_HP_Inter3);
  tree->SetBranchAddress("HP_Inter4", & HP_Inter4, & b_HP_Inter4);
  tree->SetBranchAddress("HP_Kick1", & HP_Kick1, & b_HP_Kick1);
  tree->SetBranchAddress("HP_Kick2", & HP_Kick2, & b_HP_Kick2);
  tree->SetBranchAddress("HP_Kick3", & HP_Kick3, & b_HP_Kick3);
  tree->SetBranchAddress("HP_Kick4", & HP_Kick4, & b_HP_Kick4);
}



} // namespace CAP
