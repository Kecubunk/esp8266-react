#ifndef WiFiStatus_h
#define WiFiStatus_h

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <AsyncJson.h>
#include <IPAddress.h>

#define WIFI_STATUS_SERVICE_PATH "/rest/wifiStatus"

class WiFiStatus {

  public:

    WiFiStatus(AsyncWebServer *server);

  private:

    AsyncWebServer* _server;

    // static functions for logging WiFi events to the UART
    static void onStationModeConnected(WiFiEvent_t event, WiFiEventInfo_t info);
    static void onStationModeDisconnected(WiFiEvent_t event, WiFiEventInfo_t info);
    static void onStationModeGotIP(WiFiEvent_t event, WiFiEventInfo_t info);

    void wifiStatus(AsyncWebServerRequest *request);

};

#endif // end WiFiStatus_h
