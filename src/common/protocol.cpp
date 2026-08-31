// UART communication protocol between sensor node and relay node
#include "protocol.h"
#include <string.h>

// Simple XOR checksum
uint8_t computeChecksum(uint8_t *data, uint8_t len) {
  uint8_t checksum = 0;
  for (uint8_t i = 0; i < len; i++) {
    checksum = checksum ^ data[i]; // XOR checksum
  }
  return checksum;
}

// Sender side, extracts from frame to write into buffer
void encodeFrame(Frame *frame, uint8_t *outBuffer) {
  uint8_t idx = 0;
  outBuffer[idx++] = START_BYTE; // idx 0, should be 0xAA
  outBuffer[idx++] = NUM_SENSORS; // idx 1, length of payload, should be 5

  for (int i = 0; i < NUM_SENSORS; i++) {
    outBuffer[idx++] = frame->distances[i]; // idx 2,3,4,5,6 
  }

  uint8_t checksum = computeChecksum(&outBuffer[2], NUM_SENSORS);
  outBuffer[idx++] = checksum; // idx 7
}

void ParserInit(Parser *parser) {
  parser->state = IDLE;
  parser->payloadIndex = 0;
}

// Receiver side. returns True if successful
bool ParserProcess(Parser *parser, uint8_t byte, Frame *outFrame) {
  switch (parser->state) {

    case IDLE:
      if (byte == START_BYTE) {
        parser->state = LENGTH_CHECK;
      }
      // else: stay in IDLE, ignore stray bytes
      break;

    case LENGTH_CHECK:
      if (byte == NUM_SENSORS) { // only accept expected length
        parser->length = byte;
        parser->payloadIndex = 0;
        parser->state = PAYLOAD;
      } else {
        parser->state = IDLE; // unexpected length, resync
      }
      break;

    case PAYLOAD:
      parser->payload[parser->payloadIndex++] = byte;
      if (parser->payloadIndex >= parser->length) {
        parser->state = CHECKSUM;
      }
      break;

    case CHECKSUM: {
      uint8_t expected = computeChecksum(parser->payload, parser->length);
      parser->state = IDLE; // always reset after this, success or fail

      if (byte == expected) {
        // unpack into the Frame struct
        for (int i = 0; i < NUM_SENSORS; i++) {
          outFrame->distances[i] = parser->payload[i];
        }

        return true; // valid frame ready
      }

      // checksum mismatch
      break;
    }
  }
  return false;
}