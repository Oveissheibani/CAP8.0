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
#ifndef CAP__ResolutionFunction
#define CAP__ResolutionFunction
#include "HistogramGroup.hpp"

namespace CAP
{

class ResolutionFunction
{
public:
   
  ResolutionFunction();
  ResolutionFunction(const ResolutionFunction & f);
  ResolutionFunction operator=(const ResolutionFunction & rf);
  ~ResolutionFunction() {}
  void setCoefficients(int _option,
                       const std::vector<double> & _biasVsPt,
                       const std::vector<double> & _biasVsEta,
                       const std::vector<double> & _rmsVsPt,
                       const std::vector<double> & _rmsVsEta);
  double smear(double pt, double eta, double phi, double & output);

protected:

  int option;
  std::vector<double> biasVsPt;
  std::vector<double> biasVsEta;
  std::vector<double> rmsVsPt;
  std::vector<double> rmsVsEta;
};

} // namespace CAP

#endif /* CAP__ResolutionFunction */
