#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

// --- Orchestrator container ------------------------------------------------
#pragma link C++ class CAP::RunAnalysis+;

// --- DB loaders (two names exist in shipped .ini files) -------------------
#pragma link C++ class CAP::ParticleTypeTask+;
#pragma link C++ class CAP::ParticleDbTask+;

// --- Filter creators ------------------------------------------------------
#pragma link C++ class CAP::EventFilterCreator+;
#pragma link C++ class CAP::ParticleFilterCreator+;
#pragma link C++ class CAP::FilterCreator+;

// --- Loop drivers ---------------------------------------------------------
// CAP::FileIterator is provided by libParticles — DO NOT add a stub here.

// --- Calculators (post-processing tasks for RunDerived) ------------------
#pragma link C++ class CAP::ParticleSingleCalculator+;
#pragma link C++ class CAP::ParticlePairCalculator+;
#pragma link C++ class CAP::ParticlePair3DCalculator+;
#pragma link C++ class CAP::GlobalCalculator+;

// --- Balance Function calculators (post-processing tasks for RunBf) -------
// Both 1D and 3D variants.  The 1D stub lives in StubParticlePairBfCalculator.hpp
// (renamed file, same class name) so rootcling doesn't trip on the broken
// shipped header at src/ParticlePair/ParticlePairBfCalculator.hpp.
#pragma link C++ class CAP::ParticlePair3DBfCalculator+;
#pragma link C++ class CAP::ParticlePairBfCalculator+;

#endif
