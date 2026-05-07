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
#ifndef CAP__CanvasList
#define CAP__CanvasList
#include "Aliases.hpp"
#include "TCanvas.h"
#include "TSystem.h"
#include "CanvasConfiguration.hpp"

namespace CAP
{

class CanvasList
{
public:

  CanvasList();
  CanvasList(const CanvasList & source);
  virtual ~CanvasList() {}
  CanvasList & operator=(const CanvasList & source);

  unsigned int size() const { return _canvases.size();  }
  std::vector<TCanvas*> & canvases() {  return _canvases;}
  const std::vector<TCanvas*> & canvases() const {  return _canvases;}

  void createDirectory(const String & dirName);
  TCanvas * createCanvas(const String & canvasName, const Configuration & canvasConfig, int inc=200);
  TCanvas * createCanvasXX(int nx, int ny, const String & canvasName, const Configuration & canvasConfig, int inc=30);

  void printCanvas(TCanvas * canvas,
                   const String & directoryName="./",
                   bool printGif=0,
                   bool printPdf=1,
                   bool printSvg=0,
                   bool printPng=0,
                   bool printC=0);
  void printAllCanvas(const String & outputPath,
                      bool printGif=0,
                      bool printPdf=1,
                      bool printSvg=0,
                      bool printPng=0,
                      bool printC=0);

protected:

  std::vector<TCanvas*> _canvases;

  ClassDef(CanvasList,0)

};

} // namespace CAP

#endif /* CanvasList_hpp */
