#include "ApiFunctions.h"

void apiPostData(int temperature){
  if(WiFi.status()== WL_CONNECTED){
    
    Serial.println("API posting data: ");
    Serial.print("    Temp sensada:");
    Serial.println(temperature);
    Serial.print("    Temp seteada:");
    Serial.println(tempset1);
    
    HTTPClient http;
    
    String serverPath = "http://control.ezefsoftware.com.ar/?"; // 167.71.59.68
    serverPath += "tempsens=";
    serverPath += temperature;
    serverPath += "&tempset=";
    serverPath += tempset1;
    
    Serial.println(serverPath);

    http.begin(serverPath.c_str());
    http.addHeader("Host", "control.ezefsoftware.com.ar");

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