#ifndef H_GLOBALS
    #define H_GLOBALS

    #include <Arduino.h>
    #include <tempo.h>
    #include <ESP8266WiFi.h>
    #include <ESP8266HTTPClient.h>
    #include <ESP8266WebServer.h>
    #include <OneWire.h>
    #include <DallasTemperature.h>

    
    //
    // Direcciones de la EEPROM
    //
    #define ADDR1 0 // Direccion de la EEPROM para la tempSet
    #define ADDR_MODE_ACTIVE 1 // 1: modo normal; 2:modo temperaturas escalonadas
    #define ADDR_DAYS_PASSED 2 // Dias transcurridos desde que se activo el modo Temp escalonadas
    #define ADDR_HOURS_PASSED 3 // Horas transcurridf the insert function. Update your command to include the file which contains the definition of that functias desde que se activo el modo Temp escalonadas

    #define ADDR_TEMP1_VALUE 4
    #define ADDR_TEMP1_HOURS 5
    #define ADDR_TEMP2_VALUE 6
    #define ADDR_TEMP2_HOURS 7
    #define ADDR_TEMP3_VALUE 8
    #define ADDR_TEMP3_HOURS 9

    #define RELAY1 5 

    #define ONE_WIRE_BUS 14
    #define TEMPERATURE_PRECISION 9
    #define HISTERESIS 0.3

    #ifndef STASSID
    #define STASSID "THESSID"
    #define STAPSK  "THEPASSWORD"
    #endif

    extern const char* ssid;
    extern const char* password;

    extern byte tempset1;
    extern byte relay1; // Extern means declared as global variable

    extern float tempsensada1;

    extern OneWire oneWire;
    extern DallasTemperature sensors;

    extern Tempo t_temp; // temporizador para la lectura de temperatura
    extern Tempo tempo_wifi_retry; // resilient connection retry to WIFI each minute
    extern ESP8266WebServer server;
#endif