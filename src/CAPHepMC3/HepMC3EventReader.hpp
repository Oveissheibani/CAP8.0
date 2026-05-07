/* **********************************************************************
 * CAP::HepMC3EventReader
 *
 * Reads HepMC3 events from a file (ASCII / .hepmc by default) and copies
 * the final-state particles into the CAP event stream so the existing
 * analyzers (Single, Pair, Pair3D, Global, NuDyn, Spherocity, PtPt) can
 * consume them exactly as they would consume PythiaEventGenerator output.
 *
 * Design follows the PythiaEventGenerator pattern:
 *   - inherits from EventProcessor (so the EventIterator drives it)
 *   - initialize()   opens the input file
 *   - execute()      reads the next event, walks status==1 particles,
 *                    populates CAP::Particle objects, calls
 *                    theEvent.addParticle(...)
 *   - finalize()     closes the file
 *
 * Loose coupling: this class only knows about HepMC3 — it doesn't link
 * against HERWIG or ThePEG.  Any HepMC3 producer (HERWIG, EPOS, Sherpa,
 * MadGraph, even Pythia run with HepMC3 output) can feed it.
 *
 * Configuration keys read from <task>:* :
 *   HepMC3InputFile       : path to .hepmc / .hepmc3 file (required)
 *   SaveFinalOnly         : 1 → only status==1 particles (default 1).
 *                           BACKWARD-COMPAT alias for `KeepStatuses=1`.
 *                           If KeepStatuses is set, SaveFinalOnly is ignored.
 *   KeepStatuses          : comma-separated list of HepMC status codes to
 *                           keep (e.g. "1" or "1,2" or "1,2,11,21,23,51,52"
 *                           or "all").  Default empty == fall back to
 *                           SaveFinalOnly behaviour.  Enables BF-per-stage
 *                           by letting users keep any subset of the
 *                           generator's status code zoo.
 *   SaveQuarks            : 1 → keep quarks/gluons (default 0)
 *   SaveNeutrinos         : 1 → keep neutrinos (default 0)
 *   SavePhotons           : 1 → keep photons (default 0)
 *   SaveGaugeBosons       : 1 → keep W/Z/H/etc (default 0)
 *   RemovePhotons         : convenience alias matching Pythia's
 * ********************************************************************/
#ifndef CAP__HepMC3EventReader
#define CAP__HepMC3EventReader

#include "EventProcessor.hpp"
#include "Event.hpp"
#include "EventFilter.hpp"
#include "Particle.hpp"
#include "ParticleFilter.hpp"
#include "ParticleDb.hpp"

#include <memory>      // std::shared_ptr; HepMC3 hands back shared_ptr.
#include <set>         // std::set<int> — kept HepMC status codes

// HepMC3 forward decls keep this header lightweight; we only need the
// concrete types in the .cpp.  shared_ptr<HepMC3::Reader> works with
// the forward decl as long as the destructor / assignment happens
// where Reader is complete (in the .cpp).
namespace HepMC3 {
  class Reader;
  class GenEvent;
}

namespace CAP
{

class HepMC3EventReader
:
public EventProcessor
{
public:

  HepMC3EventReader();
  HepMC3EventReader(const HepMC3EventReader & task);
  HepMC3EventReader & operator=(const HepMC3EventReader & task);
  virtual ~HepMC3EventReader();

  virtual void setDefaultConfiguration();
  virtual void initialize();
  virtual void execute();
  virtual void finalize();

protected:
  // Resolved at initialize() time from <task>:* keys.
  std::string _inputFileName;
  bool        _saveFinalOnly   = true;
  bool        _saveQuarks      = false;
  bool        _saveNeutrinos   = false;
  bool        _savePhotons     = false;
  bool        _saveGaugeBosons = false;
  // Set of HepMC status codes to keep.  Empty + _saveFinalOnly=true →
  // {1} (final-state only, the default).  Empty + _saveFinalOnly=false
  // → all statuses pass.  Non-empty → exact whitelist.  Populated by
  // initialize() from the KeepStatuses config key.
  std::set<int> _keepStatuses;
  bool        _keepAllStatuses = false;   // true if user said "all"

  // The HepMC3 reader.  HepMC3::deduce_reader() returns shared_ptr,
  // so we hold one too.  Forward decl is OK because shared_ptr's
  // deleter is deduced at the assignment point in the .cpp.
  std::shared_ptr<HepMC3::Reader> _reader;

  // End-of-file flag.  Set when the reader exhausts the input.
  bool _eof = false;

  ClassDef(HepMC3EventReader, 0)
};

} // namespace CAP

#endif // CAP__HepMC3EventReader
