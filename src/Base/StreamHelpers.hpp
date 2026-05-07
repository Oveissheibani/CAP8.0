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
#ifndef CAP__StreamHelpers
#define CAP__StreamHelpers
#include <iostream>
#include <fstream>
#include "Aliases.hpp"

namespace CAP
{
  std::ifstream & openInputAsciiFile(const String & name,
                                     bool verbose=true);

  std::ofstream & openOutputAsciiFile(const String & name,
                                      bool verbose=true);

  std::ifstream & openInputAsciiFile(const String & path,
                                     const String & name,
                                     const String & extension,
                                     bool verbose=true);

  std::ofstream & openOutputAsciiFile(const String & path,
                                      const String & name,
                                      const String & extension,
                                      bool verbose=true);

  std::ifstream & openInputBinaryFile(const String & path,
                                      const String & fileName,
                                      const String & extension,
                                      bool verbose=true);

  std::ofstream & openOutputBinaryFile(const String & outputPath,
                                       const String & fileName,
                                       const String & extension,
                                       bool verbose=true);

  void copyFile(const String & inputPath,  const String & inputFileName, const String & inExtension,
                const String & outputPath, const String & outputFileName, const String & outExtension,
                bool verbose=true);


}

#endif /* CAP__FileManager */
