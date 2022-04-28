#include "waterlib.h"

WG::Motor* motor;
WG::GPIO* gpio;

void setup() {
  WG::begin(2129);

  motor = WG::getMotor(0);
  gpio = WG::getGPIO(0);
}

inline void runInDirection(WG::MotorDirection dir) {
  motor->setDirection(dir);
  motor->setSpeed(255);
  gpio->set(true);
  delay(1000);
  motor->setSpeed(0);
  gpio->set(false);
  delay(1000);
}

void loop() {
  runInDirection(WG::MotorDirection::FORWARD);
  runInDirection(WG::MotorDirection::REVERSE);
}
