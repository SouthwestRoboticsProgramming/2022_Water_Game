#ifndef INCLUDE_WATERLIB_H
#define INCLUDE_WATERLIB_H

#include <arduino.h>
#include <Servo.h>

namespace WG {
  enum RobotState {
    DISABLED,
    TELEOP
  };
  RobotState getCurrentState();
  
  class RobotBase {
    public:
      RobotBase(int team);

      virtual void robotInit() {}
      virtual void robotPeriodic() {}

      virtual void disabledInit() {}
      virtual void disabledPeriodic() {}

      virtual void teleopInit() {}
      virtual void teleopPeriodic() {}
  };
  extern RobotBase* createRobot();

  const uint8_t AXIS_COUNT = 8;
  const uint8_t CONTROLLER_COUNT = 4;
  class Controller {
    public:
      bool getButton(uint8_t id);
      float getAxis(uint8_t id);

      // Internal constructor
      Controller(uint8_t id);
    private:
      uint32_t buttons;
      float axes[AXIS_COUNT];
  };
  Controller* getController(uint8_t id);
  
  enum MotorDirection {
    FORWARD,
    REVERSE
  };

  const uint8_t MOTOR_COUNT = 4;
  class Motor {
    public:
      // Speed is from 0 (stopped) to 255 (full power).
      void setSpeed(uint8_t);

      void setDirection(MotorDirection);

      // Equivalent to setSpeed(0).
      inline void stop() { setSpeed(0); }

      // Internal constructor
      Motor(uint8_t id);
    private:
      uint8_t positive, negative, enable;
  };
  Motor* getMotor(uint8_t id);

  // Forward declaration
  class GPIOServo;

  const uint8_t GPIO_COUNT = 7;
  class GPIO {
    public:
      // Defaults to true if no input is present
      bool read();
      
      void write(bool);

      // Internal constructor
      GPIO(uint8_t id);
    private:
      uint8_t pin;
      uint8_t currentPinMode, defaultPinMode;
      void setPinMode(uint8_t);

    friend class GPIOServo;
  };
  GPIO* getGPIO(uint8_t id);

  const uint8_t GPIOSERVO_COUNT = 2;
  class GPIOServo {
    public:
      // Angle in degrees from 0 to 180
      void setAngle(uint8_t angle);

      // Internal constructor
      GPIOServo(GPIO*);
    private:
      Servo servo;
  };
  GPIOServo* getGPIOServo(uint8_t id);

  // Internal API, do not use!
  namespace Internal {
    extern int team;
    extern RobotBase* robot;
    extern RobotState currentState;
    
    void enterState(RobotState state);
    int main();
  }
}

#endif
