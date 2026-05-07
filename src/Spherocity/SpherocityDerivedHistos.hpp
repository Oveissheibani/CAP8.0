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
#ifndef CAP__SpherocityDerivedHistos
#define CAP__SpherocityDerivedHistos
#include "SpherocityHistos.hpp"

namespace CAP
{
      
class SpherocityDerivedHistos: public SpherocityHistos
{
public:
  SpherocityDerivedHistos();
  SpherocityDerivedHistos(const SpherocityDerivedHistos & group);
  SpherocityDerivedHistos & operator=(const SpherocityDerivedHistos & group);
  virtual ~SpherocityDerivedHistos() {}
  virtual void configure(const String & taskName,
                               const String & objectType,
                               const Configuration & configuration,
                               unsigned int index);
  virtual void create();
  virtual void loadFrom(TFile & inputFile);
  virtual void calculateDerivedHistograms(SpherocityHistos & baseHistos);

protected:

  void cloneD(const SpherocityDerivedHistos & group);
   
  ClassDef(SpherocityDerivedHistos, 1);
};

} // namespace CAP


#endif
