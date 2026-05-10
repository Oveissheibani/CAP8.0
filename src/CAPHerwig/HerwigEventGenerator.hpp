/* **********************************************************************
 * CAP::HerwigEventGenerator — embedded HERWIG 7 driver.
 *
 * In-process event generator that mirrors PythiaEventGenerator.  Loads a
 * pre-prepared HERWIG .run file (output of "Herwig read input.in") via
 * ThePEG's PersistentIStream, instantiates the EventGenerator, and on
 * every execute() call generates one event and copies its final-state
 * particles into the CAP event stream.
 *
 * Required PRE-step (run once before CAP):
 *   source /Users/.../LocalHerwig/.../opt/herwig-env.sh
 *   Herwig read your-input.in     # writes your-input.run
 *
 * Required at run-time (handled in initialize()):
 *   - LHAPDF_DATA_PATH set so PDFs load
 *   - ThePEG plugin dirs (lib/ThePEG, lib/Herwig) registered with the
 *     dynamic loader (RTLD_GLOBAL on Herwig.so so plugins see symbols)
 *
 * Configuration keys (read from <task>:* via Configuration):
 *   HerwigRunFile         — path to .run file (REQUIRED)
 *   LHAPDFDataPath        — LHAPDF data dir; falls back to env var
 *   HerwigPluginPath      — colon-separated plugin search paths
 *   SaveFinalOnly         — only status==1 particles (default 1)
 *   SaveQuarks            — keep partons (default 0)
 *   SaveNeutrinos         — keep ν (default 0)
 *   SavePhotons           — keep γ (default 0)
 *   SaveGaugeBosons       — keep W/Z/H/etc (default 0)
 *   RemovePhotons         — legacy alias matching Pythia (default 1)
 * ********************************************************************/
#ifndef CAP__HerwigEventGenerator
#define CAP__HerwigEventGenerator

#include "EventProcessor.hpp"
#include "Event.hpp"
#include "EventFilter.hpp"
#include "Particle.hpp"
#include "ParticleFilter.hpp"
#include "ParticleDb.hpp"

#include <set>     // status-code keep set

// We deliberately do NOT forward-declare anything from the ThePEG
// namespace here.  ThePEG::Ptr is a template defined via macros in
// <ThePEG/Config/Pointers.h> with a non-trivial trait specialisation
// system; any forward decl conflicts with the real definition the
// .cpp later includes.  The class instead holds the EventGenerator
// pointer as a void*, with the cast to ThePEG::EGPtr* localised to
// HerwigEventGenerator.cpp.

namespace CAP {

class HerwigEventGenerator
:
public EventProcessor
{
public:

  HerwigEventGenerator();
  HerwigEventGenerator(const HerwigEventGenerator & task);
  HerwigEventGenerator & operator=(const HerwigEventGenerator & task);
  virtual ~HerwigEventGenerator();

  virtual void setDefaultConfiguration();
  virtual void initialize();
  virtual void execute();
  virtual void finalize();

protected:
  std::string _runFileName;
  std::string _lhapdfDataPath;
  std::string _pluginPath;     // colon-separated dirs
  bool        _saveFinalOnly   = true;
  bool        _saveQuarks      = false;
  bool        _saveNeutrinos   = false;
  bool        _savePhotons     = false;
  bool        _saveGaugeBosons = false;
  // Status-code whitelist (empty + _saveFinalOnly=true → final only,
  // empty + _saveFinalOnly=false → all, otherwise explicit list).
  std::set<int> _keepStatuses;
  bool        _keepAllStatuses = false;

  // ThePEG owns the EventGenerator via a smart pointer.  We hold it in
  // a void* and cast in the .cpp to avoid leaking ThePEG headers.
  void *      _egHandle    = nullptr;
  // Vector of all retained EventPtrs (held as void* so the .hpp doesn't
  // need ThePEG headers).  We never release individual elements —
  // ThePEG/Herwig has cross-event back-pointers that crash if we
  // free even one early.  All events get released together at
  // finalize() / destructor time.  Memory cost: ~kB per event, OK
  // for typical 10⁶-event runs.
  void *      _allEventsPtr = nullptr;   // std::vector<ThePEG::EventPtr> *
  long        _eventCount   = 0;

  ClassDef(HerwigEventGenerator, 0)
};

} // namespace CAP

#endif // CAP__HerwigEventGenerator
