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
#ifndef CAP__RootFileManager
#define CAP__RootFileManager
#include "Manager.hpp"

namespace CAP
{

class RootFileManager : public Manager<TFile>
{
public:

  RootFileManager();
  RootFileManager(const RootFileManager & Manager);
  RootFileManager & operator=(const RootFileManager & manager);
  virtual ~RootFileManager() { }
//  virtual void resetObjectAt(unsigned int index);
//  virtual void clearObjectAt(unsigned int index);

  ClassDef(Manager,0)
};



} // namespace CAP

#endif /* CAP_Manager */


