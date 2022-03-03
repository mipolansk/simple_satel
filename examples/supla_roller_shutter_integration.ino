/**
 * In this example addition library SuplaRest was used, to make an integration between Satel alarm
 * system and Supla Smart Home automation. In this integration you can control your Supla roller shutters
 * from Satel Integra product
 */
#include <SimpleSatelLib.h>
#include <SuplaRestLib.h>

#define WIFI_SSID "<set your WIFI SSID>"
#define WIFI_PWD "<set your WIFI password>"

#define SATEL_HOST "<set ip of your ETHM-1 module>"
#define SATEL_PORT 7094 // Change if needed, default port is used

#define SUPLA_USER "<set your Supla user>"
#define SUPLA_PWD "<set your Supla password>"

// In your Integra you need to define the outputs as MONO.
// You need two outputs for each roller shutter - one to trigger openning, another one for closing.
// Defining outputs use first one for opening and second one for clossing the roller.
// In this example I'll use outputs no 29, 30, 31 and 32
#define FIRST_OUTPUT_NO 29
bool processing[4] = {0};

// Each roller is double to make the processing easier
int rollers[4] = { 
  1111, 1111,
  2222, 2222
};

void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PWD);
  SuplaRest.begin(SUPLA_USER, SUPLA_PWD);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("WiFI connecting");
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(1000);
    }
    delay(1000);
    Serial.println(".");
  }

  if (!SimpleSatel.connected() && !SimpleSatel.connect(SATEL_HOST, SATEL_PORT)) {
    Serial.println("Could not connect with ETHM module...");
    return;
  }

  Result<SSatel::OutputsStateAnswer> outputsState = SimpleSatel.readOutputsState();
  if (outputsState.isSuccess()) {
    for (int i=0; i<16; i++) {
      int output = i + FIRST_OUTPUT_NO;

      if (outputsState.getData().isOutputActive(output) && !processing[i]) {
        Serial.print("Processing: "); Serial.println(output);
        openCloseRollerShutter(i);
        processing[i] = true;
      } else if (!outputsState.getData().isOutputActive(output) && processing[i]) {
        Serial.print("Cleanning: "); Serial.println(output);
        processing[i] = false;
      }
    }
  }

  SuplaRest.iterate();
  delay(500);
}

void openCloseRollerShutter(int idx) {
  if (idx % 2 == 0) {
    Serial.println("Opening");
    SuplaRest.openRollerShutter(rollers[idx]);
  } else {
    Serial.println("Cloasing");
    SuplaRest.closeRollerShutter(rollers[idx]);
  }
}
