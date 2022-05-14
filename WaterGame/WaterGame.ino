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

  public:
    WaterGameRobot() : RobotBase(2129) {
      controller = WG::getController(CONTROLLER_ID);
      servo = new WG::GPIOServo(WG::getGPIO(SERVO_ID));
      servo2 = new WG::GPIOServo(WG::getGPIO(SERVO2_ID));
    }

    void robotPeriodic() override { 
      float stickX = controller->getAxis(LEFT_STICK_X);
      float stickY = controller->getAxis(LEFT_STICK_Y);

      uint8_t servoAngle = (uint8_t) (stickX * 80 + 90);
      uint8_t servoAngle2 = (uint8_t) (stickY * 80 + 90);

      servo->setAngle(servoAngle);
      servo2->setAngle(servoAngle2);
    }
};

WG::RobotBase* WG::createRobot() {
  return new WaterGameRobot();
}
