#ifndef INCLUDE_WATERLIB_H
#define INCLUDE_WATERLIB_H

#include <arduino.h>
#include <Servo.h>
#include "waternet.h"

namespace WG {
  enum RobotState : uint8_t {
    DISABLED,
    TELEOP
  };
  RobotState getCurrentState();
  
  class RobotBase {
    public:
      RobotBase(uint16_t team);

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
      void read(WG::Internal::PacketInControls*);
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
      uint8_t currentPinMode;
      void setPinMode(uint8_t);

    friend class GPIOServo;
  };
  GPIO* getGPIO(uint8_t id);

  const uint8_t GPIOSERVO_COUNT = 2;
  class GPIOServo {
    public:
      // Assigns a servo to a GPIO pin
      GPIOServo(GPIO*);
      
      // Angle in degrees from 0 to 180
      // Note: An angle at the far extremes may not be
      //       physically possible on your servo, causing
      //       its motor to stall.
      void setAngle(uint8_t angle);
    private:
      GPIO* gpio;
      Servo servo;
  };

  // Internal API, do not use!
  namespace Internal {
    extern uint16_t team;
    extern RobotBase* robot;
    extern RobotState currentState;
    
    void enterState(RobotState state);
    void setup();
    void loop();
  }
}

#endif
