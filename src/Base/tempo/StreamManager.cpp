/* **********************************************************************
 * Copyright (C) 2019-2024, Claude Pruneau, Victor Gonzalez     
 * All rights reserved.
 *
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 *
 * Author: Claude Pruneau,   04/01/2024
 *
 * *********************************************************************/
#include "StreamManager.hpp"

namespace CAP
{

StreamManager::StreamManager()
:
rootInputFiles(),
rootOutputFiles(),
streamInputFiles(),
streamOutputFiles(),
verbose(false)
{ /* no ops */ }

// shallow copies only
StreamManager::StreamManager(const StreamManager & fileManager)
:
rootInputFiles(fileManager.rootInputFiles),
rootOutputFiles(fileManager.rootOutputFiles),
streamInputFiles(fileManager.streamInputFiles),
streamOutputFiles(fileManager.streamOutputFiles),
verbose(fileManager.verbose)
{ /* no ops */ }


// shallow copies only
StreamManager StreamManager::operator=(const StreamManager & fileManager)
{
  if (this!=&fileManager)
    {
    rootInputFiles    = fileManager.rootInputFiles;
    rootOutputFiles   = fileManager.rootOutputFiles;
    streamInputFiles  = fileManager.streamInputFiles;
    streamOutputFiles = fileManager.streamOutputFiles;
    verbose           = fileManager.verbose;
    }
  return *this;
}

StreamManager::~StreamManager()
{
  // clean up
  closeRootInputFiles();
  closeRootOutputFiles();
  closeStreamInputFiles();
  closeStreamOutputFiles();
}

void StreamManager::addRootInputFile(TFile * file)
{
  rootInputFiles.push_back(file);
}

void StreamManager::addRootOutputFile(TFile * file)
{
  rootOutputFiles.push_back(file);
}

void StreamManager::addStreamInputFile(std::ifstream * file)
{
  streamInputFiles.push_back(file);
}

void StreamManager::addStreamOutputFile(std::ofstream * file)
{
  streamOutputFiles.push_back(file);
}


void StreamManager::setStreamManagerVerbose(bool _verbose)
{
  verbose = _verbose;
}

bool StreamManager::isStreamManagerVerbose() const
{
  return verbose;
}

TFile * StreamManager::openRootFile(const String & path,
                                  const String & name,
                                  const String & ioOption)
{
  String fileName;
  if (name.BeginsWith("/"))
    fileName = name;
  else
    {
    String filePath = path;
    int slash = filePath.First('/');
    int len   = filePath.Length();
    if (len>0 && (len-1)!=slash) filePath += "/";
    fileName = filePath;
    fileName += name;
    }
  if (!fileName.EndsWith(".root")) fileName += ".root";
  if (verbose)  std::cout << "StreamManager::openRootFile(...) Opening file: " << fileName << " with option: " << ioOption << std::endl;
  TFile * file = new TFile(fileName,ioOption);
  if (!file) throw  FileException(fileName,"File not found","StreamManager::openRootFile()");
  if (!file->IsOpen())  throw  FileException(fileName,"File not found/opened","StreamManager::openRootFile()");
  if (verbose) std::cout << "StreamManager::openRootFile(...) File opened successfully." << std::endl;
  return file;
}

void StreamManager::openRootInputFiles(std::vector<String> & names,
                                     const String   & ioOption)
{
  int nFiles = names.size();
  if (verbose)  std::cout << "StreamManager::openRootInputFiles(...) Opening nFiles: " << nFiles << " with option: " << ioOption << std::endl;
  if (nFiles<1)  throw  Exception("No file names supplied","StreamManager::openRootInputFiles(...)");
  for (auto & name : names)
    {
    rootInputFiles.push_back( openRootFile("",name,ioOption));
    }
  if (verbose)  std::cout << "StreamManager::openRootInputFiles(...) Completed successfully" << std::endl;
}

void StreamManager::openRootOutputFiles(std::vector<String> & names,
                                     const String   & ioOption)
{
  int nFiles = names.size();
  if (verbose)  std::cout << "StreamManager::openRootOutputFiles(...) Opening nFiles: " << nFiles << " with option: " << ioOption << std::endl;
  if (nFiles<1)  throw  Exception("No file names supplied","StreamManager::openRootOutputFiles(...)");
  for (auto & name : names)
    {
    rootInputFiles.push_back( openRootFile("",name,ioOption));
    }
  if (verbose)  std::cout << "StreamManager::openRootOutputFiles(...) Completed successfully" << std::endl;
}



std::ifstream & StreamManager::openInputAsciiFile(const String & path,
                                                const String & name,
                                                const String & extension)
{
  String fileName = path;
  // make sure that if an path is given, it ends with '/'
  int slash = fileName.First('/');
  int len = fileName.Length();
  //  std::cout << slash << std::endl;
  //  std::cout << len << std::endl;
  if (len>0 && (len-1)!=slash) fileName += "/";
  fileName += name;
  if (!fileName.EndsWith(extension)) fileName += extension;
  if (verbose)
    std::cout << "StreamManager::openInputAsciiFile(...) Attempting to open output file named: " << fileName << std::endl;
  std::ios_base::openmode mode = std::ios_base::in;
  if (verbose)  std::cout << "Opening file: " << fileName << " with option: " << mode << std::endl;
  std::ifstream * file = new std::ifstream(fileName.Data(),mode);
  if (!file || !file->is_open()) throw  FileException(fileName,"File not found","StreamManager::openInputAsciiFile()");
  if (verbose)
    std::cout << "StreamManager::openInputAsciiFile(...) File: " << fileName << " successfully opened." << std::endl;
  return *file;
}

std::ofstream & StreamManager::openOutputAsciiFile(const String & path,
                                            const String & name,
                                            const String & extension,
                                            const String & ioOption)
{
  String fileName = path;
  // make sure that if an path is given, it ends with '/'
  int slash = fileName.First('/');
  int len = fileName.Length();
  //  std::cout << slash << std::endl;
  //  std::cout << len << std::endl;
  if (len>0 && (len-1)!=slash) fileName += "/";
  fileName += name;
  if (!fileName.EndsWith(extension)) fileName += extension;
  if (verbose)
    std::cout << "StreamManager::openOutputAsciiFile(...) Attempting to open output file named: " << fileName << " in mode: " << ioOption << std::endl;
  std::ios_base::openmode mode = std::ios_base::out|std::ios_base::app;
  if (verbose)  std::cout << "Opening file: " << fileName << " with option: " << ioOption << std::endl;
  std::ofstream * file = new std::ofstream(fileName.Data(),mode);
  if (!file || !file->is_open()) throw  FileException(fileName,"File not found","StreamManager::openInputAsciiFile()");
  if (verbose)
    std::cout << "StreamManager::openOutputAsciiFile(...) File: " << fileName << " successfully opened." << std::endl;
  return *file;
}


std::ifstream & StreamManager::openInputBinaryFile(const String & path,
                                            const String & name,
                                            const String & extension)
{
  String fileName = path;
  // make sure that if an path is given, it ends with '/'
  int slash = fileName.First('/');
  int len = fileName.Length();
  //  std::cout << slash << std::endl;
  //  std::cout << len << std::endl;
  if (len>0 && (len-1)!=slash) fileName += "/";
  fileName += name;
  if (!fileName.EndsWith(extension)) fileName += extension;
  if (verbose)
    std::cout << "StreamManager::openOutputAsciiFile(...) Attempting to open output file named: " << fileName << std::endl;
  std::ios_base::openmode mode = std::ios_base::in|std::ios::binary;
  if (verbose)  std::cout << "Opening file: " << fileName <<  std::endl;
  std::ifstream * file = new std::ifstream(fileName.Data(),mode);
  if (!file || !file->is_open()) throw  FileException(fileName,"File not found","StreamManager::openInputAsciiFile()");
  if (verbose)
    std::cout << "StreamManager::openOutputAsciiFile(...) File: " << fileName << " successfully opened." << std::endl;
  return *file;
}



std::ofstream & StreamManager::openOutputBinaryFile(const String & path,
                                             const String & name,
                                             const String & extension,
                                             const String & ioOption)
{
  String fileName = path;
  // make sure that if an path is given, it ends with '/'
  int slash = fileName.First('/');
  int len = fileName.Length();
  //  std::cout << slash << std::endl;
  //  std::cout << len << std::endl;
  if (len>0 && (len-1)!=slash) fileName += "/";
  fileName += name;
  if (!fileName.EndsWith(extension)) fileName += extension;
  if (verbose)
    std::cout << "StreamManager::openOutputAsciiFile(...) Attempting to open output file named: " << fileName  << std::endl;
  std::ios_base::openmode mode = std::ios_base::out|std::ios::binary;
  if (verbose)  std::cout << "Opening file: " << fileName <<  std::endl;
  std::ofstream * file = new std::ofstream(fileName.Data(),mode);
  if (!file || !file->is_open()) throw  FileException(fileName,"File not found","StreamManager::openInputAsciiFile()");
  if (verbose)
    std::cout << "StreamManager::openOutputAsciiFile(...) File: " << fileName << " successfully opened." << std::endl;
  return *file;
}

void StreamManager::copyFile(const String & inputPath,  const String & inputFileName, const String & inExtension,
                    const String & outputPath, const String & outputFileName, const String & outExtension)
{
  std::ifstream & inputFile  = openInputBinaryFile(inputPath,inputFileName,inExtension);
  std::ofstream & outputFile = openOutputBinaryFile(outputPath,outputFileName,outExtension);
  if (verbose)
    std::cout << "Attempting to copy file " << inputPath+inputFileName << " to " << outputPath+outputFileName << std::endl;
  // do a binary buffer copy
  outputFile << inputFile.rdbuf();
  inputFile.close();
  outputFile.close();
  if (verbose) std::cout << " Successfully completed!" << std::endl;
}


void StreamManager::closeRootFile(TFile * file)
{
  if (file && file->IsOpen()) file->Close();
}

void StreamManager::closeRootFiles(std::vector<TFile *> & files)
{
  for (auto & file : files) closeRootFile(file);
}

void StreamManager::closeRootInputFiles()
{
  closeRootFiles(rootInputFiles);
  rootInputFiles.clear();
}

void StreamManager::closeRootOutputFiles()
{
  closeRootFiles(rootOutputFiles);
  rootOutputFiles.clear();
}

void StreamManager::closeStreamInputFiles()
{
/* no ops -- needs to be implemented */
}

void StreamManager::closeStreamOutputFiles()
{
  /* no ops -- needs to be implemented */
}



String StreamManager::removeRootExtension(const String fileName)
{
  String name = fileName;
  int dot = name.Last('.');
  int len = name.Length();
  //  std::cout << dot << std::endl;
  //  std::cout << len << std::endl;
  if (dot==len-5 ) name.Remove(dot,len-dot);
  return name;
}




} // namespace CAP


