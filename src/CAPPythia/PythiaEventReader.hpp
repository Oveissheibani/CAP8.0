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
#ifndef CAP__PythiaEventReader
#define CAP__PythiaEventReader
#include "EventProcessor.hpp"
#include "RootChainManager.hpp"
#include "EventProcessor.hpp"
#include "Event.hpp"
#include "EventFilter.hpp"
#include "EventEfficiency.hpp"
#include "EventSmearer.hpp"
#include "Particle.hpp"
#include "ParticleFilter.hpp"
#include "ParticleEfficiency.hpp"
#include "ParticleSmearer.hpp"

namespace CAP
{
//!
//! Class defining a ROOT Tree reader for PYTHIA events produced by Pythia8.
//! The Init() method maps the tree branch onto variables used by this reader
//! Particles may be copied into a CAP event stream for analysis.
//!
class PythiaEventReader
:
public EventProcessor,
RootChainManager
{
public:

  PythiaEventReader();
  PythiaEventReader(const PythiaEventReader & task);
  PythiaEventReader & operator=(const PythiaEventReader & task);
  virtual ~PythiaEventReader() {}
  virtual void setDefaultConfiguration();
  virtual void initialize();
  virtual void execute();
  
protected:

  bool saveFinalOnly;
  bool savePhotons;
  bool saveNeutrinos;
  bool saveQuarks;
  bool saveGaugeBosons;
  
  static const int nMaxTracks = 5000;

  Int_t           nParticles;
  UInt_t          tracks_fUniqueID[nMaxTracks];   //[tracks_]
  UInt_t          tracks_fBits[nMaxTracks];   //[tracks_]
  Short_t         tracks_fLineColor[nMaxTracks];   //[tracks_]
  Short_t         tracks_fLineStyle[nMaxTracks];   //[tracks_]
  Short_t         tracks_fLineWidth[nMaxTracks];   //[tracks_]
  Int_t           tracks_fPdgCode[nMaxTracks];   //[tracks_]
  Int_t           tracks_fStatusCode[nMaxTracks];   //[tracks_]
  Int_t           tracks_fMother[nMaxTracks][2];   //[tracks_]
  Int_t           tracks_fDaughter[nMaxTracks][2];   //[tracks_]
  Float_t         tracks_fWeight[nMaxTracks];   //[tracks_]
  Double_t        tracks_fCalcMass[nMaxTracks];   //[tracks_]
  Double_t        tracks_fPx[nMaxTracks];   //[tracks_]
  Double_t        tracks_fPy[nMaxTracks];   //[tracks_]
  Double_t        tracks_fPz[nMaxTracks];   //[tracks_]
  Double_t        tracks_fE[nMaxTracks];   //[tracks_]
  Double_t        tracks_fVx[nMaxTracks];   //[tracks_]
  Double_t        tracks_fVy[nMaxTracks];   //[tracks_]
  Double_t        tracks_fVz[nMaxTracks];   //[tracks_]
  Double_t        tracks_fVt[nMaxTracks];   //[tracks_]
  Double_t        tracks_fPolarTheta[nMaxTracks];   //[tracks_]
  Double_t        tracks_fPolarPhi[nMaxTracks];   //[tracks_]
  Int_t           HP_Inc1;
  Int_t           HP_Inc2;
  Int_t           HP_Inc3;
  Int_t           HP_Inc4;
  Int_t           HP_Out1;
  Int_t           HP_Out2;
  Int_t           HP_Out3;
  Int_t           HP_Out4;
  Int_t           HP_Inter1;
  Int_t           HP_Inter2;
  Int_t           HP_Inter3;
  Int_t           HP_Inter4;
  Int_t           HP_Kick1;
  Int_t           HP_Kick2;
  Int_t           HP_Kick3;
  Int_t           HP_Kick4;

  // List of branches
  TBranch        *b_tracks_;   //!
  TBranch        *b_tracks_fUniqueID;   //!
  TBranch        *b_tracks_fBits;   //!
  TBranch        *b_tracks_fLineColor;   //!
  TBranch        *b_tracks_fLineStyle;   //!
  TBranch        *b_tracks_fLineWidth;   //!
  TBranch        *b_tracks_fPdgCode;   //!
  TBranch        *b_tracks_fStatusCode;   //!
  TBranch        *b_tracks_fMother;   //!
  TBranch        *b_tracks_fDaughter;   //!
  TBranch        *b_tracks_fWeight;   //!
  TBranch        *b_tracks_fCalcMass;   //!
  TBranch        *b_tracks_fPx;   //!
  TBranch        *b_tracks_fPy;   //!
  TBranch        *b_tracks_fPz;   //!
  TBranch        *b_tracks_fE;   //!
  TBranch        *b_tracks_fVx;   //!
  TBranch        *b_tracks_fVy;   //!
  TBranch        *b_tracks_fVz;   //!
  TBranch        *b_tracks_fVt;   //!
  TBranch        *b_tracks_fPolarTheta;   //!
  TBranch        *b_tracks_fPolarPhi;   //!
  TBranch        *b_HP_Inc1;   //!
  TBranch        *b_HP_Inc2;   //!
  TBranch        *b_HP_Inc3;   //!
  TBranch        *b_HP_Inc4;   //!
  TBranch        *b_HP_Out1;   //!
  TBranch        *b_HP_Out2;   //!
  TBranch        *b_HP_Out3;   //!
  TBranch        *b_HP_Out4;   //!
  TBranch        *b_HP_Inter1;   //!
  TBranch        *b_HP_Inter2;   //!
  TBranch        *b_HP_Inter3;   //!
  TBranch        *b_HP_Inter4;   //!
  TBranch        *b_HP_Kick1;   //!
  TBranch        *b_HP_Kick2;   //!
  TBranch        *b_HP_Kick3;   //!
  TBranch        *b_HP_Kick4;   //!


  ClassDef(PythiaEventReader,0)
};

}

#endif /* CAP__PythiaEventReader */
