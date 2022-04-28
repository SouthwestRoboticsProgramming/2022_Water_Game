#include "waterlib.h"

namespace WG {
  void begin(int teamNumber) {
    Serial.begin(9600);
  }

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
