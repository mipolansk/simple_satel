/**
 * It's a simple example, where the library is used to check violated zones.
 */

#include <SimpleSatelLib.h>
#include <WiFi.h>

#define WIFI_SSID "<set your WIFI SSID>"
#define WIFI_PWD "<set your WIFI password>"

#define SATEL_HOST "<set ip of your ETHM-1 module>"
#define SATEL_PORT 7094 // Change if needed, default port is used

WiFiClient client;
bool sent = false;

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PWD);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("WiFI connecting");
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(1000);
    }
    Serial.println(".");
  }

  if (!SimpleSatel.connected() && !SimpleSatel.connect(SATEL_HOST, SATEL_PORT)) {
    Serial.println("Could not connect with ETHM module...");
    return;
  }

  Result<SSatel::ZonesViolationAnswer> zonesState = SimpleSatel.readZonesViolation();
  if (zonesState.isSuccess()) {
    if (zonesState.getData().isZoneViolated(3)) {
      Serial.println("Zone No 3 is vialoted!");
    }
    print(zonesState.getData());
  }
  else {
    Serial.println("No response");
  }

  delay(3000);
}

void print(SSatel::Frame frame) {
  byte bytes[23];
  frame.toBytes(bytes);

  for (int i = 0; i < 23; i++) {
    Serial.print(bytes[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}