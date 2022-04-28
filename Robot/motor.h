#ifndef INCLUDE_MOTOR_H
#define INCLUDE_MOTOR_H

#include <arduino.h>

namespace WG {
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
}

#endif
