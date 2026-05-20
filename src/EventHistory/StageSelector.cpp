/* **********************************************************************
 * CAP::StageSelector — implementation.  See header for design.
 * ********************************************************************/
#include "StageSelector.hpp"
#include "StageStatusCodes.hpp"   // stageFromName()

#include <algorithm>
#include <cctype>

namespace CAP
{

// ----------------------------------------------------------------------
std::string StageRequest::describe() const
{
  std::string m;
  switch (mode)
    {
    case SelectionMode::FinalState:         m = "FinalState";         break;
    case SelectionMode::Snapshot:           m = "Snapshot";           break;
    case SelectionMode::AncestryProjection: m = "AncestryProjection"; break;
    }
  return m + " @ " + stageName(stage);
}

// ----------------------------------------------------------------------
StageRequest StageRequest::fromString(const std::string & spec)
{
  StageRequest req;   // defaults to FinalState

  // Split on the first ':' into a head keyword and an optional stage.
  std::string head = spec, tail;
  const std::string::size_type colon = spec.find(':');
  if (colon != std::string::npos)
    {
    head = spec.substr(0, colon);
    tail = spec.substr(colon + 1);
    }

  // Lower-case the head for keyword matching.
  std::string h;
  for (char c : head)
    h.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(c))));

  if (h == "snapshot")
    {
    req.mode  = SelectionMode::Snapshot;
    req.stage = stageFromName(tail);
    }
  else if (h == "ancestry" || h == "projection")
    {
    req.mode  = SelectionMode::AncestryProjection;
    req.stage = stageFromName(tail);
    }
  else
    {
    // A bare stage name: FinalState stays FinalState; any other known
    // stage becomes a Snapshot at that stage.
    const Stage s = stageFromName(spec);
    if (s == Stage::FinalState || s == Stage::Unknown)
      {
      req.mode  = SelectionMode::FinalState;
      req.stage = Stage::FinalState;
      }
    else
      {
      req.mode  = SelectionMode::Snapshot;
      req.stage = s;
      }
    }
  return req;
}

// ----------------------------------------------------------------------
std::vector<int> StageSelector::select(const EventHistory & history,
                                       const StageRequest & request) const
{
  switch (request.mode)
    {
    case SelectionMode::FinalState:
      return history.finalState();

    case SelectionMode::Snapshot:
      return history.collectStage(request.stage);

    case SelectionMode::AncestryProjection:
      {
      std::vector<int> out;
      const std::vector<int> finals = history.finalState();
      for (int f : finals)
        {
        const std::vector<int> ancestors =
          history.ancestorsAtStage(f, request.stage);
        for (int a : ancestors)
          if (std::find(out.begin(), out.end(), a) == out.end())
            out.push_back(a);
        }
      return out;
      }
    }
  // Unreachable — every SelectionMode is handled above.
  return history.finalState();
}

} // namespace CAP
