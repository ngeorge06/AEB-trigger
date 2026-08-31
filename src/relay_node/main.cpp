#include <Arduino.h>
#include <SoftwareSerial.h>
#include "protocol.h"

const int hazards = 2;
const int brake = 3;

SoftwareSerial relay(12, 13); // RX, TX

Parser parser;


void setup() {
  relay.begin(9600);
  Serial.begin(9600); // to computer

  ParserInit(&parser); // initialise parser

  pinMode(hazards, OUTPUT);
  pinMode(brake, OUTPUT);
}

void loop() {
  while (relay.available()){
    uint8_t b = relay.read(); //one byte at a time

    Frame frame;
    if (ParserProcess(&parser, b, &frame)) {
      
      for (int i = 0; i < NUM_SENSORS; i++) {
        Serial.print(frame.distances[i]);
        Serial.print(", ")
      }
      Serial.println();
      
    }


  }
}
