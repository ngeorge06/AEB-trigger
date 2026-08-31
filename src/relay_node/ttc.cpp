
#include "ttc.h"
#include <Arduino.h> // for millis()

// Thresholds based on Highway Code Rule 126: ~0.67s reaction time,
// plus ~0.25s system latency budget = ~0.92s minimum safety margin.
// Rounded thresholds below give staged warnings before that margin runs out.
#define TTC_EMERGENCY_SEC 0.5
#define TTC_BRAKE_SEC     1.0
#define TTC_WARNING_SEC   2.0


void ttc_init(TTCState *state) {
  state->prevDistance = 255;
  state->prevTime = 0;
  state->hasPrev = false;
}

AEBStatus ttc_update(TTCState *state, uint8_t currentDistance) {
  uint32_t now = millis();

  // no valid reading right now — nothing to assess, stay safe, but don't
  // let a missing reading pretend to be "very close"
  if (currentDistance == 255) {
    state->hasPrev = false;
    return OK;
  }

  // first real reading ever, or resuming after a gap — no speed data yet
  if (!state->hasPrev) {
    state->prevDistance = currentDistance;
    state->prevTime = now;
    state->hasPrev = true;
    return OK;
  }

  float dt = (now - state->prevTime) / 1000.0; // seconds
  if (dt <= 0) {
    return OK; // guard against a zero/negative gap
  }

  float distanceChange = (float)state->prevDistance - (float)currentDistance; // cm, positive = closing
  float closingSpeed = (distanceChange / 100.0) / dt; // convert cm to m, then m/s

  // update state for next call, regardless of outcome below
  state->prevDistance = currentDistance;
  state->prevTime = now;

  if (closingSpeed <= 0) {
    // not approaching (steady, or moving away)
    return OK;
  }

  float ttcSeconds = (currentDistance / 100.0) / closingSpeed; // distance in meters / speed

  if (ttcSeconds < TTC_EMERGENCY_SEC) {
    return AEB;
  } else if (ttcSeconds < TTC_BRAKE_SEC) {
    return BRAKE;
  } 
  return OK;
}