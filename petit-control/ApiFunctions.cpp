#include "ApiFunctions.h"

void apiPostData(){
  if(WiFi.status()== WL_CONNECTED){
    
    Serial.println("API posting data: ");
    Serial.print("    Temp sensada:");
    Serial.println(tempsensada1);
    Serial.print("    Temp Ambiente:");
    Serial.println(roomTemperature);
    Serial.print("    Temp seteada:");
    Serial.println(tempset1);
    
    HTTPClient http;
    
    String serverPath = "http://home.ezefsoftware.com.ar:8100/api/temperature?"; // 167.71.59.68
    serverPath += "temperature_read=";
    serverPath += tempsensada1;
    serverPath += "&temperature_set=";
    serverPath += tempset1;
    serverPath += "&room_temperature=";
    serverPath += roomTemperature;
    serverPath += "&cold_actuator=";
    serverPath += relay1 ? "on" : "off";
    serverPath += "&heat_actuator=";
    serverPath += relay_heat_working ? "on" : "off";
    
    Serial.println(serverPath);

    http.begin(serverPath.c_str());
    // http.addHeader("Host", "home.ezefsoftware.com.ar");

    int httpResponseCode = http.POST("");
    
    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      //String payload = http.getString();
      //Serial.println(payload);
    } else {
      Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpResponseCode).c_str());
    }
    
    http.end();
  } else {
    Serial.println("WIFI not connected");
  }
}