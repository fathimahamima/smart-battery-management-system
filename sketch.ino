#include "battery.h"
#include "safety.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
unsigned long previousUpdate = 0;
const unsigned long UPDATE_INTERVAL = 1000; // 1 second
void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Battery System");

  lcd.setCursor(0, 1);
  lcd.print("Starting...");
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

    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("Pack:");
    lcd.print(battery.packVoltage, 2);
    lcd.print("V");

    lcd.setCursor(0, 1);

    switch (safetyState) {

      case SAFE:
        lcd.print("SAFE");
        break;

      case WEAK_CELL:
        lcd.print("WEAK CELL");
        break;

      case OVERVOLTAGE:
        lcd.print("OVERVOLTAGE");
        break;

      case SENSOR_FAULT:
        lcd.print("SENSOR FAULT");
        break;

      case RAPID_FLUCTUATION:
        lcd.print("RAPID CHANGE");
        break;

      default:
        lcd.print("UNKNOWN");
        break;
    }

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