#ifndef CAP_PrintHelpers
#define CAP_PrintHelpers
#include "Aliases.hpp"
#include <iostream>
#include <iomanip>

namespace CAP
{
char selectFiller(unsigned int style=0);
void printCR(unsigned int n=1);
void printLine(unsigned int n=1);
void printString(const char * keyword);
void printValue(const char * keyword, bool value);
void printValue(const char * keyword, unsigned int value);
void printValue(const char * keyword, size_t value);
void printValue(const char * keyword, int    value);
void printValue(const char * keyword, long   value);
void printValue(const char * keyword, long long value);
void printValue(const char * keyword, double value);
void printValue(const char * keyword, const char * value);
void printValue(const char * keyword, const String value);
void printRange(const char * keyword, int minimum, int maximum);
void printRange(const char * keyword, double minimum, double maximum);
void printDualRange(const char * keyword,
                  double minimum,  double maximum,
                  double minimum2, double maximum2);
void printDualRange(const char * keyword,
                  int minimum,  int maximum,
                  int minimum2, int maximum2);

extern std::ostream * gOutputStream;
extern unsigned int   gOutputStyle;
extern unsigned int   gOutputLength;

} // namespace CAP

#endif
