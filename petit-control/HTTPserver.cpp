#include <Arduino.h>
#include "Globals.h"
#include "HTTPserver.h"
#include "HTML.h"
#include "TemperatureControlFunctions.h"

void initHTTPserver(){
  server.on("/", handleRoot);
  server.on("/simple_mode", handleSetSimpleModeSubmit);
  server.on("/save_stepped_mode", handleSteppedModeSubmit);
  server.on("/init-stepped-mode", handleInitSteppedMode);

  server.on("/bootstrap.min.css", bootstrap);
  server.on("bootstrap.min.css", bootstrap);

  server.onNotFound(handleNotFound);
  server.begin();
}

void redirectHome(){
  // server.send(200, "text/html", html_control()); 

  server.sendHeader("Location", String("/"), true);
  server.send ( 302, "text/plain", "");
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

void handleInitSteppedMode(){
  startSteppedModeTemperatureControl();
  redirectHome();
}

void handleSteppedModeSubmit(){

  int i = 0;
  String temp_input_names[] = {
    "step_1_temperature",
    "step_2_temperature",
    "step_3_temperature",
    "step_4_temperature",
    "step_5_temperature",
    "step_6_temperature",
    "step_7_temperature",
    "step_8_temperature",
    "step_9_temperature",
    "step_10_temperature"
  };

  String times_input_names[] = {
    "step_1_hours",
    "step_2_hours",
    "step_3_hours",
    "step_4_hours",
    "step_5_hours",
    "step_6_hours",
    "step_7_hours",
    "step_8_hours",
    "step_9_hours",
    "step_10_hours"
  };
  TempStep submittedTempSteps[10];

  for (int j=0; j <= 9; j++){
    if(server.arg(temp_input_names[j]).toInt() > 0 && server.arg(times_input_names[j]).toInt() > 0){
      submittedTempSteps[i] = {(byte) server.arg(temp_input_names[j]).toInt(), (byte) server.arg(times_input_names[j]).toInt() }; // step_1_temperature should be the input field name in the
      i++;
    }
  }

  saveSteppedModeTemperatureControl(submittedTempSteps);
}

/*
void handleSubmit(){
  String pass;

  if (!server.hasArg("password")) return returnFail("BAD ARGS");
  pass = server.arg("password");
  if (pass == "angela") {
    server.send(200, "text/html", html_control());
  }  else {
    returnFail("shupala");
  }
}
*/

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
  server.send(200, "text/html", html_control());
  /*
  if (server.hasArg("password")) {
    handleSubmit();
  }
  else {
    server.send(200, "text/html", html_login());
  }
  */
}