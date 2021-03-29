#include "TemperatureControlFunctions.h"

TempStep * temperatureSteps[10];

void getTemps(){
  sensors.requestTemperatures();
  tempsensada1= sensors.getTempCByIndex(0);
}

void control(){
  getTemps();
  apiPostData(tempsensada1);
  
  if (tempsensada1 > tempset1 + HISTERESIS) {
    digitalWrite(RELAY1, LOW);
    Serial.print("Relay 1 ON - T1:");
    Serial.println(tempsensada1);
    relay1=true;
  } else {
    if (tempsensada1 < tempset1 - HISTERESIS) {
      digitalWrite(RELAY1,HIGH);
      Serial.print("Relay 1 OFF- T1:");
      Serial.println(tempsensada1);
      relay1=false;
    }
  } 
}

void setFerm1(int new_temp){
  Serial.print(" Seteo de Ferm 1: ");
  Serial.println(server.arg("tempset"));

  tempset1 = (byte) new_temp;
  EEPROM.put(ADDR1,tempset1);
  EEPROM.commit();
  control();
}

void startSimpleModeTemperatureControl(int new_temperature){
  setTemperatureControlMode(SIMPLE_MODE);
  setFerm1(new_temperature);
}

void saveSteppedModeTemperatureControl(TempStep submittedTempSteps[10]) {

  // Calculate accumulated hours for each temp step
  submittedTempSteps[0].starts_at = 0;
  submittedTempSteps[1].starts_at = submittedTempSteps[0].starts_at + submittedTempSteps[1].hours;
  submittedTempSteps[2].starts_at = submittedTempSteps[1].starts_at + submittedTempSteps[2].hours;
  submittedTempSteps[3].starts_at = submittedTempSteps[2].starts_at + submittedTempSteps[3].hours;
  submittedTempSteps[4].starts_at = submittedTempSteps[3].starts_at + submittedTempSteps[4].hours;
  submittedTempSteps[5].starts_at = submittedTempSteps[4].starts_at + submittedTempSteps[5].hours;
  submittedTempSteps[6].starts_at = submittedTempSteps[5].starts_at + submittedTempSteps[6].hours;
  submittedTempSteps[7].starts_at = submittedTempSteps[6].starts_at + submittedTempSteps[7].hours;
  submittedTempSteps[8].starts_at = submittedTempSteps[7].starts_at + submittedTempSteps[8].hours;
  submittedTempSteps[9].starts_at = submittedTempSteps[8].starts_at + submittedTempSteps[9].hours;

  // Store values on EEPROM
  EEPROM.put(EEPROM_ADDR_TEMP1_VALUE,submittedTempSteps[0].temperature);
  EEPROM.put(EEPROM_ADDR_TEMP1_HOURS,submittedTempSteps[0].hours);
  EEPROM.put(EEPROM_ADDR_TEMP1_ACCUMULATED_HOURS,submittedTempSteps[0].starts_at);

  EEPROM.put(EEPROM_ADDR_TEMP2_VALUE,submittedTempSteps[1].temperature);
  EEPROM.put(EEPROM_ADDR_TEMP2_HOURS,submittedTempSteps[1].hours);
  EEPROM.put(EEPROM_ADDR_TEMP2_ACCUMULATED_HOURS,submittedTempSteps[1].starts_at);

  EEPROM.put(EEPROM_ADDR_TEMP3_VALUE,submittedTempSteps[2].temperature);
  EEPROM.put(EEPROM_ADDR_TEMP3_HOURS,submittedTempSteps[2].hours);
  EEPROM.put(EEPROM_ADDR_TEMP3_ACCUMULATED_HOURS,submittedTempSteps[2].starts_at);

  EEPROM.put(EEPROM_ADDR_TEMP4_VALUE,submittedTempSteps[3].temperature);
  EEPROM.put(EEPROM_ADDR_TEMP4_HOURS,submittedTempSteps[3].hours);
  EEPROM.put(EEPROM_ADDR_TEMP4_ACCUMULATED_HOURS,submittedTempSteps[3].starts_at);

  EEPROM.put(EEPROM_ADDR_TEMP5_VALUE,submittedTempSteps[4].temperature);
  EEPROM.put(EEPROM_ADDR_TEMP5_HOURS,submittedTempSteps[4].hours);
  EEPROM.put(EEPROM_ADDR_TEMP5_ACCUMULATED_HOURS,submittedTempSteps[4].starts_at);

  EEPROM.put(EEPROM_ADDR_TEMP6_VALUE,submittedTempSteps[5].temperature);
  EEPROM.put(EEPROM_ADDR_TEMP6_HOURS,submittedTempSteps[5].hours);
  EEPROM.put(EEPROM_ADDR_TEMP6_ACCUMULATED_HOURS,submittedTempSteps[5].starts_at);

  EEPROM.put(EEPROM_ADDR_TEMP7_VALUE,submittedTempSteps[6].temperature);
  EEPROM.put(EEPROM_ADDR_TEMP7_HOURS,submittedTempSteps[6].hours);
  EEPROM.put(EEPROM_ADDR_TEMP7_ACCUMULATED_HOURS,submittedTempSteps[6].starts_at);

  EEPROM.put(EEPROM_ADDR_TEMP8_VALUE,submittedTempSteps[7].temperature);
  EEPROM.put(EEPROM_ADDR_TEMP8_HOURS,submittedTempSteps[7].hours);
  EEPROM.put(EEPROM_ADDR_TEMP8_ACCUMULATED_HOURS,submittedTempSteps[7].starts_at);

  EEPROM.put(EEPROM_ADDR_TEMP9_VALUE,submittedTempSteps[8].temperature);
  EEPROM.put(EEPROM_ADDR_TEMP9_HOURS,submittedTempSteps[8].hours);
  EEPROM.put(EEPROM_ADDR_TEMP9_ACCUMULATED_HOURS,submittedTempSteps[8].starts_at);

  EEPROM.put(EEPROM_ADDR_TEMP10_VALUE,submittedTempSteps[9].temperature);
  EEPROM.put(EEPROM_ADDR_TEMP10_HOURS,submittedTempSteps[9].hours);
  EEPROM.put(EEPROM_ADDR_TEMP10_ACCUMULATED_HOURS,submittedTempSteps[9].starts_at);
  EEPROM.commit();

  // Set values on global variable (point global to submittedTempSteps)
  *temperatureSteps = submittedTempSteps;
}

void startSteppedModeTemperatureControl(){
  // change control mode
  setTemperatureControlMode(STEPPED_MODE);

  // set hour counter to 0
  hoursPassedSinceSteppedControlModeStarted = 0;

  // change setted temp based on steps
  evaluateTemperatureChange();
}

void setTemperatureControlMode(int mode){
  if (mode == SIMPLE_MODE){
    EEPROM.put(EEPROM_ADDR_ACTIVE_MODE, SIMPLE_MODE);
    temperatureControlMode = SIMPLE_MODE;
  } else if (mode == STEPPED_MODE) {
    EEPROM.put(EEPROM_ADDR_ACTIVE_MODE, STEPPED_MODE);
    temperatureControlMode = STEPPED_MODE;
  }
  EEPROM.commit();
}

void evaluateTemperatureChange(){
  // Determine if we need to change the temperature based on the stored steps

  if (hoursPassedSinceSteppedControlModeStarted == 0){
    // it has just started, use first temp
    tempset1 = (int) temperatureSteps[0]->temperature;
    control();
  } else {
    // Determine in which step we are
    for (byte i = 0; i < 9; ){
      // if the accumulated time is greather that the calulated time for this step but we have the previous step temperature activated. move to the next step
      // TODO add check for temperature = 0, wich means do not change temperatures.
      if (hoursPassedSinceSteppedControlModeStarted >= temperatureSteps[i+1]->starts_at && tempset1 == temperatureSteps[i]->temperature){
        setFerm1((int) temperatureSteps[i+1]->temperature);
        break;
      }
    }
  }
}

void updateElapsedHours(){
  // I can't store such a big number in the EEPROM with only one byte, so I store the calculated days and hours
  hoursPassedSinceSteppedControlModeStarted++;
  int days_passed = hoursPassedSinceSteppedControlModeStarted / 24;
  int hours_passed = hoursPassedSinceSteppedControlModeStarted - (days_passed * 24);

  EEPROM.put(EEPROM_ADDR_DAYS_PASSED, (byte) days_passed);
  EEPROM.put(EEPROM_ADDR_HOURS_PASSED, (byte) hours_passed);
  EEPROM.commit();
}

int readElapsedHoursFromEEPROM(){
  byte hours_passed = EEPROM.read(EEPROM_ADDR_HOURS_PASSED);
  byte days_passed = EEPROM.read(EEPROM_ADDR_DAYS_PASSED);

  return (days_passed * 24) + hours_passed;
}

