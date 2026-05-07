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
#ifndef CAP__EposEventReader
#define CAP__EposEventReader
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

class EposEventReader
:
public EventProcessor,
public RootChainManager
{
public:

  EposEventReader();
  virtual ~EposEventReader() {}
  void setDefaultConfiguration();
  void configure(const Configuration & configuration);
  void initialize();
  void execute();

protected:

  static const int arraySize = 40000;

   // Declaration of leaf types
   Int_t           events;
   Int_t           nParticles;
   Float_t         impact;
   Int_t           pid[arraySize];   //[Mult]
   Float_t         px[arraySize];   //[Mult]
   Float_t         py[arraySize];   //[Mult]
   Float_t         pz[arraySize];   //[Mult]
   Float_t         e[arraySize];   //[Mult]

   // List of branches
   TBranch        *b_Events;   //!
   TBranch        *b_Mult;   //!
   TBranch        *b_Impact;   //!
   TBranch        *b_PID;   //!
   TBranch        *b_Px;   //!
   TBranch        *b_Py;   //!
   TBranch        *b_Pz;   //!
   TBranch        *b_E;   //!
 
  ClassDef(EposEventReader,0)
};

}


#endif /* CAP__EposEventReader */
