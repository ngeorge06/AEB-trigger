// UART communication protocol between sensor node and relay node

#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>

#define START_BYTE 0xAA // 10101010, statistically less likely to have errors compared to 0xFF or 0x00
#define NUM_SENSORS 5
#define FRAME_SIZE (2 + NUM_SENSORS + 1) // START + LENGTH + payload + CHECKSUM

struct Frame{
  uint8_t distances[NUM_SENSORS]; // 0-255
};

enum State {
  IDLE, LENGTH_CHECK, PAYLOAD, CHECKSUM
};

struct Parser {
  State state;
  uint8_t length;
  uint8_t payload[NUM_SENSORS];
  uint8_t payloadIndex;
};

void ParserInit(Parser *parser);
bool ParserProcess(Parser *parser, uint8_t byte, Frame *outFrame);
uint8_t computeChecksum(uint8_t *data, uint8_t len);
void encodeFrame(Frame *frame, uint8_t *outBuffer);

#endif