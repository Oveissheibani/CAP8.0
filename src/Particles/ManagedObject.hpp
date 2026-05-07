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
#ifndef CAP__ManagedObject
#define CAP__ManagedObject
#include "Object.hpp"
#include "Configuration.hpp"

namespace CAP
{

  class ManagedObject
  :
  public Object
  {
  public:

  ManagedObject();
  ManagedObject(const ManagedObject & source);
  ManagedObject & operator=(const ManagedObject & rhs);
  virtual ~ManagedObject() {}
  virtual void configure(const String & taskName  __attribute__((unused)),
                               const String & objectType  __attribute__((unused)),
                               const Configuration & configuration  __attribute__((unused)),
                               unsigned int index  __attribute__((unused))){}

  virtual void initialize() {}
  virtual void create() {}
  virtual void loadFrom(TFile & file __attribute__((unused))) {}
  virtual void loadFromAscii(std::ifstream & file __attribute__((unused))) {}
  virtual void loadFromAscii2(std::ifstream & file1 __attribute__((unused)),
                                  std::ifstream & file2 __attribute__((unused))) {}
  virtual void saveTo(TFile & file __attribute__((unused))) {}
  virtual void saveToAscii(std::ofstream & file __attribute__((unused))) {}
  virtual void saveToAscii2(std::ofstream & file1 __attribute__((unused)),
                                  std::ofstream & file2 __attribute__((unused))) {}

  virtual void scaleObject(double factor __attribute__((unused))) { }
  virtual void reset(){ }
  virtual void clear(){ }
  virtual void print() const {}

  protected:

  ClassDef(ManagedObject,0)
  };

}

#endif /* CAP__ManagedObject */

