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
#ifndef CAP__Plotter
#define CAP__Plotter
#include "TSystem.h"
#include "TStyle.h"
#include "TGraph.h"

#include "CanvasList.hpp"
#include "CanvasConfiguration.hpp"
#include "GraphConfiguration.hpp"
//#include "LegendConfiguration.hpp"
#include "Task.hpp"

namespace CAP
{

class Plotter 
:
public Task,
public CanvasList
{
public:

  Plotter();
  Plotter(const Plotter & task);
  Plotter operator=(const Plotter & task);
  virtual ~Plotter() {}

  //!
  //! Initialize the configuration parameter of the task to their default value;
  //!
  virtual void setDefaultConfiguration();

  //!
  //! Configure  this analysis task
  //!
  virtual void configure(const Configuration & configuration);

  TCanvas *  plot(const String & _canvasName,
                  vector<TH1*> _histograms,
                  const String & _xTitle="",  double _xMin=1.0, double _xMax=0.0,
                  const String & _yTitle="",  double _yMin=1.0, double _yMax=0.0,
                  bool   _createLegend=false,
                  double _xLegendLeft=0.1,
                  double _xLegendRight=0.2,
                  double _yLegendLow=0.4,
                  double _yLegendHigh=0.5,
                  double _legendTextSize=0.05,
                  int    canvasIndex=0,
                  bool   overlay=true);

  TCanvas *  plot(const String & _canvasName,
                  vector<TH2*> _histograms,
                  const String & _xTitle="",  double _xMin=1.0, double _xMax=0.0,
                  const String & _yTitle="",  double _yMin=1.0, double _yMax=0.0,
                  const String & _zTitle="",  double _zMin=1.0, double _zMax=0.0,
                  bool   _createLegend=false,
                  double _xLegendLeft=0.1,
                  double _xLegendRight=0.2,
                  double _yLegendLow=0.4,
                  double _yLegendHigh=0.5,
                  double _legendTextSize=0.05,
                  int    canvasIndex=0);


  void setProperties(TH1 * h,
                     const GraphConfiguration & graphConfiguration,
                     const String & xTitle="none",
                     const String & yTitle="none",
                     const String & zTitle="none");

  void setProperties(const vector<TH1*> & histograms,
                     const vector<GraphConfiguration> & graphConfigurations,
                     const String & xTitle="none",
                     const String & yTitle="none");

  void setProperties(const vector<TH2*> & histograms,
                     const vector<GraphConfiguration> & graphConfigurations,
                     const String & xTitle="none",
                     const String & yTitle="none",
                     const String & zTitle="none");

  void setProperties(TGraph * g,
                     const GraphConfiguration & graphConfiguration,
                     const String & xTitle="",
                     const String & yTitle="");

  void setProperties(const vector<TGraph*> & graphs,
                     const vector<GraphConfiguration> & graphConfigurations,
                     const String & xTitle="",
                     const String & yTitle="");

  vector<GraphConfiguration> createGraphConfigurationPalette(int n, int dim);

  void setDefaultOptions(bool color);

  void addCanvasConfiguartion(CanvasConfiguration::CanvasFormat format,
                              CanvasConfiguration::CanvasAxes axes);
  void addGraphConfiguration(unsigned int n, unsigned int nDimensions);

protected:

  vector<CanvasConfiguration> canvasPalette;
  vector<GraphConfiguration> graphPalette;

  ClassDef(Plotter,0)
};

} // namespace CAP

#endif /* CAP__Plotter */
