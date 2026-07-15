#include <Arduino.h>
#include "safety.h"
SafetyState safetyState = SAFE;

bool hasWeakCell() {

  for (int i = 0; i < NUM_CELLS; i++) {

    if (battery.cellVoltage[i] < WEAK_CELL_THRESHOLD) {
      return true;
    }

  }

  return false;
}
bool hasOverVoltage() {

  for (int i = 0; i < NUM_CELLS; i++) {

    if (battery.cellVoltage[i] > OVERVOLTAGE_THRESHOLD) {
      return true;
    }

  }

  return false;
}
void processSafety() {

  if (hasOverVoltage()) {
    safetyState = OVERVOLTAGE;
}
else if (hasWeakCell()) {
    safetyState = WEAK_CELL;
}
else {
    safetyState = SAFE;
}

  switch (safetyState) {

    case SAFE:
      digitalWrite(RELAY_PIN, HIGH);
      break;

    case WEAK_CELL:
      digitalWrite(RELAY_PIN, LOW);
      Serial.println("WARNING: Weak Cell Detected");
      break;
    case OVERVOLTAGE:
      digitalWrite(RELAY_PIN, LOW);
      Serial.println("WARNING: Overvoltage Detected");
      break;

    default:
      break;
  }
  
}