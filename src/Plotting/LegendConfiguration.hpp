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
#ifndef CAP__LegendConfiguration
#define CAP__LegendConfiguration
#include <iostream>
#include <vector>
#include "Aliases.hpp"
#include "TLatex.h"
#include "Configuration.hpp"
using std::vector;

namespace CAP
{


//!
//! Class encapsulates the  properties or attributes of the legend of a graph.
//!
class LegendConfiguration : public Configuration
{
public:

 LegendConfiguration();
 LegendConfiguration(double _xLeft,
                      double _xRight,
                      double _yLow,
                      double _yHigh,
                      double _textSize);

  LegendConfiguration(double _xLeft,
                      double _xRight,
                      double _yLow,
                      double _yHigh,
                      double _textSize,
                      VectorString & _entries);
  //!
  //!Copy CTOR
  //!
  LegendConfiguration(const LegendConfiguration & source);

  //!
  //!DTOR
  //!
  virtual ~LegendConfiguration(){}

  //!
  //!Assignment operator (COPY)
  //!
  LegendConfiguration & operator=(const LegendConfiguration & source);
  
  //!
  //! Set the default graph configuration
  //!
  virtual void setDefaultConfiguration();

  void addLegend(const String & legend);
  String legendAt(int index) const;
  void setLegendAt(int index,const String & legend);

  void clearLegend()
  {
  legends.clear();
  }

  bool useLegend() const
  {
  return valueBool("UseLegend");
  }


  bool useLabels()  const
  {
    return valueBool("useLabels")  &&  labels.size()>0;
  }

  void clearLabels()
  {
  labels.clear();
  }


  int nLabels() const
  {
  return labels.size();
  }

  void addLabel(const String & text, double x, double y, double angle, int color,  double fontSize);

  TLatex * labelAt(int index) const;

  void drawLabels();

  VectorString  texts() const
  {
  return legends;
  }

protected:

//  bool drawLegend;
//  bool drawBorder;
//  int  nColumns;
//  int  borderColor;
//  int  borderSize;
//  int  fillColor;
//  int  fillStyle;
//  int  fontIndex;
//  int  fontColor;
//  int  fontSize;
//  int  textAlign;
//  double xLeft;
//  double xRight;
//  double yLow;
//  double yHigh;
//  String header;

  VectorString  legends;
  vector<TLatex*> labels;

  ClassDef(LegendConfiguration,0)
};

} // namespace CAP

#endif
