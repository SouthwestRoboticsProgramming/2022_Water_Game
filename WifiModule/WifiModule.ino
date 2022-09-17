#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

static const char* WIFI_SSID = "WaterGame";
static const char* WIFI_PASS = "Password!";

WiFiServer server(80);

void setup() {  
  Serial.begin(115200);

  WiFi.setSleep(WIFI_PS_NONE);

  if (!WiFi.softAP(WIFI_SSID, WIFI_PASS)) {
    while (true) {}
  }
  
  IPAddress ip = WiFi.softAPIP();
 
  server.begin();
  server.setNoDelay(true);
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    while (client.connected()) {
      if (client.available() > 0) {
        Serial.write(client.read());
      }

      if (Serial.available() > 0) {
        client.write(Serial.read());
      }
    }

    client.stop();
  }
}
