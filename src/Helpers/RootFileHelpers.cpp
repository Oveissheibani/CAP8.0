#include "Exceptions.hpp"
#include "PrintHelpers.hpp"
#include "RootFileHelpers.hpp"
#include "MathConstants.hpp"
#include "MathBasicFunctions.hpp"
#include "TROOT.h"
#include "TSystem.h"
#include "TParameter.h"
#include "TSystemDirectory.h"
#include "TRandom.h"

namespace CAP
{

//!
//!Open Root file at the given path and with the given file name
//!ioOption : OLD, NEW, RECREATE
//
TFile * openRootFile(const String & path,
                     const String & name,
                     const String & ioOption,
                     bool verbose)
{
  String fileName;
  if (name.BeginsWith("/"))
    fileName = name;
  else
    {
    String filePath = path;
    int slash = filePath.First('/');
    int len   = filePath.Length();
    if (len>0  &&  (len-1)!=slash) filePath += "/";
    fileName = filePath;
    fileName += name;
    }
  if (!fileName.EndsWith(".root")) fileName += ".root";
  if (verbose)
    {
    printCR();
    printValue("openRootFile(...) Opening file",fileName);
    printValue("ioOption",ioOption);
    }
  TFile * file = new TFile(fileName,ioOption);
  if (!file) throw  FileException(fileName,"File not found",__FUNCTION__);
  if (!file->IsOpen())  throw  FileException(fileName,"File not found/opened",__FUNCTION__);
  if (verbose) std::cout << "openRootFile(...) File opened successfully." << std::endl;
  return file;
}

TFile * openRootFile(const String & name,
                     const String & ioOption,
                     bool verbose)
{
  String fileName = name;
  if (!fileName.EndsWith(".root")) fileName += ".root";
  if (verbose)
    {
    printCR();
    printValue("openRootFile(...) Opening file",fileName);
    printValue("ioOption",ioOption);
    }
  TFile * file = new TFile(fileName,ioOption);
  if (!file) throw  FileException(fileName,"File not found","openRootFile()");
  if (!file->IsOpen())  throw  FileException(fileName,"File not found/opened",__FUNCTION__);
  if (verbose)
    {
    printCR();
    printValue("openRootFile(...) File opened successfully",fileName);
    }
  return file;
}

TFile * openOldRootFile(const String & name,
                        bool verbose)
{
  return openRootFile(name,"OLD",verbose);
}

TFile * openOldRootFile(const String & path,
                        const String & name,
                        bool verbose)
{
  return openRootFile(path,name,"OLD",verbose);
}

TFile * openNewRootFile(const String & name,
                        bool verbose)
{
  return openRootFile(name,"NEW",verbose);
}

TFile * openNewRootFile(const String & path,
                        const String & name,
                        bool verbose)
{
  return openRootFile(path,name,"NEW",verbose);
}

TFile * openRecreateRootFile(const String & name,
                             bool verbose)
{
  return openRootFile(name,"RECREATE",verbose);
}

TFile * openRecreateRootFile(const String & path,
                             const String & name,
                             bool verbose)
{
  return openRootFile(path,name,"RECREATE",verbose);
}

std::vector<TFile*> openRootFiles(std::vector<String> & names,
                                  const String  & ioOption,
                                  bool verbose)
{
  int nFiles = names.size();
  if (nFiles<1)  throw FileException("Empty file list","File not found/opened",__FUNCTION__);
  if (verbose)
    {
    printCR();
    printValue("openRootFiles(...) Opening N files",names.size());
    }
  std::vector<TFile*> files;
  for (auto & name : names)
    {
    files.push_back( openRootFile(name,ioOption,verbose));
    }
  if (verbose)
    {
    printCR();
    printString("openRootFiles(...) Completed successfully");
    }
  return files;
}

std::vector<TFile*> openRootFiles(const String & path,
                                  std::vector<String> & names,
                                  const String  & ioOption,
                                  bool verbose)
{
  int nFiles = names.size();
  if (nFiles<1)  throw  FileException("Empty file list","File not found/opened",__FUNCTION__);
  if (verbose)
    {
    printCR();
    printValue("openRootFiles(...) Opening N files",names.size());
    printValue("openRootFiles(...) Path",path);
    }
  std::vector<TFile*> files;
  for (auto & name : names)
    {
    files.push_back( openRootFile(path,name,ioOption,verbose));
    }
  if (verbose)  std::cout << "openRootInputFiles(...) Completed successfully" << std::endl;
  return files;
}

std::vector<TString> listDirsIn(const TString & pathname,
                                bool verbose)
{
  TString directoryName = pathname;
  std::vector<TString>  subdirectories;
  if (!directoryName.EndsWith("/")) directoryName += "/";
  if (verbose) printValue("listDirsIn() Search path",pathname);
  TSystemDirectory topDirectory(directoryName, directoryName);
  TList *files = topDirectory.GetListOfFiles();
  if (!files) throw FileException("TList* is a nullpointer","File not found/opened",__FUNCTION__);
  TSystemFile *file;
  TString fname;
  TIter next(files);
  while ((file=(TSystemFile*) next()) )
    {
    fname = file->GetName();
    if (file->IsDirectory()   &&  !fname.BeginsWith(".")  ) subdirectories.push_back(fname);
    }
  return subdirectories;
}


std::vector<String> listFilesInDir(const String & pathname,
                                   const String & ext,
                                   bool prependPath,
                                   bool verbose,
                                   int  maximumDepth,
                                   int  currentDepth)
{
  String dirname = pathname;
  int depth = currentDepth;
  if (!dirname.EndsWith("/")) dirname += "/";
  if (verbose)
    {
    printCR();
    printString("listFilesInDir() Searching files");
    printValue("listFilesInDir() MaximumDepth:",maximumDepth);
    printValue("listFilesInDir() CurrentDepth:",depth);
    }
  TSystemDirectory dir(dirname, dirname);
  TList *files = dir.GetListOfFiles();
  std::vector<String>  fileNames;
  std::vector<String>  subdirs;
  if (!files) throw FileException("Null TList","No files found", __FUNCTION__);
  TSystemFile *file;
  String fname;
  TIter next(files);
  while ((file=(TSystemFile*)next()))
    {
    fname = file->GetName();
    if (file->IsDirectory()  &&  !fname.BeginsWith(".") )
      {
      subdirs.push_back(fname);
      }
    else if (fname.EndsWith(ext))
      {
      if (prependPath)
        fileNames.push_back(dirname+fname);
      else
        fileNames.push_back(fname);
      //std::cout << fname.Data() << std::endl;
      //if (verbose) std::cout << fname << std::endl;
      }
    }

  int nSubdirs = subdirs.size();
  ++depth;
  if (nSubdirs>0  &&  depth<=maximumDepth)
    {
    for (int iDir=0; iDir<nSubdirs; iDir++)
      {
      std::vector<String> additionalFiles;
      String  subdirname = dirname;
      subdirname += "/";
      subdirname += subdirs[iDir];
      subdirname += "/";
      additionalFiles = listFilesInDir(subdirname,ext,prependPath,verbose,maximumDepth,depth);
      int nAdditionalfiles = additionalFiles.size();
      for (int iFile=0;iFile<nAdditionalfiles;iFile++)
        fileNames.push_back(additionalFiles[iFile]);
      }
    }
  if (verbose)
    {
    printCR();
    printValue("listFilesInDir() Number of files found",fileNames.size());
    for (Size_t k=0; k<fileNames.size();k++)
      {
      String s = "File "; s += k;
      printValue(s,fileNames[k]);
      }
    printString("Returning up one level.... ");
    }
  return fileNames;
}

//!
//! Get file  names at the given location that match all  patterns in includePatterns and exclude patterns in excludePatterns
//!
std::vector<String>  listFilesInDir(const String & pathName,
                                    const std::vector<String> & includePatterns,
                                    const std::vector<String> & excludePatterns,
                                    bool prependPath,
                                    bool verbose,
                                    int  maximumDepth,
                                    int  currentDepth)
{
  std::vector<String> outputList;
  std::vector<String> fileList = listFilesInDir(pathName,".root",prependPath,verbose,maximumDepth,currentDepth);
  unsigned int nNames = fileList.size();
  if (verbose)
    {
    printCR();
    printValue("nNames",nNames);
    printValue("nIncludes",includePatterns.size());
    printValue("nExcludes",excludePatterns.size());
    }

  for (auto & name : fileList)
    {
    bool include = true;
    for (auto & pattern : includePatterns)
      if (!name.Contains(pattern)) { include = false; break;}
    if (!include) continue;
    for (auto & pattern : excludePatterns)
      if (!name.Contains(pattern)) { include = false; break;}
    if (include)
      {
      if (name.Contains(".root"))
        {
        int dot = name.First('.');
        int len = name.Length();
        if (dot==len-5 )
          name.Remove(dot,len-dot);
        }
      //String check = pathName+name;
      //std::cout << " CHECK:::: " << check << std::endl;
      outputList.push_back(name);
      }
    }
  return outputList;
}

String removeRootExtension(const String fileName)
{
  String name = fileName;
  int dot = name.Last('.');
  int len = name.Length();
  if (dot==len-5 ) name.Remove(dot,len-dot);
  return name;
}

void exportParameter(TFile & outputFile,
                     const String & parameterName,
                     long value,
                     bool verbose)
{
  if (verbose)
    {
    printCR();
    printValue("exportParameter() output file",outputFile.GetName());
    printValue("exportParameter() parameterName",parameterName);
    printValue("exportParameter() value",value);
    }
  outputFile.cd();
  TParameter<Long64_t>(parameterName,value,'+').Write();
}

long importParameter(TFile & inputFile,
                     const String & parameterName,
                     bool verbose)
{
  TParameter<Long64_t> *par = (TParameter<Long64_t> *) inputFile.Get(parameterName);
  if (!par)
    {
    if (verbose)
      {
      printCR();
      printValue("importParameter() input file",inputFile.GetName());
      printValue("importParameter() Property not found:",parameterName);
      }
    throw FileException(parameterName,"Property not found",__FUNCTION__);
    }
  double value = par->GetVal();
  delete par;
  if (verbose)
    {
    printCR();
    printValue("importParameter() parameterName",parameterName);
    printValue("importParameter() value",value);
    }
  return value;
}

void exportAccountant(Accountant & accountant, TFile & outputFile, bool verbose)
{
  if (verbose) printString("Accountant::exportTo(...)");
  unsigned int size = accountant.size();
  String className  = "Accountant";
  String objectName = accountant.name();
  String outputName;

  // output the number of elements
  outputName = className;
  outputName = "_";
  outputName = objectName;
  outputName = "_N";
  if (verbose) printValue(outputName,size);
  exportParameter(outputFile,outputName,size);

  unsigned int k = 0;
  for (auto value : accountant.values())
    {
    outputName = className;
    outputName = "_";
    outputName = objectName;
    outputName = "_";
    outputName += k;
    if (verbose) printValue(outputName,value);
    exportParameter(outputFile,outputName,value);
    k++;
    }
}

void importAccountant(Accountant & accountant, TFile & inputFile, bool verbose)
{
  if (verbose) printString("Accountant::importFrom(...)");
  unsigned int size = accountant.size();;
  unsigned int n;
  String className  = "Accountant";
  String objectName = accountant.name();
  String inputName;

  if (size==0) // need to read the size first
    {
    inputName = className;
    inputName = "_";
    inputName = objectName;
    inputName = "_N";
    n = importParameter(inputFile,inputName);
    if (verbose) printValue(inputName,n);
    }
  else
    {
    n = size;
    }
  for (unsigned int k = 0; k<size; k++)
    {
    inputName = className;
    inputName = "_";
    inputName = objectName;
    inputName = "_";
    inputName = k;
    long value = importParameter(inputFile,inputName);
    if (verbose) printValue(inputName,value);
    if (size==0)
      accountant.values().push_back(value);
    else
      accountant.setValueAt(k,value);
    }
}

String makeFileName(const String & inputPath,
                         const String & fileName,
                         const String & extension)
{
  String inputFileName = inputPath;
  // make sure that if an inputPath is given, it ends with '/'
  int slash = inputFileName.First('/');
  int len   = inputFileName.Length();
  //  std::cout << slash << std::endl;
  //  std::cout << len << std::endl;
  if (len>0  &&  (len-1)!=slash) inputFileName += "/";
  inputFileName += fileName;
  if (!inputFileName.EndsWith(extension)) inputFileName += extension;
  return inputFileName;
}

void createDirectory(const String path,
                     bool verbose)
{
  if (verbose)
    {
    printCR();
    printValue("createDirectory(const String path) Creating directory (if non-existing)",path);
    }
  gSystem->mkdir(path,1);
}

String substitute(const String & inputString,
                  const String & subString,
                  const String & newSubString)
{
  String outputString(inputString);
  outputString.ReplaceAll(subString,newSubString);
  return outputString;
}





} // namespace CAP
