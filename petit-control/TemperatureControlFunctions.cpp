#include "TemperatureControlFunctions.h"

TempStep temperatureSteps[10] = {
  {0,0,0},
  {0,0,0},
  {0,0,0},
  {0,0,0},
  {0,0,0},
  {0,0,0},
  {0,0,0},
  {0,0,0},
  {0,0,0},
  {0,0,0}
};


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

   for (byte i = 0; i < 10; i++){
     temperatureSteps[i] = {submittedTempSteps[i].temperature, submittedTempSteps[i].hours, submittedTempSteps[i].starts_at};
   }

  // Calculate accumulated hours for each temp step
  temperatureSteps[0].starts_at = 0;
  temperatureSteps[1].starts_at = temperatureSteps[0].starts_at + temperatureSteps[0].hours;
  temperatureSteps[2].starts_at = temperatureSteps[1].starts_at + temperatureSteps[1].hours;
  temperatureSteps[3].starts_at = temperatureSteps[2].starts_at + temperatureSteps[2].hours;
  temperatureSteps[4].starts_at = temperatureSteps[3].starts_at + temperatureSteps[3].hours;
  temperatureSteps[5].starts_at = temperatureSteps[4].starts_at + temperatureSteps[4].hours;
  temperatureSteps[6].starts_at = temperatureSteps[5].starts_at + temperatureSteps[5].hours;
  temperatureSteps[7].starts_at = temperatureSteps[6].starts_at + temperatureSteps[6].hours;
  temperatureSteps[8].starts_at = temperatureSteps[7].starts_at + temperatureSteps[7].hours;
  temperatureSteps[9].starts_at = temperatureSteps[8].starts_at + temperatureSteps[8].hours;

  // Store values on EEPROM
  EEPROM.put(EEPROM_ADDR_TEMP1_VALUE,temperatureSteps[0].temperature);
  EEPROM.put(EEPROM_ADDR_TEMP1_HOURS,temperatureSteps[0].hours);

  EEPROM.put(EEPROM_ADDR_TEMP2_VALUE,temperatureSteps[1].temperature);
  EEPROM.put(EEPROM_ADDR_TEMP2_HOURS,temperatureSteps[1].hours);

  EEPROM.put(EEPROM_ADDR_TEMP3_VALUE,temperatureSteps[2].temperature);
  EEPROM.put(EEPROM_ADDR_TEMP3_HOURS,temperatureSteps[2].hours);

  EEPROM.put(EEPROM_ADDR_TEMP4_VALUE,temperatureSteps[3].temperature);
  EEPROM.put(EEPROM_ADDR_TEMP4_HOURS,temperatureSteps[3].hours);

  EEPROM.put(EEPROM_ADDR_TEMP5_VALUE,temperatureSteps[4].temperature);
  EEPROM.put(EEPROM_ADDR_TEMP5_HOURS,temperatureSteps[4].hours);

  EEPROM.put(EEPROM_ADDR_TEMP6_VALUE,temperatureSteps[5].temperature);
  EEPROM.put(EEPROM_ADDR_TEMP6_HOURS,temperatureSteps[5].hours);

  EEPROM.put(EEPROM_ADDR_TEMP7_VALUE,temperatureSteps[6].temperature);
  EEPROM.put(EEPROM_ADDR_TEMP7_HOURS,temperatureSteps[6].hours);

  EEPROM.put(EEPROM_ADDR_TEMP8_VALUE,temperatureSteps[7].temperature);
  EEPROM.put(EEPROM_ADDR_TEMP8_HOURS,temperatureSteps[7].hours);

  EEPROM.put(EEPROM_ADDR_TEMP9_VALUE,temperatureSteps[8].temperature);
  EEPROM.put(EEPROM_ADDR_TEMP9_HOURS,temperatureSteps[8].hours);

  EEPROM.put(EEPROM_ADDR_TEMP10_VALUE,temperatureSteps[9].temperature);
  EEPROM.put(EEPROM_ADDR_TEMP10_HOURS,temperatureSteps[9].hours);
  EEPROM.commit();
  debugTemperatureSteps();

  if ( temperatureControlMode == STEPPED_MODE){
    evaluateTemperatureChange();
  }
}

void startSteppedModeTemperatureControl(){
  // change control mode
  setTemperatureControlMode(STEPPED_MODE);

  // set hour counter to 0
  hoursPassedSinceSteppedControlModeStarted = 0;

  setCurrentTemperatureStep(0);

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

  if (currentTemperatureStep == 0 && hoursPassedSinceSteppedControlModeStarted == 0){
    // it has just started, use first temp
    setFerm1(temperatureSteps[0].temperature);
  } else if (currentTemperatureStep < 9){ // We have only 10 steps, so do not move beyond that point

    if (temperatureSteps[currentTemperatureStep + 1].temperature > 0 && temperatureSteps[currentTemperatureStep + 1].hours > 0 && hoursPassedSinceSteppedControlModeStarted >= temperatureSteps[currentTemperatureStep + 1].starts_at){        
        setFerm1((int) temperatureSteps[currentTemperatureStep + 1].temperature);
        setCurrentTemperatureStep(currentTemperatureStep + 1);
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

byte readCurrentTemperatureStepFromEEPROM(){
  return EEPROM.read(EEPROM_ADDR_CURRENT_TEMPERATURE_STEP);
}

void setCurrentTemperatureStep(byte step){
  currentTemperatureStep = (byte) step;
  EEPROM.put(EEPROM_ADDR_CURRENT_TEMPERATURE_STEP, (byte) step);
  EEPROM.commit();
}

void loadSteppedTemperaturesFromEEPROM(){
  temperatureSteps[0].temperature = EEPROM.read(EEPROM_ADDR_TEMP1_VALUE);
  temperatureSteps[0].hours = EEPROM.read(EEPROM_ADDR_TEMP1_HOURS);
  temperatureSteps[0].starts_at = 0;

  temperatureSteps[1].temperature = EEPROM.read(EEPROM_ADDR_TEMP2_VALUE);
  temperatureSteps[1].hours = EEPROM.read(EEPROM_ADDR_TEMP2_HOURS);
  temperatureSteps[1].starts_at = temperatureSteps[0].starts_at + temperatureSteps[0].hours;

  temperatureSteps[2].temperature = EEPROM.read(EEPROM_ADDR_TEMP3_VALUE);
  temperatureSteps[2].hours = EEPROM.read(EEPROM_ADDR_TEMP3_HOURS);
  temperatureSteps[2].starts_at = temperatureSteps[1].starts_at + temperatureSteps[1].hours;

  temperatureSteps[3].temperature = EEPROM.read(EEPROM_ADDR_TEMP4_VALUE);
  temperatureSteps[3].hours = EEPROM.read(EEPROM_ADDR_TEMP4_HOURS);
  temperatureSteps[3].starts_at = temperatureSteps[2].starts_at + temperatureSteps[2].hours;

  temperatureSteps[4].temperature = EEPROM.read(EEPROM_ADDR_TEMP5_VALUE);
  temperatureSteps[4].hours = EEPROM.read(EEPROM_ADDR_TEMP5_HOURS);
  temperatureSteps[4].starts_at = temperatureSteps[3].starts_at + temperatureSteps[3].hours;

  temperatureSteps[5].temperature = EEPROM.read(EEPROM_ADDR_TEMP6_VALUE);
  temperatureSteps[5].hours = EEPROM.read(EEPROM_ADDR_TEMP6_HOURS);
  temperatureSteps[5].starts_at = temperatureSteps[4].starts_at + temperatureSteps[4].hours;

  temperatureSteps[6].temperature = EEPROM.read(EEPROM_ADDR_TEMP7_VALUE);
  temperatureSteps[6].hours = EEPROM.read(EEPROM_ADDR_TEMP7_HOURS);
  temperatureSteps[6].starts_at = temperatureSteps[5].starts_at + temperatureSteps[5].hours;

  temperatureSteps[7].temperature = EEPROM.read(EEPROM_ADDR_TEMP8_VALUE);
  temperatureSteps[7].hours = EEPROM.read(EEPROM_ADDR_TEMP8_HOURS);
  temperatureSteps[7].starts_at = temperatureSteps[6].starts_at + temperatureSteps[6].hours;

  temperatureSteps[8].temperature = EEPROM.read(EEPROM_ADDR_TEMP9_VALUE);
  temperatureSteps[8].hours = EEPROM.read(EEPROM_ADDR_TEMP9_HOURS);
  temperatureSteps[8].starts_at = temperatureSteps[7].starts_at + temperatureSteps[7].hours;

  temperatureSteps[9].temperature = EEPROM.read(EEPROM_ADDR_TEMP10_VALUE);
  temperatureSteps[9].hours = EEPROM.read(EEPROM_ADDR_TEMP10_HOURS);
  temperatureSteps[9].starts_at = temperatureSteps[8].starts_at + temperatureSteps[8].hours;
  debugTemperatureSteps();
}

