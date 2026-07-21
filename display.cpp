#include "display.h"
#include "battery.h"
#include "safety.h"
#include "runtime.h"

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

  // Fault Priority Override
if (safetyState != SAFE) {

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("!! WARNING !!");

  lcd.setCursor(0, 1);

  switch (safetyState) {

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
      lcd.print("FAULT");
      break;
  }

  return;
}
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
  case ANALYTICS_SCREEN:

    lcd.setCursor(0,0);
    lcd.print("Avg:");
    lcd.print(battery.averageVoltage,2);
    lcd.print("V");

    lcd.setCursor(0,1);
    lcd.print("Imb:");
    lcd.print(battery.imbalance,1);
    lcd.print("%");

    break;
  case PROTECTION_SCREEN:

  lcd.setCursor(0,0);
  lcd.print("Relay: ");

  if (safetyState == SAFE)
  lcd.print("ON ");
  else
  lcd.print("OFF");

  lcd.setCursor(0,1);
  lcd.print("State:");

  switch(safetyState){

    case SAFE:
      lcd.print("SAFE");
      break;

    case WEAK_CELL:
      lcd.print("WEAK");
      break;

    case OVERVOLTAGE:
      lcd.print("OV");
      break;

    case SENSOR_FAULT:
      lcd.print("SENSOR");
      break;

    case RAPID_FLUCTUATION:
      lcd.print("RAPID");
      break;

    default:
      lcd.print("UNKNOWN");
  }

  break;

  
  case DIAGNOSTIC_SCREEN:

  lcd.setCursor(0, 0);
  lcd.print("Mode:");

switch (runtimeMode) {

  case NORMAL:
    lcd.print("NORMAL");
    break;

  case DEGRADED:
    lcd.print("DEGRADED");
    break;

  case FAILSAFE:
    lcd.print("FAILSAFE");
    break;

  case SHUTDOWN:
    lcd.print("SHUTDOWN");
    break;
}

  lcd.setCursor(0, 1);

  if (safetyState == SAFE)
    lcd.print("No Faults");
  else
    lcd.print("Fault Active");

  break;
  default:
    lcd.setCursor(0,0);
    lcd.print("Coming Soon");
    break;
}
}