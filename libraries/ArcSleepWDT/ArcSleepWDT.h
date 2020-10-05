/*
  ArcSleepWDT - a provider of ArcSched's WDT (SLEEP_MODE_STANDBY and SLEEP_MODE_PWR_DOWN) functionality

  Copyright 2013-2020 Graeme Elsworthy, Arcsine Pty Ltd, graeme@arcsine.com.au
*/

#ifndef _ArcSleepWDT_h
#define _ArcSleepWDT_h

#include <ArcSleep.h>

struct WDTSleepTrack {
  uint32_t period;
  uint32_t maxError;
  uint32_t requestAccumulation;
  uint32_t actualAccumulation;
};

class ArcSleepWDT : public ArcSleep {
  public:
    void setup();
    char *getName();
    uint8_t getSleepMode();
    void setSleepMode(uint8_t);
    uint16_t getMaxSleepTime();
    void setMaxSleepTime(uint32_t);
    bool getCanSleep();
    void setCanSleep(bool);
    void calibrate(uint32_t, uint32_t);
    uint32_t sleepFor(uint32_t);
    void int0Vect(void (*)(), uint8_t);
    void int1Vect(void (*)(), uint8_t);
    
  private:
    uint8_t sleepMode;
    uint16_t maxSleepTime;
    bool canSleep;
    uint32_t scale(uint32_t);
    uint32_t inverseScale(uint32_t);
    void prescale8000();
    void prescale4000();
    void prescale2000();
    void prescale1000();
    void prescale500();
    void prescale250();
    void prescale125();
    void prescale64();
    void prescale32();
    void prescale16();
    float timeScaleFactor;
};

#endif
