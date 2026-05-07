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
#ifndef CAP__FileIterator
#define CAP__FileIterator
#include "EventProcessor.hpp"

namespace CAP
{

  //!
  //! File  Iterator designed to repeat the same series of tasks on many files/directories
  //!
  class FileIterator
  :
  public EventProcessor
  {
  public:

  FileIterator();
  FileIterator(const FileIterator & task);
  FileIterator operator=(const FileIterator & task);
  virtual ~FileIterator() {}
  virtual void setDefaultConfiguration();
  virtual void configure(const Configuration & configuration);
  virtual void execute();

  protected:

//  bool isGrid;
//  int nBunches;
//  int nSubBunches;
//  String bunchLabel;
//  String subBunchLabel;
  int _nLevels;
  ClassDef(FileIterator,0)
  };

} // namespace CAP

#endif /* CAP__FileIterator */
