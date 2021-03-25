#ifndef H_TEMPERATURECONTROLFUNCTIONS
  #define H_TEMPERATURECONTROLFUNCTIONS
  #include <EEPROM.h>
  #include "Globals.h"
  #include "ApiFunctions.h"

  struct TempStep {
    byte temperature;
    byte hours;
    int starts_at; // calculated accumulated hours since control starts for this tempstep should start
  };

  extern TempStep * temperatureSteps[10];

  void setferm1(int new_temp);
  void getTemps();
  void control();
  void setTemperatureControlMode(int mode);
  void startSimpleModeTemperatureControl(int new_temperature);
  void startSteppedModeTemperatureControl(TempStep submittedTempSteps[10]);
  void updateElapsedHours();
  void evaluateTemperatureChange();
  int readElapsedHoursFromEEPROM();
#endif