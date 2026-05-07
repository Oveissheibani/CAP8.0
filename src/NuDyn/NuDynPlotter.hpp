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
#ifndef CAP__NuDynPlotter
#define CAP__NuDynPlotter
#include "Plotter.hpp"
#include "NuDynHistos.hpp"
#include "NuDynDerivedHistos.hpp"

namespace CAP
{


class NuDynPlotter : public Plotter
{
public:
  
  NuDynPlotter();
  virtual ~NuDynPlotter();
  void makePlots(const String & canvasNameBase,
                 NuDynHistos * nuDynHistosUsed,
                 NuDynDerivedHistos * nuDynDerivedHistosUsed,
                 CanvasConfiguration & canvasConfigurationUsed,
                 GraphConfiguration  ** graphConfigurationsUsed);

  void makeComparisonPlots(const String & canvasNameBase,
                           Configuration & canvasConfiguration,
                           int nGraphs,
                           NuDynHistos ** nuDynHistos,
                           NuDynDerivedHistos ** nuDynDerivedHistos,
                           String ** histLabels,
                           GraphConfiguration  ** graphConfigurations);

  void makeNudynComparisonPlots(const String & canvasNameBase,
                                Configuration & canvasConfiguration,
                                int nGraphs,
                                NuDynHistos ** nuDynHistos,
                                NuDynDerivedHistos ** nuDynDerivedHistos,
                                String ** histLabels,
                                GraphConfiguration  ** graphConfigurations);

  ClassDef(NuDynPlotter,0)
};


} // namespace CAP

#endif /* CAP__NuDynPlotter */
