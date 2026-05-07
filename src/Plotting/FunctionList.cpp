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
#include "TROOT.h"
#include "TF1.h"
#include "TF2.h"
#include "FunctionList.hpp"
#include "PrintHelpers.hpp"

using CAP::FunctionList;

ClassImp(FunctionList);

FunctionList::FunctionList()
:
functions()
{
  /* no ops*/
}


FunctionList::FunctionList(const FunctionList & source)
:
functions(source.functions) // shallow copy
{
  /* no ops*/
}

FunctionList::~FunctionList()
{
  // root is in charge///
}

FunctionList & FunctionList::operator=(const FunctionList & source)
{
  if (this!=&source)
    {
    functions  = source.functions;
    }
  return *this;
}

void FunctionList::setDefaultOptions(bool color)
{
  if (color)
    gStyle->SetPalette(1,0);
  else
    gStyle->SetPalette(7,0);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);
  gStyle->SetOptDate(0);
  gStyle->SetOptTitle(0);
  gStyle->SetPadBorderMode(0);
}

//!
//! Setting Function Properties
//!
void FunctionList::setFunctionProperties(TF1 * f,
                                          GraphConfiguration & graphConfiguration,
                                          bool verbose)
{
  if (verbose)
    {
    printCR();
    printValue("Setting properties of function",f->title());
    }
  //String name =
  f->SetLineColor(graphConfiguration.valueInt("lineColor"));
  f->SetLineStyle(graphConfiguration.valueInt("lineStyle"));
  f->SetLineWidth(graphConfiguration.valueInt("lineWidth"));
  TAxis * xAxis = (TAxis *) f->GetXaxis();
  xAxis->SetNdivisions(graphConfiguration.valueDouble("nXDivisions"));
  xAxis->SetTitleSize(graphConfiguration.valueDouble("xTitleSize"));
  xAxis->SetTitleOffset(graphConfiguration.valueDouble("xTitleOffset"));
  //xAxis->SetTitle(graphConfiguration.xTitle);
  xAxis->SetLabelSize(graphConfiguration.valueDouble("xLabelSize"));
  xAxis->SetLabelOffset(graphConfiguration.valueDouble("xLabelOffset"));
  TAxis * yAxis = (TAxis *) f->GetYaxis();
  yAxis->SetNdivisions(graphConfiguration.valueInt("nYDivisions"));
  yAxis->SetTitleSize(graphConfiguration.valueDouble("yTitleSize"));
  yAxis->SetTitleOffset(graphConfiguration.valueDouble("yTitleOffset"));
  yAxis->SetLabelSize(graphConfiguration.valueDouble("yLabelSize"));
  yAxis->SetLabelOffset(graphConfiguration.valueDouble("yLabelOffset"));
  //yAxis->SetTitle(graphConfiguration.yTitle);
  if (f->IsA() == TF2::Class() )
    {
    TAxis * zAxis = (TAxis *) f->GetZaxis();
    zAxis->SetNdivisions(graphConfiguration.valueInt("nZDivisions"));
    zAxis->SetTitleSize(graphConfiguration.valueDouble("zTitleSize"));
    zAxis->SetTitleOffset(graphConfiguration.valueDouble("zTitleOffset"));
    zAxis->SetLabelSize(graphConfiguration.valueDouble("zLabelSize"));
    zAxis->SetLabelOffset(graphConfiguration.valueDouble("zLabelOffset"));
    }
}

//!
//!Plot all functions if this group in separate canvases
//!
//void FunctionList::plotAllFunctions(const String & outputPath, bool doPrint)
//{
////  GraphConfiguration  * gc1D = new GraphConfiguration(1,0);
////  GraphConfiguration  * gc2D = new GraphConfiguration(2,0);
////  CanvasConfiguration * cc1D = new CanvasConfiguration();
////  CanvasConfiguration * cc2D = new CanvasConfiguration();
////  CanvasGroup    * canvasGroup = new CanvasGroup();
////  canvasGroup->createDirectory(outputPath);
////  String name;
////  for (auto & f : functions)
////    {
////    name = f->name();
////    if (f->IsA() == TF1::Class())
////      {
////      if (verbose)
////        {
////        printValue("Plotting 1D function named",f->title());
////        }
////      canvasGroup->createCanvas(name, *cc1D, 30);
////      setFunctionProperties(f, *gc1D);
////      f->Draw();
////      }
////    else if (f->IsA() == TF2::Class())
////      {
////      if (verbose)
////        {
////        printValue("Plotting 2D function named",f->title());
////        }
////      canvasGroup->createCanvas(name, *cc2D, 30);
////      setFunctionProperties(f, *gc2D);
////      f->Draw("SURF3");
////      }
////    }
////  if (doPrint) canvasGroup->printAllCanvas(outputPath);
//}


void FunctionList::push_back(TF1* function)
{
  if (!function) throw NullPointerException(__FUNCTION__);
  functions.push_back(function);
}

TF1* FunctionList::functionAt(unsigned int index)
{
  if (index>=functions.size()) throw OutOfBoundException(__FUNCTION__);
  return functions[index];
}
const std::vector<TF1*> & FunctionList::functions() const
{
  return functions;
}

std::vector<TF1*> & FunctionList::functions()
{
  return functions;
}

int FunctionList::size()
{
  return functions.size();
}
