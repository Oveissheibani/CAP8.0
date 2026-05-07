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

#include "LegendConfiguration.hpp"
using CAP::LegendConfiguration;
using CAP::String;

ClassImp(LegendConfiguration);

LegendConfiguration::LegendConfiguration()
:
legends(),
labels()
{
  setDefaultConfiguration();
}

LegendConfiguration::LegendConfiguration(double _xLeft,
                                         double _xRight,
                                         double _yLow,
                                         double _yHigh,
                                         double _textSize)
:
legends(),
labels()
{
  setDefaultConfiguration();
  addProperty("textSize",_textSize);
  addProperty("xLeft",   _xLeft);
  addProperty("xRight",  _xRight);
  addProperty("yLow",    _yLow);
  addProperty("yHigh",   _yHigh);
}

LegendConfiguration::LegendConfiguration(double _xLeft,
                                         double _xRight,
                                         double _yLow,
                                         double _yHigh,
                                         double _textSize,
                                         VectorString & _entries)
:
legends(),
labels()
{
  setDefaultConfiguration();
  addProperty("textSize",_textSize);
  addProperty("xLeft",   _xLeft);
  addProperty("xRight",  _xRight);
  addProperty("yLow",    _yLow);
  addProperty("yHigh",   _yHigh);

  int n = _entries.size();
  if (n>0) addProperty("useTitles",false);
  for (int k=0; k<n; k++)
    {
    legends.push_back(_entries[k]);
    }
}


void LegendConfiguration::setDefaultConfiguration()
{
  addProperty("useLegend",    false);
  addProperty("useLabels",    false);
  addProperty("drawLegend",   true);
  addProperty("useNDC",false); // use PAD coordinates by defaul not NDC

  //addProperty("drawBorder",   false);
  addProperty("useTitles",    true);
  addProperty("nColumns",     1);
  addProperty("borderColor",  0);
  addProperty("borderSize",   0);
  addProperty("fillColor",    5);
  addProperty("fillStyle",    0);
  addProperty("textIndex",    22);
  addProperty("textColor",    1);
  addProperty("textSize",     0.05);
  addProperty("textAlign",    12);
  addProperty("xLeft",        0.1);
  addProperty("xRight",       0.2);
  addProperty("yLow",         0.1);
  addProperty("yHigh",        0.2);
  addProperty("header",       String(""));
}

LegendConfiguration::LegendConfiguration(const LegendConfiguration & source)
:
Configuration(source),
legends(source.legends),
labels(source.labels)
{

}


LegendConfiguration & LegendConfiguration::operator=(const LegendConfiguration & source)
{
  if (this!=&source)
    {
    Configuration::operator=(source);
    legends = source.legends;
    labels  = source.labels;
    }
  return *this;
}

void LegendConfiguration::addLegend(const String & legend)
{
  legends.push_back(legend);
}

String LegendConfiguration::legendAt(int index)  const
{
  if (index<0 || index>=int(legends.size()))
    return String("NotDefined");
  else
    return legends[index];
}

void LegendConfiguration::setLegendAt(int index,const String & legend)
{
  if (index<0)
    return;
  else if (index < int(legends.size()))
    legends[index] = legend;
  else
    {
    for (int k=0; k<(index-int(legends.size())); k++) legends.push_back(String(""));
    legends.push_back(legend);
    }
}


void LegendConfiguration::addLabel(const String & text, double x, double y, double angle, int color,  double fontSize)
{
  TLatex * label = new TLatex(x,y,text);
  label->SetTextColor(color);
  label->SetTextAngle(angle);
  label->SetTextSize(fontSize);
  labels.push_back(label);
}

TLatex * LegendConfiguration::labelAt(int index) const
{
  if (index<0 || index>=int(labels.size()))
    {
    return nullptr;
    }
  else
    return labels[index];
}

void LegendConfiguration::drawLabels()
{
  for (int k=0; k<int(labels.size()); k++) labels[k]->Draw();
}
