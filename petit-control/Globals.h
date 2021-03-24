#ifndef H_GLOBALS
    #define H_GLOBALS

    #include <Arduino.h>
    #include <tempo.h>
    #include <ESP8266WiFi.h>
    #include <ESP8266HTTPClient.h>
    #include <ESP8266WebServer.h>
    #include <OneWire.h>
    #include <DallasTemperature.h>

    #define DEBUG_ON // Comment this to disable debugging

    
    //
    // Direcciones de la EEPROM
    //
    #define ADDR1 0 // Direccion de la EEPROM para la tempSet
    #define EEPROM_ADDR_ACTIVE_MODE 1

    // Temperature control modes 
    #define SIMPLE_MODE 1
    #define STEPPED_MODE 2

    // Steped control mode defines
    #define EEPROM_ADDR_DAYS_PASSED 2 // Dias transcurridos desde que se activo el modo Temp escalonadas
    #define EEPROM_ADDR_HOURS_PASSED 3 // Horas transcurridas desde que se activo el modo Temp escalonadas

    #define EEPROM_ADDR_TEMP1_VALUE 4
    #define EEPROM_ADDR_TEMP1_HOURS 5
    #define EEPROM_ADDR_TEMP1_ACCUMULATED_HOURS 6

    #define EEPROM_ADDR_TEMP2_VALUE 7
    #define EEPROM_ADDR_TEMP2_HOURS 8
    #define EEPROM_ADDR_TEMP2_ACCUMULATED_HOURS 9

    #define EEPROM_ADDR_TEMP3_VALUE 10
    #define EEPROM_ADDR_TEMP3_HOURS 11
    #define EEPROM_ADDR_TEMP3_ACCUMULATED_HOURS 12

    #define EEPROM_ADDR_TEMP4_VALUE 13
    #define EEPROM_ADDR_TEMP4_HOURS 14
    #define EEPROM_ADDR_TEMP4_ACCUMULATED_HOURS 15

    #define EEPROM_ADDR_TEMP5_VALUE 16
    #define EEPROM_ADDR_TEMP5_HOURS 17
    #define EEPROM_ADDR_TEMP5_ACCUMULATED_HOURS 18

    #define EEPROM_ADDR_TEMP6_VALUE 19
    #define EEPROM_ADDR_TEMP6_HOURS 20
    #define EEPROM_ADDR_TEMP6_ACCUMULATED_HOURS 21

    #define EEPROM_ADDR_TEMP7_VALUE 22
    #define EEPROM_ADDR_TEMP7_HOURS 23
    #define EEPROM_ADDR_TEMP7_ACCUMULATED_HOURS 24

    #define EEPROM_ADDR_TEMP8_VALUE 25
    #define EEPROM_ADDR_TEMP8_HOURS 26
    #define EEPROM_ADDR_TEMP8_ACCUMULATED_HOURS 27

    #define EEPROM_ADDR_TEMP9_VALUE 28
    #define EEPROM_ADDR_TEMP9_HOURS 29
    #define EEPROM_ADDR_TEMP9_ACCUMULATED_HOURS 30

    #define EEPROM_ADDR_TEMP10_VALUE 31
    #define EEPROM_ADDR_TEMP10_HOURS 32
    #define EEPROM_ADDR_TEMP10_ACCUMULATED_HOURS 33

    #define RELAY1 5 

    #define ONE_WIRE_BUS 14
    #define TEMPERATURE_PRECISION 9
    #define HISTERESIS 0.3

    extern const char* ssid;
    extern const char* password;

    extern byte tempset1;
    extern byte relay1; // Extern means declared as global variable

    extern float tempsensada1;

    extern byte temperatureControlMode;

    extern int hoursPassedSinceSteppedControlModeStarted;

    extern OneWire oneWire;
    extern DallasTemperature sensors;

    extern Tempo t_temp; // temporizador para la lectura de temperatura
    extern Tempo tempo_wifi_retry; // resilient connection retry to WIFI each minute
    extern Tempo tempo_step_change; 
    extern ESP8266WebServer server;
#endif