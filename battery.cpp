#include <Arduino.h>
#include "battery.h"

BatteryData battery;

void readCellVoltages() {

  battery.packVoltage = 0;

  for (int i = 0; i < NUM_CELLS; i++) {

    int adcValue = analogRead(cellPins[i]);

    battery.cellVoltage[i] =
      MIN_CELL_VOLTAGE +
      ((adcValue / 4095.0) * (MAX_CELL_VOLTAGE - MIN_CELL_VOLTAGE));

    battery.packVoltage += battery.cellVoltage[i];
  }

  battery.averageVoltage = battery.packVoltage / NUM_CELLS;
}
int findHighestCell() {
  int highest = 0;

  for (int i = 1; i < NUM_CELLS; i++) {
    if (battery.cellVoltage[i] > battery.cellVoltage[highest]) {
      highest = i;
    }
  }

  return highest;
}
int findLowestCell() {
  int lowest = 0;

  for (int i = 1; i < NUM_CELLS; i++) {
    if (battery.cellVoltage[i] < battery.cellVoltage[lowest]) {
      lowest = i;
    }
  }

  return lowest;
}
float calculateImbalance() {

  float difference =
      battery.cellVoltage[battery.highestCell] -
      battery.cellVoltage[battery.lowestCell];

  return (difference / battery.averageVoltage) * 100.0;
}
String classifyBatteryHealth() {

  if (battery.imbalance < HEALTHY_THRESHOLD)
    return "Healthy";

  else if (battery.imbalance < MINOR_THRESHOLD)
    return "Minor Imbalance";

  else if (battery.imbalance < CRITICAL_THRESHOLD)
    return "Critical Imbalance";

  else
    return "Pack Failure";
}
void processBattery() {

  readCellVoltages();

  battery.highestCell = findHighestCell();
  battery.lowestCell = findLowestCell();
  battery.imbalance = calculateImbalance();
  battery.healthStatus = classifyBatteryHealth();


}