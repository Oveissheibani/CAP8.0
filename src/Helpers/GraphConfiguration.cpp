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
#include "GraphConfiguration.hpp"

ClassImp(CAP::GraphConfiguration);

namespace CAP
{
GraphConfiguration::GraphConfiguration()
:
Configuration()
{
  setDefaultConfiguration();
}

GraphConfiguration::GraphConfiguration(const GraphConfiguration & source)
:
Configuration(source)
{   }


GraphConfiguration & GraphConfiguration::operator=(const GraphConfiguration & source)
{
  if (this!=&source)
    {
    Configuration::operator=(source);
    }
  return *this;
}

void GraphConfiguration::setDefaultConfiguration()
{
  addProperty("npx", 100);
  addProperty("npy", 100);
  addProperty("lineColor", 1);
  addProperty("lineStyle", 1);
  addProperty("lineWidth", 1);
  addProperty("markerColor",   1);
  addProperty("markerStyle",  20);
  addProperty("markerSize",  1.1);
  addProperty("systsColor", 1);
  addProperty("systsStyle", 3003);
  addProperty("systsWidth", 1);
  addProperty("nXDivisions", 5);
  addProperty("xTitleSize", 0.06);
  addProperty("xTitleOffset", 1.1);
  addProperty("xLabelSize",  0.06);
  addProperty("xLabelOffset", 0.01);
  addProperty("xTitle",       String("x"));
  addProperty("nYDivisions",     5);
  addProperty("yTitleSize",   0.06);
  addProperty("yTitleOffset",  1.1);
  addProperty("yLabelSize",   0.06);
  addProperty("yLabelOffset", 0.01);
  addProperty("yTitle",        String("y"));
  addProperty("nZDivisions",     5);
  addProperty("zTitleSize",   0.06);
  addProperty("zTitleOffset",  1.5);
  addProperty("zLabelSize",   0.05);
  addProperty("zLabelOffset", 0.01);
  addProperty("zLabelOffset", 0.01);
  addProperty("zTitle",        String("z"));
  addProperty("PlotOption",    String("E0"));

  addProperty("systsColor",1);
  addProperty("systsStyle",1001);
  addProperty("systsWidth",1);
}

void GraphConfiguration::setPropertiesFor(int dim, int type)
{
  if (dim==1 || dim==0)
    {
   
    switch (type)
      {
        default:

        case  0:
        addProperty("lineColor",   kBlack);
        addProperty("lineStyle",   kSolid);
        addProperty("lineWidth",   2);
        addProperty("markerColor", kBlack);
        addProperty("markerStyle", kFullSquare);
        addProperty("markerSize",  1.1);
        addProperty("systsColor",  kBlack);
        addProperty("systsStyle",  3003);
        addProperty("systsWidth",  1);
        break;
        
        case  1:
        addProperty("lineColor",   kRed);
        addProperty("lineStyle",   kSolid);
        addProperty("lineWidth",   2);
        addProperty("markerColor", kRed);
        addProperty("markerStyle", kFullSquare);
        addProperty("markerSize",  1.1);
        addProperty("systsColor",  kRed);
        addProperty("systsStyle",  3003);
        addProperty("systsWidth",  1);
        break;
        
        case  2:
        addProperty("lineColor",   kBlue);
        addProperty("lineStyle",   kSolid);
        addProperty("lineWidth",   2);
        addProperty("markerColor", kBlue);
        addProperty("markerStyle", kFullSquare);
        addProperty("markerSize",  1.1);
        addProperty("systsColor",  kBlue);
        addProperty("systsStyle",  3003);
        addProperty("systsWidth",  1);
        break;


        
        case  3:
        addProperty("lineColor",   kMagenta);
        addProperty("lineStyle",   kSolid);
        addProperty("lineWidth",   2);
        addProperty("markerColor", kMagenta);
        addProperty("markerStyle", kFullSquare);
        addProperty("markerSize",  1.1);
        addProperty("systsColor",  kMagenta);
        addProperty("systsStyle",  3003);
        addProperty("systsWidth",  1);
        break;
        
        case  4:
        addProperty("lineColor",   kGreen);
        addProperty("lineStyle",   kSolid);
        addProperty("lineWidth",   2);
        addProperty("markerColor", kGreen);
        addProperty("markerStyle", kFullSquare);
        addProperty("markerSize",  1.1);
        addProperty("systsColor",  kGreen);
        addProperty("systsStyle",  3003);
        addProperty("systsWidth",  1);
        break;

        case  5:
        addProperty("lineColor",   kYellow);
        addProperty("lineStyle",   kSolid);
        addProperty("lineWidth",   2);
        addProperty("markerColor", kYellow);
        addProperty("markerStyle", kFullSquare);
        addProperty("markerSize",  1.1);
        addProperty("systsColor",  kYellow);
        addProperty("systsStyle",  3003);
        addProperty("systsWidth",  1);
        break;
        
        case  6:
        addProperty("lineColor",   kPink);
        addProperty("lineStyle",   kSolid);
        addProperty("lineWidth",   2);
        addProperty("markerColor", kPink);
        addProperty("markerStyle", kFullCircle);
        addProperty("markerSize",  1.1);
        addProperty("systsColor",  kPink);
        addProperty("systsStyle",  3003);
        addProperty("systsWidth",  1);
        break;
        
        case  7:
        addProperty("lineColor",   kBlue+2);
        addProperty("lineStyle",   kSolid);
        addProperty("lineWidth",   2);
        addProperty("markerColor", kBlue+2);
        addProperty("markerStyle", kFullCircle);
        addProperty("markerSize",  1.1);
        addProperty("systsColor",  kBlue+2);
        addProperty("systsStyle",  3003);
        addProperty("systsWidth",  1);
        break;
        
        case  8:
        addProperty("lineColor",   kRed+2);
        addProperty("lineStyle",   kSolid);
        addProperty("lineWidth",   2);
        addProperty("markerColor", kRed+2);
        addProperty("markerStyle", kFullCircle);
        addProperty("markerSize",  1.1);
        addProperty("systsColor",  kRed+2);
        addProperty("systsStyle",  3003);
        addProperty("systsWidth",  1);
        break;
        
        case  9:
        addProperty("lineColor",   kGreen+2);
        addProperty("lineStyle",   kSolid);
        addProperty("lineWidth",   2);
        addProperty("markerColor", kGreen+2);
        addProperty("markerStyle", kFullCircle);
        addProperty("markerSize",  1.1);
        addProperty("systsColor",  kGreen+2);
        addProperty("systsStyle",  3003);
        addProperty("systsWidth",  1);
        break;
        

        case  10:
        addProperty("lineColor",   kBlack);
        addProperty("lineStyle",   kSolid);
        addProperty("lineWidth",   2);
        addProperty("markerColor", kBlack);
        addProperty("markerStyle", kFullCircle);
        addProperty("markerSize",  1.1);
        addProperty("systsColor",  kBlack);
        addProperty("systsStyle",  3003);
        addProperty("systsWidth",  1);
        break;
        
        case  11:
        addProperty("lineColor",   kRed);
        addProperty("lineStyle",   kSolid);
        addProperty("lineWidth",   2);
        addProperty("markerColor", kRed);
        addProperty("markerStyle", kFullCircle);
        addProperty("markerSize",  1.1);
        addProperty("systsColor",  kRed);
        addProperty("systsStyle",  3003);
        addProperty("systsWidth",  1);
        break;
        
        case  12:
        addProperty("lineColor",   kGreen);
        addProperty("lineStyle",   kSolid);
        addProperty("lineWidth",   2);
        addProperty("markerColor", kGreen);
        addProperty("markerStyle", kFullCircle);
        addProperty("markerSize",  1.1);
        addProperty("systsColor",  kGreen);
        addProperty("systsStyle",  3003);
        addProperty("systsWidth",  1);
        break;
        
        case  13:
        addProperty("lineColor",   kMagenta);
        addProperty("lineStyle",   kSolid);
        addProperty("lineWidth",   2);
        addProperty("markerColor", kMagenta);
        addProperty("markerStyle", kFullCircle);
        addProperty("markerSize",  1.1);
        addProperty("systsColor",  kMagenta);
        addProperty("systsStyle",  3003);
        addProperty("systsWidth",  1);
        break;
        
        case  14:
        addProperty("lineColor",   kBlue);
        addProperty("lineStyle",   kSolid);
        addProperty("lineWidth",   2);
        addProperty("markerColor", kBlue);
        addProperty("markerStyle", kFullCircle);
        addProperty("markerSize",  1.1);
        addProperty("systsColor",  kBlue);
        addProperty("systsStyle",  3003);
        addProperty("systsWidth",  1);
        break;
        
        case  15:
        addProperty("lineColor",   kYellow);
        addProperty("lineStyle",   kSolid);
        addProperty("lineWidth",   2);
        addProperty("markerColor", kYellow);
        addProperty("markerStyle", kFullCircle);
        addProperty("markerSize",  1.1);
        addProperty("systsColor",  kYellow);
        addProperty("systsStyle",  3003);
        addProperty("systsWidth",  1);
        break;
        
        case  16:
        addProperty("lineColor",   kPink);
        addProperty("lineStyle",   kSolid);
        addProperty("lineWidth",   2);
        addProperty("markerColor", kPink);
        addProperty("markerStyle", kFullCircle);
        addProperty("markerSize",  1.1);
        addProperty("systsColor",  kPink);
        addProperty("systsStyle",  3003);
        addProperty("systsWidth",  1);
        break;
        
        case  17:
        addProperty("lineColor",   kBlue+2);
        addProperty("lineStyle",   kSolid);
        addProperty("lineWidth",   2);
        addProperty("markerColor", kBlue+2);
        addProperty("markerStyle", kFullCircle);
        addProperty("markerSize",  1.1);
        addProperty("systsColor",  kBlue+2);
        addProperty("systsStyle",  3003);
        addProperty("systsWidth",  1);
        break;
        
        case  18:
        addProperty("lineColor",   kRed+2);
        addProperty("lineStyle",   kSolid);
        addProperty("lineWidth",   2);
        addProperty("markerColor", kRed+2);
        addProperty("markerStyle", kFullCircle);
        addProperty("markerSize",  1.1);
        addProperty("systsColor",  kRed+2);
        addProperty("systsStyle",  3003);
        addProperty("systsWidth",  1);
        break;
        
        case  19:
        addProperty("lineColor",   kGreen+2);
        addProperty("lineStyle",   kSolid);
        addProperty("lineWidth",   2);
        addProperty("markerColor", kGreen+2);
        addProperty("markerStyle", kFullCircle);
        addProperty("markerSize",  1.1);
        addProperty("systsColor",  kGreen+2);
        addProperty("systsStyle",  3003);
        addProperty("systsWidth",  1);
        break;
        
        case  20:
        addProperty("lineColor",   kBlack);
        addProperty("lineStyle",   kSolid);
        addProperty("lineWidth",   2);
        addProperty("markerColor", kBlack);
        addProperty("markerStyle", kFullDiamond);
        addProperty("markerSize",  1.1);
        addProperty("systsColor",  kBlack);
        addProperty("systsStyle",  3003);
        addProperty("systsWidth",  1);
        break;
        
        case  21:
        addProperty("lineColor",   kRed);
        addProperty("lineStyle",   kSolid);
        addProperty("lineWidth",   2);
        addProperty("markerColor", kRed);
        addProperty("markerStyle", kFullDiamond);
        addProperty("markerSize",  1.1);
        addProperty("systsColor",  kRed);
        addProperty("systsStyle",  3003);
        addProperty("systsWidth",  1);
        break;
        
        case  22:
        addProperty("lineColor",   kGreen);
        addProperty("lineStyle",   kSolid);
        addProperty("lineWidth",   2);
        addProperty("markerColor", kGreen);
        addProperty("markerStyle", kFullDiamond);
        addProperty("markerSize",  1.1);
        addProperty("systsColor",  kGreen);
        addProperty("systsStyle",  3003);
        addProperty("systsWidth",  1);
        break;
        
        case  23:
        addProperty("lineColor",   kMagenta);
        addProperty("lineStyle",   kSolid);
        addProperty("lineWidth",   2);
        addProperty("markerColor", kMagenta);
        addProperty("markerStyle", kFullDiamond);
        addProperty("markerSize",  1.1);
        addProperty("systsColor",  kMagenta);
        addProperty("systsStyle",  3003);
        addProperty("systsWidth",  1);
        break;
        
        case  24:
        addProperty("lineColor",   kBlue);
        addProperty("lineStyle",   kSolid);
        addProperty("lineWidth",   2);
        addProperty("markerColor", kBlue);
        addProperty("markerStyle", kFullDiamond);
        addProperty("markerSize",  1.1);
        addProperty("systsColor",  kBlue);
        addProperty("systsStyle",  3003);
        addProperty("systsWidth",  1);
        break;
        
        case  25:
        addProperty("lineColor",   kYellow);
        addProperty("lineStyle",   kSolid);
        addProperty("lineWidth",   2);
        addProperty("markerColor", kYellow);
        addProperty("markerStyle", kFullDiamond);
        addProperty("markerSize",  1.1);
        addProperty("systsColor",  kYellow);
        addProperty("systsStyle",  3003);
        addProperty("systsWidth",  1);
        break;
        
        case  26:
        addProperty("lineColor",   kPink);
        addProperty("lineStyle",   kSolid);
        addProperty("lineWidth",   2);
        addProperty("markerColor", kPink);
        addProperty("markerStyle", kFullDiamond);
        addProperty("markerSize",  1.1);
        addProperty("systsColor",  kPink);
        addProperty("systsStyle",  3003);
        addProperty("systsWidth",  1);
        break;
        
        case  27:
        addProperty("lineColor",   kBlue+2);
        addProperty("lineStyle",   kSolid);
        addProperty("lineWidth",   2);
        addProperty("markerColor", kBlue+2);
        addProperty("markerStyle", kFullDiamond);
        addProperty("markerSize",  1.1);
        addProperty("systsColor",  kBlue+2);
        addProperty("systsStyle",  3003);
        addProperty("systsWidth",  1);
        break;
        
        case  28:
        addProperty("lineColor",   kRed+2);
        addProperty("lineStyle",   kSolid);
        addProperty("lineWidth",   2);
        addProperty("markerColor", kRed+2);
        addProperty("markerStyle", kFullDiamond);
        addProperty("markerSize",  1.1);
        addProperty("systsColor",  kRed+2);
        addProperty("systsStyle",  3003);
        addProperty("systsWidth",  1);
        break;
        
        case  29:
        addProperty("lineColor",   kGreen+2);
        addProperty("lineStyle",   kSolid);
        addProperty("lineWidth",   2);
        addProperty("markerColor", kGreen+2);
        addProperty("markerStyle", kFullDiamond);
        addProperty("markerSize",  1.1);
        addProperty("systsColor",  kGreen+2);
        addProperty("systsStyle",  3003);
        addProperty("systsWidth",  1);
        break;
        
        }
    }
  else if (dim==2)
    {
    addProperty("nXDivisions", 5);
    addProperty("xTitleSize", 0.05);
    addProperty("xTitleOffset", 1.3);
    addProperty("xLabelSize",  0.05);
    addProperty("xLabelOffset", 0.002);
    addProperty("xTitle",        String("x"));
    addProperty("nYDivisions",     5);
    addProperty("yTitleSize",   0.05);
    addProperty("yTitleOffset",  1.5);
    addProperty("yLabelSize",   0.05);
    addProperty("yLabelOffset", 0.05);
    addProperty("yTitle",        String("y"));
    addProperty("nZDivisions",     5);
    addProperty("zTitleSize",   0.05);
    addProperty("zTitleOffset",  1.5);
    addProperty("zLabelSize",   0.05);
    addProperty("zLabelOffset",0.005);
    addProperty("zTitle",        String("z"));
    addProperty("PlotOption",String("SURF3"));
    }
}

std::vector<GraphConfiguration> GraphConfiguration::createConfigurationPalette(unsigned int n, int dimension)
{
  std::vector<GraphConfiguration> configs;

  for (unsigned int k=0; k<n; k++)
    {
    //GraphConfiguration * c = new GraphConfiguration(dimension, k);
    GraphConfiguration config;
    config.setPropertiesFor(dimension,n);
    configs.push_back( config );
    }
  return configs;
}

} // namespace CAP
