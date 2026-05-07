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
#include "Timer.hpp"
#include "PrintHelpers.hpp"

ClassImp(CAP::Timer);

CAP::Timer::Timer()
:
startTime(),
stopTime(),
oldStop(),
totalDuration(),
hours(0),
minutes(0),
seconds(0),
deltaDays(0),
deltaHours(0),
deltaMinutes(0),
deltaSeconds(0)
{
  start();
}

CAP::Timer::Timer(const CAP::Timer & timer)
:
startTime(timer.startTime),
stopTime(timer.stopTime),
oldStop(timer.oldStop),
totalDuration(timer.totalDuration),
hours(timer.hours),
minutes(timer.minutes),
seconds(timer.seconds),
deltaDays(timer.seconds),
deltaHours(timer.seconds),
deltaMinutes(timer.seconds),
deltaSeconds(timer.seconds)
{   }

CAP::Timer & CAP::Timer::operator=(const CAP::Timer & timer)
{
  if (this!=&timer)
    {
    startTime     = timer.startTime;
    stopTime      = timer.stopTime;
    oldStop       = timer.oldStop;
    totalDuration = timer.totalDuration;
    hours         = timer.hours;
    minutes       = timer.minutes;
    seconds       = timer.seconds;
    deltaDays     = timer.seconds;
    deltaHours    = timer.seconds;
    deltaMinutes  = timer.seconds;
    deltaSeconds  = timer.seconds;
    }
  return *this;
}


void CAP::Timer::start()
{
  startTime = high_resolution_clock::now();
  oldStop   = startTime;
}
void CAP::Timer::stop()
{
  stopTime = high_resolution_clock::now();
  intervalDuration = chrono::duration_cast<chrono::seconds>(stopTime - oldStop);
  totalDuration    = chrono::duration_cast<chrono::seconds>(stopTime - startTime);
  days     = (int)(totalDuration.count()/(24*3600));
  hours    = (int)(totalDuration.count()/3600);
  minutes  = (int)((totalDuration.count() - 3600 * hours)/60);
  seconds  = totalDuration.count() - 60 * minutes - 3600 * hours;
  deltaDays    = (int)(intervalDuration.count()/(24*3600));
  deltaHours   = (int)(intervalDuration.count()/(3600));;
  deltaMinutes = (int)((intervalDuration.count() - 3600 * deltaHours)/60);
  deltaSeconds = intervalDuration.count() - 60*deltaMinutes - 3600*deltaHours - 24*3600*deltaDays;
  oldStop  = stopTime;
}

void CAP::Timer::print()  const
{
  printValue("DeltaDays",deltaDays);
  printValue("DeltaHours",deltaHours);
  printValue("DeltaMinutes",deltaMinutes);
  printValue("DeltaSeconds",deltaSeconds);
}


