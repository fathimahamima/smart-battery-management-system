#include <Arduino.h>
#include "safety.h"
SafetyState safetyState = SAFE;
unsigned long faultStartTime = 0;
const unsigned long RECOVERY_DELAY = 3000; // 3 seconds
float previousVoltage[NUM_CELLS] = {0};
bool firstReading = true;
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
bool hasSensorFault() {

  for (int i = 0; i < NUM_CELLS; i++) {

    if (battery.cellVoltage[i] < 0.5 ||
        battery.cellVoltage[i] > 4.3) {

      return true;
    }

  }

  return false;
}
bool hasRapidFluctuation() {

  // Store the first valid readings and don't trigger a fault
  if (firstReading) {

    for (int i = 0; i < NUM_CELLS; i++) {
      previousVoltage[i] = battery.cellVoltage[i];
    }

    firstReading = false;
    return false;
  }

  for (int i = 0; i < NUM_CELLS; i++) {

    float difference = abs(battery.cellVoltage[i] - previousVoltage[i]);

    previousVoltage[i] = battery.cellVoltage[i];

    if (difference > RAPID_CHANGE_THRESHOLD) {
      return true;
    }
  }

  return false;
}
void processSafety() {

if (hasSensorFault()) {
    faultStartTime = 0;
    safetyState = SENSOR_FAULT;
}
else if (hasRapidFluctuation()) {
    faultStartTime = 0;
    safetyState = RAPID_FLUCTUATION;
}
else if (hasOverVoltage()) {
    faultStartTime = 0;
    safetyState = OVERVOLTAGE;
}
else if (hasWeakCell()) {
    faultStartTime = 0;
    safetyState = WEAK_CELL;
}
else {

    if (faultStartTime == 0) {
        faultStartTime = millis();
    }

    if (millis() - faultStartTime >= RECOVERY_DELAY) {
        safetyState = SAFE;
    }

}
  switch (safetyState) {

    case SAFE:
      digitalWrite(RELAY_PIN, HIGH);
      digitalWrite(LED_PIN, LOW);
      noTone(BUZZER_PIN);
      break;

    case WEAK_CELL:
      digitalWrite(RELAY_PIN, LOW);
      digitalWrite(LED_PIN, HIGH);
      tone(BUZZER_PIN, 1000);   // 1000 Hz
      Serial.println("WARNING: Weak Cell Detected");
      break;
    case OVERVOLTAGE:
      digitalWrite(RELAY_PIN, LOW);
      digitalWrite(LED_PIN, HIGH);
      tone(BUZZER_PIN, 1000);   // 1000 Hz
      Serial.println("WARNING: Overvoltage Detected");
      break;
    case SENSOR_FAULT:
      digitalWrite(RELAY_PIN, LOW);
      digitalWrite(LED_PIN, HIGH);
      tone(BUZZER_PIN, 1500);

      Serial.println("WARNING: Sensor Fault");
      break;
    case RAPID_FLUCTUATION:
      digitalWrite(RELAY_PIN, LOW);
      digitalWrite(LED_PIN, HIGH);
      tone(BUZZER_PIN, 1200);

      Serial.println("WARNING: Rapid Voltage Fluctuation");
      break;

      break;

    default:
      break;
  }
  
}