/* **********************************************************************
 * CAP::StageSelector  —  resolve "which particles?" against a history
 *
 * Part of the parton-tracking feature (Phase 2).
 *
 * A StageRequest is what an `analysis_stage` configuration knob
 * resolves to.  A StageSelector then turns that request, together with
 * a filled EventHistory, into the concrete list of particles an
 * analysis should run over.
 *
 * Three selection modes:
 *   - FinalState          the status==1 particles a detector records
 *                         (the default — byte-identical to today).
 *   - Snapshot            every particle present at one chosen stage,
 *                         e.g. all primary hadrons, or all partons
 *                         entering hadronization.
 *   - AncestryProjection  walk every final hadron back up the DAG and
 *                         collect its ancestors at one chosen stage —
 *                         the basis for "correlations of the partons
 *                         that the final hadrons came from".
 *
 * Pure C++14, depends only on EventHistory.  No ROOT, no generators.
 * ********************************************************************/
#ifndef CAP__StageSelector
#define CAP__StageSelector

#include <vector>
#include <string>

#include "EventHistory.hpp"
#include "StageTaxonomy.hpp"

namespace CAP
{

enum class SelectionMode
{
  FinalState,          // status==1 particles (default)
  Snapshot,            // every particle at one chosen stage
  AncestryProjection   // final hadrons' ancestors at one chosen stage
};

// A complete "which particles?" request.
struct StageRequest
{
  SelectionMode mode  = SelectionMode::FinalState;
  Stage         stage = Stage::FinalState;   // Snapshot: the target stage
                                             // Projection: the ancestor stage

  std::string describe() const;

  // Parse a configuration string (case-insensitive).  Accepted forms:
  //   "FinalState"                  -> FinalState
  //   "PrimaryHadrons"              -> Snapshot at PrimaryHadrons
  //   "PartonsPreHadronization"     -> Snapshot at that stage
  //   "snapshot:<stage>"            -> Snapshot at <stage>
  //   "ancestry:<stage>"            -> AncestryProjection to <stage>
  // Anything unrecognised falls back to FinalState (the safe default).
  static StageRequest fromString(const std::string & spec);
};

// Resolves a StageRequest against a filled EventHistory.
class StageSelector
{
public:
  // Indices of the nodes in `history` selected by `request`.
  std::vector<int> select(const EventHistory & history,
                          const StageRequest & request) const;
};

} // namespace CAP

#endif // CAP__StageSelector
