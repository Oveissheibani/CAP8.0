/* ----------------------------------------------------------------------
 * Implementation of CAP::ParticlePairBfCalculator.
 *
 * Driven by stage 3 (RunBf).  Reads:
 *   - PairDerivedGen.root   (R2_*, C2_*, n1n1_* histos per filter pair)
 *   - SingleDerivedGen.root (n1_phi, n1_eta, ... per filter)
 * iterates (eventFilter × speciesPair1 × speciesPair2), computes:
 *   - CI = 0.25 * sum of 4 charge combinations
 *   - CD = 0.5  * (1Bar_2 + 1_2Bar - 1_2 - 1Bar_2Bar)
 *   - BalFct = (US - LS) / bin_width
 * for each pair observable, and writes them to PairBFGen.root.
 *
 * Loyalty note: the math (compute_CI / compute_CD / compute_BalFct)
 * is transcribed line-for-line from
 *   src/ParticlePair/BalanceFunctionCalculator.cpp:140-260
 * (Pruneau's untouched code).  The orchestration loop is reconstructed
 * from the commented-out original at lines 422-640 of the same file.
 * --------------------------------------------------------------------*/
#include "StubParticlePairBfCalculator.hpp"
#include "PrintHelpers.hpp"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include <cstdlib>
#include <vector>

ClassImp(CAP::ParticlePairBfCalculator);

namespace CAP {

ParticlePairBfCalculator::ParticlePairBfCalculator()
:
Task()
{
  appendClassName("ParticlePairBfCalculator");
  setMinimumReportLevel(Object::kInfo);
  setName("ParticlePairBfCalculator");
  setTitle("ParticlePairBfCalculator");
}

ParticlePairBfCalculator::ParticlePairBfCalculator(const ParticlePairBfCalculator & s)
:
Task(s)
{ }

ParticlePairBfCalculator &
ParticlePairBfCalculator::operator=(const ParticlePairBfCalculator & r)
{
  if (this != &r) Task::operator=(r);
  return *this;
}

// ----------------------------------------------------------------------
//  Pruneau's math, transcribed from BalanceFunctionCalculator.cpp
// ----------------------------------------------------------------------
TH2 * ParticlePairBfCalculator::compute_CI(TH2 * obs_1_2,
                                            TH2 * obs_1Bar_2,
                                            TH2 * obs_1_2Bar,
                                            TH2 * obs_1Bar_2Bar,
                                            const TString & outName)
{
  if (!obs_1_2 || !obs_1Bar_2 || !obs_1_2Bar || !obs_1Bar_2Bar) return nullptr;
  // BalanceFunctionCalculator.cpp:152-162
  TH2 * obs = static_cast<TH2*>(obs_1Bar_2->Clone());
  obs->SetName(outName);
  obs->SetTitle(outName);
  obs->Add(obs_1_2Bar);
  obs->Add(obs_1_2);
  obs->Add(obs_1Bar_2Bar);
  obs->Scale(0.25);
  obs->SetDirectory(nullptr);
  return obs;
}

TH2 * ParticlePairBfCalculator::compute_CD(TH2 * obs_1_2,
                                            TH2 * obs_1Bar_2,
                                            TH2 * obs_1_2Bar,
                                            TH2 * obs_1Bar_2Bar,
                                            const TString & outName)
{
  if (!obs_1_2 || !obs_1Bar_2 || !obs_1_2Bar || !obs_1Bar_2Bar) return nullptr;
  // BalanceFunctionCalculator.cpp:191-201
  TH2 * obs = static_cast<TH2*>(obs_1Bar_2->Clone());
  obs->SetName(outName);
  obs->SetTitle(outName);
  obs->Add(obs_1_2Bar);
  obs->Add(obs_1_2,       -1.0);
  obs->Add(obs_1Bar_2Bar, -1.0);
  obs->Scale(0.5);
  obs->SetDirectory(nullptr);
  return obs;
}

TH2 * ParticlePairBfCalculator::compute_BalFct(TH2 * obs_US,
                                                TH2 * obs_LS,
                                                const TString & outName)
{
  if (!obs_US || !obs_LS) return nullptr;
  // BalanceFunctionCalculator.cpp:230-243
  TH2 * obs = static_cast<TH2*>(obs_US->Clone());
  obs->SetName(outName);
  obs->SetTitle(outName);
  obs->Add(obs_LS, -1.0);
  double wx = obs->GetXaxis()->GetBinWidth(1);
  if (wx > 0.0) obs->Scale(1.0 / wx);
  // rho1 normalization is commented-out in the original (line 243); same here.
  obs->SetDirectory(nullptr);
  return obs;
}

// ----------------------------------------------------------------------
//  Helpers: name-construction matching what stage 2 wrote.
// ----------------------------------------------------------------------
static TString pairHistName(const TString & ef,
                            const TString & pf1,
                            const TString & pf2,
                            const TString & obsName)
{
  TString s = "PPDerived_";
  s += ef;  s += "_";
  s += pf1; s += "_";
  s += pf2; s += "_";
  s += obsName;
  return s;
}

// ----------------------------------------------------------------------
//  Main: orchestrator
// ----------------------------------------------------------------------
void ParticlePairBfCalculator::execute()
{
  _taskExecuted.increment();

  const String taskName = name();   // "ParticlePairBfCalculator"

  // 1. Filter list from config (subtask-key propagation already mapped
  //    RunBf:Subtask<i>:foo to ParticlePairBfCalculator:foo).
  int nEF = _configuration.valueInt(taskName + ":nEventFilters");
  int nPF = _configuration.valueInt(taskName + ":nParticleFilters");
  if (nEF <= 0 || nPF <= 0)
    {
    printString("ParticlePairBfCalculator: nEF or nPF is 0 — skipping");
    return;
    }
  if ((nPF % 2) != 0)
    {
    printString("ParticlePairBfCalculator: nParticleFilters odd — "
                "BF needs particles+antiparticles in matched pairs.");
    return;
    }
  const int nSpecies = nPF / 2;

  std::vector<TString> efNames(nEF), pfNames(nPF);
  for (int k = 0; k < nEF; ++k)
    {
    String key = taskName; key += ":EventFilterName"; key += k;
    efNames[k] = _configuration.valueString(key);
    }
  for (int k = 0; k < nPF; ++k)
    {
    String key = taskName; key += ":ParticleFilterName"; key += k;
    pfNames[k] = _configuration.valueString(key);
    }

  // 2. I/O paths.
  const char * importDir = std::getenv("CAP_HISTOS_IMPORT_PATH");
  const char * exportDir = std::getenv("CAP_HISTOS_EXPORT_PATH");
  if (!importDir || !exportDir)
    {
    printString("ParticlePairBfCalculator: CAP_HISTOS_*_PATH unset");
    return;
    }
  String pairInPath = String(importDir) + "PairDerivedGen.root";
  String outPath    = String(exportDir) + "PairBFGen.root";

  TFile * fPair = TFile::Open(pairInPath.Data(), "READ");
  if (!fPair || fPair->IsZombie())
    {
    printValue("ParticlePairBfCalculator: cannot open", pairInPath);
    return;
    }

  TFile * fOut = TFile::Open(outPath.Data(), "RECREATE");
  if (!fOut || fOut->IsZombie())
    {
    printValue("ParticlePairBfCalculator: cannot create", outPath);
    fPair->Close();
    return;
    }

  // 3. Pair observables to compute BF for.  These are the histograms
  //    ParticlePairDerivedHistos::create() produces (pt-pt is excluded
  //    because BF is conventionally a function of Δη / Δφ, not pT).
  const std::vector<TString> obsList = {
    "R2_phiphi",
    "R2_etaeta",
    "R2_DetaDphi_shft",
    "R2_DetaDphi",
    "C2_phiphi",
    "C2_etaeta",
  };

  long nWritten = 0;
  long nSkipped = 0;

  // 4. Orchestrator — iterate (event filter × species pair).  Mirrors
  //    BalanceFunctionCalculator.cpp:585-628 (commented-out original).
  fOut->cd();
  for (const TString & pObs : obsList)
    {
    for (int iEF = 0; iEF < nEF; ++iEF)
      {
      const TString & en = efNames[iEF];

      for (int i = 0; i < nSpecies; ++i)
        {
        for (int j = 0; j < nSpecies; ++j)
          {
          const TString & pn1    = pfNames[i];
          const TString & pn1Bar = pfNames[i + nSpecies];
          const TString & pn2    = pfNames[j];
          const TString & pn2Bar = pfNames[j + nSpecies];

          TH2 * obs_1_2       = static_cast<TH2*>(fPair->Get(
              pairHistName(en, pn1,    pn2,    pObs)));
          TH2 * obs_1Bar_2    = static_cast<TH2*>(fPair->Get(
              pairHistName(en, pn1Bar, pn2,    pObs)));
          TH2 * obs_1_2Bar    = static_cast<TH2*>(fPair->Get(
              pairHistName(en, pn1,    pn2Bar, pObs)));
          TH2 * obs_1Bar_2Bar = static_cast<TH2*>(fPair->Get(
              pairHistName(en, pn1Bar, pn2Bar, pObs)));

          if (!obs_1_2 || !obs_1Bar_2 || !obs_1_2Bar || !obs_1Bar_2Bar)
            {
            ++nSkipped;
            continue;
            }

          TString base = "PPBf_"; base += en;
          base += "_"; base += pn1;
          base += "_"; base += pn2;
          base += "_"; base += pObs;

          TH2 * h_CI  = compute_CI(obs_1_2, obs_1Bar_2,
                                   obs_1_2Bar, obs_1Bar_2Bar,
                                   base + "_CI");
          TH2 * h_CD  = compute_CD(obs_1_2, obs_1Bar_2,
                                   obs_1_2Bar, obs_1Bar_2Bar,
                                   base + "_CD");
          // BF combos per Pruneau line 616-617:
          //   B2_1_2Bar  = BalFct(US=obs_1_2Bar,    LS=obs_1Bar_2Bar)
          //   B2_1Bar_2  = BalFct(US=obs_1Bar_2,    LS=obs_1_2)
          TH2 * h_BFa = compute_BalFct(obs_1_2Bar, obs_1Bar_2Bar,
                                       base + "_B2_1_2Bar");
          TH2 * h_BFb = compute_BalFct(obs_1Bar_2, obs_1_2,
                                       base + "_B2_1Bar_2");

          for (TH2 * h : { h_CI, h_CD, h_BFa, h_BFb })
            {
            if (!h) continue;
            h->SetDirectory(fOut);
            h->Write();
            ++nWritten;
            }
          }
        }
      }
    }

  fOut->Write();
  fOut->Close();
  fPair->Close();

  printCR();
  printValue("ParticlePairBfCalculator: wrote BF histograms", nWritten);
  printValue("ParticlePairBfCalculator: skipped (missing input)", nSkipped);
  printValue("ParticlePairBfCalculator: output file", outPath);
}

} // namespace CAP
