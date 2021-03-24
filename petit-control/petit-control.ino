#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <tempo.h>
#include <FS.h>

#include "StepTemperatureFunctions.h"
#include "HTML.h"
#include "TemperatureControlFunctions.h"
#include "Globals.h"
#include "ApiFunctions.h"

void handleRoot()
{
  if (server.hasArg("password")) {
    handleSubmit();
  }
  else {
    server.send(200, "text/html", html_login());
  }
}

void returnFail(String msg)
{
  server.sendHeader("Connection", "close");
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(500, "text/plain", msg + "\r\n");
}

void redirectHome()
{
  server.send(200, "text/html", html_principal()); 
}

void handleSubmit()
{
  String pass;

  if (!server.hasArg("password")) return returnFail("BAD ARGS");
  pass = server.arg("password");
  if (pass == "angela") {
    server.send(200, "text/html", html_principal());
  }  else {
    returnFail("shupala");
  }
}

void returnOK()
{
  server.sendHeader("Connection", "close");
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", "OK\r\n");
}

void handleNotFound()
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}
void setferm1(){
  if ( server.arg("tempset") != ""){
    Serial.print(" Seteo de Ferm 1: ");
    Serial.println(server.arg("tempset"));
    tempset1= (byte)server.arg("tempset").toInt();
    EEPROM.put(ADDR1,tempset1);
    EEPROM.commit();
    control();
    redirectHome();
  }else{
    returnFail("Temperatura seteada Vacia");
  }  
}


void initWIFI()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,password);
}

void setup(void)
{
  Serial.begin(115200); delay(10);
  EEPROM.begin(512); delay(10);
  sensors.begin(); delay(10);
  
  initWIFI(); delay(10);
  
  server.on("/", handleRoot);
  server.on("/ferm1", setferm1);
  server.on("/bootstrap.min.css", bootstrap);
  server.on("bootstrap.min.css", bootstrap);
  server.onNotFound(handleNotFound);
  server.begin();

  SPIFFS.begin(); 

  tempset1 = EEPROM.read(ADDR1);

  getTemps();

  Serial.print(" Sensor1: ");
  Serial.print(tempsensada1);

  pinMode(RELAY1, OUTPUT);
  digitalWrite(RELAY1,HIGH);
}


void loop(void)
{
  // Resilient connection to WIFI
  if (WiFi.status() != WL_CONNECTED && tempo_wifi_retry.state()){
    initWIFI();
  }
  server.handleClient();
  
  if (t_temp.state()){ // realiza la lectura de la temperatura, actualiza el lcd y comanda los relays.
    control();
  }  
}
