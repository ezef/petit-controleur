#include "Globals.h"

byte relay1 = 0;
float tempsensada1;
byte temperatureControlMode;
byte currentTemperatureStep;
byte tempset1;
int hoursPassedSinceSteppedControlModeStarted;

const char* ssid = "THESSID";
const char* password = "THEPASSWORD";

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

Tempo t_temp(30*1000); // temporizador para la lectura de temperatura
Tempo tempo_step_change(3600*1000); // see if it's time to evaluate step change and store accumulated hours
Tempo tempo_wifi_retry(60*1000); // resilient connection retry to WIFI each minute
ESP8266WebServer server(80);