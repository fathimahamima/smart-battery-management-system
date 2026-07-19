#ifndef SAFETY_H
#define SAFETY_H

#include <Arduino.h>
#include "battery.h"


enum SafetyState {
  SAFE,
  WEAK_CELL,
  OVERVOLTAGE,
  SENSOR_FAULT,
  RAPID_FLUCTUATION
};

extern SafetyState safetyState;
bool hasWeakCell();
bool hasOverVoltage();
bool hasSensorFault();
bool hasRapidFluctuation();
void processSafety();

#endif