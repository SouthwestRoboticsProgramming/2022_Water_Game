#include "waterlib.h"

WG::Motor motor(0);

void setup() {
  WG::begin(2129);
}

void loop() {
  motor.setDirection(WG::MotorDirection::FORWARD);
  motor.setSpeed(255);
  delay(1000);
  motor.stop();
  delay(1000);
  motor.setDirection(WG::MotorDirection::REVERSE);
  motor.setSpeed(255);
  delay(1000);
  motor.stop();
  delay(1000);
}
