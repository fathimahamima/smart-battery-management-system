#include "battery.h"
#include "safety.h"
#include "display.h"
#include "runtime.h"
unsigned long previousUpdate = 0;
const unsigned long UPDATE_INTERVAL = 1000; // 1 second
void setup() {
  Serial.begin(115200);
  initDisplay();
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);   // Relay ON initially
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
}

void loop() {

  unsigned long currentMillis = millis();

  if (currentMillis - previousUpdate >= UPDATE_INTERVAL) {

    previousUpdate = currentMillis;

    processBattery();
    processSafety();
    processRuntime();

    updateDisplay();

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
  }
}