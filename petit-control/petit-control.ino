
#include "HTML.h"
#include "TemperatureControlFunctions.h"
#include "Globals.h"
#include "ApiFunctions.h"
#include "HTTPserver.h"


void initWIFI() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,password);
}

void setup(void) {
  Serial.begin(115200); delay(10);
  EEPROM.begin(512); delay(10);
  sensors.begin(); delay(10);
  
  initWIFI(); delay(10);  
  initHTTPserver(); delay(10);
  SPIFFS.begin(); delay(10);

  tempset1 = EEPROM.read(ADDR1);
  getTemps();

  temperatureControlMode = EEPROM.read(EEPROM_ADDR_ACTIVE_MODE);
  if (temperatureControlMode == STEPPED_MODE){
     hoursPassedSinceSteppedControlModeStarted = readElapsedHoursFromEEPROM();
     currentTemperatureStep = readCurrentTemperatureStepFromEEPROM();
  }
  loadSteppedTemperaturesFromEEPROM();

  Serial.print(" Sensor1: ");
  Serial.print(tempsensada1);

  pinMode(RELAY1, OUTPUT);
  digitalWrite(RELAY1,HIGH);
}

void loop(void) {
  // Resilient connection to WIFI
  if (WiFi.status() != WL_CONNECTED && tempo_wifi_retry.state()){
    initWIFI();
  }

  server.handleClient();

  if (tempo_step_change.state() && temperatureControlMode == STEPPED_MODE ){
    updateElapsedHours();
    evaluateTemperatureChange();
  } 

  if (t_temp.state()){ 
    control();
  }  
}
