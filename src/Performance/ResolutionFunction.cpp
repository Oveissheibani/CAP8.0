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
#include "ResolutionFunction.hpp"
#include "PrintHelpers.hpp"
#include "TRandom.h"
#include "Exceptions.hpp"

ClassImp(CAP::ResolutionFunction);

namespace CAP
{

ResolutionFunction::ResolutionFunction()
:
option(0),
biasVsPt(),
biasVsEta(),
rmsVsPt(),
rmsVsEta()
{
  // set no bias and no smearing...

  biasVsPt.push_back(0.0);
  biasVsPt.push_back(0.0);
  biasVsPt.push_back(0.0);
  biasVsPt.push_back(0.0);
  biasVsEta.push_back(0.0);
  biasVsEta.push_back(0.0);

  rmsVsPt.push_back(0.0);
  rmsVsPt.push_back(0.0);
  rmsVsPt.push_back(0.0);
  rmsVsPt.push_back(0.0);
  rmsVsEta.push_back(0.0);
  rmsVsEta.push_back(0.0);
}

ResolutionFunction::ResolutionFunction(const ResolutionFunction & rf)
:
option(rf.option),
biasVsPt(rf.biasVsPt),
biasVsEta(rf.biasVsEta),
rmsVsPt(rf.rmsVsPt),
rmsVsEta(rf.rmsVsEta)
{    }

ResolutionFunction ResolutionFunction::operator=(const ResolutionFunction & rf)
{
  if (this!=&rf)
  {
  option   = rf.option;
  biasVsPt = rf.biasVsPt;
  biasVsPt = rf.biasVsEta;
  biasVsPt = rf.rmsVsPt;
  biasVsPt = rf.rmsVsEta;
  }
  return *this;
}

void ResolutionFunction::setCoefficients(int _option,
                                         const std::vector<double> & _biasVsPt,
                                         const std::vector<double> & _biasVsEta,
                                         const std::vector<double> & _rmsVsPt,
                                         const std::vector<double> & _rmsVsEta)
{
  option     = _option;
  biasVsPt   = _biasVsPt;
  biasVsEta  = _biasVsEta;
  rmsVsPt    = _rmsVsPt;
  rmsVsEta   = _rmsVsEta;
  if (biasVsPt.size() !=4) throw Exception("biasVsPt.size()!=4",__FUNCTION__);
  if (biasVsEta.size()!=2) throw Exception("biasVsEta.size()!=2",__FUNCTION__);
  if (rmsVsPt.size()  !=4) throw Exception("rmsVsPt.size()!=4",__FUNCTION__);
  if (rmsVsEta.size() !=2) throw Exception("rmsVsEta.size()!=2",__FUNCTION__);
}

double ResolutionFunction::smear(double pt, double eta, double phi, double & output)
{
  double ptSq = pt*pt;
  double etaSq = eta*eta;
  double bias = biasVsPt[0]/pt + biasVsPt[1] + biasVsPt[2]*pt + biasVsPt[3]*ptSq + biasVsEta[0]*eta + biasVsEta[1]*etaSq;
  double rms  = rmsVsPt[0]/pt  + rmsVsPt[1]  + rmsVsPt[2]*pt  + rmsVsPt[3]*ptSq  + rmsVsEta[0]*eta  + rmsVsEta[1]*etaSq;
//  if (option==0)
//    {
//    printValue("option",option);
//    printValue("pt",pt);
//    printValue("eta",eta);
//    printValue("phi",phi);
//    printValue("bias",bias);
//    printValue("rms",rms);
//    }
  switch (option)
    {
      case 0: output = gRandom->Gaus(pt+bias,rms);  break;
      case 1: output = gRandom->Gaus(eta+bias,rms); break;
      case 2: output = gRandom->Gaus(phi+bias,rms); break;
    }
  return output;
}

} // namespace CAP
