#ifndef H_TEMPERATURECONTROLFUNCTIONS
  #define H_TEMPERATURECONTROLFUNCTIONS
  #include <EEPROM.h>
  #include "Globals.h"
  #include "ApiFunctions.h"
  #include "debug.h"

  struct TempStep {
    byte temperature;
    byte hours;
    int starts_at; // calculated accumulated hours since control starts for this tempstep should start
  };

  extern TempStep temperatureSteps[10];

  extern uint8_t sensor_fridge_address[8];
  extern uint8_t sensor_ambience_address[8];

  void setferm1(int new_temp);
  void getTemps();
  void control();
  void setTemperatureControlMode(int mode);
  void startSimpleModeTemperatureControl(int new_temperature);
  void saveSteppedModeTemperatureControl(TempStep submittedTempSteps[10]);
  void startSteppedModeTemperatureControl();
  void updateElapsedHours();
  void evaluateTemperatureChange();
  int readElapsedHoursFromEEPROM();
  void loadSteppedTemperaturesFromEEPROM();
  byte readCurrentTemperatureStepFromEEPROM();
  void setCurrentTemperatureStep(byte step);
#endif