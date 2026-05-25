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

#include "Pythia8/Pythia.h"

#include "TFile.h"
#include "TH1D.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace CAP;

namespace
{
void usage(const char * prog)
{
  std::cout <<
    "Usage: " << prog << " [options]\n"
    "  -n, --events N     number of events to generate   (default 10000)\n"
    "  -s, --species PDG  |pdg| of the species to study   (default 0 = all)\n"
    "      --ecm GeV      pp centre-of-mass energy        (default 13000)\n"
    "      --seed N       Pythia random seed              (default 12345)\n"
    "      --process X    'soft' or 'hard' QCD            (default soft)\n"
    "      --config FILE  extra Pythia commands, one per line (the hook\n"
    "                     for the mechanism-ablation ladder)\n"
    "      --xmldoc DIR   Pythia xmldoc directory         (default: auto)\n"
    "  -o, --out FILE     output ROOT file                (default provenance.root)\n"
    "  -h, --help         show this message\n";
}
} // namespace

int main(int argc, char ** argv)
{
  long        nEvents = 10000;
  int         species = 0;
  double      ecm     = 13000.0;
  long        seed    = 12345;
  std::string process = "soft";
  std::string config;
  std::string xmldoc;
  std::string outName = "provenance.root";

  for (int i = 1; i < argc; ++i)
    {
    const std::string a = argv[i];
    // fetch the value that follows an option
    auto val = [&](const char * what) -> std::string
      {
      if (i + 1 >= argc)
        { std::cerr << "missing value for " << what << "\n"; std::exit(2); }
      return argv[++i];
      };
    if      (a == "-n" || a == "--events")  nEvents = std::atol(val("--events").c_str());
    else if (a == "-s" || a == "--species") species = std::atoi(val("--species").c_str());
    else if (a == "--ecm")                  ecm     = std::atof(val("--ecm").c_str());
    else if (a == "--seed")                 seed    = std::atol(val("--seed").c_str());
    else if (a == "--process")              process = val("--process");
    else if (a == "--config")               config  = val("--config");
    else if (a == "--xmldoc")               xmldoc  = val("--xmldoc");
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

  // ---- event loop -----------------------------------------------------
  PythiaHistoryBuilder  builder;
  ProvenanceTagger      tagger;
  ProvenanceObservables obs(species);
  EventHistory          history;

  long generated = 0;
  for (long i = 0; i < nEvents; ++i)
    {
    if (!pythia->next()) continue;
    builder.build(pythia->event, history);
    const std::vector<ProvenanceTag> tags = tagger.tagFinalState(history);
    obs.accumulate(history, tags);
    ++generated;
    if (generated % 1000 == 0)
      std::cout << "  ... " << generated << " events\r" << std::flush;
    }
  std::cout << "  generated " << generated << " event(s)\n\n";

  // ---- report ---------------------------------------------------------
  const std::string summary = obs.report();
  std::cout << summary << "\n";

  // ---- ROOT output ----------------------------------------------------
  TFile fout(outName.c_str(), "RECREATE");
  if (fout.IsZombie())
    { std::cerr << "cannot open output file: " << outName << "\n"; return 1; }
  for (const auto & kv : obs.histograms())
    {
    const Hist1D & h = kv.second;
    TH1D th(h.name.c_str(), h.title.c_str(), h.nbins, h.lo, h.hi);
    for (int b = 0; b < h.nbins; ++b)
      th.SetBinContent(b + 1, h.counts[static_cast<size_t>(b)]);
    th.SetEntries(h.entries);
    th.Write();
    }
  fout.Close();
  std::cout << "wrote " << obs.histograms().size()
            << " histograms to " << outName << "\n";

  // ---- text summary next to the ROOT file -----------------------------
  const std::string txtName = outName + ".txt";
  std::ofstream tf(txtName.c_str());
  if (tf)
    {
    tf << "provenance-study summary\n"
       << "  events="  << generated << "  ecm=" << ecm
       << "  process=" << process   << "  seed=" << seed << "\n\n"
       << summary;
    std::cout << "wrote summary to " << txtName << "\n";
    }

  return 0;
}
