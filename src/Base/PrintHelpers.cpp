#include "PrintHelpers.hpp"

namespace CAP
{

char selectFiller(unsigned int style)
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
void printCR(unsigned int n)
{
  for (unsigned int k=0; k<n; k++) (*gOutputStream) << std::endl;
}

//!
//! Print a line on (*gOutputStream) according to the properties size and gOutputStyle.
//!
void printLine(unsigned int n)
{
  char filler = selectFiller(4);
  for (unsigned int k=0; k<n; k++)
    (*gOutputStream) <<  std::left << std::setw(2*gOutputLength) << std::setfill(filler)<< filler << std::endl;
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

void printValue(const char * keyword, const String value)
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

std::ostream * gOutputStream = & std::cout;
unsigned int   gOutputStyle  = 0;
unsigned int   gOutputLength = 50;

} // namespace CAP

