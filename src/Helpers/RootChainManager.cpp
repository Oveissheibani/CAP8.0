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
#include "RootChainManager.hpp"
#include "Exceptions.hpp"
#include "PrintHelpers.hpp"

ClassImp(CAP::RootChainManager);

namespace CAP
{

RootChainManager::RootChainManager()
:
_chain(nullptr),
_treeIndex(0),
_dataFile(nullptr),
_nEntries(0),
_nBytes(0),
_nb(0),
_entryIndex(0),
_clonesMaxArraySize(10000)
{
  /* no ops */
}

RootChainManager::RootChainManager(const RootChainManager & manager)
:
_chain(manager._chain),
_treeIndex(manager._treeIndex),
_dataFile(manager._dataFile),
_nEntries(manager._nEntries),
_nBytes(manager._nBytes),
_nb(manager._nb),
_entryIndex(manager._entryIndex),
_clonesMaxArraySize(manager._clonesMaxArraySize)
{
  /* no ops */
}

RootChainManager RootChainManager::operator=(const RootChainManager & manager)
{
  if (this!=&manager)
    {
    _chain      = manager._chain;
    _treeIndex  = manager._treeIndex;
    _dataFile   = manager._dataFile;
    _nEntries   = manager._nEntries;
    _nBytes     = manager._nBytes;
    _nb         = manager._nb;
    _entryIndex = manager._entryIndex;
    _clonesMaxArraySize = manager._clonesMaxArraySize;
    }
  return *this;
}


void RootChainManager::show(Long64_t entry)
{
  if (!_chain) throw Exception("No chain","RootChainManager::Show(Long64_t entry)");
  _chain->Show(entry);
}

Int_t RootChainManager::entry(Long64_t entry)
{
  if (!_chain) throw Exception("No chain","RootChainManager::GetEntry(Long64_t entry)");
  return _chain->GetEntry(entry);
}


Long64_t RootChainManager::loadTree(Long64_t entry)
{
  // Set the environment to read one entry
  if (!_chain) throw Exception("No chain","RootChainManager::LoadTree(Long64_t entry)");
  Long64_t centry = _chain->LoadTree(entry);
  if (centry < 0) return centry;
  if (_chain->GetTreeNumber() != _treeIndex)
    {
    _treeIndex = _chain->GetTreeNumber();
    }
  return centry;
}


void RootChainManager::initializeImportTree(const String & treeName)
{
  _chain = new TChain(treeName);
  if (!_chain) throw Exception("No chain",__FUNCTION__);
}

void RootChainManager::initializeExportTree(const String & treeName __attribute__((unused)) )
{
  throw Exception("No chain",__FUNCTION__);
}


void RootChainManager::importEvent()
{
  setTreeIndex(-1);
  _entryIndex = 0;
  _nEntries = _chain->GetEntriesFast();
  if (_nEntries < 1) throw Exception("nEntries < 1",__FUNCTION__);
  _nBytes = 0;
  _nb = 0;
}


void RootChainManager::exportEvent()
{
  throw Exception("Not available",__FUNCTION__);
}

} // namespace CAP
