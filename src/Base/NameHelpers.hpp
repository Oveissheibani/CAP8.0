/* **********************************************************************
 * Copyright (C) 2019-20242, Claude Pruneau
 * All rights reserved.
 *
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 *
 * Author: Claude Pruneau,   04/01/2024
 *
 * *********************************************************************/
#ifndef CAP__NameHelpers
#define CAP__NameHelpers
#include "Aliases.hpp"

namespace CAP
{
const String createName(const String & s0,
                        const String & s1,
                        int option=0);

const String createName(const String & s0,
                        const String & s1,
                        const String & s2,
                        int option=0);

const String createName(const String & s0,
                        const String & s1,
                        const String & s2,
                        const String & s3,
                        int option=0);

const String createName(const String & s0,
                        const String & s1,
                        const String & s2,
                        const String & s3,
                        const String & s4,
                        int option=0);

const String createName(const String & s0,
                        const String & s1,
                        const String & s2,
                        const String & s3,
                        const String & s4,
                        const String & s5,
                        int option=0);

const String createName(const String & s0,
                        const String & s1,
                        const String & s2,
                        const String & s3,
                        const String & s4,
                        const String & s5,
                        const String & s6,
                        int option=0);

const String createName(const std::vector<String> & tokens,
                        int option=0);

const String createName(const String & s0, int k,
                        const String & s1);

const String createName(const String & s0, int k0,
                        const String & s1, int k1,
                        const String & s2);

const String createNameI(const String & s0,
                         const String & s1,
                         int i1);

const String createNameII(const String & s0,
                          const String & s1,
                          int i1,
                          int i2);

const String createNameIII(const String & s0,
                           const String & s1,
                           int i1,
                           int i2,
                           int i3);
const String createNameIIII(const String & s0,
                            const String & s1,
                            int i1,
                            int i2,
                            int i3,
                            int i4);

const String createNameIS(const String & s0,
                          int i1,
                          const String & suffix);

const String createNameIIS(const String & s0,
                           int i1,
                           int i2,
                           const String & suffix);

const String createNameIIIS(const String & s0,
                            int i1,
                            int i2,
                            int i3,
                            const String & suffix);
const String createNameIIIIS(const String & s0,
                             int i1,
                             int i2,
                             int i3,
                             int i4,
                             const String & suffix);

const String createTitle(const String & observable,
                         const String & particle1,
                         const String & particle2);

const String createTitle(const String & observable,
                         const String & particle1);

String createKey(const String & taskName, const String & suffix);
String createKey(const String & taskName, const String & objectType, const String & suffix);
String createKey(const String & taskName, const String & objectType, unsigned int index, const String & suffix);
String createKey(const String & taskName, const String & objectType, unsigned int index1, const String & s,unsigned int index2,const String & suffix );

} // namespace CAP

#endif /* CAP__NameHelpers */
