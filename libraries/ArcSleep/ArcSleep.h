/*
  ArcSleep - a base class for ArcShed's sleep mode functionality

  Copyright 2013-2020 Graeme Elsworthy, Arcsine Pty Ltd, graeme@arcsine.com.au
*/

#ifndef _ArcSleep_h
#define _ArcSleep_h

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <stdint.h>
#endif

class ArcSleep {
  public:
    void setup();
    virtual char *getName();
    virtual uint8_t getSleepMode();
    virtual void setSleepMode(uint8_t);
    virtual uint16_t getMaxSleepTime();
    virtual void setMaxSleepTime(uint32_t);
    virtual bool getCanSleep();
    virtual void setCanSleep(bool);
    virtual void calibrate(uint32_t, uint32_t); // request, actual
    virtual uint32_t sleepFor(uint32_t);
#ifdef ARDUINO
    virtual void int0Vect(void (*)(), uint8_t);  // isr, LOW | CHANGE | RISING | FALLING
    virtual void int1Vect(void (*)(), uint8_t);  // isr, LOW | CHANGE | RISING | FALLING
#endif /* ARDUINO */
    void hastaLaVista();
    
  private:
    uint8_t sleepMode;
    uint16_t maxSleepTime;
    bool canSleep;
};

#endif
