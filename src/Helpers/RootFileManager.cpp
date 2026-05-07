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
#include "Manager.hpp"

namespace CAP
{
RootFileManager::RootFileManager()
:
Manager<TFile>()
{ }

RootFileManager::RootFileManager(const RootFileManager & manager)
:
Manager<TFile>(manager)
{ }


RootFileManager & RootFileManager::operator=(const RootFileManager & manager)
{
  if (this!=&manager)
    {
    Manager<TFile>::operator=(manager);
    }
  return *this;
}


//void RootFileManager::resetObjectAt(unsigned int index)
//{
//  if (!owns(index)) throw ManagerException(index,"Invalid Index",__FUNCTION__);
//  objects[index]->Reset();
//}
//
//void RootFileManager::clearObjectAt(unsigned int index)
//{
//  if (!owns(index)) throw ManagerException(index,"Invalid Index",__FUNCTION__);
//  objects[index]->Clear();
//}


} // namespace CAP
