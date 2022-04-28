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
      // Sets the speed of the motor. Speed is from 0 to 255
      void setSpeed(uint8_t);

      // Sets the direction of the motor
      void setDirection(MotorDirection);

      // Stops the motor
      void stop();

      // Internal constructor. Use getMotor(id) instead
      Motor(uint8_t id);
    private:
      uint8_t _positive, _negative, _enable;
  };
  Motor* getMotor(uint8_t id);

  // GPIO
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

#endif // ifndef INCLUDE_WATERLIB_H
