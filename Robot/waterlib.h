#ifndef INCLUDE_WATERLIB_H
#define INCLUDE_WATERLIB_H
#include <arduino.h>

namespace WG {
  // Call if your program encounters an unrecoverable error.
  void crash(char* message);
  
  // Sets up the WaterGame library. This must be called in setup()
  void begin(int teamNumber);

  // Motors
  enum MotorDirection { FORWARD, REVERSE };
  const uint8_t MOTOR_COUNT = 4;
  class Motor {
    public:
      Motor(uint8_t id);

      // Sets the speed of the motor. Speed is from 0 to 255
      void setSpeed(uint8_t);

      // Sets the direction of the motor
      void setDirection(MotorDirection);

      // Stops the motor
      void stop();
    private:
      uint8_t _positive, _negative, _enable;
  };
}

#endif // ifndef INCLUDE_WATERLIB_H
