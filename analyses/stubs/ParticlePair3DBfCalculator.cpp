#include "ParticlePair3DBfCalculator.hpp"
#include "PrintHelpers.hpp"
#include "TFile.h"
#include <cstdlib>
ClassImp(CAP::ParticlePair3DBfCalculator);
namespace CAP {
ParticlePair3DBfCalculator::ParticlePair3DBfCalculator() : Task() {
  appendClassName("ParticlePair3DBfCalculator");
  setMinimumReportLevel(Object::kInfo);
  setName("ParticlePair3DBfCalculator"); setTitle("ParticlePair3DBfCalculator");
}
ParticlePair3DBfCalculator::ParticlePair3DBfCalculator(const ParticlePair3DBfCalculator & s) : Task(s) {}
ParticlePair3DBfCalculator & ParticlePair3DBfCalculator::operator=(const ParticlePair3DBfCalculator & r) {
  if (this != &r) Task::operator=(r); return *this;
}
void ParticlePair3DBfCalculator::execute() {
  _taskExecuted.increment();
  String inFile  = _configuration.valueString(name() + ":HistogramsImportFile");
  String outFile = _configuration.valueString(name() + ":HistogramsExportFile");
  if (inFile.IsNull() || outFile.IsNull() ||
      inFile == "NONE" || outFile == "NONE") return;
  const char * importDir = std::getenv("CAP_HISTOS_IMPORT_PATH");
  const char * exportDir = std::getenv("CAP_HISTOS_EXPORT_PATH");
  if (!importDir || !exportDir) return;
  String fullIn  = String(importDir) + inFile;
  String fullOut = String(exportDir) + outFile;
  printCR();
  printValue("ParticlePair3DBfCalculator: copy",
             String(fullIn) + " -> " + fullOut);
  if (!TFile::Cp(fullIn.Data(), fullOut.Data(), false)) {
    printString("ParticlePair3DBfCalculator: TFile::Cp FAILED");
  }
}
}
