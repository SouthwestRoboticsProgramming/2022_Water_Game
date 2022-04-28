#include "motor.h"

namespace WG {
  static const uint8_t MOTOR_ENABLE[]   = {9, 4, 12, 14};
  static const uint8_t MOTOR_POSITIVE[] = {0, 3, 21, 18};
  static const uint8_t MOTOR_NEGATIVE[] = {2, 5, 19, 16};

  static Motor MOTORS[] = {
    Motor(0), Motor(1), Motor(2), Motor(3)
  };

  Motor* getMotor(uint8_t id) {
    return &MOTORS[id];
  }

  Motor::Motor(uint8_t id) {
    _enable = MOTOR_ENABLE[id];
    _positive = MOTOR_POSITIVE[id];
    _negative = MOTOR_NEGATIVE[id];

    pinMode(_enable, OUTPUT);
    pinMode(_positive, OUTPUT);
    pinMode(_negative, OUTPUT);

    setDirection(MotorDirection::FORWARD);
    stop();
  }

  void Motor::setSpeed(uint8_t speed) {
    analogWrite(_enable, speed);
  }

  void Motor::setDirection(MotorDirection direction) {
    if (direction == MotorDirection::FORWARD) {
      digitalWrite(_positive, HIGH);
      digitalWrite(_negative, LOW);
    } else {
      digitalWrite(_positive, LOW);
      digitalWrite(_negative, HIGH);
    }
  }

  void Motor::stop() {
    analogWrite(_enable, 0);
  }
}
