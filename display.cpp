#include "display.h"
#include "battery.h"
#include "safety.h"

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
DisplayScreen currentScreen = OVERVIEW_SCREEN;

unsigned long previousScreenChange = 0;
const unsigned long SCREEN_INTERVAL = 2000; // 2 seconds

void initDisplay() {

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Battery System");

  lcd.setCursor(0, 1);
  lcd.print("Starting...");
}

void updateDisplay() {
  unsigned long currentMillis = millis();

if (currentMillis - previousScreenChange >= SCREEN_INTERVAL) {

  previousScreenChange = currentMillis;

  currentScreen = (DisplayScreen)((currentScreen + 1) % 5);

}

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Pack:");
  lcd.print(battery.packVoltage, 2);
  lcd.print("V");

  lcd.setCursor(0, 1);

  switch (currentScreen) {

  case OVERVIEW_SCREEN:

    lcd.setCursor(0,0);
    lcd.print("Pack:");
    lcd.print(battery.packVoltage,2);
    lcd.print("V");

    lcd.setCursor(0,1);

    switch(safetyState){

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
    }

    break;

  case CELL_SCREEN:

    lcd.setCursor(0,0);
    lcd.print("C1:");
    lcd.print(battery.cellVoltage[0],1);

    lcd.print(" C2:");
    lcd.print(battery.cellVoltage[1],1);

    lcd.setCursor(0,1);

    lcd.print("C3:");
    lcd.print(battery.cellVoltage[2],1);

    lcd.print(" C4:");
    lcd.print(battery.cellVoltage[3],1);

    break;

  default:
    lcd.setCursor(0,0);
    lcd.print("Coming Soon");
    break;
}
}