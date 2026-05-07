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
#include "PropertiesParser.hpp"
#include "PrintHelpers.hpp"
#include "StreamHelpers.hpp"

ClassImp(CAP::PropertiesParser);

namespace CAP
{

PropertiesParser::PropertiesParser()
:
Parser()
{
  appendClassName("PropertiesParser");
  setName("PropertiesParser");
}


void  PropertiesParser::loadFrom(Properties & _properties,
                                     const String & _inputPath,
                                     const String & _inputFileName,
                                     const String & _extension)
{
  std::ifstream & inputFile = openInputAsciiFile(_inputPath,_inputFileName,_extension);;
  if (reportDebug(__FUNCTION__))
    {
    printCR();
    printValue("Parsing file",_inputPath + _inputFileName);
    }
  //Property    parameter;
  String      aString;
  int          lineCount;
  int          i;
  char         textBuffer[200];

  inputFile.seekg(0, std::ios::beg);
  lineCount = 0;
  while ((!inputFile.eof()))
    {
    lineCount++;
    inputFile.getline(textBuffer,200);
    aString = textBuffer;
    if((aString.IsNull()) || (aString.IsWhitespace()) || (aString[0] == '#') || (aString[0] == ';')) {
//      if (reportDebug(__FUNCTION__))
//        cout << "\tlineCount "<<lineCount<<" Ignoring  : "<<aString.Data() << endl;
      continue;
    } else if(aString.Contains('[')) {
//      if (reportDebug(__FUNCTION__))
//        cout << "\tlineCount "<<lineCount<<" Section   : "<<aString.Data() << endl;
      continue;
    } else if(!aString.Contains('=')) {
      if (reportDebug(__FUNCTION__))
        {
        printCR();
        printValue("lineCount",lineCount);
        printValue("WARNING String",aString.Data());
        }
      continue;
    }
    aString.ReplaceAll(" ","");
    aString.ReplaceAll("\t","");
    //    parameter.keyword = "";
    //    parameter.value   = "";
    //    for(i=0; aString[i] != '='; i++)   parameter.keyword += aString[i];
    //    for(i++; i<aString.Length(); i++)  parameter.value += aString[i];
    //    _configuration.addProperty(parameter);
    String keyword = "";
    String value   = "";
    for(i=0; aString[i] != '='; i++)   keyword += aString[i];
    for(i++; i<aString.Length(); i++)  value   += aString[i];
    _properties.addProperty(keyword,value);
    if (reportDebug(__FUNCTION__))
      {
      printCR();
      printValue("lineCount",lineCount);
      printValue("Property name",keyword);
      printValue("Property value",value);
      }
    }
}

void  PropertiesParser::saveTo(Properties & _properties,
                                 const String & _path,
                                 const String & _fileName,
                                 const String & _extension)
{
  std::ofstream & outputFile = openOutputAsciiFile(_path,_fileName,_extension);
  for (auto & property : _properties.properties()) property.saveTo(outputFile);
  outputFile.close();
}

}
