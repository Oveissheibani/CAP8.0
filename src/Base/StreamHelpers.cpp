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
#include "StreamHelpers.hpp"
#include "PrintHelpers.hpp"
#include "Exceptions.hpp"
#include "TSystem.h"
//#include "TSystemFile.h"
//#include "TSystemDirectory.h"

namespace CAP
{

std::ifstream & openInputAsciiFile(const String & fileName,
                                   bool verbose)
{
  std::ios_base::openmode mode = std::ios_base::in;
  if (verbose)
    {
    printCR();
    printValue("openInputAsciiFile(...) Open input file named",fileName);
    printValue("openInputAsciiFile(...) with option",mode);
    }
  std::ifstream * file = new std::ifstream(fileName.Data(),mode);
  if (!file || !file->is_open()) throw  FileException(fileName,"File not found","openInputAsciiFile()");
  if (verbose)
    {
    printCR();
    printValue("openInputAsciiFile(...) Successfully opened file named",fileName);
    }
  return *file;
}


std::ofstream & openOutputAsciiFile(const String & fileName,
                                    bool verbose)
{
  std::ios_base::openmode mode = std::ios_base::out|std::ios_base::app;
  if (verbose)
    {
    printCR();
    printValue("openOutputAsciiFile(...) Open input file named",fileName);
    printValue("openOutputAsciiFile(...) with option",mode);
    }
  std::ofstream * file = new std::ofstream(fileName.Data(),mode);
  if (!file || !file->is_open()) throw  FileException(fileName,"File not found","openInputAsciiFile()");
  if (verbose)
    {
    printCR();
    printValue("openOutputAsciiFile(...) Successfully opened file named",fileName);
    }
  return *file;
}

std::ifstream & openInputAsciiFile(const String & path,
                                   const String & name,
                                   const String & extension,
                                   bool verbose)
{
  String fileName = path;
  // make sure that if an path is given, it ends with '/'
  int slash = fileName.First('/');
  int len = fileName.Length();
  //  std::cout << slash << std::endl;
  //  std::cout << len << std::endl;
  if (len>0  &&  (len-1)!=slash) fileName += "/";
  fileName += name;
  if (!fileName.EndsWith(extension)) fileName += extension;
  std::ios_base::openmode mode = std::ios_base::in;
  if (verbose)
    {
    printCR();
    printValue("openInputAsciiFile(...) Open input file named",fileName);
    printValue("openInputAsciiFile(...) with option",mode);
    }
  std::ifstream * file = new std::ifstream(fileName.Data(),mode);
  if (!file || !file->is_open()) throw  FileException(fileName,"File not found","openInputAsciiFile()");
  if (verbose)
    std::cout << "openInputAsciiFile(...) File: " << fileName << " successfully opened." << std::endl;
  return *file;
}


std::ofstream & openOutputAsciiFile(const String & path,
                                    const String & name,
                                    const String & extension,
                                    bool verbose)
{
  String fileName = path;
  // make sure that if an path is given, it ends with '/'
  int slash = fileName.First('/');
  int len = fileName.Length();
  //  std::cout << slash << std::endl;
  //  std::cout << len << std::endl;
  if (len>0  &&  (len-1)!=slash) fileName += "/";
  fileName += name;
  if (!fileName.EndsWith(extension)) fileName += extension;
  //std::ios_base::openmode mode = std::ios::out; ////std::ios::out|std::ios_base::app;
  if (verbose)
    {
    printCR();
    printValue("openInputAsciiFile(...) Open output file named",fileName);
    //printValue("openInputAsciiFile(...) with option",mode);
    }
  std::ofstream * file = new std::ofstream(fileName.Data(),std::ios::out | std::ios::trunc);
  if (!file || !file->is_open()) throw  FileException(fileName,"File not found","openInputAsciiFile()");
  if (verbose)
    std::cout << "openOutputAsciiFile(...) File: " << fileName << " successfully opened." << std::endl;
  return *file;
}


std::ifstream & openInputBinaryFile(const String & path,
                                    const String & name,
                                    const String & extension,
                                    bool verbose)
{
  String fileName = path;
  // make sure that if an path is given, it ends with '/'
  int slash = fileName.First('/');
  int len = fileName.Length();
  //  std::cout << slash << std::endl;
  //  std::cout << len << std::endl;
  if (len>0  &&  (len-1)!=slash) fileName += "/";
  fileName += name;
  if (!fileName.EndsWith(extension)) fileName += extension;
  std::ios_base::openmode mode = std::ios_base::in|std::ios::binary;
  if (verbose)
    {
    printCR();
    printValue("openInputBinaryFile(...) Open output file named",fileName);
    printValue("openInputBinaryFile(...) with option",mode);
    }
  std::ifstream * file = new std::ifstream(fileName.Data(),mode);
  if (!file || !file->is_open()) throw  FileException(fileName,"File not found","openInputAsciiFile()");
  if (verbose)
    {
    printCR();
    printValue("openInputBinaryFile(...) Successfully opened file named",fileName);
    }
  return *file;
}

std::ofstream & openOutputBinaryFile(const String & path,
                                    const String & name,
                                     const String & extension,
                                     bool verbose)
{
  String fileName = path;
  // make sure that if an path is given, it ends with '/'
  int slash = fileName.First('/');
  int len = fileName.Length();
  //  std::cout << slash << std::endl;
  //  std::cout << len << std::endl;
  if (len>0  &&  (len-1)!=slash) fileName += "/";
  fileName += name;
  if (!fileName.EndsWith(extension)) fileName += extension;
  std::ios_base::openmode mode = std::ios_base::out|std::ios::binary;
  if (verbose)
    {
    printCR();
    printValue("openOuputBinaryFile(...) Open output binary file named",fileName);
    }
  std::ofstream * file = new std::ofstream(fileName.Data(),mode);
  if (!file || !file->is_open()) throw  FileException(fileName,"File not found","openInputAsciiFile()");
  if (verbose)
    {
    printCR();
    printValue("openOuputBinaryFile(...) Successfully opened file named",fileName);
    }
  return *file;
}


void copyFile(const String & inputPath,  const String & inputFileName, const String & inExtension,
              const String & outputPath, const String & outputFileName, const String & outExtension,
              bool verbose)
{
  std::ifstream & inputFile  = openInputBinaryFile(inputPath,inputFileName,inExtension);
  std::ofstream & outputFile = openOutputBinaryFile(outputPath,outputFileName,outExtension);
  if (verbose)
    {
    printCR();
    printValue("copyFile(...) Copying from file",inputFileName);
    printValue("copyFile(...) to file",outputFileName);
    }
  // do a binary buffer copy
  outputFile << inputFile.rdbuf();
  inputFile.close();
  outputFile.close();
  if (verbose)
    {
    printCR();
    printString("copyFile(...) Copy successfully completed");
    }
}

} // namespace CAP

