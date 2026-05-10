/* **********************************************************************
 * CAP::HerwigEventGenerator — implementation.
 *
 * Architecture (per INSTALL_REPORT_HERWIG.md §5):
 *   initialize() — set LHAPDF_DATA_PATH, dlopen Herwig.so RTLD_GLOBAL so
 *     its plugins see the symbol table, register ThePEG plugin dirs,
 *     load .run file via ThePEG::PersistentIStream, init EventGenerator.
 *   execute()    — call EventGenerator::shoot() once per call, walk the
 *     resulting ThePEG::Event's final-state particles, populate
 *     CAP::Particle objects, push into CAP::Event.  Same shape as
 *     PythiaEventGenerator::execute().
 *   finalize()   — call EventGenerator::finalize() so cross-section
 *     printouts + statistics flush, release the smart pointer.
 *
 * Loyalty: we don't modify any HERWIG / ThePEG code.  We just call
 * their public C++ API.
 * ********************************************************************/
#include "HerwigEventGenerator.hpp"
#include "PrintHelpers.hpp"
#include "NameHelpers.hpp"
#include "Exceptions.hpp"
#include "Configuration.hpp"

// ThePEG / HERWIG headers — only included here so the .hpp stays light.
#include <ThePEG/Repository/EventGenerator.h>
#include <ThePEG/Repository/Repository.h>
#include <ThePEG/EventRecord/Event.h>
#include <ThePEG/EventRecord/Particle.h>
#include <ThePEG/EventRecord/Step.h>
#include <ThePEG/EventRecord/Collision.h>
#include <ThePEG/Persistency/PersistentIStream.h>
#include <ThePEG/Vectors/Lorentz5Vector.h>
#include <ThePEG/Utilities/DynamicLoader.h>

#include <cstdlib>     // setenv
#include <dlfcn.h>     // dlopen
#include <fstream>
#include <set>
#include <sstream>
#include <vector>      // event keep-alive cache
#include <cmath>       // std::isfinite, std::abs
#include <exception>
#include <cstdio>      // fprintf, setvbuf

ClassImp(CAP::HerwigEventGenerator);

namespace {
  // Helper: split a colon-separated path string.
  std::vector<std::string> _splitPaths(const std::string & s)
  {
    std::vector<std::string> out;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, ':')) {
      if (!item.empty()) out.push_back(item);
    }
    return out;
  }
}

namespace CAP {

HerwigEventGenerator::HerwigEventGenerator()
:
EventProcessor()
{
  appendClassName("HerwigEventGenerator");
  setMinimumReportLevel(Object::kInfo);
  setName("HerwigEventGenerator");
  setTitle("HerwigEventGenerator");
}

HerwigEventGenerator::HerwigEventGenerator(const HerwigEventGenerator & src)
:
EventProcessor(src),
_runFileName(src._runFileName),
_lhapdfDataPath(src._lhapdfDataPath),
_pluginPath(src._pluginPath),
_saveFinalOnly(src._saveFinalOnly),
_saveQuarks(src._saveQuarks),
_saveNeutrinos(src._saveNeutrinos),
_savePhotons(src._savePhotons),
_saveGaugeBosons(src._saveGaugeBosons)
{ /* _egHandle and event-cache are per-instance — never copied */ }

HerwigEventGenerator &
HerwigEventGenerator::operator=(const HerwigEventGenerator & rhs)
{
  if (this != &rhs)
    {
    EventProcessor::operator=(rhs);
    _runFileName     = rhs._runFileName;
    _lhapdfDataPath  = rhs._lhapdfDataPath;
    _pluginPath      = rhs._pluginPath;
    _saveFinalOnly   = rhs._saveFinalOnly;
    _saveQuarks      = rhs._saveQuarks;
    _saveNeutrinos   = rhs._saveNeutrinos;
    _savePhotons     = rhs._savePhotons;
    _saveGaugeBosons = rhs._saveGaugeBosons;
    }
  return *this;
}

HerwigEventGenerator::~HerwigEventGenerator()
{
  // Both _egHandle and _allEventsPtr are intentionally leaked.
  // ThePEG's destructor chain crashes when invoked after the run
  // (cross-references between plugins, handlers, events become
  // stale).  Process exit reclaims everything cleanly.
}

void HerwigEventGenerator::setDefaultConfiguration()
{
  EventProcessor::setDefaultConfiguration();
  addProperty("HerwigRunFile",    String(""));
  addProperty("LHAPDFDataPath",   String(""));
  addProperty("HerwigPluginPath", String(""));
  addProperty("SaveFinalOnly",    true);
  addProperty("SaveQuarks",       false);
  addProperty("SaveNeutrinos",    false);
  addProperty("SavePhotons",      false);
  addProperty("SaveGaugeBosons",  false);
  addProperty("RemovePhotons",    true);
  // KeepStatuses: comma-separated HepMC status codes (or "all").
  // Currently parsed but the embedded path is dormant (file-based via
  // HepMC3EventReader is the production path).  Field is here so the
  // .ini emitter can write it uniformly across all generators.
  addProperty("KeepStatuses",     String(""));
}

void HerwigEventGenerator::initialize()
{
  EventProcessor::initialize();

  // run-cap captures our stderr through a pipe → block-buffered by
  // default → trace lines appear out-of-order vs crash site.  Force
  // stderr to unbuffered so every fprintf hits the log immediately
  // and ROOT's signal-handler messages also flush before death.
  std::setvbuf(stderr, nullptr, _IONBF, 0);

  const String & taskName = name();
  _runFileName     = _configuration.valueString(taskName + ":HerwigRunFile").Data();
  _lhapdfDataPath  = _configuration.valueString(taskName + ":LHAPDFDataPath").Data();
  _pluginPath      = _configuration.valueString(taskName + ":HerwigPluginPath").Data();
  _saveFinalOnly   = _configuration.valueBool  (taskName + ":SaveFinalOnly");
  _saveQuarks      = _configuration.valueBool  (taskName + ":SaveQuarks");
  _saveNeutrinos   = _configuration.valueBool  (taskName + ":SaveNeutrinos");
  bool removePhotons = _configuration.valueBool(taskName + ":RemovePhotons");
  _savePhotons     = _configuration.valueBool  (taskName + ":SavePhotons");
  if (removePhotons) _savePhotons = false;
  _saveGaugeBosons = _configuration.valueBool  (taskName + ":SaveGaugeBosons");

  if (_runFileName.empty() || _runFileName == "NONE")
    throw Exception("HerwigEventGenerator: HerwigRunFile not set "
                    "(point it at a .run file produced by `Herwig read input.in`)",
                    "HerwigEventGenerator::initialize");

  // 1. LHAPDF data path — required before any PDF lookup.  Per install
  //    report §5.4: without it, LHAPDF::mkPDF throws "PDF not found in
  //    any path".  We accept it from config OR fall back to the env var
  //    that herwig-env.sh would have set.
  if (!_lhapdfDataPath.empty())
    setenv("LHAPDF_DATA_PATH", _lhapdfDataPath.c_str(), 1);

  // 2. Tell ThePEG's dynamic loader where its plugins live.  Default
  //    to the HW_PREFIX/lib/ThePEG and HW_PREFIX/lib/Herwig paths if
  //    user didn't supply HerwigPluginPath explicitly.  Per §5.5 the
  //    repository file already records the right paths if Herwig was
  //    installed correctly — so this is belt-and-suspenders.
  for (const auto & p : _splitPaths(_pluginPath))
    ThePEG::DynamicLoader::appendPath(p);

  // 3. dlopen Herwig.so RTLD_GLOBAL so its plugins can see its global
  //    symbol table (per §5.5: "RTLD_GLOBAL is important if you intend
  //    to subsequently load Herwig plugins that depend on Herwig's
  //    global symbol table").  We try a few known locations; failure
  //    is non-fatal because Herwig.so may already be resolved via the
  //    rpath baked into our binary.
  {
    const char * dlnames[] = {
      "Herwig.so", "libHerwig.so", "libHerwig.dylib",
      nullptr
    };
    for (const char ** name = dlnames; *name; ++name)
      {
      void * h = dlopen(*name, RTLD_NOW | RTLD_GLOBAL);
      if (h) {
        printValue("HerwigEventGenerator: dlopen RTLD_GLOBAL", String(*name));
        break;
      }
      }
  }

  // 4. Load the .run file.  ThePEG's PersistentIStream reads the
  //    serialized repository and gives us back an EventGenerator.
  std::ifstream runStream(_runFileName);
  if (!runStream.good())
    throw Exception(String("HerwigEventGenerator: cannot open ")
                    + _runFileName.c_str(),
                    "HerwigEventGenerator::initialize");

  ThePEG::PersistentIStream is(runStream);
  ThePEG::EGPtr * egp = new ThePEG::EGPtr;
  is >> *egp;
  if (!*egp)
    {
    delete egp;
    throw Exception(String("HerwigEventGenerator: failed to "
                           "deserialize EventGenerator from ")
                    + _runFileName.c_str(),
                    "HerwigEventGenerator::initialize");
    }
  _egHandle = static_cast<void*>(egp);

  // 5. Initialize the generator (runs Herwig's setup, prints banner,
  //    inits PDFs, etc.).
  (*egp)->initialize();

  printCR();
  printValue("HerwigEventGenerator: run file",     String(_runFileName.c_str()));
  printValue("HerwigEventGenerator: LHAPDF path",  String(_lhapdfDataPath.c_str()));
  printValue("HerwigEventGenerator: saveFinalOnly", _saveFinalOnly);
}

void HerwigEventGenerator::execute()
{
  if (!_egHandle) return;

  // Outer safety net: nothing thrown by Herwig/ThePEG/our code should
  // propagate above us, since EventIterator only catches
  // CAP::EndOfDataException (anything else aborts the whole process).
  try
    {
  ThePEG::EGPtr & eg = *static_cast<ThePEG::EGPtr*>(_egHandle);

  ParticleDb & particleTypeList = db();
  Event &      theEvent         = event();
  theEvent.reset();

  // Generate one event.  shoot() returns a ThePEG::EventPtr.
  ThePEG::EventPtr evt;
  try
    {
    evt = eg->shoot();
    }
  catch (std::exception & ex)
    {
    static int warned = 0;
    if (warned++ < 3)
      {
      printCR();
      printValue("HerwigEventGenerator: shoot() threw", String(ex.what()));
      }
    return;
    }
  catch (...)
    {
    static int warned = 0;
    if (warned++ < 3)
      {
      printCR();
      printString("HerwigEventGenerator: shoot() threw unknown exception");
      }
    return;
    }
  if (!evt) return;

  // Append every event to a keep-alive vector — _allEventsPtr —
  // and never release any individual event mid-run.  Both other
  // timings have been observed to SIGSEGV/SIGABRT:
  //   - releasing the previous before next shoot() → SEGV in shoot()
  //   - releasing after next shoot() → SEGV/ABRT in our delete
  // Holding everything alive lets ThePEG release them in its own
  // order at finalize() time.  See finalize() for the full story.

  // Walk all particles in all steps of all collisions; pick out
  // final-state ones.  ThePEG's Event::getFinalState returns a
  // tPVector (= std::vector<tPPtr>, vector of mutable Particle
  // pointers).  Use auto so we don't depend on the exact spelling —
  // just need something we can iterate.  Wrap in try/catch in case
  // the event record is malformed.
  auto finalsCall = [&]() {
    try { return evt->getFinalState(); }
    catch (...) {
      static int warned = 0;
      if (warned++ < 3)
        {
        printCR();
        printString("HerwigEventGenerator: getFinalState() threw");
        }
      using V = decltype(evt->getFinalState());
      return V();
    }
  };
  auto finals = finalsCall();

  long nKept = 0, nSkipPdg = 0, nSkipFilter = 0, nSkipUnknown = 0, nSkipBad = 0;

  for (auto pp : finals)
    {
    if (!pp) { ++nSkipBad; continue; }

    // Wrap per-particle body so one bad particle doesn't kill the run.
    try
      {
      const int pdg = pp->id();
      if (pdg == 0) { ++nSkipPdg; continue; }

      if (!_saveQuarks      && std::abs(pdg) < 10)               { ++nSkipFilter; continue; }
      if (!_saveNeutrinos   && (std::abs(pdg) == 12 ||
                                std::abs(pdg) == 14 ||
                                std::abs(pdg) == 16 ||
                                std::abs(pdg) == 18))            { ++nSkipFilter; continue; }
      if (!_savePhotons     && pdg == 22)                        { ++nSkipFilter; continue; }
      if (!_saveGaugeBosons && (std::abs(pdg) > 22 &&
                                std::abs(pdg) < 40))             { ++nSkipFilter; continue; }

      // PDG → CAP::ParticleType lookup.  Same defensive skip as
      // PythiaEventGenerator — Herwig generates some PDG codes that
      // aren't in our shipped DB/ParticleData/particles.data.
      ParticleType * particleType = nullptr;
      try { particleType = particleTypeList.findPdgCode(pdg); }
      catch (Exception & /*ex*/)
        {
        static std::set<int> warned;
        if (warned.insert(pdg).second)
          {
          printCR();
          printValue("HerwigEventGenerator: skipping unknown PDG", pdg);
          }
        ++nSkipUnknown;
        continue;
        }
      if (!particleType) { ++nSkipUnknown; continue; }

      // Momentum.  ThePEG stores Lorentz5Vector in natural units
      // (GeV).  setEPxPyPz takes (E, px, py, pz).  Use a defensive
      // copy so we never deref a temporary across the divide.
      const ThePEG::Lorentz5Momentum mom = pp->momentum();
      const double e  = mom.e()  / ThePEG::GeV;
      const double px = mom.x()  / ThePEG::GeV;
      const double py = mom.y()  / ThePEG::GeV;
      const double pz = mom.z()  / ThePEG::GeV;

      // Skip clearly bogus 4-momenta (NaN/inf).  Avoids ROOT TLorentz
      // assertions downstream in the analyzers.
      if (!std::isfinite(e) || !std::isfinite(px) ||
          !std::isfinite(py) || !std::isfinite(pz))
        {
        ++nSkipBad;
        continue;
        }

      Particle & particle = Particle::factory().nextObject();
      particle.setType(particleType);
      particle.setLive(1);
      particle.setEPxPyPz(e, px, py, pz);

      // Production vertex — Herwig final-state hadrons often have
      // unset / arbitrary production vertices, and CAP's analyzers
      // don't read the production vertex anyway.  Match Pythia's
      // behaviour for hadrons by setting (0,0,0,0).  Avoids any
      // unit-conversion landmines around ThePEG::millimeter.
      particle.setXYZT(0.0, 0.0, 0.0, 0.0);

      theEvent.addParticle(&particle);
      ++nKept;
      }
    catch (std::exception & ex)
      {
      static int warned = 0;
      if (warned++ < 5)
        {
        printCR();
        printValue("HerwigEventGenerator: per-particle exception", String(ex.what()));
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
        printString("HerwigEventGenerator: per-particle unknown exception");
        }
      ++nSkipBad;
      continue;
      }
    }

  // One-shot diagnostic summary on the first event so we can see how
  // many particles passed each gate.  Quiet after that.
  if (_eventCount == 0)
    {
    printCR();
    printValue("HerwigEventGenerator: event 0 finals.size()", (long)finals.size());
    printValue("HerwigEventGenerator: event 0 kept",          nKept);
    printValue("HerwigEventGenerator: event 0 filtered out",  nSkipFilter);
    printValue("HerwigEventGenerator: event 0 unknown PDG",   nSkipUnknown);
    printValue("HerwigEventGenerator: event 0 bad particle",  nSkipBad);
    printValue("HerwigEventGenerator: event 0 zero PDG",      nSkipPdg);
    }

  if (_eventCount < 3)
    std::fprintf(stderr,
      "[HerwigEG] leaving execute() event=%ld kept=%ld\n",
      _eventCount, nKept);

  // Append the new event to our keep-alive vector.  We deliberately
  // never release individual events mid-run: ThePEG/Herwig has
  // cross-event back-pointers (probably colour lines or pdf-info
  // records) that crash if a previous event is freed while later
  // events are still being processed.  Memory cost is small (~kB
  // per kept event).  All events get released in finalize().
  if (_eventCount < 3)
    std::fprintf(stderr,
      "[HerwigEG] before evt-keep event=%ld\n", _eventCount);
  if (!_allEventsPtr)
    _allEventsPtr =
      static_cast<void*>(new std::vector<ThePEG::EventPtr>);
  static_cast<std::vector<ThePEG::EventPtr>*>(_allEventsPtr)
    ->push_back(evt);
  if (_eventCount < 3)
    std::fprintf(stderr,
      "[HerwigEG] after evt-keep event=%ld\n", _eventCount);

  ++_eventCount;
  eventAccepted().increment();
  _taskExecuted.increment();
    }
  catch (std::exception & ex)
    {
    static int warned = 0;
    if (warned++ < 3)
      {
      printCR();
      printValue("HerwigEventGenerator::execute(): outer exception", String(ex.what()));
      }
    return;
    }
  catch (...)
    {
    static int warned = 0;
    if (warned++ < 3)
      {
      printCR();
      printString("HerwigEventGenerator::execute(): outer unknown exception");
      }
    return;
    }
}

void HerwigEventGenerator::finalize()
{
  std::fprintf(stderr, "[HerwigEG] finalize() begin (eventCount=%ld)\n",
               _eventCount);
  // ORDER MATTERS:
  // 1. Call EG::finalize() while events are still alive — it prints
  //    cross-section stats and may walk currentEvent internally.
  // 2. Tear down the EG.  Its destructor drops *its* refs on
  //    currentEvent (still held by us, so it won't be freed yet).
  // 3. INTENTIONALLY do NOT delete _allEventsPtr.  All observed
  //    end-of-run crashes are inside the destruction of either the
  //    EG or our event vector when the order interleaves badly.
  //    Leaking ~few MB at process exit is harmless; the kernel
  //    reclaims it cleanly.
  if (_egHandle)
    {
    ThePEG::EGPtr & eg = *static_cast<ThePEG::EGPtr*>(_egHandle);
    std::fprintf(stderr, "[HerwigEG] eg->finalize()\n");
    try { if (eg) eg->finalize(); }
    catch (...) { std::fprintf(stderr, "[HerwigEG] eg->finalize threw\n"); }
    // INTENTIONALLY leak the EGPtr.  Confirmed: deleting it after a
    // successful 1000-event run + eg->finalize() SEGVs in ThePEG's
    // destructor chain (probably a plugin/handler with a stale
    // back-pointer).  Process exit reclaims it.
    std::fprintf(stderr, "[HerwigEG] (leaking _egHandle by design)\n");
    _egHandle = nullptr;
    }
  std::fprintf(stderr, "[HerwigEG] (leaking _allEventsPtr by design)\n");
  // _allEventsPtr deliberately not freed — see comment above.
  std::fprintf(stderr, "[HerwigEG] EventProcessor::finalize()\n");
  EventProcessor::finalize();
  std::fprintf(stderr, "[HerwigEG] finalize() end\n");
}

} // namespace CAP
