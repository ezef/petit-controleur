#include <Arduino.h>
#include "Globals.h"
#include "HTTPserver.h"
#include "HTML.h"
#include "TemperatureControlFunctions.h"

void initHTTPserver(){
  server.on("/", handleRoot);
  server.on("/ferm1", handleSetFermSubmit);
  server.on("/bootstrap.min.css", bootstrap);
  server.on("bootstrap.min.css", bootstrap);
  server.onNotFound(handleNotFound);
  server.begin();
}
void redirectHome(){
  server.send(200, "text/html", html_principal()); 
}

void handleSetFermSubmit(){
  if ( server.arg("tempset") != ""){
    Serial.print(" Seteo de Ferm 1: ");
    Serial.println(server.arg("tempset"));
    
    setferm1(server.arg("tempset").toInt());
    redirectHome();
  }else{
    returnFail("Temperatura seteada Vacia");
  }
}

void handleSubmit(){
  String pass;

  if (!server.hasArg("password")) return returnFail("BAD ARGS");
  pass = server.arg("password");
  if (pass == "angela") {
    server.send(200, "text/html", html_principal());
  }  else {
    returnFail("shupala");
  }
}

void returnOK(){
  server.sendHeader("Connection", "close");
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", "OK\r\n");
}

void handleNotFound(){
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

void returnFail(String msg){
  server.sendHeader("Connection", "close");
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(500, "text/plain", msg + "\r\n");
}

void handleRoot(){
  if (server.hasArg("password")) {
    handleSubmit();
  }
  else {
    server.send(200, "text/html", html_login());
  }
}