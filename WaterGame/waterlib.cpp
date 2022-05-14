#include "waterlib.h"

namespace WG {
  RobotState getCurrentState() {
    return WG::Internal::currentState;
  }

  RobotBase::RobotBase(uint16_t team) {
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

  void Controller::read(WG::Internal::PacketInControls* packet) {
    buttons = packet->buttonMask;
    for (int i = 0; i < AXIS_COUNT; i++) {
      axes[i] = packet->axes[i] / 127.0f;
    }
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
    en       = MOTOR_PINS_ENABLE  [id];
    positive = MOTOR_PINS_POSITIVE[id];
    negative = MOTOR_PINS_NEGATIVE[id];

    pinMode(en, OUTPUT);
    pinMode(positive, OUTPUT);
    pinMode(negative, OUTPUT);

    setDirection(MotorDirection::FORWARD);
    stop();
    enabled = isRobotEnabled();
  }

  void Motor::setSpeed(uint8_t speed) {
    if (enabled) {
      analogWrite(en, speed);
    }
  }

  void Motor::setDirection(MotorDirection direction) {
    switch (direction) {
      case MotorDirection::FORWARD:
        digitalWrite(negative, LOW);
        digitalWrite(positive, HIGH);
        break;
      case MotorDirection::REVERSE:
        digitalWrite(positive, LOW);
        break;
    }
  }

  void Motor::enable() {
    enabled = true;
  }

  void Motor::disable() {
    enabled = false;
    stop();
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
  static const uint8_t GPIO_PINS[GPIO_COUNT] = {4, 5, 1, 1, 1, 1, 1};

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

  static GPIOServo* SERVOS[2];

  GPIOServo::GPIOServo(GPIO* g) {
    pin = g->pin;
    enabled = isRobotEnabled();
    if (enabled) {
      servo.attach(pin);
    }

    if (SERVOS[0] == nullptr) {
      SERVOS[0] = this;
    } else {
      SERVOS[1] = this;
    }
  }

  void GPIOServo::setAngle(uint8_t angle) {
    if (enabled) {
      servo.write(angle);
    }
  }

  void GPIOServo::enable() {
    if (!enabled) {
      servo.attach(pin);
    }
    enabled = true;
  }

  void GPIOServo::disable() {
    if (enabled) {
      servo.detach();
    }
    enabled = false;
  }

  namespace Internal {
    static const uint8_t RSL_PIN = 11;
    
    uint16_t team;
    RobotBase* robot;
    RobotState currentState;
    
    void enterState(RobotState state) {
      currentState = state;
      
      switch (state) {
        case RobotState::DISABLED:
          disablePeripherals();
          robot->disabledInit();
          break;
        case RobotState::TELEOP:
          enablePeripherals();
          robot->teleopInit();
          break;
      }
    }

    void enablePeripherals() {
      for (int i = 0; i < MOTOR_COUNT; i++) {
        MOTORS[i].enable();
      }

      for (int i = 0; i < GPIOSERVO_COUNT; i++) {
        if (SERVOS[i])
          SERVOS[i]->enable();
      }
    }

    void disablePeripherals() {
      for (int i = 0; i < MOTOR_COUNT; i++) {
        MOTORS[i].disable();
      }

      for (int i = 0; i < GPIOSERVO_COUNT; i++) {
        if (SERVOS[i])
          SERVOS[i]->disable();
      }
    }
    
    void setup() {
      Serial.begin(9600);
      pinMode(RSL_PIN, OUTPUT);

      SERVOS[0] = nullptr;
      SERVOS[1] = nullptr;
      
      robot = createRobot();
      robot->robotInit();
      enterState(RobotState::DISABLED);
    }

    void loop() {
      WG::Internal::readIncomingPackets();
      robot->robotPeriodic();

      switch (currentState) {
        case RobotState::DISABLED:
          digitalWrite(RSL_PIN, HIGH);
          robot->disabledPeriodic();
          break;
        case RobotState::TELEOP:
          digitalWrite(RSL_PIN, millis() % 500 < 250);
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
