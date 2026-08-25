// header file for ttc.cpp

// if ttc.h is not defined, define it. avoids multiple inclusion of the same file.
#ifndef TTC_H 
#define TTC_H

#include <Arduino.h>
const int numSensors = 5; // number of sensors

// 
enum result : uint8_t {
  OK = 0,
  BRAKE = 1,
  ABS = 2
};

// declaring update function for ttc.cpp, returns of type result (enum)
// parameters: array of type int16_t of size numSensors (5), and a uint32_t t (time in microseconds)
result ttcUpdate(const int16_t distances[numSensors], uint32_t t);

int16_t ttcSpeed(void);
int16_t ttcTime(void);
void ttcReset(void);

#endif