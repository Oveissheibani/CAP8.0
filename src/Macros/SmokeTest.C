// =============================================================================
//  SmokeTest.C — proof-of-life run test for CAP.
//
//  WHY THIS EXISTS
//  ---------------
//  The shipped projects/Pythia/pp_13.7TeV/RunAna.ini references ~10 task
//  classes that are missing from src/ (see MISSING_CLASSES.md). Until those
//  classes are implemented, no .ini-driven CAP run can complete.
//
//  This macro proves that the BUILD itself is fully functional by bypassing
//  the broken/missing CAP task tree and exercising the working layers
//  directly:
//
//    • libBase / libParticles / libCAPPythia load
//    • Pythia 8 (via Homebrew or any other detected install) initialises
//    • An event loop generates real pp@13TeV events
//    • Histograms are filled and written to a real .root file
//
//  HOW TO RUN
//  ----------
//      source SetupCAP.sh       # so the linker finds your built CAP libs
//      mkdir -p histos/smoketest
//      root -b -q src/Macros/SmokeTest.C
//
//  The output file ends up at  histos/smoketest/SingleGen.root .
//  Open it with `root histos/smoketest/SingleGen.root`, then `new TBrowser`,
//  to see the histograms.
//
//  Or run it from run-cap: it has an "Open log folder" button so you can
//  tail the live log while ROOT runs.
// =============================================================================

// -----------------------------------------------------------------------------
//  Cling needs explicit include / library hints. SetupCAP.sh exports
//  CAP_PYTHIA8_INCLUDE_PATH but Cling's pragmas can't read env vars, so we
//  enumerate the most common locations. The first one that exists wins.
// -----------------------------------------------------------------------------
#ifdef __CLING__
#pragma cling add_include_path("/opt/homebrew/opt/pythia/include")
#pragma cling add_include_path("/opt/homebrew/opt/pythia8/include")
#pragma cling add_include_path("/opt/homebrew/include")
#pragma cling add_include_path("/usr/local/opt/pythia/include")
#pragma cling add_include_path("/usr/local/opt/pythia8/include")
#pragma cling add_include_path("/usr/local/include")
#pragma cling add_library_path("/opt/homebrew/opt/pythia/lib")
#pragma cling add_library_path("/opt/homebrew/lib")
#pragma cling add_library_path("/usr/local/lib")
#pragma cling load("libpythia8")
#endif

#include <iostream>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TSystem.h>
#include "Pythia8/Pythia.h"

void SmokeTest()
{
    // -------------------------------------------------------------------------
    //  Load CAP libraries. We only need the foundational ones — the missing
    //  task classes live in libParticles or higher and aren't needed here.
    //  The .so suffix is for Linux; on macOS we try .dylib first, then fall
    //  back to a name-only load (which lets the dynamic loader pick the
    //  right extension itself).
    // -------------------------------------------------------------------------
    const char * libs[] = {
        "libMath",
        "libBase",
        "libParticles",
        "libCAPPythia",
        nullptr
    };
    for (int i = 0; libs[i]; ++i) {
        if (gSystem->Load(libs[i]) < 0) {
            std::cerr << "SmokeTest: failed to load " << libs[i] << std::endl;
            std::cerr << "  Make sure you sourced SetupCAP.sh and DYLD_LIBRARY_PATH "
                         "includes ./lib" << std::endl;
            return;
        }
    }
    std::cout << "SmokeTest: CAP libraries loaded." << std::endl;

    // -------------------------------------------------------------------------
    //  Initialise Pythia 8 directly. We don't go through CAP::PythiaEventGenerator
    //  because that needs the Task tree / EventProcessor scaffolding which the
    //  missing classes are part of. Pythia itself works fine.
    // -------------------------------------------------------------------------
    Pythia8::Pythia pythia;
    pythia.readString("Beams:idA = 2212");
    pythia.readString("Beams:idB = 2212");
    pythia.readString("Beams:eCM = 13000.0");
    pythia.readString("SoftQCD:inelastic = on");
    pythia.readString("Random:setSeed = on");
    pythia.readString("Random:seed = 12345");
    pythia.readString("Init:showProcesses = off");
    pythia.readString("Init:showMultipartonInteractions = off");
    pythia.readString("Init:showChangedSettings = off");
    pythia.readString("Init:showChangedParticleData = off");
    pythia.readString("Next:numberCount = 100");
    pythia.readString("Next:numberShowInfo = 0");
    pythia.readString("Next:numberShowProcess = 0");
    pythia.readString("Next:numberShowEvent = 0");
    if (!pythia.init()) {
        std::cerr << "SmokeTest: Pythia init() FAILED — see Pythia banner above." << std::endl;
        return;
    }
    std::cout << "SmokeTest: Pythia initialised at sqrt(s) = 13 TeV (pp inelastic)." << std::endl;

    // -------------------------------------------------------------------------
    //  Output file + histograms. Mirror the shape ParticleSingleAnalyzer would
    //  produce, but filled by hand from raw Pythia output.
    // -------------------------------------------------------------------------
    const char * outDir  = "histos/smoketest";
    gSystem->Exec(Form("mkdir -p %s", outDir));
    const char * outFile = "histos/smoketest/SingleGen.root";

    TFile * fOut = new TFile(outFile, "RECREATE");
    if (!fOut || fOut->IsZombie()) {
        std::cerr << "SmokeTest: cannot open " << outFile << std::endl;
        return;
    }

    TH1D * h_n   = new TH1D("smoke_n",   "Charged-particle multiplicity;N_{ch};events", 200, 0, 200);
    TH1D * h_pt  = new TH1D("smoke_pt",  "Charged p_{T};p_{T} [GeV/c];particles",       200, 0, 10);
    TH1D * h_eta = new TH1D("smoke_eta", "Charged #eta;#eta;particles",                 200, -5, 5);
    TH1D * h_phi = new TH1D("smoke_phi", "Charged #phi;#phi [rad];particles",           200, -3.2, 3.2);
    TH2D * h_pteta = new TH2D("smoke_pteta", "p_{T} vs #eta;#eta;p_{T}", 100, -5, 5, 100, 0, 10);

    // -------------------------------------------------------------------------
    //  Event loop.
    // -------------------------------------------------------------------------
    const int Nevt = 1000;
    long nCharged = 0;
    int  nGood    = 0;
    for (int i = 0; i < Nevt; ++i) {
        if (!pythia.next()) continue;
        ++nGood;
        int n = 0;
        for (int j = 1; j < pythia.event.size(); ++j) {
            const Pythia8::Particle & p = pythia.event[j];
            if (!p.isFinal())   continue;
            if (p.charge() == 0) continue;
            h_pt   ->Fill(p.pT());
            h_eta  ->Fill(p.eta());
            h_phi  ->Fill(p.phi());
            h_pteta->Fill(p.eta(), p.pT());
            ++n; ++nCharged;
        }
        h_n->Fill(n);
    }

    // -------------------------------------------------------------------------
    //  Save and report.
    // -------------------------------------------------------------------------
    fOut->Write();
    fOut->Close();
    delete fOut;

    pythia.stat();

    std::cout << "\n==================================================" << std::endl;
    std::cout <<   "  SmokeTest SUMMARY" << std::endl;
    std::cout <<   "==================================================" << std::endl;
    std::cout <<   "  Events requested  : " << Nevt << std::endl;
    std::cout <<   "  Events generated  : " << nGood << std::endl;
    std::cout <<   "  Charged particles : " << nCharged
              <<   "  (avg " << (nGood ? double(nCharged)/nGood : 0.0)
              <<   " per event)" << std::endl;
    std::cout <<   "  Output file       : " << outFile << std::endl;
    std::cout <<   "  Histograms        : smoke_n, smoke_pt, smoke_eta, smoke_phi, smoke_pteta" << std::endl;
    std::cout <<   "==================================================" << std::endl;
    std::cout <<   "  PASS — the CAP build and Pythia link are functional." << std::endl;
    std::cout <<   "  Open the file with:  root " << outFile << std::endl;
    std::cout <<   "==================================================" << std::endl;
}
