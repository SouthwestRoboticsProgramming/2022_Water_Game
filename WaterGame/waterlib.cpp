#include "waterlib.h"

namespace WG {
  RobotState getCurrentState() {
    return WG::Internal::currentState;
  }

  RobotBase::RobotBase(int team) {
    WG::Internal::team = team;
  }

  // --- Controller ----------------------

  static Controller CONTROLLERS[CONTROLLER_COUNT] = {
    Controller(0), Controller(1), Controller(2), Controller(3)
  };

  Controller* getController(uint8_t id) {
    return &CONTROLLERS[id];
  }

  Controller::Controller(uint8_t id) {
    buttons = 0;
    for (int i = 0; i < AXIS_COUNT; i++) {
      axes[i] = 0.0f;
    }
  }

  bool Controller::getButton(uint8_t id) {
    return (buttons & (1 << id)) != 0;
  }

  float Controller::getAxis(uint8_t id) {
    return axes[id];
  }

  // --- Motor ---------------------------

  static Motor MOTORS[MOTOR_COUNT] = {
    Motor(0), Motor(1), Motor(2), Motor(3)
  };

  Motor* getMotor(uint8_t id) {
    return &MOTORS[id];
  }

  // TODO: Define actual pin numbers
  static const uint8_t MOTOR_PINS_ENABLE  [MOTOR_COUNT] = {1, 1, 1, 1}; // Must be PWM capable
  static const uint8_t MOTOR_PINS_POSITIVE[MOTOR_COUNT] = {1, 1, 1, 1};
  static const uint8_t MOTOR_PINS_NEGATIVE[MOTOR_COUNT] = {1, 1, 1, 1};

  Motor::Motor(uint8_t id) {
    enable   = MOTOR_PINS_ENABLE  [id];
    positive = MOTOR_PINS_POSITIVE[id];
    negative = MOTOR_PINS_NEGATIVE[id];

    pinMode(enable, OUTPUT);
    pinMode(positive, OUTPUT);
    pinMode(negative, OUTPUT);

    setDirection(MotorDirection::FORWARD);
    stop();
  }

  void Motor::setSpeed(uint8_t speed) {
    analogWrite(enable, speed);
  }

  void Motor::setDirection(MotorDirection direction) {
    switch (direction) {
      case MotorDirection::FORWARD:
        digitalWrite(positive, HIGH);
        digitalWrite(negative, LOW);
        break;
      case MotorDirection::REVERSE:
        digitalWrite(positive, LOW);
        digitalWrite(negative, HIGH);
        break;
    }
  }

  // --- GPIO ----------------------------

  static GPIO GPIOS[GPIO_COUNT] = {
    GPIO(0), GPIO(1), GPIO(2), GPIO(3),
    GPIO(4), GPIO(5), GPIO(6)
  };

  GPIO* getGPIO(uint8_t id) {
    return &GPIOS[id];
  }

  // TODO: Define actual pin numbers
  // Port 1 and 2 should pe PWM capable for servos
  static const uint8_t GPIO_PINS[GPIO_COUNT] = {1, 1, 1, 1, 1, 1, 1};

  GPIO::GPIO(uint8_t id) {
    pin = GPIO_PINS[id];
    currentPinMode = INPUT_PULLUP;
    pinMode(pin, INPUT_PULLUP);
  }

  bool GPIO::read() {
    setPinMode(INPUT_PULLUP);
    return digitalRead(pin);
  }

  void GPIO::write(bool value) {
    setPinMode(OUTPUT);
    digitalWrite(pin, value);
  }

  void GPIO::setPinMode(uint8_t mode) {
    if (currentPinMode != mode) {
      currentPinMode = mode;
      pinMode(pin, mode);
    }
  }

  // --- GPIOServo -----------------------

  static GPIOServo SERVOS[GPIOSERVO_COUNT] = {
    GPIOServo(&GPIOS[0]), GPIOServo(&GPIOS[1])
  };

  GPIOServo* getGPIOServo(uint8_t id) {
    return &SERVOS[id];
  }

  GPIOServo::GPIOServo(GPIO* gpio) {
    servo.attach(gpio->pin);
  }

  void GPIOServo::setAngle(uint8_t angle) {
    servo.write(angle);
  }

  namespace Internal {
    int team;
    RobotBase* robot;
    RobotState currentState;
    
    void enterState(RobotState state) {
      currentState = state;
      
      switch (state) {
        case RobotState::DISABLED:
          robot->disabledInit();
          break;
        case RobotState::TELEOP:
          robot->teleopInit();
          break;
      }
    }
    
    void setup() {
      Serial.begin(9600);
      
      robot = createRobot();
      robot->robotInit();
      enterState(RobotState::DISABLED);
    }

    void loop() {
      WG::Internal::readIncomingPackets();
      
      switch (currentState) {
        case RobotState::DISABLED:
          robot->disabledPeriodic();
          break;
        case RobotState::TELEOP:
          robot->teleopPeriodic();
          break;
      }
    }
  }
}

void setup() {
  WG::Internal::setup();
}

void loop() {
  WG::Internal::loop();
}
