#include <WiFiStatus.h>

WiFiStatus::WiFiStatus(AsyncWebServer *server) : _server(server) {
  _server->on(WIFI_STATUS_SERVICE_PATH, HTTP_GET, std::bind(&WiFiStatus::wifiStatus, this, std::placeholders::_1));

  WiFi.onEvent(onStationModeConnected, WiFiEvent_t::SYSTEM_EVENT_STA_CONNECTED); 
  WiFi.onEvent(onStationModeDisconnected, WiFiEvent_t::SYSTEM_EVENT_STA_DISCONNECTED); 
  WiFi.onEvent(onStationModeGotIP, WiFiEvent_t::SYSTEM_EVENT_STA_GOT_IP); 
}

void WiFiStatus::onStationModeConnected(WiFiEvent_t event, WiFiEventInfo_t info) {
  Serial.print("WiFi Connected.");
}

void WiFiStatus::onStationModeDisconnected(WiFiEvent_t event, WiFiEventInfo_t info) {
  Serial.print("WiFi Disconnected. Reason code=");
  Serial.println(info.disconnected.reason);
}

void WiFiStatus::onStationModeGotIP(WiFiEvent_t event, WiFiEventInfo_t info) {
  Serial.print("WiFi Got IP. localIP=");
  Serial.print(WiFi.localIP().toString());
  Serial.print(", hostName=");
  Serial.println(WiFi.getHostname());
}

void WiFiStatus::wifiStatus(AsyncWebServerRequest *request) {
  AsyncJsonResponse * response = new AsyncJsonResponse();
  JsonObject& root = response->getRoot();
  wl_status_t status = WiFi.status();
  root["status"] = (uint8_t) status;
  if (status == WL_CONNECTED){
    root["local_ip"] = WiFi.localIP().toString();
    root["rssi"] = WiFi.RSSI();
    root["ssid"] = WiFi.SSID();
    root["bssid"] = WiFi.BSSIDstr();
    root["channel"] = WiFi.channel();
    root["subnet_mask"] = WiFi.subnetMask().toString();
    root["gateway_ip"] = WiFi.gatewayIP().toString();
    IPAddress dnsIP1 = WiFi.dnsIP(0);
    IPAddress dnsIP2 = WiFi.dnsIP(1);
    if (dnsIP1 != 0U){
      root["dns_ip_1"] = dnsIP1.toString();
    }
    if (dnsIP2 != 0U){
      root["dns_ip_2"] = dnsIP2.toString();
    }
  }
  response->setLength();
  request->send(response);
}
