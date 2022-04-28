#include "gpio.h"

namespace WG {
  static const uint8_t GPIO_PINS[] = {1, 6, 11, 13, 15, 17, 20};

  static GPIO GPIOS[] = {
    GPIO(0), GPIO(1), GPIO(2), GPIO(3), GPIO(4), GPIO(5), GPIO(6)
  };

  GPIO* getGPIO(uint8_t id) {
    return &GPIOS[id];
  }

  GPIO::GPIO(uint8_t id) {
    _pin = GPIO_PINS[id];
  }

  bool GPIO::get() {
    _setPinMode(INPUT_PULLUP);
    return digitalRead(_pin);
  }

  void GPIO::set(bool value) {
    _setPinMode(OUTPUT);
    digitalWrite(_pin, value);
  }

  inline void GPIO::_setPinMode(uint8_t mode) {
    if (mode != _pinMode) {
      pinMode(_pin, mode);
      _pinMode = mode;
    }
  }
}
