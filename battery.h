#ifndef BATTERY_H
#define BATTERY_H
#include <Arduino.h>
#include "config.h"

struct BatteryData {
  float cellVoltage[NUM_CELLS];
  float packVoltage;
  float averageVoltage;
  float imbalance;
  int highestCell;
  int lowestCell;
  String healthStatus;
};

extern BatteryData battery;

// Function declarations
void readCellVoltages();
void processBattery();
int findHighestCell();
int findLowestCell();
float calculateImbalance();
String classifyBatteryHealth();
#endif