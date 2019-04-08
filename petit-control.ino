/*
 * Demonstrate using an http server and an HTML form to control an LED.
 * The http server runs on the ESP8266.
 *
 * Connect to "http://esp8266WebForm.local" or "http://<IP address>"
 * to bring up an HTML form to control the LED connected GPIO#0. This works
 * for the Adafruit ESP8266 HUZZAH but the LED may be on a different pin on
 * other breakout boards.
 *
 * Imperatives to turn the LED on/off using a non-browser http client.
 * For example, using wget.
 * $ wget http://esp8266webform.local/ledon
 * $ wget http://esp8266webform.local/ledoff
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <tempo.h>
#include "constantes.h"

#define ADDR1 0 //direccion de la EEPROM para la tempSet1

#define RELAY1 5 

#define ONE_WIRE_BUS 14
#define TEMPERATURE_PRECISION 9
#define HISTERESIS 0.3

byte tempset1;
byte relay1 = 0;

float tempsensada1;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress direccionsensor1 = { 0x28, 0xEE, 0xE3, 0xF, 0x15, 0x16, 0x2, 0xAF };

Tempo t_temp(30*1000); // temporizador para la lectura de temperatura
unsigned long ultimoTiempo;

ESP8266WebServer server(80);
WiFiClientSecure client;

String html_principal(){
String ret="pepe";
return ret;
}

void handleRoot()
{
    server.send(200, "text/html", html_principal());
}

void returnFail(String msg)
{
  server.sendHeader("Connection", "close");
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(500, "text/plain", msg + "\r\n");
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
    returnOK();
  }else{
    returnFail("Temperatura seteada Vacia");
  }  
}
void setup(void)
{
  Serial.begin(115200);
  delay(10);
  EEPROM.begin(512);
  delay(10);
  sensors.begin();
  delay(10);
  delay(10);
  Serial.println("");
  ultimoTiempo=millis();

  // Wait for connection

  server.on("/", handleRoot);
  server.on("/ferm1", setferm1);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.print("Connect to http://");

  tempset1= EEPROM.read(ADDR1);

  getTemps();

  Serial.print(" Sensor1: ");
  Serial.print(tempsensada1);

  pinMode(RELAY1, OUTPUT);
  digitalWrite(RELAY1,HIGH);
}
void getTemps(){
  sensors.requestTemperatures();
  tempsensada1= sensors.getTempC(direccionsensor1);
}

void control(){
  getTemps();
  
  if (tempsensada1 > tempset1 + HISTERESIS){
    digitalWrite(RELAY1, LOW);
    Serial.print("Relay 1 ON - T1:");
    Serial.println(tempsensada1);
    relay1=true;
  }else{
    if (tempsensada1 < tempset1 - HISTERESIS){
      digitalWrite(RELAY1,HIGH);
      Serial.print("Relay 1 OFF- T1:");
      Serial.println(tempsensada1);
      relay1=false;
    }
  } 
}

void loop(void)
{
  server.handleClient();
  if (t_temp.state()){ // realiza la lectura de la temperatura, actualiza el lcd y comanda los relays.
    control();
  }
  
}
