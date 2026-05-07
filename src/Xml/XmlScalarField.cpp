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
#include "XmlScalarField.hpp"
#include "StreamHelpers.hpp"
#include "Exceptions.hpp"

ClassImp(CAP::XmlScalarField);

namespace CAP
{

  XmlScalarField::XmlScalarField()
  :
  XmlDocument()
  {   }

  XmlScalarField::XmlScalarField(const String & _inputPath,
                                 const String & _inputFileName)
  :
  XmlDocument(_inputPath,_inputFileName)
  {   }

  XmlScalarField::XmlScalarField(const String & _inputFile)
  :
  XmlDocument(_inputFile)
  {   }


  MATH::ScalarField XmlScalarField::getXmlScalarField()
  {
  MATH::ScalarField scalarField;
  String   vName;
  String   vAxis[3];
  double   vMin[3];
  double   vMax[3];
  int      vPts[3];

  if (!currentTag || !(currentTag->name == "VECTOR3D") )
    throw FileException("Bad tag","ScalarField::setValue(...)",__FUNCTION__);

  vName = getXmlAttribute("name"); currentTag = currentTag->child; // goto first AXIS
                                                                   // extract AXIS Tags
  for(int i=0; i<3; i++)
    {
    vAxis[i] = getXmlAttribute("name"); currentTag = currentTag->child; // goto <DETAIL name="min">
    vMin[i]  = getXmlContent().Atof(); currentTag = currentTag->next;  // goto <DETAIL name="ampx">
    vMax[i]  = getXmlContent().Atof(); currentTag = currentTag->next;  // goto <DETAIL name="pts">
    vPts[i]  = getXmlContent().Atoi(); currentTag = currentTag->parent->next; // goto next AXIS or DATA
    }
  // extract ARRAY Tag -> ScalarField object
  // CAP 3/18/2023
  // instead of instantiating directly, invoke a factory and set the object
  // the factory then handles the memory ampnagement.
  //  ScalarField = new ScalarField(vName.Data(),vMin[0],vampx[0],vPts[0],vMin[1],vampx[1],vPts[1],vMin[2],vampx[2],vPts[2]);
  // memory leak here -- beware...

  scalarField.setValue(vName.Data(),vMin[0],vMax[0],vPts[0],vMin[1],vMax[1],vPts[1],vMin[2],vMax[2],vPts[2]);
  XmlParser parser;
  std::ifstream & inputFile = openInputAsciiFile(inputPath,inputFileName,".xml");
  inputFile.seekg(currentTag->begin);
  inputFile.precision(16);
  String buffer = "";
  double value;
  //char   chr;

  for(int i=0; i<vPts[0]; i++)
    for(int j=0; j<vPts[1]; j++)
      for(int k=0; k<vPts[2]; k++)
        {
        inputFile >> value;
        scalarField(i,j,k) = value;
        }
  inputFile.close();
  return scalarField;
  }

} // namespace CAP
