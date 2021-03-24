#ifndef H_HTTPSERVER
#define H_HTTPSERVER
    #include <Arduino.h>

    void initHTTPserver();
    void redirectHome();
    void handleSubmit();
    void returnOK();
    void handleNotFound();
    void returnFail(String msg);
    void handleRoot();
    void handleSetSimpleModeSubmit();
    void handleSteppedModeSubmit();
    

#endif