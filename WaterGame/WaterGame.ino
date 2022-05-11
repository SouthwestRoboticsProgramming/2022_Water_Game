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
      digitalWrite(11, controller->getButton(0));
      servo->setAngle((uint8_t) (controller->getAxis(5) * 80.0f + 90));
    }
};

WG::RobotBase* WG::createRobot() {
  return new WaterGameRobot();
}
