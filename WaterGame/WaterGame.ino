#include "waterlib.h"

#define CONTROLLER_ID 0
#define MOTOR_ID 0
#define GPIO_ID 0
#define SERVO_ID 1

class WaterGameRobot : public WG::RobotBase {
  private:
    WG::Controller* controller;
    WG::Motor* motor;
    WG::GPIO* gpio;
    WG::GPIOServo* servo;

  public:
    WaterGameRobot() : RobotBase(2129) {
      controller = WG::getController(CONTROLLER_ID);
      motor = WG::getMotor(MOTOR_ID);
      gpio = WG::getGPIO(GPIO_ID);
      servo = new WG::GPIOServo(WG::getGPIO(SERVO_ID));
    }

    void robotPeriodic() override { 
      if (controller->getButton(0)) {
        motor->setSpeed(255);
      } else {
        motor->stop();
      }

      digitalWrite(11, controller->getButton(0));
      servo->setAngle(controller->getButton(2) ? 170 : 10);
    }
};

WG::RobotBase* WG::createRobot() {
  return new WaterGameRobot();
}
