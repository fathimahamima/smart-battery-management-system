#ifndef DISPLAY_H
#define DISPLAY_H

enum DisplayScreen {
  OVERVIEW_SCREEN,
  CELL_SCREEN,
  ANALYTICS_SCREEN,
  PROTECTION_SCREEN,
  DIAGNOSTIC_SCREEN
};

void initDisplay();
void updateDisplay();

#endif