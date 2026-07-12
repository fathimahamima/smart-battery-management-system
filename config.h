#ifndef CONFIG_H
#define CONFIG_H

// Number of battery cells
#define NUM_CELLS 4

// Analog pins
const int cellPins[NUM_CELLS] = {34, 35, 32, 33};

// Battery voltage range
const float MIN_CELL_VOLTAGE = 3.0;
const float MAX_CELL_VOLTAGE = 4.2;

// Health thresholds (%)
const float HEALTHY_THRESHOLD = 5.0;
const float MINOR_THRESHOLD = 10.0;
const float CRITICAL_THRESHOLD = 20.0;

#endif