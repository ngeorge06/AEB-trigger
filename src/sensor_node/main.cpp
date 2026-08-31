#include <Arduino.h>
#include <SoftwareSerial.h>
#include "protocol.h"

const int numSensors = NUM_SENSORS; // number of sensors, from protocol.h

// trig pin first, followed by echo pin
const int trigPins[numSensors] = {2,4,6,8,10};
const int echoPins[numSensors] = {3,5,7,9,11};

SoftwareSerial relay(12, 13); // RX, TX

long getDistance(int trigPin, int echoPin) {

  digitalWrite(trigPin, LOW); // HC-SR04 suggests a short LOW for a clean HIGH pulse
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // pulseIn is blocking.
  long duration = pulseIn(echoPin, HIGH, 30000); // time how long the echo pin recieves HIGH

  if (duration == 0) {
    return 255; // error value
  }

  long cm = duration * 0.0343 / 2; // cm per microseconds, divided by 2 (accounting for both ways)
  if (cm > 200) {
    return 255; // out of range
  }
  return (uint8_t)cm;
}

void setup() {
  relay.begin(9600);

  // initialise pins
  for (int i = 0; i < numSensors; i++) {
    pinMode(trigPins[i], OUTPUT);
    pinMode(echoPins[i], INPUT);
  }
}

void loop() {
  Frame frame; // empty frame

  // get distance
  for (int i = 0; i < numSensors; i++) {
    frame.distances[i] = getDistance(trigPins[i], echoPins[i]);
    delay(12); // avoid cross-talk, need 60ms minimum for re-trigger
  }

  // prepare for transfer
  uint8_t buffer[FRAME_SIZE]; // 7 bytes
  encodeFrame(&frame, buffer); // buffer is automatically passed as a pointer

  // SEND
  for (int i = 0; i < FRAME_SIZE; i++){
    relay.write(buffer[i])
  }
}



