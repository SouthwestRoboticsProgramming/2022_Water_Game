#include "waterlib.h"

#define CONTROLLER_ID 0
#define SERVO_ID 0
#define SERVO2_ID 1

#define LEFT_STICK_X 0
#define LEFT_STICK_Y 1

class WaterGameRobot : public WG::RobotBase {
  private:
    WG::Controller* controller;
    WG::GPIOServo* servo;
    WG::GPIOServo* servo2;
    WG::Motor* motor;

  public:
    WaterGameRobot() : RobotBase(2129) {
      controller = WG::getController(CONTROLLER_ID);
      motor = WG::getMotor(0);
    }

    void robotPeriodic() override { 
      float stickX = controller->getAxis(LEFT_STICK_X);
      float stickY = controller->getAxis(LEFT_STICK_Y);
      
      if (stickY > 0) {
        motor->setSpeed((uint8_t) (stickY * 255));
        motor->setDirection(WG::MotorDirection::REVERSE);
      } else {
        motor->setSpeed((uint8_t) (stickY * -255));
        motor->setDirection(WG::MotorDirection::FORWARD);
      }
    }
};

WG::RobotBase* WG::createRobot() {
  return new WaterGameRobot();
}
