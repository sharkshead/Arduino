/*
MIT License

Copyright (c) 2013-2020 Graeme Elsworthy <github@sharkshead.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
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

Copyright (c) 2020 Graeme Elsworthy <github@sharkshead.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
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
