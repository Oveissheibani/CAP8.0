/* **********************************************************************
 * Copyright (C) 2019-2024, Claude Pruneau
 * All rights reserved.
 *
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 *
 * Author: Claude Pruneau,   04/01/2024-2024
 *
 * *********************************************************************/
#ifndef CAP__StreamManager
#define CAP__StreamManager
#include "Aliases.hpp"
#include "StreamHelpers.hpp"
#include "Manager.hpp"

namespace CAP
{

class StreamManager : public Manager<stream*>
{
protected:

public:

  StreamManager();
  StreamManager(const StreamManager & fileManager);
  StreamManager operator=(const StreamManager & fileManager);
  virtual ~StreamManager();

  void addFile(TFile * file);

  TFile * openFile(const String & inputPath,
                   const String & fileName,
                   const String & ioOption) ;

  void openInputFiles(std::vector<String> & fileNames);
  void openOutputFiles(std::vector<String> & fileNames,
                           const String & ioOption="RECREATE");

  //!
  //! Open the root file named "fileName" located on the path "inputPath", using options specified by "ioOption".
  //! @param inputPath path where to find or create the file.
  //! @param fileName name of the root file to open for i/o.
  //! @param ioOption i/o options.
  //! @return Pointer to the file if successfully open or a null pointer if the file could not be opened.
  //!
  std::ifstream & openInputAsciiFile(const String & path,
                                     const String & name,
                                     const String & extension);

  std::ofstream & openOutputAsciiFile(const String & path,
                                      const String & name,
                                      const String & extension,
                                      const String & ioOption="");

  std::ifstream & openInputBinaryFile(const String & path,
                                      const String & fileName,
                                      const String & extension);

  std::ofstream & openOutputBinaryFile(const String & outputPath,
                                       const String & fileName,
                                       const String & extension,
                                       const String & ioOption="");
  void closeRootFile(TFile * file);
  void closeRootFiles(std::vector<TFile *> & files);
  void closeRootInputFiles();
  void closeRootOutputFiles();
  void closeStreamInputFiles();
  void closeStreamOutputFiles();

  void copyFile(const String & inputPath,  const String & inputFileName, const String & inExtension,
                const String & outputPath, const String & outputFileName, const String & outExtension);
  //!
  //! Read the given (long) value with the given name in the given output file.
  //!
  long readParameter(TFile & inputFile, const String & parameterName);

  //!
  //! Save the given (long) value with the given name in the given output file.
  //!
  void writeParameter(TFile & outputFile, const String & parameterName, long value);



  std::vector<String> listFilesInDir(const String & pathname,
                              const String & ext,
                              bool prependPath=true,
                              bool verbose=false,
                              int  maximumDepth=1,
                              int  currentDepth=0);


  std::vector<String> listFilesInDir(const String & pathName,
                              std::vector<String> includePatterns,
                              std::vector<String> excludePatterns,
                              bool prependPath=true,
                              bool verbose=false,
                              int  maximumDepth=1,
                              int  currentDepth=0);

  //!
  //!Get selected files in the given directory. The selection is made by means of two sets of configuration properties
  //!of the form IncludePattern# and ExcludePattern# (where # represents a number between 0 and 20. These include
  //!and exclude patterns are used to assemble two lists, std::vector<String> includePatterns and std::vector<String> excludePatterns)
  //!which are then used to identify all file names of the given folder that match the includePatterns but exclude the excludePatterns.
  //!
  //std::vector<String> getSelectedFileNamesFrom(const String & folder);


  //!
  //!Removes the ".root" ending of the given string if it has this ending.
  //!This function is used in the management of file names for subsampling analyses and
  //!the calculatioin of derived histograms.
  //!
  String removeRootExtension(const String fileName);

  String makeFileName(const String & inputPath,
                      const String & fileName,
                      const String & extension);

  void createDirectory(const String path);

  ClassDef(StreamManager,0)
};



class OStreamStreamManager : public Manager<ostream*>
{
protected:


public:

  StreamManager();
  StreamManager(const StreamManager & fileManager);
  StreamManager operator=(const StreamManager & fileManager);
  virtual ~StreamManager();

  void addRootInputFile(TFile * file);
  void addRootOutputFile(TFile * file);
  void addStreamInputFile(std::ifstream * file);
  void addStreamOutputFile(std::ofstream * file);

  void setStreamManagerVerbose(bool verbose);
  bool isStreamManagerVerbose() const;

  //!
  //! Open the root file named "fileName" located on the path "inputPath", using options specified by "ioOption".
  //! @param inputPath path where to find or create the file.
  //! @param fileName name of the root file to open for i/o.
  //! @param ioOption i/o options.
  //! @return Pointer to the file if successfully open or a null pointer if the file could not be opened.
  //!
  //! Sometimes the caller sets the path within the file name... so one
  //! should not prepend the path to the name... By convention, we assume that
  //! if the file name begins with '/', it is an absolute path. The functions also makes
  //! sure that if an inputPath is given, it ends with '/'. One also checks that
  //! the root extension is included in the file name.
  //!
  TFile * openRootFile(const String & inputPath,
                       const String & fileName,
                       const String & ioOption) ;

  void openRootInputFiles(std::vector<String> & fileNames,
                          const String & ioOption="OLD");
  void openRootOutputFiles(std::vector<String> & fileNames,
                           const String & ioOption="RECREATE");

  //!
  //! Open the root file named "fileName" located on the path "inputPath", using options specified by "ioOption".
  //! @param inputPath path where to find or create the file.
  //! @param fileName name of the root file to open for i/o.
  //! @param ioOption i/o options.
  //! @return Pointer to the file if successfully open or a null pointer if the file could not be opened.
  //!
  std::ifstream & openInputAsciiFile(const String & path,
                                     const String & name,
                                     const String & extension);

  std::ofstream & openOutputAsciiFile(const String & path,
                                      const String & name,
                                      const String & extension,
                                      const String & ioOption="");

  std::ifstream & openInputBinaryFile(const String & path,
                                      const String & fileName,
                                      const String & extension);

  std::ofstream & openOutputBinaryFile(const String & outputPath,
                                       const String & fileName,
                                       const String & extension,
                                       const String & ioOption="");
  void closeRootFile(TFile * file);
  void closeRootFiles(std::vector<TFile *> & files);
  void closeRootInputFiles();
  void closeRootOutputFiles();
  void closeStreamInputFiles();
  void closeStreamOutputFiles();

  void copyFile(const String & inputPath,  const String & inputFileName, const String & inExtension,
                const String & outputPath, const String & outputFileName, const String & outExtension);
  //!
  //! Read the given (long) value with the given name in the given output file.
  //!
  long readParameter(TFile & inputFile, const String & parameterName);

  //!
  //! Save the given (long) value with the given name in the given output file.
  //!
  void writeParameter(TFile & outputFile, const String & parameterName, long value);



  std::vector<String> listFilesInDir(const String & pathname,
                              const String & ext,
                              bool prependPath=true,
                              bool verbose=false,
                              int  maximumDepth=1,
                              int  currentDepth=0);


  std::vector<String> listFilesInDir(const String & pathName,
                              std::vector<String> includePatterns,
                              std::vector<String> excludePatterns,
                              bool prependPath=true,
                              bool verbose=false,
                              int  maximumDepth=1,
                              int  currentDepth=0);

  //!
  //!Get selected files in the given directory. The selection is made by means of two sets of configuration properties
  //!of the form IncludePattern# and ExcludePattern# (where # represents a number between 0 and 20. These include
  //!and exclude patterns are used to assemble two lists, std::vector<String> includePatterns and std::vector<String> excludePatterns)
  //!which are then used to identify all file names of the given folder that match the includePatterns but exclude the excludePatterns.
  //!
  //std::vector<String> getSelectedFileNamesFrom(const String & folder);


  //!
  //!Removes the ".root" ending of the given string if it has this ending.
  //!This function is used in the management of file names for subsampling analyses and
  //!the calculatioin of derived histograms.
  //!
  String removeRootExtension(const String fileName);

  String makeFileName(const String & inputPath,
                      const String & fileName,
                      const String & extension);

  void createDirectory(const String path);

  ClassDef(StreamManager,0)
};

}

#endif /* CAP__StreamManager */
