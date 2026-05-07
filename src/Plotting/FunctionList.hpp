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

#ifndef CAP__FunctionList
#define CAP__FunctionList
#include <stdio.h>
#include "TClass.h"
#include "TFile.h"
#include "TAxis.h"
#include "TSystem.h"
#include "TStyle.h"
#include "TMath.h"
#include "TPad.h"
#include "TF1.h"
#include "TF2.h"
#include "TRandom.h"
#include "Aliases.hpp"
#include "TLatex.h"
#include "TLine.h"
#include "TArrow.h"
#include "TLegend.h"
#include "GraphConfiguration.hpp"
#include "CanvasConfiguration.hpp"
#include "CanvasList.hpp"
using namespace std;

namespace CAP
{


class FunctionList  // : public Collection<TF1>
{

protected:

  std::vector<TF1*> functions;

public:
  
  /////////////////////////////////////////////////////////////
  // Member functions
  /////////////////////////////////////////////////////////////
  FunctionList();
  FunctionList(const FunctionList & source);
  virtual ~FunctionList();
  FunctionList & operator=(const FunctionList & source);

  void setDefaultOptions(bool color=0);
  void setFunctionProperties(TF1 * f,
                             GraphConfiguration & graphConfiguration,
                             bool verbose=false);
  //void plotAllFunctions(const String & outputPath, bool doPrint=false);

  void push_back(TF1* function);
  TF1* functionAt(unsigned int index);
  const std::vector<TF1*> & functions() const;
  std::vector<TF1*> & functions();
  ();
  
  ClassDef(FunctionList,0)
}; // FunctionList


} // namespace CAP

#endif /* FunctionList_hpp */
