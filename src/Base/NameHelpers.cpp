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
#include "NameHelpers.hpp"
#include "PrintHelpers.hpp"

namespace CAP
{

const String createName(const String & s0,
                        const String & s1,
                        int option)
{
  String separator;
  switch (option)
    {
      default:
      case 0: separator = "_"; break;
      case 1: separator = " "; break;
      case 2: separator = ""; break;
    }
  String name = s0;
  name += separator;
  name += s1;
  return name;
}

const String createName(const String & s0,
                        const String & s1,
                        const String & s2,
                        int option)
{
  String separator;
  switch (option)
    {
      default:
      case 0: separator = "_"; break;
      case 1: separator = " "; break;
      case 2: separator = ""; break;
    }
  String name = s0;
  name += separator;
  name += s1;
  name += separator;
  name += s2;
  return name;
}

const String createName(const String & s0,
                        const String & s1,
                        const String & s2,
                        const String & s3,
                        int option)
{
  String separator;
  switch (option)
    {
      default:
      case 0: separator = "_"; break;
      case 1: separator = " "; break;
      case 2: separator = ""; break;
    }
  String name = s0;
  name += separator;
  name += s1;
  name += separator;
  name += s2;
  name += separator;
  name += s3;
  return name;
}


const String createName(const String & s0,
                        const String & s1,
                        const String & s2,
                        const String & s3,
                        const String & s4,
                        int option)
{
  String separator;
  switch (option)
    {
      default:
      case 0: separator = "_"; break;
      case 1: separator = " "; break;
      case 2: separator = ""; break;
    }
  String name = s0;
  name += separator;
  name += s1;
  name += separator;
  name += s2;
  name += separator;
  name += s3;
  name += separator;
  name += s4;
  return name;
}

const String createName(const String & s0,
                        const String & s1,
                        const String & s2,
                        const String & s3,
                        const String & s4,
                        const String & s5,
                        int option)
{
  String separator;
  switch (option)
    {
      default:
      case 0: separator = "_"; break;
      case 1: separator = " "; break;
      case 2: separator = ""; break;
    }
  String name = s0;
  name += separator;
  name += s1;
  name += separator;
  name += s2;
  name += separator;
  name += s3;
  name += separator;
  name += s4;
  name += separator;
  name += s5;
  return name;
}

const String createName(const String & s0,
                        const String & s1,
                        const String & s2,
                        const String & s3,
                        const String & s4,
                        const String & s5,
                        const String & s6,
                        int option)
{
  String separator;
  switch (option)
    {
      default:
      case 0: separator = "_"; break;
      case 1: separator = " "; break;
      case 2: separator = ""; break;
    }
  String name = s0;
  name += separator;
  name += s1;
  name += separator;
  name += s2;
  name += separator;
  name += s3;
  name += separator;
  name += s4;
  name += separator;
  name += s5;
  name += separator;
  name += s6;
  return name;
}

const String createName(const std::vector<String> & tokens,
                        int option)
{
  String separator;
  switch (option)
    {
      default:
      case 0: separator = "_"; break;
      case 1: separator = " "; break;
      case 2: separator = ""; break;
    }
  String name = "";
  if (tokens.size()<1)
    name = "NoName";
  else
    {
    name = tokens[0];
    for (unsigned int iToken=1; iToken<tokens.size(); iToken++)
      {
      name += separator;
      name += tokens[iToken];
      }
    }
  return name;
}

const String createName(const String & s0, int k, const String & s1)
{
  String s = s0;
  s += k;
  s += s1;
  return s;
}

const String createName(const String & s0, int k0, const String & s1, int k1, const String & s2)
{
  String s = s0;
  s += k0;
  s += s1;
  s += k1;
  s += s2;
  return s;
}




const String createNameI(const String & s0,
                         const String & s1,
                         int i1)
{
  String separator;
  int option = 0; // for now
  switch (option)
    {
      default:
      case 0: separator = "_"; break;
      case 1: separator = " "; break;
      case 2: separator = ""; break;
    }
  String name = s0;
  name += separator;
  name += s1;
  name += separator;
  name += i1;
  name += separator;
  return name;
}

const String createNameII(const String & s0,
                          const String & s1,
                          int i1,
                          int i2)
{
  String separator;
  int option = 0; // for now
  switch (option)
    {
      default:
      case 0: separator = "_"; break;
      case 1: separator = " "; break;
      case 2: separator = ""; break;
    }
  String name = s0;
  name += separator;
  name += s1;
  name += separator;
  name += i1;
  name += i2;
  name += separator;
  return name;
}

const String createNameIII(const String & s0,
                           const String & s1,
                           int i1,
                           int i2,
                           int i3)
{
  String separator;
  int option = 0; // for now
  switch (option)
    {
      default:
      case 0: separator = "_"; break;
      case 1: separator = " "; break;
      case 2: separator = ""; break;
    }
  String name = s0;
  name += separator;
  name += s1;
  name += separator;
  name += i1;
  name += i2;
  name += i3;
  name += separator;
  return name;
}

const String createNameIIII(const String & s0,
                            const String & s1,
                            int i1,
                            int i2,
                            int i3,
                            int i4)
{
  String separator;
  int option = 0; // for now
  switch (option)
    {
      default:
      case 0: separator = "_"; break;
      case 1: separator = " "; break;
      case 2: separator = ""; break;
    }
  String name = s0;
  name += separator;
  name += s1;
  name += separator;
  name += i1;
  name += i2;
  name += i3;
  name += i4;
  name += separator;
  return name;
}


///

const String createNameIS(const String & s0,
                          int i1,
                          const String & suffix)
{
  String separator;
  int option = 0; // for now
  switch (option)
    {
      default:
      case 0: separator = "_"; break;
      case 1: separator = " "; break;
      case 2: separator = ""; break;
    }
  String name = s0;
  name += separator;
  name += i1;
  name += separator;
  name += suffix;
  return name;
}

const String createNameIIS(const String & s0,
                           int i1,
                           int i2,
                           const String & suffix)
{
  String separator;
  int option = 0; // for now
  switch (option)
    {
      default:
      case 0: separator = "_"; break;
      case 1: separator = " "; break;
      case 2: separator = ""; break;
    }
  String name = s0;
  name += separator;
  name += i1;
  name += i2;
  name += separator;
  name += suffix;
  return name;
}

const String createNameIIIS(const String & s0,
                            int i1,
                            int i2,
                            int i3,
                            const String & suffix)
{
  String separator;
  int option = 0; // for now
  switch (option)
    {
      default:
      case 0: separator = "_"; break;
      case 1: separator = " "; break;
      case 2: separator = ""; break;
    }
  String name = s0;
  name += separator;
  name += i1;
  name += i2;
  name += i3;
  name += separator;
  name += suffix;
  return name;
}

const String createNameIIIIS(const String & s0,
                             int i1,
                             int i2,
                             int i3,
                             int i4,
                             const String & suffix)
{
  String separator;
  int option = 0; // for now
  switch (option)
    {
      default:
      case 0: separator = "_"; break;
      case 1: separator = " "; break;
      case 2: separator = ""; break;
    }
  String name = s0;
  name += separator;
  name += i1;
  name += i2;
  name += i3;
  name += i4;
  name += separator;
  name += suffix;
  return name;
}

const  String createTitle(const String & observable,
                          const String & particle1,
                          const String & particle2)
{
  String lb = "{";
  String rb = "}";
  String bb = "";
  String p1 = particle1;
  String p2 = particle2;
  //  if (p1.Contains("#bar"))  p1 = "#bar{p}";
  //  if (p2.Contains("#bar"))  p2 = "#bar{p}";

  String title;
  title = observable;
  title += "^{";
  title += p1;
  title += p2;
  title += "}";
  return title;
}

const  String createTitle(const String & observable,
                          const String & particle)
{
  String lb = "{";
  String rb = "}";
  String bb = "";
  String p1 = particle;
  String title;
  title = observable;
  title += "^{";
  title += p1;
  title += "}";
  return title;
}

String createKey(const String & taskName,
                 const String & suffix)
{
  String key = taskName;
  key += ":";
  key += suffix;
  return key;
}


String createKey(const String & taskName,
                 const String & objectType,
                 const String & suffix)
{
  String key = taskName;
  key += ":";
  key += objectType;
  key += ":";
  key += suffix;
  //printValue("key",key);
  return key;
}

String createKey(const String & taskName,
                 const String & objectType,
                 unsigned int index,
                 const String & suffix)
{
  String key = taskName;
  key += ":";
  key += objectType;
  key += "_";
  key += index;
  key += ":";
  key += suffix;
  //printValue("key",key);
  return key;
}

String createKey(const String & taskName,
                 const String & objectType,
                 unsigned int index1,
                 const String & s,
                 unsigned int index2,
                 const String & suffix)
{
  String key = taskName;
  key += ":";
  key += objectType;
  key += "_";
  key += index1;
  key += ":";
  key += s;
  key += "_";
  key += index2;
  key += ":";
  key += suffix;
  //printValue("key",key);
  return key;
}

} // namespace CAP
