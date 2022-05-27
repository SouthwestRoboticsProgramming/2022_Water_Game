#define SERIAL_USB Serial
#define SERIAL_PIN Serial1

static const long BAUD = 115200;
static const int LED_PIN = 11;

long ledOnTimestamp = 0;

void setup() {
  SERIAL_USB.begin(BAUD);
  SERIAL_PIN.begin(BAUD);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  if (SERIAL_USB.available()) {
    SERIAL_PIN.write(SERIAL_USB.read());
    ledOnTimestamp = millis();
  }
  if (SERIAL_PIN.available()) {
    SERIAL_USB.write(SERIAL_PIN.read());
    ledOnTimestamp = millis();
  }

  // Flash the LED to indicate activity
  digitalWrite(LED_PIN, millis() - ledOnTimestamp < 20);
}
