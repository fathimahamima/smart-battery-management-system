#include "runtime.h"
#include "safety.h"
#include "battery.h"
FaultLog lastFault = { NO_FAULT, 0 };
RuntimeMode runtimeMode = NORMAL;
bool adcFrozen = false;
bool relayMismatch = false;
float lastAverageVoltage = 0.0;
unsigned long lastVoltageChange = 0;

const unsigned long ADC_FREEZE_TIME = 5000; // 5 seconds
const float ADC_CHANGE_THRESHOLD = 0.02;

bool hasInvalidReading() {

  for (int i = 0; i < NUM_CELLS; i++) {

    if (battery.cellVoltage[i] < 0.0 ||
        battery.cellVoltage[i] > 5.0) {

      return true;
    }

  }

  return false;
}
void logFault(FaultType fault) {

  if (lastFault.fault != fault) {

    lastFault.fault = fault;
    lastFault.timestamp = millis();

  }

}
bool hasRelayMismatch() {

  bool expectedRelayState = (safetyState == SAFE);

  bool actualRelayState = digitalRead(RELAY_PIN);

  if (expectedRelayState != actualRelayState) {
    return true;
  }

  return false;
}
void processRuntime() {

  if (hasInvalidReading()) {

    runtimeMode = SHUTDOWN;
    logFault(INVALID_READING_FAULT);
    return;

  }
  relayMismatch = hasRelayMismatch();

if (relayMismatch) {

  runtimeMode = SHUTDOWN;
  logFault(SENSOR_FAULT_LOG);
  return;

}

  float difference = abs(battery.averageVoltage - lastAverageVoltage);

  if (difference > ADC_CHANGE_THRESHOLD) {

    lastAverageVoltage = battery.averageVoltage;
    lastVoltageChange = millis();

    adcFrozen = false;
  }
  else {

    if (millis() - lastVoltageChange >= ADC_FREEZE_TIME) {
      adcFrozen = true;
    }

  }

  if (adcFrozen) {

    runtimeMode = DEGRADED;
    logFault(ADC_FROZEN_FAULT);

  }
  else {

    switch (safetyState) {

      case SAFE:
        runtimeMode = NORMAL;
        break;

      case RAPID_FLUCTUATION:
        runtimeMode = DEGRADED;
        logFault(RAPID_FLUCTUATION_FAULT);
        break;

      case WEAK_CELL:
        runtimeMode = FAILSAFE;
        logFault(WEAK_CELL_FAULT);
        break;

      case OVERVOLTAGE:
        runtimeMode = FAILSAFE;
        logFault(OVERVOLTAGE_FAULT);
        break;

      case SENSOR_FAULT:
        runtimeMode = SHUTDOWN;
        logFault(SENSOR_FAULT_LOG);
        break;

      default:
        runtimeMode = FAILSAFE;
        break;
    }

  }
}