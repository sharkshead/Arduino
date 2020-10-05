/*
  ArcSleepWDT - a provider of ArcSched's WDT (SLEEP_MODE_STANDBY and SLEEP_MODE_PWR_DOWN) functionality

  Copyright 2013-2020 Graeme Elsworthy, Arcsine Pty Ltd, graeme@arcsine.com.au
*/

#include <Arduino.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>
#include <ArcSleepWDT.h>

volatile bool arcSleepWDTAmSleeping;
volatile void (*arcSleepWDTInt0)();
volatile void (*arcSleepWDTInt1)();
volatile uint8_t adcsra;

ISR(WDT_vect) {
  if(arcSleepWDTAmSleeping) {
    sleep_disable();
    // Turn the WDT off
    wdt_reset();
    MCUSR &= ~_BV(WDRF);
    WDTCSR |= _BV(WDCE) | _BV(WDE);
    WDTCSR = 0;
    WDTCSR &= ~_BV(WDIE);
  
    power_all_enable();
    ADCSRA = adcsra;  // Restore the ADC.
    arcSleepWDTAmSleeping = false;
  }
}

void arcSleepWDTInt0Vect() {
  if(arcSleepWDTAmSleeping) {
    sleep_disable();
    // Turn the WDT off
    wdt_reset();
    MCUSR &= ~_BV(WDRF);
    WDTCSR |= _BV(WDCE) | _BV(WDE);
    WDTCSR = 0;
    WDTCSR &= ~_BV(WDIE);
  
    power_all_enable();
    ADCSRA = adcsra;  // Restore the ADC.
    arcSleepWDTAmSleeping = false;
  }
  if(arcSleepWDTInt0 != (volatile void (*)()) 0) {
    (*arcSleepWDTInt0)();
  }
}

void arcSleepWDTInt1Vect() {
  if(arcSleepWDTAmSleeping) {
    sleep_disable();
    // Turn the WDT off
    wdt_reset();
    MCUSR &= ~_BV(WDRF);
    WDTCSR |= _BV(WDCE) | _BV(WDE);
    WDTCSR = 0;
    WDTCSR &= ~_BV(WDIE);
  
    power_all_enable();
    ADCSRA = adcsra;  // Restore the ADC.
    arcSleepWDTAmSleeping = false;
  }
  if(arcSleepWDTInt1 != (volatile void (*)()) 0) {
    (*arcSleepWDTInt1)();
  }
}

void ArcSleepWDT::setup() {
  sleepMode = SLEEP_MODE_PWR_DOWN;
  maxSleepTime = 8000;
  canSleep = false;
  timeScaleFactor = 0.88;
  arcSleepWDTAmSleeping = false;
  // Turn off the brown-out detector and ensure the WDT is off
  cli();
  // MCUCR = _BV(BODS) | _BV(BODSE);  // turn off brown-out detector
  // MCUCR = _BV(BODS); 
  wdt_reset();
  MCUSR &= ~_BV(WDRF);
  WDTCSR |= _BV(WDCE) | _BV(WDE);
  WDTCSR = 0;
  sei();
}

char *ArcSleepWDT::getName() {
  return (char *) "WDT";
}

uint8_t ArcSleepWDT::getSleepMode() {
  return sleepMode;
}

void ArcSleepWDT::setSleepMode(uint8_t mode) {
  // Any sleep mode is usable with the WDT
  if((mode == SLEEP_MODE_IDLE) || (mode == SLEEP_MODE_ADC) || (mode == SLEEP_MODE_PWR_SAVE) || (mode == SLEEP_MODE_STANDBY) || (mode == SLEEP_MODE_PWR_DOWN)) {
    sleepMode = mode;
  }
}

uint16_t ArcSleepWDT::getMaxSleepTime() {
  return maxSleepTime;
}

void ArcSleepWDT::setMaxSleepTime(uint32_t time) {
  // 8s is the WDT's maximum sleep time
  if(time > 8000) {
    maxSleepTime = 8000;
  } else {
    maxSleepTime = time;
  }
}

bool ArcSleepWDT::getCanSleep() {
  return canSleep;
}

void ArcSleepWDT::setCanSleep(bool c) {
  canSleep = c;
}

void ArcSleepWDT::calibrate(uint32_t r, uint32_t a) {
  if((r != 0) && (a != 0) && (r != a)) {
    timeScaleFactor *= r;
    timeScaleFactor /= a;
  }
}

void ArcSleepWDT::prescale8000() {
    WDTCSR |= _BV(WDCE) | _BV(WDE);
    WDTCSR = _BV(WDE) | _BV(WDP3) | _BV(WDP0);
}

void ArcSleepWDT::prescale4000() {
    WDTCSR |= _BV(WDCE) | _BV(WDE);
    WDTCSR = _BV(WDE) | _BV(WDP3);
}

void ArcSleepWDT::prescale2000() {
    WDTCSR |= _BV(WDCE) | _BV(WDE);
    WDTCSR = _BV(WDE) | _BV(WDP2) | _BV(WDP1) | _BV(WDP0);
}

void ArcSleepWDT::prescale1000() {
    WDTCSR |= _BV(WDCE) | _BV(WDE);
    WDTCSR = _BV(WDE) | _BV(WDP2) | _BV(WDP1);
}

void ArcSleepWDT::prescale500() {
    WDTCSR |= _BV(WDCE) | _BV(WDE);
    WDTCSR = _BV(WDE) | _BV(WDP2) | _BV(WDP0);
}

void ArcSleepWDT::prescale250() {
    WDTCSR |= _BV(WDCE) | _BV(WDE);
    WDTCSR = _BV(WDE) | _BV(WDP2);
}

void ArcSleepWDT::prescale125() {
    WDTCSR |= _BV(WDCE) | _BV(WDE);
    WDTCSR = _BV(WDE) | _BV(WDP1) | _BV(WDP0);
}

void ArcSleepWDT::prescale64() {
    WDTCSR |= _BV(WDCE) | _BV(WDE);
    WDTCSR = _BV(WDE) | _BV(WDP1);
}

void ArcSleepWDT::prescale32() {
    WDTCSR |= _BV(WDCE) | _BV(WDE);
    WDTCSR = _BV(WDE) | _BV(WDP0);
}

void ArcSleepWDT::prescale16() {
    WDTCSR |= _BV(WDCE) | _BV(WDE);
    WDTCSR = _BV(WDE);
}

uint32_t ArcSleepWDT::scale(uint32_t ms) {
  return ms * timeScaleFactor;
}

uint32_t ArcSleepWDT::inverseScale(uint32_t ms) {
  return ms / timeScaleFactor;
}

uint32_t ArcSleepWDT::sleepFor(uint32_t ms) {
  uint32_t s;
  uint32_t sleepTime;
  
  // scale the requested sleep time
  s = scale(ms);
  
  // Are we allowed to sleep? Or is the time to sleep less than our minimum?
  if((! canSleep) || (s < 16)) {
    return 0;
  }
  
  // Next, set up the WDT
  cli();
  wdt_reset();
  if(s >= 8000) {
    prescale8000();
    sleepTime = inverseScale(8000);
  } else if(s >= 4000) {
    prescale4000();
    sleepTime = inverseScale(4000);
  } else if(s >= 2000) {
    prescale2000();
    sleepTime = inverseScale(2000);
  } else if(s >= 1000) {
    prescale1000();
    sleepTime = inverseScale(1000);
  } else if(s >= 500) {
    prescale500();
    sleepTime = inverseScale(500);
  } else if(s >= 250) {
    prescale250();
    sleepTime = inverseScale(250);
  } else if(s >= 125) {
    prescale125();
    sleepTime = inverseScale(125);
  } else if(s >= 64) {
    prescale64();
    sleepTime = inverseScale(64);
  } else if(s >= 32) {
    prescale32();
    sleepTime = inverseScale(32);
  } else {
    prescale16();
    sleepTime = inverseScale(16);
  }
  WDTCSR |= _BV(WDIE);
  
  // Next, sleep
  adcsra = ADCSRA;
  ADCSRA = 0;  // Power off the ADC.
  power_all_disable();
  set_sleep_mode(sleepMode);
  // cli();
  sleep_enable();
  arcSleepWDTAmSleeping = true;
  sei();
  sleep_cpu();
  
  return sleepTime;
}

void ArcSleepWDT::int0Vect(void (*isr)(), uint8_t mode) {
  arcSleepWDTInt0 = (volatile void (*)()) isr;
  if(isr != (void (*)()) 0) {
    attachInterrupt(digitalPinToInterrupt(2), arcSleepWDTInt0Vect, mode);
  } else {
    detachInterrupt(digitalPinToInterrupt(2));
  }
}

void ArcSleepWDT::int1Vect(void (*isr)(), uint8_t mode) {
  arcSleepWDTInt1 = (volatile void (*)()) isr;
  if(isr != (void (*)()) 0) {
    attachInterrupt(digitalPinToInterrupt(3), arcSleepWDTInt1Vect, mode);
  } else {
    detachInterrupt(digitalPinToInterrupt(3));
  }
}
