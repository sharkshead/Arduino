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
