/* **********************************************************************
 *  test_phase3e — STANDALONE sanity test for Phase 3e tagger extensions.
 *
 *  THIS FILE IS NOT COMPILED BY CMAKE.  It is a hand-runnable unit test
 *  kept here so future contributors can re-verify the tagger after
 *  changes.  CMakeLists.txt enumerates its sources explicitly and does
 *  not pick this file up; if you add a unit-test framework later, port
 *  the assertions to it and delete this file.
 *
 *  Builds a hand-crafted EventHistory with two MPI scatters, one ISR
 *  and one FSR branch, and a charm D0 -> pi+ cascade, then verifies
 *  that ProvenanceTagger populates the new fields (mpiIndex,
 *  fromISR/FSR, fromCharmChain/fromBottomChain, decayChainDepth) and
 *  that the new pair helpers behave correctly.
 *
 *  Compile (from src/EventHistory):
 *    g++ -std=c++14 -Wall -Wextra -I. \
 *        EventHistory.cpp ProvenanceTagger.cpp \
 *        ProvenanceObservables.cpp PairProvenanceObservables.cpp \
 *        test_phase3e.cpp -o /tmp/test_phase3e
 *    /tmp/test_phase3e
 * ********************************************************************/
#include "EventHistory.hpp"
#include "ProvenanceTagger.hpp"
#include "PairProvenanceObservables.hpp"

#include <cassert>
#include <cstdio>

using namespace CAP;

static int addNode(EventHistory & h, int pdg, Stage stage, bool isFinal)
{
  ParticleNode n;
  n.pdg     = pdg;
  n.stage   = stage;
  n.isFinal = isFinal;
  // Give every node a non-trivial momentum so eta/phi don't blow up.
  n.px = 0.3; n.py = 0.4; n.pz = 0.5; n.e = 1.0;
  return h.addNode(n);
}

int main()
{
  EventHistory h;

  // ---- two MPI scatters; one ISR branch; one FSR branch ----
  //                 hard parton (u)
  //                       |
  //                  ISR parton (g)
  //                       |
  //              parton-pre-hadronization (u)
  //                       |
  //                  primary pi+
  // and a second MPI vertex producing a separate parton -> pi-.
  const int hard1 = addNode(h, 2,  Stage::HardProcess,             false);
  const int isr1  = addNode(h, 21, Stage::ISR,                     false);
  const int phPre1= addNode(h, 2,  Stage::PartonsPreHadronization, false);
  const int piA   = addNode(h, 211,Stage::PrimaryHadrons,          true);
  h.link(hard1, isr1);
  h.link(isr1,  phPre1);
  h.link(phPre1, piA);

  const int mpi2  = addNode(h, -1, Stage::MPI,                     false);
  const int fsr2  = addNode(h, 21, Stage::FSR,                     false);
  const int phPre2= addNode(h, -1, Stage::PartonsPreHadronization, false);
  const int piB   = addNode(h, -211,Stage::PrimaryHadrons,         true);
  h.link(mpi2,  fsr2);
  h.link(fsr2,  phPre2);
  h.link(phPre2, piB);

  // ---- charm cascade: D0 -> K- pi+, the pi+ is a final hadron ----
  // c quark (hard) -> c quark (pre-had) -> D0 (primary) -> pi+ (decay)
  const int cHard = addNode(h, 4,   Stage::HardProcess,             false);
  const int cPre  = addNode(h, 4,   Stage::PartonsPreHadronization, false);
  const int dZero = addNode(h, 421, Stage::PrimaryHadrons,          false);
  const int piC   = addNode(h, 211, Stage::DecayProducts,           true);
  h.link(cHard, cPre);
  h.link(cPre,  dZero);
  h.link(dZero, piC);

  // ---- ProvenanceTagger ----
  ProvenanceTagger tagger;
  ProvenanceTag tA = tagger.tag(h, piA);
  ProvenanceTag tB = tagger.tag(h, piB);
  ProvenanceTag tC = tagger.tag(h, piC);

  // piA: hard1 ancestor, ISR but not FSR, no MPI, depth 0 (primary), no HF.
  std::printf("piA: hardIdx=%d mpiIdx=%d ISR=%d FSR=%d HFc=%d HFb=%d depth=%d\n",
              tA.hardPartonIndex, tA.mpiIndex,
              tA.fromISR, tA.fromFSR,
              tA.fromCharmChain, tA.fromBottomChain,
              tA.decayChainDepth);
  assert(tA.hardPartonIndex == hard1);
  assert(tA.mpiIndex        == -1);
  assert(tA.fromISR         == true);
  assert(tA.fromFSR         == false);
  assert(tA.fromCharmChain  == false);
  assert(tA.fromBottomChain == false);
  assert(tA.decayChainDepth == 0);                // primary

  // piB: no hard ancestor, MPI ancestor at mpi2, FSR yes, ISR no.
  std::printf("piB: hardIdx=%d mpiIdx=%d ISR=%d FSR=%d HFc=%d HFb=%d depth=%d\n",
              tB.hardPartonIndex, tB.mpiIndex,
              tB.fromISR, tB.fromFSR,
              tB.fromCharmChain, tB.fromBottomChain,
              tB.decayChainDepth);
  assert(tB.hardPartonIndex == -1);
  assert(tB.mpiIndex        == mpi2);
  assert(tB.fromISR         == false);
  assert(tB.fromFSR         == true);
  assert(tB.decayChainDepth == 0);                // primary

  // piC: hard ancestor cHard, charm chain through D0, depth 1.
  std::printf("piC: hardIdx=%d mpiIdx=%d ISR=%d FSR=%d HFc=%d HFb=%d depth=%d\n",
              tC.hardPartonIndex, tC.mpiIndex,
              tC.fromISR, tC.fromFSR,
              tC.fromCharmChain, tC.fromBottomChain,
              tC.decayChainDepth);
  assert(tC.hardPartonIndex == cHard);
  assert(tC.fromCharmChain  == true);
  assert(tC.fromBottomChain == false);
  assert(tC.decayChainDepth == 1);                // pi+ <- D0 (one step)

  // ---- pair helpers ----
  // piA / piC both have hard ancestors but DIFFERENT hard partons.
  assert(!sharesHardProcessAncestor(tA, tC));
  // Neither piA nor piC has an MPI ancestor.
  assert(!sharesMPIVertex(tA, tC));
  // piB has an MPI ancestor and piA doesn't.
  assert(!sharesMPIVertex(tA, tB));
  // piB shared with itself.
  assert(sharesMPIVertex(tB, tB));

  // ---- PairProvenanceObservables — make sure the new histograms exist
  PairProvenanceObservables obs(211);   // pi+/-
  std::vector<ProvenanceTag> tags = { tA, tB, tC };
  obs.accumulate(h, tags);

  const std::map<std::string, Hist1D> & hist = obs.histograms();
  // We expect at least these new keys to have been created:
  const char * expected[] = {
    "dphi_pair_MPI_NoMPI", "dphi_pair_MPI_OneSideMPI",
    "dphi_pair_Shower_MixedShower",
    "dphi_pair_HF_OneCharm", "dphi_pair_HF_NoHeavyFlavour",
    "pair_decay_depth_max",
    "event_multiplicity",
  };
  for (const char * k : expected)
    {
    if (hist.find(k) == hist.end())
      {
      std::printf("MISSING histogram key: %s\n", k);
      return 1;
      }
    }

  std::printf("\nALL assertions passed.\n");
  std::printf("Pair count = %ld (expected 3 unordered pairs)\n", obs.pairs());
  assert(obs.pairs() == 3);
  return 0;
}
