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
using CAP::CanvasConfiguration;

ClassImp(CanvasConfiguration);

CanvasConfiguration::CanvasConfiguration()
:
Configuration()
{
  setDefaultConfiguration();
}


//enum CanvasFormat   { PortraitTight, Portrait, PortraitWide, SquareTight, Square, SquareWide, LandscapeTight, Landscape, LandscapeWide, LandscapeXtop };
//enum CanvasAxes     { Linear, LogX, LogY, LogZ, LogXY, LogXZ, LogYZ, LogXYZ  };
CanvasConfiguration::CanvasConfiguration(CanvasFormat format, CanvasAxes axes)
:
Configuration()
{
  setDefaultConfiguration();
  // chose the aspect ratio and the margin style (tight, regular, wide)
  setFormat(format);
  // chose the axes settings
  setAxes(axes);
}


//!
//! Copy CTOR
//!
CanvasConfiguration::CanvasConfiguration(const Configuration & config)
:
Configuration(config)
{}

////////////////////////////////////////////////////
// Assignment operator
////////////////////////////////////////////////////
CanvasConfiguration & CanvasConfiguration::operator=(const Configuration & config)
{
  if (this!=&config) Configuration::operator=(config);
  return *this;
}

CanvasConfiguration::~CanvasConfiguration()
{
}

void CanvasConfiguration::setDefaultConfiguration()
{
  addProperty("windowXPosition",     20);
  addProperty("windowYPosition",     20);
  addProperty("windowWidth",        700);
  addProperty("windowHeight",       700);
  addProperty("windowLeftMargin",  0.15);
  addProperty("windowTopMargin",   0.03);
  addProperty("windowRightMargin", 0.03);
  addProperty("windowBottomMargin",0.15);
  addProperty("windowLogX",  false);
  addProperty("windowLogY",  false);
  addProperty("windowLogZ",  false);
  addProperty("windowTheta", 35.0);
  addProperty("windowPhi",   25.0);
  addProperty("useTickx",    true);
  addProperty("useTicky",    true);
  addProperty("useGridx",    false);
  addProperty("useGridy",    false);

  addProperty("windowFillColor", kWhite);
  addProperty("windowFillStyle", 1001);
  addProperty("windowBorderSize",0);
  addProperty("windowBorderMode",0);
}


void CanvasConfiguration::setFormat(CanvasFormat format)
{
  switch (format)
    {
      case PortraitTight:
      addProperty("windowXPosition",     20);
      addProperty("windowYPosition",     20);
      addProperty("windowWidth",        500);
      addProperty("windowHeight",       800);
      addProperty("windowLeftMargin",  0.10);
      addProperty("windowTopMargin",   0.03);
      addProperty("windowRightMargin", 0.03);
      addProperty("windowBottomMargin",0.10);
      break;
    
      case Portrait:
      addProperty("windowXPosition",     20);
      addProperty("windowYPosition",     20);
      addProperty("windowWidth",        500);
      addProperty("windowHeight",       800);
      addProperty("windowLeftMargin",  0.15);
      addProperty("windowTopMargin",   0.05);
      addProperty("windowRightMargin", 0.05);
      addProperty("windowBottomMargin",0.15);
      break;

      case PortraitWide:
      addProperty("windowXPosition",     20);
      addProperty("windowYPosition",     20);
      addProperty("windowWidth",        500);
      addProperty("windowHeight",       800);
      addProperty("windowLeftMargin",  0.20);
      addProperty("windowTopMargin",   0.10);
      addProperty("windowRightMargin", 0.10);
      addProperty("windowBottomMargin",0.20);
      break;

      case SquareTight:
      addProperty("windowXPosition",     20);
      addProperty("windowYPosition",     20);
      addProperty("windowWidth",        700);
      addProperty("windowHeight",       700);
      addProperty("windowLeftMargin",  0.10);
      addProperty("windowTopMargin",   0.03);
      addProperty("windowRightMargin", 0.03);
      addProperty("windowBottomMargin",0.10);
      break;

      case Square:
      addProperty("windowXPosition",     20);
      addProperty("windowYPosition",     20);
      addProperty("windowWidth",        700);
      addProperty("windowHeight",       700);
      addProperty("windowLeftMargin",  0.15);
      addProperty("windowTopMargin",   0.05);
      addProperty("windowRightMargin", 0.05);
      addProperty("windowBottomMargin",0.15);
      break;

      case SquareWide:
      addProperty("windowXPosition",     20);
      addProperty("windowYPosition",     20);
      addProperty("windowWidth",        700);
      addProperty("windowHeight",       700);
      addProperty("windowLeftMargin",  0.20);
      addProperty("windowTopMargin",   0.05);
      addProperty("windowRightMargin", 0.10);
      addProperty("windowBottomMargin",0.20);
      break;

      case LandscapeTight:
      addProperty("windowXPosition",     20);
      addProperty("windowYPosition",     20);
      addProperty("windowWidth",        800);
      addProperty("windowHeight",       500);
      addProperty("windowLeftMargin",  0.10);
      addProperty("windowTopMargin",   0.03);
      addProperty("windowRightMargin", 0.03);
      addProperty("windowBottomMargin",0.10);
      break;

      case Landscape:
      addProperty("windowXPosition",     20);
      addProperty("windowYPosition",     20);
      addProperty("windowWidth",        800);
      addProperty("windowHeight",       500);
      addProperty("windowLeftMargin",  0.15);
      addProperty("windowTopMargin",   0.05);
      addProperty("windowRightMargin", 0.10);
      addProperty("windowBottomMargin",0.15);
      break;
      
      case LandscapeWide:
      addProperty("windowXPosition",     20);
      addProperty("windowYPosition",     20);
      addProperty("windowWidth",        800);
      addProperty("windowHeight",       500);
      addProperty("windowLeftMargin",  0.20);
      addProperty("windowTopMargin",   0.05);
      addProperty("windowRightMargin", 0.15);
      addProperty("windowBottomMargin",0.15);
      break;

      case LandscapeXtop:
      addProperty("windowXPosition",     20);
      addProperty("windowYPosition",     20);
      addProperty("windowWidth",        800);
      addProperty("windowHeight",       600);
      addProperty("windowLeftMargin",  0.20);
      addProperty("windowTopMargin",   0.20);
      addProperty("windowRightMargin", 0.05);
      addProperty("windowBottomMargin",0.15);
      break;

    };
}


void CanvasConfiguration::setAxes(CanvasAxes axes)
{
  switch (axes)
    {
      case Linear:
      addProperty("windowLogX", false);
      addProperty("windowLogY", false);
      addProperty("windowLogZ", false);
      break;
     
      case LogX:
      addProperty("windowLogX", true);
      addProperty("windowLogY", false);
      addProperty("windowLogZ", false);
      break;
      
      case LogY:
      addProperty("windowLogX", false);
      addProperty("windowLogY", true);
      addProperty("windowLogZ", false);
      break;

      case LogZ:
      addProperty("windowLogX", false);
      addProperty("windowLogY", false);
      addProperty("windowLogZ", true);
      break;
      
      case LogXY:
      addProperty("windowLogX", true);
      addProperty("windowLogY", true);
      addProperty("windowLogZ", false);
      break;
      
      case LogXZ:
      addProperty("windowLogX", true);
      addProperty("windowLogY", false);
      addProperty("windowLogZ", true);
      break;
      
      case LogYZ:
      addProperty("windowLogX", false);
      addProperty("windowLogY", true);
      addProperty("windowLogZ", true);
      break;
      
      case LogXYZ:
      addProperty("windowLogX", true);
      addProperty("windowLogY", true);
      addProperty("windowLogZ", true);
      break;
    };
}

void CanvasConfiguration::setMargins(float left, float top, float right, float bottom)
{
  addProperty("windowLeftMargin",  left);
  addProperty("windowTopMargin",   top);
  addProperty("windowRightMargin", right);
  addProperty("windowBottomMargin",bottom);
}

void CanvasConfiguration::setSize(int width, int heigh)
{
  addProperty("windowWidth",        width);
  addProperty("windowHeight",       heigh);
}

void CanvasConfiguration::setPosition(int x, int y)
{
  addProperty("windowXPosition",x);
  addProperty("windowYPosition",y);
}
