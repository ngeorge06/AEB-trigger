#include <Arduino.h>

const int hazards = 2;
const int brake = 3;

void setup() {
  pinMode(hazards, OUTPUT);
  pinMode(brake, OUTPUT);
}

void loop() {
  digitalWrite(hazards, LOW);
  digitalWrite(brake, LOW);
  delay(1000);
  digitalWrite(brake, HIGH);
  delay(2000);
  digitalWrite(hazards, LOW);
  delay(200);
  digitalWrite(hazards, HIGH);
  delay(200);
  digitalWrite(hazards, LOW);
  delay(200);
  digitalWrite(hazards, HIGH);
  delay(200);
  digitalWrite(hazards, LOW);
  delay(200);
  digitalWrite(hazards, HIGH);
  delay(200);
}
