#ifndef SAFETY_H
#define SAFETY_H

#include <Arduino.h>
#include "battery.h"


enum SafetyState {
  SAFE,
  WEAK_CELL,
  OVERVOLTAGE,
  SENSOR_FAULT
};

extern SafetyState safetyState;
bool hasWeakCell();
bool hasOverVoltage();
void processSafety();


#endif