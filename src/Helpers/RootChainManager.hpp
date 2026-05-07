/* **********************************************************************
 * Copyright (C) 2019-2024, Claude Pruneau
 * All rights reserved.
 *
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 *
 * Author: Claude Pruneau,   04/01/2024
 *
 * *********************************************************************/
#ifndef CAP__RootChainManager
#define CAP__RootChainManager
#include "Aliases.hpp"
#include "TChain.h"
#include "TTree.h"
#include "TBranch.h"

namespace CAP
{

class RootChainManager
{

public:

  RootChainManager();
  RootChainManager(const RootChainManager & manager);
  RootChainManager operator=(const RootChainManager & manager);
  virtual ~RootChainManager() {}

  int clonesMaxArraySize() const { return _clonesMaxArraySize;}
  void setClonesMaxArraySize(int value) { _clonesMaxArraySize = value; }
  void show(Long64_t entry);
  Int_t entry(Long64_t entry);
  Long64_t loadTree(Long64_t entry);

  //!
  //!Get pointer to the root input chain
  //!
  TChain  * chain() { return _chain;}
  void setTreeIndex(int value) { _treeIndex = value; }
  int treeIndex() const  { return _treeIndex; }
  void initializeImportTree(const String & treeName);
  void initializeExportTree(const String & treeName);
  void importEvent();
  void exportEvent();

protected:

  TChain  *_chain;     //!pointer to the analyzed (input)  TTree or TChain
  Int_t    _treeIndex; //!current Tree number in an input  TChain
  TFile   *_dataFile;
  Long64_t _nEntries; //! number of entries in an input TTree or TChain
  Long64_t _nBytes;
  Long64_t _nb;
  long _entryIndex;
  int  _clonesMaxArraySize;

  ClassDef(RootChainManager,0)

};

}
#endif /* CAP_RootTreeReader */
