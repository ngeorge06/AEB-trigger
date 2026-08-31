// header file for ttc.cpp

// if ttc.h is not defined, define it. avoids multiple inclusion of the same file.
#ifndef TTC_H 
#define TTC_H

#include <stdint.h>


// Different states
enum Status : uint8_t {
  OK, BRAKE, AEB
};

struct TTCState {
  uint8_t prevDistance; // cm, from the last call
  uint32_t prevTime; // millis() at the last call
  bool hasPrev; // false until the first real reading comes in
} TTCState;

void ttcInit(TTCState *state);
AEBStatus ttcUpdate(TTCState *state, uint8_t currentDistance);

#endif