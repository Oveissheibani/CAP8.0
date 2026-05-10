/* **********************************************************************
 * CAP::HepMC3EventReader — implementation.  See header for design.
 * ********************************************************************/
#include "HepMC3EventReader.hpp"
#include "PrintHelpers.hpp"
#include "NameHelpers.hpp"
#include "Exceptions.hpp"
#include "Configuration.hpp"

// HepMC3 — we use the auto-detect ReaderFactory so the same code reads
// .hepmc (ASCII), .hepmc3 (variant ASCII), .root (ROOT TTree), etc.
#include "HepMC3/ReaderFactory.h"
#include "HepMC3/Reader.h"
#include "HepMC3/GenEvent.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/FourVector.h"

#include <sstream>     // KeepStatuses parsing
#include <cctype>      // std::tolower
#include <stdexcept>   // std::stoi

#include <set>
#include <cmath>

ClassImp(CAP::HepMC3EventReader);

namespace CAP {

HepMC3EventReader::HepMC3EventReader()
:
EventProcessor()
{
  appendClassName("HepMC3EventReader");
  setMinimumReportLevel(Object::kInfo);
  setName("HepMC3EventReader");
  setTitle("HepMC3EventReader");
}

HepMC3EventReader::HepMC3EventReader(const HepMC3EventReader & src)
:
EventProcessor(src),
_inputFileName(src._inputFileName),
_saveFinalOnly(src._saveFinalOnly),
_saveQuarks(src._saveQuarks),
_saveNeutrinos(src._saveNeutrinos),
_savePhotons(src._savePhotons),
_saveGaugeBosons(src._saveGaugeBosons)
{ /* _reader is per-instance — never copied */ }

HepMC3EventReader & HepMC3EventReader::operator=(const HepMC3EventReader & rhs)
{
  if (this != &rhs)
    {
    EventProcessor::operator=(rhs);
    _inputFileName   = rhs._inputFileName;
    _saveFinalOnly   = rhs._saveFinalOnly;
    _saveQuarks      = rhs._saveQuarks;
    _saveNeutrinos   = rhs._saveNeutrinos;
    _savePhotons     = rhs._savePhotons;
    _saveGaugeBosons = rhs._saveGaugeBosons;
    }
  return *this;
}

HepMC3EventReader::~HepMC3EventReader()
{
  // shared_ptr handles teardown automatically.
}

// ------------------------------------------------------------------
//  setDefaultConfiguration — register the keys we read.  Mirrors the
//  Pythia generator's pattern so .ini files look familiar.
// ------------------------------------------------------------------
void HepMC3EventReader::setDefaultConfiguration()
{
  EventProcessor::setDefaultConfiguration();
  addProperty("HepMC3InputFile", String(""));
  addProperty("SaveFinalOnly",   true);
  addProperty("SaveQuarks",      false);
  addProperty("SaveNeutrinos",   false);
  addProperty("SavePhotons",     false);
  addProperty("SaveGaugeBosons", false);
  // RemovePhotons is the legacy Pythia spelling; treat it as an alias
  // for SavePhotons=0 so existing .ini files keep working.
  addProperty("RemovePhotons",   true);
  // KeepStatuses: comma-separated HepMC status codes to keep (e.g. "1",
  // "1,2", "1,2,11,21,23,51,52", or "all").  Empty falls back to
  // SaveFinalOnly behaviour (back-compat).  Enables BF-per-stage where
  // user wants subsets of the generator's status zoo (partons / shower
  // / pre-decay hadrons / final hadrons).
  addProperty("KeepStatuses",    String(""));
}

// Helper: parse a comma-separated status-list spec into the set + flag.
// Recognises "" (empty → fall back to SaveFinalOnly), "all"/"any"/"*"
// (keep all statuses), and any comma-separated integer list.
namespace {
  void _parseStatusSpec(const std::string & spec,
                        std::set<int> & out, bool & keepAll)
  {
    out.clear();
    keepAll = false;
    std::string s; s.reserve(spec.size());
    for (char c : spec) if (c != ' ' && c != '\t') s.push_back(c);
    if (s.empty()) return;
    std::string lower = s;
    for (auto & c : lower) c = std::tolower(c);
    if (lower == "all" || lower == "any" || lower == "*")
      { keepAll = true; return; }
    std::string token;
    std::stringstream ss(s);
    while (std::getline(ss, token, ','))
      {
      if (token.empty()) continue;
      try { out.insert(std::stoi(token)); }
      catch (...) { /* ignore garbage tokens — defensive */ }
      }
  }
}

// ------------------------------------------------------------------
//  initialize — read the configuration, open the HepMC3 file.
// ------------------------------------------------------------------
void HepMC3EventReader::initialize()
{
  EventProcessor::initialize();

  const String & taskName = name();
  _inputFileName   = _configuration.valueString(taskName + ":HepMC3InputFile").Data();
  _saveFinalOnly   = _configuration.valueBool  (taskName + ":SaveFinalOnly");
  _saveQuarks      = _configuration.valueBool  (taskName + ":SaveQuarks");
  _saveNeutrinos   = _configuration.valueBool  (taskName + ":SaveNeutrinos");
  // RemovePhotons=1 (legacy default) implies SavePhotons=0.
  bool removePhotons = _configuration.valueBool(taskName + ":RemovePhotons");
  _savePhotons     = _configuration.valueBool  (taskName + ":SavePhotons");
  if (removePhotons) _savePhotons = false;
  _saveGaugeBosons = _configuration.valueBool  (taskName + ":SaveGaugeBosons");

  // KeepStatuses: parse "1,2,11,21,..." or "all" or "" (back-compat).
  // Empty: fall back to SaveFinalOnly (=> {1} or {} per old behaviour).
  // Non-empty: explicit whitelist.
  const std::string statusSpec =
    _configuration.valueString(taskName + ":KeepStatuses").Data();
  _parseStatusSpec(statusSpec, _keepStatuses, _keepAllStatuses);
  if (statusSpec.empty())
    {
    // Back-compat: SaveFinalOnly=1 → {1}; SaveFinalOnly=0 → keep all.
    if (_saveFinalOnly) _keepStatuses.insert(1);
    else                _keepAllStatuses = true;
    }

  if (_inputFileName.empty() || _inputFileName == "NONE")
    throw Exception("HepMC3EventReader: HepMC3InputFile not set "
                    "(use <task>:HepMC3InputFile = path/to/events.hepmc)",
                    "HepMC3EventReader::initialize");

  printCR();
  printValue("HepMC3EventReader: input file",  String(_inputFileName.c_str()));
  printValue("HepMC3EventReader: saveFinalOnly", _saveFinalOnly);
  printValue("HepMC3EventReader: KeepStatuses",  String(statusSpec.c_str()));
  printValue("HepMC3EventReader: keepAllStatuses", _keepAllStatuses);
  {
    std::stringstream ss; bool first = true;
    for (int s : _keepStatuses) { if (!first) ss << ","; ss << s; first = false; }
    printValue("HepMC3EventReader: parsed status set", String(ss.str().c_str()));
  }

  // ReaderFactory auto-detects the format from the file extension /
  // first line and returns the right concrete Reader (ASCII, ROOT,
  // ROOTtree, plain ASCII).  Returns shared_ptr<Reader> — we hold one.
  _reader = HepMC3::deduce_reader(_inputFileName);
  if (!_reader || _reader->failed())
    throw Exception(String("HepMC3EventReader: cannot open ")
                    + _inputFileName.c_str(),
                    "HepMC3EventReader::initialize");

  _eof = false;
}

// ------------------------------------------------------------------
//  execute — read one event, copy final-state particles into the
//  CAP event stream.  Mirrors PythiaEventGenerator::execute() so the
//  downstream analyzers see exactly the same particle interface.
// ------------------------------------------------------------------
void HepMC3EventReader::execute()
{
  if (_eof || !_reader) return;

  ParticleDb & particleTypeList = db();
  Event &      theEvent         = event();
  theEvent.reset();

  HepMC3::GenEvent ev(HepMC3::Units::GEV, HepMC3::Units::MM);
  if (!_reader->read_event(ev) || _reader->failed())
    {
    _eof = true;
    return;
    }

  for (auto & p : ev.particles())
    {
    if (!p) continue;
    const int pdg    = p->pid();
    const int status = p->status();   // HepMC3 uses Pythia status conventions

    // Status-code filter: KeepStatuses (or back-compat SaveFinalOnly).
    // _keepAllStatuses=true means accept anything; otherwise we
    // require status ∈ _keepStatuses set.  Empty set + !keepAll
    // means "nothing passes" — that's the user's choice.
    if (!_keepAllStatuses && _keepStatuses.count(status) == 0) continue;
    if (!_saveQuarks     && std::abs(pdg) < 10)               continue;
    if (!_saveNeutrinos  && (std::abs(pdg) == 12 ||
                             std::abs(pdg) == 14 ||
                             std::abs(pdg) == 16 ||
                             std::abs(pdg) == 18))            continue;
    if (!_savePhotons    && pdg == 22)                        continue;
    if (!_saveGaugeBosons && (std::abs(pdg) > 22 &&
                              std::abs(pdg) < 40))            continue;

    // PDG → ParticleType lookup.  Same defensive pattern as
    // PythiaEventGenerator: skip unknown PDGs with a one-shot warning
    // rather than aborting the run.  HepMC3 events from HERWIG often
    // carry mass-eigenstate kaons (130, 310) and excited resonances
    // that aren't in the shipped DB/ParticleData/particles.data.
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
        printValue("HepMC3EventReader: skipping unknown PDG", pdg);
        }
      continue;
      }

    Particle & particle = Particle::factory().nextObject();
    particle.setType(particleType);
    particle.setLive(1);
    const HepMC3::FourVector & m = p->momentum();
    particle.setEPxPyPz(m.e(), m.px(), m.py(), m.pz());

    // Production vertex — HepMC3 stores it on the production GenVertex.
    // If absent, fall back to (0,0,0,0).  CAP uses XYZT with t in the
    // first slot: see Particle::setXYZT(x,y,z,t).
    if (auto pv = p->production_vertex())
      {
      const HepMC3::FourVector & xv = pv->position();
      particle.setXYZT(xv.x(), xv.y(), xv.z(), xv.t());
      }
    else
      {
      particle.setXYZT(0.0, 0.0, 0.0, 0.0);
      }

    theEvent.addParticle(&particle);
    }

  eventAccepted().increment();
  _taskExecuted.increment();
}

// ------------------------------------------------------------------
//  finalize — close the file.
// ------------------------------------------------------------------
void HepMC3EventReader::finalize()
{
  if (_reader)
    {
    _reader->close();
    _reader.reset();      // releases the shared_ptr; Reader dtor runs
    }
  EventProcessor::finalize();
}

} // namespace CAP
