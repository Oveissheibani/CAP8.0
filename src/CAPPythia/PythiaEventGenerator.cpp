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
#include "PythiaEventGenerator.hpp"
#include "ParticleDb.hpp"
#include <set>
#include <cmath>     // std::isfinite, std::abs
#include <exception>
#include <sstream>   // KeepStatuses parsing
#include <cctype>    // std::tolower
#include <stdexcept> // std::stoi
ClassImp(CAP::PythiaEventGenerator);

namespace CAP
{

PythiaEventGenerator::PythiaEventGenerator()
:
EventProcessor(),
pythia(nullptr),
saveFinalOnly(true),
savePhotons(false),
saveNeutrinos(false),
saveQuarks(false),
saveGaugeBosons(false)
{
  appendClassName("PythiaEventGenerator");
  setMinimumReportLevel(Object::kInfo);
  setName("PythiaEventGenerator");
  setTitle("PythiaEventGenerator");
}

PythiaEventGenerator::PythiaEventGenerator(const PythiaEventGenerator & source)
:
EventProcessor(source),
pythia(source.pythia),
saveFinalOnly(source.saveFinalOnly),
savePhotons(source.savePhotons),
saveNeutrinos(source.saveNeutrinos),
saveQuarks(source.saveQuarks),
saveGaugeBosons(source.saveGaugeBosons)
{ }

PythiaEventGenerator & PythiaEventGenerator::operator=(const PythiaEventGenerator & rhs)
{
  if (this!=&rhs)
    {
    EventProcessor::operator=(rhs);
    pythia = rhs.pythia;
    saveFinalOnly    = rhs.saveFinalOnly;
    savePhotons      = rhs.savePhotons;
    saveNeutrinos    = rhs.saveNeutrinos;
    saveQuarks       = rhs.saveQuarks;
    saveGaugeBosons  = rhs.saveGaugeBosons;
    }
  return *this;
}

void PythiaEventGenerator::setDefaultConfiguration()
{
  if (reportDebug(__FUNCTION__)) { printCR(); }
  EventProcessor::setDefaultConfiguration();
  const String & taskName = name();
  _configuration.addProperty(createKey(taskName,"Print:Banner"),     false);
  _configuration.addProperty(createKey(taskName,"Print:Statistics"), false);
  _configuration.addProperty(createKey(taskName,"Print:NEvents"),    0);
  _configuration.addProperty(createKey(taskName,"Beams:idA"),        2212);
  _configuration.addProperty(createKey(taskName,"Beams:idB"),        2212);
  _configuration.addProperty(createKey(taskName,"Beams:frameType"),  2);
  _configuration.addProperty(createKey(taskName,"Beams:eCM"),        2700.0);
  _configuration.addProperty(createKey(taskName,"Beams:eA"),         1350.0);
  _configuration.addProperty(createKey(taskName,"Beams:eB"),         1350.0);
  _configuration.addProperty(createKey(taskName,"SetSeed"),          true);
  _configuration.addProperty(createKey(taskName,"SeedValue"),        121211);
  _configuration.addProperty(createKey(taskName,"UseQCDCR"),         true);
  _configuration.addProperty(createKey(taskName,"UseRopes"),         false);
  _configuration.addProperty(createKey(taskName,"UseShoving"),       false);
  _configuration.addProperty(createKey(taskName,"xmlInputPath"),     String(""));
  _configuration.addProperty(createKey(taskName,"SaveFinalOnly"),    true);
  _configuration.addProperty(createKey(taskName,"SavePhotons"),      false);
  _configuration.addProperty(createKey(taskName,"SaveNeutrinos"),    false);
  _configuration.addProperty(createKey(taskName,"SaveQuarks"),       false);
  _configuration.addProperty(createKey(taskName,"SaveGaugeBosons"),  false);
  // KeepStatuses: comma-separated HepMC status codes (e.g. "1", "1,2",
  // "1,2,11,21,23,51,52", "all").  Empty: fall back to SaveFinalOnly.
  // Enables BF-per-stage analyses on Pythia by letting users pick any
  // subset of the parton/shower/decay chain.
  _configuration.addProperty(createKey(taskName,"KeepStatuses"),     String(""));

  for (int k=0; k<30; k++)
    {
    String key = taskName; key += ":Option"; key += k;
    _configuration.addProperty(key, String("none"));
    }
}

//!
//! Initialize generator
//! pythia->Initialize(2212 /* p */, 2212 /* p */, 14000. /* GeV */);
//!
void PythiaEventGenerator::initialize()
{
  if (reportStart(__FUNCTION__)) { printCR(); }
  EventProcessor::initialize();
  const String & taskName = name();
  pythia = new Pythia8::Pythia(valueString("xmlInputPath").Data(), valueBool("Print:Banner"));
  pythia->settings.mode("Beams:idA",       _configuration.valueInt(createKey(taskName,"Beams:idA")));
  pythia->settings.mode("Beams:idB",       _configuration.valueInt(createKey(taskName,"Beams:idB")));
  pythia->settings.mode("Beams:frameType", _configuration.valueInt(createKey(taskName,"Beams:frameType")));
  switch (_configuration.valueInt(createKey(taskName,"Beams:frameType")))
    {
      default:
      case 1:
      pythia->settings.parm("Beams:eCM",_configuration.valueDouble(createKey(taskName,"Beams:eCM")));
      break;
      case 2:
      pythia->settings.parm("Beams:eA",_configuration.valueDouble(createKey(taskName,"Beams:eA")));
      pythia->settings.parm("Beams:eB",_configuration.valueDouble(createKey(taskName,"Beams:eB")));
      break;
    }
  saveFinalOnly   = _configuration.valueBool(createKey(taskName,"SaveFinalOnly"));
  savePhotons     = _configuration.valueBool(createKey(taskName,"SavePhotons"));
  saveNeutrinos   = _configuration.valueBool(createKey(taskName,"SaveNeutrinos"));
  saveQuarks      = _configuration.valueBool(createKey(taskName,"SaveQuarks"));
  saveGaugeBosons = _configuration.valueBool(createKey(taskName,"SaveGaugeBosons"));

  // Parse KeepStatuses into _keepStatuses set.  Empty: back-compat with
  // SaveFinalOnly behaviour.  "all"/"any"/"*": _keepAllStatuses=true.
  // Otherwise: comma-separated integer list.  Used in execute() instead
  // of the boolean isFinal() check, so users can target partons,
  // intermediate hadrons, or any other Pythia status zoo subset.
  {
    const std::string spec =
      _configuration.valueString(createKey(taskName,"KeepStatuses")).Data();
    _keepStatuses.clear();
    _keepAllStatuses = false;
    std::string s; s.reserve(spec.size());
    for (char c : spec) if (c != ' ' && c != '\t') s.push_back(c);
    if (!s.empty())
      {
      std::string lower = s;
      for (auto & c : lower) c = std::tolower(c);
      if (lower == "all" || lower == "any" || lower == "*")
        { _keepAllStatuses = true; }
      else
        {
        std::string token; std::stringstream ss(s);
        while (std::getline(ss, token, ','))
          {
          if (token.empty()) continue;
          try { _keepStatuses.insert(std::stoi(token)); }
          catch (...) { /* ignore garbage */ }
          }
        }
      }
    else
      {
      // Back-compat: SaveFinalOnly=1 → {1}; =0 → all statuses.
      if (saveFinalOnly) _keepStatuses.insert(1);
      else               _keepAllStatuses = true;
      }
    printCR();
    printValue("Pythia: KeepStatuses spec",      String(spec.c_str()));
    printValue("Pythia: keepAllStatuses",        _keepAllStatuses);
    {
      std::stringstream ss2; bool first = true;
      for (int v : _keepStatuses)
        { if (!first) ss2 << ","; ss2 << v; first = false; }
      printValue("Pythia: parsed status set",    String(ss2.str().c_str()));
    }
  }

  if (valueDouble("SetSeed"))
    {
    String  seedValueString = "Random:seed = ";
    seedValueString += _configuration.valueLong(createKey(taskName,"SeedValue"));
    pythia->readString("Random:setSeed = on");
    pythia->readString(seedValueString.Data());
    printValue("Pythia:Random:setSeed","ON");
    printValue("Pythia:Random:SeedValue",seedValueString);
    }
  for (int k=0; k<30; k++)
    {
    String key = taskName; key += ":Option"; key += k;
    String value = _configuration.valueString(key);
    if (!value.Contains("none") )
      {
      String s = "Pythia:"; s+=key;
      printValue(s,value);
      pythia->readString(value.Data());
      }
    }
  if(valueBool(  "UseQCDCR"))
    {
    printValue("Pythia:UseQCDCR","ON");
    pythia->readString("MultiPartonInteractions:pT0Ref = 2.15");
    pythia->readString("BeamRemnants:remnantMode = 1");
    pythia->readString("BeamRemnants:saturation = 5");
    pythia->readString("ColourReconnection:mode = 1");
    pythia->readString("ColourReconnection:allowDoubleJunRem = off");
    pythia->readString("ColourReconnection:m0 = 0.3");
    pythia->readString("ColourReconnection:allowJunctions = on");
    pythia->readString("ColourReconnection:junctionCorrection = 1.2");
    pythia->readString("ColourReconnection:timeDilationMode = 2");
    pythia->readString("ColourReconnection:timeDilationPar = 0.18");
    if(!valueBool("UseRopes")) pythia->readString("Ropewalk:RopeHadronization = off");
    }
  if(valueBool("UseQCDCR")  &&  valueBool(  "UseRopes"))
    {
    printValue("Pythia:UseQCDCR","ON");
    printValue("Pythia:UseRopes","ON");
    pythia->readString("Ropewalk:RopeHadronization = on");
    pythia->readString("Ropewalk:doShoving = on");
    pythia->readString("Ropewalk:doFlavour = on");
    pythia->readString("Ropewalk:tInit = 1.5");
    pythia->readString("Ropewalk:deltat = 0.05");
    pythia->readString("Ropewalk:tShove = 0.1");
    pythia->readString("Ropewalk:gAmplitude = 0.");// # Set shoving strength to 0 explicitly
    pythia->readString("Ropewalk:r0 = 0.5");
    pythia->readString("Ropewalk:m0 = 0.2");
    pythia->readString("Ropewalk:beta = 0.1");
    pythia->readString("PartonVertex:setVertex = on");
    pythia->readString("PartonVertex:protonRadius = 0.7");
    pythia->readString("PartonVertex:emissionWidth = 0.1");
  }
  if(!valueBool("UseQCDCR")  &&  valueBool(  "UseRopes"))
    throw Exception("ropes w/o the necessary junctions! Flip kQCDCR=kTRUE",__FUNCTION__);
  if(valueBool("UseShoving"))
    {
    printValue("Pythia:UseShoving","ON");
    pythia->readString("Ropewalk:RopeHadronization = on");
    pythia->readString("Ropewalk:doShoving = on");
    pythia->readString("Ropewalk:doFlavour = off");
    pythia->readString("Ropewalk:tInit = 1.5");
    pythia->readString("Ropewalk:rCutOff = 10.0");
    pythia->readString("Ropewalk:limitMom =  on");
    pythia->readString("Ropewalk:pTcut = 2.0");
    pythia->readString("Ropewalk:deltat = 0.1");
    pythia->readString("Ropewalk:deltay = 0.1");
    pythia->readString("Ropewalk:tShove = 1.");
    pythia->readString("Ropewalk:deltat = 0.1");
    pythia->readString("Ropewalk:gAmplitude = 10.0");
    pythia->readString("Ropewalk:gExponent = 1.0");
    pythia->readString("Ropewalk:r0 = 0.41");
    pythia->readString("Ropewalk:m0 = 0.2");
    pythia->readString("PartonVertex:setVertex = on");
    pythia->readString("PartonVertex:protonRadius = 0.7");
    pythia->readString("PartonVertex:emissionWidth = 0.1");
    }
  pythia->init();
//  if (reportDebug(__FUNCTION__))
//    {
//    pythia->settings.listAll();
//    pythia->settings.listChanged();
//    }
}

void PythiaEventGenerator::execute()
{
  // Outer safety net — EventIterator only catches CAP::EndOfDataException;
  // anything else propagates up and aborts the whole process.  We keep
  // every per-particle hazard local so the run survives oddities.
  try
    {
  ParticleDb & particleTypeList = db();   // (renamed: ParticleTypeList -> ParticleDb)
  Event & theEvent = event();
  theEvent.reset();
  if (!pythia->next()) return;
  int nParticleToCopy   = pythia->event.size();
  if (pythia->event[0].id() == 90)
    {
    nParticleToCopy--;
    }

  long nKept = 0, nSkipPdg = 0, nSkipFilter = 0, nSkipUnknown = 0, nSkipBad = 0;

  for (int i = 1; i <= nParticleToCopy; i++)
    {
    // Per-particle try/catch: never let one bad particle take down
    // the run.  Keeps the BF-per-stage workflow robust no matter
    // how exotic Pythia gets on rare events.
    try
      {
      int pdg = pythia->event[i].id();
      if (pdg == 0) { ++nSkipPdg; continue; }
      // Status-code filter — uses Pythia's HepMC-equivalent status code
      // so "1" means final-state in both the Pythia and HepMC3 paths.
      // Back-compat: when KeepStatuses is empty + saveFinalOnly=true we
      // populated _keepStatuses={1} in initialize().
      if (!_keepAllStatuses)
        {
        const int s = pythia->event[i].statusHepMC();
        if (_keepStatuses.count(s) == 0) { ++nSkipFilter; continue; }
        }
      if (!saveQuarks      &&  abs(pdg)<10)                 { ++nSkipFilter; continue; } // skip quarks, gluons, etc
      if (!saveNeutrinos   &&  (abs(pdg)==12 || abs(pdg)==14  || abs(pdg)==16 || abs(pdg)==18))
                                                            { ++nSkipFilter; continue; }
      if (!savePhotons     &&  pdg==22)                     { ++nSkipFilter; continue; }
      if (!saveGaugeBosons  &&  (abs(pdg)>22  &&  abs(pdg)<40))
                                                            { ++nSkipFilter; continue; }
      // The shipped DB/ParticleData/particles.data is incomplete — it lacks
      // mass-eigenstate kaons (K_L=130, K_S=310) and a number of excited /
      // exotic species that Pythia happily generates. Treat any PDG that
      // isn't in the DB as "skip this particle" rather than aborting the
      // run. The first time we see each unknown code we log it once.
      ParticleType * particleType = nullptr;
      try
        {
        particleType = particleTypeList.findPdgCode(pdg);
        }
      catch (Exception & /*ex*/)
        {
        static std::set<int> warned;
        if (warned.insert(pdg).second)
          {
          printCR();
          printValue("PythiaEventGenerator: skipping unknown PDG", pdg);
          }
        ++nSkipUnknown;
        continue;
        }
      catch (...)
        {
        ++nSkipUnknown;
        continue;
        }
      if (!particleType) { ++nSkipUnknown; continue; }

      // Defensive copy + NaN/inf guard on the 4-momentum.  Avoids
      // ROOT TLorentzVector assertions when Pythia hands back
      // pathological kinematics (rare, but happens on grid jobs).
      const double e  = pythia->event[i].e();
      const double px = pythia->event[i].px();
      const double py = pythia->event[i].py();
      const double pz = pythia->event[i].pz();
      if (!std::isfinite(e) || !std::isfinite(px) ||
          !std::isfinite(py) || !std::isfinite(pz))
        {
        ++nSkipBad;
        continue;
        }

      Particle & particle = Particle::factory().nextObject();
      particle.setType(particleType);
      particle.setLive(1);
      // Note: setEPxPyPz takes (E, px, py, pz) — different from the old setPxPyPzE.
      particle.setEPxPyPz(e, px, py, pz);
      particle.setXYZT(pythia->event[i].xProd(),
                       pythia->event[i].yProd(),
                       pythia->event[i].zProd(),
                       pythia->event[i].tProd());
      theEvent.addParticle(&particle);
      ++nKept;
      }
    catch (std::exception & ex)
      {
      static int warned = 0;
      if (warned++ < 5)
        {
        printCR();
        printValue("PythiaEventGenerator: per-particle exception", String(ex.what()));
        }
      ++nSkipBad;
      continue;
      }
    catch (...)
      {
      static int warned = 0;
      if (warned++ < 5)
        {
        printCR();
        printString("PythiaEventGenerator: per-particle unknown exception");
        }
      ++nSkipBad;
      continue;
      }
    }

  // One-shot diagnostic on event 0 so the user can see how many
  // particles passed each gate.  Helps debug filter selections.
  static long _eventCount = 0;
  if (_eventCount == 0)
    {
    printCR();
    printValue("PythiaEventGenerator: event 0 nParticleToCopy", (long)nParticleToCopy);
    printValue("PythiaEventGenerator: event 0 kept",            nKept);
    printValue("PythiaEventGenerator: event 0 filtered out",    nSkipFilter);
    printValue("PythiaEventGenerator: event 0 unknown PDG",     nSkipUnknown);
    printValue("PythiaEventGenerator: event 0 bad particle",    nSkipBad);
    printValue("PythiaEventGenerator: event 0 zero PDG",        nSkipPdg);
    }
  ++_eventCount;

  eventAccepted().increment();    // accessor returns Accountant&
  _taskExecuted.increment();
    }
  catch (std::exception & ex)
    {
    static int warned = 0;
    if (warned++ < 3)
      {
      printCR();
      printValue("PythiaEventGenerator::execute(): outer exception", String(ex.what()));
      }
    return;
    }
  catch (...)
    {
    static int warned = 0;
    if (warned++ < 3)
      {
      printCR();
      printString("PythiaEventGenerator::execute(): outer unknown exception");
      }
    return;
    }
}

void PythiaEventGenerator::finalize()
{
  if (reportDebug(__FUNCTION__)) { printCR(); }
  // The CAP task tree calls finalize() twice on each subtask in some
  // configurations (RunAnalysis::finalize -> finalizeSubTasks ->
  // EventIterator::finalize -> finalizeSubTasks again). Make this
  // method idempotent so the second visit doesn't deref a freed
  // Pythia instance and segfault.
  if (pythia == nullptr) return;
  printCR();
  printLine();
  pythia->stat();
  printLine();
  printCR();
  delete pythia;
  pythia = nullptr;
}

} // namespace CAP
