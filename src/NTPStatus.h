#ifndef NTPStatus_h
#define NTPStatus_h

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <AsyncJson.h>
#include <TimeLib.h>
#include <NtpClientLib.h>

#define NTP_STATUS_SERVICE_PATH "/rest/ntpStatus"

class NTPStatus {

  public:

    NTPStatus(AsyncWebServer *server);

  private:

    AsyncWebServer* _server;

    void ntpStatus(AsyncWebServerRequest *request);

};

#endif // end NTPStatus_h
