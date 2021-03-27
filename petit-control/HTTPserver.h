#ifndef H_HTTPSERVER
#define H_HTTPSERVER
    #include <Arduino.h>

    void initHTTPserver();
    void redirectHome();
    // void handleSubmit();
    void handleNotFound();
    void returnFail(String msg);
    void handleRoot();
    void handleSetSimpleModeSubmit();
    void handleSteppedModeSubmit();
    void handleInitSteppedMode();
    

#endif