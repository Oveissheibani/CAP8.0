/* **********************************************************************
 * CAP::FragmentationSystems — implementation.  See header for physics.
 * ********************************************************************/
#include "FragmentationSystems.hpp"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <set>
#include <sstream>

namespace CAP
{

namespace
{
// Lambda-measure infrared scale (GeV): systems lighter than m0 contribute 0.
constexpr double LAMBDA_M0 = 1.0;

bool pdgIsBoundary(int pdg)
{
  const int a = std::abs(pdg);
  return a == 81 || a == 91 || a == 92;        // Herwig cluster & friends
}

// Disjoint-set (union-find) with path halving.
struct DSU
{
  std::vector<int> p;
  explicit DSU(int n) : p(static_cast<std::size_t>(n)) {
    for (int i = 0; i < n; ++i) p[static_cast<std::size_t>(i)] = i;
  }
  int find(int x) {
    while (p[static_cast<std::size_t>(x)] != x) {
      p[static_cast<std::size_t>(x)] =
        p[static_cast<std::size_t>(p[static_cast<std::size_t>(x)])];
      x = p[static_cast<std::size_t>(x)];
    }
    return x;
  }
  void unite(int a, int b) {
    a = find(a); b = find(b);
    if (a != b) p[static_cast<std::size_t>(b)] = a;
  }
};
} // namespace

// ---------------------------------------------------------------------------
//  classifiers
// ---------------------------------------------------------------------------
int FragmentationSystems::chargeSign(int pdg)
{
  const int a = std::abs(pdg);
  const int s = pdg > 0 ? 1 : -1;
  switch (a)
    {
    case 211: case 321: case 2212: case 3222:            // pi+ K+ p Sigma+
      return s;
    case 11: case 13: case 15:                           // e- mu- tau-
    case 3112: case 3312: case 3334:                     // Sigma- Xi- Omega-
      return -s;
    case 213: case 323: case 411: case 431: case 521:    // rho+ K*+ D+ Ds+ B+
    case 2214: case 2224: case 3224: case 3114: case 3314:
      return (a == 1114 || a == 3114 || a == 3314) ? -s : s;
    case 1114:                                           // Delta-
      return -s;
    default:
      return 0;                                          // neutral / unknown
    }
}

bool FragmentationSystems::isBaryon(int pdg)
{
  const int a = std::abs(pdg);
  return a >= 1000 && a < 10000 && ((a / 1000) % 10) > 0;
}

bool FragmentationSystems::hasStrangeQuark(int pdg)
{
  const int a = std::abs(pdg);
  if (a < 100 || a >= 10000) return false;
  return ((a / 10)   % 10) == 3 ||
         ((a / 100)  % 10) == 3 ||
         ((a / 1000) % 10) == 3;
}

double FragmentationSystems::rapidity(const ParticleNode & n)
{
  const double e = n.e, pz = n.pz;
  if (e <= 0.0)        return 0.0;
  if (e - pz <= 1e-12) return  20.0;
  if (e + pz <= 1e-12) return -20.0;
  return 0.5 * std::log((e + pz) / (e - pz));
}

// ---------------------------------------------------------------------------
//  system keys: nearest parton / boundary ancestors of one primary hadron
// ---------------------------------------------------------------------------
std::vector<int>
FragmentationSystems::systemKeys(const EventHistory & h, int idx) const
{
  std::vector<int> keys;
  std::set<int>    seen;
  std::vector<int> frontier = h.node(idx).parents;
  int depth = 0;
  while (!frontier.empty() && depth < 6)
    {
    std::vector<int> next;
    for (int p : frontier)
      {
      if (p < 0 || p >= h.size() || seen.count(p)) continue;
      seen.insert(p);
      const ParticleNode & pn = h.node(p);
      if (pn.isParton() || pdgIsBoundary(pn.pdg))
        keys.push_back(p);                       // stop this branch here
      else
        next.insert(next.end(), pn.parents.begin(), pn.parents.end());
      }
    frontier.swap(next);
    ++depth;
    }
  return keys;
}

// ---------------------------------------------------------------------------
//  histogram axes (lazily created by name)
// ---------------------------------------------------------------------------
Hist1D & FragmentationSystems::H(const std::string & name)
{
  std::map<std::string,Hist1D>::iterator it = _hist.find(name);
  if (it != _hist.end()) return it->second;

  int nb; double lo, hi;
  if      (name == "frag_nsystems")           { nb = 81;  lo = -0.5; hi = 80.5; }
  else if (name == "frag_nhad_per_system")    { nb = 61;  lo = -0.5; hi = 60.5; }
  else if (name == "frag_system_mass")        { nb = 120; lo = 0.0;  hi = 60.0; }
  else if (name == "frag_system_mass_zoom")   { nb = 60;  lo = 0.0;  hi = 6.0;  }
  else if (name == "frag_nhad_vs_mass_sum" ||
           name == "frag_nhad_vs_mass_n")     { nb = 60;  lo = 0.0;  hi = 60.0; }
  else if (name == "frag_y_span")             { nb = 80;  lo = 0.0;  hi = 16.0; }
  else if (name == "frag_lambda")             { nb = 100; lo = 0.0;  hi = 100.0;}
  else if (name == "frag_neighbor_ptbal")     { nb = 40;  lo = -1.0; hi = 1.0;  }
  else if (name.rfind("frag_neighbor_dy", 0) == 0)
                                              { nb = 60;  lo = 0.0;  hi = 6.0;  }
  else if (name.rfind("frag_pair_dy", 0) == 0 ||
           name.rfind("frag_bbar_dy", 0) == 0 ||
           name.rfind("frag_strange_dy", 0) == 0)
                                              { nb = 40;  lo = 0.0;  hi = 8.0;  }
  else if (name.rfind("frag_cluster_mass", 0) == 0)
                                              { nb = 60;  lo = 0.0;  hi = 12.0; }
  else                                        { nb = 100; lo = 0.0;  hi = 100.0;}

  _hist[name] = Hist1D(name, name, nb, lo, hi);
  return _hist[name];
}

// ---------------------------------------------------------------------------
//  accumulation
// ---------------------------------------------------------------------------
void FragmentationSystems::accumulate(const EventHistory & history)
{
  ++_events;

  // ---- collect primaries and build the systems (union-find) ------------
  const std::vector<int> prim = history.collectStage(Stage::PrimaryHadrons);
  const int nP = static_cast<int>(prim.size());
  if (nP == 0) { H("frag_nsystems").fill(0.0); return; }

  DSU dsu(nP);
  std::map<int,int> keyOwner;                 // ancestor node -> primary slot
  for (int i = 0; i < nP; ++i)
    {
    const std::vector<int> keys = systemKeys(history, prim[static_cast<std::size_t>(i)]);
    for (int k : keys)
      {
      std::map<int,int>::iterator it = keyOwner.find(k);
      if (it == keyOwner.end()) keyOwner[k] = i;
      else                      dsu.unite(it->second, i);
      }
    }
  // group primaries by system root
  std::map<int,std::vector<int>> systems;     // root -> primary slots
  for (int i = 0; i < nP; ++i) systems[dsu.find(i)].push_back(i);

  H("frag_nsystems").fill(static_cast<double>(systems.size()));

  // ---- per-system observables -------------------------------------------
  double lambdaTot = 0.0;
  // (system id, rapidity, charge sign, pdg) of every primary, for the
  // same- vs cross-system pair loops below.
  struct PInfo { int sys; double y; int q; int pdg; double px, py; };
  std::vector<PInfo> info;
  info.reserve(static_cast<std::size_t>(nP));

  int sysId = 0;
  for (const auto & kv : systems)
    {
    const std::vector<int> & members = kv.second;
    double px = 0, py = 0, pz = 0, e = 0;
    double yMin = 1e9, yMax = -1e9;
    for (int slot : members)
      {
      const ParticleNode & n =
        history.node(prim[static_cast<std::size_t>(slot)]);
      px += n.px; py += n.py; pz += n.pz; e += n.e;
      const double y = rapidity(n);
      yMin = std::min(yMin, y); yMax = std::max(yMax, y);
      PInfo pi; pi.sys = sysId; pi.y = y;
      pi.q = chargeSign(n.pdg); pi.pdg = n.pdg;
      pi.px = n.px; pi.py = n.py;
      info.push_back(pi);
      }
    const double m2 = e*e - px*px - py*py - pz*pz;
    const double m  = m2 > 0.0 ? std::sqrt(m2) : 0.0;
    const double nH = static_cast<double>(members.size());

    H("frag_nhad_per_system").fill(nH);
    H("frag_system_mass").fill(m);
    H("frag_system_mass_zoom").fill(m);
    H("frag_nhad_vs_mass_sum").fill(m, nH);
    H("frag_nhad_vs_mass_n").fill(m);
    if (members.size() > 1) H("frag_y_span").fill(yMax - yMin);
    if (m > LAMBDA_M0) lambdaTot += std::log((m * m) / (LAMBDA_M0 * LAMBDA_M0));

    // -- within-system ordering: sort members by rapidity ----------------
    std::vector<std::pair<double,std::size_t>> ordered;
    for (std::size_t j = info.size() - members.size(); j < info.size(); ++j)
      ordered.push_back(std::make_pair(info[j].y, j));
    std::sort(ordered.begin(), ordered.end());
    for (std::size_t j = 0; j + 1 < ordered.size(); ++j)
      {
      const PInfo & a = info[ordered[j].second];
      const PInfo & b = info[ordered[j + 1].second];
      const double dy = std::fabs(b.y - a.y);
      if (a.q != 0 && b.q != 0)
        H(a.q == b.q ? "frag_neighbor_dy_SS"
                     : "frag_neighbor_dy_OS").fill(dy);
      // local pT compensation: cos of the azimuthal opening angle
      const double pa = std::sqrt(a.px*a.px + a.py*a.py);
      const double pb = std::sqrt(b.px*b.px + b.py*b.py);
      if (pa > 1e-9 && pb > 1e-9)
        H("frag_neighbor_ptbal").fill(
          (a.px*b.px + a.py*b.py) / (pa * pb));
      }
    ++sysId;
    }
  H("frag_lambda").fill(lambdaTot);

  // ---- same- vs cross-system pair loops ---------------------------------
  for (std::size_t i = 0; i < info.size(); ++i)
    for (std::size_t j = i + 1; j < info.size(); ++j)
      {
      const PInfo & a = info[i];
      const PInfo & b = info[j];
      const bool same = (a.sys == b.sys);
      const double dy = std::fabs(a.y - b.y);
      if (a.q != 0 && b.q != 0)
        {
        if (a.q == b.q) H(same ? "frag_pair_dy_SS_same"
                               : "frag_pair_dy_SS_cross").fill(dy);
        else            H(same ? "frag_pair_dy_OS_same"
                               : "frag_pair_dy_OS_cross").fill(dy);
        }
      // baryon - antibaryon pairing (opposite baryon number)
      if (isBaryon(a.pdg) && isBaryon(b.pdg) &&
          ((a.pdg > 0) != (b.pdg > 0)))
        H(same ? "frag_bbar_dy_same" : "frag_bbar_dy_cross").fill(dy);
      // strangeness locality (both carry a strange valence quark)
      if (hasStrangeQuark(a.pdg) && hasStrangeQuark(b.pdg))
        H(same ? "frag_strange_dy_same" : "frag_strange_dy_cross").fill(dy);
      }

  // ---- Herwig-only: explicit cluster (PDG 81) fission chain --------------
  for (int i = 0; i < history.size(); ++i)
    {
    const ParticleNode & n = history.node(i);
    if (std::abs(n.pdg) != 81) continue;
    const double m = n.mass();
    H("frag_cluster_mass_all").fill(m);
    bool topCluster = true;
    for (int p : n.parents)
      if (p >= 0 && p < history.size() &&
          std::abs(history.node(p).pdg) == 81) { topCluster = false; break; }
    if (topCluster) H("frag_cluster_mass_top").fill(m);
    bool decaying = false;
    for (int c : n.children)
      if (c >= 0 && c < history.size() &&
          std::abs(history.node(c).pdg) >= 100) { decaying = true; break; }
    if (decaying) H("frag_cluster_mass_decaying").fill(m);
    }
}

// ---------------------------------------------------------------------------
//  text summary — sections carry the unique "fragmentation:" prefix
// ---------------------------------------------------------------------------
std::string FragmentationSystems::report() const
{
  std::ostringstream os;
  os << std::fixed << std::setprecision(4);
  os << "FragmentationSystems — " << _events << " event(s)\n";

  auto meanOf = [this](const std::string & name) -> double
    {
    std::map<std::string,Hist1D>::const_iterator it = _hist.find(name);
    if (it == _hist.end()) return 0.0;
    const Hist1D & h = it->second;
    double sum = 0.0, n = 0.0;
    for (int b = 0; b < h.nbins; ++b)
      {
      const double c = h.counts[static_cast<std::size_t>(b)];
      sum += c * h.binCenter(b); n += c;
      }
    return n > 0.0 ? sum / n : 0.0;
    };
  auto integralOf = [this](const std::string & name) -> double
    {
    std::map<std::string,Hist1D>::const_iterator it = _hist.find(name);
    return it == _hist.end() ? 0.0
                             : it->second.integral() + it->second.overflow;
    };

  os << "  fragmentation: systems summary:\n";
  os << "    systems_per_event      " << meanOf("frag_nsystems")        << "\n";
  os << "    hadrons_per_system     " << meanOf("frag_nhad_per_system") << "\n";
  os << "    mean_system_mass       " << meanOf("frag_system_mass")     << "\n";
  os << "    mean_y_span            " << meanOf("frag_y_span")          << "\n";
  os << "    lambda_per_event       " << meanOf("frag_lambda")          << "\n";

  // Charge-ordering fractions: OS / (OS+SS), same- vs cross-system.  The
  // string prediction is OS_same > OS_cross (local charge conservation).
  const double nbOS = integralOf("frag_neighbor_dy_OS");
  const double nbSS = integralOf("frag_neighbor_dy_SS");
  const double sOS  = integralOf("frag_pair_dy_OS_same");
  const double sSS  = integralOf("frag_pair_dy_SS_same");
  const double cOS  = integralOf("frag_pair_dy_OS_cross");
  const double cSS  = integralOf("frag_pair_dy_SS_cross");
  os << "  fragmentation: ordering summary:\n";
  if (nbOS + nbSS > 0.0)
    os << "    OS_fraction_neighbors  " << nbOS / (nbOS + nbSS) << "\n";
  if (sOS + sSS > 0.0)
    os << "    OS_fraction_same       " << sOS / (sOS + sSS)   << "\n";
  if (cOS + cSS > 0.0)
    os << "    OS_fraction_cross      " << cOS / (cOS + cSS)   << "\n";
  os << "    neighbor_ptbal_mean    " << meanOf("frag_neighbor_ptbal") << "\n";
  const double nClus = integralOf("frag_cluster_mass_all");
  if (nClus > 0.0)
    {
    os << "  fragmentation: cluster summary:\n";
    os << "    clusters_per_event     " << nClus / (_events > 0 ? _events : 1)
       << "\n";
    os << "    mean_cluster_mass      " << meanOf("frag_cluster_mass_all")
       << "\n";
    }
  return os.str();
}

} // namespace CAP
