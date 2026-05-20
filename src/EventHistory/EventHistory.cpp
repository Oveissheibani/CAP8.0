/* **********************************************************************
 * CAP::EventHistory — implementation.  See EventHistory.hpp for design.
 * ********************************************************************/
#include "EventHistory.hpp"

#include <algorithm>
#include <sstream>

namespace CAP
{

// ----------------------------------------------------------------------
int EventHistory::addNode(const ParticleNode & n)
{
  _nodes.push_back(n);
  return static_cast<int>(_nodes.size()) - 1;
}

// ----------------------------------------------------------------------
void EventHistory::link(int parentIndex, int childIndex)
{
  const int n = size();
  if (parentIndex < 0 || childIndex < 0)   return;
  if (parentIndex >= n || childIndex >= n) return;
  if (parentIndex == childIndex)           return;

  std::vector<int> & kids = _nodes[parentIndex].children;
  if (std::find(kids.begin(), kids.end(), childIndex) == kids.end())
    kids.push_back(childIndex);

  std::vector<int> & dads = _nodes[childIndex].parents;
  if (std::find(dads.begin(), dads.end(), parentIndex) == dads.end())
    dads.push_back(parentIndex);
}

// ----------------------------------------------------------------------
std::vector<int> EventHistory::collectStage(Stage s) const
{
  std::vector<int> out;
  for (int i = 0; i < size(); ++i)
    if (_nodes[i].stage == s) out.push_back(i);
  return out;
}

// ----------------------------------------------------------------------
std::vector<int> EventHistory::finalState() const
{
  std::vector<int> out;
  for (int i = 0; i < size(); ++i)
    if (_nodes[i].isFinal) out.push_back(i);
  return out;
}

// ----------------------------------------------------------------------
//  Breadth-first walk up the parent links.  A branch stops as soon as
//  it reaches a node at the requested stage; the `seen` vector guards
//  against cycles (a malformed record should never hang the analysis).
// ----------------------------------------------------------------------
std::vector<int> EventHistory::ancestorsAtStage(int i, Stage s) const
{
  std::vector<int> out;
  const int n = size();
  if (i < 0 || i >= n) return out;

  std::vector<char> seen(static_cast<size_t>(n), 0);
  seen[static_cast<size_t>(i)] = 1;

  std::vector<int> frontier = _nodes[i].parents;
  while (!frontier.empty())
    {
    std::vector<int> next;
    for (int p : frontier)
      {
      if (p < 0 || p >= n)            continue;
      if (seen[static_cast<size_t>(p)]) continue;
      seen[static_cast<size_t>(p)] = 1;

      if (_nodes[p].stage == s)
        out.push_back(p);
      else
        for (int gp : _nodes[p].parents) next.push_back(gp);
      }
    frontier.swap(next);
    }
  return out;
}

// ----------------------------------------------------------------------
Stage EventHistory::deepestStage() const
{
  Stage deepest = Stage::Unknown;
  bool  any     = false;
  for (const ParticleNode & node : _nodes)
    {
    if (node.stage == Stage::Unknown) continue;
    if (!any || stageOrder(node.stage) < stageOrder(deepest))
      {
      deepest = node.stage;
      any     = true;
      }
    }
  return deepest;
}

// ----------------------------------------------------------------------
HistoryCapability EventHistory::capability() const
{
  HistoryCapability cap;
  cap.deepestStage = deepestStage();
  for (const ParticleNode & node : _nodes)
    {
    if (isPartonic(node.stage))             cap.hasPartons        = true;
    if (node.stage == Stage::PrimaryHadrons) cap.hasPrimaryHadrons = true;
    if (node.stage == Stage::DecayProducts)  cap.hasDecayChain     = true;
    if (!node.parents.empty() || !node.children.empty())
      cap.hasGraphLinks = true;
    }
  return cap;
}

// ----------------------------------------------------------------------
std::string HistoryCapability::describe() const
{
  std::ostringstream os;
  os << "deepest stage = "  << stageName(deepestStage)
     << "; partons = "        << (hasPartons        ? "yes" : "no")
     << "; primaryHadrons = " << (hasPrimaryHadrons ? "yes" : "no")
     << "; decayChain = "     << (hasDecayChain     ? "yes" : "no")
     << "; graphLinks = "     << (hasGraphLinks     ? "yes" : "no");
  return os.str();
}

} // namespace CAP
