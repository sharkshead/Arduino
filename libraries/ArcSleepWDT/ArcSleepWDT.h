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
