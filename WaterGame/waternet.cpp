#include "waterlib.h"

namespace WG {
  namespace Internal {
    static const uint8_t START_BYTE = 0xA5;
    static const uint8_t BUFFER_SIZE = 64;

    static const int ERROR_CODE_INVALID_LEN = 1000;
    static const int ERROR_CODE_INVALID_PACKET_ID = 500;
    static const int ERROR_CODE_INCORRECT_LEN = 100;
    
    static uint8_t readBuf[BUFFER_SIZE];
    static uint8_t readPtr;
    static uint8_t readLen;

    static uint8_t byteRead;

    enum ReadState { AWAIT_START, READ_LENGTH, READ_CONTENT };
    static ReadState readState;

    void panic(int i) {
      digitalWrite(11, HIGH);
      delay(i);
      digitalWrite(11, LOW);
      delay(i);
    }

    void handlePacket() {
      uint8_t packetId = readBuf[0];
      if (packetId == PacketTypeIn::TEST) {
        if (readLen != sizeof(PacketInTest) + 1)
          panic(ERROR_CODE_INCORRECT_LEN);

        PacketInTest* packet = (PacketInTest*) (&readBuf[1]);

        digitalWrite(11, packet->led ? HIGH : LOW);
      } else {
        panic(ERROR_CODE_INVALID_PACKET_ID);
      }
    }

    void readIncomingPackets() {
      if (!Serial) return;

      while (Serial.available() > 0) {
        byteRead = Serial.read();

        switch (readState) {
          case ReadState::AWAIT_START:
            if (byteRead == START_BYTE)
              readState = ReadState::READ_LENGTH;
            break;
          case ReadState::READ_LENGTH:
            readLen = byteRead;
            if (readLen > BUFFER_SIZE)
              panic(ERROR_CODE_INVALID_LEN);
            readPtr = 0;
            readState = ReadState::READ_CONTENT;
            break;
          case ReadState::READ_CONTENT:
            readBuf[readPtr++] = byteRead;
            if (readPtr == readLen) {
              handlePacket();
              readState = ReadState::AWAIT_START;
            }
            break;
        }
      }
    }
  }
}
