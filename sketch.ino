#include "battery.h"

void setup() {
  Serial.begin(115200);
}

void loop() {

  processBattery();

  for (int i = 0; i < NUM_CELLS; i++) {
    Serial.print("Cell ");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.print(battery.cellVoltage[i], 2);
    Serial.println(" V");
  }

  Serial.println();

  Serial.print("Pack Voltage: ");
  Serial.print(battery.packVoltage, 2);
  Serial.println(" V");

  Serial.print("Average Voltage: ");
  Serial.print(battery.averageVoltage, 2);
  Serial.println(" V");

  Serial.println("----------------------");

  delay(1000);
}