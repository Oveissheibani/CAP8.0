/* **********************************************************************
 *  provenance-study  —  Phase 3b runner of the parton-tracking feature
 *
 *  A self-contained study, deliberately isolated from the main CAP
 *  analyzer chain:
 *
 *    1. generate pp events with Pythia 8
 *    2. build the EventHistory provenance graph from each event record
 *    3. tag every final-state hadron with its origin (ProvenanceTagger)
 *    4. accumulate single-particle + global observables, decomposed by
 *       origin class and by parent-parton flavour (ProvenanceObservables)
 *    5. write the histograms to a ROOT file + a text summary
 *
 *  It answers, directly: what fraction of the final-state yield is a
 *  direct hadronization product, what fraction is resonance feed-down,
 *  and how the spectra of each origin differ.
 *
 *  Build: produced as bin/provenance-study when CAP_ENABLE_PYTHIA=ON.
 *  Usage: ./bin/provenance-study --help
 * ********************************************************************/
#include "EventHistory.hpp"
#include "PythiaHistoryBuilder.hpp"
#include "ProvenanceTagger.hpp"
#include "ProvenanceObservables.hpp"
#include "PairProvenanceObservables.hpp"

#include "Pythia8/Pythia.h"

// Optional HepMC3 input path: lets provenance-study consume events from ANY
// generator that writes HepMC3 (Herwig, Sherpa, EPOS, ...) via the existing
// HepMC3HistoryBuilder, instead of generating with Pythia.  Compiled only when
// the build enables HepMC3 (see CMakeLists CAP_ENABLE_HEPMC3).
#ifdef CAP_ENABLE_HEPMC3
#include "HepMC3HistoryBuilder.hpp"
#include "HepMC3/GenEvent.h"
#include "HepMC3/ReaderFactory.h"
#endif

#include "TFile.h"
#include "TH1D.h"

#include <algorithm>     // std::find for the DAG validator, std::min
#include <cctype>        // std::isspace in parseSpeciesList
#include <chrono>
#include <cmath>
#include <cstdio>        // snprintf in jsonEscape
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>        // std::numeric_limits — kinematic-cut defaults
#include <memory>
#include <sstream>
#include <string>
#include <vector>

using namespace CAP;

namespace
{
// File-static pointer to the live Pythia particle DB, used by the
// resonance resolver below.  Set in main() after pythia->init().
Pythia8::Pythia * s_pythia = nullptr;

// Resolver consulted by ProvenanceTagger::isResonance.  Returns
//   1  — yes, short-lived resonance (strong / EM decay)
//   0  — no, stable / long-lived (weak-decay parent or fully stable)
//  -1  — don't know (PDG not in DB) — fall back to the curated list
//
// Rationale:
//   - Strong/EM decays:   rho, omega, phi, K*, Delta, eta, eta'...
//                         Pythia stores them with mWidth > 0 (Breit-Wigner)
//                         and tau0 == 0.  Detect via mWidth > 0.
//   - Weak decays / longer-lived:   K0S (cτ ~ 27 mm), Lambda (~79 mm),
//                         D (~0.1 mm), B (~0.5 mm).  Pythia stores them
//                         with mWidth == 0 and tau0 > 0 in mm.  Treat
//                         anything with cτ above the threshold as a
//                         weak-decay parent, NOT a resonance.
//   - Stable:             tau0 == 0 and mWidth == 0 — pion, kaon-charged
//                         (treated effectively stable in Pythia), proton.
//                         Return 0.
//
// Threshold below the weak-decay scale: 0.01 mm = 10 µm cleanly separates
// even charm cτ (~100 µm) from "resonance".  Adjust later if a finer cut
// is needed for B-physics studies.
constexpr double CTAU_RESONANCE_MM = 0.01;

int pythiaResonanceResolver(int pdg)
{
  if (!s_pythia) return -1;
  const Pythia8::ParticleData & pd = s_pythia->particleData;
  const int apdg = std::abs(pdg);
  if (!pd.isParticle(apdg)) return -1;
  const double mWidth  = pd.mWidth(apdg);
  const double tau0_mm = pd.tau0(apdg);
  // Strong / EM-decaying resonance: non-zero width is the unambiguous
  // signal.  Pythia stores rho, omega, phi, K*, Delta, eta', etc. with
  // mWidth > 0.  These are exactly the species we want to call resonances.
  if (mWidth > 0.0) return 1;
  // Long-lived but finite lifetime (K0S, Lambda, charm, bottom, ...):
  // tau0 > threshold → not a resonance.
  if (tau0_mm > CTAU_RESONANCE_MM) return 0;
  // Borderline / undefined lifetimes — anything with tau0 in (0, threshold)
  // is treated as a resonance (very short-lived); zero tau0 and zero width
  // is genuinely stable (pi±, K±, p, e, ...) and not a resonance.
  if (tau0_mm > 0.0 && tau0_mm < CTAU_RESONANCE_MM) return 1;
  return 0;
}

void usage(const char * prog)
{
  std::cout <<
    "Usage: " << prog << " [options]\n"
    "  -n, --events N        events to generate           (default 10000)\n"
    "  -s, --species PDG     |pdg| of the species, OR a comma-separated\n"
    "                        list e.g. 211,321,2212.  Single PDG keeps\n"
    "                        legacy histogram names; a list emits both\n"
    "                        single-particle observables per species and\n"
    "                        pair observables for every species pair,\n"
    "                        tagged with _S<pdg> / _S<a>x<b> suffixes.\n"
    "                        Default: 0 (all final-state hadrons).\n"
    "      --ecm GeV         pp centre-of-mass energy     (default 13000)\n"
    "      --seed N          Pythia random seed           (default 12345)\n"
    "      --process X       'soft' or 'hard' QCD         (default soft)\n"
    "      --config FILE     extra Pythia commands        (the mechanism\n"
    "                        ladder hook; alias: --pythia-cmnd)\n"
    "      --xmldoc DIR      Pythia xmldoc directory      (default: auto)\n"
    "      --hepmc3 FILE     read events from a HepMC3 file (Herwig/Sherpa/\n"
    "                        EPOS/...) instead of generating with Pythia;\n"
    "                        requires a HepMC3-enabled build\n"
    "      --pt-min  X       reject hadrons with pT < X   (default 0.0)\n"
    "      --pt-max  X       reject hadrons with pT > X   (default infinity)\n"
    "      --eta-min X       reject hadrons with eta < X  (default -infinity)\n"
    "      --eta-max X       reject hadrons with eta > X  (default infinity)\n"
    "      --mult-low  N     LowMult cut (default 20; pair mult-bin boundary)\n"
    "      --mult-high N     HighMult cut (default 80)\n"
    "      --sphero-low X    JetLike/MidShape S0 cut (default 0.3)\n"
    "      --sphero-high X   MidShape/Isotropic S0 cut (default 0.7)\n"
    "      --validate-graph  abort on the first malformed event-history DAG\n"
    "      --dump-events N   dump the first N events' full ancestry to a\n"
    "                        sibling .events.json file for the GUI explorer\n"
    "  -o, --out FILE        output ROOT file             (default provenance.root)\n"
    "  -h, --help            show this message\n";
}

// ---------------------------------------------------------------------------
//  Tiny JSON writer — only what the explorer dump needs.
//
//  We deliberately avoid pulling in a third-party JSON library.  The schema
//  is fixed and shallow; std::ostream manipulation is simpler than adding a
//  dependency.  The output is one JSON document containing a top-level
//  `events: [ { event_id, multiplicity, hadrons: [...], pairs: [...] }, ...]`
//  plus a header block with the run configuration.  Strings are escaped
//  the minimum amount needed for valid JSON (quote, backslash, control
//  chars below 0x20).
// ---------------------------------------------------------------------------
std::string jsonEscape(const std::string & s)
{
  std::ostringstream os;
  for (char c : s)
    {
    switch (c)
      {
      case '"':  os << "\\\""; break;
      case '\\': os << "\\\\"; break;
      case '\n': os << "\\n";  break;
      case '\t': os << "\\t";  break;
      case '\r': os << "\\r";  break;
      default:
        if (static_cast<unsigned char>(c) < 0x20)
          {
          char buf[8];
          std::snprintf(buf, sizeof(buf), "\\u%04x",
                        static_cast<unsigned int>(static_cast<unsigned char>(c)));
          os << buf;
          }
        else os << c;
      }
    }
  return os.str();
}

// Map a ProvenanceTag to a compact JSON object string.  Only fields useful
// for the GUI explorer are emitted; the full record can be re-derived by
// re-running provenance-study against the same seed if needed.
std::string tagToJson(const ProvenanceTag & t,
                      const ParticleNode & node)
{
  std::ostringstream os;
  os << std::fixed << std::setprecision(4);
  os << "{"
     << "\"finalIndex\":"   << t.finalIndex << ","
     << "\"finalPdg\":"     << t.finalPdg   << ","
     << "\"pt\":"           << node.pt()    << ","
     << "\"eta\":"          << node.eta()   << ","
     << "\"phi\":"          << node.phi()   << ","
     << "\"productionStage\":\"" << stageName(t.productionStage) << "\","
     << "\"isPrimary\":"     << (t.isPrimaryHadron  ? 1 : 0) << ","
     << "\"isFromDecay\":"   << (t.isFromDecay      ? 1 : 0) << ","
     << "\"isFromResonance\":" << (t.isFromResonance ? 1 : 0) << ","
     << "\"parentHadronPdg\":" << t.parentHadronPdg << ","
     << "\"resonancePdg\":"  << t.resonancePdg  << ","
     << "\"leadPartonPdg\":" << t.leadPartonPdg << ","
     << "\"hardPartonPdg\":" << t.hardPartonPdg << ","
     << "\"initiatingPartonPdg\":" << t.initiatingPartonPdg << ","
     << "\"hardPartonIndex\":" << t.hardPartonIndex << ","
     << "\"mpiIndex\":"      << t.mpiIndex << ","
     << "\"fromISR\":"       << (t.fromISR  ? 1 : 0) << ","
     << "\"fromFSR\":"       << (t.fromFSR  ? 1 : 0) << ","
     << "\"fromCharmChain\":"  << (t.fromCharmChain  ? 1 : 0) << ","
     << "\"fromBottomChain\":" << (t.fromBottomChain ? 1 : 0) << ","
     << "\"decayChainDepth\":" << t.decayChainDepth << ","
     << "\"deepestStage\":\""  << stageName(t.deepestStage) << "\","
     << "\"partonAncestors\":[";
  for (std::size_t k = 0; k < t.partonAncestorIndices.size(); ++k)
    {
    if (k) os << ",";
    os << t.partonAncestorIndices[k];
    }
  os << "]"
     << "}";
  return os.str();
}

// Lightweight DAG-consistency check used by --validate-graph.  Returns
// empty string on success, otherwise a short description of the first
// inconsistency.  Catches the usual symptoms of a botched event record:
// dangling indices, missing back-edges, self-loops.
std::string validateHistory(const EventHistory & h)
{
  const int n = h.size();
  for (int i = 0; i < n; ++i)
    {
    const ParticleNode & node = h.node(i);
    for (int p : node.parents)
      {
      if (p < 0 || p >= n)
        return "node " + std::to_string(i) + " has out-of-range parent "
               + std::to_string(p);
      if (p == i)
        return "node " + std::to_string(i) + " is its own parent";
      const auto & kids = h.node(p).children;
      if (std::find(kids.begin(), kids.end(), i) == kids.end())
        return "node " + std::to_string(i) + " lists parent " +
               std::to_string(p) + " but parent does not list it as a child";
      }
    for (int c : node.children)
      {
      if (c < 0 || c >= n)
        return "node " + std::to_string(i) + " has out-of-range child "
               + std::to_string(c);
      if (c == i)
        return "node " + std::to_string(i) + " is its own child";
      }
    }
  return "";
}
} // namespace

// Parse "211,321,2212" → [211, 321, 2212].  Accepts whitespace and
// dashes (negative PDGs are absolute-valued downstream).  An empty token
// is silently dropped; a non-integer raises.
std::vector<int> parseSpeciesList(const std::string & spec)
{
  std::vector<int> out;
  std::stringstream ss(spec);
  std::string tok;
  while (std::getline(ss, tok, ','))
    {
    // strip whitespace
    std::string s;
    for (char c : tok)
      if (!std::isspace(static_cast<unsigned char>(c))) s += c;
    if (s.empty()) continue;
    out.push_back(std::atoi(s.c_str()));
    }
  if (out.empty()) out.push_back(0);
  return out;
}

int main(int argc, char ** argv)
{
  long        nEvents = 10000;
  int         species = 0;
  std::vector<int> speciesList{0};        // canonical default: "all hadrons"
  double      ecm     = 13000.0;
  long        seed    = 12345;
  std::string process = "soft";
  std::string config;
  std::string xmldoc;
  std::string hepmcFile;     // non-empty => read HepMC3 events, skip Pythia gen
  std::string outName = "provenance/runs/provenance.root";

  // ---- kinematic cuts (Phase 4 / audit fix #9) ------------------------
  double ptMin  = 0.0;
  double ptMax  = std::numeric_limits<double>::infinity();
  double etaMin = -std::numeric_limits<double>::infinity();
  double etaMax =  std::numeric_limits<double>::infinity();
  // ---- multiplicity-bin thresholds (audit fix #3) ---------------------
  int    multLow  = 20;
  int    multHigh = 80;
  // Event-shape (transverse spherocity) bin thresholds — JetLike below
  // spheroLow, Isotropic above spheroHigh, MidShape in between.
  double spheroLow  = 0.3;
  double spheroHigh = 0.7;
  bool   validateGraph = false;
  long   dumpEvents    = 0;   // 0 = no JSON dump; N>0 = dump first N events

  for (int i = 1; i < argc; ++i)
    {
    const std::string a = argv[i];
    auto val = [&](const char * what) -> std::string
      {
      if (i + 1 >= argc)
        { std::cerr << "missing value for " << what << "\n"; std::exit(2); }
      return argv[++i];
      };
    if      (a == "-n" || a == "--events")  nEvents = std::atol(val("--events").c_str());
    else if (a == "-s" || a == "--species")
      {
      // Accept either a single PDG (e.g. "211") or a comma-separated list
      // (e.g. "211,321,2212").  Single keeps legacy histogram names.
      const std::string raw = val("--species");
      speciesList = parseSpeciesList(raw);
      species = speciesList.front();        // legacy alias for the single int
      }
    else if (a == "--ecm")                  ecm     = std::atof(val("--ecm").c_str());
    else if (a == "--seed")                 seed    = std::atol(val("--seed").c_str());
    else if (a == "--process")              process = val("--process");
    else if (a == "--config" ||
             a == "--pythia-cmnd")          config  = val("--config");
    else if (a == "--xmldoc")               xmldoc  = val("--xmldoc");
    else if (a == "--hepmc3")               hepmcFile = val("--hepmc3");
    else if (a == "--pt-min")               ptMin   = std::atof(val("--pt-min").c_str());
    else if (a == "--pt-max")               ptMax   = std::atof(val("--pt-max").c_str());
    else if (a == "--eta-min")              etaMin  = std::atof(val("--eta-min").c_str());
    else if (a == "--eta-max")              etaMax  = std::atof(val("--eta-max").c_str());
    else if (a == "--mult-low")             multLow  = std::atoi(val("--mult-low").c_str());
    else if (a == "--mult-high")            multHigh = std::atoi(val("--mult-high").c_str());
    else if (a == "--sphero-low")           spheroLow  = std::atof(val("--sphero-low").c_str());
    else if (a == "--sphero-high")          spheroHigh = std::atof(val("--sphero-high").c_str());
    else if (a == "--validate-graph")       validateGraph = true;
    else if (a == "--dump-events")          dumpEvents    = std::atol(val("--dump-events").c_str());
    else if (a == "-o" || a == "--out")     outName = val("--out");
    else if (a == "-h" || a == "--help")    { usage(argv[0]); return 0; }
    else { std::cerr << "unknown option: " << a << "\n"; usage(argv[0]); return 2; }
    }

  std::cout << "provenance-study — Phase 3b (parton-tracking)\n"
            << "  events  : " << nEvents << "\n"
            << "  species : " << (species ? std::to_string(species)
                                          : std::string("all final hadrons")) << "\n"
            << "  ecm     : " << ecm << " GeV\n"
            << "  process : " << process << "\n"
            << "  output  : " << outName << "\n\n";

  // ---- Pythia 8 setup -------------------------------------------------
  std::unique_ptr<Pythia8::Pythia> pythia(
      xmldoc.empty() ? new Pythia8::Pythia()
                     : new Pythia8::Pythia(xmldoc));

  pythia->readString("Beams:idA = 2212");
  pythia->readString("Beams:idB = 2212");
  pythia->readString("Beams:eCM = " + std::to_string(ecm));
  pythia->readString(process == "hard" ? "HardQCD:all = on"
                                       : "SoftQCD:nonDiffractive = on");
  pythia->readString("Random:setSeed = on");
  pythia->readString("Random:seed = " + std::to_string(seed));

  // Extra commands — this is the hook the mechanism-ablation ladder
  // (Engine 1) uses: a .cmnd file that toggles shower / MPI / CR / rope.
  if (!config.empty())
    {
    std::ifstream cf(config.c_str());
    if (!cf)
      { std::cerr << "cannot open --config file: " << config << "\n"; return 2; }
    std::string line;
    while (std::getline(cf, line))
      {
      if (line.empty() || line[0] == '#' || line[0] == '!') continue;
      pythia->readString(line);
      }
    std::cout << "  applied Pythia commands from " << config << "\n\n";
    }

  if (!pythia->init())
    { std::cerr << "Pythia initialisation failed.\n"; return 1; }

  // Install the data-driven resonance resolver — once Pythia is initialised
  // its particle database is queryable for cτ values, which is much more
  // accurate than the curated PDG list (audit fix #4).  The curated list
  // remains as a fallback in case the resolver returns -1.
  s_pythia = pythia.get();
  ProvenanceTagger::setResonanceResolver(pythiaResonanceResolver);

  // ---- event loop -----------------------------------------------------
  PythiaHistoryBuilder      builder;
  ProvenanceTagger          tagger;
  // Kinematic cuts + multiplicity thresholds are forwarded into both
  // accumulators so the per-pion filter is applied uniformly.  Old
  // single-arg constructors stay backward-compatible via default values.
  // Pick the constructor based on whether the user supplied a multi-
  // species list.  Single-PDG path keeps legacy histogram names exactly.
  std::unique_ptr<ProvenanceObservables>     obsPtr;
  std::unique_ptr<PairProvenanceObservables> pairPtr;
  if (speciesList.size() > 1)
    {
    obsPtr  = std::unique_ptr<ProvenanceObservables>(
        new ProvenanceObservables(speciesList, ptMin, ptMax, etaMin, etaMax));
    pairPtr = std::unique_ptr<PairProvenanceObservables>(
        new PairProvenanceObservables(speciesList, ptMin, ptMax, etaMin, etaMax,
                                      multLow, multHigh,
                                      spheroLow, spheroHigh));
    }
  else
    {
    obsPtr  = std::unique_ptr<ProvenanceObservables>(
        new ProvenanceObservables(species, ptMin, ptMax, etaMin, etaMax));
    pairPtr = std::unique_ptr<PairProvenanceObservables>(
        new PairProvenanceObservables(species, ptMin, ptMax, etaMin, etaMax,
                                      multLow, multHigh,
                                      spheroLow, spheroHigh));
    }
  ProvenanceObservables &     obs     = *obsPtr;
  PairProvenanceObservables & pairObs = *pairPtr;
  EventHistory              history;

  // ---- optional HepMC3 event source (Herwig etc.) ---------------------
  // When --hepmc3 is given we read events from the file via the existing
  // HepMC3HistoryBuilder instead of generating with Pythia.  Pythia is still
  // initialised above so the resonance resolver (cτ-by-PDG, generator-
  // agnostic) stays available for the FromResonance tag.
#ifdef CAP_ENABLE_HEPMC3
  HepMC3HistoryBuilder            hepBuilder;
  std::shared_ptr<HepMC3::Reader> hepReader;
  if (!hepmcFile.empty())
    {
    hepReader = HepMC3::deduce_reader(hepmcFile);
    if (!hepReader || hepReader->failed())
      {
      std::cerr << "cannot open --hepmc3 file: " << hepmcFile << "\n";
      return 2;
      }
    std::cout << "  reading HepMC3 events from " << hepmcFile << "\n\n";
    }
#else
  if (!hepmcFile.empty())
    {
    std::cerr << "--hepmc3 given but this build has no HepMC3 support "
                 "(rebuild with CAP_ENABLE_HEPMC3=ON)\n";
    return 2;
    }
#endif

  // Tally what actually happens in the loop so it can be reported and
  // surfaced in the .root.txt summary — audit fix #2.
  long attempted     = 0;
  long pythia_failed = 0;
  long graph_failed  = 0;
  long succeeded     = 0;
  const auto t0 = std::chrono::steady_clock::now();

  // Event-explorer dump buffer.  We collect at most `dumpEvents` events
  // worth of JSON-serialized payloads in memory; written to disk at the
  // end alongside the .root and .txt summaries.  Capped so a runaway
  // --dump-events 1_000_000 doesn't OOM.
  const long dumpCap = std::min<long>(dumpEvents, 10000);
  std::vector<std::string> eventDumps;
  eventDumps.reserve(static_cast<std::size_t>(dumpCap));

  for (long i = 0; i < nEvents; ++i)
    {
    if (!hepmcFile.empty())
      {
#ifdef CAP_ENABLE_HEPMC3
      HepMC3::GenEvent ge;
      hepReader->read_event(ge);
      if (hepReader->failed()) break;          // end of HepMC3 file
      ++attempted;
      hepBuilder.build(ge, history);
#endif
      }
    else
      {
      ++attempted;
      if (!pythia->next()) { ++pythia_failed; continue; }
      builder.build(pythia->event, history);
      }
    if (validateGraph)
      {
      const std::string err = validateHistory(history);
      if (!err.empty())
        {
        std::cerr << "\n--validate-graph: DAG integrity failure on event "
                  << i << ": " << err << "\n";
        ++graph_failed;
        continue;
        }
      }
    const std::vector<ProvenanceTag> tags = tagger.tagFinalState(history);
    obs.accumulate(history, tags);
    pairObs.accumulate(history, tags);
    ++succeeded;

    // --- per-event dump for the explorer (opt-in, capped) ---------------
    if (dumpEvents > 0 &&
        static_cast<long>(eventDumps.size()) < dumpCap)
      {
      std::ostringstream js;
      js << std::fixed << std::setprecision(4);
      js << "{\"event_id\":" << (succeeded - 1) << ","
         << "\"multiplicity\":" << tags.size() << ",";

      // Full event-history DAG.  Each entry is one node — enough to
      // render the genealogy tree client-side: index, pdg, stage,
      // isFinal, kinematics, and parent indices.  Children are derivable
      // from the parent edges so we don't duplicate.  Skip Pythia's
      // bookkeeping pseudo-particle at index 0.
      js << "\"nodes\":[";
      bool firstNode = true;
      for (int ni = 1; ni < history.size(); ++ni)
        {
        const ParticleNode & n = history.node(ni);
        if (!firstNode) js << ","; firstNode = false;
        js << "{\"i\":" << ni
           << ",\"pdg\":"   << n.pdg
           << ",\"stage\":\"" << stageName(n.stage) << "\""
           << ",\"isFinal\":" << (n.isFinal ? 1 : 0)
           << ",\"pt\":"   << n.pt()
           << ",\"eta\":"  << n.eta()
           << ",\"phi\":"  << n.phi()
           << ",\"parents\":[";
        for (std::size_t k = 0; k < n.parents.size(); ++k)
          {
          if (k) js << ",";
          js << n.parents[k];
          }
        js << "]}";
        }
      js << "],";

      js << "\"hadrons\":[";
      // Only emit hadrons matching the species filter AND the acceptance
      // window, so the explorer view is in lock-step with the histograms.
      bool first = true;
      std::vector<int> studiedIdx;
      for (std::size_t k = 0; k < tags.size(); ++k)
        {
        const ProvenanceTag & t = tags[k];
        if (species != 0 && std::abs(t.finalPdg) != species) continue;
        if (t.finalIndex < 0 || t.finalIndex >= history.size()) continue;
        const ParticleNode & node = history.node(t.finalIndex);
        const double pt = node.pt(), eta = node.eta();
        if (pt < ptMin || pt > ptMax)   continue;
        if (eta < etaMin || eta > etaMax) continue;
        if (!first) js << ","; first = false;
        js << tagToJson(t, node);
        studiedIdx.push_back(static_cast<int>(k));
        }
      // Pair classifications — every unordered pair of studied hadrons.
      js << "],\"pairs\":[";
      first = true;
      for (std::size_t i = 0; i < studiedIdx.size(); ++i)
        for (std::size_t j = i + 1; j < studiedIdx.size(); ++j)
          {
          const ProvenanceTag & ta = tags[studiedIdx[i]];
          const ProvenanceTag & tb = tags[studiedIdx[j]];
          const PairClass pc       = classifyPair(ta, tb);
          if (!first) js << ","; first = false;
          js << "{\"a\":" << ta.finalIndex
             << ",\"b\":" << tb.finalIndex
             << ",\"class\":\"" << pairClassName(pc) << "\""
             << ",\"sharesHardParton\":"
             << (sharesHardProcessAncestor(ta, tb) ? 1 : 0)
             << ",\"sharesMPI\":"
             << (sharesMPIVertex(ta, tb) ? 1 : 0)
             << ",\"sharesDecayParent\":"
             << (sharesDecayParent(ta, tb) ? 1 : 0)
             << ",\"flavourPair\":[" << ta.hardPartonPdg
             << "," << tb.hardPartonPdg << "]"
             << "}";
          }
      js << "]}";
      eventDumps.push_back(js.str());
      }

    (void)0;  // keep block self-contained
    if (succeeded % 1000 == 0)
      std::cout << "  ... " << succeeded << " events\r" << std::flush;
    }
  const auto t1 = std::chrono::steady_clock::now();
  const double wallSec = std::chrono::duration<double>(t1 - t0).count();
  std::cout << "  attempted " << attempted
            << "; succeeded " << succeeded
            << "; pythia failed " << pythia_failed;
  if (validateGraph) std::cout << "; graph failed " << graph_failed;
  std::cout << "  (" << wallSec << " s)\n\n";
  const long generated = succeeded;   // kept for the rest of the function

  // ---- reports --------------------------------------------------------
  const std::string summarySingle = obs.report();
  const std::string summaryPair   = pairObs.report();
  std::cout << summarySingle << "\n" << summaryPair << "\n";

  // ---- ROOT output ----------------------------------------------------
  // Ensure the parent directory of --out exists (mkdir -p), so a default
  // like 'provenance/runs/...' works on a fresh checkout.
  {
    std::string::size_type sep = outName.find_last_of('/');
    if (sep != std::string::npos)
      {
      std::string dir = outName.substr(0, sep);
      (void)std::system(("mkdir -p '" + dir + "'").c_str());
      }
  }
  TFile fout(outName.c_str(), "RECREATE");
  if (fout.IsZombie())
    { std::cerr << "cannot open output file: " << outName << "\n"; return 1; }

  // Convert every in-memory Hist1D in a map to a ROOT TH1D and write it.
  // Audit fix #1: Sumw2() + explicit bin errors so downstream plotters
  // can rely on sqrt(N) error bars instead of ROOT's quietly-implicit
  // fallback.  Underflow / overflow are also propagated so the bin-0
  // and bin-(nbins+1) counts are visible to anyone using GetBinContent.
  auto writeHistos = [](const std::map<std::string,Hist1D> & hs)
    {
    for (const auto & kv : hs)
      {
      const Hist1D & h = kv.second;
      TH1D th(h.name.c_str(), h.title.c_str(), h.nbins, h.lo, h.hi);
      th.Sumw2();                                       // enable proper errors
      for (int b = 0; b < h.nbins; ++b)
        {
        const double c = h.counts[static_cast<size_t>(b)];
        th.SetBinContent(b + 1, c);
        th.SetBinError  (b + 1, std::sqrt(c >= 0.0 ? c : 0.0));
        }
      // Underflow / overflow carry information about bin coverage.
      th.SetBinContent(0,           h.underflow);
      th.SetBinError  (0,           std::sqrt(h.underflow >= 0.0 ? h.underflow : 0.0));
      th.SetBinContent(h.nbins + 1, h.overflow);
      th.SetBinError  (h.nbins + 1, std::sqrt(h.overflow  >= 0.0 ? h.overflow  : 0.0));
      th.SetEntries(h.entries);
      th.Write();
      }
    };
  writeHistos(obs.histograms());
  writeHistos(pairObs.histograms());
  const size_t nHist = obs.histograms().size() + pairObs.histograms().size();
  fout.Close();
  std::cout << "wrote " << nHist << " histograms to " << outName << "\n";

  // ---- text summary next to the ROOT file -----------------------------
  const std::string txtName = outName + ".txt";
  std::ofstream tf(txtName.c_str());
  if (tf)
    {
    tf << "provenance-study summary\n"
       << "  events="    << generated
       << "  attempted=" << attempted
       << "  pythia_failed=" << pythia_failed
       << "  graph_failed="  << graph_failed
       << "  ecm="       << ecm
       << "  process="   << process
       << "  seed="      << seed
       << "  species_list=";
    for (std::size_t k = 0; k < speciesList.size(); ++k)
      tf << (k ? "," : "") << speciesList[k];
    tf << "  legacy_species=" << species
       << "  pt_min="    << ptMin
       << "  pt_max="    << ptMax
       << "  eta_min="   << etaMin
       << "  eta_max="   << etaMax
       << "  mult_low="  << multLow
       << "  mult_high=" << multHigh
       << "  sphero_low="  << spheroLow
       << "  sphero_high=" << spheroHigh
       << "  wall_seconds=" << wallSec << "\n\n"
       << summarySingle << "\n" << summaryPair;
    std::cout << "wrote summary to " << txtName << "\n";
    }

  // ---- explorer event dump -------------------------------------------
  // One JSON document with a small header + an array of events.  Keeps
  // the schema stable enough that the GUI doesn't need to know the run
  // configuration to render the events.
  if (dumpEvents > 0 && !eventDumps.empty())
    {
    const std::string jsonName = outName + ".events.json";
    std::ofstream jf(jsonName.c_str());
    if (jf)
      {
      jf << std::fixed << std::setprecision(4);
      jf << "{\n"
         << "  \"run\": {\n"
         << "    \"events\":"       << generated
         << ", \"ecm\":"            << ecm
         << ", \"process\":\""      << jsonEscape(process) << "\""
         << ", \"seed\":"           << seed
         << ", \"species\":"        << species
         << ", \"pt_min\":"         << ptMin
         << ", \"pt_max\":"         << (std::isfinite(ptMax)  ? ptMax  : 1e30)
         << ", \"eta_min\":"        << (std::isfinite(etaMin) ? etaMin : -1e30)
         << ", \"eta_max\":"        << (std::isfinite(etaMax) ? etaMax :  1e30)
         << ", \"mult_low\":"       << multLow
         << ", \"mult_high\":"      << multHigh
         << "\n  },\n"
         << "  \"events\": [\n";
      for (std::size_t k = 0; k < eventDumps.size(); ++k)
        {
        if (k) jf << ",\n";
        jf << "    " << eventDumps[k];
        }
      jf << "\n  ]\n}\n";
      std::cout << "wrote " << eventDumps.size()
                << " event(s) to " << jsonName << "\n";
      }
    else
      {
      std::cerr << "warning: could not open " << jsonName << " for write\n";
      }
    }

  return 0;
}
