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

Tempo t_temp(30*1000); // temporizador para la lectura de temperatura

ESP8266WebServer server(80);
IPAddress local_IP(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

String html_principal(){
String ret="<html><head>"
"    <title>Control Masonico</title>"
" <meta charset=\"utf-8\"> "
" <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
" <link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css\">"
"</head>"
""
"<body>"
"<div class=\"container-fluid\">"
" <div class=\"well\">"
"    <h1>Control Masonico</h1>"
" </div>"
" <div class=\"row\">"
"   <div class=\"col-md-6 \">"
"     <form action=\"/ferm1\" method=\"post\">"
"       <div class=\"form-group\">"
"           <label for=\"sarasa\">Temp heladera: <span class=\"label label-primary\"> ";
ret += tempsensada1;
ret+= "</span></label>"
"           <span class=\"label label-default\">";
ret += tempset1;
ret+= "</span>"
"           <input type=\"text\" class=\"form-control\" name=\"tempset\">"
"       </div>"
"       <div class=\"form-group\">"
"         <input type=\"submit\" class=\"btn btn-default\" value=\"Enviar\"> "
"       </div>"
"     </form>"
"   </div>"
" </div>"
"</div>"
"</body>"
"</html>";
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

void redirectHome()
{
  server.sendHeader("Location", "/",true); 
  server.send(302, "text/plain",""); 
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
    redirectHome();
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
  WiFi.softAPConfig(local_IP, gateway, subnet)
  delay(10);
  WiFi.softAP("Mason");
  delay(10);
  Serial.println("");

  server.on("/", handleRoot);
  server.on("/ferm1", setferm1);
  server.onNotFound(handleNotFound);
  server.begin();

  tempset1= EEPROM.read(ADDR1);

  getTemps();

  Serial.print(" Sensor1: ");
  Serial.print(tempsensada1);

  pinMode(RELAY1, OUTPUT);
  digitalWrite(RELAY1,HIGH);
}
void getTemps(){
  sensors.requestTemperatures();
  tempsensada1= sensors.getTempCByIndex(0);
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
