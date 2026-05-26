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

#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
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

// ---- parsed report data ---------------------------------------------------
struct ClassRow { std::string name, count, pct; };

struct ReportData
{
  std::string events, ecm, process, seed, species;
  std::vector<ClassRow> origin, parton, pairs;
  std::vector<std::string>              ladderRungs;   // column headers
  std::vector<std::vector<std::string>> ladderRows;    // group,obs,vals...
  std::vector<std::pair<std::string,std::string>> figures;  // (path, caption)
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
    // section markers
    if (line.find("by origin:")                   != std::string::npos)
      { section = "origin"; continue; }
    if (line.find("by parton flavour:")           != std::string::npos)
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
        if      (section == "origin") d.origin.push_back(r);
        else if (section == "parton") d.parton.push_back(r);
        else if (section == "pair")   d.pairs.push_back(r);
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

void fillConfigTable(LatexTable & t, const ReportData & d)
{
  t.setColumnSpec("l l");
  t.setHeaderRows(1);
  t.addRow({ L("parameter"), L("value") });
  t.addRow({ L("events"),         L(d.events.empty()  ? "?" : d.events) });
  t.addRow({ L("$\\sqrt{s}$"),    L((d.ecm.empty() ? "?" : d.ecm) + " GeV") });
  t.addRow({ L("QCD process"),    L(d.process.empty() ? "?" : d.process) });
  t.addRow({ L("random seed"),    L(d.seed.empty()    ? "?" : d.seed) });
  t.addRow({ L("species (|pdg|)"),L(d.species.empty() ? "all" : d.species) });
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

// ===========================================================================
//  Paper mode  —  an 'article' document.
// ===========================================================================
void buildPaper(LatexDocument & doc, const ReportData & d)
{
  doc.addPackage("graphicx", "");
  doc.addPackage("float",    "");   // enables the [H] placement specifier
  doc.addAbstract(L(
    "This report presents a provenance decomposition of pion production in "
    "Pythia 8.  Every final-state hadron is traced through a generator-"
    "agnostic event-history graph back to its parent partons, separating the "
    "yield and the two-particle correlation into the parts that originate in "
    "the partonic stage, in hadronization, and in subsequent decays."));

  doc.addSection(L("Introduction"), L("sec:intro"));
  doc.addText(L(
    "Final-state hadron observables mix several physical origins: direct "
    "hadronization products, resonance feed-down, and weak-decay products.  "
    "This analysis attaches a full provenance record to every hadron so that "
    "each contribution can be measured separately."));
  doc.endSection();

  doc.addSection(L("Method"), L("sec:method"));
  doc.addText(L(
    "For every event the Pythia record is converted into an EventHistory "
    "directed graph.  A ProvenanceTagger walks each final hadron back up the "
    "graph, recording its production stage, its parent hadron (and whether "
    "that parent is a short-lived resonance), and its ancestor partons.  "
    "Single-particle spectra and two-particle correlations are then "
    "accumulated, decomposed by those provenance classes."));
  doc.endSection();

  doc.addSection(L("Configuration and binning"), L("sec:config"));
  fillConfigTable(doc.addTable(L("Monte-Carlo run configuration."),
                               L("tab:config")), d);
  fillBinningTable(doc.addTable(L("Analysis binning."), L("tab:binning")));
  doc.endSection();

  doc.addSection(L("Single-particle results"), L("sec:single"));
  if (!d.origin.empty())
    fillClassTable(doc.addTable(L("Pion yield by production origin."),
                                L("tab:origin")), "origin class", d.origin);
  if (!d.parton.empty())
    fillClassTable(doc.addTable(L("Pion yield by ancestor-parton flavour."),
                                L("tab:parton")), "parton class", d.parton);
  // Single-particle section catches anything that is not a pair / ladder
  // figure — origin, parton, n_parton_ancestors, future single-particle.
  for (std::size_t i = 0; i < d.figures.size(); ++i)
    if (!figIs(d.figures[i].first, "pair") &&
        !figIs(d.figures[i].first, "ladder"))
      doc.addFigure(L(d.figures[i].first), L(""), L(texEscape(d.figures[i].second)));
  doc.endSection();

  doc.addSection(L("Two-particle results"), L("sec:pair"));
  if (!d.pairs.empty())
    fillClassTable(doc.addTable(L("Same-event pion pairs by ancestry."),
                                L("tab:pair")), "pair class", d.pairs);
  for (std::size_t i = 0; i < d.figures.size(); ++i)
    if (figIs(d.figures[i].first, "pair"))
      doc.addFigure(L(d.figures[i].first), L(""), L(texEscape(d.figures[i].second)));
  doc.endSection();

  if (!d.ladderRows.empty())
    {
    doc.addSection(L("Mechanism ladder"), L("sec:ladder"));
    doc.addText(L(
      "The same analysis is repeated as Pythia mechanisms are switched on "
      "one at a time (shower, MPI, colour reconnection, rope hadronization), "
      "showing how each mechanism shifts the provenance breakdown."));
    fillLadderTable(doc.addTable(L("Provenance breakdown across the "
                                   "mechanism ladder (percentages)."),
                                 L("tab:ladder")), d);
    for (std::size_t i = 0; i < d.figures.size(); ++i)
      if (figIs(d.figures[i].first, "ladder"))
        doc.addFigure(L(d.figures[i].first), L(""), L(texEscape(d.figures[i].second)));
    doc.endSection();
    }

  doc.addSection(L("Discussion"), L("sec:discussion"));
  doc.addText(L(
    "The resonance / weak-decay split depends on a curated resonance list; "
    "the primary fraction, taken directly from the production stage, is "
    "exact.  Sub-percent pair fractions carry statistical uncertainty that "
    "should be quantified before quotation."));
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
  std::string title   = "Provenance Decomposition of Pion Production in Pythia 8";
  std::string author  = "CAP parton-tracking";
  std::string email   = "";
  std::string affil   = "Wayne State University";
  std::string outdir  = "provenance/reports";
  std::string outName = "provenance-report";
  bool runPdf = false;

  for (int i = 1; i < argc; ++i)
    {
    std::string a = argv[i];
    std::string nextv;
    bool hasNext = (i + 1 < argc);
    if (hasNext) nextv = argv[i + 1];
    if      (a == "--mode"        && hasNext) { mode    = nextv; ++i; }
    else if (a == "--summary"     && hasNext) { summary = nextv; ++i; }
    else if (a == "--ladder"      && hasNext) { ladder  = nextv; ++i; }
    else if (a == "--figures"     && hasNext) { figures = nextv; ++i; }
    else if (a == "--title"       && hasNext) { title   = nextv; ++i; }
    else if (a == "--author"      && hasNext) { author  = nextv; ++i; }
    else if (a == "--email"       && hasNext) { email   = nextv; ++i; }
    else if (a == "--affiliation" && hasNext) { affil   = nextv; ++i; }
    else if (a == "--outdir"      && hasNext) { outdir  = nextv; ++i; }
    else if (a == "--out"         && hasNext) { outName = nextv; ++i; }
    else if (a == "--pdf")                    { runPdf  = true; }
    else if (a == "-h" || a == "--help")
      {
      std::cout <<
        "Usage: provenance-report --summary FILE [options]\n"
        "  --mode paper|presentation   document type (default paper)\n"
        "  --summary FILE              provenance-study .root.txt summary\n"
        "  --ladder FILE               ladder-comparison.csv (optional)\n"
        "  --figures DIR               cap-provenance-plot output dir (optional)\n"
        "  --title / --author / --email / --affiliation   metadata\n"
        "  --outdir DIR  --out NAME    output location (default ./provenance-report)\n"
        "  --pdf                       run pdflatex on the generated .tex\n";
      return 0;
      }
    else { std::cerr << "unknown option: " << a << "\n"; return 2; }
    }

  if (summary.empty())
    { std::cerr << "provenance-report: --summary is required\n"; return 2; }

  ReportData d;
  parseSummary(summary, d);
  if (!ladder.empty())  parseLadder(ladder, d);
  if (!figures.empty()) parseFigures(figures, d);

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

  if (mode == "presentation")
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
