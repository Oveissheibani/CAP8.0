/* ********************************************************************* * Copyright (C) 2019-2024, Claude Pruneau, Victor Gonzalez     
 * All rights reserved.
 *
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 *
 * Author: Claude Pruneau,   04/01/2024
 *
 * *********************************************************************/
#ifndef CAP__Timer
#define CAP__Timer
#include <wchar.h>
#include <iostream>
#include <chrono>
#include <fstream>
#include "TObject.h"
using namespace std;
using namespace std::chrono;

namespace CAP
{
class Timer
{
public:

  Timer();
  Timer(const Timer & timer);
  virtual ~Timer() { }
  Timer & operator=(const Timer & timer);
  void start();
  void stop();
  void print() const;

  high_resolution_clock::time_point startTime;
  high_resolution_clock::time_point stopTime;
  high_resolution_clock::time_point oldStop;
  duration<double> totalDuration;
  duration<double> intervalDuration;
  int    days;
  int    hours;
  int    minutes;
  double seconds;
  int    deltaDays;
  int    deltaHours;
  int    deltaMinutes;
  double deltaSeconds;

  ClassDef(Timer,0)
};

}



#endif /* CAP__Timer */
