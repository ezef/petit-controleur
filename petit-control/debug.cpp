#include "debug.h"

// Debug prints
void debugTemperatureSteps(){
  Serial.println("Temperature steps:");

  for (byte i = 0; i < 9;){
    Serial.print("Step ");
    Serial.print(i);
    Serial.print(": ");
    
    Serial.print(temperatureSteps[i]->temperature);
    Serial.print("º, for ");
    Serial.print(temperatureSteps[i]->hours);
    Serial.print("hours, starting at ");
    Serial.print(temperatureSteps[i]->starts_at);
    Serial.println("accumulated hours ");
  }
}