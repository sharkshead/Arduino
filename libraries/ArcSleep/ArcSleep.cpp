/*
  ArcSleep - a base class for ArcShed's sleep mode functionality

  Copyright 2013-2020 Graeme Elsworthy, Arcsine Pty Ltd, graeme@arcsine.com.au
*/

#ifdef ARDUINO
#include <avr/sleep.h>
#include <avr/power.h>
#include <ArcSleep.h>
#else
#include "ArcSleep.h"
#endif

void ArcSleep::setup() {
#ifdef ARDUINO
  sleepMode = SLEEP_MODE_IDLE;
#else
  sleepMode = 0;
#endif
  maxSleepTime = 0;
  canSleep = false;

#ifdef ARDUNIO
  arcSleepInt0Vect = (vopid (*)()) 0;
  arcSleepInt1Vect = (vopid (*)()) 0;
#endif /* ARDUINO */
}

char *ArcSleep::getName() {
  return (char *) "null";
}

uint8_t ArcSleep::getSleepMode() {
  return sleepMode;
}

void ArcSleep::setSleepMode(uint8_t s) {
  sleepMode = s;
}

uint16_t ArcSleep::getMaxSleepTime() {
  return maxSleepTime;
}

void ArcSleep::setMaxSleepTime(uint32_t t) {
  maxSleepTime = 0;
}

bool ArcSleep::getCanSleep() {
  return canSleep;
}

void ArcSleep::setCanSleep(bool c) {
  canSleep = false;
}

float ArcSleep::getScaleFactor() {
  return 0.0;
}

uint8_t ArcSleep::getTrack() {
  return 0;
}

void ArcSleep::calibrate(uint32_t request, uint32_t actual) {
}

uint32_t ArcSleep::sleepFor(uint32_t ms) {
  return 0;
}

#ifdef ARDUINO
void ArcSleep::int0Vect(void (*isr)(), uint8_t mode) {
  if(isr != (void (*)()) 0) {
    attachInterrupt(digitalPinToInterrupt(2), isr, mode);
  } else {
    detachInterrupt(digitalPinToInterrupt(2));
  }
}

void ArcSleep::int1Vect(void (*isr)(), uint8_t mode) {
  if(isr != (void (*)()) 0) {
    attachInterrupt(digitalPinToInterrupt(3), isr, mode);
  } else {
    detachInterrupt(digitalPinToInterrupt(3));
  }
}
#endif /* ARDUINO */

void ArcSleep::hastaLaVista() {
  while(true) {
    cli();
    ADCSRA = 0;  // Power off the ADC.
    power_all_disable();
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    sleep_cpu();
  }
}
