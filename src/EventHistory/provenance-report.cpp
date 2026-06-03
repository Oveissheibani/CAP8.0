/* **********************************************************************
 *  provenance-report  —  report-builder for the parton-tracking feature
 *
 *  Assembles a complete report of a provenance-study run using CAP's own
 *  src/Latex module (CAP::LatexDocument) — its first real consumer.
 *
 *  Two modes:
 *    --mode paper          an 'article' document (sections, tables,
 *                          figures, abstract)
 *    --mode presentation   a 'beamer' slide deck (one frame per topic)
 *
 *  It captures everything from configuration through to the final plots:
 *    - run configuration   (events, sqrt(s), process, seed, species),
 *      parsed from the provenance-study .root.txt summary
 *    - analysis binning     (documented axis ranges)
 *    - result tables        (origin / parton / pair decompositions)
 *    - the mechanism-ladder comparison table (from ladder-comparison.csv)
 *    - every figure listed in cap-provenance-plot's figures.manifest
 *
 *  Inputs are plain text — no ROOT file is opened — so the builder is a
 *  pure document assembler.  Build: bin/provenance-report.
 *
 *  Usage:
 *    provenance-report --summary pions.root.txt --figures provenance-plots \
 *                      --ladder ladder-runs/ladder-comparison.csv --mode paper
 * ********************************************************************/
#include "LatexDocument.hpp"

#include <algorithm>    // std::min / std::max for species-pair ordering
#include <cctype>
#include <array>
#include <cstdio>       // popen for git/hostname capture
#include <cstdlib>
#include <functional>   // std::function predicates for renderFiguresPaired
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include <unistd.h>     // getcwd, for absolute figure paths

using namespace CAP;

namespace
{

// ---- small helpers --------------------------------------------------------
String L(const std::string & s) { return String(s.c_str()); }   // std -> TString

std::string trim(const std::string & s)
{
  std::string::size_type a = s.find_first_not_of(" \t\r\n");
  if (a == std::string::npos) return "";
  std::string::size_type b = s.find_last_not_of(" \t\r\n");
  return s.substr(a, b - a + 1);
}

// Escape LaTeX-special characters in text taken from external sources
// (figure captions etc.), so a stray '#', '_' or '%' cannot break the
// LaTeX compile.  Our own hard-coded prose / math is not passed through.
std::string texEscape(const std::string & s)
{
  std::string o;
  for (char c : s)
    {
    switch (c)
      {
      case '#': case '$': case '%': case '&':
      case '_': case '{': case '}':
        o += '\\'; o += c;                 break;
      case '~':  o += "\\textasciitilde{}"; break;
      case '^':  o += "\\textasciicircum{}";break;
      case '\\': o += "\\textbackslash{}";  break;
      default:   o += c;                    break;
      }
    }
  return o;
}

// Resolve a possibly-relative path to absolute, so figure paths in the
// generated .tex work no matter what directory pdflatex is invoked in.
std::string absPath(const std::string & p)
{
  if (!p.empty() && p[0] == '/') return p;
  char buf[4096];
  if (::getcwd(buf, sizeof(buf))) return std::string(buf) + "/" + p;
  return p;
}

std::vector<std::string> splitCSV(const std::string & line)
{
  std::vector<std::string> out;
  std::string cell;
  std::istringstream is(line);
  while (std::getline(is, cell, ',')) out.push_back(trim(cell));
  return out;
}

// ---- file slurping & process capture --------------------------------------

// Read an entire file as a string; empty on failure.  Used to embed
// Pythia .cmnd configurations into the reproducibility appendix.
std::string slurp(const std::string & path)
{
  std::ifstream f(path.c_str());
  if (!f) return "";
  std::ostringstream os;
  os << f.rdbuf();
  return os.str();
}

// Capture a single line of stdout from a shell command.  Used to query
// `git rev-parse HEAD`, `hostname`, `date`, etc.  Returns empty on
// failure.  We deliberately keep the implementation tiny — popen is
// adequate for the few short queries we make.
std::string captureCommand(const std::string & cmd)
{
  std::string out;
  FILE * fp = ::popen(cmd.c_str(), "r");
  if (!fp) return out;
  char buf[1024];
  while (std::fgets(buf, sizeof(buf), fp)) out.append(buf);
  ::pclose(fp);
  return trim(out);
}

// Strip an absolute path down to its basename (no directories).
std::string basename(const std::string & p)
{
  std::string::size_type s = p.find_last_of('/');
  return s == std::string::npos ? p : p.substr(s + 1);
}

// Strip the file extension off a basename.
std::string stem(const std::string & p)
{
  std::string b = basename(p);
  std::string::size_type d = b.find_last_of('.');
  return d == std::string::npos ? b : b.substr(0, d);
}

// ---- parsed report data ---------------------------------------------------
struct ClassRow { std::string name, count, pct; };

// One mechanism-ladder rung's .cmnd file, slurped for the appendix.
struct RungConfig
{
  std::string name;     // rung label, e.g. "shower+MPI"
  std::string path;     // absolute path to the .cmnd file (for diagnostics)
  std::string content;  // verbatim contents
};

struct ReportData
{
  std::string events, ecm, process, seed, species;
  std::vector<int> speciesList;   // parsed from species_list=; >1 => multi-species
  std::string studiedHadrons, samePairs;   // headline counts (long strings)
  std::vector<ClassRow> origin, parton, pairs;
  std::vector<ClassRow> initparton;   // by INITIATING (hard-scatter) parton
  std::vector<std::string>              ladderRungs;   // column headers
  std::vector<std::vector<std::string>> ladderRows;    // group,obs,vals...
  std::vector<std::pair<std::string,std::string>> figures;  // (path, caption)

  // ---- reproducibility envelope (new) -----------------------------------
  std::string pythiaCmnd;                     // verbatim pythia.cmnd content
  std::string pythiaCmndPath;                 // its path (for the appendix)
  std::vector<RungConfig> rungConfigs;        // one per ladder rung
  std::string gitSha, gitBranch, hostname, reportDate;
  std::string rootVersion, buildHost;         // optional, free-form
};

// Parse a provenance-study .root.txt summary.
void parseSummary(const std::string & path, ReportData & d)
{
  std::ifstream f(path.c_str());
  if (!f) { std::cerr << "  ! cannot open summary: " << path << "\n"; return; }
  std::string line, section;
  while (std::getline(f, line))
    {
    // configuration line
    if (line.find("events=") != std::string::npos &&
        line.find("ecm=")    != std::string::npos)
      {
      std::istringstream is(line);
      std::string tok;
      while (is >> tok)
        {
        std::string::size_type eq = tok.find('=');
        if (eq == std::string::npos) continue;
        std::string k = tok.substr(0, eq), v = tok.substr(eq + 1);
        if      (k == "events")  d.events  = v;
        else if (k == "ecm")     d.ecm     = v;
        else if (k == "process") d.process = v;
        else if (k == "seed")    d.seed    = v;
        else if (k == "species_list")
          {
          // Comma-separated |pdg| list, e.g. "211,321,2212".  More than one
          // entry switches the report into multi-species layout.
          std::istringstream ls(v);
          std::string cell;
          while (std::getline(ls, cell, ','))
            {
            cell = trim(cell);
            if (cell.empty()) continue;
            try { d.speciesList.push_back(std::stoi(cell)); }
            catch (...) { }
            }
          }
        }
      continue;
      }
    // species, taken from the first "species |pdg| = N" occurrence
    std::string::size_type sp = line.find("species |pdg| =");
    if (sp != std::string::npos && d.species.empty())
      {
      std::string num;
      for (char c : line.substr(sp))
        if (std::isdigit(static_cast<unsigned char>(c))) num += c;
      if (!num.empty()) d.species = num;
      }

    // Headline counts from the single-particle and pair summaries —
    // e.g. "ProvenanceObservables — 200000 event(s), 18611381 studied
    // hadron(s)" and "PairProvenanceObservables ... 1239328831 same-event
    // pair(s)".
    {
    std::string::size_type k = line.find("studied hadron(s)");
    if (k != std::string::npos)
      {
      // Pull the number sitting just before the phrase.
      std::string num;
      std::string::size_type i = k;
      while (i > 0 && std::isspace(static_cast<unsigned char>(line[i-1]))) --i;
      while (i > 0 && std::isdigit(static_cast<unsigned char>(line[i-1])))
        { num = line[i-1] + num; --i; }
      if (!num.empty()) d.studiedHadrons = num;
      }
    k = line.find("same-event pair(s)");
    if (k != std::string::npos)
      {
      std::string num;
      std::string::size_type i = k;
      while (i > 0 && std::isspace(static_cast<unsigned char>(line[i-1]))) --i;
      while (i > 0 && std::isdigit(static_cast<unsigned char>(line[i-1])))
        { num = line[i-1] + num; --i; }
      if (!num.empty()) d.samePairs = num;
      }
    }
    // section markers
    if (line.find("by origin:")                   != std::string::npos)
      { section = "origin"; continue; }
    // Check the more-specific "initiating parton" marker BEFORE the generic
    // parton-flavour one.  The endpoint section's label may carry a
    // "(string endpoint)" qualifier, so match on the "by parton flavour"
    // prefix rather than an exact string.
    if (line.find("by initiating parton flavour") != std::string::npos)
      { section = "initparton"; continue; }
    if (line.find("by parton flavour")            != std::string::npos)
      { section = "parton"; continue; }
    if (line.find("pair correlation by ancestry:")!= std::string::npos)
      { section = "pair";   continue; }
    // class rows:  <name> <count> <pct> %
    if (!section.empty())
      {
      std::istringstream is(line);
      std::string name, count, pct, sym;
      if (is >> name >> count >> pct >> sym && sym == "%")
        {
        ClassRow r; r.name = name; r.count = count; r.pct = pct;
        if      (section == "origin")     d.origin.push_back(r);
        else if (section == "parton")     d.parton.push_back(r);
        else if (section == "initparton") d.initparton.push_back(r);
        else if (section == "pair")       d.pairs.push_back(r);
        }
      }
    }
}

// Parse a cap-mechanism-ladder ladder-comparison.csv.
void parseLadder(const std::string & path, ReportData & d)
{
  std::ifstream f(path.c_str());
  if (!f) { std::cerr << "  ! cannot open ladder csv: " << path << "\n"; return; }
  std::string line;
  bool header = true;
  while (std::getline(f, line))
    {
    if (trim(line).empty()) continue;
    std::vector<std::string> cells = splitCSV(line);
    if (header)
      {
      for (std::size_t i = 2; i < cells.size(); ++i)
        d.ladderRungs.push_back(cells[i]);
      header = false;
      }
    else
      {
      d.ladderRows.push_back(cells);
      }
    }
}

// Parse cap-provenance-plot's figures.manifest ("file | caption" per line).
// Stores absolute figure paths so pdflatex finds them no matter where it runs.
void parseFigures(const std::string & dir, ReportData & d)
{
  const std::string adir = absPath(dir);
  std::string manifest = adir + "/figures.manifest";
  std::ifstream f(manifest.c_str());
  if (!f) { std::cerr << "  ! no figures.manifest in " << dir << "\n"; return; }
  std::string line;
  while (std::getline(f, line))
    {
    std::string::size_type bar = line.find('|');
    if (bar == std::string::npos) continue;
    std::string file = trim(line.substr(0, bar));
    std::string cap  = trim(line.substr(bar + 1));
    if (!file.empty())
      d.figures.push_back(std::make_pair(adir + "/" + file, cap));
    }
}

// True when a figure filename belongs to a given group prefix.
bool figIs(const std::string & path, const std::string & prefix)
{
  std::string::size_type slash = path.find_last_of('/');
  std::string base = (slash == std::string::npos) ? path
                                                  : path.substr(slash + 1);
  return base.compare(0, prefix.size(), prefix) == 0;
}

// Predicates used by renderFiguresPaired() to decide which section a
// figure belongs to.  The "single-particle" predicate is the exclusion-
// based one — anything not pair_ and not ladder_ — so future single-
// particle figures land in the right place without code changes.
bool isPairFigure(const std::string & p)   { return figIs(p, "pair");   }
bool isLadderFigure(const std::string & p) { return figIs(p, "ladder"); }
bool isCompareFigure(const std::string & p){ return figIs(p, "compare"); }
bool isSingleFigure(const std::string & p)
{
  return !isPairFigure(p) && !isLadderFigure(p) && !isCompareFigure(p);
}

// True when `s` ends with `suf`.  Used to select the figures belonging to a
// given species suffix (_S<pdg>) or species-pair suffix (_S<a>x<b>).  A
// trailing pdg never aliases another because any longer number changes the
// trailing characters (e.g. "_S211" does not match "..._S2112").
bool endsWith(const std::string & s, const std::string & suf)
{
  return s.size() >= suf.size() &&
         s.compare(s.size() - suf.size(), suf.size(), suf) == 0;
}

// ---- plot-interpretation captions -----------------------------------------
//
// Goal: each figure should not just be DESCRIBED ("Delta-phi correlation
// decomposed by pair ancestry") but INTERPRETED — "read it like this: the
// near-side peak is X, the broad shoulder is Y, the flat floor is Z."
// The map below is keyed by the figure stem (filename minus extension)
// and returns a short "How to read this plot" paragraph that appears
// AFTER the descriptive caption.  Falls back to empty for figures the
// map doesn't know about (so the descriptive caption still shows).
//
// The text is kept in plain English, escaped automatically.  Stems are
// matched literally; longest prefix wins so a generic 'pair_dphi' falls
// through to the more specific 'pair_dphi_mpi' first.
//
const std::map<std::string,std::string> & figureInterpretations()
{
  // The map data lives in a sibling file so editors / reviewers can
  // change captions in one place without scrolling past hundreds of
  // lines of structural code (audit fix #14).  The .inc file defines
  // the static const map AND emits its own `return M;` — nothing more
  // is needed in this function body.
#include "provenance-report-captions.inc"
}

// Look up a figure's "how to read this" prose by stem.  Empty string means
// no interpretation available, in which case the descriptive caption stands
// on its own.
std::string interpretFigure(const std::string & figPath)
{
  const auto & M = figureInterpretations();
  std::string s = stem(figPath);
  auto it = M.find(s);
  if (it != M.end()) return it->second;
  return "";
}

// Compose the final caption: the descriptive sentence from
// cap-provenance-plot, joined with (if we have one) a "How to read this"
// note.  The two are separated by an em-dash + space — NOT a `\par`,
// because LaTeX's \caption{...} argument is fragile and forbids paragraph
// breaks.  Using \par here was the source of the "Paragraph ended before
// \NR@gettitle was complete" crash observed in pdflatex.
std::string composedCaption(const std::pair<std::string,std::string> & fig)
{
  const std::string desc = texEscape(fig.second);
  const std::string how  = interpretFigure(fig.first);
  if (how.empty()) return desc;
  // The triple-tilde gives roughly an em-dash worth of non-breaking
  // horizontal space; \textit on the label keeps the visual separation
  // without introducing structural breaks.
  return desc + "~~~\\textit{How to read this:}~~~" + texEscape(how);
}

// ---- paired figures (two plots per Figure) -------------------------------
//
// Each entry is (stem_A, stem_B, joint_caption).  The joint caption goes
// under the whole Figure; the descriptive + "How to read this" prose for
// each plot becomes a sub-caption above each minipage.  If either of the
// two figures is missing from the manifest (e.g. shower rung lacks an
// MPI-on plot) the pair degrades gracefully to a single-figure render.
//
// The pairing rule: put naturally complementary plots side-by-side so the
// reader can compare them in one eye-pass — Delta-phi vs Delta-eta of the
// same decomposition, SS vs OS for one observable, etc.
struct PairedFig { const char * a; const char * b; const char * joint; };
const std::vector<PairedFig> & pairedGroups()
{
  static const std::vector<PairedFig> P = {
    // ----- core same-event correlations ---------------------------------
    {"pair_dphi", "pair_deta",
     "Two faces of the same two-particle correlation: azimuthal "
     "(left) and pseudorapidity (right).  Reading them side-by-side "
     "shows which ancestry classes have correlated phi but uncorrelated "
     "eta (string-fragmentation-like) versus correlated in both "
     "(resonance-like or jet-like)."},

    {"pair_dphi_shapes", "pair_deta_shapes",
     "Same as the previous figure but each class is normalised to unit "
     "area, so shape — not yield — is what you compare.  Use this when "
     "the four-order-of-magnitude log spread on the unnormalised plots "
     "is obscuring small shape changes."},

    {"pair_mass_OS", "pair_mass_SS",
     "Opposite-sign (left) vs same-sign (right) pair invariant mass.  "
     "Resonance peaks live on the OS side — rho, omega, K*, phi.  SS "
     "shows the combinatorial + partonic continuum without resonance "
     "contamination; OS minus SS is the resonance-driven excess."},

    // ----- balance-function-style SS/OS splits --------------------------
    {"pair_dphi_OS", "pair_dphi_SS",
     "Opposite-sign vs same-sign Delta-phi correlation.  OS carries the "
     "resonance signal at the near-side peak; SS is the partonic + "
     "combinatorial background.  This is exactly the decomposition a "
     "balance function uses."},

    {"pair_deta_OS", "pair_deta_SS",
     "Opposite-sign vs same-sign Delta-eta correlation.  Same balance-"
     "function logic in the pseudorapidity direction."},

    // ----- MPI / shower / heavy-flavour cross-sections ------------------
    {"pair_dphi_mpi", "pair_deta_mpi",
     "MPI relationship — Delta-phi (left), Delta-eta (right).  Compare "
     "the SameMPI line against the NoMPI line in BOTH plots: same-MPI "
     "pairs concentrate near (0, 0); cross-MPI pairs are nearly flat in "
     "both, the signature of dispersed-across-scatters background."},

    {"pair_dphi_shower", "pair_deta_shower",
     "Shower lineage at the pair level.  BothFSR pairs (jet-like) are "
     "sharply correlated in both panels; BothISR and MixedShower spread "
     "wider; NoShower is essentially flat."},

    {"pair_dphi_hf", "pair_deta_hf",
     "Heavy-flavour decay-chain content of the pair.  Bottom-chain "
     "pairs (purple) have characteristically wide opening angles in "
     "both panels — the signature of long decay cascades."},

    {"pair_dphi_sharedparton_depth", "pair_dphi_sharedparton_mpi",
     "Two refinements of the SharedParton class on the SAME observable.  "
     "Left: same hard-process parton (jet-like, narrow) vs shower/MPI-"
     "only sharing.  Right: did the partonic correlation survive INSIDE "
     "one MPI scatter, or was it dispersed across multiple?  Reading "
     "these together answers 'where did the SharedParton signal go "
     "when MPI was switched on'."},

    // ----- multiplicity-bin progression ---------------------------------
    {"pair_dphi_lowmult", "pair_dphi_highmult",
     "How the pair-class breakdown evolves with event multiplicity.  "
     "LowMult (left) is dominated by the primary hard scatter; HighMult "
     "(right) by accumulated MPI activity.  Compare the SharedParton "
     "and Unrelated lines."},

    // ----- ladder ladders -----------------------------------------------
    {"ladder_dphi_pair_SharedParton", "ladder_dphi_pair_Unrelated",
     "Same observable, four rungs of Pythia.  Left: how does the "
     "SharedParton correlation change as MPI / CR / rope are added.  "
     "Right: how the combinatorial floor grows in parallel.  The two "
     "behaviours are anti-correlated — that is the MPI-dilution story."},

    {"ladder_dphi_pair_MPI_SameMPI", "ladder_dphi_pair_MPI_CrossMPI",
     "Within-MPI versus cross-MPI pair correlation across the ladder.  "
     "Cross-MPI is empty for the shower-only rung (no MPI exists yet); "
     "the way it lights up between rungs quantifies how much of the "
     "Unrelated floor is accidental MPI overlap."},

    {"ladder_pt_HF_FromCharmChain", "ladder_pt_HF_FromBottomChain",
     "Heavy-flavour-feed-down pion pT across the ladder.  Charm (left) "
     "and bottom (right) populations evolve differently with MPI."},

    // ----- single-particle complements ---------------------------------
    {"origin_pt", "origin_eta",
     "Pion pT (left) and eta (right) by production origin.  Compare "
     "the primary vs feed-down lines: feed-down is soft-pT-dominated "
     "but eta-flat; primary spans the full pT range with the same "
     "eta shape."},

    {"sp_pt_shower", "sp_pt_mpi",
     "Pion pT decomposed two different ways: by shower lineage (left) "
     "and by MPI presence (right).  Reading them together shows whether "
     "the high-pT tail is dominated by FSR-touching ancestries OR by "
     "MPI-sourced ancestries — the two are correlated but not identical."},

    {"origin_pt_fraction", "parton_pt_fraction",
     "Local class composition vs pT.  Left: feed-down vs primary; "
     "right: parton-flavour breakdown.  Each curve is N_class / N_all "
     "in that pT bin — even a tiny global class can dominate locally."},

    // ----- flavor-mixing (task #53) -------------------------------------
    {"pair_dphi_flavmix", "pair_deta_flavmix",
     "Flavor-mixing decomposition of the pair correlation.  Side-by-"
     "side Delta-phi (left) and Delta-eta (right) lets you see which "
     "joint flavour classes are correlated in azimuth alone (string-"
     "like) versus correlated in both (jet-like, same-vertex).  This "
     "is the answer to 'how much flavour mixing is driving the pair "
     "correlation'."},

    {"pair_dphi_sharedparton_flavmix", "pair_deta_sharedparton_flavmix",
     "Same decomposition restricted to SharedParton pairs only — i.e. "
     "the genuinely-partonic correlation broken down by flavour "
     "content.  Use this to separate 'same-flavour vertex' partonic "
     "correlation from 'cross-flavour' contributions."},

    // ----- event-shape (spherocity) binning (task #54) ------------------
    {"pair_dphi_jetlike", "pair_dphi_isotropic",
     "Two ends of the event-shape spectrum.  Jet-like events (left) "
     "should show an enhanced SharedParton near-side peak; isotropic "
     "events (right) wash that out and let Unrelated dominate.  The "
     "difference between these two columns IS the event-topology "
     "effect on the pair correlation."},

    {"pair_deta_jetlike", "pair_deta_isotropic",
     "Same JetLike-vs-Isotropic contrast in pseudorapidity.  The "
     "SharedParton near-side eta peak should narrow visibly in the "
     "jet-like column."},
  };
  return P;
}

// Emit a side-by-side figure into the LaTeX stream via doc.addText.
// Uses two equal-width minipages (~48% of \textwidth each) so the
// rendered size matches roughly half of the original centred figures.
// Each panel gets its own sub-caption (the standalone "How to read
// this" prose) above the joint caption.
void emitPairedFigure(LatexDocument & doc,
                      const std::pair<std::string,std::string> & figA,
                      const std::pair<std::string,std::string> & figB,
                      const std::string & jointCaption,
                      const std::string & label = "")
{
  std::ostringstream os;
  os << "\\begin{figure}[H]\\centering\n"
     << "\\begin{minipage}[t]{0.48\\textwidth}\\centering\n"
     << "\\includegraphics[width=\\linewidth]{" << figA.first << "}\n"
     << "\\par\\footnotesize\\textit{(left)} "
     << composedCaption(figA) << "\n"
     << "\\end{minipage}\\hfill\n"
     << "\\begin{minipage}[t]{0.48\\textwidth}\\centering\n"
     << "\\includegraphics[width=\\linewidth]{" << figB.first << "}\n"
     << "\\par\\footnotesize\\textit{(right)} "
     << composedCaption(figB) << "\n"
     << "\\end{minipage}\n"
     << "\\caption{" << texEscape(jointCaption) << "}\n";
  if (!label.empty()) os << "\\label{" << label << "}\n";
  os << "\\end{figure}\n";
  doc.addText(L(os.str()));
}

// Build a stem -> figure-record index over the manifest so the pairing
// loop can pluck figures by stem regardless of their order in the file.
std::map<std::string, std::pair<std::string,std::string>>
indexFiguresByStem(const std::vector<std::pair<std::string,std::string>> & figs)
{
  std::map<std::string, std::pair<std::string,std::string>> M;
  for (const auto & f : figs) M[stem(f.first)] = f;
  return M;
}

// Render every figure in `figs` that matches `groupPredicate` (e.g.
// isPairFigure or isLadderFigure), pairing them where the pairedGroups()
// table says so and leaving the rest as singletons.
//
// `suffix` supports multi-species output: the pairedGroups() table is keyed
// by the legacy bare stems (e.g. "pair_dphi"), so to pair the per-species
// figures we append the species / species-pair suffix to each table entry
// before looking it up (e.g. "pair_dphi" + "_S211x321").  The default empty
// suffix reproduces the legacy single-species pairing exactly.
//
// The predicate is a std::function so callers can pass a capturing lambda
// (used to restrict to one species suffix); plain function pointers such as
// isPairFigure still convert implicitly.
void renderFiguresPaired(
    LatexDocument & doc,
    const std::vector<std::pair<std::string,std::string>> & figs,
    const std::function<bool(const std::string &)> & groupPredicate,
    const std::string & suffix = "")
{
  const auto index = indexFiguresByStem(figs);
  std::set<std::string> consumed;

  // First pass — paired groups.
  for (const auto & pg : pairedGroups())
    {
    const std::string a = std::string(pg.a) + suffix;
    const std::string b = std::string(pg.b) + suffix;
    if (consumed.count(a) || consumed.count(b)) continue;
    auto ia = index.find(a);
    auto ib = index.find(b);
    if (ia == index.end() || ib == index.end()) continue;
    if (!groupPredicate(ia->second.first) ||
        !groupPredicate(ib->second.first)) continue;
    emitPairedFigure(doc, ia->second, ib->second, pg.joint);
    consumed.insert(a);
    consumed.insert(b);
    }

  // Second pass — anything in the predicate group that wasn't paired
  // gets rendered as the standalone full-width figure (existing behaviour).
  for (const auto & f : figs)
    {
    if (!groupPredicate(f.first)) continue;
    if (consumed.count(stem(f.first))) continue;
    doc.addFigure(L(f.first), L(""), L(composedCaption(f)));
    }
}

// ---- glossary -------------------------------------------------------------
struct GlossaryEntry { const char * term; const char * definition; };

const std::vector<GlossaryEntry> & glossary()
{
  static const std::vector<GlossaryEntry> G = {
    {"Primary",
     "A hadron formed directly at hadronization from a string or cluster; "
     "no preceding hadronic parent.  In Pythia this is anything tagged "
     "Stage::PrimaryHadrons.  Exact, no curated list needed."},
    {"FromResonance",
     "A hadron produced by the decay of a short-lived (strong / "
     "electromagnetic) resonance such as rho, omega, phi, K*, Delta.  "
     "Tagged using a curated PDG list."},
    {"FromWeakDecay",
     "A hadron produced by a weak decay — typically descended from K0S, "
     "Lambda, charm or bottom hadrons.  These have macroscopic lifetimes; "
     "what the detector sees as a 'primary track' is usually one of these."},
    {"SameResonance (pair)",
     "Both hadrons in the pair are decay products of ONE resonance.  This "
     "is the canonical 'resonance contamination' of a same-event "
     "correlation."},
    {"SameWeakParent (pair)",
     "Both hadrons in the pair share one weak-decay parent (e.g. both "
     "from a single K0S)."},
    {"SharedParton (pair)",
     "Both hadrons trace back to at least one common pre-hadronization "
     "parton.  This is the inherited partonic correlation — jet-like or "
     "string-like."},
    {"Unrelated (pair)",
     "No shared ancestry of any kind.  Combinatorial pairs that "
     "happen to be in the same event."},
    {"SameMPI / CrossMPI / OneSideMPI / NoMPI",
     "MPI relationship at the pair level.  SameMPI: both hadrons came "
     "from ONE multi-parton-interaction scatter.  CrossMPI: each came from "
     "a DIFFERENT MPI scatter.  OneSideMPI: only one carries an MPI "
     "ancestor.  NoMPI: both come from the primary hard scatter (or beam "
     "remnants) — no MPI in their ancestry."},
    {"BothFSR / BothISR / MixedShower / NoShower",
     "Shower-stage lineage of the pair.  BothFSR: both hadrons walked "
     "through FSR (final-state radiation).  BothISR: both through ISR.  "
     "MixedShower: one from each / different shower mix.  NoShower: "
     "neither hadron has any shower ancestry."},
    {"FromCharmChain / FromBottomChain",
     "The hadronic parent chain walked back from this hadron passes "
     "through a charm or bottom hadron at some point.  Useful for "
     "isolating heavy-flavour feed-down which otherwise hides inside "
     "FromResonance / FromWeakDecay."},
    {"Mechanism ladder",
     "A set of Pythia configurations differing only in which mechanisms "
     "are switched on: shower-only, +MPI, +MPI+CR, +MPI+CR+rope.  "
     "Comparing the provenance breakdown across rungs isolates the effect "
     "of each mechanism."},
    {"LowMult / MidMult / HighMult",
     "Event-multiplicity bins.  LowMult: fewer than 20 studied hadrons in "
     "the event.  MidMult: 20-79.  HighMult: 80 or more.  Cuts are "
     "deliberately generator-agnostic."},
  };
  return G;
}

// ---- headline numbers -----------------------------------------------------
//
// Extract one row's percentage from a ClassRow list (case-insensitive).
// Returns empty string if not found.
std::string pctOf(const std::vector<ClassRow> & rows, const std::string & name)
{
  for (const auto & r : rows)
    if (r.name == name) return r.pct;
  return "";
}

// Find a ladder cell.  rows[i] is "group,observable,rung1Val,rung2Val,..."
// Returns the cell value for (group:observable, columnName).
std::string ladderCell(const ReportData & d,
                       const std::string & group,
                       const std::string & obs,
                       const std::string & rung)
{
  int col = -1;
  for (std::size_t i = 0; i < d.ladderRungs.size(); ++i)
    if (d.ladderRungs[i] == rung) { col = static_cast<int>(i); break; }
  if (col < 0) return "";
  for (const auto & row : d.ladderRows)
    if (row.size() >= 2 + d.ladderRungs.size() &&
        row[0] == group && row[1] == obs)
      return row[2 + col];
  return "";
}

// Compute (first, last, delta) for one ladder observable, or empty
// strings if the ladder isn't present.
std::array<std::string,3> ladderDelta(const ReportData & d,
                                      const std::string & group,
                                      const std::string & obs)
{
  std::array<std::string,3> r{"", "", ""};
  if (d.ladderRungs.empty()) return r;
  const std::string first = ladderCell(d, group, obs, d.ladderRungs.front());
  const std::string last  = ladderCell(d, group, obs, d.ladderRungs.back());
  r[0] = first; r[1] = last;
  try
    {
    if (!first.empty() && !last.empty())
      {
      double a = std::stod(first), b = std::stod(last);
      std::ostringstream os;
      os << std::fixed << std::setprecision(2) << (b - a);
      r[2] = (b >= a ? "+" : "") + os.str();
      }
    }
  catch (...) { }
  return r;
}

// ---- table helpers --------------------------------------------------------
void fillClassTable(LatexTable & t, const std::string & col0,
                    const std::vector<ClassRow> & rows)
{
  t.setColumnSpec("l r r");
  t.setHeaderRows(1);
  t.addRow({ L(col0), L("count"), L("fraction") });
  for (std::size_t i = 0; i < rows.size(); ++i)
    t.addRow({ L(rows[i].name), L(rows[i].count), L(rows[i].pct + "\\%") });
}

// Side-by-side fraction comparison of one decomposition (origin / parton /
// pair) between two generators.  Columns: class | <labelA> % | <labelB> %.
// The row set is the UNION of classes present in either generator, ordered by
// A first then any B-only classes, so a class that only one generator produces
// (e.g. a parton flavour with zero yield in the cluster model) still appears,
// with "--" where it is absent.
void fillCompareTable(LatexTable & t, const std::string & col0,
                      const std::string & labelA, const std::string & labelB,
                      const std::vector<ClassRow> & rowsA,
                      const std::vector<ClassRow> & rowsB)
{
  t.setColumnSpec("l r r");
  t.setHeaderRows(1);
  t.addRow({ L(col0), L(labelA + " (\\%)"), L(labelB + " (\\%)") });

  std::vector<std::string> order;
  std::set<std::string> seen;
  for (const auto & r : rowsA)
    if (seen.insert(r.name).second) order.push_back(r.name);
  for (const auto & r : rowsB)
    if (seen.insert(r.name).second) order.push_back(r.name);

  for (const auto & name : order)
    {
    const std::string a = pctOf(rowsA, name);
    const std::string b = pctOf(rowsB, name);
    t.addRow({ L(name),
               L(a.empty() ? "--" : a),
               L(b.empty() ? "--" : b) });
    }
}

// N-column fraction table: one column per (generator, mechanism-rung).  Rows
// are the UNION of class names across every column, in first-seen order; a
// cell is "--" where that column lacks the class.  Used by the mechanism-
// ladder comparison so you can read a class's fraction left-to-right as
// mechanisms (MPI, CR) are switched on, in both generators.
void fillMultiTable(LatexTable & t, const std::string & col0,
                    const std::vector<std::string> & labels,
                    const std::vector<std::vector<ClassRow>> & cols)
{
  std::string spec = "l";
  for (std::size_t i = 0; i < labels.size(); ++i) spec += " r";
  t.setColumnSpec(spec);
  t.setHeaderRows(1);

  std::vector<String> header;
  header.push_back(L(col0));
  for (const auto & lab : labels) header.push_back(L(lab));
  t.addRow(header);

  std::vector<std::string> order;
  std::set<std::string> seen;
  for (const auto & col : cols)
    for (const auto & r : col)
      if (seen.insert(r.name).second) order.push_back(r.name);

  for (const auto & name : order)
    {
    std::vector<String> row;
    row.push_back(L(name));
    for (const auto & col : cols)
      {
      const std::string v = pctOf(col, name);
      row.push_back(L(v.empty() ? "--" : v));
      }
    t.addRow(row);
    }
}

void fillConfigTable(LatexTable & t, const ReportData & d)
{
  t.setColumnSpec("l l");
  t.setHeaderRows(1);
  t.addRow({ L("parameter"), L("value") });
  t.addRow({ L("events"),         L(d.events.empty()  ? "?" : d.events) });
  t.addRow({ L("$\\sqrt{s}$"),    L((d.ecm.empty() ? "?" : d.ecm) + " GeV") });
  t.addRow({ L("QCD process"),    L(d.process.empty() ? "?" : d.process) });
  t.addRow({ L("random seed"),    L(d.seed.empty()    ? "?" : d.seed) });
  std::string speciesCell;
  if (d.speciesList.size() > 1)
    {
    for (std::size_t i = 0; i < d.speciesList.size(); ++i)
      speciesCell += (i ? ", " : "") + std::to_string(d.speciesList[i]);
    }
  else
    speciesCell = d.species.empty() ? "all" : d.species;
  t.addRow({ L("species (|pdg|)"), L(speciesCell) });
}

void fillBinningTable(LatexTable & t)
{
  // The analysis binning is fixed in ProvenanceObservables /
  // PairProvenanceObservables; documented here for the record.
  t.setColumnSpec("l r l");
  t.setHeaderRows(1);
  t.addRow({ L("observable"), L("bins"), L("range") });
  t.addRow({ L("$p_{T}$"),       L("100"), L("0 -- 10 GeV") });
  t.addRow({ L("$\\eta$"),       L("120"), L("$-6$ -- $6$") });
  t.addRow({ L("$\\Delta\\eta$"),L("80"),  L("$-8$ -- $8$") });
  t.addRow({ L("$\\Delta\\phi$"),L("72"),  L("$-\\pi$ -- $\\pi$") });
}

void fillLadderTable(LatexTable & t, const ReportData & d)
{
  std::string spec = "l l";
  for (std::size_t i = 0; i < d.ladderRungs.size(); ++i) spec += " r";
  t.setColumnSpec(L(spec));
  t.setHeaderRows(1);
  std::vector<String> head;
  head.push_back(L("group"));
  head.push_back(L("observable"));
  for (std::size_t i = 0; i < d.ladderRungs.size(); ++i)
    head.push_back(L(d.ladderRungs[i]));
  t.addRow(head);
  for (std::size_t r = 0; r < d.ladderRows.size(); ++r)
    {
    std::vector<String> row;
    for (std::size_t c = 0; c < d.ladderRows[r].size(); ++c)
      row.push_back(L(d.ladderRows[r][c]));
    t.addRow(row);
    }
}

// ---- glossary rendering ---------------------------------------------------
void fillGlossaryTable(LatexTable & t)
{
  t.setColumnSpec("p{0.22\\textwidth} p{0.72\\textwidth}");
  t.setHeaderRows(1);
  t.addRow({ L("term"), L("meaning") });
  for (const auto & g : glossary())
    t.addRow({ L(g.term), L(texEscape(g.definition)) });
}

// ---- executive-summary bullets --------------------------------------------
void addExecutiveSummary(LatexDocument & doc, const ReportData & d)
{
  doc.addSection(L("Headlines"), L("sec:headlines"));
  doc.addText(L(
    "This run's findings, in a sentence each.  See the cited sections for "
    "the supporting plot or table."));

  std::ostringstream itm;
  itm << "\\begin{itemize}\n";

  // --- run scale ---
  if (!d.events.empty())
    {
    itm << "\\item \\textbf{Scale.}  " << d.events << " events";
    if (!d.studiedHadrons.empty()) itm << ", " << d.studiedHadrons
                                      << " studied pions";
    if (!d.samePairs.empty())      itm << ", " << d.samePairs
                                      << " same-event pairs.";
    else                           itm << ".";
    itm << "  (Sec.~\\ref{sec:config}.)\n";
    }

  // --- origin composition ---
  std::string pPri  = pctOf(d.origin, "Primary");
  std::string pRes  = pctOf(d.origin, "FromResonance");
  std::string pWeak = pctOf(d.origin, "FromWeakDecay");
  if (!pPri.empty() || !pRes.empty() || !pWeak.empty())
    {
    itm << "\\item \\textbf{Where pions come from.}  Primary "
        << pPri << "\\%, from resonance " << pRes
        << "\\%, from weak decay " << pWeak
        << "\\%.  Direct hadronization is the minority; resonance feed-down "
           "dominates.  (Tab.~\\ref{tab:origin}.)\n";
    }

  // --- parton flavour composition ---
  std::string pLight = pctOf(d.parton, "LightQuark");
  std::string pStr   = pctOf(d.parton, "Strange");
  std::string pGluon = pctOf(d.parton, "Gluon");
  if (!pLight.empty())
    {
    itm << "\\item \\textbf{Ancestor flavour.}  Light-quark "
        << pLight << "\\%, strange " << pStr
        << "\\%, gluon " << pGluon
        << "\\%.  The gluon fraction is small but non-zero only when MPI "
           "is on.  (Tab.~\\ref{tab:parton}.)\n";
    }

  // --- pair ancestry composition ---
  std::string ppRes = pctOf(d.pairs, "SameResonance");
  std::string ppSP  = pctOf(d.pairs, "SharedParton");
  std::string ppU   = pctOf(d.pairs, "Unrelated");
  if (!ppU.empty())
    {
    itm << "\\item \\textbf{Pair ancestry.}  SameResonance "
        << ppRes << "\\%, SharedParton " << ppSP
        << "\\%, Unrelated " << ppU
        << "\\%.  The ancestry-driven part of the same-event correlation "
           "is " << ppRes << "+" << ppSP
        << "\\%; the rest is combinatorial.  (Tab.~\\ref{tab:pair}.)\n";
    }

  // --- ladder deltas ---
  if (!d.ladderRows.empty())
    {
    auto sp = ladderDelta(d, "pair",   "SharedParton");
    auto un = ladderDelta(d, "pair",   "Unrelated");
    auto gl = ladderDelta(d, "parton", "Gluon");
    if (!sp[0].empty() && !sp[1].empty())
      itm << "\\item \\textbf{Mechanism ladder.}  SharedParton goes "
          << sp[0] << "\\% $\\to$ " << sp[1] << "\\%, a shift of "
          << sp[2] << "\\%.  Unrelated goes " << un[0] << "\\% $\\to$ "
          << un[1] << "\\%.  Gluon-ancestry pions go " << gl[0]
          << "\\% $\\to$ " << gl[1]
          << "\\% — appearing only when MPI is on.  "
             "(Sec.~\\ref{sec:ladder}.)\n";
    }

  itm << "\\end{itemize}\n";
  doc.addText(L(itm.str()));
  doc.endSection();
}

// ===========================================================================
//  Paper mode  —  an 'article' document.
// ===========================================================================
void buildPaper(LatexDocument & doc, const ReportData & d)
{
  doc.addPackage("graphicx", "");
  doc.addPackage("float",    "");   // [H] placement
  doc.addPackage("hyperref", "");   // cross-refs in headlines
  doc.addPackage("listings", "");   // verbatim Pythia config

  doc.addAbstract(L(
    "Every final-state pion in Pythia 8 events is traced back through a "
    "generator-agnostic event-history DAG to its parton ancestors and its "
    "decay parent (if any).  Single-particle and two-particle observables "
    "are then decomposed by that ancestry, separating direct hadronization, "
    "resonance feed-down, weak-decay feed-down, and partonic-stage "
    "correlation.  The same analysis is repeated as Pythia mechanisms (MPI, "
    "Color Reconnection, rope hadronization) are switched on, isolating "
    "which mechanism is responsible for each effect.  The report is laid "
    "out as: headline findings, then how to read the document, then a "
    "glossary, then single-particle and pair results, then the mechanism "
    "ladder, then the reproducibility appendix."));

  // ---------------- executive summary (NEW) -----------------------------
  addExecutiveSummary(doc, d);

  // ---------------- how to read this report (NEW) -----------------------
  doc.addSection(L("How to read this report"), L("sec:howto"));
  doc.addText(L(
    "The report is organised top-down: scan the headline bullets above, "
    "consult the glossary if any class names are unfamiliar, then jump to "
    "the section the headline cited.  Every figure carries TWO captions: "
    "a short description (what the plot IS) and a 'how to read this' note "
    "(what the reader should LOOK AT).  Read the second.  Each section also "
    "begins with a one-paragraph orientation that ties its figures and "
    "tables together.  The Reproducibility appendix at the end records the "
    "exact Pythia configuration used, including any decay disablers or "
    "lifetime cuts, plus the git revision of the analysis code — enough to "
    "re-run this study unchanged."));
  doc.endSection();

  // ---------------- glossary (NEW) --------------------------------------
  doc.addSection(L("Glossary of provenance classes"), L("sec:glossary"));
  doc.addText(L(
    "Every class used in the rest of the report is defined here in one "
    "sentence."));
  fillGlossaryTable(doc.addTable(L("Provenance-class definitions."),
                                 L("tab:glossary")));
  doc.endSection();

  // ---------------- method (kept, lightly expanded) ---------------------
  doc.addSection(L("Method"), L("sec:method"));
  doc.addText(L(
    "Each Pythia event record is converted into an EventHistory directed-"
    "acyclic graph — one node per particle, edges for mother/daughter "
    "links.  Each node carries a generator-agnostic Stage label (Beam, "
    "HardProcess, MPI, ISR, FSR, BeamRemnants, PartonsPreHadronization, "
    "PrimaryHadrons, DecayProducts, FinalState) and its kinematics.  For "
    "every final-state hadron the ProvenanceTagger walks parent links "
    "upward, recording (a) the production stage, (b) the first non-parton "
    "parent and whether it is a curated resonance, (c) the set of pre-"
    "hadronization parton ancestors plus the deepest hard-process parton, "
    "(d) any MPI / ISR / FSR ancestor, (e) any charm or bottom hadron in "
    "the parent chain, and (f) the depth of the hadronic decay chain.  "
    "Single-particle observables ($p_{T}$, $\\eta$, multiplicity) and "
    "two-particle observables ($\\Delta\\phi$, $\\Delta\\eta$, pair mass) "
    "are then accumulated, with each fill split by these provenance tags."));
  doc.endSection();

  // ---------------- configuration ---------------------------------------
  doc.addSection(L("Configuration and binning"), L("sec:config"));
  fillConfigTable(doc.addTable(L("Monte-Carlo run configuration."),
                               L("tab:config")), d);
  fillBinningTable(doc.addTable(L("Analysis binning."), L("tab:binning")));
  doc.endSection();

  // ---------------- single-particle results -----------------------------
  doc.addSection(L("Single-particle results"), L("sec:single"));
  doc.addText(L(
    "We first ask: where does each pion come from?  Two complementary "
    "tables answer this — Tab.~\\ref{tab:origin} splits the yield by "
    "production origin (Primary / from resonance / from weak decay), and "
    "Tab.~\\ref{tab:parton} splits it by the flavour of the ancestor parton.  "
    "The figures that follow show how the shape of the $p_{T}$ and "
    "$\\eta$ spectra differ between classes, and additional subdivisions by "
    "shower lineage (ISR vs FSR), MPI ancestry, heavy-flavour decay-chain "
    "content, and the decay-chain depth.  The fraction-vs-$p_{T}$ overlays "
    "show how the local class composition changes with momentum — feed-down "
    "dominates the soft region, primary dominates the hard."));
  if (!d.origin.empty())
    fillClassTable(doc.addTable(L("Pion yield by production origin."),
                                L("tab:origin")), "origin class", d.origin);
  if (!d.parton.empty())
    fillClassTable(doc.addTable(L("Pion yield by string-endpoint parton "
                                  "flavour (Lund endpoints are quarks, so "
                                  "Gluon is 0\\% by construction)."),
                                L("tab:parton")), "endpoint parton", d.parton);
  if (!d.initparton.empty())
    {
    doc.addText(L(
      "The table above is the string-ENDPOINT flavour and is gluon-free by "
      "construction.  The table below gives the INITIATING hard-scatter "
      "parton instead -- the quark- vs gluon-jet origin -- which IS "
      "gluon-capable and is the number to read for gluon-originated yield."));
    fillClassTable(doc.addTable(L("Pion yield by INITIATING (hard-scatter) "
                                  "parton flavour -- the gluon-vs-quark "
                                  "origin."),
                                L("tab:initparton")), "initiating parton",
                   d.initparton);
    doc.addText(L(
      "The initiating parton in Tab.~\\ref{tab:initparton} is the topmost "
      "parton in the hadron's lineage -- the INITIAL-STATE parton extracted "
      "from the proton (its parents are the beams).  This answers \"did this "
      "hadron's lineage start from a gluon or a quark?\" and is gluon-capable, "
      "unlike the string-endpoint flavour above which is gluon-free by "
      "construction.  Note it is distinct from the outgoing gluon-jet-vs-"
      "quark-jet origin (the parton just after the hard scatter): for a single "
      "generator both views are available, but only this initial-state "
      "definition is recoverable identically across generators, so it is the "
      "one used in the cross-generator comparison report."));
    }
  if (d.speciesList.size() > 1)
    {
    // Multi-species: one subsection per species, each rendering only that
    // species' single-particle figures (suffix _S<pdg>).
    doc.addText(L(
      "This run studied several species; the single-particle figures below "
      "are grouped into one subsection per species."));
    for (int s : d.speciesList)
      {
      const std::string sfx = "_S" + std::to_string(s);
      doc.addText(L("\\subsection*{Single-particle spectra (species PDG "
                    + std::to_string(s) + ")}"));
      renderFiguresPaired(
        doc, d.figures,
        [sfx](const std::string & p)
          { return isSingleFigure(p) && endsWith(stem(p), sfx); },
        sfx);
      }
    }
  else
    {
    renderFiguresPaired(doc, d.figures, isSingleFigure);
    }
  doc.endSection();

  // ---------------- two-particle results --------------------------------
  doc.addSection(L("Two-particle results"), L("sec:pair"));
  doc.addText(L(
    "Single-particle decomposition tells us where individual pions come "
    "from.  Two-particle observables answer a sharper question: what "
    "fraction of the SAME-EVENT pair CORRELATION is ancestry-driven, and "
    "of what kind?  Tab.~\\ref{tab:pair} gives the global breakdown.  The "
    "figures that follow show $\\Delta\\phi$, $\\Delta\\eta$, and pair "
    "invariant-mass distributions decomposed by pair class — and "
    "further sub-divided by charge sign (SS vs OS), parent resonance, "
    "shared-parton sharing depth, MPI relationship, shower lineage, "
    "heavy-flavour content, decay-chain depth, and event-multiplicity "
    "bin.  Use the 'how to read this' note under each figure to find the "
    "feature the plot is meant to highlight."));
  if (!d.pairs.empty())
    fillClassTable(doc.addTable(L("Same-event pion pairs by ancestry."),
                                L("tab:pair")), "pair class", d.pairs);
  if (d.speciesList.size() > 1)
    {
    // Multi-species: one subsection per species-pair (canonical a<=b),
    // N(N+1)/2 of them, each rendering only that pair's figures
    // (suffix _S<a>x<b>).
    doc.addText(L(
      "With several species studied, the pair observables are split into "
      "one subsection per species combination (each unordered pair of "
      "species, including same-species pairs)."));
    for (std::size_t ai = 0; ai < d.speciesList.size(); ++ai)
      for (std::size_t bi = ai; bi < d.speciesList.size(); ++bi)
        {
        const int p1 = std::min(d.speciesList[ai], d.speciesList[bi]);
        const int p2 = std::max(d.speciesList[ai], d.speciesList[bi]);
        const std::string sfx = "_S" + std::to_string(p1) + "x"
                              + std::to_string(p2);
        doc.addText(L("\\subsection*{Two-particle correlations (species PDG "
                      + std::to_string(p1) + " $\\times$ "
                      + std::to_string(p2) + ")}"));
        renderFiguresPaired(
          doc, d.figures,
          [sfx](const std::string & p)
            { return isPairFigure(p) && endsWith(stem(p), sfx); },
          sfx);
        }
    }
  else
    {
    renderFiguresPaired(doc, d.figures, isPairFigure);
    }
  doc.endSection();

  // ---------------- mechanism ladder ------------------------------------
  if (!d.ladderRows.empty())
    {
    doc.addSection(L("Mechanism ladder"), L("sec:ladder"));
    doc.addText(L(
      "The ladder repeats the entire analysis as Pythia mechanisms are "
      "switched on one rung at a time: shower-only, +MPI, +Color "
      "Reconnection, +rope hadronization.  Reading the table left-to-right "
      "shows which mechanism causes which observable to move.  The most "
      "striking shifts in this run: gluon ancestry, which is identically "
      "zero in pure-shower runs, appears only once MPI is on; the "
      "SharedParton pair fraction halves between shower and shower+MPI "
      "(the MPI-dilution effect); the strange-parton fraction nearly "
      "doubles.  Color Reconnection and rope hadronization produce more "
      "subtle, harder-to-read shifts.  The figures repeat one observable "
      "per plot with all four rungs overlaid, normalised so shape changes "
      "are visible at a glance."));
    fillLadderTable(doc.addTable(L("Provenance breakdown across the "
                                   "mechanism ladder (percentages)."),
                                 L("tab:ladder")), d);
    renderFiguresPaired(doc, d.figures, isLadderFigure);
    doc.endSection();
    }

  // ---------------- conclusions -----------------------------------------
  doc.addSection(L("Conclusions and open questions"), L("sec:concl"));
  doc.addText(L(
    "Three findings stand out from this run.  First, the majority of pions "
    "in soft QCD are NOT direct hadronization products: roughly two-thirds "
    "come from resonance feed-down or weak decay, and that fraction is "
    "stable across mechanisms.  Second, gluon-ancestry pions appear only "
    "when MPI is enabled — the gluon population in soft QCD is overwhelm"
    "ingly an MPI phenomenon.  Third, the SharedParton component of the "
    "two-particle correlation is approximately halved when MPI turns on; "
    "the MPI-relationship sub-plots show whether that correlation moved "
    "into the cross-MPI category (a dispersal of the partonic correlation "
    "across multiple scatters) or vanished entirely.  Open questions "
    "include statistical uncertainties on the small pair classes "
    "(SameResonance is at the 0.2-0.3 percent level, requiring a "
    "jackknife or sqrt(N) treatment before quotation), comparison with "
    "alternative tunes, and extension to other species (kaons, protons)."));
  doc.endSection();

  // ---------------- reproducibility appendix (NEW) ----------------------
  doc.addSection(L("Appendix: Reproducibility envelope"), L("sec:repro"));
  doc.addText(L(
    "Everything required to exactly reproduce this run is reported here: "
    "the Pythia configuration the standalone study used, the per-rung "
    "configurations the mechanism ladder used, and the analysis-code "
    "version (git revision)."));
  {
  // Repro metadata table.
  LatexTable & t = doc.addTable(L("Analysis environment."),
                                L("tab:repro"));
  t.setColumnSpec("l l");
  t.setHeaderRows(1);
  t.addRow({ L("metadata"), L("value") });
  if (!d.gitSha.empty())
    t.addRow({ L("git revision"), L(d.gitSha) });
  if (!d.gitBranch.empty())
    t.addRow({ L("git branch"),   L(d.gitBranch) });
  if (!d.hostname.empty())
    t.addRow({ L("host"),         L(d.hostname) });
  if (!d.reportDate.empty())
    t.addRow({ L("generated"),    L(d.reportDate) });
  if (!d.rootVersion.empty())
    t.addRow({ L("ROOT version"), L(d.rootVersion) });
  if (!d.buildHost.empty())
    t.addRow({ L("build host"),   L(d.buildHost) });
  }

  if (!d.pythiaCmnd.empty())
    {
    doc.addText(L("\\subsection*{Standalone Pythia configuration}"));
    if (!d.pythiaCmndPath.empty())
      doc.addText(L("Source file: \\texttt{" +
                    texEscape(d.pythiaCmndPath) + "}"));
    doc.addText(L(
      "\\begin{lstlisting}[basicstyle=\\ttfamily\\footnotesize,"
      "frame=single,breaklines=true]\n"
      + d.pythiaCmnd + "\n\\end{lstlisting}"));
    }

  if (!d.rungConfigs.empty())
    {
    doc.addText(L("\\subsection*{Mechanism-ladder rung configurations}"));
    for (const auto & rc : d.rungConfigs)
      {
      doc.addText(L("\\paragraph{" + texEscape(rc.name) + "}\\mbox{}\\\\"));
      doc.addText(L(
        "\\begin{lstlisting}[basicstyle=\\ttfamily\\footnotesize,"
        "frame=single,breaklines=true]\n"
        + rc.content + "\n\\end{lstlisting}"));
      }
    }
  doc.endSection();
}

// ===========================================================================
//  Comparison mode  —  ONE report contrasting two event generators.
//
//  Driven by two parsed summaries (dA = primary, e.g. Pythia; dB = the other,
//  e.g. Herwig) plus the shared figure directory whose compare_*.png each
//  overlay BOTH generators.  The individual single-generator reports are still
//  produced separately; this is the cross-generator synthesis the analyst
//  reads to see, side by side, where each generator sources its particles.
// ===========================================================================
void buildComparison(LatexDocument & doc, const ReportData & dA,
                     const ReportData & dB,
                     const std::string & labelA, const std::string & labelB)
{
  doc.addPackage("graphicx", "");
  doc.addPackage("float",    "");
  doc.addPackage("hyperref", "");

  doc.addAbstract(L(
    "The same provenance analysis is applied to two event generators, " +
    labelA + " and " + labelB + ", and their results are placed side by "
    "side.  Each final-state hadron is traced through a generator-agnostic "
    "event-history DAG to its production origin (direct hadronization, "
    "resonance feed-down, weak-decay feed-down) and to the flavour of its "
    "ancestor parton; each same-event pair is classified by shared ancestry.  "
    "Because the two generators use fundamentally different hadronization "
    "models -- " + labelA + " a Lund string, " + labelB + " a cluster model "
    "-- the genealogy fractions reveal, quantitatively, how each builds its "
    "final state.  Every comparison figure overlays both generators."));

  // ---- how to read ------------------------------------------------------
  doc.addSection(L("How to read this comparison"), L("sec:howto"));
  doc.addText(L(
    "This report contrasts " + labelA + " and " + labelB + ".  The genealogy "
    "tables give the production-origin and parton-flavour fractions for both "
    "generators in adjacent columns -- read across a row to see how the two "
    "differ for one class.  The pair-origin table does the same for the "
    "two-particle correlation.  Every figure overlays both generators in "
    "their fixed colours (" + labelA + " blue, " + labelB + " red).  A curve "
    "labelled \"(no entries)\" means that generator does not populate that "
    "observable -- itself a physics statement, not a missing plot."));
  doc.endSection();

  // ---- what is comparable (the research note) --------------------------
  doc.addSection(L("What is comparable between the two generators"),
                 L("sec:comparable"));
  doc.addText(L(
    "The two generators are not the same program with different numbers; "
    "they implement different physics models.  Some components are "
    "conceptually shared and so can be compared mechanism-for-mechanism, "
    "while others are structurally different and explain the divergences "
    "seen in the tables below."));
  {
  LatexTable & t = doc.addTable(L("Generator components: what is comparable."),
                                L("tab:comparable"));
  t.setColumnSpec("l l");
  t.setHeaderRows(1);
  t.addRow({ L("component"), L("comparability") });
  t.addRow({ L("Multiparton interactions (MPI)"),
             L("Both model MPI as the source of the underlying event; can be "
               "toggled on/off in each.  Directly comparable.") });
  t.addRow({ L("Colour reconnection (CR)"),
             L("Both reconnect colour lines before hadronization (" + labelA +
               " an MPI-based scheme, " + labelB + " a cluster/spacetime "
               "scheme).  Comparable in effect, not in implementation.") });
  t.addRow({ L("Parton shower"),
             L(labelA + " uses a $p_{T}$-ordered dipole shower; " + labelB +
               " an angular-ordered shower.  Both produce ISR/FSR, so the "
               "presence of shower radiation is comparable, but per-particle "
               "ISR-vs-FSR lineage is generator-internal.") });
  t.addRow({ L("Hadronization"),
             L(labelA + ": Lund string; " + labelB + ": cluster.  NOT "
               "interchangeable -- this is the principal source of the "
               "genealogy differences (primary fraction, gluon ancestry, "
               "multiplicity).") });
  }
  doc.addText(L(
    "One consequence matters for the figures: per-particle MPI-index and "
    "ISR/FSR shower-origin tags are read directly from the " + labelA +
    " status codes, but must be inferred from the " + labelB + " HepMC "
    "graph, which does not preserve them.  The MPI-relationship and "
    "shower-lineage pair decompositions are therefore shown only in the "
    "individual " + labelA + " report; this comparison restricts itself to "
    "observables BOTH generators populate, so no comparison figure is "
    "silently single-generator."));
  doc.endSection();

  // ---- genealogy comparison (origin + parton) --------------------------
  doc.addSection(L("Genealogy comparison"), L("sec:genealogy"));
  doc.addText(L(
    "Where does each generator get its particles from?  "
    "Tab.~\\ref{tab:cmp-origin} splits the single-particle yield by "
    "production origin; Tab.~\\ref{tab:cmp-parton} by the flavour of the "
    "ancestor parton.  Identical analysis, both generators, side by side."));
  if (!dA.origin.empty() || !dB.origin.empty())
    fillCompareTable(doc.addTable(L("Yield by production origin, " + labelA +
                                    " vs " + labelB + "."),
                                  L("tab:cmp-origin")),
                     "origin class", labelA, labelB, dA.origin, dB.origin);
  if (!dA.parton.empty() || !dB.parton.empty())
    fillCompareTable(doc.addTable(L("Yield by string-endpoint parton flavour, "
                                    + labelA + " vs " + labelB + "."),
                                  L("tab:cmp-parton")),
                     "endpoint parton", labelA, labelB, dA.parton, dB.parton);
  doc.addText(L(
    "Two senses of \"parton flavour\" are reported and they answer different "
    "questions.  Tab.~\\ref{tab:cmp-parton} is the STRING-ENDPOINT flavour: "
    "the quark the hadron's fragmenting string terminated on.  Gluons are "
    "never string endpoints, so this column is gluon-free BY CONSTRUCTION in "
    "both generators -- that 0\\% is correct, not a bug.  "
    "Tab.~\\ref{tab:cmp-initparton} instead gives the INITIATING hard-scatter "
    "parton -- the quark- vs gluon-jet origin -- which at LHC soft QCD is "
    "gluon-dominated.  This is the number to read for \"what fraction of "
    "pions originate from a gluon\"."));
  if (!dA.initparton.empty() || !dB.initparton.empty())
    fillCompareTable(doc.addTable(L("Yield by INITIATING (hard-scatter) parton "
                                    "flavour, " + labelA + " vs " + labelB +
                                    " -- the gluon-vs-quark origin."),
                                  L("tab:cmp-initparton")),
                     "initiating parton", labelA, labelB,
                     dA.initparton, dB.initparton);
  doc.addText(L(
    "Two caveats on reading Tab.~\\ref{tab:cmp-initparton}.  First, the "
    "initiating parton here is the INITIAL-STATE parton extracted from the "
    "proton -- the topmost parton in the hadron's lineage, the one whose own "
    "parents are the beams -- i.e. \"did this hadron's lineage start from a "
    "gluon or a quark?\".  It is deliberately defined this way because it is "
    "recoverable identically in both generators (it needs only the graph "
    "topology, not generator-specific status codes), which is what makes the "
    "comparison fair.  It is NOT the same as the outgoing gluon-jet-vs-quark-"
    "jet origin (the parton AFTER the hard scatter); that requires the "
    "hard-process tag the Pythia status codes carry but the HepMC graph does "
    "not, so it is reported only in the individual Pythia report.  Second, the "
    "robust, model-independent statement is the WITHIN-generator one: a large "
    "fraction of pions originate from a gluon in both generators (it is NOT "
    "zero -- the zero in the string-endpoint table is a definitional artefact, "
    "not physics).  The exact CROSS-generator magnitude carries some "
    "dependence on how each generator structures its initial-state / "
    "beam-remnant graph, so the gap between the two columns should be read as "
    "indicative rather than a precise model discriminant."));
  doc.endSection();

  // ---- pair-origin comparison ------------------------------------------
  doc.addSection(L("Pair-origin comparison"), L("sec:pairorigin"));
  doc.addText(L(
    "The two-particle question is sharper: of the same-event pair "
    "correlation, what fraction is ancestry-driven, and of what kind?  "
    "Tab.~\\ref{tab:cmp-pair} gives the breakdown for both generators.  "
    "SharedParton measures correlation inherited from a common parton "
    "(jet- or string-like); SameResonance and SameWeakParent measure "
    "decay-induced correlation; Unrelated is the combinatorial floor.  "
    "Comparing the SharedParton fraction between generators asks whether "
    "the string and cluster models seed the partonic correlation "
    "differently."));
  if (!dA.pairs.empty() || !dB.pairs.empty())
    fillCompareTable(doc.addTable(L("Same-event pairs by ancestry, " + labelA +
                                    " vs " + labelB + "."),
                                  L("tab:cmp-pair")),
                     "pair class", labelA, labelB, dA.pairs, dB.pairs);
  doc.endSection();

  // ---- comparison figures (each overlays both generators) --------------
  doc.addSection(L("Comparison figures"), L("sec:cmpfigs"));
  doc.addText(L(
    "Each figure overlays the same observable for both generators, "
    "normalised so shape differences are visible at a glance.  Origin and "
    "parton-flavour spectra come first, then the pair correlations, then "
    "the single-plot genealogy summaries (decay-chain depth, parton-ancestor "
    "count, common-ancestor stage, multiplicity, spherocity)."));
  renderFiguresPaired(doc, dA.figures, isCompareFigure);
  doc.endSection();

  // ---- configuration ----------------------------------------------------
  doc.addSection(L("Run configuration"), L("sec:cmpconfig"));
  doc.addText(L(
    labelA + ": " + (dA.events.empty() ? "?" : dA.events) + " events at "
    "$\\sqrt{s}=" + (dA.ecm.empty() ? "?" : dA.ecm) + "$ GeV.  " +
    labelB + ": " + (dB.events.empty() ? "?" : dB.events) + " events at "
    "$\\sqrt{s}=" + (dB.ecm.empty() ? "?" : dB.ecm) + "$ GeV.  Both runs use "
    "the same provenance tagger, species selection, and acceptance window."));
  doc.endSection();
}

// ===========================================================================
//  Mechanism-ladder comparison  —  ONE report with one column per
//  (generator, mechanism-rung), e.g. Pythia/Herwig x {baseline, +MPI, +MPI+CR}.
//  Reading a row left-to-right shows how a provenance fraction responds as MPI
//  and Colour Reconnection are switched on, in each generator.
//  `cols` is the ordered list of (column-label, parsed summary).  `figs` owns
//  the overlay figures (compare_*.png, which overlay every column).
// ===========================================================================
void buildLadderComparison(
    LatexDocument & doc,
    const std::vector<std::pair<std::string, ReportData>> & cols,
    const ReportData & figs)
{
  doc.addPackage("graphicx", "");
  doc.addPackage("float",    "");
  doc.addPackage("hyperref", "");

  std::vector<std::string>              labels;
  std::vector<std::vector<ClassRow>>    origin, parton, initp, pairs;
  for (const auto & c : cols)
    {
    labels.push_back(c.first);
    origin.push_back(c.second.origin);
    parton.push_back(c.second.parton);
    initp .push_back(c.second.initparton);
    pairs .push_back(c.second.pairs);
    }

  doc.addAbstract(L(
    "The provenance analysis is repeated as the two principal soft-QCD "
    "mechanisms -- multiparton interactions (MPI) and colour reconnection "
    "(CR) -- are switched on one at a time, in BOTH generators.  Each column "
    "is one (generator, mechanism-rung) configuration; reading a row "
    "left-to-right isolates the effect of each mechanism on where the final "
    "pions come from.  The same generator-agnostic tagger is applied "
    "throughout, so the columns are directly comparable."));

  doc.addSection(L("How to read this ladder"), L("sec:howto"));
  doc.addText(L(
    "Every column is the identical provenance analysis on a different "
    "mechanism configuration.  The rungs are cumulative: 'baseline' has MPI "
    "and CR OFF, '+MPI' turns MPI on, '+MPI+CR' turns both on.  Compare a "
    "row across a generator's three columns to read off what MPI then CR do "
    "to that fraction; compare the two generators' blocks to see whether the "
    "mechanism acts the same way in the string and cluster models.  A '--' "
    "cell means that class is absent in that configuration."));
  doc.endSection();

  doc.addSection(L("Genealogy across the mechanism ladder"), L("sec:ladder"));
  doc.addText(L(
    "Production-origin fractions across every configuration.  Watch the "
    "Primary row (direct hadronization) and the feed-down rows shift as MPI "
    "adds soft production and CR rearranges the colour flow."));
  fillMultiTable(doc.addTable(L("Production origin (\\%) across the mechanism "
                                "ladder."), L("tab:lad-origin")),
                 "origin class", labels, origin);

  doc.addText(L(
    "Initiating-parton flavour -- the gluon-vs-quark origin.  The gluon "
    "fraction is expected to grow sharply once MPI is on (MPI is a gluon-rich "
    "phenomenon)."));
  fillMultiTable(doc.addTable(L("Initiating (hard-scatter) parton flavour "
                                "(\\%) across the ladder."),
                              L("tab:lad-initparton")),
                 "initiating parton", labels, initp);

  doc.addText(L(
    "String-endpoint parton flavour (gluon-free by construction in both "
    "generators -- see the genealogy report for the distinction)."));
  fillMultiTable(doc.addTable(L("String-endpoint parton flavour (\\%) across "
                                "the ladder."), L("tab:lad-parton")),
                 "endpoint parton", labels, parton);

  doc.addText(L(
    "Two-particle ancestry.  The SharedParton fraction typically HALVES when "
    "MPI turns on (the partonic correlation is diluted across multiple "
    "scatters); CR then redistributes it further."));
  fillMultiTable(doc.addTable(L("Same-event pair ancestry (\\%) across the "
                                "ladder."), L("tab:lad-pair")),
                 "pair class", labels, pairs);
  doc.endSection();

  doc.addSection(L("Ladder overlay figures"), L("sec:ladfigs"));
  doc.addText(L(
    "Each figure overlays every (generator, rung) configuration for one "
    "observable, normalised so shape changes are visible.  Curves are "
    "coloured by generator; rungs of the same generator share a colour "
    "family."));
  renderFiguresPaired(doc, figs.figures, isCompareFigure);
  doc.endSection();
}

// ===========================================================================
//  Presentation mode  —  a 'beamer' deck.  addFrame does not change the
//  current scope, so we enter / leave each frame explicitly.
// ===========================================================================
void buildPresentation(LatexDocument & doc, const ReportData & d)
{
  doc.addPackage("graphicx", "");
  doc.addPackage("float",    "");   // enables the [H] placement specifier

  {
  LatexFrame & f = doc.addFrame(L("Motivation"));
  doc.setCurrentScope(&f);
  doc.addText(L(
    "Pion observables mix direct hadronization, resonance feed-down and "
    "weak decays.  We trace every hadron back to its parent partons to "
    "separate these contributions."));
  doc.setCurrentScope(&doc);
  }

  {
  LatexFrame & f = doc.addFrame(L("Run configuration"));
  doc.setCurrentScope(&f);
  fillConfigTable(doc.addTable(L(""), L("")), d);
  doc.setCurrentScope(&doc);
  }

  if (!d.origin.empty())
    {
    LatexFrame & f = doc.addFrame(L("Yield by production origin"));
    doc.setCurrentScope(&f);
    fillClassTable(doc.addTable(L(""), L("")), "origin class", d.origin);
    doc.setCurrentScope(&doc);
    }

  if (!d.pairs.empty())
    {
    LatexFrame & f = doc.addFrame(L("Two-particle correlation by ancestry"));
    doc.setCurrentScope(&f);
    fillClassTable(doc.addTable(L(""), L("")), "pair class", d.pairs);
    doc.setCurrentScope(&doc);
    }

  for (std::size_t i = 0; i < d.figures.size(); ++i)
    {
    LatexFrame & f = doc.addFrame(L("Figure"));
    doc.setCurrentScope(&f);
    doc.addFigure(L(d.figures[i].first), L(""), L(texEscape(d.figures[i].second)));
    doc.setCurrentScope(&doc);
    }

  if (!d.ladderRows.empty())
    {
    LatexFrame & f = doc.addFrame(L("Mechanism ladder"));
    doc.setCurrentScope(&f);
    fillLadderTable(doc.addTable(L(""), L("")), d);
    doc.setCurrentScope(&doc);
    }
}

} // namespace

// ===========================================================================
int main(int argc, char ** argv)
{
  std::string mode    = "paper";
  std::string summary, ladder, figures;
  std::string summary2, label1 = "Pythia 8", label2 = "Herwig 7";
  std::string title   = "Provenance Decomposition of Pion Production in Pythia 8";
  std::string author  = "CAP parton-tracking";
  std::string email   = "";
  std::string affil   = "Wayne State University";
  std::string outdir  = "provenance/reports";
  std::string outName = "provenance-report";
  std::string pythiaConfig, ladderDir, gitShaArg, hostnameArg;
  std::string summariesArg;   // "label1=path1;label2=path2;..." -> ladder mode
  bool runPdf = false;

  for (int i = 1; i < argc; ++i)
    {
    std::string a = argv[i];
    std::string nextv;
    bool hasNext = (i + 1 < argc);
    if (hasNext) nextv = argv[i + 1];
    if      (a == "--mode"        && hasNext) { mode    = nextv; ++i; }
    else if (a == "--summary"     && hasNext) { summary = nextv; ++i; }
    else if (a == "--summary2"    && hasNext) { summary2 = nextv; ++i; }
    else if (a == "--summaries"   && hasNext) { summariesArg = nextv; ++i; }
    else if (a == "--label1"      && hasNext) { label1  = nextv; ++i; }
    else if (a == "--label2"      && hasNext) { label2  = nextv; ++i; }
    else if (a == "--ladder"      && hasNext) { ladder  = nextv; ++i; }
    else if (a == "--figures"     && hasNext) { figures = nextv; ++i; }
    else if (a == "--title"       && hasNext) { title   = nextv; ++i; }
    else if (a == "--author"      && hasNext) { author  = nextv; ++i; }
    else if (a == "--email"       && hasNext) { email   = nextv; ++i; }
    else if (a == "--affiliation" && hasNext) { affil   = nextv; ++i; }
    else if (a == "--outdir"      && hasNext) { outdir  = nextv; ++i; }
    else if (a == "--out"         && hasNext) { outName = nextv; ++i; }
    else if (a == "--config"      && hasNext) { pythiaConfig = nextv; ++i; }
    else if (a == "--ladder-dir"  && hasNext) { ladderDir    = nextv; ++i; }
    else if (a == "--git-sha"     && hasNext) { gitShaArg    = nextv; ++i; }
    else if (a == "--hostname"    && hasNext) { hostnameArg  = nextv; ++i; }
    else if (a == "--pdf")                    { runPdf  = true; }
    else if (a == "-h" || a == "--help")
      {
      std::cout <<
        "Usage: provenance-report --summary FILE [options]\n"
        "  --mode paper|presentation   document type (default paper)\n"
        "  --summary FILE              provenance-study .root.txt summary\n"
        "  --summary2 FILE             second generator's summary -> COMPARISON\n"
        "                              report (genealogy/pair tables + overlay\n"
        "                              figures for both generators)\n"
        "  --label1 NAME --label2 NAME generator labels (default Pythia 8 /\n"
        "                              Herwig 7)\n"
        "  --summaries \"L1=p1;L2=p2;...\"  mechanism-LADDER report: one column\n"
        "                              per (generator,rung) summary, e.g.\n"
        "                              \"Pythia base=a.txt;Pythia +MPI=b.txt;...\"\n"
        "  --ladder FILE               ladder-comparison.csv (optional)\n"
        "  --figures DIR               cap-provenance-plot output dir (optional)\n"
        "  --config FILE               Pythia .cmnd used in the standalone run\n"
        "                              (embedded in the reproducibility appendix)\n"
        "  --ladder-dir DIR            directory holding the ladder rung .cmnd\n"
        "                              files (one per rung, embedded in appendix)\n"
        "  --git-sha SHA               override (default: 'git rev-parse HEAD')\n"
        "  --hostname HOST             override (default: gethostname / hostname)\n"
        "  --title / --author / --email / --affiliation   metadata\n"
        "  --outdir DIR  --out NAME    output location (default ./provenance-report)\n"
        "  --pdf                       run pdflatex on the generated .tex\n";
      return 0;
      }
    else { std::cerr << "unknown option: " << a << "\n"; return 2; }
    }

  if (summary.empty() && summariesArg.empty())
    { std::cerr << "provenance-report: --summary (or --summaries) is required\n";
      return 2; }

  // ---- Mechanism-ladder mode: --summaries "label=path;label=path;..." -----
  // One column per (generator, rung).  Built and returned early; the rest of
  // main() is the single-/two-summary path.
  if (!summariesArg.empty())
    {
    std::vector<std::pair<std::string, ReportData>> cols;
    std::string item;
    std::istringstream ss(summariesArg);
    while (std::getline(ss, item, ';'))
      {
      if (trim(item).empty()) continue;
      std::string::size_type eq = item.find('=');
      if (eq == std::string::npos) continue;
      const std::string lab  = trim(item.substr(0, eq));
      const std::string path = trim(item.substr(eq + 1));
      ReportData rd;
      parseSummary(path, rd);
      cols.emplace_back(lab, rd);
      }
    if (cols.empty())
      { std::cerr << "provenance-report: --summaries parsed no columns\n";
        return 2; }
    ReportData figs;
    if (!figures.empty()) parseFigures(figures, figs);
    std::cout << "provenance-report — ladder mode, " << cols.size()
              << " column(s), figures=" << figs.figures.size() << "\n";

    LatexDocument ldoc;
    ldoc.setTitle(L(title));
    ldoc.addAuthor(L(author), L(email), L(affil));
    ldoc.setUseToday(true);
    ldoc.setOutputPath(L(outdir));
    ldoc.setOutFileName(L(outName));
    ldoc.setDocumentClassName("article");
    buildLadderComparison(ldoc, cols, figs);
    (void)std::system(("mkdir -p '" + outdir + "'").c_str());
    ldoc.create();
    std::cout << "  wrote " << outdir << "/" << outName << ".tex\n";
    if (runPdf)
      {
      std::string cmd = "cd '" + outdir + "' && pdflatex -interaction="
        "nonstopmode '" + outName + ".tex' > /dev/null 2>&1"
        " && pdflatex -interaction=nonstopmode '" + outName
        + ".tex' > /dev/null 2>&1";
      int rc = std::system(cmd.c_str());
      std::cout << (rc == 0
                    ? "  wrote " + outdir + "/" + outName + ".pdf\n"
                    : "  pdflatex returned nonzero — compile by hand\n");
      }
    return 0;
    }

  ReportData d;
  parseSummary(summary, d);
  if (!ladder.empty())  parseLadder(ladder, d);
  if (!figures.empty()) parseFigures(figures, d);

  // ---- reproducibility envelope ----------------------------------------
  // Pythia config — embedded verbatim into the appendix.
  if (!pythiaConfig.empty())
    {
    d.pythiaCmnd     = slurp(pythiaConfig);
    d.pythiaCmndPath = pythiaConfig;
    }
  // Ladder rung configs — slurp every .cmnd in the directory whose name
  // matches a rung listed in the comparison CSV.  Skip the catch-all
  // ladder.txt and any non-rung files.
  if (!ladderDir.empty())
    {
    for (const auto & rung : d.ladderRungs)
      {
      // Match the script's filename convention: "+" -> "_", " " -> "_".
      std::string tag = rung;
      for (char & c : tag) { if (c == '+' || c == ' ') c = '_'; }
      const std::string path = ladderDir + "/" + tag + ".cmnd";
      const std::string body = slurp(path);
      if (!body.empty())
        d.rungConfigs.push_back({rung, path, body});
      }
    }
  // Git revision + hostname + date — query the runtime environment.
  d.gitSha     = gitShaArg.empty()
                 ? captureCommand("git rev-parse HEAD 2>/dev/null")
                 : gitShaArg;
  d.gitBranch  = captureCommand(
                   "git rev-parse --abbrev-ref HEAD 2>/dev/null");
  d.hostname   = hostnameArg.empty()
                 ? captureCommand("hostname 2>/dev/null")
                 : hostnameArg;
  d.reportDate = captureCommand("date '+%Y-%m-%d %H:%M %Z' 2>/dev/null");

  std::cout << "provenance-report — mode=" << mode << "\n"
            << "  origin rows=" << d.origin.size()
            << "  parton rows=" << d.parton.size()
            << "  pair rows="   << d.pairs.size()
            << "  ladder rows=" << d.ladderRows.size()
            << "  figures="     << d.figures.size() << "\n";

  LatexDocument doc;
  doc.setTitle(L(title));
  doc.addAuthor(L(author), L(email), L(affil));
  doc.setUseToday(true);
  doc.setOutputPath(L(outdir));
  doc.setOutFileName(L(outName));

  // Comparison mode: a second summary switches the whole document into a
  // cross-generator comparison (genealogy + pair tables in adjacent columns,
  // overlay figures).  The per-generator individual reports are produced by
  // separate invocations with a single --summary.
  ReportData d2;
  const bool comparison = !summary2.empty();
  if (comparison)
    {
    parseSummary(summary2, d2);
    std::cout << "  comparison: " << label1 << " vs " << label2
              << "  (gen2 origin=" << d2.origin.size()
              << " parton=" << d2.parton.size()
              << " pair=" << d2.pairs.size() << ")\n";
    }

  if (comparison)
    {
    doc.setDocumentClassName("article");
    buildComparison(doc, d, d2, label1, label2);
    }
  else if (mode == "presentation")
    {
    doc.setDocumentClassName("beamer");
    doc.setThemeName("Madrid");
    buildPresentation(doc, d);
    }
  else
    {
    doc.setDocumentClassName("article");
    buildPaper(doc, d);
    }

  // Ensure the output directory exists before LatexDocument tries to write to it.
  (void)std::system(("mkdir -p '" + outdir + "'").c_str());
  doc.create();
  std::cout << "  wrote " << outdir << "/" << outName << ".tex\n";

  if (runPdf)
    {
    std::string cmd = "cd '" + outdir + "' && pdflatex -interaction=nonstopmode '"
                      + outName + ".tex' > /dev/null 2>&1"
                      " && pdflatex -interaction=nonstopmode '"
                      + outName + ".tex' > /dev/null 2>&1";
    int rc = std::system(cmd.c_str());
    if (rc == 0) std::cout << "  wrote " << outdir << "/" << outName << ".pdf\n";
    else         std::cout << "  pdflatex returned " << rc
                           << " — compile " << outName << ".tex by hand\n";
    }
  return 0;
}
