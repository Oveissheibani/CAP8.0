#ifndef CAP_RootFileHelpers
#define CAP_RootFileHelpers
#include "Aliases.hpp"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TLegend.h"
#include "TLine.h"
#include "TArrow.h"
#include "TLatex.h"
#include "Accountant.hpp"

namespace CAP
{
TFile * openRootFile(const String & path, const String & name,const String & ioOption,bool verbose=0);
TFile * openRootFile(const String & name, const String & ioOption,bool verbose=0);
TFile * openOldRootFile(const String & path, const String & name,bool verbose=0);
TFile * openOldRootFile(const String & name,bool verbose=0);
TFile * openNewRootFile(const String & path, const String & name,bool verbose=0);
TFile * openNewRootFile(const String & name,bool verbose=0);
TFile * openRecreateRootFile(const String & path, const String & name,bool verbose=0);
TFile * openRecreateRootFile(const String & name,bool verbose=0);
std::vector<TFile*> openRootFiles(std::vector<String> & names, const String  & ioOption,bool verbose=0);
std::vector<TFile*> openRootFiles(const String & path,std::vector<String> & names,const String  & ioOption,bool verbose=0);

std::vector<String> listDirsIn(const String & pathname,bool verbose=false);
std::vector<String> listFilesInDir(const String & pathname,
                                   const String & ext,
                                   bool prependPath=1,
                                   bool verbose=0,
                                   int  maximumDepth=1,
                                   int  currentDepth=0);

//!
//! Get file  names at the given location that match all  patterns in includePatterns and exclude patterns in excludePatterns
//!
std::vector<String>  listFilesInDir(const String & pathName,
                                    const std::vector<String> & includePatterns,
                                    const std::vector<String> & excludePatterns,
                                    bool prependPath=1,
                                    bool verbose=0,
                                    int  maximumDepth=1,
                                    int  currentDepth=0);

String removeRootExtension(const String fileName);
long importParameter(TFile & inputFile,
                     const String & parameterName,
                     bool verbose=0);
void exportParameter(TFile & outputFile,
                     const String & parameterName,
                     long value,
                     bool verbose=0);

void exportAccountant(Accountant & account, TFile & outputFile, bool verbose=0);
void importAccountant(Accountant & account, TFile & inputFile, bool verbose=0);

String makeFileName(const String & inputPath,
                    const String & fileName,
                    const String & extension);
void createDirectory(const String path, bool verbose=false);

String  substitute(const String & inputString, const String & subString, const String & newSubString);

} // namespace CAP

#endif
