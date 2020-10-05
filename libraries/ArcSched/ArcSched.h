/*
  ArcSched - a single threaded, millisecond-based function scheduler

  Copyright 2013-2020 Graeme Elsworthy, Arcsine Pty Ltd, graeme@arcsine.com.au
*/

#ifndef _ArcSched_h
#define _ArcSched_h

#ifdef ARDUINO
#include <Arduino.h>
#include <avr/sleep.h>
#include <ArcSleep.h>
#else
#include <stdint.h>
#include <sys/types.h>
#include "ArcSleep.h"
#endif

// The struct which is used to communicate between the scheduler and the
// called functions. When a function is called this is used to send
// information to the function from the scheduler and for the function to send
// changes back to the scheduler.
struct SchedFunctionDetail {
  // details that can go function to scheduler and scheduler to function
  uint8_t priority;
  bool priorityFlag;
  uint32_t interval;
  bool intervalFlag;

  // details that go scheduler to function
  uint32_t overshoot;
  
  // details that go function to scheduler
  bool removeFlag;
};

// The struct used internally by the scheduler to manage functions
struct SchedDetail {
  void (*function)(SchedFunctionDetail *, void *);
  void *argument;
  uint8_t priority;
  uint32_t lastRunTime;
  uint32_t interval;
  uint32_t runCount;
  char *name;
  SchedDetail *next;
};

// The scheduler priorities. Lower number means higher priority
#define SchedPriorityHigh 0
#define SchedPriorityMedium 1
#define SchedPriorityLow 2

class ArcSched {
  public:
    void setPriority(SchedFunctionDetail *, uint8_t);
    void setInterval(SchedFunctionDetail *, uint32_t);
    void setInterval(const char *, uint32_t);
    void deregister(SchedFunctionDetail *);
    uint32_t milliseconds(uint32_t);
    uint32_t seconds(uint32_t);
    uint32_t minutes(uint32_t);
    uint32_t hours(uint32_t);
    uint32_t days(uint32_t);
    bool registerFunction(void (*)(SchedFunctionDetail *, void *), void *, const char *, uint8_t, uint32_t);
    bool deregisterFunction(char *);
    bool requeueFunction(char *);
    ArcSleep *getSleepProcessor();
    void setSleepProcessor(ArcSleep *);
    uint32_t millis();
    void setSleepOverride();
    void setup();
    void loop();
    uint32_t getTimeToNextJob();

  private:
    SchedDetail *list;
    uint32_t millisOffset;
    uint32_t time;
    ArcSleep *sleepProcessor;
    bool sleepOverride;
    uint32_t timeToNextJob;
};

#endif
