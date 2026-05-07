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

std::ostream * gOutputStream = &std::cout;
unsigned int   gOutputStyle  = 0;
unsigned int   gOutputLength = 50;

char selectFiller(unsigned int style=0)
{
  char filler;
  switch (style)
    {
      default:
      case 0: filler = '.'; break;
      case 1: filler = ' '; break;
      case 2: filler = '_'; break;
      case 3: filler = '-'; break;
      case 4: filler = '='; break;
    }
  return filler;
}

//!
//! Print n carriage returns.
//!
void printCR(unsigned int n=1)
{
  for (unsigned int k=0; k<n; k++) cout << std::endl;
}

//!
//! Print a line on (*gOutputStream) according to the properties size and gOutputStyle.
//!
void printLine(unsigned int n)
{
  char filler = selectFiller(4);
  for (unsigned int k=0; k<n; k++)
    cout <<  std::left << std::setw(2*gOutputLength) << std::setfill(filler)<< filler << std::endl;
}

//!
//! Print a line on (*gOutputStream) according to the properties size and gOutputStyle.
//!
void printString(const char * keyword)
{
  if (gOutputStyle>0)
    (*gOutputStream) << keyword <<  std::endl;
  else
    {
    char filler = selectFiller(gOutputStyle);
    (*gOutputStream) <<  std::left << std::setw(gOutputLength) << std::setfill(filler)<< keyword << " : " <<  std::endl;
    }
}

void printValue(const char * keyword, bool   value)
{
  char filler = selectFiller(gOutputStyle);
  (*gOutputStream) <<  std::left << std::setw(gOutputLength) << std::setfill(filler)<< keyword << " : " << value << std::endl;
}

void printValue(const char * keyword, size_t value)
{
  char filler = selectFiller(gOutputStyle);
  (*gOutputStream) <<  std::left << std::setw(gOutputLength) << std::setfill(filler)<< keyword << " : " << value <<  std::endl;
}

void printValue(const char * keyword, unsigned int value)
{
  char filler = selectFiller(gOutputStyle);
  (*gOutputStream) <<  std::left << std::setw(gOutputLength) << std::setfill(filler)<< keyword << " : " << value <<  std::endl;
}

void printValue(const char * keyword, int    value)
{
  char filler = selectFiller(gOutputStyle);
  (*gOutputStream) <<  std::left << std::setw(gOutputLength) << std::setfill(filler)<< keyword << " : " << value <<  std::endl;
}

void printValue(const char * keyword, long   value)
{
  char filler = selectFiller(gOutputStyle);
  (*gOutputStream) <<  std::left << std::setw(gOutputLength) << std::setfill(filler)<< keyword << " : " << value << std::endl;
}

void printValue(const char * keyword, long long value)
{
  char filler = selectFiller(gOutputStyle);
  (*gOutputStream) <<  std::left << std::setw(gOutputLength) << std::setfill(filler)<< keyword << " : " << value << std::endl;
}


void printValue(const char * keyword, double value)
{
  char filler = selectFiller(gOutputStyle);
  (*gOutputStream) <<  std::left << std::setw(gOutputLength) << std::setfill(filler)<< keyword << " : " << value << std::endl;
}

void printValue(const char * keyword, const char * value)
{
  char filler = selectFiller(gOutputStyle);
  (*gOutputStream) <<  std::left << std::setw(gOutputLength) << std::setfill(filler)<< keyword << " : " << value << std::endl;
}

void printValue(const char * keyword, const TString  value)
{
  char filler = selectFiller(gOutputStyle);
  (*gOutputStream) <<  std::left << std::setw(gOutputLength) << std::setfill(filler)<< keyword << " : " << value << std::endl;
}

void printRange(const char * keyword, double minimum, double maximum)
{
  char filler = selectFiller(gOutputStyle);
  (*gOutputStream) << std::left << std::setw(gOutputLength) << std::setfill(filler)<< keyword
  << " Minimum=" << minimum << "  Maximum=" << maximum << std::endl;
}

void printRange(const char * keyword, int minimum, int maximum)
{
  char filler = selectFiller(gOutputStyle);
  (*gOutputStream) << std::left << std::setw(gOutputLength) << std::setfill(filler)<< keyword
  << " Minimum=" << minimum << "  Maximum=" << maximum << std::endl;
}

void printDualRange(const char * keyword,
                    double minimum,  double maximum,
                    double minimum2, double maximum2)
{
  char filler = selectFiller(gOutputStyle);
  (*gOutputStream) << std::left << std::setw(gOutputLength) << std::setfill(filler)<< keyword
  << " Minimum=" << minimum << "  Maximum=" << maximum
  << " Mimumum2=" << minimum2 << "  Maximum2=" << maximum2
  << std::endl;
}

void printDualRange(const char * keyword,
                    int minimum,  int maximum,
                    int minimum2, int maximum2)
{
  char filler = selectFiller(gOutputStyle);
  (*gOutputStream) << std::left << std::setw(gOutputLength) << std::setfill(filler)<< keyword
  << " Minimum=" << minimum << "  Maximum=" << maximum
  << " Mimumum2=" << minimum2 << "  Maximum2=" << maximum2
  << std::endl;
}



std::ifstream & openInputAsciiFile(const TString  & fileName,
                                   bool verbose)
{
  std::ios_base::openmode mode = std::ios_base::in;
  if (verbose)
    {
    printCR();
    printValue("openInputAsciiFile(...) Open input file named",fileName);
    printValue("openInputAsciiFile(...) with option",mode);
    }
  std::ifstream * file = new std::ifstream(fileName.Data(),mode);
  if (!file || !file->is_open())
    {
    cout << "Failed to open input file named " <<fileName<< endl;
    }
  if (verbose)
    {
    printCR();
    printValue("openInputAsciiFile(...) Successfully opened file named",fileName);
    }
  return *file;
}


std::ofstream & openOutputAsciiFile(const TString  & fileName,
                                    bool verbose)
{
  std::ios_base::openmode mode = std::ios_base::out|std::ios_base::app;
  if (verbose)
    {
    printCR();
    printValue("openOutputAsciiFile(...) Open input file named",fileName);
    printValue("openOutputAsciiFile(...) with option",mode);
    }
  std::ofstream * file = new std::ofstream(fileName.Data(),mode);
  if (!file || !file->is_open())
    {
    cout << "Failed to open output file named " <<fileName<< endl;
    }
  if (verbose)
    {
    printCR();
    printValue("openOutputAsciiFile(...) Successfully opened file named",fileName);
    }
  return *file;
}

//int parseNumber(int flavorIndex, int content) const
//{
//  if (content/1000000 > 0) throw Exception("More than 6 quark flavors??? value/1000000 > 0",__FUNCTION__);
//  int value = 0;
//  switch (flavorIndex)
//    {
//      case 0:  value = int(content  % 10); break;
//      case 1:  value = int(content/10  % 10); break;
//      case 2:  value = int(content/100  % 10); break;
//      case 3:  value = int(content/1000  % 10); break;
//      case 4:  value = int(content/10000  % 10); break;
//      case 5:  value = int(content/100000  % 10); break;
//      default: throw Exception("Invalid flavor index",__FUNCTION__);
//    }
//  return value;
//}

int getQuarks(double nd, double nu, double ns=0, double nc=0, double nb=0, double nt=0)
{
  return nd + nu*10 + ns*100 + nc*1000 + nb*10000 + nt*100000;
}



int Converter(const TString & inputFileName="./particles.data",
              const TString & outnputFileName="./particlesNew.data")
{
  char   buff[300];
  char   name[30];
  char   title[30];
  int    pdgCode, quarks, antiquarks, leptons, index;
  double charge, mass, width, spin, isospin, isospin3, nq, ns, nc, naq, nas, nac;

  std::ifstream & inputFile  = openInputAsciiFile(inputFileName,true);
  std::ofstream & outputFile = openOutputAsciiFile(outnputFileName,true);

  index = 0;
  while (!inputFile.eof())
    {
    inputFile.getline(buff,400);
    if (!(*buff)) continue;
    if (*buff == '#')
      {
      outputFile << buff << endl;
      continue;
      }
    istringstream * iss = new istringstream(buff);
//    (*iss) >> name >> title >> mass >> width >> spin >> isospin >> isospin3 >> quarks >> antiquarks >> leptons  >> pdgCode;
    (*iss) >> name >> title >> mass >> width >> spin >> isospin >> isospin3 >> nq  >> ns >>  naq  >> nas  >> nc  >> nac >> pdgCode;

    double uncertainty = 0.0;
    charge  = 0.0;
    quarks  = getQuarks(nq,nq,nc);
    antiquarks = getQuarks(naq,naq,nac);
    leptons = 0;
    outputFile
    << std::left  << std::setw(30) << std::setfill(' ') << name
    << std::left  << std::setw(30) << std::setfill(' ') << title
    << std::right << std::setw(15) << std::fixed        << pdgCode
    << std::right << std::setw(10) << std::setprecision(3) << charge
    << std::right << std::setw(15) << std::setprecision(6) << mass
    << std::right << std::setw(15) << std::setprecision(6) << uncertainty
    << std::right << std::setw(15) << std::setprecision(6) << width
    << std::right << std::setw(15) << std::setprecision(6) << uncertainty
    << std::right << std::setw(10)  << std::setprecision(1) << spin
    << std::right << std::setw(10)  << std::setprecision(1) << isospin
    << std::right << std::setw(10)  << std::setprecision(1) << isospin3
    << std::right << std::setw(10) << std::fixed  << quarks
    << std::right << std::setw(10) << std::fixed  << antiquarks
    << std::right << std::setw(10) << std::fixed  << leptons
    << endl;



    printValue("name",name);
//    printValue("title",title);
//    //printValue("charge",charge);
//    printValue("mass",mass);
//    printValue("width",width);
//    printValue("spin",spin);
//    printValue("isospin",isospin);
//    printValue("isospin3",isospin3);
//
//    printValue("nq",nq);
//    printValue("ns",ns);
//    printValue("naq",naq);
//    printValue("nas",nas);
//    printValue("nc",nc);
//    printValue("nac",nac);

//    printValue("quarks",quarks);
//    printValue("antiquarks",antiquarks);
//    printValue("leptons",leptons);
    printValue("pdgCode",pdgCode);
    delete iss;
    index++;
    }

  return 0;
}

