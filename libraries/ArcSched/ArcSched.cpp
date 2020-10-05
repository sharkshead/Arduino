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

#ifdef ARDUINO
#include "ArcSched.h"
#include <avr/power.h>
#else
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include "ArcSched.h"
#endif

// Helper routines for updating a SchedFunctionDetail.
void ArcSched::setPriority(SchedFunctionDetail *fd, uint8_t priority) {
  fd->priority = priority;
  fd->priorityFlag = true;
}

void ArcSched::setInterval(SchedFunctionDetail *fd, uint32_t interval) {
  fd->interval = interval;
  fd->intervalFlag = true;
}

void ArcSched::setInterval(const char *name, uint32_t interval) {
  SchedDetail *p;
  uint32_t timeToThisJob;

  for(p = list; p != (SchedDetail *) 0; p = p->next) {
    if(strcmp(p->name, name) == 0) {
      p->interval = interval;
      timeToThisJob = p->lastRunTime + p->interval - time;
      if(timeToThisJob < timeToNextJob) {
        timeToNextJob = timeToThisJob;
      }
      break;
    }
  }
}

void ArcSched::deregister(SchedFunctionDetail *fd) {
  fd->removeFlag = true;
}

// Converters from seconds, minutes etc to milliseconds.

// This is here for completeness.
uint32_t ArcSched::milliseconds(uint32_t mi) {
  return mi;
}

// The rest are actually useful.
uint32_t ArcSched::seconds(uint32_t s) {
  return 1000L * s;
}

uint32_t ArcSched::minutes(uint32_t m) {
  return 60000L * m;
}

uint32_t ArcSched::hours(uint32_t h) {
  return 3600000L * h;
}

uint32_t ArcSched::days(uint32_t d) {
  return 86400000L * d;
}

// Register a function for use with the scheduler.
bool ArcSched::registerFunction(void (*function)(SchedFunctionDetail *, void *), void *argument, const char *name, uint8_t priority, uint32_t interval) {
  SchedDetail *p;
  SchedDetail **pp;
  uint8_t i;

  p = (SchedDetail *) malloc(sizeof(SchedDetail));
  if(p == (SchedDetail *) 0) {
    return false;
  }
  p->name = (char *) malloc(strlen(name) + 1);
  if(p->name == (char *) 0) {
    free(p);
    return false;
  }
  
  p->function = function;
  p->argument = argument;
  p->priority = priority;
  p->interval = interval;
  strcpy(p->name, name);
  p->lastRunTime = millis();
  p->next = (SchedDetail *) 0;

  pp = &list;
  while(*pp != (SchedDetail *) 0) pp = &((*pp)->next);
  *pp = p;

  return true;
}

// Deregister a function, given by it's name.
bool ArcSched::deregisterFunction(char *functionName) {
  SchedDetail *p;

  for(p = list; p != (SchedDetail *) 0; p = p->next) {
    if((p->function != (void (*)(SchedFunctionDetail *, void *)) 0) && (strcmp(p->name, functionName) == 0)) {
      // For now just clear the function pointer. It will be cleaned up at the next invocation of loop().
      p->function = (void (*)(SchedFunctionDetail *, void *)) 0;
      return true;
    }
  }

  return false;
}

// Make it look like the function has just run.
bool ArcSched::requeueFunction(char *functionName) {
  SchedDetail *p;

  for(p = list; p != (SchedDetail *) 0; p = p->next) {
    if((p->function != (void (*)(SchedFunctionDetail *, void *)) 0) && (strcmp(p->name, functionName) == 0)) {
      p->lastRunTime = millis();
      return true;
    }
  }

  return false;
}

ArcSleep* ArcSched::getSleepProcessor() {
  return sleepProcessor;
}

void ArcSched::setSleepProcessor(ArcSleep *sp) {
  sleepProcessor = sp;
}

uint32_t ArcSched::millis() {
  return ::millis() + millisOffset;
}

void ArcSched::setSleepOverride() {
  sleepOverride = true;
}

uint32_t ArcSched::getTimeToNextJob() {
  return timeToNextJob;
}

// Initialise the scheduler.
void ArcSched::setup() {
  list = (SchedDetail *) 0;
  millisOffset = 0;
  sleepProcessor = (ArcSleep *) 0;
  sleepOverride = false;
}

// The main scheduler processor.
void ArcSched::loop() {
  SchedDetail *p;
  SchedDetail **pp;
  SchedFunctionDetail fd;
  // uint32_t time;
  // uint32_t timeToNextJob;
  uint32_t timeToThisJob;
  bool aJobRan;

  // Go through the list of jobs and run those that are ready to run.
  // Also keep a track of the time to the next job to be run so we can make
  // a decision about powering down the processor or not.
  do {
    timeToNextJob = 0xffffffff;
    aJobRan = false;
    for(uint8_t priority = SchedPriorityHigh; priority <= SchedPriorityLow; priority++) {
      for(p = list; p != (SchedDetail *) 0; p = p->next) {
        if((p->function != (void (*)(SchedFunctionDetail *, void *)) 0) && (p->priority == priority)) {
          time = millis();
          if((time - p->lastRunTime) >= p->interval) {
            // Fill out the function detail struct.
            fd.priority = p->priority;
            fd.priorityFlag = false;
            fd.interval = p->interval;
            fd.intervalFlag = false;
            // fixme - doesn't work for jobs run soon after boot.
            fd.overshoot = millis() - p->lastRunTime - p->interval;
            fd.removeFlag = false;

            // Call the function.
            (*p->function)(&fd, p->argument);
            aJobRan = true;

            // Deregister the entry if requested.
            if(fd.removeFlag) {
              p->function = (void (*)(SchedFunctionDetail *, void *)) 0;
            } else {
              p->lastRunTime += p->interval;
              // Update priority and interval if requested.
              if(fd.priorityFlag) p->priority = fd.priority;
              if(fd.intervalFlag) p->interval = fd.interval;
            }
          } else {
            timeToThisJob = p->lastRunTime + p->interval - time;
            if(timeToThisJob < timeToNextJob) {
              timeToNextJob = timeToThisJob;
            }
          }
        }
      }
    }
  } while(aJobRan);

  // Clean up the list by removing deregistered entries.
  for(pp = &list; *pp != (SchedDetail *) 0; ) {
    if((*pp)->function == (void (*)(SchedFunctionDetail *, void *)) 0) {
      p = *pp;
      *pp = p->next;
      free(p->name);
      free(p);
    } else {
      pp = &((*pp)->next);
    }
  }
  
  // Sleep if we can
  if((! sleepOverride) && (sleepProcessor != (ArcSleep *) 0)) {
    millisOffset += sleepProcessor->sleepFor(timeToNextJob);
  }
  sleepOverride = false;
}
