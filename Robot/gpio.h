#ifndef INCLUDE_GPIO_H
#define INCLUDE_GPIO_H

#include <arduino.h>

namespace WG {
  const uint8_t GPIO_COUNT = 7;
  
  class GPIO {
    public:
      bool get();
      void set(bool);

      // Internal constructor. Use getGPIO(id) instead
      GPIO(uint8_t id);
    private:
      uint8_t _pin;
      uint8_t _pinMode;

      inline void _setPinMode(uint8_t mode);
  };
  
  GPIO* getGPIO(uint8_t id);
}

#endif
