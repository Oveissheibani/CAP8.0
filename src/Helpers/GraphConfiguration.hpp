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
#ifndef CAP__GraphConfiguration
#define CAP__GraphConfiguration
//#include "Aliases.hpp"
#include "Configuration.hpp"
#include <vector>

namespace CAP
{


//!
//! Class encapsulates the plotting properties or attributes of a graph or histograms.
//! This class is designed to work with other CAP graphics classes to set the plotting
//! attributes of graphs (TGraph), histograms (TH1*), and data graphs (DataGraph) displaying
//! data values, statistical error bars, and systematic error bars.
//!
//! Lines are used to draw connections between points as well as  (statistical) error bars.
//! Markers are used to represent the data point position/value
//! Systs are used to represent the systematic error bars or bounds.
//!
class GraphConfiguration : public Configuration
{
public:
  GraphConfiguration();
  GraphConfiguration(const GraphConfiguration & source);
  virtual ~GraphConfiguration(){}
  GraphConfiguration & operator=(const GraphConfiguration & source);
  
  virtual void setDefaultConfiguration();
  virtual void setPropertiesFor(int dim, int type);

  static std::vector<GraphConfiguration> createConfigurationPalette(unsigned int n, int dimension);

  ClassDef(GraphConfiguration,0)
};

} // namespace CAP

#endif
