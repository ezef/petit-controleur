#include <Arduino.h>
#include "Globals.h"
#include "HTTPserver.h"
#include "HTML.h"
#include "TemperatureControlFunctions.h"

void initHTTPserver(){
  server.on("/", handleRoot);
  server.on("/simple_mode", handleSetSimpleModeSubmit);
  server.on("/stepped_mode", handleSteppedModeSubmit);

  server.on("/bootstrap.min.css", bootstrap);
  server.on("bootstrap.min.css", bootstrap);

  server.onNotFound(handleNotFound);
  server.begin();
}

void redirectHome(){
  server.send(200, "text/html", html_principal()); 
}

void handleSetSimpleModeSubmit(){
  if (server.method() != HTTP_POST) {
    server.send(405, "text/plain", "Method Not Allowed");
  } else {
    if ( server.arg("tempset") != ""){
      startSimpleModeTemperatureControl(server.arg("tempset").toInt());
      redirectHome();
    }else{
      returnFail("Temperatura seteada Vacia");
    }
  }
}

void handleSteppedModeSubmit(){

  // TODO validate all of this first, do not store empty values
  TempStep submittedTempSteps[10];
  submittedTempSteps[0] = {(byte) server.arg('step_1_temperature').toInt(), (byte) server.arg('step_1_hours').toInt() }; // step_1_temperature should be the input field name in the
  submittedTempSteps[1] = {(byte) server.arg('step_2_temperature').toInt(), (byte) server.arg('step_2_hours').toInt() };
  submittedTempSteps[2] = {(byte) server.arg('step_3_temperature').toInt(), (byte) server.arg('step_3_hours').toInt() };
  submittedTempSteps[3] = {(byte) server.arg('step_4_temperature').toInt(), (byte) server.arg('step_4_hours').toInt() };
  submittedTempSteps[4] = {(byte) server.arg('step_5_temperature').toInt(), (byte) server.arg('step_5_hours').toInt() };
  submittedTempSteps[5] = {(byte) server.arg('step_6_temperature').toInt(), (byte) server.arg('step_6_hours').toInt() };
  submittedTempSteps[6] = {(byte) server.arg('step_7_temperature').toInt(), (byte) server.arg('step_7_hours').toInt() };
  submittedTempSteps[7] = {(byte) server.arg('step_8_temperature').toInt(), (byte) server.arg('step_8_hours').toInt() };
  submittedTempSteps[8] = {(byte) server.arg('step_9_temperature').toInt(), (byte) server.arg('step_9_hours').toInt() };
  submittedTempSteps[9] = {(byte) server.arg('step_10_temperature').toInt(), (byte) server.arg('step_10_hours').toInt() };

  startSteppedModeTemperatureControl(submittedTempSteps);
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