#include "TemperatureControlFunctions.h"
#include "Globals.h"
#include "ApiFunctions.h"

void getTemps(){
  sensors.requestTemperatures();
  tempsensada1= sensors.getTempCByIndex(0);
}

void control(){
  getTemps();
  apiPostData(tempsensada1);
  
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

