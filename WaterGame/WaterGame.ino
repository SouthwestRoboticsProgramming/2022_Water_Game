#include "waterlib.h"

#define CONTROLLER_ID 0
#define MOTOR_ID 0
#define GPIO_ID 0
#define SERVO_ID 1

#define LEFT_STICK_X 0
#define LEFT_STICK_Y 1

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
      float stickX = controller->getAxis(LEFT_STICK_X);
      float stickY = controller->getAxis(LEFT_STICK_Y);

      uint8_t servoAngle = (uint8_t) (stickX * 80 + 90);
      uint8_t motorSpeed = (uint8_t) (abs(stickY) * 255);      
      WG::MotorDirection motorDirection;
      if (stickY <= 0) {
        motorDirection = WG::MotorDirection::FORWARD;
      } else {
        motorDirection = WG::MotorDirection::REVERSE;
      }

      servo->setAngle(servoAngle);
      motor->setSpeed(motorSpeed);
      motor->setDirection(motorDirection);
    }
};

WG::RobotBase* WG::createRobot() {
  return new WaterGameRobot();
}
