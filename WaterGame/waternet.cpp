#include "waterlib.h"

namespace WG {
  static const uint8_t START_BYTE = 0xA5;
  
  void log(const char* str) {
    WG::Internal::PacketOutLog packet;
    packet.buf[32] = 0;

    uint8_t index = 0;
    while (*str) {
      packet.buf[index] = *str;
      index++;
      str++;
      if (index == 32) {
        Serial.write(START_BYTE);
        Serial.write(sizeof(WG::Internal::PacketOutLog));
        Serial.write((uint8_t*) &packet, sizeof(WG::Internal::PacketOutLog));
        index = 0;
      }
    }

    if (index != 0) {
      packet.buf[index] = 0;
      Serial.write(START_BYTE);
      Serial.write(sizeof(WG::Internal::PacketOutLog));
      Serial.write((uint8_t*) &packet, sizeof(WG::Internal::PacketOutLog));
    }
  }
  
  namespace Internal {
    static uint8_t readBuf[MAX_PACKET_LEN];
    static uint8_t readPtr;
    static uint8_t readLen;

    static uint8_t byteRead;

    enum ReadState { AWAIT_START, READ_LENGTH, READ_CONTENT };
    static ReadState readState;

    #define CHECK_LEN(type) if (readLen != sizeof(type) + 1) {readState = ReadState::AWAIT_START; return; }
    #define DECODE_PACKET(type) (type*) (&readBuf[1])
    #define SEND_PACKET(packet, type) Serial.write(START_BYTE); Serial.write(sizeof(type)); Serial.write((uint8_t*) &packet, sizeof(type))

    static inline void handlePacketHello() {
      if (readLen != 1)
        readState = ReadState::AWAIT_START;
    }

    static inline void handlePacketPing() {
      CHECK_LEN(PacketInPing);
      PacketInPing* ping = DECODE_PACKET(PacketInPing);

      PacketOutPong pong;
      pong.payload = ping->payload;

      SEND_PACKET(pong, PacketOutPong);
    }

    static inline void handlePacketState() {
      CHECK_LEN(PacketInState);
      PacketInState* state = DECODE_PACKET(PacketInState);

      WG::Internal::enterState((RobotState) state->state);
    }

    static inline void handlePacketControls() {
      CHECK_LEN(PacketInControls);
      PacketInControls* controls = DECODE_PACKET(PacketInControls);

      Controller* c = getController(controls->controllerId);
      c->read(controls);
    }

    #undef CHECK_LEN
    #undef DECODE_PACKET
    #undef SEND_PACKET

    void handlePacket() {
      uint8_t packetId = readBuf[0];

      switch (packetId) {
        case PacketTypeIn::HELLO:
          handlePacketHello();
          break;
        case PacketTypeIn::PING:
          handlePacketPing();
          break;
        case PacketTypeIn::STATE:
          handlePacketState();
          break;
        case PacketTypeIn::CONTROLS:
          handlePacketControls();
          break;
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
            if (readLen > MAX_PACKET_LEN) {
              readState = ReadState::AWAIT_START;
              break;
            }
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
