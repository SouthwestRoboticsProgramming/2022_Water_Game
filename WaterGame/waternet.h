#ifndef INCLUDE_WATERNET_H
#define INCLUDE_WATERNET_H

#include "waterlib.h"

namespace WG {
  namespace Internal {
    enum PacketTypeIn : uint8_t {
      TEST = 0x01
    };

    struct PacketInTest {
      bool led;
    };
    
    void readIncomingPackets();
  }
}

#endif
