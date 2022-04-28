#ifndef INCLUDE_WATERLIB_H
#define INCLUDE_WATERLIB_H
#include <arduino.h>

#include "motor.h"
#include "gpio.h"

namespace WG {
  // Sets up the WaterGame library. This must be called in setup()
  void begin(int teamNumber);
}

#endif // ifndef INCLUDE_WATERLIB_H
