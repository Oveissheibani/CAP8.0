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
#ifndef CAP__XmlScalarField
#define CAP__XmlScalarField
#include "XmlDocument.hpp"
#include "ScalarField.hpp"

namespace CAP
{

class XmlScalarField : public XmlDocument
{
public:

  friend XmlParser;

  XmlScalarField();
  XmlScalarField(const String & _inputPath,
                 const String & _inputFileName);
  XmlScalarField(const String & _inputFile);
  virtual ~XmlScalarField() {}
  MATH::ScalarField getXmlScalarField();
  ClassDef(XmlScalarField,0)

};



} // namespace CAP

#endif /* XmlDocument */
