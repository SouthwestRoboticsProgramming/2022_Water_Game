#include "waterlib.h"

WG::Motor motor(0);
WG::GPIO gpio(0);

void setup() {
  WG::begin(2129);
}

inline void runInDirection(WG::MotorDirection dir) {
  motor.setDirection(dir);
  motor.setSpeed(255);
  gpio.set(true);
  delay(1000);
  motor.setSpeed(0);
  gpio.set(false);
  delay(1000);
}

void loop() {
  runInDirection(WG::MotorDirection::FORWARD);
  runInDirection(WG::MotorDirection::REVERSE);
}
