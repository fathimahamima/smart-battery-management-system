#ifndef RUNTIME_H
#define RUNTIME_H

enum RuntimeMode {
  NORMAL,
  DEGRADED,
  FAILSAFE,
  SHUTDOWN
};
enum FaultType {
  NO_FAULT,
  WEAK_CELL_FAULT,
  OVERVOLTAGE_FAULT,
  SENSOR_FAULT_LOG,
  RAPID_FLUCTUATION_FAULT,
  ADC_FROZEN_FAULT,
  INVALID_READING_FAULT
};

struct FaultLog {
  FaultType fault;
  unsigned long timestamp;
};

extern FaultLog lastFault;

extern RuntimeMode runtimeMode;
extern bool adcFrozen;
extern bool relayMismatch;

void processRuntime();
bool hasInvalidReading();

#endif