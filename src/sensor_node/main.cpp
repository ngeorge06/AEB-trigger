#include <Arduino.h>

const int numSensors = 5; // number of sensors

// trig pin first, followed by echo pin
const int trigPins[numSensors] = {2,4,6,8,10};
const int echoPins[numSensors] = {3,5,7,9,11};

// array to store distance values for each sensor
long distances[numSensors]; 

// define functions
long getDistance(int trigPin, int echoPin);
void serialOutput();

void setup() {
  Serial.begin(115200);

  // initialise pins
  for (int i = 0; i < numSensors; i++) {
    pinMode(trigPins[i], OUTPUT);
    pinMode(echoPins[i], INPUT);
  }
}

void loop() {

  // start time (for response time calculation)
  uint32_t t0 = micros(); 

  for (int i = 0; i < numSensors; i++) {
    distances[i] = getDistance(trigPins[i], echoPins[i]);
    delay(12); // avoid cross-talk, need 60ms minimum for re-trigger
  }

  serialOutput();

  uint32_t t1 = micros(); //end time 
  Serial.println(t1 - t0);

  delay(1000); // slowing down for debugging purposes.
}

long getDistance(int trigPin, int echoPin) {

  digitalWrite(trigPin, LOW); // HC-SR04 suggests a short LOW for a clean HIGH pulse
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // pulseIn is blocking.
  long duration = pulseIn(echoPin, HIGH, 30000); // time how long the echo pin recieves HIGH

  if (duration == 0) {
    return -1; // error value
  }

  return duration * 0.0343 / 2; // cm per microseconds, divided by 2 (accounting for both ways)
}

// to modify (transfer via UART instead of printing to serial monitor)
void serialOutput() {
  for (int i = 0; i < numSensors; i++) {
    Serial.print(distances[i]);
    Serial.print(",");
  }
}