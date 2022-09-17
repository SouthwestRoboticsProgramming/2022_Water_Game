#ifndef INCLUDE_WATERNET_H
#define INCLUDE_WATERNET_H

#include "waterlib.h"

/*
 * Teensy protocol:
 * 
 * Startup sequence:
 *   1. Check for tethered connection and initialize it if present, otherwise try wifi
 *   2. Repeatedly send CHECK until receive HELLO
 *   3. Send HANDSHAKE with team number and deviceType = ROBOT(0x00)
 * 
 * While running:
 *   Respond to PING with PONG with same payload (for latency checking)
 *   Change state if STATE is received
 *   Decode controller data from CONTROLS
 *   
 *   Send LOG if a message is to be logged
 */

#define NET_SERIAL Serial1
#define SERIAL_BAUD 115200

namespace WG {
  // Send a string message to the driver station.
  // Try to avoid this as much as possible, as it consumes a lot of bandwidth
  void log(const char* str);
  
  namespace Internal {
    const uint8_t MAX_PACKET_LEN = 64;

    // To make sure the wifi upload packet is real and not just a read error
    const uint32_t WIFI_UPLOAD_MAGIC = 0x7ABC045A;
    
    enum PacketTypeIn : uint8_t {
      HELLO = 0x00,
      PING = 0x01,
      STATE = 0x02,
      CONTROLS = 0x03,
      INIT_WIFI_UPLOAD = 0x04
    };
    enum PacketTypeOut : uint8_t {
      CHECK = 0x00,
      PONG = 0x01,
      HANDSHAKE = 0x02,
      LOG = 0x03
    };

    struct PacketInPing {
      uint64_t payload;
    };
    struct PacketInState {
      uint8_t state;
    };
    struct PacketInControls {
      uint8_t controllerId;
      uint32_t buttonMask;
      int8_t axes[8];
    };
    struct PacketInInitWifiUpload {
      uint32_t magic;
    };

    struct PacketOutCheck {
      uint8_t _TYPE = PacketTypeOut::CHECK;
    };
    struct PacketOutPong {
      uint8_t _TYPE = PacketTypeOut::PONG;
      uint64_t payload;
    };
    struct PacketOutHandshake {
      uint8_t _TYPE = PacketTypeOut::HANDSHAKE;
      uint8_t deviceType = 0x00;
      uint16_t team;
    };
    struct PacketOutLog {
      uint8_t _TYPE = PacketTypeOut::LOG;
      char buf[33]; // One more than 32 for null-termination
    };
    
    void readIncomingPackets();
  }
}

#endif
