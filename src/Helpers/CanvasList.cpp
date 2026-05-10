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
#include "CanvasList.hpp"

ClassImp(CAP::CanvasList);

namespace CAP
{

CanvasList::CanvasList()
:
_canvases()
{
  /* no ops*/
}

CanvasList::CanvasList(const CanvasList & source)
:
_canvases(source._canvases)
{
  /* no ops*/
}


CanvasList & CanvasList::operator=(const CanvasList & source)
{
  if (this!=&source)
    {
    _canvases = source._canvases;
    }
  return *this;
}




//!
//!Create a directory or folder in the file system
//!If the directory alearfy exists, do nothing.
//!
void CanvasList::createDirectory(const String & dirName)
{
  gSystem->mkdir(dirName,1);
}

//!
//! Create a canvas
//!
TCanvas * CanvasList::createCanvas(const String & canvasName,
                                    const Configuration & configuration,
                                    int inc)
{
  int xInc __attribute__((unused)) = inc*size();
  TCanvas * canvas = new TCanvas(canvasName,
                                 canvasName,
                                 inc+configuration.valueInt("windowXPosition"),
                                 configuration.valueInt("windowYPosition"),
                                 configuration.valueInt("windowWidth"),
                                 configuration.valueInt("windowHeight") );
  canvas->SetLogx(        configuration.valueBool("windowLogX") );
  canvas->SetLogy(        configuration.valueBool("windowLogY") );
  canvas->SetLogz(        configuration.valueBool("windowLogZ") );
  canvas->SetRightMargin( configuration.valueDouble("windowRightMargin") );
  canvas->SetLeftMargin(  configuration.valueDouble("windowLeftMargin") );
  canvas->SetBottomMargin(configuration.valueDouble("windowBottomMargin") );
  canvas->SetTopMargin(   configuration.valueDouble("windowTopMargin") );
  canvas->SetTicky(       configuration.valueBool("useTickx") );
  canvas->SetTickx(       configuration.valueBool("useTicky") );
  canvas->SetGridx(       configuration.valueBool("useGridx") );
  canvas->SetGridy(       configuration.valueBool("useGridy") );
  canvas->SetTheta(       configuration.valueDouble("windowTheta") );
  canvas->SetPhi(         configuration.valueDouble("windowPhi") );
  canvas->SetFillColor(   configuration.valueInt("windowFillColor") );
  canvas->SetFillStyle(   configuration.valueInt("windowFillStyle") );
  canvas->SetBorderSize(  configuration.valueInt("windowBorderSize") );
  canvas->SetBorderMode(  configuration.valueInt("windowBorderMode") );
  canvas->SetTicky(1);
  canvas->SetTickx(1);
  _canvases.push_back(canvas);
  return canvas;
}

////////////////////////////////////////////////////
// Create a canvas
////////////////////////////////////////////////////
TCanvas * CanvasList::createCanvasXX(int nx, int ny, const String & canvasName, const Configuration & configuration, int inc)
{
  int xInc __attribute__((unused)) = inc*size();
  TCanvas * canvas = new TCanvas(canvasName,
                                 canvasName,
                                 xInc+configuration.valueInt("windowXPosition"),
                                 configuration.valueInt("windowYPosition"),
                                 configuration.valueInt("windowWidth"),
                                 configuration.valueInt("windowHeight") );
  canvas->Divide(nx,ny,0,0);
  canvas->SetLogx(        configuration.valueBool("windowLogX") );
  canvas->SetLogy(        configuration.valueBool("windowLogY") );
  canvas->SetLogz(        configuration.valueBool("windowLogZ") );
  canvas->SetRightMargin( configuration.valueDouble("windowRightMargin") );
  canvas->SetLeftMargin(  configuration.valueDouble("windowLeftMargin") );
  canvas->SetBottomMargin(configuration.valueDouble("windowBottomMargin") );
  canvas->SetTopMargin(   configuration.valueDouble("windowTopMargin") );
  canvas->SetTheta(       configuration.valueDouble("windowTheta") );
  canvas->SetPhi(         configuration.valueDouble("windowPhi") );

  canvas->SetFillColor(  configuration.valueInt("windowFillColor") );
  canvas->SetFillStyle(  configuration.valueInt("windowFillStyle") );
  canvas->SetBorderSize( configuration.valueDouble("windowBorderSize") );
  canvas->SetBorderMode( configuration.valueDouble("windowBorderMode") );
  _canvases.push_back(canvas);
  return canvas;
}

//!
//! Print Canvas
//!
void CanvasList::printCanvas(TCanvas * canvas,
                              const String & outputPath,
                              bool printGif,
                              bool printPdf,
                              bool printSvg,
                              bool printPng,
                              bool printC)
{
  createDirectory(outputPath);
  String fileName = outputPath;
  fileName += "/";
  fileName += canvas->GetName();
  if (printGif) canvas->Print(fileName+".gif");
  if (printPdf) canvas->Print(fileName+".pdf");
  if (printSvg) canvas->Print(fileName+".svg");
  if (printPng) canvas->Print(fileName+".png");
  if (printC)   canvas->Print(fileName+".C");
}

//!
//! Print All Canvases Owed by this group
//!
void CanvasList::printAllCanvas(const String & outputPath,
                                 bool printGif,
                                 bool printPdf,
                                 bool printSvg,
                                 bool printPng,
                                 bool printC)
{
  for (auto & canvas : _canvases)
    printCanvas(canvas,outputPath,printGif,printPdf,printSvg,printPng,printC);
}


} // namespace CAP

